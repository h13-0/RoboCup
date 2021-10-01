#include "AppleDetector.hpp"
#include <vector>
#include <math.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

std::vector<cv::RotatedRect> RoboCup::AppleDetector::Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image, cv::OutputArray OutputFilledBinaryContours) noexcept
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
		this->Detector::ConvertToHSV_FULL(InputBGR_Image, hsvImage);
	}

	if (OutputFilledBinaryContours.needed())
	{
		OutputFilledBinaryContours.create(InputBGR_Image.size(), CV_8UC1);
		outputContours = OutputFilledBinaryContours.getMat();
		outputContours.setTo(0);
		outputContoursRequired = true;
	}

	Mat filterOutout = Mat::zeros(hsvImage.size(), CV_8UC1);
	for (auto filter : positiveFilters)
	{
		Mat positiveOutput;
		filter.FilterWithHSV_FULL_Image(hsvImage, positiveOutput);
		bitwise_or(filterOutout, positiveOutput, filterOutout);
	}

#ifdef _DEBUG
	//imshow("ApplePositive", filterOutout);
#endif

	if (negativeFilters.size() > 0)
	{
		Mat negativeOutout = Mat::zeros(hsvImage.size(), CV_8UC1);
		for (auto filter : negativeFilters)
		{
			Mat subNegativeOutput;
			filter.FilterWithHSV_FULL_Image(hsvImage, subNegativeOutput);
			bitwise_or(negativeOutout, subNegativeOutput, negativeOutout);
		}
		bitwise_not(negativeOutout, negativeOutout);
		bitwise_and(filterOutout, negativeOutout, filterOutout);
	}

	//Corrosion image to eliminate pear and kiwi interference.
	Mat erodeKernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	erode(filterOutout, filterOutout, erodeKernel);

#ifdef _DEBUG
	//imshow("AppleFinal", filterOutout);
#endif

	//Judge the roundness of the edge to select the apple.
	vector<vector<Point>> contours;
	findContours(filterOutout, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	
	vector<RotatedRect> result;
	for (unsigned int index = 0; index < contours.size(); index ++)
	{
		float size = contourArea(contours[index]);
		if (size > 1300)
		{
			RotatedRect minRect = minAreaRect(Mat(contours[index]));
			Size2f rectSize = minRect.size;
			float lengthWidthRatio = (rectSize.width > rectSize.height) ? (rectSize.height / rectSize.width) : (rectSize.width / rectSize.height);
			cout << lengthWidthRatio << endl;
			if (lengthWidthRatio > 0.65)
			{
				result.push_back(minRect);
				if (outputContoursRequired)
				{
					drawContours(outputContours, contours, index, Scalar(255), CV_FILLED);
				}
			}
		}
	}

	if (outputContoursRequired)
	{
		dilate(filterOutout, filterOutout, erodeKernel);
	}

	return result;
}
