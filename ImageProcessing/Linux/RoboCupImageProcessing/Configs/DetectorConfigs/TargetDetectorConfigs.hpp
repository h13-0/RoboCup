#pragma once
#include "yaml-cpp/yaml.h"
#include "DetectorConfigs.hpp"
namespace RoboCup
{
	class TargetDetectorConfigs : public DetectorConfigs
	{
	public:
		TargetDetectorConfigs(const YAML::Node& node);
		~TargetDetectorConfigs() { };

	private:

	};
}
