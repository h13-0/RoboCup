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

void RoboCup::FrameBufferMonitor::getFrameBufferInfo(const std::string& TargetFrameBufferFile, uint32_t& BitsPerPixel, int& X_ResolutionVirtual, int& Y_ResolutionVirtual)
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
        throw std::runtime_error("Could not open FrameBuffer file: " + TargetFrameBufferFile);
	    //throw ParameterInvalid("Could not open FrameBuffer file: " + TargetFrameBufferFile);
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

void RoboCup::FrameBufferMonitor::Display(cv::InputArray InputImage)
{
    using namespace cv;

    cv::Mat frame;
    if (InputImage.channels() == 1)
    {
        cvtColor(InputImage, frame, COLOR_GRAY2BGR);
    }
    else if (InputImage.channels() == 3) {
        frame = InputImage.getMat().clone();
    }
    else {
	    throw std::runtime_error("Only 3-channel and 1-channel images are supported.");
        //throw ParameterInvalid("Only 3-channel and 1-channel images are supported.");
    }

    float scalingRatio = fmin(float(xResolution) / frame.cols, float(yResolution) / frame.rows);

    int rows = frame.rows * scalingRatio;
    int cols = frame.cols * scalingRatio;
    resize(frame, frame, Size(cols, rows));

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
}

#endif
