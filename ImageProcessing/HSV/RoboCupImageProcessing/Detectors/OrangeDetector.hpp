#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "OrangeDetectorConfigs.hpp"
#include "Detector.hpp"

namespace RoboCup
{
	class OrangeDetector : public Detector
	{
	public:
		OrangeDetector(const std::vector<h13::HSV_Threshold_t>& PositiveThresholds = { h13::HSV_Threshold(3, 26, 106, 255, 0, 255) },
			const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = { h13::HSV_Threshold(7, 21, 100, 150, 0, 255) },
			const std::vector<h13::HSV_Threshold_t>& ReflectiveThresholds = { },
			float MinimumSize = 1600,
			int ErodeKernelSize = 5) noexcept
			: Detector(PositiveThresholds, NegativeThresholds, ReflectiveThresholds) { };
		
#if defined(__USE_DETECTOR_CONFIGS__)
		OrangeDetector(const OrangeDetectorConfigs& configs) noexcept
			: Detector(configs.GetPositiveThresholds(), configs.GetNegativeThresholds(), configs.GetReflectiveThresholds())
			, minimumSize(configs.GetMinimumSize()), erodeKernelSize(configs.GetErodeKernelSize()) { };
#endif

		~OrangeDetector() noexcept { };

		std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept;
	
	private:
		float minimumSize;
		int erodeKernelSize;
	};
}
