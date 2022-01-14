#include "OrangeDetector.hpp"

std::vector<cv::RotatedRect> RoboCup::OrangeDetector::Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image, cv::OutputArray OutputFilledBinaryContours) noexcept
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

	Mat filterOutout;
	Filter(hsvImage, filterOutout);

	Mat erodeKernel = getStructuringElement(MORPH_ELLIPSE, Size(erodeKernelSize, erodeKernelSize));
	erode(filterOutout, filterOutout, erodeKernel);

	vector<vector<Point>> contours;
	findContours(filterOutout, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	vector<RotatedRect> result;
	for (unsigned int index = 0; index < contours.size(); index++)
	{
		float size = contourArea(contours[index]);
		if (size > minimumSize)
		{
			RotatedRect minRect = minAreaRect(Mat(contours[index]));
			result.push_back(minRect);
			if (outputContoursRequired)
			{
				drawContours(outputContours, contours, index, Scalar(255), CV_FILLED);
			}
		}
	}

	return result;
}
