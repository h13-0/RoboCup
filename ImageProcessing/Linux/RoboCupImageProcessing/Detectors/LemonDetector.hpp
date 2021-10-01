#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "Detector.hpp"

namespace RoboCup
{
	class LemonDetector : public Detector
	{
	public:
		LemonDetector(const std::vector<h13::HSV_Threshold_t>& PositiveThresholds = { h13::HSV_Threshold(15, 36, 75, 255, 116, 255), h13::HSV_Threshold(35, 50, 50, 220, 80, 255) },
			const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = { h13::HSV_Threshold(12, 24, 101, 255, 150, 255) },
			const std::vector<h13::HSV_Threshold_t>& ReflectiveThresholds = { })
			: Detector(PositiveThresholds, NegativeThresholds, ReflectiveThresholds) { };
		~LemonDetector() { };

		std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept;
	};
}