/*
 * DisplayResult.c
 *
 *  Created on: 2021��11��28��
 *      Author: h13
 */
#include "DisplayResult.h"
#include "UI.h"

static OfficialScriptChineseFont_t convertIntToOfficialScriptChinese(uint8_t Number)
{
	switch (Number) {
		case 1:
			return OfficialScriptChsYi;
			break;

		case 2:
			return OfficialScriptChsLiang;
			break;

		case 3:
			return OfficialScriptChsSan;
			break;

		case 4:
			return OfficialScriptChsSi;
			break;
		
		case 5:
			return OfficialScriptChsWu;
			break;

		case 6:
			return OfficialScriptChsLiu;
			break;

		case 7:
			return OfficialScriptChsQi;
			break;

		default:
			return OfficialScriptChsQi;
			break;
	}
}

void DisplayResult(FruitIdentifyResult_t *Result)
{
	uint8_t currentX = 0, currentY = 0;
	UI_Clear();

	//Apple
	DrawChinese26(currentX, currentY, OfficialScriptChsPing);
	currentX += 26;
	DrawChinese26(currentX, currentY, OfficialScriptChsGuo);
	currentX += 26;

	if(Result -> AppleNumber > 0)
	{
		DrawChinese26(currentX, currentY, convertIntToOfficialScriptChinese(Result -> AppleNumber));
		currentX += 26;
	} else {
		DrawChinese26(currentX, currentY, OfficialScriptChsYi);
		currentX += 26;
	}
	DrawChinese26(currentX, currentY, OfficialScriptChsGe);
	currentY += 26;
	currentX = 0;

	//Banana
	if(Result -> BananaNumber > 0)
	{
		DrawChinese26(currentX, currentY, OfficialScriptChsXiang);
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsJiao);
		currentX += 26;
		DrawChinese26(currentX, currentY, convertIntToOfficialScriptChinese(Result -> BananaNumber));
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsGe);
		currentY += 26;
		currentX = 0;
	}

	//KiwiFruit
	if(Result -> KiwiFruitNumber > 0)
	{
		DrawChinese26(currentX, currentY, OfficialScriptChsMi);
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsHou);
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsTao);
		currentX += 26;
		DrawChinese26(currentX, currentY, convertIntToOfficialScriptChinese(Result -> KiwiFruitNumber));
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsGe);
		currentY += 26;
		currentX = 0;
	}

	//Lemon
	if(Result -> LemonNumber > 0)
	{
		DrawChinese26(currentX, currentY, OfficialScriptChsNing);
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsMeng);
		currentX += 26;
		DrawChinese26(currentX, currentY, convertIntToOfficialScriptChinese(Result -> LemonNumber));
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsGe);
		currentY += 26;
		currentX = 0;
	}

	//Orange
	if(Result -> OrangeNumber > 0)
	{
		DrawChinese26(currentX, currentY, OfficialScriptChsJu);
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsZi);
		currentX += 26;
		DrawChinese26(currentX, currentY, convertIntToOfficialScriptChinese(Result -> OrangeNumber));
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsGe);
		currentY += 26;
		currentX = 0;
	}

	//Peach
	if(Result -> PeachNumber > 0)
	{
		DrawChinese26(currentX, currentY, OfficialScriptChsTao);
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsZi);
		currentX += 26;
		DrawChinese26(currentX, currentY, convertIntToOfficialScriptChinese(Result -> PeachNumber));
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsGe);
		currentY += 26;
		currentX = 0;
	}

	//Pear
	if(Result -> PearNumber > 0)
	{
		DrawChinese26(currentX, currentY, OfficialScriptChsXiang);
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsLi);
		currentX += 26;
		DrawChinese26(currentX, currentY, convertIntToOfficialScriptChinese(Result -> PearNumber));
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsGe);
		currentY += 26;
		currentX = 0;
	}

	//Pitaya
	if(Result -> PitayaNumber > 0)
	{
		DrawChinese26(currentX, currentY, OfficialScriptChsHuo);
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsLong);
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsGuo);
		currentX += 26;
		DrawChinese26(currentX, currentY, convertIntToOfficialScriptChinese(Result -> PitayaNumber));
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsGe);
		currentY += 26;
		currentX = 0;
	}

	//SnowPear
	if(Result -> SnowPearNumber > 0)
	{
		DrawChinese26(currentX, currentY, OfficialScriptChsXue);
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsLi);
		currentX += 26;
		DrawChinese26(currentX, currentY, convertIntToOfficialScriptChinese(Result -> SnowPearNumber));
		currentX += 26;
		DrawChinese26(currentX, currentY, OfficialScriptChsGe);
		currentY += 26;
		currentX = 0;
	}
}
