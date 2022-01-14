#pragma once
#include <string>
#include <functional>
#include <mutex>
#include <vector>

//TODO: Support large and small end mode.
namespace h13
{
	class Protocol
	{
	public:
		virtual ~Protocol() { };
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

		/// <summary>
		/// Typedef of callback function.
		/// </summary>
		typedef std::function<void(void *DataPtr, int Length)> Callback_t;
		typedef struct KeyCallback
		{
			KeyCallback(const std::string& Key, const Callback_t& Callback)
				: Key(Key), Callback(Callback) { };
			std::string Key;
			Callback_t Callback;
		} KeyCallback_t;

		/// <summary>
		/// Add Callback handle.
		/// </summary>
		/// <param name="Key">The key you want listen.</param>
		/// <param name="Callback">Callback method.</param>
		void AddKeyCallback(std::string Key, Callback_t Callback)
		{
			std::unique_lock<std::mutex> lock(callbackQueueMutex);
			keyCallbacks.push_back(KeyCallback(Key, Callback));
		};

	protected:
		std::mutex callbackQueueMutex;
		std::vector<KeyCallback_t> keyCallbacks;
	};
}
