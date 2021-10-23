#pragma once
#include "yaml-cpp/yaml.h"
#include "DetectorConfigs.hpp"
namespace RoboCup
{
	class OrangeDetectorConfigs : public DetectorConfigs
	{
	public:
		/// <summary>
		/// Construct with default parameters.
		/// </summary>
		/// <param name=""></param>
		OrangeDetectorConfigs(void) noexcept
		{
			positiveThresholds = { h13::HSV_Threshold(3, 26, 106, 255, 0, 255) };
			negativeThresholds = { h13::HSV_Threshold(7, 21, 100, 150, 0, 255) };
			reflectiveThresholds = { };
			minimumSize = 1600;
		};

		/// <summary>
		/// Use profile construction.
		/// note: The default construction is used when the configuration file is wrong.
		/// </summary>
		/// <param name="Node">Config node.</param>
		OrangeDetectorConfigs(const YAML::Node& Node) noexcept : OrangeDetectorConfigs()
		{
			GetParameters(Node, positiveThresholds, negativeThresholds, reflectiveThresholds, minimumSize);
			erodeKernelSize = GetIntegerValue(Node["ErodeKernelSize"], erodeKernelSize);
		};

		~OrangeDetectorConfigs() noexcept { };

		int GetErodeKernelSize(void) const noexcept
		{
			return erodeKernelSize;
		}

	private:
		int erodeKernelSize = 5;
		float maximumLengthWidthRatio = 0.65;
	};
}
