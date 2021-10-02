#include "AppleDetectorConfigs.hpp"
#include "ConfigParameterInvalid.hpp"
#define GLOG_NO_ABBREVIATED_SEVERITIES 
#include <glog/logging.h>

RoboCup::AppleDetectorConfigs::AppleDetectorConfigs(const YAML::Node& Node)
	: AppleDetectorConfigs()
{
	GetThresholds(Node, positiveThresholds, negativeThresholds, reflectiveThresholds);
	erodeKernelSize = GetIntegerValue(Node["ErodeKernelSize"], erodeKernelSize);
	minimumSize = GetFloatValue(Node["MinimumSize"], minimumSize);
	minimunLengthWidthRatio = GetFloatValue(Node["MinimunLengthWidthRatio"], minimunLengthWidthRatio);
}

