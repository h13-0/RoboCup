#pragma once
#include <string>
#include "yaml-cpp/yaml.h"

#include "AppleDetectorConfigs.hpp"
#include "FruitDetectorConfigs.hpp"
#include "TargetDetectorConfigs.hpp"

namespace RoboCup
{
	class Configs
	{
	public:
		Configs(const std::string& PathOfConfigFile);
		Configs(const YAML::Node& Yaml) : configs(Yaml) { };
		~Configs() {};

		void GenerateDefaultConfigFiles(const std::string& PathOfConfigFile);
		FruitDetectorConfigs GetFruitDetectorSettings();
		TargetDetectorConfigs GetTargetDetectorSettings();
		AppleDetectorConfigs GetAppleDetectorSettings();

	private:
		YAML::Node configs;
	};
}
