#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
//#include "AppleDetectorConfigs.hpp"
#include "Detector.hpp"

namespace RoboCup
{
	class TargetDetector : public Detector
	{
	public:
		TargetDetector();
		~TargetDetector();

	private:

	};

	TargetDetector::TargetDetector()
	{
	}

	TargetDetector::~TargetDetector()
	{
	}
}
