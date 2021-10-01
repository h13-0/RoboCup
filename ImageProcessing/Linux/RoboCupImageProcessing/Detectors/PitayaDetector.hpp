#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "Detector.hpp"

namespace RoboCup
{
	class PitayaDetector : public Detector
	{
	public:
		PitayaDetector(const std::vector<h13::HSV_Threshold_t>& PositiveThresholds = { h13::HSV_Threshold(210, 255, 95, 255, 125, 255) },
			const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = { } )
			: Detector(PositiveThresholds, NegativeThresholds) { };
		~PitayaDetector() { };

		std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept;
	};
}