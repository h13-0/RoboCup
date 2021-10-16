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
    , mode(WorkingMode::FruitDetection), appleDetector(AppleDetector(Configs.GetAppleDetectorSettings()))
    , targetDetector(TargetDetectorConfigs(Configs.GetTargetDetectorSettings()))
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
            else if (!cmd.compare("FruitDetection"))
            {
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

    while (1)
    {
        if (capture.isOpened())
        {
            Mat frame;
            while (capture.read(frame))
            {
                //Check exit flag.
                {
                    std::unique_lock<std::mutex> lock(exitFlagMutex);
                    if (this->exit)
                    {
                        break;
                    }
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
                    standBy(frame);
                    break;
                case RoboCup::WorkingMode::WorkingMode::AppleDetectMax:
                    appleDetectMax(frame);
                    break;
                case RoboCup::WorkingMode::WorkingMode::AppleDetectLeft:
                    appleDetectLeft(frame);
                    break;
                case RoboCup::WorkingMode::WorkingMode::AppleDetectRight:
                    appleDetectRight(frame);
                    break;
                case RoboCup::WorkingMode::WorkingMode::TargetDetection:
                    targetDetect(frame);
                    break;
                case RoboCup::WorkingMode::WorkingMode::FruitDetection:
                    fruitDetection(frame);
                    break;
                default:
                    break;
                }
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> tm = end - start;
                if (_mode != WorkingMode::StandBy)
                {
                    putText(frame, "fps: " + to_string(1000 / tm.count()), Point(0, 24), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));
                }
                putText(frame, "Mode: " + RoboCup::WorkingMode::GetLabelString(_mode), Point(0, frame.cols - 24), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));
                if (monitor)
                {
                    monitor->Display("Final", frame);
                }
                reportWorkingMode(_mode);
            }
        }
        else {
            LOG(FATAL) << "Could not open camera.";
        }

        exited = true;
        exitedConditionVariable.notify_one();
    }
}

void RoboCup::MainWorkingFlow::standBy(cv::InputOutputArray InputOutputBGR_Image)
{

}

void RoboCup::MainWorkingFlow::appleDetectMax(cv::InputOutputArray InputOutputBGR_Image)
{
    using namespace cv;
    auto results = appleDetector.Detect(InputOutputBGR_Image);
    for (auto result : results)
    {
        Point2f point[4];
        //result.points(point);
        result.points(point);
        Rect rect = result.boundingRect();
        rectangle(InputOutputBGR_Image, rect, Scalar(0, 255, 0), 2);
        putText(InputOutputBGR_Image, "Target Apple", Point(rect.x, rect.y - 16), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
        for (int index = 0; index < 4; index++)
        {
            line(InputOutputBGR_Image, point[index], point[(index + 1) % 4], cv::Scalar(255, 0, 0), 2);
        }
    }
}

void RoboCup::MainWorkingFlow::appleDetectLeft(cv::InputOutputArray InputOutputBGR_Image)
{
    using namespace cv;
    auto results = appleDetector.Detect(InputOutputBGR_Image);
    for (auto result : results)
    {
        Point2f point[4];
        //result.points(point);
        result.points(point);
        Rect rect = result.boundingRect();
        rectangle(InputOutputBGR_Image, rect, Scalar(0, 255, 0), 2);
        putText(InputOutputBGR_Image, "Target Apple", Point(rect.x, rect.y - 16), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
        for (int index = 0; index < 4; index++)
        {
            line(InputOutputBGR_Image, point[index], point[(index + 1) % 4], cv::Scalar(255, 0, 0), 2);
        }
    }
}

void RoboCup::MainWorkingFlow::appleDetectRight(cv::InputOutputArray InputOutputBGR_Image)
{
    using namespace cv;
    auto results = appleDetector.Detect(InputOutputBGR_Image);
    for (auto result : results)
    {
        Point2f point[4];
        //result.points(point);
        result.points(point);
        Rect rect = result.boundingRect();
        rectangle(InputOutputBGR_Image, rect, Scalar(0, 255, 0), 2);
        putText(InputOutputBGR_Image, "Target Apple", Point(rect.x, rect.y - 16), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
        for (int index = 0; index < 4; index++)
        {
            line(InputOutputBGR_Image, point[index], point[(index + 1) % 4], cv::Scalar(255, 0, 0), 2);
        }
    }
}

void RoboCup::MainWorkingFlow::targetDetect(cv::InputOutputArray InputOutputBGR_Image)
{
    using namespace std;
    using namespace cv;

    auto targets = targetDetector.Detect(InputOutputBGR_Image);
    for (auto target : targets)
    {
        Point2f point[4];
        target.points(point);
        for (int index = 0; index < 4; index++)
        {
            line(InputOutputBGR_Image, point[index], point[(index + 1) % 4], cv::Scalar(0, 0, 255), 2);
        }
    }
}

void RoboCup::MainWorkingFlow::fruitDetection(cv::InputOutputArray InputOutputBGR_Image)
{
    using namespace std;
    using namespace cv;

    Mat mask;
    auto results = fruitDetector.Detect(InputOutputBGR_Image);
    for (auto result : results)
    {
        Point2f point[4];
        //result.points(point);
        result.Rect.points(point);
        Rect rect = result.Rect.boundingRect();
        rectangle(InputOutputBGR_Image, rect, Scalar(0, 255, 0), 2);
        putText(InputOutputBGR_Image, Fruit_t::GetLabelString(result.FruitType), Point(rect.x, rect.y - 16), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
        for (int index = 0; index < 4; index++)
        {
            line(InputOutputBGR_Image, point[index], point[(index + 1) % 4], cv::Scalar(255, 0, 0), 2);
        }
    }
}

void RoboCup::MainWorkingFlow::reportWorkingMode(const WorkingMode::WorkingMode& Mode)
{
    protocol->SendPacket("WM", int8_t(Mode));
}

void RoboCup::MainWorkingFlow::sendAppleCoordinates(const float& X_Coordinates, const float& Y_Coordinates)
{
    protocol->SendPacket("AppCenX", X_Coordinates);
    protocol->SendPacket("AppCenY", Y_Coordinates);
}

void RoboCup::MainWorkingFlow::sendTargetCoordinates(const float& X_Coordinates, const float& Y_Coordinates)
{
    //protocol->SendPacket("", );
    //protocol->SendPacket("", );
}


