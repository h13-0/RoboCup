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
        monitor = new OpenCV_Monitor();
        break;
    }
#else
    monitor = new OpenCV_Monitor();
#endif

    switch (appConfigs.GetVideoDeviceType())
    {
    case VideoDeviceType::Camera:
        capture.open(appConfigs.GetCameraID(), cv::CAP_V4L2);
        break;

    case VideoDeviceType::VideoStream:
        capture.open(appConfigs.GetVideoStreamPath(), cv::CAP_V4L2);
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
                mode = WorkingMode::TargetDetect;
                LOG(INFO) << "Switch to mode: TargetDetect";
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
                switch (mode)
                {
                case RoboCup::WorkingMode::WorkingMode::StandBy:
                    StandBy(frame);
                    break;
                case RoboCup::WorkingMode::WorkingMode::AppleDetectMax:
                    AppleDetectMax(frame);
                    break;
                case RoboCup::WorkingMode::WorkingMode::AppleDetectLeft:
                    AppleDetectLeft(frame);
                    break;
                case RoboCup::WorkingMode::WorkingMode::AppleDetectRight:
                    AppleDetectRight(frame);
                    break;
                case RoboCup::WorkingMode::WorkingMode::TargetDetect:
                    TargetDetect(frame);
                    break;
                case RoboCup::WorkingMode::WorkingMode::FruitDetection:
                    FruitDetection(frame);
                    break;
                default:
                    break;
                }
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> tm = end - start;
                putText(frame, "fps: " + to_string(1000 / tm.count()), Point(0, 24), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));
                putText(frame, "Mode: " + RoboCup::WorkingMode::GetLabelString(mode), Point(0, frame.cols - 24), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));
                monitor->Display("Final", frame);
                ReportWorkingMode(mode);
            }
        }
        else {
            LOG(FATAL) << "Could not open camera.";
        }

        exited = true;
        exitedConditionVariable.notify_one();
    }
}

void RoboCup::MainWorkingFlow::StandBy(cv::InputOutputArray InputOutputBGR_Image)
{

}

void RoboCup::MainWorkingFlow::AppleDetectMax(cv::InputOutputArray InputOutputBGR_Image)
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

void RoboCup::MainWorkingFlow::AppleDetectLeft(cv::InputOutputArray InputOutputBGR_Image)
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

void RoboCup::MainWorkingFlow::AppleDetectRight(cv::InputOutputArray InputOutputBGR_Image)
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

void RoboCup::MainWorkingFlow::TargetDetect(cv::InputOutputArray InputOutputBGR_Image)
{

}

void RoboCup::MainWorkingFlow::FruitDetection(cv::InputOutputArray InputOutputBGR_Image)
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

void RoboCup::MainWorkingFlow::ReportWorkingMode(const WorkingMode::WorkingMode& Mode)
{
    protocol->SendPacket("WM", int8_t(Mode));
}

void RoboCup::MainWorkingFlow::SendAppleCoordinates(const float& X_Coordinates, const float& Y_Coordinates)
{
    protocol->SendPacket("AppCenX", X_Coordinates);
    protocol->SendPacket("AppCenY", Y_Coordinates);
}

void RoboCup::MainWorkingFlow::SendTargetCoordinates(const float& X_Coordinates, const float& Y_Coordinates)
{
    //protocol->SendPacket("", );
    //protocol->SendPacket("", );
}
