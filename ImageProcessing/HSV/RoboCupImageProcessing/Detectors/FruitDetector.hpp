#pragma once
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

#include "EnumMacros.h"
#include "HSV_Filter.hpp"
#include "FruitDetectorConfigs.hpp"

#include "AppleDetector.hpp"
#include "LemonDetector.hpp"
#include "OrangeDetector.hpp"
#include "BananaDetector.hpp"
#include "PitayaDetector.hpp"
#include "PeachDetector.hpp"
#include "SnowPearDetector.hpp"
#include "KiwiFruitDetector.hpp"
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
		Result(RoboCup::Fruit_t::Fruit_t FruitType, cv::RotatedRect Rect)
			: FruitType(FruitType), Rect(Rect){ };
		RoboCup::Fruit_t::Fruit_t FruitType;
		cv::RotatedRect Rect;
	} FruitDetectResult_t;

	class FruitDetector
	{
	public:
		/// <summary>
		/// Construct function.
		/// </summary>
		/// <param name="configs">Configs.</param>
		FruitDetector(const FruitDetectorConfigs& configs) noexcept;

		~FruitDetector() noexcept { };

		/// <summary>
		/// Detect fruit in BGR Image.
		/// </summary>
		/// <param name="Input">BGR Image.</param>
		/// <note>Identification sequence:
		/// Apple->Lemon->Orange->Banana->Pitaya
		///		->Peach->SnowPear->Pear->KiwiFruit
		/// (Confidence ranking from high to low.) </note>
		/// <returns>Result in std::vector<RoboCup::Result_t>.</returns>
		std::vector<RoboCup::FruitDetectResult_t> Detect(cv::InputArray InputBGR_Image) noexcept;

	private:
		static void maskXOR_Operation(cv::InputArray InputImage, cv::InputArray InputMask, cv::OutputArray OutputImage);
		static bool rectSizeCompare(const cv::RotatedRect& Rect1, const cv::RotatedRect& Rect2);
		std::vector<RoboCup::FruitDetectResult_t> checkFruitDetectionResult(const std::vector<RoboCup::FruitDetectResult_t>& Results);

		FruitDetectorConfigs configs;

		/// <summary>
		/// Detectors.
		/// </summary>
		RoboCup::AppleDetector appleDetector;
		RoboCup::LemonDetector lemonDetector;
		RoboCup::OrangeDetector orangeDetector;
		RoboCup::BananaDetector bananaDetector;
		RoboCup::PitayaDetector pitayaDetector;
		RoboCup::PeachDetector peachDetector;
		RoboCup::KiwiFruitDetector kiwiFruitDetector;
		RoboCup::SnowPearDetector snowPearDetector;
		RoboCup::PearDetector pearDetector;
	};
}
