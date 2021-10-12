#pragma once
#include "yaml-cpp/yaml.h"
#include "DetectorConfigs.hpp"
namespace RoboCup
{
	class SnowPearDetectorConfigs : public DetectorConfigs
	{
	public:
		/// <summary>
		/// Construct with default parameters.
		/// </summary>
		/// <param name=""></param>
		SnowPearDetectorConfigs(void) noexcept
		{
			positiveThresholds = { h13::HSV_Threshold(40, 75, 21, 100, 80, 255) };
			negativeThresholds = { h13::HSV_Threshold(0, 50, 0, 255, 0, 255) };
			reflectiveThresholds = { h13::HSV_Threshold(55, 70, 110, 255, 80, 255) };
			minimumSize = 900;
		};

		/// <summary>
		/// Use profile construction.
		/// note: The default construction is used when the configuration file is wrong.
		/// </summary>
		/// <param name="Node">Config node.</param>
		SnowPearDetectorConfigs(const YAML::Node& Node) noexcept : SnowPearDetectorConfigs()
		{
			GetParameters(Node, positiveThresholds, negativeThresholds, reflectiveThresholds, minimumSize);
			minimumLengthWidthRatio = GetFloatValue(Node["MinimumLengthWidthRatio"], minimumLengthWidthRatio);
			minimumContourArea = GetFloatValue(Node["MinimumContourArea"], minimumContourArea);
			maximumContourArea = GetFloatValue(Node["MaximumContourArea"], maximumContourArea);
		};

		~SnowPearDetectorConfigs() noexcept { };

		float GetMinimumLengthWidthRatio(void) const noexcept
		{
			return minimumLengthWidthRatio;
		};

		float GetMinimumContourArea(void) const noexcept
		{
			return minimumContourArea;
		};

		float GetMaximumContourArea(void) const noexcept
		{
			return maximumContourArea;
		};

	private:
		float minimumLengthWidthRatio = 0.5;
		float minimumContourArea = 2200;
		float maximumContourArea = 15000;
	};
}