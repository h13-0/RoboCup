#pragma once
#include <string>
#include "yaml-cpp/yaml.h"

#include "AppleDetectorConfigs.hpp"
#include "FruitDetectorConfigs.hpp"
#include "TargetDetectorConfigs.hpp"
#include "ApplicationConfigs.hpp"

namespace RoboCup
{
	class Configs
	{
	public:
		Configs(void) = default;
		Configs(const std::string& PathOfConfigFile);
		Configs(const YAML::Node& Yaml) : configs(Yaml) { };
		~Configs() {};

		void GenerateDefaultConfigFiles(const std::string& PathOfConfigFile);
		FruitDetectorConfigs GetFruitDetectorSettings() const;
		TargetDetectorConfigs GetTargetDetectorSettings() const;
		AppleDetectorConfigs GetAppleDetectorSettings() const;
		ApplicationConfigs GetApplicationConfigs() const;

	private:
		YAML::Node configs;
	};
}
