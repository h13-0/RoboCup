/*
 * DrawText.c
 *
 *  Created on: 2022Äê3ÔÂ26ÈÕ
 *      Author: h13
 */

#include "DrawText.h"
#include "DrawPicture.h"

void DrawChinese26(uint16_t X, uint16_t Y, OfficialScriptChineseFont_t ChineseCharacterIndex)
{
	DrawPicture(X, Y, 26, 26, (uint16_t *)GetOfficialScriptChineseFontPtr(ChineseCharacterIndex));
}
