#include "FrameBuffer.hpp"
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

#include "FrameBufferMonitor.hpp"

#include <iostream>

extern "C"
{
	void* Init(char *TargetFrameBufferFile)
	{
		return new RoboCup::FrameBufferMonitor(TargetFrameBufferFile);
	}

	void Display(void* FrameBufferHandle, unsigned char* Image, int Rows, int Cols)
	{
		((RoboCup::FrameBufferMonitor*)FrameBufferHandle)->Display(cv::Mat(Rows, Cols, CV_8UC3, Image));
	}

	int GetFrameBufferX_Resolution(void* FrameBufferHandle)
	{
		int xRes = 0, yRes = 0;
		((RoboCup::FrameBufferMonitor*)FrameBufferHandle)->GetFrameBufferResolution(xRes, yRes);
		return xRes;
	}

    int GetFrameBufferY_Resolution(void* FrameBufferHandle)
    {
        int xRes = 0, yRes = 0;
        ((RoboCup::FrameBufferMonitor*)FrameBufferHandle)->GetFrameBufferResolution(xRes, yRes);
        return yRes;
    }

	void DeInit(void* FrameBufferHandle)
	{
		((RoboCup::FrameBufferMonitor*)FrameBufferHandle)->~FrameBufferMonitor();
	}
}
