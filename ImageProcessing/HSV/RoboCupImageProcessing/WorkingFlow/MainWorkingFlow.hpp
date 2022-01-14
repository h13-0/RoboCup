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
		FruitIdentify
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
		void FruitIdentify(cv::InputOutputArray InputOutputBGR_Image);

		void reportWorkingMode(const WorkingMode::WorkingMode& Mode);

		void sendAppleCoordinates(const float& X_Coordinates, const float& Y_Coordinates);

		void sendTargetCoordinates(const float& X_Coordinates, const float& Y_Coordinates);

		void sendFruitDetectResult(const std::vector<RoboCup::FruitDetectResult_t>& Results);

		/// <summary>
		/// Base object.
		/// </summary>
		Monitor *monitor;
		h13::SerialProtocol* protocol;
		cv::VideoCapture capture;
		FruitDetector fruitDetector;
		AppleDetector appleDetector;
		TargetDetector targetDetector;

		/// <summary>
		/// Base variables.
		/// </summary>
		WorkingMode::WorkingMode mode;
		std::mutex modeMutex;
		uint8_t FruitIdentifyFinished;
		std::mutex FruitIdentifyFinishedMutex;
		cv::Mat frame;
		std::mutex frameMutex;

		/// <summary>
		/// Variables for safe exit.
		/// </summary>
		bool exit;
		bool exited;
		std::condition_variable exitedConditionVariable;
		std::mutex exitFlagMutex;
	};
}