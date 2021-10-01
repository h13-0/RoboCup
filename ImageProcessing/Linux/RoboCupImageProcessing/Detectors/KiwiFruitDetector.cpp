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

	Mat filterOutout;
	this->Detector::Filter(hsvImage, filterOutout);

	Mat erodeKernel = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));
	erode(filterOutout, filterOutout, erodeKernel);
	//imshow("erode", filterOutout);
	
	dilate(filterOutout, filterOutout, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	//imshow("dilate", filterOutout);

	return std::vector<cv::RotatedRect>();
}
