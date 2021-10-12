#pragma once
#include "yaml-cpp/yaml.h"
#include "DetectorConfigs.hpp"
namespace RoboCup
{
	class AppleDetectorConfigs : public DetectorConfigs
	{
	public:
		/// <summary>
		/// Construct with default parameters.
		/// </summary>
		/// <param name=""></param>
		AppleDetectorConfigs(void) noexcept
		{
			positiveThresholds = { h13::HSV_Threshold(70, 102, 50, 255, 79, 255) };
			negativeThresholds = { h13::HSV_Threshold(210, 255, 95, 255, 125, 255), h13::HSV_Threshold(0, 74, 60, 255, 0, 255) };
			reflectiveThresholds = { h13::HSV_Threshold(70, 255, 0, 255, 253, 255) };
			minimumSize = 1300;
		};

		/// <summary>
		/// Use profile construction.
		/// note: The default construction is used when the configuration file is wrong.
		/// </summary>
		/// <param name="Node">Config node.</param>
		AppleDetectorConfigs(const YAML::Node& Node) noexcept : AppleDetectorConfigs()
		{
			GetParameters(Node, positiveThresholds, negativeThresholds, reflectiveThresholds, minimumSize);
			erodeKernelSize = GetIntegerValue(Node["ErodeKernelSize"], erodeKernelSize);
			minimumLengthWidthRatio = GetFloatValue(Node["MinimumLengthWidthRatio"], minimumLengthWidthRatio);
		};

		~AppleDetectorConfigs() noexcept { };

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
		float minimumLengthWidthRatio = 0.65;
	};
}