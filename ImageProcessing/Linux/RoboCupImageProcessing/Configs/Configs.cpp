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

RoboCup::FruitDetectorConfigs RoboCup::Configs::GetFruitDetectorSettings()
{
	return FruitDetectorConfigs(configs["FruitDetectorSettings"]);
}

RoboCup::TargetDetectorConfigs RoboCup::Configs::GetTargetDetectorSettings()
{
	return TargetDetectorConfigs(configs["TargetDetectorSettings"]);
}

RoboCup::AppleDetectorConfigs RoboCup::Configs::GetAppleDetectorSettings()
{
	return AppleDetectorConfigs(configs["AppleDetectorSettings"]);
}
