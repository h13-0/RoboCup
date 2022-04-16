/*
 * SendResultViaMessage.c
 *
 *  Created on: 2021Äê11ÔÂ28ÈÕ
 *      Author: h13
 */
#include "SendResultViaMessage.h"
#include "Drivers.h"

__attribute__((always_inline)) static inline void addChineseNumberCharacterToMessage(LuatMessage_t *Msg, int8_t Number)
{
	switch(Number)
	{
	case 0:
		break;

	case 1:
		LuatAddMessageContent(Msg, UnicodeChsYi);
		break;

	case 2:
		LuatAddMessageContent(Msg, UnicodeChsLiang);
		break;

	case 3:
		LuatAddMessageContent(Msg, UnicodeChsSan);
		break;

	case 4:
		LuatAddMessageContent(Msg, UnicodeChsSi);
		break;

	case 5:
		LuatAddMessageContent(Msg, UnicodeChsWu);
		break;

	case 6:
		LuatAddMessageContent(Msg, UnicodeChsLiu);
		break;

	case 7:
		LuatAddMessageContent(Msg, UnicodeChsQi);
		break;

	default:
		break;
	}
}

void SendResultViaMessage(const FruitIdentifyResult_t *Result)
{
	char messageBuffer[128] = { 0 };
	LuatMessage_t message = LuatCreatNewMessage(messageBuffer, 128);

	//Apple
	LuatAddMessageContents(&message, UnicodeChsPing, UnicodeChsGuo);
	if(Result -> AppleNumber > 0)
	{
		addChineseNumberCharacterToMessage(&message, Result -> AppleNumber);
	} else {
		LuatAddMessageContents(&message, UnicodeChsYi);
	}
	LuatAddMessageContent(&message, UnicodeChsGe);

	//Banana
	if(Result -> BananaNumber > 0)
	{
		LuatAddMessageContents(&message, UnicodeChsComma, UnicodeChsXiang, UnicodeChsJiao);
		addChineseNumberCharacterToMessage(&message, Result -> BananaNumber);
		LuatAddMessageContents(&message, UnicodeChsGe);
	}

	//KiwiFruit
	if(Result -> KiwiFruitNumber > 0)
	{
		LuatAddMessageContents(&message, UnicodeChsComma, UnicodeChsMi, UnicodeChsHou, UnicodeChsTao);
		addChineseNumberCharacterToMessage(&message, Result -> KiwiFruitNumber);
		LuatAddMessageContents(&message, UnicodeChsGe);
	}

	//Lemon
	if(Result -> LemonNumber > 0)
	{
		LuatAddMessageContents(&message, UnicodeChsComma, UnicodeChsNing, UnicodeChsMeng);
		addChineseNumberCharacterToMessage(&message, Result -> LemonNumber);
		LuatAddMessageContents(&message, UnicodeChsGe);
	}

	//Orange
	if(Result -> OrangeNumber > 0)
	{
		LuatAddMessageContents(&message, UnicodeChsComma, UnicodeChsJu, UnicodeChsZi);
		addChineseNumberCharacterToMessage(&message, Result -> OrangeNumber);
		LuatAddMessageContents(&message, UnicodeChsGe);
	}

	//Peach
	if(Result -> PeachNumber > 0)
	{
		LuatAddMessageContents(&message, UnicodeChsComma, UnicodeChsTao, UnicodeChsZi);
		addChineseNumberCharacterToMessage(&message, Result -> PeachNumber);
		LuatAddMessageContents(&message, UnicodeChsGe);
	}

	//Pear
	if(Result -> PearNumber > 0)
	{
		LuatAddMessageContents(&message, UnicodeChsComma, UnicodeChsXiang, UnicodeChsLi);
		addChineseNumberCharacterToMessage(&message, Result -> PearNumber);
		LuatAddMessageContents(&message, UnicodeChsGe);
	}

	//Pitaya
	if(Result -> PitayaNumber > 0)
	{
		LuatAddMessageContents(&message, UnicodeChsComma, UnicodeChsHuo, UnicodeChsLong, UnicodeChsGuo);
		addChineseNumberCharacterToMessage(&message, Result -> PitayaNumber);
		LuatAddMessageContents(&message, UnicodeChsGe);
	}

	//SnowPear
	if(Result -> SnowPearNumber > 0)
	{
		LuatAddMessageContents(&message, UnicodeChsComma, UnicodeChsXue, UnicodeChsLi);
		addChineseNumberCharacterToMessage(&message, Result -> SnowPearNumber);
		LuatAddMessageContents(&message, UnicodeChsGe);
	}

	LuatSendMessage(&message, TargetPhone);
	LuatSendMessage(&message, TargetPhone);
}
