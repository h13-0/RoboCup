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
/// Use positive and negative as masks.
/// </summary>
/// <param name="InputHSV_FULL_Image"> Input image in HSV_FULL. </param>
/// <param name="OutputBinaryImage"> Output mask in Binary. </param>
void RoboCup::Detector::Filter(cv::InputArray InputHSV_FULL_Image, cv::OutputArray OutputBinaryMask)
{
	using namespace std;
	using namespace cv;

	OutputBinaryMask.create(InputHSV_FULL_Image.size(), CV_8UC1);
	Mat output = OutputBinaryMask.getMat();
	output.setTo(0);

	for (auto& filter : positiveFilters)
	{
		Mat positiveOutput;
		filter.FilterWithHSV_FULL_Image(InputHSV_FULL_Image, positiveOutput);
		bitwise_or(output, positiveOutput, output);
	}
#ifdef _DEBUG
	imshow("pos", output);
#endif
	if (negativeFilters.size() > 0)
	{
		Mat negativeOutout = Mat::zeros(InputHSV_FULL_Image.size(), CV_8UC1);
		for (auto& filter : negativeFilters)
		{
			Mat filterOutput;
			filter.FilterWithHSV_FULL_Image(InputHSV_FULL_Image, filterOutput);
			bitwise_or(filterOutput, negativeOutout, negativeOutout);
		}

		bitwise_not(negativeOutout, negativeOutout);
		bitwise_and(output, negativeOutout, output);
	}
#ifdef _DEBUG
	imshow("filter", output);
#endif
}

/// <summary>
/// Calculate rect overlap rate.
/// </summary>
/// <param name="Rect1"></param>
/// <param name="Rect2"></param>
/// <returns>overlap rate.</returns>
float RoboCup::Detector::calculateRectOverlapRate(const cv::Rect& Rect1, const cv::Rect& Rect2)
{
	double x1 = fmax(Rect1.x, Rect2.x);
	double x2 = fmin(Rect1.x + Rect1.width, Rect2.x + Rect2.width);
	double y1 = fmax(Rect1.y, Rect2.y);
	double y2 = fmin(Rect1.y + Rect1.height, Rect2.y + Rect2.height);
	if ((x2 < x1) || (y2 < y1))
	{
		return 0.0f;
	}
	else {
		return ((x2 - x1) * (y2 - y1)) / fmin(Rect1.area(), Rect2.area());
	}
}
