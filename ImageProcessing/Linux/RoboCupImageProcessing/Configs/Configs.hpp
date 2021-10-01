#pragma once
#include <string>
#include "yaml-cpp/yaml.h"

namespace RoboCup
{
	class Configs
	{
	public:
		Configs(const std::string& PathOfConfigFile);
		Configs(const YAML::Node& Yaml);
		~Configs() {};

		void GenerateDefaultConfigFiles(const std::string& PathOfConfigFile);

	private:

	};
}
