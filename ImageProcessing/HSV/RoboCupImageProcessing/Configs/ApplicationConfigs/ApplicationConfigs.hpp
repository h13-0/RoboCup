#pragma once
#include <string>
#include "yaml-cpp/yaml.h"
#include "BaseConfigs.hpp"
#include "ProgramConflict.hpp"
#define GLOG_NO_ABBREVIATED_SEVERITIES 
#include <glog/logging.h>

namespace RoboCup
{
	enum class MonitorType
	{
		OpenCV,
		FrameBuffer,
		None
	};

	enum class VideoDeviceType
	{
		Camera,
		VideoStream
	};

	class ApplicationConfigs : private BaseConfigs
	{
	public:
		ApplicationConfigs(const YAML::Node& Node) noexcept
		{
#if defined(__linux__)
			std::string mt;
			mt = GetStringValue(Node["MonitorType"], "");
			frameBufferPath = "/dev/fb0";
			if (!mt.compare("Auto"))
			{
				monitorType = MonitorType::FrameBuffer;
				frameBufferPath = GetStringValue(Node["FrameBufferPath"], frameBufferPath);
			}
			else if (!mt.compare("OpenCV"))
			{
				monitorType = MonitorType::OpenCV;
			}
			else if (!mt.compare("FrameBuffer"))
			{
				monitorType = MonitorType::FrameBuffer;
				frameBufferPath = GetStringValue(Node["FrameBufferPath"], frameBufferPath);
			}
			else {
				LOG(WARNING) << R"(Read configuration node ["ApplicationSettings"]["MonitorType"] failed, using MonitorType: None)";
				monitorType = MonitorType::None;
				frameBufferPath = GetStringValue(Node["FrameBufferPath"], frameBufferPath);
			}
#endif
			if (IsInteger(Node["VideoDevice"]))
			{
				videoDeviceType = VideoDeviceType::Camera;
				cameraID = GetIntegerValue(Node["VideoDevice"], 0);
			}
			else {
				videoDeviceType = VideoDeviceType::VideoStream;
				videoStreamPath = GetStringValue(Node["VideoDevice"], "");
			}

			serialPort = GetStringValue(Node["SerialPort"], "");
		};
		~ApplicationConfigs() { };

		MonitorType GetMonitorType(void) const noexcept
		{
#if defined(_WIN32)
			return MonitorType::OpenCV;
#else
			return monitorType;
#endif
		};

#if defined(__linux__)
		std::string GetFrameBufferPath(void) const noexcept
		{
			return frameBufferPath;
		};
#endif
		
		VideoDeviceType GetVideoDeviceType(void) const noexcept
		{
			return videoDeviceType;
		};

		int GetCameraID(void)
		{
			if (cameraID < 0)
			{
				throw ProgramConflict("Unable to get CameraID in VideoStream mode.");
			}
			return cameraID;
		};

		std::string GetVideoStreamPath(void) const noexcept
		{
			return videoStreamPath;
		};

		std::string GetSerialPort(void) const noexcept
		{
			return serialPort;
		};

	private:
#if defined(__linux__)
		MonitorType monitorType;
		std::string frameBufferPath;
#endif
		VideoDeviceType videoDeviceType;
		int cameraID = -1;
		std::string videoStreamPath;

		std::string serialPort;
	};
}
