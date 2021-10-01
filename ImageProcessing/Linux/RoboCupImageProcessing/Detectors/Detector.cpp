#include "Detector.hpp"
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

#include "HSV_Filter.hpp"

/// <summary>
/// Convert BGR image to HSV_FULL.
/// </summary>
/// <param name="InputBGR_Image"> Input image in BGR. </param>
/// <param name="OutputHSV_FULL_Image"> Input image in HSV_FULL. </param>
void RoboCup::Detector::ConvertToHSV_FULL(cv::InputArray InputBGR_Image, cv::OutputArray OutputHSV_FULL_Image)
{
	cv::cvtColor(InputBGR_Image, OutputHSV_FULL_Image, cv::COLOR_BGR2HSV_FULL);
}

/// <summary>
/// Obtain color patches according to fixedand common methods.
/// </summary>
/// <param name="InputHSV_FULL_Image"> Input image in HSV_FULL. </param>
/// <param name="OutputBinaryImage"> Output image in Binary. </param>
void RoboCup::Detector::Filter(cv::InputArray InputHSV_FULL_Image, cv::OutputArray OutputBinaryImage)
{
	using namespace std;
	using namespace cv;

	OutputBinaryImage.create(InputHSV_FULL_Image.size(), CV_8UC1);
	Mat output = OutputBinaryImage.getMat();
	output.setTo(0);

	for (auto filter : positiveFilters)
	{
		Mat positiveOutput;
		filter.FilterWithHSV_FULL_Image(InputHSV_FULL_Image, positiveOutput);
		bitwise_or(output, positiveOutput, output);
	}
#ifdef _DEBUG
	//imshow("pos", output);
#endif
	if (negativeFilters.size() > 0)
	{
		Mat negativeOutout = Mat::zeros(InputHSV_FULL_Image.size(), CV_8UC1);
		for (auto filter : negativeFilters)
		{
			Mat filterOutput;
			filter.FilterWithHSV_FULL_Image(InputHSV_FULL_Image, filterOutput);
			bitwise_or(filterOutput, negativeOutout, negativeOutout);
		}

		bitwise_not(negativeOutout, negativeOutout);
		bitwise_and(output, negativeOutout, output);
	}
#ifdef _DEBUG
	//imshow("filter", output);
#endif
}
