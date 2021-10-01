#pragma once
#include <thread>
#include <mutex>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

namespace h13
{
	class OpenCV_Monitor
	{
	public:
		OpenCV_Monitor()
		{
			std::thread refreshThread([this]
			{
				while (1)
				{
					cv::waitKey(10);
					std::unique_lock<std::mutex> lock(exitMutex);
					if (this -> exit)
					{
						break;
					}
				}
			});
			refreshThread.detach();
		};
		~OpenCV_Monitor()
		{ 
			std::unique_lock<std::mutex> lock(exitMutex);
			exit = true;
		};

		void Display(const std::string& Tittle, cv::InputArray InputImage);

	private:
		std::mutex exitMutex;
		bool exit = false;
	};
}
