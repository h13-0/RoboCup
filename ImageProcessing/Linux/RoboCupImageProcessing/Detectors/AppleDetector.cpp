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
		ConvertToHSV_FULL(InputBGR_Image, hsvImage);
	}

	if (OutputFilledBinaryContours.needed())
	{
		OutputFilledBinaryContours.create(InputBGR_Image.size(), CV_8UC1);
		outputContours = OutputFilledBinaryContours.getMat();
		outputContours.setTo(0);
		outputContoursRequired = true;
	}

	Mat filterOutout = Mat::zeros(hsvImage.size(), CV_8UC1);

	Filter(hsvImage, filterOutout);
	
	//Corrosion image to eliminate pear and kiwi interference.
	Mat erodeKernel = getStructuringElement(MORPH_ELLIPSE, Size(erodeKernelSize, erodeKernelSize));
	erode(filterOutout, filterOutout, erodeKernel);

	//Judge the roundness of the edge to select the apple.
	vector<vector<Point>> contours;
	findContours(filterOutout, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	
	vector<RotatedRect> result;
	for (unsigned int index = 0; index < contours.size(); index ++)
	{
		float size = contourArea(contours[index]);
		if (size > minimumSize)
		{
			RotatedRect minRect = minAreaRect(Mat(contours[index]));
			Size2f rectSize = minRect.size;
			float lengthWidthRatio = (rectSize.width > rectSize.height) ? (rectSize.height / rectSize.width) : (rectSize.width / rectSize.height);
			if (lengthWidthRatio > minimumLengthWidthRatio)
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
