#pragma once
#include "yaml-cpp/yaml.h"
#include "DetectorConfigs.hpp"
namespace RoboCup
{
	class KiwiFruitDetectorConfigs : public DetectorConfigs
	{
	public:
		/// <summary>
		/// Construct with default parameters.
		/// </summary>
		/// <param name=""></param>
		KiwiFruitDetectorConfigs(void) noexcept
		{
			positiveThresholds = { h13::HSV_Threshold(60, 80, 80, 180, 50, 255), h13::HSV_Threshold(45, 110, 22, 206, 15, 255) };
			negativeThresholds = { h13::HSV_Threshold(0, 51, 0, 26, 0, 255), h13::HSV_Threshold(0, 255, 0, 40, 0, 255) };
			reflectiveThresholds = { };
			minimumSize = 1300;
		};

		/// <summary>
		/// Use profile construction.
		/// note: The default construction is used when the configuration file is wrong.
		/// </summary>
		/// <param name="Node">Config node.</param>
		KiwiFruitDetectorConfigs(const YAML::Node& Node) noexcept : KiwiFruitDetectorConfigs()
		{
			GetParameters(Node, positiveThresholds, negativeThresholds, reflectiveThresholds, minimumSize);
			minimumLengthWidthRatio = GetFloatValue(Node["MinimumLengthWidthRatio"], minimumLengthWidthRatio);
		};

		float GetMinimumLengthWidthRatio(void) const noexcept
		{
			return minimumLengthWidthRatio;
		}

		~KiwiFruitDetectorConfigs() noexcept { };

	private:
		float minimumLengthWidthRatio = 0.65;
	};
}