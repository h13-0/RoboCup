#pragma once
#if defined(__linux__)
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

#include "Monitor.hpp"

namespace RoboCup
{
	class FrameBufferMonitor : public Monitor
	{
	public:
		FrameBufferMonitor(const std::string &TargetFrameBufferFile);
		~FrameBufferMonitor();

		void Display(const std::string& Title, cv::InputArray InputImage);
		void GetFrameBufferResolution(uint16_t &X_Resolution, uint16_t &Y_Resolution)
		{
			X_Resolution = xResolution;
			Y_Resolution = yResolution;
		};

		void Refresh(int MillisecondsToDelay);
		
	private:
		static void getFrameBufferInfo(const std::string& TargetFrameBufferFile, uint32_t& BitsPerPixel, uint16_t& X_ResolutionVirtual, uint16_t& Y_ResolutionVirtual);

		std::mutex bufferMutex;
		cv::Mat buffer;

		uint16_t xResolution;
		uint16_t yResolution;
		uint32_t bitsPerPixel;
		std::ofstream frameBufferFileStream;		
	};
}
#endif