#pragma once
#if defined(__linux__)
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

namespace RoboCup
{
	class FrameBufferMonitor
	{
	public:
		FrameBufferMonitor(const std::string &TargetFrameBufferFile);
		~FrameBufferMonitor();

		void Display(cv::InputArray InputImage);
		void GetFrameBufferResolution(int &X_Resolution, int &Y_Resolution)
		{
			X_Resolution = xResolution;
			Y_Resolution = yResolution;
		};

	private:
		static void getFrameBufferInfo(const std::string& TargetFrameBufferFile, uint32_t& BitsPerPixel, int& X_ResolutionVirtual, int& Y_ResolutionVirtual);

		int xResolution;
		int yResolution;
		uint32_t bitsPerPixel;
		std::ofstream frameBufferFileStream;		
	};
}
#endif
