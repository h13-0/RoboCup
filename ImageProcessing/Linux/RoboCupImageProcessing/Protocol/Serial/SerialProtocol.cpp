#include "SerialProtocol.hpp"
#if defined(__linux__)
#include <string>
#include <stdexcept>
#include <thread>
#include "serial.h"

h13::SerialProtocol::SerialProtocol(const std::string& Port, int BaudRate, const int BufferSize, const int RefreshTimeOut)
	: Protocol()
{
	serial = serial_new();
	if (serial_open(serial, Port.c_str(), BaudRate) < 0)
	{
		throw std::runtime_error("Could not open serial port: " + Port);
	}

	exit = false;
	exited = false;
	std::thread receiveMethod([&](const int BufferSize, const int RefreshTimeOut)
	{
		uint8_t* buffer = new uint8_t[BufferSize];
		int index = 0;
		while (true)
		{
			//Check exit flag.
			{
				std::unique_lock<std::mutex> lock(exitFlagMutex);
				if (this->exit)
				{
					break;
				}
			}
			
			//Read serial.
			int bytes = serial_read(serial, buffer + index, BufferSize - index, RefreshTimeOut);
			if (bytes > 0)
			{
				index += bytes;

				//Check packet end flag.
				int packetLength = 0;
				for (int i = 0; i <= index - 2; i++)
				{
					if (!memcmp(buffer + i, "\r\n", 2))
					{
						packetLength = i;
						break;
					}
				}

				//Packet end.
				if (packetLength)
				{
					//find ':' to split key from packet.
					char* posPtr = strchr((char*)buffer, ':');
					int pos = 0;
					if (posPtr != NULL)
					{
						pos = posPtr - (char*)buffer;
					}

					//if key exist.
					if (pos)
					{
						std::unique_lock<std::mutex> lock(this -> callbackQueueMutex);
						std::string key((char*)buffer, 0, pos);
						//Callback.
						for (auto callback : keyCallbacks)
						{
							if (!callback.Key.compare(key))
							{
								callback.Callback((buffer + pos + 1), packetLength - pos - 1);
							}
						}
					}
					
					memmove(buffer, buffer + packetLength + 2, index - packetLength - 2);
					memset(buffer + index - packetLength - 2, 0, packetLength + 2);
					index = 0;
				}

				//buffer overflow.
				//TODO: Forward buffer.
				if ((!packetLength) && (index == BufferSize - 1))
				{
					memset(buffer, 0, BufferSize);
					index = 0;
					//memmove(buffer, buffer);
				}
			}
		}
		exited = true;
		exitedConditionVariable.notify_one();
	}, BufferSize, RefreshTimeOut);

	receiveMethod.detach();
}

h13::SerialProtocol::~SerialProtocol()
{
	std::unique_lock<std::mutex> lock(exitFlagMutex);
	exit = true;
	exitedConditionVariable.wait(lock, [this] { return exited; });

	serial_close(serial);
	serial_free(serial);
}
#endif
