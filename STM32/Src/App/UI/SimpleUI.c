/*
 * SimpleUI.c
 *
 *  Created on: 2021Äê10ÔÂ23ÈÕ
 *      Author: h13
 */

#include "SimpleUI.h"
#include "SSD1283.h"
void DrawPicture(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t *DataPtr)
{
	LCDSetRegion(X, Y, X + Width - 1, Y + Height - 1);
	LCD_Write16(DataPtr, Width * Height);
}
