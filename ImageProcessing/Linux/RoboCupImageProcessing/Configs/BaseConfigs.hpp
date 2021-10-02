#pragma once
#include "yaml-cpp/yaml.h"
#include <string>
#include <vector>
#include <HSV_Filter.hpp>

namespace RoboCup
{
	class BaseConfigs
	{
	public:
		bool GetBoolValue(const YAML::Node& Node, bool DefaultValue) const noexcept;
		std::string GetStringValue(const YAML::Node& Node, const std::string& DefalutValue) const noexcept;
		int GetIntegerValue(const YAML::Node& Node, int DefauleValue) const noexcept;
		std::vector<h13::HSV_Threshold_t> GetHSV_Thresholds(const YAML::Node& Node) const;
		float GetFloatValue(const YAML::Node& Node, float DefaultValue) const noexcept;

		bool IsInteger(const YAML::Node& Node) const noexcept;
		bool IsString(const YAML::Node& Node) const noexcept;
	};
}

