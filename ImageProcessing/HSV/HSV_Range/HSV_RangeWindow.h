#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HSV_RangeWindow.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

namespace HSV_Range
{
	typedef enum
	{
		RGB_Mode,
		HSV_Mode,
	} PreviewMode_t;

	typedef std::function<void(unsigned int)> SliderChangedCallback_t;
	typedef std::function<void(PreviewMode_t)> PreviewModeChangedCallback_t;

	class HSV_RangeWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		HSV_RangeWindow(QWidget* parent = Q_NULLPTR);

		void AddPreviewModeChangedCalback(PreviewModeChangedCallback_t& Callback)
		{
			std::lock_guard<std::mutex> lock(previewModeChangedCallbackMutex);
			addPreviewModeChangedCallbackFunction(Callback, previewModeChangedCallbacks);
		};

		void AddHueMaximumSliderCallback(SliderChangedCallback_t& Callback)
		{
			std::lock_guard<std::mutex> lock(hMaxSliderChangedCallbackMutex);
			addSliderChangedCallbackFunction(Callback, hMaxSliderChangedCallbacks);
		};

		void AddHueMinimumSliderCallback(SliderChangedCallback_t& Callback)
		{
			std::lock_guard<std::mutex> lock(hMinSliderChangedCallbackMutex);
			addSliderChangedCallbackFunction(Callback, hMinSliderChangedCallbacks);
		};

		void AddSaturationMaximumSliderCallback(SliderChangedCallback_t& Callback)
		{
			std::lock_guard<std::mutex> lock(sMaxSliderChangedCallbackMutex);
			addSliderChangedCallbackFunction(Callback, sMaxSliderChangedCallbacks);
		};

		void AddSaturationMinimumSliderCallback(SliderChangedCallback_t& Callback)
		{
			std::lock_guard<std::mutex> lock(sMinSliderChangedCallbackMutex);
			addSliderChangedCallbackFunction(Callback, sMinSliderChangedCallbacks);
		};

		void AddValueMaximumSliderCallback(SliderChangedCallback_t& Callback)
		{
			std::lock_guard<std::mutex> lock(vMaxSliderChangedCallbackMutex);
			addSliderChangedCallbackFunction(Callback, vMaxSliderChangedCallbacks);
		};

		void AddValueMinimumSliderCallback(SliderChangedCallback_t& Callback)
		{
			std::lock_guard<std::mutex> lock(vMinSliderChangedCallbackMutex);
			addSliderChangedCallbackFunction(Callback, vMinSliderChangedCallbacks);
		};

	private:
		void addPreviewModeChangedCallbackFunction(PreviewModeChangedCallback_t& Callback, std::vector<PreviewModeChangedCallback_t>& TargetCallbacks);
		void addSliderChangedCallbackFunction(SliderChangedCallback_t& Callback, std::vector<SliderChangedCallback_t>& TargetCallbacks);
		
		void executePreviewModeChangedCallback(const std::vector<PreviewModeChangedCallback_t>& Callbacks, PreviewMode_t NewMode);
		void executeSliderChangedCallback(const std::vector<SliderChangedCallback_t>& Callbacks, unsigned int CurrentValue);
		
		Ui::HSV_RangeClass ui;

		/// <summary>
		/// Callback functions and mutexes.
		/// </summary>
		std::vector<PreviewModeChangedCallback_t> previewModeChangedCallbacks;
		std::mutex previewModeChangedCallbackMutex;

		std::vector<SliderChangedCallback_t> hMaxSliderChangedCallbacks;
		std::mutex hMaxSliderChangedCallbackMutex;

		std::vector<SliderChangedCallback_t> hMinSliderChangedCallbacks;
		std::mutex hMinSliderChangedCallbackMutex;

		std::vector<SliderChangedCallback_t> sMaxSliderChangedCallbacks;
		std::mutex sMaxSliderChangedCallbackMutex;

		std::vector<SliderChangedCallback_t> sMinSliderChangedCallbacks;
		std::mutex sMinSliderChangedCallbackMutex;

		std::vector<SliderChangedCallback_t> vMaxSliderChangedCallbacks;
		std::mutex vMaxSliderChangedCallbackMutex;

		std::vector<SliderChangedCallback_t> vMinSliderChangedCallbacks;
		std::mutex vMinSliderChangedCallbackMutex;

	private slots:
		void refreshImageMethod(cv::Mat Image);

		void rgbPreviewRadioButtonChanged(bool Status)
		{
			if (Status)
			{
				std::lock_guard<std::mutex> lock(previewModeChangedCallbackMutex);
				executePreviewModeChangedCallback(previewModeChangedCallbacks, RGB_Mode);
			}
		};

		void hsvPreviewRadioButtonChanged(bool Status)
		{
			if (Status)
			{
				std::lock_guard<std::mutex> lock(previewModeChangedCallbackMutex);
				executePreviewModeChangedCallback(previewModeChangedCallbacks, HSV_Mode);
			}
		};

		void hueMaximumSliderChanged(unsigned int Value)
		{
			std::lock_guard<std::mutex> lock(hMaxSliderChangedCallbackMutex);
			executeSliderChangedCallback(hMaxSliderChangedCallbacks, Value);
		};

		void hueMinimumSliderChanged(unsigned int Value)
		{
			std::lock_guard<std::mutex> lock(hMinSliderChangedCallbackMutex);
			executeSliderChangedCallback(hMinSliderChangedCallbacks, Value);
		};

		void saturationMaximumSliderChanged(unsigned int Value)
		{
			std::lock_guard<std::mutex> lock(sMaxSliderChangedCallbackMutex);
			executeSliderChangedCallback(sMaxSliderChangedCallbacks, Value);
		};

		void saturationMinimumSliderChanged(unsigned int Value)
		{
			std::lock_guard<std::mutex> lock(sMinSliderChangedCallbackMutex);
			executeSliderChangedCallback(sMinSliderChangedCallbacks, Value);
		};

		void valueMaximumSliderChanged(unsigned int Value)
		{
			std::lock_guard<std::mutex> lock(vMaxSliderChangedCallbackMutex);
			executeSliderChangedCallback(vMaxSliderChangedCallbacks, Value);
		};

		void valueMinimumSliderChanged(unsigned int Value)
		{
			std::lock_guard<std::mutex> lock(vMinSliderChangedCallbackMutex);
			executeSliderChangedCallback(vMinSliderChangedCallbacks, Value);
		};

	signals:
		void RefreshImage(cv::Mat Image);
	};
}
