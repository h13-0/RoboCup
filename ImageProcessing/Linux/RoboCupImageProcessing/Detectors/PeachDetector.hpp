#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "PeachDetectorConfigs.hpp"
#include "Detector.hpp"

namespace RoboCup
{
	class PeachDetector : public Detector
	{
	public:
		PeachDetector(const std::vector<h13::HSV_Threshold_t>& PositiveThresholds = { h13::HSV_Threshold(0, 18, 40, 190, 135, 255), h13::HSV_Threshold(220, 255, 15, 140, 130, 255) },
			const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = { h13::HSV_Threshold(9, 202, 0, 61, 0, 255) },
			const std::vector<h13::HSV_Threshold_t>& ReflectiveThresholds = { h13::HSV_Threshold(70, 255, 0, 255, 253, 255) },
			float MinimumSize = 1300,
			int ErodeKernelSize = 10,
			float MinimunLengthWidthRatio = 0.5)
			: Detector(PositiveThresholds, NegativeThresholds, ReflectiveThresholds)
			, erodeKernelSize(ErodeKernelSize), minimumSize(MinimumSize), minimumLengthWidthRatio(MinimunLengthWidthRatio) { };

#if defined(__USE_DETECTOR_CONFIGS__)
		PeachDetector(const PeachDetectorConfigs& configs) noexcept
			: Detector(configs.GetPositiveThresholds(), configs.GetNegativeThresholds(), configs.GetReflectiveThresholds())
			, minimumSize(configs.GetMinimumSize()), erodeKernelSize(configs.GetErodeKernelSize()), minimumLengthWidthRatio(configs.GetMinimumLengthWidthRatio()) { };
#endif

		~PeachDetector() { };

		std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept;
	
	private:
		float minimumSize;
		int erodeKernelSize;
		float minimumLengthWidthRatio;
	};
}

