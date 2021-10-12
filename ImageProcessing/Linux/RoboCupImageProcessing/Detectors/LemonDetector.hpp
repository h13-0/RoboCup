#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "LemonDetectorConfigs.hpp"
#include "Detector.hpp"

namespace RoboCup
{
	class LemonDetector : public Detector
	{
	public:
		LemonDetector(const std::vector<h13::HSV_Threshold_t>& PositiveThresholds = { h13::HSV_Threshold(15, 36, 110, 255, 116, 255), h13::HSV_Threshold(35, 50, 85, 220, 80, 255) },
			const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = { h13::HSV_Threshold(12, 24, 101, 255, 150, 255) },
			const std::vector<h13::HSV_Threshold_t>& ReflectiveThresholds = { },
			float MinimumSize = 1300,
			int ErodeKernelSize = 5,
			float MinimunLengthWidthRatio = 0.55) noexcept
			: Detector(PositiveThresholds, NegativeThresholds, ReflectiveThresholds)
			, erodeKernelSize(ErodeKernelSize), minimumSize(MinimumSize), minimunLengthWidthRatio(MinimunLengthWidthRatio) { };

#if defined(__USE_DETECTOR_CONFIGS__)
		LemonDetector(const LemonDetectorConfigs& configs) noexcept
			: Detector(configs.GetPositiveThresholds(), configs.GetNegativeThresholds(), configs.GetReflectiveThresholds())
			, erodeKernelSize(configs.GetErodeKernelSize()), minimumSize(configs.GetMinimumSize()), minimunLengthWidthRatio(configs.GetMinimumLengthWidthRatio()) { };
#endif
		~LemonDetector() noexcept { };

		std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept;
	
	private:
		float minimumSize;
		int erodeKernelSize;
		float minimunLengthWidthRatio;
	};
}