#include "FruitDetectorConfigs.hpp"

#define GLOG_NO_ABBREVIATED_SEVERITIES 
#include <glog/logging.h>

RoboCup::FruitDetectorConfigs::FruitDetectorConfigs(const YAML::Node& Node) noexcept
{
	isAppleDetectorEnabled = GetBoolValue(Node["AppleDetector"]["Enable"], isAppleDetectorEnabled);
	if (isAppleDetectorEnabled)
	{
		appleDetectorQuantityLimit = GetIntegerValue(Node["AppleDetector"]["QuantityLimit"], appleDetectorQuantityLimit);
		appleDetectorConfigs = AppleDetectorConfigs(Node["AppleDetector"]["DetectorConfigs"]);
	}
	else {
		appleDetectorQuantityLimit = 0;
	}
	
	isBananaDetectorEnabled = GetBoolValue(Node["BananaDetector"]["Enable"], isBananaDetectorEnabled);
	if (isBananaDetectorEnabled)
	{
		bananaDetectorQuantityLimit = GetIntegerValue(Node["BananaDetector"]["QuantityLimit"], bananaDetectorQuantityLimit);
		bananaDetectorConfigs = BananaDetectorConfigs(Node["BananaDetector"]["DetectorConfigs"]);
	}
	else {
		bananaDetectorQuantityLimit = 0;
	}

	isKiwiFruitDetectorEnabled = GetBoolValue(Node["KiwiFruitDetector"]["Enable"], isKiwiFruitDetectorEnabled);
	if (isKiwiFruitDetectorEnabled)
	{
		kiwiFruitDetectorQuantityLimit = GetIntegerValue(Node["KiwiFruitDetector"]["QuantityLimit"], kiwiFruitDetectorQuantityLimit);
		kiwiFruitDetectorConfigs = KiwiFruitDetectorConfigs(Node["KiwiFruitDetector"]["DetectorConfigs"]);
	}
	else {
		kiwiFruitDetectorQuantityLimit = 0;
	}

	isLemonDetectorEnabled = GetBoolValue(Node["LemonDetector"]["Enable"], isLemonDetectorEnabled);
	if (isLemonDetectorEnabled)
	{
		lemonDetectorQuantityLimit = GetIntegerValue(Node["LemonDetector"]["QuantityLimit"], lemonDetectorQuantityLimit);
		lemonDetectorConfigs = LemonDetectorConfigs(Node["LemonDetector"]["DetectorConfigs"]);
	}
	else {
		lemonDetectorQuantityLimit = 0;
	}

	isOrangeDetectorEnabled = GetBoolValue(Node["OrangeDetector"]["Enable"], isOrangeDetectorEnabled);
	if (isOrangeDetectorEnabled)
	{
		orangeDetectorQuantityLimit = GetIntegerValue(Node["OrangeDetector"]["QuantityLimit"], orangeDetectorQuantityLimit);
		orangeDetectorConfigs = OrangeDetectorConfigs(Node["OrangeDetector"]["DetectorConfigs"]);
	}
	else {
		orangeDetectorQuantityLimit = 0;
	}

	isPeachDetectorEnabled = GetBoolValue(Node["PeachDetector"]["Enable"], isPeachDetectorEnabled);
	if (isPeachDetectorEnabled)
	{
		peachDetectorQuantityLimit = GetIntegerValue(Node["PeachDetector"]["QuantityLimit"], peachDetectorQuantityLimit);
		peachDetectorConfigs = PeachDetectorConfigs(Node["PeachDetector"]["DetectorConfigs"]);
	}
	else {
		peachDetectorQuantityLimit = 0;
	}

	isPearDetectorEnabled = GetBoolValue(Node["PearDetector"]["Enable"], isPearDetectorEnabled);
	if (isPearDetectorEnabled)
	{
		pearDetectorQuantityLimit = GetIntegerValue(Node["PearDetector"]["QuantityLimit"], pearDetectorQuantityLimit);
		pearDetectorConfigs = PearDetectorConfigs(Node["PearDetector"]["DetectorConfigs"]);
	}
	else {
		pearDetectorQuantityLimit = 0;
	}

	isPitayaDetectorEnabled = GetBoolValue(Node["PitayaDetector"]["Enable"], isPitayaDetectorEnabled);
	if (isPitayaDetectorEnabled)
	{
		pitayaDetectorQuantityLimit = GetIntegerValue(Node["PitayaDetector"]["QuantityLimit"], pitayaDetectorQuantityLimit);
		pitayaDetectorConfigs = PitayaDetectorConfigs(Node["PitayaDetector"]["DetectorConfigs"]);
	}
	else {
		pitayaDetectorQuantityLimit = 0;
	}

	isSnowPearDetectorEnabled = GetBoolValue(Node["SnowPearDetector"]["Enable"], isSnowPearDetectorEnabled);
	if (isSnowPearDetectorEnabled)
	{
		snowPearDetectorQuantityLimit = GetIntegerValue(Node["SnowPearDetector"]["QuantityLimit"], snowPearDetectorQuantityLimit);
		snowPearDetectorConfigs = SnowPearDetectorConfigs(Node["SnowPearDetector"]["DetectorConfigs"]);
	}
	else {
		snowPearDetectorQuantityLimit = 0;
	}
}
