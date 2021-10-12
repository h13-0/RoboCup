#include "PearDetector.hpp"
std::vector<cv::RotatedRect> RoboCup::PearDetector::Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image, cv::OutputArray OutputFilledBinaryContours) noexcept
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

	Mat erodeKernel = getStructuringElement(MORPH_ELLIPSE, Size(erodeKernelSize, erodeKernelSize));
	Mat positiveOutput = Mat::zeros(InputBGR_Image.size(), CV_8UC1);

	for (auto filter : positiveFilters)
	{
		Mat filterOutput;
		filter.FilterWithHSV_FULL_Image(hsvImage, filterOutput);
		erode(filterOutput, filterOutput, erodeKernel);
		dilate(filterOutput, filterOutput, erodeKernel);
		bitwise_or(filterOutput, positiveOutput, positiveOutput);
	}

	vector<RotatedRect> result;
	vector<vector<Point>> contours;

	findContours(positiveOutput, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
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
				result.push_back(minRect);
				if (outputContoursRequired)
				{
					drawContours(outputContours, contours, index, Scalar(255), -1);
				}
			}
		}
	}

	return result;
}
