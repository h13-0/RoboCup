#include "DetectorConfigs.hpp"
#include "ConfigParameterInvalid.hpp"
#define GLOG_NO_ABBREVIATED_SEVERITIES 
#include <glog/logging.h>
void RoboCup::DetectorConfigs::GetThresholds(const YAML::Node& Node, std::vector<h13::HSV_Threshold_t>& PositiveThresholds, std::vector<h13::HSV_Threshold_t>& NegativeThresholds, std::vector<h13::HSV_Threshold_t>& ReflectiveThresholds)
{
	using namespace google;
	try {
		PositiveThresholds = GetHSV_Thresholds(Node["PositiveThresholds"]);
	}
	catch (ConfigParameterInvalid exception)
	{
		LOG(WARNING) << "Could not convert PositiveThresholds to std::vector<h13::HSV_Threshold_t>.";
	}

	try {
		NegativeThresholds = GetHSV_Thresholds(Node["NegativeThresholds"]);
	}
	catch (ConfigParameterInvalid exception)
	{
		LOG(WARNING) << "Could not convert NegativeThresholds to std::vector<h13::HSV_Threshold_t>.";
	}

	try {
		ReflectiveThresholds = GetHSV_Thresholds(Node["ReflectiveThresholds"]);
	}
	catch (ConfigParameterInvalid exception)
	{
		LOG(WARNING) << "Could not convert ReflectiveThresholds to std::vector<h13::HSV_Threshold_t>.";
	}
}
