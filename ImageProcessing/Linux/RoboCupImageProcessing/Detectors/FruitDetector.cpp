#include "FruitDetector.hpp"
#include <algorithm>

#include "HSV_Filter.hpp"

//Base detectors.
#include "AppleDetector.hpp"   //95
#include "LemonDetector.hpp"   //95
#include "OrangeDetector.hpp"  //95
#include "BananaDetector.hpp"  //95
#include "PitayaDetector.hpp"  //95

//Advance detectors.
#include "PearDetector.hpp"  //
#include "PeachDetector.hpp" //90, there is a problem with the white side.
#include "SnowPearDetector.hpp" //90
#include "KiwiFruitDetector.hpp"

RoboCup::FruitDetector::FruitDetector(const FruitDetectorConfigs& configs) noexcept
	: configs(configs)
{ 
	if (configs.IsAppleDetectorEnabled())
	{
		appleDetector = AppleDetector(configs.GetAppleDetectorConfigs());
	}

	if (configs.IsBananaDetectorEnabled())
	{
		bananaDetector = BananaDetector(configs.GetBananaDetectorConfigs());
	}

	if (configs.IsKiwiFruitDetectorEnabled())
	{
		kiwiFruitDetector = KiwiFruitDetector(configs.GetKiwiFruitDetectorConfigs());
	}

	if (configs.IsLemonDetectorEnabled())
	{
		lemonDetector = LemonDetector(configs.GetLemonDetectorConfigs());
	}

	if (configs.IsOrangeDetectorEnabled())
	{
		orangeDetector = OrangeDetector(configs.GetOrangeDetectorConfigs());
	}

	if (configs.IsPeachDetectorEnabled())
	{
		peachDetector = PeachDetector(configs.GetPeachDetectorConfigs());
	}

	if (configs.IsPearDetectorEnabled())
	{
		pearDetector = PearDetector(configs.GetPearDetectorConfigs());
	}

	if (configs.IsPitayaDetectorEnabled())
	{
		pitayaDetector = PitayaDetector(configs.GetPitayaDetectorConfigs());
	}

	if (configs.IsSnowPearDetectorEnabled())
	{
		snowPearDetector = SnowPearDetector(configs.GetSnowPearDetectorConfigs());
	}
}


