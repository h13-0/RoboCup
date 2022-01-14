#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

namespace RoboCup
{
	class Monitor
	{
	public:
		virtual ~Monitor() { };
		/// <summary>
		/// Send a new image to monitor.
		/// note: Some subclasses will only display images whose title is "Final".
		/// </summary>
		/// <param name="Title">Title for image.</param>
		/// <param name="InputImage">Image.</param>
		virtual void Display(const std::string& Title, cv::InputArray InputImage) = 0;
	};
}