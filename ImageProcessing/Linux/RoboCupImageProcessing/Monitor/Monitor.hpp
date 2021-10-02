#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

namespace RoboCup
{
	class Monitor
	{
	public:
		virtual void Display(const std::string& Tittle, cv::InputArray InputImage) = 0;
	};
}