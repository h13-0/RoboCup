#include "MainWorkingFlow.hpp"

#include <chrono>
#include <string>

#define GLOG_NO_ABBREVIATED_SEVERITIES 
#include <glog/logging.h>

#include "FruitDetector.hpp"
#include "Monitor.hpp"
#include "FrameBufferMonitor.hpp"
#include "OpenCV_Monitor.hpp"
#include "SerialProtocol.hpp"

RoboCup::MainWorkingFlow::MainWorkingFlow(const Configs& Configs)
    : WorkingFlow(Configs), fruitDetector(FruitDetector(Configs.GetFruitDetectorSettings()))
    , mode(WorkingMode::StandBy), appleDetector(AppleDetector(Configs.GetAppleDetectorSettings()))
    , targetDetector(TargetDetectorConfigs(Configs.GetTargetDetectorSettings()))
    , fruitDetectionFinished(0), exit(false), exited(false)
{
    using namespace cv;
    using namespace h13;
    ApplicationConfigs appConfigs(Configs.GetApplicationConfigs());

#if defined(__linux__)
    switch (appConfigs.GetMonitorType())
    {
    case MonitorType::FrameBuffer:
        monitor = new FrameBufferMonitor(appConfigs.GetFrameBufferPath());
        break;

    case MonitorType::OpenCV:
        monitor = new OpenCV_Monitor();
        break;

    default:
        monitor = nullptr;
        break;
    }
#else
    monitor = new OpenCV_Monitor();
#endif

    switch (appConfigs.GetVideoDeviceType())
    {
    case VideoDeviceType::Camera:
#if defined(__linux__)
        capture.open(appConfigs.GetCameraID(), cv::CAP_V4L2);
#else
        capture.open(appConfigs.GetCameraID());
#endif
        break;

    case VideoDeviceType::VideoStream:
#if defined(__linux__)
        capture.open(appConfigs.GetVideoStreamPath(), cv::CAP_V4L2);
#else
        capture.open(appConfigs.GetVideoStreamPath());
#endif
        break;

    default:
        capture.open(0, cv::CAP_V4L2);
        break;
    }

    protocol = new SerialProtocol(appConfigs.GetSerialPort(), 115200);
    
    //Add mode switch callback.
    protocol -> AddKeyCallback("CMD",
        [&](void* DataPtr, int Length)
        {
            std::string cmd((char *)DataPtr, 0, Length);
            std::unique_lock<std::mutex> lock(modeMutex);
            if (!cmd.compare("AppleDetectMax"))
            {
                mode = WorkingMode::AppleDetectMax;
                LOG(INFO) << "Switch to mode: AppleDetectMax";
            }
            else if (!cmd.compare("AppleDetectLeft"))
            {
                mode = WorkingMode::AppleDetectLeft;
                LOG(INFO) << "Switch to mode: AppleDetectLeft";
            }
            else if (!cmd.compare("AppleDetectRight"))
            {
                mode = WorkingMode::AppleDetectRight;
                LOG(INFO) << "Switch to mode: AppleDetectRight";
            }
            else if (!cmd.compare("TargetDetect"))
            {
                mode = WorkingMode::TargetDetection;
                LOG(INFO) << "Switch to mode: TargetDetection";
            }
            else if (!cmd.compare("FruitDetect"))
            {
                if (mode != WorkingMode::FruitDetection)
                {
                    std::unique_lock<std::mutex> lockFlag(fruitDetectionFinishedMutex);
                    fruitDetectionFinished = 0;
                }
                mode = WorkingMode::FruitDetection;
                LOG(INFO) << "Switch to mode: FruitDetection";
            }
        }
    );
}

RoboCup::MainWorkingFlow::~MainWorkingFlow() noexcept
{
    std::unique_lock<std::mutex> lock(exitFlagMutex);
    exit = true;
    exitedConditionVariable.wait(lock, [this] { return exited; });
    delete protocol;
    delete monitor;
}

