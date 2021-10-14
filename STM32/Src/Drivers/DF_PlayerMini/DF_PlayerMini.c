/*
 * DF_PlayerMini.c
 *
 *  Created on: 2021Äê10ÔÂ14ÈÕ
 *      Author: h13
 */

#include "DF_PlayerMini.h"
#include <stddef.h>
#include <string.h>

static DF_PlayerMini_t *players[VoiceDeviceNumber] = { NULL };
static uint8_t currentPlayerNumbers = 0;

DF_PlayerError_t DF_PlayerInit(DF_PlayerMini_t *Player)
{
	GPIO_Init(&Player -> BusyIO, Input);
	Player -> _latestVoicePositionInQueue = 0;
	if(currentPlayerNumbers < StepperNumbers)
	{
		players[currentPlayerNumbers] = Player;
		Player -> _playerID = currentPlayerNumbers;
		currentPlayerNumbers ++;
		return DF_PlayerOK;
	} else {
		return DF_PlayerBufferIsFull;
	}
}

DF_PlayerError_t DF_PlayerJoinQueue(DF_PlayerMini_t *Player, VoiceID_t VoiceID)
{
	if(Player -> _latestVoicePositionInQueue < VoiceQueueSize)
	{
		Player -> VoiceQueue[Player -> _latestVoicePositionInQueue] = VoiceID;
		return DF_PlayerOK;
	} else {
		return DF_PlayerBufferIsFull;
	}
}

//TODO: Improve api(document is in ../Docs/DFPlayer Mini.pdf).
typedef enum
{
	DF_PlayNext = 0x01,
	DF_SetVolume = 0x06,
	DF_Reset = 0x0c,
	DF_PlayByID = 0x0d,
	DF_Pause = 0x0e,
	DF_Stop = 0x16,
} Command_t;

__attribute__((always_inline)) static void sendCommand(USART_t USART_Port, Command_t Command, uint16_t parameter)
{
	//Send packet head.
	SerialSend(USART_Port, 0x7E);
	SerialSend(USART_Port, 0xFF);
	SerialSend(USART_Port, 0x06);

	//Send Command.
	SerialSend(USART_Port, Command);
	SerialSend(USART_Port, 0x00);       //No need feedback.

	//Send parameters.
#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
	SerialSend(USART_Port, parameter >> 8);
	SerialSend(USART_Port, parameter & 0xFF);
#elif (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
	SerialSend(USART_Port, parameter & 0xFF);
	SerialSend(USART_Port, parameter >> 8);
#endif

	//Send packet tail.
	SerialSend(USART_Port, 0xEF);
}

__attribute__((always_inline)) static void playByID(USART_t USART_Port, VoiceID_t ID)
{
	sendCommand(USART_Port, DF_PlayByID, ID);
}

__attribute__((always_inline)) static void resetPlayer(USART_t USART_Port)
{
	sendCommand(USART_Port, DF_PlayByID, 0);
}

DF_PlayerError_t DF_PlayerReset(DF_PlayerMini_t *Player)
{
	sendCommand(Player -> USART_Port, DF_Reset, 0);
	return DF_PlayerOK;
}

void DF_PlayerVoiceQueueTimerHandler(void)
{
	return;
	for(uint8_t playerID = 0; playerID < currentPlayerNumbers; playerID ++)
	{
		if( (GPIO_ReadLevel(&players[playerID] -> BusyIO)) && (players[playerID] -> _latestVoicePositionInQueue > 0) )
		{
			//Play next voice.
			memmove(players[playerID] -> VoiceQueue, (players[playerID] -> VoiceQueue + 1), VoiceQueueSize - 1);
			playByID(players[playerID] -> USART_Port, players[playerID] -> VoiceQueue[0]);
		}
	}
}
