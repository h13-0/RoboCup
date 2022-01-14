#include "TargetDetector.hpp"

RoboCup::TargetDetector::TargetDetector(const std::vector<h13::HSV_Threshold_t>& PositiveThresholds,
	const std::vector<h13::HSV_Threshold_t>& NegativeThresholds,
	const std::vector<h13::HSV_Threshold_t>& ReflectiveThresholds,
	float MinimumPositiveSize,
	float MinimumPositiveLengthWidthRatio,
	float MinimumReflectiveSize,
	float MinimumReflectiveLengthWidthRatio,
	float MinimumRectOverlapRate)
	: Detector(PositiveThresholds, NegativeThresholds, ReflectiveThresholds)
	, minimumPositiveSize(MinimumPositiveSize), minimumPositiveLengthWidthRatio(MinimumPositiveLengthWidthRatio)
	, minimumReflectiveSize(MinimumReflectiveSize), minimumReflectiveLengthWidthRatio(MinimumReflectiveLengthWidthRatio)
	, minimumRectOverlapRate(MinimumRectOverlapRate)
{
}

std::vector<cv::RotatedRect> RoboCup::TargetDetector::Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image, cv::OutputArray OutputFilledBinaryContours) noexcept
{
	using namespace std;
	using namespace cv;

	Mat hsvImage;
	Mat outputContours;
	bool outputContoursRequired = false;

	CV_Assert(!InputBGR_Image.empty());

	if (InputBGR_Image.size() == InputHSV_FULL_Image.size())
	{
		hsvImage = InputHSV_FULL_Image.getMat();
	}
	else {
		ConvertToHSV_FULL(InputBGR_Image, hsvImage);
	}

	if (OutputFilledBinaryContours.needed())
	{
		OutputFilledBinaryContours.create(InputBGR_Image.size(), CV_8UC1);
		outputContours = OutputFilledBinaryContours.getMat();
		outputContours.setTo(0);
		outputContoursRequired = true;
	}

	//Find the positive section.
	Mat positiveFilterOutout = Mat::zeros(hsvImage.size(), CV_8UC1);
	for (auto filter : positiveFilters)
	{
		Mat positiveOutput;
		filter.FilterWithHSV_FULL_Image(hsvImage, positiveOutput);
		bitwise_or(positiveFilterOutout, positiveOutput, positiveFilterOutout);
	}

	Mat erodeKernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	erode(positiveFilterOutout, positiveFilterOutout, erodeKernel);

	vector<vector<Point>> positiveContours;
	findContours(positiveFilterOutout, positiveContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	vector<Rect> positiveRects;
	for (unsigned int index = 0; index < positiveContours.size(); index++)
	{
		float size = contourArea(positiveContours[index]);
		if (size > minimumPositiveSize)
		{
			RotatedRect minRect = minAreaRect(Mat(positiveContours[index]));
			Size2f rectSize = minRect.size;
			float lengthWidthRatio = (rectSize.width > rectSize.height) ? (rectSize.height / rectSize.width) : (rectSize.width / rectSize.height);
			if (lengthWidthRatio > minimumPositiveLengthWidthRatio)
			{
				positiveRects.push_back(minRect.boundingRect());
			}
		}
	}

	//Find the reflective section.
	Mat reflectiveOutput = Mat::zeros(hsvImage.size(), CV_8UC1);
	for (auto filter : reflectiveFilters)
	{
		Mat positiveOutput;
		filter.FilterWithHSV_FULL_Image(hsvImage, positiveOutput);
		bitwise_or(reflectiveOutput, positiveOutput, reflectiveOutput);
	}

	vector<vector<Point>> reflectiveContours;
	findContours(reflectiveOutput, reflectiveContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	vector<RotatedRect> reflectiveRects;
	for (unsigned int index = 0; index < reflectiveContours.size(); index++)
	{
		float size = contourArea(reflectiveContours[index]);
		if (size > minimumReflectiveSize)
		{
			RotatedRect minRect = minAreaRect(Mat(reflectiveContours[index]));
			Size2f rectSize = minRect.size;
			float lengthWidthRatio = (rectSize.width > rectSize.height) ? (rectSize.height / rectSize.width) : (rectSize.width / rectSize.height);
			if (lengthWidthRatio > minimumReflectiveLengthWidthRatio)
			{
				reflectiveRects.push_back(minRect);
			}
		}
	}

	//Find best results.
	vector<RotatedRect> result;
	if ((reflectiveRects.size() > 0) && (positiveRects.size() > 0))
	{
		for (auto reflectiveRect : reflectiveRects)
		{
			for (auto positiveRect : positiveRects)
			{
				if (calculateRectOverlapRate(reflectiveRect.boundingRect(), positiveRect) > minimumRectOverlapRate)
				{
					result.push_back(reflectiveRect);
				}
			}
		}
	}
	else if (reflectiveRects.size() > 0)
	{
		return reflectiveRects;
	}

	return result;
}