int RoboCup::MainWorkingFlow::Run()
{
    using namespace cv;
    using namespace std;
    using namespace RoboCup;

    LOG(INFO) << "Woring in the MainWorkingFlow";

    exit = false;
    exited = false;

    std::thread readFrameFromDeviceThread(
        [&] {
            while (1)
            {
                if (capture.isOpened())
                {
                    Mat _frame;
                    while (capture.read(_frame))
                    {
                        unique_lock lock(frameMutex);
                        frame = _frame.clone();
                    }
                }
                LOG(FATAL) << "Could not open camera.";
                throw std::runtime_error("Could not open camera.");
            }
        }
    );

    readFrameFromDeviceThread.detach();

    while (1)
    {
        //Check exit flag.
        {
            std::unique_lock<std::mutex> lock(exitFlagMutex);
            if (this->exit)
            {
                break;
            }
        }

        //Read frame.
        Mat _frame;
        while(_frame.empty())
        {
            unique_lock lock(frameMutex);
            _frame = frame.clone();
            this_thread::sleep_for(chrono::milliseconds(100));
        }


        auto start = std::chrono::high_resolution_clock::now();
        //Dispatch.
        WorkingMode::WorkingMode _mode;
        {
            std::unique_lock<std::mutex> lock(modeMutex);
            _mode = mode;
        }
        switch (_mode)
        {
        case RoboCup::WorkingMode::WorkingMode::StandBy:
            standBy(_frame);
            break;
        case RoboCup::WorkingMode::WorkingMode::AppleDetectMax:
            appleDetectMax(_frame);
            break;
        case RoboCup::WorkingMode::WorkingMode::AppleDetectLeft:
            appleDetectLeft(_frame);
            break;
        case RoboCup::WorkingMode::WorkingMode::AppleDetectRight:
            appleDetectRight(_frame);
            break;
        case RoboCup::WorkingMode::WorkingMode::TargetDetection:
            targetDetect(_frame);
            break;
        case RoboCup::WorkingMode::WorkingMode::FruitDetection:
            fruitDetection(_frame);
            break;
        default:
            break;
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> tm = end - start;
        if (_mode != WorkingMode::StandBy)
        {
            putText(_frame, "fps: " + to_string(1000 / tm.count()), Point(0, 24), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));
        }
        putText(_frame, "Mode: " + RoboCup::WorkingMode::GetLabelString(_mode), Point(0, _frame.cols - 24), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));
        if (monitor)
        {
            monitor->Display("Final", _frame);
        }
        reportWorkingMode(_mode);
    }
    exited = true;
    exitedConditionVariable.notify_one();
    return -1;
}

void RoboCup::MainWorkingFlow::standBy(cv::InputOutputArray InputOutputBGR_Image)
{
    //Nothing to do.
}

