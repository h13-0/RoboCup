#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "PitayaDetectorConfigs.hpp"
#include "Detector.hpp"

namespace RoboCup
{
	class PitayaDetector : public Detector
	{
	public:
		PitayaDetector(const std::vector<h13::HSV_Threshold_t>& PositiveThresholds = { h13::HSV_Threshold(210, 255, 95, 255, 125, 255) },
			const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = { },
			const std::vector<h13::HSV_Threshold_t>& ReflectiveThresholds = { },
			float MinimumSize = 3000,
			int ErodeKernelSize = 5) noexcept
			: Detector(PositiveThresholds, NegativeThresholds, ReflectiveThresholds)
			, erodeKernelSize(ErodeKernelSize), minimumSize(MinimumSize) { };

#if defined(__USE_DETECTOR_CONFIGS__)
		PitayaDetector(const PitayaDetectorConfigs& configs) noexcept
			: Detector(configs.GetPositiveThresholds(), configs.GetNegativeThresholds(), configs.GetReflectiveThresholds())
			, minimumSize(configs.GetMinimumSize()), erodeKernelSize(configs.GetErodeKernelSize()) { };
#endif

		~PitayaDetector() { };

		std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept;
	
	private:
		float minimumSize;
		int erodeKernelSize;
	};
}