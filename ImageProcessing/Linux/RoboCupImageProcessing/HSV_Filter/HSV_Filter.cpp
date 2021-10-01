#include "HSV_Filter.hpp"

void h13::HSV_Filter::ConvertToHSV(cv::InputArray Input, cv::OutputArray Output)
{
	using namespace cv;
	CV_Assert(Input.type() == CV_8UC3);
	cvtColor(Input, Output, COLOR_BGR2HSV_FULL);
}

/// <summary>
/// Find the region in the image that meets the threshold and output it to the binary image.
/// </summary>
/// <param name="Input"> Input image in BGR. </param>
/// <param name="Output"> Output image in Binary. </param>
void h13::HSV_Filter::Filter(cv::InputArray Input, cv::OutputArray Output)
{
	using namespace cv;
	Mat input;
	CV_Assert(Input.type() == CV_8UC3);
	ConvertToHSV(Input, input);
	inRange(input, Scalar(hMin, sMin, vMin), Scalar(hMax, sMax, vMax), Output);
}

/// <summary>
/// Find the region in the image that meets the threshold and output it to the binary image.
/// </summary>
/// <param name="Input"> Input image in HSV_FULL. </param>
/// <param name="Output"> Output image in Binary. </param>
void h13::HSV_Filter::FilterWithHSV_FULL_Image(cv::InputArray Input, cv::OutputArray Output)
{
	using namespace cv;
	CV_Assert(Input.type() == CV_8UC3);
	inRange(Input, Scalar(hMin, sMin, vMin), Scalar(hMax, sMax, vMax), Output);
}

/// <summary>
/// Find the region in the image that meets the threshold and return target box.
/// </summary>
/// <param name="Input"> Input image in BGR. </param>
/// <returns></returns>
std::vector<h13::Rect_t> h13::HSV_Filter::Detect(cv::InputArray Input)
{
	return std::vector<h13::Rect_t>();
}

void h13::HSV_Filter::SetHueRange(unsigned int Minimum, unsigned int Maximum)
{
	hMin = Minimum;
	hMax = Maximum;
}

void h13::HSV_Filter::SetSaturationRange(unsigned int Minimum, unsigned int Maximum)
{
	sMin = Minimum;
	sMax = Maximum;
}

void h13::HSV_Filter::SetValueRange(unsigned int Minimum, unsigned int Maximum)
{
	vMin = Minimum;
	vMax = Maximum;
}

void h13::HSV_Filter::SetHueMaximumValue(unsigned int Value)
{
	hMax = Value;
}

void h13::HSV_Filter::SetHueMinimumValue(unsigned int Value)
{
	hMin = Value;
}

void h13::HSV_Filter::SetSaturationMaximumValue(unsigned int Value)
{
	sMax = Value;
}

void h13::HSV_Filter::SetSaturationMinimumValue(unsigned int Value)
{
	sMin = Value;
}

void h13::HSV_Filter::SetValueMaximumValue(unsigned int Value)
{
	vMax = Value;
}

void h13::HSV_Filter::SetValueMinimumValue(unsigned int Value)
{
	vMin = Value;
}
