#pragma once
#include "yaml-cpp/yaml.h"
#include "DetectorConfigs.hpp"
namespace RoboCup
{
	class TargetDetectorConfigs : public DetectorConfigs
	{
	public:
		/// <summary>
		/// Construct with default parameters.
		/// </summary>
		/// <param name=""></param>
		TargetDetectorConfigs(void) noexcept
		{
			positiveThresholds = { h13::HSV_Threshold(116, 180, 0, 70, 90, 255) };
			negativeThresholds = { };
			reflectiveThresholds = { h13::HSV_Threshold(240, 255, 155, 255, 80, 255) };
			minimumPositiveSize = 5000;
			minimumPositiveLengthWidthRatio = 0.4;
			minimumReflectiveSize = 2000;
			minimumReflectiveLengthWidthRatio = 0.4;
			minimumRectOverlapRate = 0.5;
		}

		/// <summary>
		/// Use profile construction.
		/// note: The default construction is used when the configuration file is wrong.
		/// </summary>
		/// <param name="Node">Config node.</param>
		TargetDetectorConfigs(const YAML::Node& Node) noexcept : TargetDetectorConfigs()
		{
			float useless;
			GetParameters(Node, positiveThresholds, negativeThresholds, reflectiveThresholds, useless);
			minimumPositiveSize = GetFloatValue(Node["MinimumPositiveSize"], minimumPositiveSize);
			minimumPositiveLengthWidthRatio = GetFloatValue(Node["MinimumPositiveLengthWidthRatio"], minimumPositiveLengthWidthRatio);
			minimumReflectiveSize = GetFloatValue(Node["MinimumReflectiveSize"], minimumReflectiveSize);
			minimumReflectiveLengthWidthRatio = GetFloatValue(Node["MinimumReflectiveLengthWidthRatio"], minimumReflectiveLengthWidthRatio);
			minimumRectOverlapRate = GetFloatValue(Node["MinimumRectOverlapRate"], minimumRectOverlapRate);
		};

		~TargetDetectorConfigs() noexcept { };

		float GetMinimumPositiveSize(void) const noexcept
		{
			return minimumPositiveSize;
		};

		float GetMinimumPositiveLengthWidthRatio(void) const noexcept
		{
			return minimumPositiveLengthWidthRatio;
		};

		float GetMinimumReflectiveSize(void) const noexcept
		{
			return minimumReflectiveSize;
		};

		float GetMinimumReflectiveLengthWidthRatio(void) const noexcept
		{
			return minimumReflectiveLengthWidthRatio;
		};

		float GetMinimumRectOverlapRate(void) const noexcept
		{
			return minimumRectOverlapRate;
		};

		float GetMinimumSize(void) = delete;

	private:
		//float GetMinimumSize(void) { };
		float minimumPositiveSize;
		float minimumPositiveLengthWidthRatio;
		float minimumReflectiveSize;
		float minimumReflectiveLengthWidthRatio;
		float minimumRectOverlapRate;
	};
}
