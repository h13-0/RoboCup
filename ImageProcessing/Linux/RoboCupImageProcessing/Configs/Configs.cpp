#include "Configs.hpp"
#include "yaml-cpp/yaml.h"

RoboCup::Configs::Configs(const std::string& PathOfConfigFile)
{
	using namespace YAML;
	configs = LoadFile(PathOfConfigFile);
}

void RoboCup::Configs::GenerateDefaultConfigFiles(const std::string& PathOfConfigFile)
{
	using namespace YAML;
}

RoboCup::FruitDetectorConfigs RoboCup::Configs::GetFruitDetectorSettings() const
{
	return FruitDetectorConfigs(configs["FruitDetectorSettings"]);
}

RoboCup::TargetDetectorConfigs RoboCup::Configs::GetTargetDetectorSettings() const
{
	return TargetDetectorConfigs(configs["TargetDetectorSettings"]["DetectorConfigs"]);
}

RoboCup::AppleDetectorConfigs RoboCup::Configs::GetAppleDetectorSettings() const
{
	return AppleDetectorConfigs(configs["AppleDetectorSettings"]["DetectorConfigs"]);
}

RoboCup::ApplicationConfigs RoboCup::Configs::GetApplicationConfigs() const
{
	return ApplicationConfigs(configs["ApplicationSettings"]);
}
