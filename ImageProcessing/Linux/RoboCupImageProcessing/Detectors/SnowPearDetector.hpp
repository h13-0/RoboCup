#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "SnowPearDetectorConfigs.hpp"
#include "Detector.hpp"

namespace RoboCup
{
	class SnowPearDetector : public Detector
	{
	public:
		SnowPearDetector(
			const std::vector<h13::HSV_Threshold_t>& PositiveThresholds = { h13::HSV_Threshold(40, 75, 21, 100, 80, 255) },
			const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = { h13::HSV_Threshold(0, 50, 0, 255, 0, 255) },
			const std::vector<h13::HSV_Threshold_t>& ReflectiveThresholds = { h13::HSV_Threshold(55, 70, 110, 255, 80, 255) }, 
			float MinimumSize = 900,
			float MinimumLengthWidthRatio = 0.5,
			float MinimumContourArea = 2200,
			float MaximumContourArea = 15000) noexcept
			: Detector(PositiveThresholds, NegativeThresholds, ReflectiveThresholds)
			, minimumSize(MinimumSize), minimumLengthWidthRatio(MinimumLengthWidthRatio), minimumContourArea(MinimumContourArea), maximumContourArea(MaximumContourArea) { };
		
#if defined(__USE_DETECTOR_CONFIGS__)
		SnowPearDetector(const SnowPearDetectorConfigs& configs) noexcept
			: Detector(configs.GetPositiveThresholds(), configs.GetNegativeThresholds(), configs.GetReflectiveThresholds())
			, minimumSize(configs.GetMinimumSize()), minimumLengthWidthRatio(configs.GetMinimumLengthWidthRatio()), maximumContourArea(configs.GetMaximumContourArea()) { };
#endif
		
		~SnowPearDetector() { };

		std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept;
	
	private:
		float minimumSize;
		float minimumLengthWidthRatio;
		float minimumContourArea;
		float maximumContourArea;
	};
}
