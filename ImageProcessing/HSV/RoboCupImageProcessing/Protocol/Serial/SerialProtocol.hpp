#pragma once
#include "Protocol.hpp"
#if defined(__linux__)
#include <string.h>
#include <mutex>
#include <condition_variable>
#include "serial.h"

namespace h13
{
	class SerialProtocol : public Protocol
	{
	public:
		SerialProtocol(const std::string& Port, int BaudRate, const int BufferSize = 512, const int RefreshTimeOut = 50);
		SerialProtocol(const SerialProtocol&) = delete;
		~SerialProtocol();

		/// <summary>
		/// Send binary packet.
		/// Packet format:
		///		${Key}:${Value in hex}\r\n
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="Key"></param>
		/// <param name="Value"></param>
		template<class T, class = typename std::enable_if< std::is_arithmetic<T>::value || std::is_same<T, std::string>::value >>
		void SendPacket(const std::string& Key, const T& Value)
		{
			uint8_t *buffer = new uint8_t[Key.length() + 1 + sizeof(Value) + 2];
			int index = 0;
			//${Key}
			memcpy(buffer + index, Key.c_str(), Key.length());
			index += Key.length();
			//':'
			*(buffer + index) = ':';
			index += 1;
			//${Value in hex}
			memcpy(buffer + index, &Value, sizeof(Value));
			index += sizeof(Value);
			//"\r\n"
			memcpy(buffer + index, "\r\n", 2);
			//Send throgh serial.
			std::unique_lock<std::mutex> lock(serialWriteMutex);
			serial_write(serial, buffer, Key.length() + 1 + sizeof(Value) + 2);
			delete buffer;
		}

	private:
		serial_t* serial;
		std::mutex serialWriteMutex;

		bool exit;
		bool exited;
		std::condition_variable exitedConditionVariable;
		std::mutex exitFlagMutex;
	};
}
#else
namespace h13
{
	class SerialProtocol : public Protocol
	{
	public:
		SerialProtocol(const std::string& Port, int BaudRate, const int BufferSize = 512, const int RefreshTimeOut = 50) { };
		SerialProtocol(const SerialProtocol&) = delete;
		~SerialProtocol() { };

		/// <summary>
		/// Send binary packet.
		/// Packet format:
		///		${Key}:${Value in hex}\r\n
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="Key"></param>
		/// <param name="Value"></param>
		template<class T, class = typename std::enable_if< std::is_arithmetic<T>::value || std::is_same<T, std::string>::value >>
		void SendPacket(const std::string& Key, const T& Value) { };

	};
}
#endif
