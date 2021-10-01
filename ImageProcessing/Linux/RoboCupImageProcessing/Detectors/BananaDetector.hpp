#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "Detector.hpp"

namespace RoboCup
{
	class BananaDetector : public Detector
	{
	public:
		BananaDetector(const std::vector<h13::HSV_Threshold_t>& PositiveThresholds = { h13::HSV_Threshold(26, 55, 60, 175, 174, 255) },
			const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = { },
			const std::vector<h13::HSV_Threshold_t>& ReflectiveThresholds = { })
			: Detector(PositiveThresholds, NegativeThresholds, ReflectiveThresholds) { };
		~BananaDetector() { };

		std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept;
	};
}
