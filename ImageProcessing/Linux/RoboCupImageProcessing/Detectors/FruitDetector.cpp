#include "FruitDetector.hpp"
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

/// <summary>
/// Detect fruit in BGR Image.
/// </summary>
/// <param name="Input">BGR Image.</param>
/// <note>Identification sequence:
/// Apple->Lemon->Orange->Orange->Banana->Pitaya
///		->Peach->SnowPear->KiwiFruit->Pear
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
	Mat mask = Mat::ones(InputBGR_Image.size(), CV_8UC1);
	Mat bgrImage = InputBGR_Image.getMat().clone();

	std::vector<cv::RotatedRect> subdetectorResults;
	std::vector<RoboCup::Result_t> finalResults;

	//Step 0: Adjust resolution.


	//Step 1: Convert to HSV_FULL.
	Mat hsvFull_Image;
	cvtColor(bgrImage, hsvFull_Image, COLOR_BGR2HSV_FULL);
	
	//Step 2: Detect Apples.
	subdetectorResults = appleDetector.Detect(bgrImage, hsvFull_Image, mask);
	dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(15, 15)));
	maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
	maskXOR_Operation(bgrImage, mask, bgrImage);
	for (auto &result : subdetectorResults)
	{
		finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::Apple, result));
	}
	
	//Step 3: Detect Lemon.
	subdetectorResults = lemonDetector.Detect(bgrImage, hsvFull_Image, mask);
	dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(20, 20)));
	maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
	maskXOR_Operation(bgrImage, mask, bgrImage);
	for (auto &result : subdetectorResults)
	{
		finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::Lemon, result));
	}

	//Step 4: Detect Orange.
	subdetectorResults = orangeDetector.Detect(bgrImage, hsvFull_Image, mask);
	dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(15, 15)));
	maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
	maskXOR_Operation(bgrImage, mask, bgrImage);
	for (auto &result : subdetectorResults)
	{
		finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::Orange, result));
	}

	//Step 5: Detect Banana.
	subdetectorResults = bananaDetector.Detect(bgrImage, hsvFull_Image, mask);
	dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
	maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
	maskXOR_Operation(bgrImage, mask, bgrImage);
	for (auto &result : subdetectorResults)
	{
		finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::Banana, result));
	}

	//Step 6: Detect Pitaya.
	subdetectorResults = pitayaDetector.Detect(bgrImage, hsvFull_Image, mask);
	dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
	maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
	maskXOR_Operation(bgrImage, mask, bgrImage);
	for (auto &result : subdetectorResults)
	{
		finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::Pitaya, result));
	}

	//Step 7: Detect Peach.
	subdetectorResults = peachDetector.Detect(bgrImage, hsvFull_Image, mask);
	dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
	maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
	maskXOR_Operation(bgrImage, mask, bgrImage);
	for (auto &result : subdetectorResults)
	{
		finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::Peach, result));
	}

	//Step 8: Detect SnowPear.
	subdetectorResults = snowPearDetector.Detect(bgrImage, hsvFull_Image, mask);
	dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
	maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
	maskXOR_Operation(bgrImage, mask, bgrImage);
	for (auto& result : subdetectorResults)
	{
		finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::SnowPear, result));
	}
	
	//Step 9: Detect KiwiFruit.
	subdetectorResults = kiwiFruitDetector.Detect(bgrImage, hsvFull_Image, mask);
	dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
	maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
	maskXOR_Operation(bgrImage, mask, bgrImage);
	for (auto& result : subdetectorResults)
	{
		finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::KiwiFruit, result));
	}

	//Step 10: Detect Pear.
	subdetectorResults = pearDetector.Detect(bgrImage, hsvFull_Image, mask);
	dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
	maskXOR_Operation(hsvFull_Image, mask, hsvFull_Image);
	maskXOR_Operation(bgrImage, mask, bgrImage);
	for (auto& result : subdetectorResults)
	{
		finalResults.push_back(RoboCup::Result(RoboCup::Fruit_t::Pear, result));
	}

	return finalResults;
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
