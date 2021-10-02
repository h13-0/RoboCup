#include "FruitDetectorConfigs.hpp"

#define GLOG_NO_ABBREVIATED_SEVERITIES 
#include <glog/logging.h>

RoboCup::FruitDetectorConfigs::FruitDetectorConfigs(const YAML::Node& Node)
{
	isAppleDetectorEnabled = GetBoolValue(Node["AppleDetector"]["Enable"], true);
	if (isAppleDetectorEnabled)
	{
		appleDetectorQuantityLimit = GetIntegerValue(Node["AppleDetector"]["QuantityLimit"], 4);
		appleDetectorConfigs = AppleDetectorConfigs(Node["AppleDetector"]["DetectorConfigs"]);
	}
	


}
