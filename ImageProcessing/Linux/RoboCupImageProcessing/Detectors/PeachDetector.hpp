#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "Detector.hpp"

namespace RoboCup
{
	class PeachDetector : public Detector
	{
	public:
		PeachDetector(const std::vector<h13::HSV_Threshold_t>& PositiveThresholds = { h13::HSV_Threshold(0, 18, 40, 190, 135, 255), h13::HSV_Threshold(220, 255, 15, 140, 130, 255) },
			const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = { h13::HSV_Threshold(9, 202, 0, 61, 0, 255) })
			: Detector(PositiveThresholds, NegativeThresholds) { };
		~PeachDetector() { };
		std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept;
	};
}