void RoboCup::MainWorkingFlow::appleDetectMax(cv::InputOutputArray InputOutputBGR_Image)
{
    using namespace cv;

    auto results = appleDetector.Detect(InputOutputBGR_Image);
    
    //Sort apple size.
    sortRotatedRectMaxToMin(results);
    
    for (int index = 0; index < results.size(); index++)
    {
        RotatedRect& result = results[index];
        Point2f point[4];
        result.points(point);
        Rect rect = result.boundingRect();
        
        if (index == 0)
        {
            rectangle(InputOutputBGR_Image, rect, Scalar(0, 255, 0), 2);
            putText(InputOutputBGR_Image, "Max Apple", Point(rect.x, rect.y - 16), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
            for (int index = 0; index < 4; index++)
            {
                line(InputOutputBGR_Image, point[index], point[(index + 1) % 4], cv::Scalar(255, 0, 0), 2);
            }
            sendAppleCoordinates(result.center.x / InputOutputBGR_Image.rows(), result.center.y / InputOutputBGR_Image.cols());
        }
        else {
            rectangle(InputOutputBGR_Image, rect, Scalar(0, 0, 255), 2);
        }
    }
}

void RoboCup::MainWorkingFlow::appleDetectLeft(cv::InputOutputArray InputOutputBGR_Image)
{
    using namespace cv;
    auto results = appleDetector.Detect(InputOutputBGR_Image);

    //Sort Apple Left to Right.
    sortRotatedRectLeftToRight(results);

    for(int index = 0; index < results.size(); index++)
    {
        RotatedRect& result = results[index];
        Point2f point[4];
        result.points(point);
        Rect rect = result.boundingRect();

        if (index == 0)
        {
            rectangle(InputOutputBGR_Image, rect, Scalar(0, 255, 0), 2);
            putText(InputOutputBGR_Image, "Left Apple", Point(rect.x, rect.y - 16), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
            for (int index = 0; index < 4; index++)
            {
                line(InputOutputBGR_Image, point[index], point[(index + 1) % 4], cv::Scalar(255, 0, 0), 2);
            }
            sendAppleCoordinates(result.center.x / InputOutputBGR_Image.rows(), result.center.y / InputOutputBGR_Image.cols());
        }
        else {
            rectangle(InputOutputBGR_Image, rect, Scalar(0, 0, 255), 2);
        }
    }
}

void RoboCup::MainWorkingFlow::appleDetectRight(cv::InputOutputArray InputOutputBGR_Image)
{
    using namespace cv;
    auto results = appleDetector.Detect(InputOutputBGR_Image);
    
    //Sort Apple Right to left.
    sortRotatedRectRightToLeft(results);

    for (int index = 0; index < results.size(); index++)
    {
        RotatedRect& result = results[index];
        Point2f point[4];
        result.points(point);
        Rect rect = result.boundingRect();

        if (index == 0)
        {
            rectangle(InputOutputBGR_Image, rect, Scalar(0, 255, 0), 2);
            putText(InputOutputBGR_Image, "Right Apple", Point(rect.x, rect.y - 16), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
            for (int index = 0; index < 4; index++)
            {
                line(InputOutputBGR_Image, point[index], point[(index + 1) % 4], cv::Scalar(255, 0, 0), 2);
            }
            sendAppleCoordinates(result.center.x / InputOutputBGR_Image.rows(), result.center.y / InputOutputBGR_Image.cols());
        }
        else {
            rectangle(InputOutputBGR_Image, rect, Scalar(0, 0, 255), 2);
        }
    }
}

void RoboCup::MainWorkingFlow::targetDetect(cv::InputOutputArray InputOutputBGR_Image)
{
    using namespace std;
    using namespace cv;

    auto targets = targetDetector.Detect(InputOutputBGR_Image);

    //Find max target.
    sortRotatedRectMaxToMin(targets);

    if (targets.size() > 0)
    {
        RotatedRect& target = targets[0];
        Point2f point[4];
        target.points(point);
        Rect rect = target.boundingRect();
        rectangle(InputOutputBGR_Image, rect, Scalar(0, 0, 255), 2);
        putText(InputOutputBGR_Image, "Target", Point(rect.x, rect.y - 16), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
        for (int index = 0; index < 4; index++)
        {
            line(InputOutputBGR_Image, point[index], point[(index + 1) % 4], cv::Scalar(255, 0, 0), 2);
        }
        sendTargetCoordinates(target.center.x / InputOutputBGR_Image.rows(), target.center.y / InputOutputBGR_Image.cols());
    }
}

void RoboCup::MainWorkingFlow::fruitDetection(cv::InputOutputArray InputOutputBGR_Image)
{
    using namespace std;
    using namespace cv;

    Mat mask;
    auto results = fruitDetector.Detect(InputOutputBGR_Image);
    for (auto& result : results)
    {
        Point2f point[4];
        result.Rect.points(point);
        Rect rect = result.Rect.boundingRect();
        rectangle(InputOutputBGR_Image, rect, Scalar(0, 255, 0), 2);
        putText(InputOutputBGR_Image, Fruit_t::GetLabelString(result.FruitType), Point(rect.x, rect.y - 16), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
        for (int index = 0; index < 4; index++)
        {
            line(InputOutputBGR_Image, point[index], point[(index + 1) % 4], cv::Scalar(255, 0, 0), 2);
        }
    }

    int _fruitDetectionFinished = 1;
    {
        std::unique_lock<std::mutex> lockFlag(fruitDetectionFinishedMutex);
        _fruitDetectionFinished = fruitDetectionFinished;
    }
    
    if (!_fruitDetectionFinished)
    {
        sendFruitDetectResult(results);
    }
}

void RoboCup::MainWorkingFlow::reportWorkingMode(const WorkingMode::WorkingMode& Mode)
{
    protocol->SendPacket("WM", uint8_t(Mode));
}

void RoboCup::MainWorkingFlow::sendAppleCoordinates(const float& X_Coordinates, const float& Y_Coordinates)
{
    protocol->SendPacket("AppCenX", X_Coordinates);
    protocol->SendPacket("AppCenY", Y_Coordinates);
}

void RoboCup::MainWorkingFlow::sendTargetCoordinates(const float& X_Coordinates, const float& Y_Coordinates)
{
    protocol->SendPacket("TarCenX", X_Coordinates);
    protocol->SendPacket("TarCenY", Y_Coordinates);
}

void RoboCup::MainWorkingFlow::sendFruitDetectResult(const std::vector<RoboCup::FruitDetectResult_t>& Results)
{
    //Unpack Results.
    int8_t appleNumber = 0;
    int8_t bananaNumber = 0;
    int8_t kiwiFruitNumber = 0;
    int8_t lemonNumber = 0;
    int8_t orangeNumber = 0;
    int8_t peachNumber = 0;
    int8_t pearNumber = 0;
    int8_t pitayaNumber = 0;
    int8_t snowPearNumber = 0;

    {
        std::unique_lock<std::mutex> lockFlag(fruitDetectionFinishedMutex);
        fruitDetectionFinished = 1;
    }

    for (auto& result : Results)
    {
        switch (result.FruitType)
        {
        case Fruit_t::Apple:
            appleNumber++;
            break;
        case Fruit_t::Banana:
            bananaNumber++;
            break;

        case Fruit_t::KiwiFruit:
            kiwiFruitNumber++;
            break;

        case Fruit_t::Lemon:
            lemonNumber++;
            break;

        case Fruit_t::Orange:
            orangeNumber++;
            break;

        case Fruit_t::Peach:
            peachNumber++;
            break;

        case Fruit_t::Pear:
            pearNumber++;
            break;

        case Fruit_t::Pitaya:
            pitayaNumber++;
            break;

        case Fruit_t::SnowPear:
            snowPearNumber++;
            break;

        default:
            break;
        }
    }
    for (int times = 0; times < 3; times++)
    {
        protocol->SendPacket("AppleNum", appleNumber);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        protocol->SendPacket("BananaNum", bananaNumber);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        protocol->SendPacket("KiwiFruitNum", kiwiFruitNumber);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        protocol->SendPacket("LemonNum", lemonNumber);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        protocol->SendPacket("OrangeNum", orangeNumber);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        protocol->SendPacket("PeachNum", peachNumber);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        protocol->SendPacket("PearNum", pearNumber);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        protocol->SendPacket("PitayaNum", pitayaNumber);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        protocol->SendPacket("SnowPearNum", snowPearNumber);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    for (int times = 0; times < 3; times++)
    {
        protocol->SendPacket("FruitDetectFinished", uint8_t(1));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