/// <summary>
/// Detect fruit in BGR Image.
/// </summary>
/// <param name="Input">BGR Image.</param>
/// <note>Identification sequence:
/// Apple->Lemon->Orange->Orange->Banana->Pitaya
///		->Peach->SnowPear->Pear->KiwiFruit
/// (Confidence ranking from high to low.) </note>
/// <returns>Result in std::vector<RoboCup::Result_t>.</returns>
std::vector<RoboCup::Result_t> RoboCup::FruitDetector::Detect(cv::InputArray InputBGR_Image) noexcept
{
	/**
	 * @note: Identification sequence:
	 * Apple -> Lemon -> -> Banana -> Pitaya
	 *		-> Peach -> SnowPear -> KiwiFruit -> Pear
	 * (Confidence ranking from high to low.)
	 **/

	using namespace cv;
	using namespace RoboCup;
	using namespace h13;

	CV_Assert(!InputBGR_Image.empty());
	Mat mask = Mat::zeros(InputBGR_Image.size(), CV_8UC1);
	Mat bgrImage = InputBGR_Image.getMat().clone();

	std::vector<cv::RotatedRect> subdetectorResults;
	std::vector<RoboCup::Result_t> finalResults;

	//Step 0: Adjust resolution.


	//Step 1: Convert to HSV_FULL.
	Mat hsvFull_Image;
	cvtColor(bgrImage, hsvFull_Image, COLOR_BGR2HSV_FULL);
	
	//Step 2: Detect Apples.
	if (configs.IsAppleDetectorEnabled())
	{
		subdetectorResults = appleDetector.Detect(bgrImage, hsvFull_Image, mask);
		dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(15, 15)));
		maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
		maskXOR_Operation(bgrImage, mask, bgrImage);
		for (auto& result : subdetectorResults)
		{
			finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::Apple, result));
		}
	}
	
	//Step 3: Detect Lemon.
	if (configs.IsLemonDetectorEnabled())
	{
		subdetectorResults = lemonDetector.Detect(bgrImage, hsvFull_Image, mask);
		dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(20, 20)));
		maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
		maskXOR_Operation(bgrImage, mask, bgrImage);
		for (auto& result : subdetectorResults)
		{
			finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::Lemon, result));
		}
	}

	//Step 4: Detect Orange.
	if (configs.IsOrangeDetectorEnabled())
	{
		subdetectorResults = orangeDetector.Detect(bgrImage, hsvFull_Image, mask);
		dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(15, 15)));
		maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
		maskXOR_Operation(bgrImage, mask, bgrImage);
		for (auto& result : subdetectorResults)
		{
			finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::Orange, result));
		}
	}

	//Step 5: Detect Banana.
	if (configs.IsBananaDetectorEnabled())
	{
		subdetectorResults = bananaDetector.Detect(bgrImage, hsvFull_Image, mask);
		dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
		maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
		maskXOR_Operation(bgrImage, mask, bgrImage);
		for (auto& result : subdetectorResults)
		{
			finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::Banana, result));
		}
	}

	//Step 6: Detect Pitaya.
	if (configs.IsPitayaDetectorEnabled())
	{
		subdetectorResults = pitayaDetector.Detect(bgrImage, hsvFull_Image, mask);
		dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
		maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
		maskXOR_Operation(bgrImage, mask, bgrImage);
		for (auto& result : subdetectorResults)
		{
			finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::Pitaya, result));
		}
	}

	//Step 7: Detect Peach.
	if (configs.IsPeachDetectorEnabled())
	{
		subdetectorResults = peachDetector.Detect(bgrImage, hsvFull_Image, mask);
		dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
		maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
		maskXOR_Operation(bgrImage, mask, bgrImage);
		for (auto& result : subdetectorResults)
		{
			finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::Peach, result));
		}
	}

	//Step 8: Detect SnowPear.
	if (configs.IsSnowPearDetectorEnabled())
	{
		subdetectorResults = snowPearDetector.Detect(bgrImage, hsvFull_Image, mask);
		dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
		maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
		maskXOR_Operation(bgrImage, mask, bgrImage);
		for (auto& result : subdetectorResults)
		{
			finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::SnowPear, result));
		}
	}
	
	//Step 9: Detect Pear.
	if (configs.IsPearDetectorEnabled())
	{
		subdetectorResults = pearDetector.Detect(bgrImage, hsvFull_Image, mask);
		dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
		maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
		maskXOR_Operation(bgrImage, mask, bgrImage);
		for (auto& result : subdetectorResults)
		{
			finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::Pear, result));
		}
	}

	//Step 10: Detect KiwiFruit.
	if (configs.IsKiwiFruitDetectorEnabled())
	{
		subdetectorResults = kiwiFruitDetector.Detect(bgrImage, hsvFull_Image, mask);
		dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
		maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
		maskXOR_Operation(bgrImage, mask, bgrImage);
		for (auto& result : subdetectorResults)
		{
			finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::KiwiFruit, result));
		}
	}

	return checkFruitDetectionResult(finalResults);
}

void RoboCup::FruitDetector::maskXOR_Operation(cv::InputArray InputImage, cv::InputArray InputMask, cv::OutputArray OutputImage)
{
	using namespace cv;
	CV_Assert(InputImage.size() == InputMask.size());
	Mat negativeMask;
	bitwise_not(InputMask, negativeMask);
	Mat *channels = new Mat[InputImage.channels()];
	split(InputImage.getMat(), channels);

	for (int index = 0; index < InputImage.channels(); index++)
	{
		bitwise_and(*(channels + index), negativeMask, *(channels + index));
	}

	merge(channels, InputImage.channels(), OutputImage);
	delete[] channels;
}

bool RoboCup::FruitDetector::rectSizeCompare(const cv::RotatedRect& Rect1, const cv::RotatedRect& Rect2)
{
	return(Rect1.size.area() > Rect2.size.area())? true : false;
}

