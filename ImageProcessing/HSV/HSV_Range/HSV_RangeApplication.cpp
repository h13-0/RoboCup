#include "HSV_RangeApplication.hpp"
#include <thread>
#include <chrono>

//Opencv
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

//HSV_Filter
#include "HSV_Filter.hpp"

h13::HSV_RangeApplication::HSV_RangeApplication(HSV_Range::HSV_RangeWindow& Window) : window(Window)
{
	using namespace HSV_Range;
	filter = HSV_Filter(0, 255, 0, 255, 0, 255);

	PreviewModeChangedCallback_t previewModeChangeCallback = [this](PreviewMode_t Mode) { mode = Mode; };
	window.AddPreviewModeChangedCalback(previewModeChangeCallback);

	SliderChangedCallback_t hMaxChangedCallback = [this](unsigned int Value) { hueMaximumChangedMethod(Value); };
	window.AddHueMaximumSliderCallback(hMaxChangedCallback);

	SliderChangedCallback_t hMinChangedCallback = [this](unsigned int Value) { hueMinimumChangedMethod(Value); };
	window.AddHueMinimumSliderCallback(hMinChangedCallback);

	SliderChangedCallback_t sMaxChangedCallback = [this](unsigned int Value) { saturationMaximumChangedMethod(Value); };
	window.AddSaturationMaximumSliderCallback(sMaxChangedCallback);

	SliderChangedCallback_t sMinChangedCallback = [this](unsigned int Value) { saturationMinimumChangedMethod(Value); };
	window.AddSaturationMinimumSliderCallback(sMinChangedCallback);

	SliderChangedCallback_t vMaxChangedCallback = [this](unsigned int Value) { valueMaximumChangedMethod(Value); };
	window.AddValueMaximumSliderCallback(vMaxChangedCallback);

	SliderChangedCallback_t vMinChangedCallback = [this](unsigned int Value) { valueMinimumChangedMethod(Value); };
	window.AddValueMinimumSliderCallback(vMinChangedCallback);

	ButtonClickedCallback_t openPhotoButtonClickedCallback = [this](void) { openPhotoButtonClickedMethod(); };
	window.AddOpenPhotoButtonClickedCallback(openPhotoButtonClickedCallback);

	ButtonClickedCallback_t openVideoButtonClickedCallback = [this](void) { openVideoButtonClickedMethod(); };
	window.AddOpenVideoButtonClickedCallback(openVideoButtonClickedCallback);
}

void h13::HSV_RangeApplication::Run(void)
{
	std::thread mainThread(&HSV_RangeApplication::mainMethod, this);
	mainThread.detach();
}

h13::HSV_RangeApplication::~HSV_RangeApplication()
{
	stopRefreshVideo();
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
	using namespace std;
	using namespace cv;

	while (true)
	{
		Mat _frame;
		{
			unique_lock<mutex> lck(refreshHSV_ConditionMutex);
			refreshHSV_Condition.wait(lck, [this]() {return refreshHSV_ConditionFlag; });
			refreshHSV_ConditionFlag = false;
			_frame = frame.clone();
		}
		
		if (!_frame.empty())
		{
			Mat outputFrame;
			outputFrameCalculate(_frame, outputFrame, mode);
			window.RefreshImage(outputFrame);
		}
		
		this_thread::sleep_for(chrono::milliseconds(10));
	}
}

void h13::HSV_RangeApplication::hueMaximumChangedMethod(unsigned int Value)
{
	using namespace std;
	filter.SetHueMaximumValue(Value);
	{
		unique_lock<mutex> lck(refreshHSV_ConditionMutex);
		refreshHSV_ConditionFlag = true;
		refreshHSV_Condition.notify_all();
	}
}

void h13::HSV_RangeApplication::hueMinimumChangedMethod(unsigned int Value)
{
	using namespace std;
	filter.SetHueMinimumValue(Value);
	{
		unique_lock<mutex> lck(refreshHSV_ConditionMutex);
		refreshHSV_ConditionFlag = true;
		refreshHSV_Condition.notify_all();
	}	
}

