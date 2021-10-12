#include "SnowPearDetector.hpp"

std::vector<cv::RotatedRect> RoboCup::SnowPearDetector::Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image, cv::OutputArray OutputFilledBinaryContours) noexcept
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

	Mat positiveFilterOutout = Mat::zeros(hsvImage.size(), CV_8UC1);
	for (auto filter : positiveFilters)
	{
		Mat positiveOutput;
		filter.FilterWithHSV_FULL_Image(hsvImage, positiveOutput);
		bitwise_or(positiveFilterOutout, positiveOutput, positiveFilterOutout);
	}

	if (negativeFilters.size() > 0)
	{
		Mat negativeOutout = Mat::zeros(hsvImage.size(), CV_8UC1);
		for (auto filter : negativeFilters)
		{
			Mat subNegativeOutput;
			filter.FilterWithHSV_FULL_Image(hsvImage, subNegativeOutput);
			bitwise_or(negativeOutout, subNegativeOutput, negativeOutout);
		}

		Mat erodeKernel = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));
		erode(positiveFilterOutout, positiveFilterOutout, erodeKernel);
		dilate(positiveFilterOutout, positiveFilterOutout, erodeKernel);

		bitwise_not(negativeOutout, negativeOutout);
		bitwise_and(positiveFilterOutout, negativeOutout, positiveFilterOutout);
	}
	
	erode(positiveFilterOutout, positiveFilterOutout, getStructuringElement(MORPH_ELLIPSE, Size(20, 20)));
	dilate(positiveFilterOutout, positiveFilterOutout, getStructuringElement(MORPH_ELLIPSE, Size(40, 40)));
	
	Mat reflectiveOutout = Mat::zeros(hsvImage.size(), CV_8UC1);
	if (reflectiveFilters.size() > 0)
	{
		
		for (auto filter : reflectiveFilters)
		{
			Mat subNegativeOutput;
			filter.FilterWithHSV_FULL_Image(hsvImage, subNegativeOutput);
			bitwise_or(reflectiveOutout, subNegativeOutput, reflectiveOutout);
		}
	}
	dilate(reflectiveOutout, reflectiveOutout, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	//dilate(filterOutout, filterOutout, erodeKernel);

	Mat filterOutout = positiveFilterOutout.clone();
	dilate(filterOutout, filterOutout, getStructuringElement(MORPH_ELLIPSE, Size(27, 27)));
	bitwise_and(filterOutout, reflectiveOutout, filterOutout);
	bitwise_or(filterOutout, positiveFilterOutout, filterOutout);
	//fil = bitwise and ero
	//filterOutout = positiveFilterOutout.clone();

	vector<vector<Point>> contours;
	findContours(filterOutout, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	vector<RotatedRect> result;

	for (unsigned int index = 0; index < contours.size(); index++)
	{
		float size = contourArea(contours[index]);
		if (size > minimumSize)
		{
			RotatedRect minRect = minAreaRect(Mat(contours[index]));
			Size2f rectSize = minRect.size;
			float lengthWidthRatio = (rectSize.width > rectSize.height) ? (rectSize.height / rectSize.width) : (rectSize.width / rectSize.height);

			if (lengthWidthRatio > minimumLengthWidthRatio)
			{
				float cArea = contourArea(contours[index]);
				if ((cArea < maximumContourArea) && cArea > minimumContourArea)
				{
					result.push_back(minRect);
					if (outputContoursRequired)
					{
						drawContours(outputContours, contours, index, Scalar(255), CV_FILLED);
					}
				}
			}
		}
	}
	return result;
}
