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
	typedef std::function<void(void)> ButtonClickedCallback_t;

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

		void AddOpenPhotoButtonClickedCallback(ButtonClickedCallback_t& Callback)
		{
			std::lock_guard<std::mutex> lock(openPhotoButtonClickedCallbackMutex);
			addButtonClickedCallbackFunction(Callback, openPhotoButtonClickedCallbacks);
		};

		void AddOpenVideoButtonClickedCallback(ButtonClickedCallback_t& Callback)
		{
			std::lock_guard<std::mutex> lock(openVideoButtonClickedCallbackMutex);
			addButtonClickedCallbackFunction(Callback, openVideoButtonClickedCallbacks);
		};

		void AddResetThresholdsButtonCallback(ButtonClickedCallback_t& Callback)
		{
			std::lock_guard<std::mutex> lock(resetThresholdsButtonClickedCallbackMutex);
			addButtonClickedCallbackFunction(Callback, resetThresholdsButtonClickedCallbacks);
		};

		std::string GetPhotoPath(void)
		{
			return getLineEditTextMethod(this->ui.PhotoPath);
		};

		std::string GetVideoPath(void)
		{
			return getLineEditTextMethod(this->ui.VideoPath);
		};

	private:
		void addPreviewModeChangedCallbackFunction(PreviewModeChangedCallback_t& Callback, std::vector<PreviewModeChangedCallback_t>& TargetCallbacks);
		void addSliderChangedCallbackFunction(SliderChangedCallback_t& Callback, std::vector<SliderChangedCallback_t>& TargetCallbacks);
		void addButtonClickedCallbackFunction(ButtonClickedCallback_t& Callback, std::vector<ButtonClickedCallback_t>& TargetCallbacks);

		void executePreviewModeChangedCallback(const std::vector<PreviewModeChangedCallback_t>& Callbacks, PreviewMode_t NewMode);
		void executeSliderChangedCallback(const std::vector<SliderChangedCallback_t>& Callbacks, unsigned int CurrentValue);
		void executeButtonClickedCallback(const std::vector<ButtonClickedCallback_t>& Callbacks);

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

		std::vector<ButtonClickedCallback_t> openPhotoButtonClickedCallbacks;
		std::mutex openPhotoButtonClickedCallbackMutex;
		
		std::vector<ButtonClickedCallback_t> openVideoButtonClickedCallbacks;
		std::mutex openVideoButtonClickedCallbackMutex;
		
		std::vector<ButtonClickedCallback_t> resetThresholdsButtonClickedCallbacks;
		std::mutex resetThresholdsButtonClickedCallbackMutex;

	private slots:
		void refreshImageMethod(cv::Mat Image);
		std::string getLineEditTextMethod(QLineEdit* LineEdit);

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

		void openPhotoButtonClicked()
		{
			std::lock_guard<std::mutex> lock(openPhotoButtonClickedCallbackMutex);
			executeButtonClickedCallback(openPhotoButtonClickedCallbacks);
		};

		void openVideoButtonClicked()
		{
			std::lock_guard<std::mutex> lock(openVideoButtonClickedCallbackMutex);
			executeButtonClickedCallback(openVideoButtonClickedCallbacks);
		};
		
		void resetThresholdsButtonClicked()
		{
			std::lock_guard<std::mutex> lock(resetThresholdsButtonClickedCallbackMutex);
			executeButtonClickedCallback(resetThresholdsButtonClickedCallbacks);
		};
		
	signals:
		void RefreshImage(cv::Mat Image);
		std::string GetLineEditText(QLineEdit* LineEdit);
		void SetOpenVideoButtonText(std::string Text);
	};
}
