#include "PeachDetector.hpp"
std::vector<cv::RotatedRect> RoboCup::PeachDetector::Detect(cv::InputArray InputBGR_Image, cv::InputArray InputHSV_FULL_Image, cv::OutputArray OutputFilledBinaryContours) noexcept
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

	Mat filterOutout;
	this->Detector::Filter(hsvImage, filterOutout);

	Mat erodeKernel = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));
	dilate(filterOutout, filterOutout, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
	erode(filterOutout, filterOutout, erodeKernel);
	dilate(filterOutout, filterOutout, erodeKernel);

	//imshow("erode", filterOutout);

	vector<vector<Point>> contours;
	findContours(filterOutout, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	vector<RotatedRect> result;

	for (unsigned int index = 0; index < contours.size(); index++)
	{
		float size = contourArea(contours[index]);
		cout << size << endl;
		if (size > 1300)
		{
			RotatedRect minRect = minAreaRect(Mat(contours[index]));
			Size2f rectSize = minRect.size;
			float lengthWidthRatio = (rectSize.width > rectSize.height) ? (rectSize.height / rectSize.width) : (rectSize.width / rectSize.height);
			cout << lengthWidthRatio << endl;
			if (lengthWidthRatio > 0.5)
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
