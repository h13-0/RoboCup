#include "FrameBufferMonitor.hpp"
#if defined(__linux__)
#include <stdexcept>
#include <chrono>
#include <thread>
#include <fstream>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <unistd.h>
#include "ParameterInvalid.hpp"

void RoboCup::FrameBufferMonitor::getFrameBufferInfo(const std::string& TargetFrameBufferFile, uint32_t& BitsPerPixel, uint16_t& X_ResolutionVirtual, uint16_t& Y_ResolutionVirtual)
{
    struct fb_var_screeninfo screenInfo;
    int fd = -1;
    fd = open(TargetFrameBufferFile.c_str(), O_RDWR);
    if (fd > 0)
    {
        if (!ioctl(fd, FBIOGET_VSCREENINFO, &screenInfo))
        {
            X_ResolutionVirtual = screenInfo.xres_virtual;
            Y_ResolutionVirtual = screenInfo.yres_virtual;
            BitsPerPixel = screenInfo.bits_per_pixel;
        }
        close(fd);
    }
    else {
        throw ParameterInvalid("Could not open FrameBuffer file: " + TargetFrameBufferFile);
    }
}

RoboCup::FrameBufferMonitor::FrameBufferMonitor(const std::string& TargetFrameBufferFile)
{
    FrameBufferMonitor::getFrameBufferInfo(TargetFrameBufferFile, bitsPerPixel, xResolution, yResolution);
    frameBufferFileStream = std::ofstream(TargetFrameBufferFile.c_str());
}

RoboCup::FrameBufferMonitor::~FrameBufferMonitor()
{
}

void RoboCup::FrameBufferMonitor::Display(const std::string& Title, cv::InputArray InputImage)
{
    using namespace cv;
    if (Title == "Final")
    {
        if (InputImage.channels() == 1)
        {
            std::unique_lock<std::mutex> lock(bufferMutex);
            cvtColor(InputImage, buffer, COLOR_GRAY2BGR);
        }
        else if (InputImage.channels() == 3) {
            std::unique_lock<std::mutex> lock(bufferMutex);
            buffer = InputImage.getMat().clone();
        }
        else {
            throw ParameterInvalid("Only 3-channel and 1-channel images are supported.");
        }

        Refresh(1);
    }
}

void RoboCup::FrameBufferMonitor::Refresh(int MillisecondsToDelay)
{
    using namespace cv;

    Mat frame;
    {
        std::unique_lock<std::mutex> lock(bufferMutex);
        frame = buffer.clone();
    }

    float scalingRatio = fmin(float(xResolution) / frame.cols, float(yResolution) / frame.rows);
    resize(frame, frame, Size(frame.cols * scalingRatio, frame.rows * scalingRatio));
    int rows = frame.rows;
    int cols = frame.cols;

    std::vector<Mat> splitedBGR;
    switch (bitsPerPixel)
    {
    case 16:
        cvtColor(frame, frame, COLOR_BGR2BGR565);
        for (int y = 0; y < rows; y++)
        {
            frameBufferFileStream.seekp(y * xResolution * 2);
            frameBufferFileStream.write(reinterpret_cast<char*>(frame.ptr(y)), cols * 2);
        }
        break;

    case 32:
        split(frame, splitedBGR);
        splitedBGR.push_back(cv::Mat(frame.size(), CV_8UC1, Scalar(255)));
        merge(splitedBGR, frame);
        for (int y = 0; y < rows; y++)
        {
            frameBufferFileStream.seekp(y * xResolution * 4);
            frameBufferFileStream.write(reinterpret_cast<char*>(frame.ptr(y)), cols * 4);
        }
        break;

    default:
        break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(MillisecondsToDelay));
}

#endif
