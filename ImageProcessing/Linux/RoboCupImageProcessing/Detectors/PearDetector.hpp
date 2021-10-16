#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "PearDetectorConfigs.hpp"
#include "Detector.hpp"

namespace RoboCup
{
	class PearDetector : public Detector
	{
	public:
		PearDetector(const std::vector<h13::HSV_Threshold_t>& PositiveThresholds = { h13::HSV_Threshold(40, 61, 145, 255, 30, 255), h13::HSV_Threshold(55, 66, 185, 255, 40, 255) },
			const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = {  }, 
			float MinimumSize = 1000,
			int ErodeKernelSize = 5,
			float MinimumLengthWidthRatio = 0.5) noexcept
			: Detector(PositiveThresholds, NegativeThresholds)
			, minimumSize(MinimumSize), erodeKernelSize(ErodeKernelSize), minimumLengthWidthRatio(MinimumLengthWidthRatio) { };
		
#if defined(__USE_DETECTOR_CONFIGS__)
		PearDetector(const PearDetectorConfigs& configs) noexcept
			: Detector(configs.GetPositiveThresholds(), configs.GetNegativeThresholds(), configs.GetReflectiveThresholds())
			, minimumSize(configs.GetMinimumSize()), erodeKernelSize(configs.GetErodeKernelSize()), minimumLengthWidthRatio(configs.GetMinimumLengthWidthRatio()) { };
#endif
		
		~PearDetector() { };

		std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept;
	
	private:
		float minimumSize;
		int erodeKernelSize;
		float minimumLengthWidthRatio;
	};
}