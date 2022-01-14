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
			const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = { h13::HSV_Threshold(0, 65, 60, 255, 0, 255) },
			const std::vector<h13::HSV_Threshold_t>& ReflectiveThresholds = { h13::HSV_Threshold(70, 255, 0, 255, 253, 255) },
			const std::vector<h13::HSV_Threshold_t>& PitayaThresholds = { h13::HSV_Threshold(210, 255, 175, 255, 125, 255) },
			float MinimumSize = 1300,
			int ErodeKernelSize = 5,
			float MinimumLengthWidthRatio = 0.65,
			float MinimumPitayaSize = 1500) noexcept
			: Detector(PositiveThresholds, NegativeThresholds, ReflectiveThresholds)
			, minimumSize(MinimumSize), erodeKernelSize(ErodeKernelSize), minimumLengthWidthRatio(MinimumLengthWidthRatio), minimumPitayaSize(MinimumPitayaSize)
		{
			for (auto threshold : PitayaThresholds)
			{
				pitayaFilters.push_back(h13::HSV_Filter(threshold));
			}
		};

#if defined(__USE_DETECTOR_CONFIGS__)
		AppleDetector(const AppleDetectorConfigs& configs) noexcept
			: Detector(configs.GetPositiveThresholds(), configs.GetNegativeThresholds(), configs.GetReflectiveThresholds())
			, minimumSize(configs.GetMinimumSize()), erodeKernelSize(configs.GetErodeKernelSize())
			, minimumLengthWidthRatio(configs.GetMinimumLengthWidthRatio()), minimumPitayaSize(configs.GetMinimumPitayaSize())
		{ 
			for (auto threshold : configs.GetPitayaThresholds())
			{
				pitayaFilters.push_back(h13::HSV_Filter(threshold));
			}
		};
#endif

		~AppleDetector() { };

		std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept;

	private:
		std::vector<h13::HSV_Filter> pitayaFilters;
		float minimumSize;
		int erodeKernelSize;
		float minimumLengthWidthRatio;
		float minimumPitayaSize;
	};
}