#pragma once
#include <condition_variable>
#include <mutex>

#include "HSV_RangeWindow.h"
#include "HSV_Filter.hpp"

namespace h13
{
	class HSV_RangeApplication
	{
	public:
		HSV_RangeApplication(HSV_Range::HSV_RangeWindow &Window);

		void Run(void);

		~HSV_RangeApplication();

	private:
		void outputFrameCalculate(cv::InputArray Input, cv::OutputArray Output, HSV_Range::PreviewMode_t Mode);

		/// <summary>
		/// 
		/// </summary>
		/// <param name=""></param>
		void mainMethod(void);

		/// <summary>
		/// Slider changed callback functions.
		/// </summary>
		/// <param name="Value"></param>
		void hueMaximumChangedMethod(unsigned int Value);
		void hueMinimumChangedMethod(unsigned int Value);

		void saturationMaximumChangedMethod(unsigned int Value);
		void saturationMinimumChangedMethod(unsigned int Value);

		void valueMaximumChangedMethod(unsigned int Value);
		void valueMinimumChangedMethod(unsigned int Value);

		void openPhotoButtonClickedMethod(void);
		void openVideoButtonClickedMethod(void);
		
		HSV_Range::PreviewMode_t mode = HSV_Range::RGB_Mode;

		HSV_Filter filter;
		HSV_Range::HSV_RangeWindow &window;

		
		
		
		/// <summary>
		/// Refresh HSV condition_variable.
		/// </summary>
		std::condition_variable refreshHSV_Condition;
		std::mutex refreshHSV_ConditionMutex;
		bool refreshHSV_ConditionFlag = false;
		cv::Mat frame;

		/// <summary>
		/// Refresh video thread variables.
		/// </summary>
		std::thread* refreshVideoThread = nullptr;
		std::mutex refreshVideoMutex;
		bool refreshVideoExitFlag = false;
		cv::VideoCapture cap;
		void refreshVideoMethod(void);
		void stopRefreshVideo(void);
	};
}
