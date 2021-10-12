#pragma once
#include <vector>
#include <array>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

#include "HSV_Filter.hpp"

namespace RoboCup
{
	/// <summary>
	/// Base class for all detectors.
	/// </summary>
	class Detector
	{
	public:
		Detector(const std::vector<h13::HSV_Threshold_t>& PositiveThresholds, const std::vector<h13::HSV_Threshold_t>& NegativeThresholds = { }, const std::vector<h13::HSV_Threshold_t>& ReflectiveThresholds = { })
		{
			using namespace h13;

			for (auto threshold : PositiveThresholds)
			{
				positiveFilters.push_back(HSV_Filter(threshold));
			}

			for (auto threshold : NegativeThresholds)
			{
				negativeFilters.push_back(HSV_Filter(threshold));
			}

			for (auto threshold : ReflectiveThresholds)
			{
				reflectiveFilters.push_back(HSV_Filter(threshold));
			}
		};

		virtual ~Detector() { };

		/// <summary>
		/// Detect target and return location.
		/// </summary>
		/// <param name="InputBGR_Image"> Input image in BGR. </param>
		/// <param name="InputHSV_FULL_Image"> Input image in HSV_FULL. </param>
		/// <returns> locations of targets. </returns>
		virtual std::vector<cv::RotatedRect> Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image = cv::noArray(), cv::OutputArray OutputFilledBinaryContours = cv::noArray()) noexcept = 0;
		
		/// <summary>
		/// Gets the filters of the current detector.
		/// </summary>
		/// <param name="PositiveFilters"> Positive Filters. </param>
		/// <param name="NegativeFilters"> Negative Filters. </param>
		/// <param name="ReflectiveFilters"> Reflective Filters. </param>
		/// <returns></returns>
		void GetFilters(std::vector<h13::HSV_Filter>& PositiveFilters, std::vector<h13::HSV_Filter>& NegativeFilters, std::vector<h13::HSV_Filter>& ReflectiveFilters) const noexcept
		{
			PositiveFilters = positiveFilters;
			NegativeFilters = negativeFilters;
			ReflectiveFilters = reflectiveFilters;
		};

	protected:
		/// <summary>
		/// Common methods of detectors.
		/// </summary>
		
		/// <summary>
		/// Convert BGR image to HSV_FULL.
		/// </summary>
		/// <param name="InputBGR_Image"> Input image in BGR. </param>
		/// <param name="OutputHSV_FULL_Image"> Input image in HSV_FULL. </param>
		void ConvertToHSV_FULL(cv::InputArray InputBGR_Image, cv::OutputArray OutputHSV_FULL_Image);

		/// <summary>
		/// Obtain color patches according to fixedand common methods.
		/// </summary>
		/// <param name="InputHSV_FULL_Image"> Input image in HSV_FULL. </param>
		/// <param name="OutputBinaryImage"> Output image in Binary. </param>
		void Filter(cv::InputArray InputHSV_FULL_Image, cv::OutputArray OutputBinaryImage);

		/// <summary>
		/// Protected variables of detectors.
		/// </summary>
		std::vector<h13::HSV_Filter> positiveFilters;
		std::vector<h13::HSV_Filter> negativeFilters;
		std::vector<h13::HSV_Filter> reflectiveFilters;
	};
}
