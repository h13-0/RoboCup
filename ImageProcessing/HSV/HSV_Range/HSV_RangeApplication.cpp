#include "HSV_RangeApplication.hpp"
#include <thread>

//Opencv
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

//HSV_Filter
#include "HSV_Filter.hpp"

h13::HSV_RangeApplication::HSV_RangeApplication(HSV_Range::HSV_RangeWindow& Window) : window(Window)
{
	using namespace HSV_Range;
	filter = HSV_Filter(0, 255, 0, 255, 0, 255);

	PreviewModeChangedCallback_t previewModeChangeCallback = [this](PreviewMode_t Mode) {mode = Mode; };
	window.AddPreviewModeChangedCalback(previewModeChangeCallback);

	SliderChangedCallback_t hMaxChangedCallback = [this](unsigned int Value) {hueMaximumChangedMethod(Value); };
	window.AddHueMaximumSliderCallback(hMaxChangedCallback);

	SliderChangedCallback_t hMinChangedCallback = [this](unsigned int Value) {hueMinimumChangedMethod(Value); };
	window.AddHueMinimumSliderCallback(hMinChangedCallback);

	SliderChangedCallback_t sMaxChangedCallback = [this](unsigned int Value) {saturationMaximumChangedMethod(Value); };
	window.AddSaturationMaximumSliderCallback(sMaxChangedCallback);

	SliderChangedCallback_t sMinChangedCallback = [this](unsigned int Value) {saturationMinimumChangedMethod(Value); };
	window.AddSaturationMinimumSliderCallback(sMinChangedCallback);

	SliderChangedCallback_t vMaxChangedCallback = [this](unsigned int Value) {valueMaximumChangedMethod(Value); };
	window.AddValueMaximumSliderCallback(vMaxChangedCallback);

	SliderChangedCallback_t vMinChangedCallback = [this](unsigned int Value) {valueMinimumChangedMethod(Value); };
	window.AddValueMinimumSliderCallback(vMinChangedCallback);
}

void h13::HSV_RangeApplication::Run(void)
{
	std::thread mainThread(&HSV_RangeApplication::mainMethod, this);
	mainThread.detach();
}

void h13::HSV_RangeApplication::outputFrameCalculate(cv::InputArray Input, cv::OutputArray Output, HSV_Range::PreviewMode_t Mode)
{
	using namespace HSV_Range;
	using namespace cv;

	Mat input = Input.getMat();
	Mat rangeMask;

	filter.Filter(input, rangeMask);

	Mat hsvImage;
	std::vector<Mat> channels;
	switch (Mode)
	{
	case RGB_Mode:
		split(input, channels);
		bitwise_and(channels[0], rangeMask, channels[0]);
		bitwise_and(channels[1], rangeMask, channels[1]);
		bitwise_and(channels[2], rangeMask, channels[2]);
		merge(channels, Output);
		break;

	case HSV_Mode:
		filter.ConvertToHSV(Input, hsvImage);
		split(hsvImage, channels);
		bitwise_and(channels[0], rangeMask, channels[0]);
		bitwise_and(channels[1], rangeMask, channels[1]);
		bitwise_and(channels[2], rangeMask, channels[2]);
		merge(channels, Output);
		break;

	default:
		break;
	}
}

void h13::HSV_RangeApplication::mainMethod(void)
{
	using namespace cv;
	//VideoCapture cap("C:/Users/h13/Pictures/Camera Roll/WIN_20210920_17_04_33_Pro.mp4");
	VideoCapture cap(0);
	while (cap.isOpened())
	{
		Mat frame;
		while (cap.read(frame))
		{
			Mat outputFrame;
			outputFrameCalculate(frame, outputFrame, mode);
			window.RefreshImage(outputFrame);
		}

		//cap = VideoCapture("C:/Users/h13/Pictures/Camera Roll/WIN_20210920_17_04_33_Pro.mp4");
	}
}

void h13::HSV_RangeApplication::hueMaximumChangedMethod(unsigned int Value)
{
	filter.SetHueMaximumValue(Value);
}

void h13::HSV_RangeApplication::hueMinimumChangedMethod(unsigned int Value)
{
	filter.SetHueMinimumValue(Value);
}

void h13::HSV_RangeApplication::saturationMaximumChangedMethod(unsigned int Value)
{
	filter.SetSaturationMaximumValue(Value);
}

void h13::HSV_RangeApplication::saturationMinimumChangedMethod(unsigned int Value)
{
	filter.SetSaturationMinimumValue(Value);
}

void h13::HSV_RangeApplication::valueMaximumChangedMethod(unsigned int Value)
{
	filter.SetValueMaximumValue(Value);
}

void h13::HSV_RangeApplication::valueMinimumChangedMethod(unsigned int Value)
{
	filter.SetValueMinimumValue(Value);
}
