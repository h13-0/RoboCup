#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "AppleDetectorConfigs.hpp"
#include "Detector.hpp"

namespace RoboCup
{
	class AppleDetector : public Detector
	{
	public:
		AppleDetector(const std::vector<h13::HSV_Threshold_t>& PositiveThresholds = { h13::HSV_Threshold(70, 102, 50, 255, 79, 255) },
			const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = { h13::HSV_Threshold(210, 255, 95, 255, 125, 255), h13::HSV_Threshold(0, 74, 60, 255, 0, 255) },
			const std::vector<h13::HSV_Threshold_t>& ReflectiveThresholds = { h13::HSV_Threshold(70, 255, 0, 255, 253, 255) }, 
			int ErodeKernelSize = 5,
			float MinimumSize = 1300,
			float MinimunLengthWidthRatio = 0.65) noexcept
			: Detector(PositiveThresholds, NegativeThresholds, ReflectiveThresholds)
			, erodeKernelSize(ErodeKernelSize), minimumSize(MinimumSize), minimunLengthWidthRatio(MinimunLengthWidthRatio) { };

#if defined(__USE_DETECTOR_CONFIGS__)
		AppleDetector(const AppleDetectorConfigs& configs) noexcept
			: Detector(configs.GetPositiveThresholds(), configs.GetNegativeThresholds(), configs.GetReflectiveThresholds())
			, erodeKernelSize(configs.GetErodeKernelSize()), minimumSize(configs.GetMinimumSize()), minimunLengthWidthRatio(configs.GetMinimunLengthWidthRatio()) { };
#endif

		~AppleDetector() { };

		std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept;

	private:
		int erodeKernelSize;
		float minimumSize;
		float minimunLengthWidthRatio;
	};
}
