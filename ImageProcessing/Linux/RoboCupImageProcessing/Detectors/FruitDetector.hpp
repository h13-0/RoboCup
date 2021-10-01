#pragma once
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "EnumMacros.h"

#include "HSV_Filter.hpp"

#include "AppleDetector.hpp"
#include "LemonDetector.hpp"
#include "OrangeDetector.hpp"
#include "BananaDetector.hpp"
#include "PitayaDetector.hpp"
#include "PeachDetector.hpp"

#include "SnowPearDetector.hpp"
#include "PearDetector.hpp"

namespace RoboCup
{
	DECLARE_ENUM(Fruit_t, 
		Apple,
		Banana,
		KiwiFruit,
		Lemon,
		Orange,
		Peach,
		Pear,
		Pitaya,
		SnowPear
	);

	typedef struct Result
	{
		Result(RoboCup::Fruit_t FruitType, cv::RotatedRect Rect)
			: FruitType(FruitType), Rect(Rect){ };
		RoboCup::Fruit_t FruitType;
		cv::RotatedRect Rect;
	} Result_t;

	class FruitDetector
	{
	public:
		FruitDetector();
		~FruitDetector() { };

		std::vector<RoboCup::Result_t> Detect(cv::InputArray InputBGR_Image) noexcept;

	private:
		static void maskXOR_Operation(cv::InputArray InputImage, cv::InputArray InputMask, cv::OutputArray OutputImage);

		RoboCup::AppleDetector appleDetector;
		RoboCup::LemonDetector lemonDetector;
		RoboCup::OrangeDetector orangeDetector;
		RoboCup::BananaDetector bananaDetector;
		RoboCup::PitayaDetector pitayaDetector;
		RoboCup::PeachDetector peachDetector;
		RoboCup::SnowPearDetector snowPearDetector;
		RoboCup::PearDetector pearDetector;
		/*
		LemonDetector lemonDetector;
		OrangeDetector orangeDetector;
		
		
		RoboCup::KiwiFruitDetector kiwiFruitDetector;
		
		SnowPearDetector snowPearDetector;
		*/
	};
}