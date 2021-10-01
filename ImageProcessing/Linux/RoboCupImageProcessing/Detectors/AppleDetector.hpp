#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "Detector.hpp"

namespace RoboCup
{
	class AppleDetector : public Detector
	{
	public:
		AppleDetector(const std::vector<h13::HSV_Threshold_t>& PositiveThresholds = { h13::HSV_Threshold(70, 102, 50, 255, 79, 255) },
			const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = { h13::HSV_Threshold(210, 255, 95, 255, 125, 255), h13::HSV_Threshold(0, 74, 60, 255, 0, 255) },
			const std::vector<h13::HSV_Threshold_t>& ReflectiveThresholds = { h13::HSV_Threshold(70, 255, 0, 255, 253, 255) })
			: Detector(PositiveThresholds, NegativeThresholds, ReflectiveThresholds) { };

		~AppleDetector() { };

		std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept;
	};
}
