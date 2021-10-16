#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "TargetDetectorConfigs.hpp"
#include "Detector.hpp"

namespace RoboCup
{
	class TargetDetector : public Detector
	{
	public:
		TargetDetector(const std::vector<h13::HSV_Threshold_t>& PositiveThresholds = { h13::HSV_Threshold(116, 180, 0, 70, 90, 255) },
			const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = { },
			const std::vector<h13::HSV_Threshold_t>& ReflectiveThresholds = { h13::HSV_Threshold(240, 255, 155, 255, 80, 255) },
			float MinimumPositiveSize = 5000,
			float MinimumPositiveLengthWidthRatio = 0.4,
			float MinimumReflectiveSize = 2000,
			float MinimumReflectiveLengthWidthRatio = 0.4,
			float MinimumRectOverlapRate = 0.5);

#if defined(__USE_DETECTOR_CONFIGS__)
		TargetDetector(const TargetDetectorConfigs& configs) noexcept
			: Detector(configs.GetPositiveThresholds(), configs.GetNegativeThresholds(), configs.GetReflectiveThresholds())
			, minimumPositiveSize(configs.GetMinimumPositiveSize()), minimumPositiveLengthWidthRatio(configs.GetMinimumPositiveLengthWidthRatio())
			, minimumReflectiveSize(configs.GetMinimumReflectiveSize()), minimumReflectiveLengthWidthRatio(configs.GetMinimumReflectiveLengthWidthRatio())
			, minimumRectOverlapRate(configs.GetMinimumRectOverlapRate()) { }
#endif

		~TargetDetector() { };

		std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept;

	private:
		float minimumPositiveSize;
		float minimumPositiveLengthWidthRatio;
		float minimumReflectiveSize;
		float minimumReflectiveLengthWidthRatio;
		float minimumRectOverlapRate;
	};
}
