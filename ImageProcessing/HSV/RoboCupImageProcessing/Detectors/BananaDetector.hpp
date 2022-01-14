#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "BananaDetectorConfigs.hpp"
#include "Detector.hpp"

namespace RoboCup
{
	class BananaDetector : public Detector
	{
	public:
		BananaDetector(const std::vector<h13::HSV_Threshold_t>& PositiveThresholds = { h13::HSV_Threshold(26, 55, 60, 175, 174, 255) },
			const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = { },
			const std::vector<h13::HSV_Threshold_t>& ReflectiveThresholds = { },
			float MinimumSize = 2000,
			int ErodeKernelSize = 5,
			int DilateKernelSize = 7,
			float MaximumLengthWidthRatio = 0.5)
			: Detector(PositiveThresholds, NegativeThresholds, ReflectiveThresholds)
			, minimumSize(MinimumSize), erodeKernelSize(ErodeKernelSize), dilateKernelSize(DilateKernelSize), maximumLengthWidthRatio(MaximumLengthWidthRatio) { };

#if defined(__USE_DETECTOR_CONFIGS__)
		BananaDetector(const BananaDetectorConfigs& configs) noexcept
			: Detector(configs.GetPositiveThresholds(), configs.GetNegativeThresholds(), configs.GetReflectiveThresholds())
			, erodeKernelSize(configs.GetErodeKernelSize()), dilateKernelSize(configs.GetDilateKernelSize()), minimumSize(configs.GetMinimumSize()), maximumLengthWidthRatio(configs.GetMaximumLengthWidthRatio()) { };
#endif

		~BananaDetector() noexcept { };

		std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept;

	private:
		float minimumSize;
		int erodeKernelSize;
		int dilateKernelSize;
		float maximumLengthWidthRatio;
	};
}
