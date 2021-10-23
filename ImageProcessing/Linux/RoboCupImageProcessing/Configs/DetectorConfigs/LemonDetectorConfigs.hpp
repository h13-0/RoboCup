#pragma once
#include "yaml-cpp/yaml.h"
#include "DetectorConfigs.hpp"
namespace RoboCup
{
	class LemonDetectorConfigs : public DetectorConfigs
	{
	public:
		/// <summary>
		/// Construct with default parameters.
		/// </summary>
		/// <param name=""></param>
		LemonDetectorConfigs(void) noexcept
		{
			positiveThresholds = { h13::HSV_Threshold(15, 36, 110, 255, 116, 255), h13::HSV_Threshold(35, 50, 85, 220, 80, 255) };
			negativeThresholds = { h13::HSV_Threshold(12, 24, 101, 255, 150, 255) };
			reflectiveThresholds = { };
			minimumSize = 1300;
		};

		/// <summary>
		/// Use profile construction.
		/// note: The default construction is used when the configuration file is wrong.
		/// </summary>
		/// <param name="Node">Config node.</param>
		LemonDetectorConfigs(const YAML::Node& Node) noexcept : LemonDetectorConfigs()
		{
			GetParameters(Node, positiveThresholds, negativeThresholds, reflectiveThresholds, minimumSize);
			erodeKernelSize = GetIntegerValue(Node["ErodeKernelSize"], erodeKernelSize);
			minimumLengthWidthRatio = GetFloatValue(Node["MinimumLengthWidthRatio"], minimumLengthWidthRatio);
		};

		~LemonDetectorConfigs() noexcept { };

		int GetErodeKernelSize(void) const noexcept
		{
			return erodeKernelSize;
		}

		float GetMinimumLengthWidthRatio(void) const noexcept
		{
			return minimumLengthWidthRatio;
		}

	private:
		int erodeKernelSize = 5;
		float minimumLengthWidthRatio = 0.55;
	};
}
