#pragma once
#include "yaml-cpp/yaml.h"
#include "BaseConfigs.hpp"
#include "AppleDetectorConfigs.hpp"

namespace RoboCup
{
	class FruitDetectorConfigs : public BaseConfigs
	{
	public:
		FruitDetectorConfigs(const YAML::Node& Node);
		~FruitDetectorConfigs() { };

	private:
		/// <summary>
		/// Detector configs.
		/// </summary>
		bool isAppleDetectorEnabled = true;
		int appleDetectorQuantityLimit = 4;
		AppleDetectorConfigs appleDetectorConfigs;

		
	};
}