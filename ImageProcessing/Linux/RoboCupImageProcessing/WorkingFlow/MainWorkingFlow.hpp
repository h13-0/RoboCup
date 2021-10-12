#pragma once
#include <condition_variable>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

#include "EnumMacros.h"

#include "WorkingFlow.hpp"
#include "Monitor.hpp"
#include "FruitDetector.hpp"
#include "Protocol.hpp"
#include "SerialProtocol.hpp"
namespace RoboCup
{
	DECLARE_ENUM(WorkingMode,
		StandBy,
		AppleDetectMax,
		AppleDetectLeft,
		AppleDetectRight,
		TargetDetect,
		FruitDetection
	);

	class MainWorkingFlow : public WorkingFlow
	{
	public:
		MainWorkingFlow(const Configs& Configs);

		~MainWorkingFlow() noexcept;

		int Run();

	private:
		/// <summary>
		/// Working method realize.
		/// </summary>
		/// <param name="InputOutputBGR_Image">Image in BGR.</param>
		void StandBy(cv::InputOutputArray InputOutputBGR_Image);
		void AppleDetectMax(cv::InputOutputArray InputOutputBGR_Image);
		void AppleDetectLeft(cv::InputOutputArray InputOutputBGR_Image);
		void AppleDetectRight(cv::InputOutputArray InputOutputBGR_Image);
		void TargetDetect(cv::InputOutputArray InputOutputBGR_Image);
		void FruitDetection(cv::InputOutputArray InputOutputBGR_Image);

		void ReportWorkingMode(const WorkingMode::WorkingMode& Mode);

		void SendAppleCoordinates(const float& X_Coordinates, const float& Y_Coordinates);

		void SendTargetCoordinates(const float& X_Coordinates, const float& Y_Coordinates);

		//void SendFruitDetectionResult();

		/// <summary>
		/// Base object.
		/// </summary>
		Monitor *monitor;
		h13::Protocol* protocol;
		cv::VideoCapture capture;
		FruitDetector fruitDetector;
		AppleDetector appleDetector;

		/// <summary>
		/// Base variables.
		/// </summary>
		WorkingMode::WorkingMode mode;
		bool exit;
		bool exited;
		std::condition_variable exitedConditionVariable;
		std::mutex exitFlagMutex;
	};
}