std::vector<RoboCup::Result_t> RoboCup::FruitDetector::checkFruitDetectionResult(const std::vector<RoboCup::Result_t>& Results)
{
	using namespace std;
	using namespace cv;

	//Unpack results.
	vector<RotatedRect> appleResults, bananaResults, kiwiFruitResults, lemonResults
		, orangeResults, peachResults, pearResults, pitayaResults, snowPearResults;

	for (auto result : Results)
	{
		switch (result.FruitType)
		{
		case Fruit_t::Apple:
			appleResults.push_back(result.Rect);
			break;
		case Fruit_t::Banana:
			bananaResults.push_back(result.Rect);
			break;

		case Fruit_t::KiwiFruit:
			kiwiFruitResults.push_back(result.Rect);
			break;

		case Fruit_t::Lemon:
			lemonResults.push_back(result.Rect);
			break;

		case Fruit_t::Orange:
			orangeResults.push_back(result.Rect);
			break;

		case Fruit_t::Peach:
			peachResults.push_back(result.Rect);
			break;

		case Fruit_t::Pear:
			pearResults.push_back(result.Rect);
			break;

		case Fruit_t::Pitaya:
			pitayaResults.push_back(result.Rect);
			break;

		case Fruit_t::SnowPear:
			snowPearResults.push_back(result.Rect);
			break;

		default:
			break;
		}
	}

	//Troubleshooting error results.


	//Check quantity limit and pack results.
	std::vector<RoboCup::Result_t> results;
	int resultSize = 0;
	int quantityLimit = 0;

	resultSize = appleResults.size();
	quantityLimit = configs.GetAppleDetectorQuantityLimit();
	if (resultSize > quantityLimit)
	{
		std::sort(appleResults.begin(), appleResults.end(), &RoboCup::FruitDetector::rectSizeCompare);
		for (int i = 0; i < quantityLimit; i++)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::Apple, appleResults[i]));
		}
	}
	else {
		for (auto result : appleResults)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::Apple, result));
		}
	}

	resultSize = bananaResults.size();
	quantityLimit = configs.GetBananaDetectorQuantityLimit();
	if (resultSize > quantityLimit)
	{
		std::sort(bananaResults.begin(), bananaResults.end(), &RoboCup::FruitDetector::rectSizeCompare);
		for (int i = 0; i < quantityLimit; i++)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::Banana, bananaResults[i]));
		}
	}
	else {
		for (auto result : bananaResults)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::Banana, result));
		}
	}

	resultSize = kiwiFruitResults.size();
	quantityLimit = configs.GetKiwiFruitDetectorQuantityLimit();
	if (resultSize > quantityLimit)
	{
		std::sort(kiwiFruitResults.begin(), kiwiFruitResults.end(), &RoboCup::FruitDetector::rectSizeCompare);
		for (int i = 0; i < quantityLimit; i++)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::KiwiFruit, kiwiFruitResults[i]));
		}
	}
	else {
		for (auto result : kiwiFruitResults)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::KiwiFruit, result));
		}
	}

	resultSize = lemonResults.size();
	quantityLimit = configs.GetLemonDetectorQuantityLimit();
	if (resultSize > quantityLimit)
	{
		std::sort(lemonResults.begin(), lemonResults.end(), &RoboCup::FruitDetector::rectSizeCompare);
		for (int i = 0; i < quantityLimit; i++)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::Lemon, lemonResults[i]));
		}
	}
	else {
		for (auto result : lemonResults)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::Lemon, result));
		}
	}

	resultSize = orangeResults.size();
	quantityLimit = configs.GetOrangeDetectorQuantityLimit();
	if (resultSize > quantityLimit)
	{
		std::sort(orangeResults.begin(), orangeResults.end(), &RoboCup::FruitDetector::rectSizeCompare);
		for (int i = 0; i < quantityLimit; i++)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::Orange, orangeResults[i]));
		}
	}
	else {
		for (auto result : orangeResults)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::Orange, result));
		}
	}

	resultSize = peachResults.size();
	quantityLimit = configs.GetPeachDetectorQuantityLimit();
	if (resultSize > quantityLimit)
	{
		std::sort(peachResults.begin(), peachResults.end(), &RoboCup::FruitDetector::rectSizeCompare);
		for (int i = 0; i < quantityLimit; i++)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::Peach, peachResults[i]));
		}
	}
	else {
		for (auto result : peachResults)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::Peach, result));
		}
	}

	resultSize = pearResults.size();
	quantityLimit = configs.GetPearDetectorQuantityLimit();
	if (resultSize > quantityLimit)
	{
		std::sort(pearResults.begin(), pearResults.end(), &RoboCup::FruitDetector::rectSizeCompare);
		for (int i = 0; i < quantityLimit; i++)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::Pear, pearResults[i]));
		}
	}
	else {
		for (auto result : pearResults)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::Pear, result));
		}
	}

	resultSize = pitayaResults.size();
	quantityLimit = configs.GetPitayaDetectorQuantityLimit();
	if (resultSize > quantityLimit)
	{
		std::sort(pitayaResults.begin(), pitayaResults.end(), &RoboCup::FruitDetector::rectSizeCompare);
		for (int i = 0; i < quantityLimit; i++)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::Pitaya, pitayaResults[i]));
		}
	}
	else {
		for (auto result : pitayaResults)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::Pitaya, result));
		}
	}

	resultSize = snowPearResults.size();
	quantityLimit = configs.GetSnowPearDetectorQuantityLimit();
	if (resultSize > quantityLimit)
	{
		std::sort(snowPearResults.begin(), snowPearResults.end(), &RoboCup::FruitDetector::rectSizeCompare);
		for (int i = 0; i < quantityLimit; i++)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::SnowPear, snowPearResults[i]));
		}
	}
	else {
		for (auto result : snowPearResults)
		{
			results.push_back(RoboCup::Result(RoboCup::Fruit_t::SnowPear, result));
		}
	}

	return results;
}
