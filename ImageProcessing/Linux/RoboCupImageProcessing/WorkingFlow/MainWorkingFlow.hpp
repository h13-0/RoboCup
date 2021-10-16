#pragma once
#include <condition_variable>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

#include "EnumMacros.h"

#include "WorkingFlow.hpp"
#include "Monitor.hpp"
#include "FruitDetector.hpp"
#include "TargetDetector.hpp"
#include "Protocol.hpp"
#include "SerialProtocol.hpp"
namespace RoboCup
{
	DECLARE_ENUM(WorkingMode,
		StandBy,
		AppleDetectMax,
		AppleDetectLeft,
		AppleDetectRight,
		TargetDetection,
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
		void standBy(cv::InputOutputArray InputOutputBGR_Image);
		void appleDetectMax(cv::InputOutputArray InputOutputBGR_Image);
		void appleDetectLeft(cv::InputOutputArray InputOutputBGR_Image);
		void appleDetectRight(cv::InputOutputArray InputOutputBGR_Image);
		void targetDetect(cv::InputOutputArray InputOutputBGR_Image);
		void fruitDetection(cv::InputOutputArray InputOutputBGR_Image);

		void reportWorkingMode(const WorkingMode::WorkingMode& Mode);

		void sendAppleCoordinates(const float& X_Coordinates, const float& Y_Coordinates);

		void sendTargetCoordinates(const float& X_Coordinates, const float& Y_Coordinates);

		//void SendFruitDetectionResult();

		/// <summary>
		/// Base object.
		/// </summary>
		Monitor *monitor;
		h13::Protocol* protocol;
		cv::VideoCapture capture;
		FruitDetector fruitDetector;
		AppleDetector appleDetector;
		TargetDetector targetDetector;

		/// <summary>
		/// Base variables.
		/// </summary>
		WorkingMode::WorkingMode mode;
		std::mutex modeMutex;

		bool exit;
		bool exited;
		std::condition_variable exitedConditionVariable;
		std::mutex exitFlagMutex;
	};
}