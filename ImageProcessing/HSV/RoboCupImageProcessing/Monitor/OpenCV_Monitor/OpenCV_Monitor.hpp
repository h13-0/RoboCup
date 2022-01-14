#pragma once
#include <thread>
#include <mutex>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

#include "Monitor.hpp"

namespace RoboCup
{
	class OpenCV_Monitor : public Monitor
	{
	public:
		OpenCV_Monitor() { };
		~OpenCV_Monitor() { };

		void Display(const std::string& Title, cv::InputArray InputImage)
		{
			cv::imshow(Title, InputImage);
			cv::waitKey(1);
		};

	private:
		std::mutex exitMutex;
		bool exit = false;
	};
}