void h13::HSV_RangeApplication::saturationMaximumChangedMethod(unsigned int Value)
{
	using namespace std;
	filter.SetSaturationMaximumValue(Value);
	{
		unique_lock<mutex> lck(refreshHSV_ConditionMutex);
		refreshHSV_ConditionFlag = true;
		refreshHSV_Condition.notify_all();
	}
}

void h13::HSV_RangeApplication::saturationMinimumChangedMethod(unsigned int Value)
{
	using namespace std;
	filter.SetSaturationMinimumValue(Value);
	{
		unique_lock<mutex> lck(refreshHSV_ConditionMutex);
		refreshHSV_ConditionFlag = true;
		refreshHSV_Condition.notify_all();
	}
}

void h13::HSV_RangeApplication::valueMaximumChangedMethod(unsigned int Value)
{
	using namespace std;
	filter.SetValueMaximumValue(Value);
	{
		unique_lock<mutex> lck(refreshHSV_ConditionMutex);
		refreshHSV_ConditionFlag = true;
		refreshHSV_Condition.notify_all();
	}
}

void h13::HSV_RangeApplication::valueMinimumChangedMethod(unsigned int Value)
{
	using namespace std;
	filter.SetValueMinimumValue(Value);
	{
		unique_lock<mutex> lck(refreshHSV_ConditionMutex);
		refreshHSV_ConditionFlag = true;
		refreshHSV_Condition.notify_all();
	}	
}

void h13::HSV_RangeApplication::openPhotoButtonClickedMethod(void)
{
	using namespace std;
	using namespace cv;
	stopRefreshVideo();
	string fileName = window.GetPhotoPath();
	try {
		unique_lock<mutex> lck(refreshHSV_ConditionMutex);
		frame = imread(fileName);
		if (!frame.empty())
		{
			refreshHSV_ConditionFlag = true;
			refreshHSV_Condition.notify_all();
		}
	}
	catch (...)
	{
		
	}
}

void h13::HSV_RangeApplication::openVideoButtonClickedMethod(void)
{
	using namespace std;
	if (refreshVideoThread == nullptr)
	{
		{
			unique_lock<mutex> lck(refreshVideoMutex);
			refreshVideoExitFlag = false;
		}
		refreshVideoThread = new thread(&HSV_RangeApplication::refreshVideoMethod, this);
		window.SetOpenVideoButtonText("关闭视频流");
	}
	else {
		stopRefreshVideo();
	}
	
}

void h13::HSV_RangeApplication::refreshVideoMethod(void)
{
	using namespace std;
	bool exit = false;
	while (!exit)
	{
		// Check exit flag.
		{
			unique_lock<mutex> lck(refreshVideoMutex);
			{
				exit = refreshVideoExitFlag;
			}
		}
		
		// Refresh frame.
		if (!cap.isOpened())
		{
			// Check whether it's a camera or a video stream.
			auto cameraID = window.GetVideoPath();
			try {
				int id = stoi(cameraID);
				if (to_string(id) == cameraID)
				{
					cap.open(id);
				}
				else {
					cap.open(cameraID);
				}
			}
			catch (...) {
				cap.open(cameraID);
			}
		}
		else {
			unique_lock<mutex> lck(refreshHSV_ConditionMutex);
			if (cap.read(frame))
			{
				refreshHSV_ConditionFlag = true;
				refreshHSV_Condition.notify_all();
			}
		}
		
		this_thread::sleep_for(chrono::milliseconds(10));
	}

	{
		unique_lock<mutex> lck(refreshVideoMutex);
		if (cap.isOpened())
		{
			cap.release();
		}

		refreshVideoExitFlag = false;
	}

}

void h13::HSV_RangeApplication::stopRefreshVideo(void)
{
	using namespace std;
	if (refreshVideoThread != nullptr)
	{
		{
			unique_lock<mutex> lck(refreshVideoMutex);
			refreshVideoExitFlag = true;
		}
		refreshVideoThread->join();
		delete refreshVideoThread;
		refreshVideoThread = nullptr;
	}
	window.SetOpenVideoButtonText("打开视频流");
}
