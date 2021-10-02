#include "KiwiFruitDetector.hpp"

std::vector<cv::RotatedRect> RoboCup::KiwiFruitDetector::Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image, cv::OutputArray OutputFilledBinaryContours) noexcept
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

	Mat filterOutout = Mat::zeros(InputBGR_Image.size(), CV_8UC1);
	for (auto filter : positiveFilters)
	{
		Mat positiveOutput;
		filter.FilterWithHSV_FULL_Image(InputHSV_FULL_Image, positiveOutput);
		bitwise_or(filterOutout, positiveOutput, filterOutout);
	}

	if (negativeFilters.size() > 0)
	{
		Mat negativeOutout = Mat::zeros(InputHSV_FULL_Image.size(), CV_8UC1);
		for (auto filter : negativeFilters)
		{
			Mat filterOutput;
			filter.FilterWithHSV_FULL_Image(InputHSV_FULL_Image, filterOutput);
			bitwise_or(filterOutput, negativeOutout, negativeOutout);
		}

		erode(negativeOutout, negativeOutout, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
		imshow("nega", negativeOutout);
		bitwise_not(negativeOutout, negativeOutout);
		bitwise_and(filterOutout, negativeOutout, filterOutout);
	}

	imshow("hsv", hsvImage);

	Mat erodeKernel = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));
	erode(filterOutout, filterOutout, erodeKernel);
	
	dilate(filterOutout, filterOutout, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	imshow("fout", filterOutout);

	vector<vector<Point>> contours;
	findContours(filterOutout, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	vector<RotatedRect> result;
	for (unsigned int index = 0; index < contours.size(); index++)
	{
		float size = contourArea(contours[index]);
		std::cout << size << endl;
		if (size > 1300)
		{
			RotatedRect minRect = minAreaRect(Mat(contours[index]));
			Size2f rectSize = minRect.size;
			float lengthWidthRatio = (rectSize.width > rectSize.height) ? (rectSize.height / rectSize.width) : (rectSize.width / rectSize.height);
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

	return result;
}
