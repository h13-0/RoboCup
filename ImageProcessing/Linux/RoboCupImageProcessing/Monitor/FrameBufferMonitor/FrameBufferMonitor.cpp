#include "FrameBufferMonitor.hpp"
#if defined(__linux__)
#include <stdexcept>
#include <fstream>
#include <stdint.h>
#include <sys/ioctl.h>

#include <linux/fb.h>
#include <fcntl.h>

void h13::FrameBufferMonitor::getFrameBufferInfo(const std::string& TargetFrameBufferFile, uint32_t& BitsPerPixel, uint16_t& X_ResolutionVirtual, uint16_t& Y_ResolutionVirtual)
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
    }
    else {
        throw std::runtime_error("Could not find FrameBuffer file: " + TargetFrameBufferFile);
    }
}

h13::FrameBufferMonitor::FrameBufferMonitor(const std::string& TargetFrameBufferFile)
{
    FrameBufferMonitor::getFrameBufferInfo(TargetFrameBufferFile, bitsPerPixel, xResolution, yResolution);
    frameBufferFileStream = std::ofstream(TargetFrameBufferFile.c_str());
}

h13::FrameBufferMonitor::~FrameBufferMonitor()
{
}

void h13::FrameBufferMonitor::Display(const std::string& Tittle, cv::InputArray InputImage)
{
    using namespace cv;
    if (Tittle == "Final")
    {
        CV_Assert((InputImage.channels() == 1) || (InputImage.channels() == 3));
        Mat frameBuffer;
        float scalingRatio = min(xResolution / InputImage.rows(), yResolution / InputImage.cols());
        resize(InputImage, frameBuffer, Size(InputImage.rows() * scalingRatio, InputImage.cols() * scalingRatio));
        int rols = frameBuffer.rows;
        int cols = frameBuffer.cols;

        std::vector<Mat> splitedBGR;
        switch (bitsPerPixel)
        {
        case 16:
            cvtColor(frameBuffer, frameBuffer, COLOR_BGR2BGR565);
            for (int y = 0; y < cols; y++)
            {
                frameBufferFileStream.seekp(y * xResolution * 2);
                frameBufferFileStream.write(reinterpret_cast<char*>(frameBuffer.ptr(y)), rols * 2);
            }
            break;

        case 32:
            split(frameBuffer, splitedBGR);
            splitedBGR.push_back(cv::Mat(frameBuffer.size(), CV_8UC1, Scalar(255)));
            merge(splitedBGR, frameBuffer);
            for (int y = 0; y < cols; y++)
            {
                frameBufferFileStream.seekp(y * xResolution * 4);
                frameBufferFileStream.write(reinterpret_cast<char*>(frameBuffer.ptr(y)), rols * 4);
            }
            break;

        default:
            break;
        }
    }
}
#endif
