#include "BaseConfigs.hpp"
#include "ConfigParameterInvalid.hpp"
#include <sstream>

bool RoboCup::BaseConfigs::GetBoolValue(const YAML::Node& Node, bool DefaultValue) const noexcept
{
	try {
		return Node.as<bool>();
	}
	catch (...)
	{
		return DefaultValue;
	}
}

std::string RoboCup::BaseConfigs::GetStringValue(const YAML::Node& Node, const std::string& DefalutValue) const noexcept
{
	try {
		return Node.as<std::string>();
	}
	catch (...)
	{
		return DefalutValue;
	}
}

int RoboCup::BaseConfigs::GetIntegerValue(const YAML::Node& Node, int DefauleValue) const noexcept
{
	try {
		return Node.as<int>();
	}
	catch (...)
	{
		return DefauleValue;
	}
}

std::vector<h13::HSV_Threshold_t> RoboCup::BaseConfigs::GetHSV_Thresholds(const YAML::Node& Node) const
{
	std::vector<int> thresholdValue;
	std::vector<h13::HSV_Threshold_t> result;
	try {
		for (auto thresholds : Node)
		{
			thresholdValue = thresholds.as<std::vector<int>>();
			if ((thresholdValue.size() == 6) && (*max_element(thresholdValue.begin(), thresholdValue.end()) <= 255) && (*min_element(thresholdValue.begin(), thresholdValue.end()) >= 0))
			{
				h13::HSV_Threshold_t threshold = h13::HSV_Threshold_t(thresholdValue[0], thresholdValue[1], thresholdValue[2], thresholdValue[3], thresholdValue[4], thresholdValue[5]);
				result.push_back(threshold);
			}
			else {
				throw ConfigParameterInvalid("Could not praise std::vector<h13::HSV_Threshold_t>.");
			}
		}
	}
	catch (const ConfigParameterInvalid &exception)
	{
		throw exception;
	}
	catch (...)
	{
		throw ConfigParameterInvalid("Could not praise std::vector<h13::HSV_Threshold_t>.");
	}
	
	return result;
}

float RoboCup::BaseConfigs::GetFloatValue(const YAML::Node& Node, float DefaultValue) const noexcept
{
	try {
		return Node.as<float>();
	}
	catch (...)
	{
		return DefaultValue;
	}
}

bool RoboCup::BaseConfigs::IsInteger(const YAML::Node& Node) const noexcept
{
	try {
		Node.as<int>();
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool RoboCup::BaseConfigs::IsString(const YAML::Node& Node) const noexcept
{
	return false;
}
