#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "Detector.hpp"

namespace RoboCup
{
	class KiwiFruitDetector : public Detector
	{
	public:
		KiwiFruitDetector(const std::vector<h13::HSV_Threshold_t>& PositiveThresholds = { h13::HSV_Threshold(60, 80, 80, 180, 50, 255) },
			const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = { h13::HSV_Threshold(70, 255, 165, 255, 0, 255) })
			: Detector(PositiveThresholds, NegativeThresholds) { };//60, 75, 80, 125, 50, 220
		~KiwiFruitDetector() { };
		std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept;
	};
}