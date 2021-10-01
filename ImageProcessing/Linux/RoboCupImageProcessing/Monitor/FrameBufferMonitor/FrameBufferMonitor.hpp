#pragma once
#if defined(__linux__)
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

namespace h13
{
	class FrameBufferMonitor
	{
	public:
		FrameBufferMonitor(const std::string &TargetFrameBufferFile);
		~FrameBufferMonitor();

		void Display(const std::string& Tittle, cv::InputArray InputImage);
		void GetFrameBufferResolution(uint16_t &X_Resolution, uint16_t &Y_Resolution)
		{
			X_Resolution = xResolution;
			Y_Resolution = yResolution;
		};

	private:
		uint16_t xResolution;
		uint16_t yResolution;
		uint32_t bitsPerPixel;
		std::ofstream frameBufferFileStream;
		static void getFrameBufferInfo(const std::string& TargetFrameBufferFile, uint32_t &BitsPerPixel, uint16_t &X_ResolutionVirtual, uint16_t &Y_ResolutionVirtual);

	};
}
#endif