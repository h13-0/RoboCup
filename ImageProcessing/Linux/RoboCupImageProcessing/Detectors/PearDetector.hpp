#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "Detector.hpp"

namespace RoboCup
{
	class PearDetector : public Detector
	{
	public:
		PearDetector(const std::vector<h13::HSV_Threshold_t>& PositiveThresholds = { h13::HSV_Threshold(40, 61, 110, 255, 30, 135), h13::HSV_Threshold(50, 66, 150, 255, 40, 190) },
			const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = {  })
			: Detector(PositiveThresholds, NegativeThresholds) { };
		~PearDetector() { };

		std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept;
	};
}