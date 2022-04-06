/*
 * SSD1283.c
 *
 *  Created on: 2021Äê6ÔÂ4ÈÕ
 *      Author: h13
 *      note:   This screen driver has been streamlined for Simple UI.
 */

#include "SSD1283.h"
#include "RobotConfigs.h"

#include "SPI.h"
#include "Clock.h"

/**
 * @brief: Select the register you want to operate on.
 * @param: register.
 */
static void lcdSelectReg(uint8_t reg) {
	LCDResetCS();
	LCDResetDC();
	SPIWriteByte(reg);
	LCDSetCS();
}

/**
 * @brief: Write data to LCD.
 * @param: data.
 */
static void lcdWriteData(uint8_t Data) {
	LCDResetCS();
	LCDSetDC();
	SPIWriteByte(Data);
	LCDSetCS();
}

/**
 * @brief: Write 16-bit data to the LCD.
 * @param: data
 * @note:  STM32 is Little-Endian.
 */
static void writeData16(uint16_t data) {
	LCDResetCS();
	LCDSetDC();
	SPIWriteByte(data >> 8);
	SPIWriteByte(data);
	LCDSetCS();
}

/**
 * @brief: Write **Two** byte to register.
 * @param:
 * 		register
 * 		data
 */
static void lcdWriteCmd(uint8_t reg, uint16_t data) {
	lcdSelectReg(reg);
	writeData16(data);
}

/**
 * @brief: Reset LCD via RST pin.
 */
static void lcdReset(void) {
	LCDResetRST();
	SleepMillisecond(100);
	LCDSetRST();
	SleepMillisecond(50);
}

/**
 * @brief: Initializes the LCD
 * @note:  **Considering the possibility of multiple SPI devices, please manually initialize SPI before initializing LCD!!**
 */
void LCD_Init(void) {
	lcdReset(); /* Reset before LCD Init. */
	lcdWriteCmd(0x10, 0x2F8E); /* power control 1 */
	lcdWriteCmd(0x11, 0x000C); /* power control 2 */
	lcdWriteCmd(0x07, 0x0021); /* display control */
	lcdWriteCmd(0x28, 0x0006); /* vcom OTP */
	lcdWriteCmd(0x28, 0x0005);
	lcdWriteCmd(0x27, 0x057F); /* further bias current setting */
	lcdWriteCmd(0x29, 0x89A1); /* vcom OTP */
	lcdWriteCmd(0x00, 0x0001); /* OSC en */
	SleepMillisecond(100);
	lcdWriteCmd(0x29, 0x80B0); /* vcom OTP */
	SleepMillisecond(30);
	lcdWriteCmd(0x29, 0xFFFE); /* vcom OTP */
	lcdWriteCmd(0x07, 0x0023); /* display control */
	SleepMillisecond(30);
	lcdWriteCmd(0x07, 0x0033); /* display control */
#if (LCD_Rotation == LCD_Rotate_0)
	lcdWriteCmd(0x01, 0x2183); /* driver output control, REV, TB, RGB */
	lcdWriteCmd(0x03, 0x6830); /* entry mode, 65K, ram, ID0 */
#elif(LCD_Rotation == LCD_Rotate_90)
	lcdWriteCmd(0x01, 0x2283);
	lcdWriteCmd(0x03, 0x6808);
#elif(LCD_Rotation == LCD_Rotate_180)
	lcdWriteCmd(0x01, 0x2183);
	lcdWriteCmd(0x03, 0x6800);
#elif(LCD_Rotation == LCD_Rotate_270)
	lcdWriteCmd(0x01, 0x2283);
	lcdWriteCmd(0x03, 0x6838);
#else
#error "Rotation Config Error!"
#endif
	lcdWriteCmd(0x2F, 0xFFFF); /* 2A ~ 2F, test */
	lcdWriteCmd(0x2C, 0x8000);
	lcdWriteCmd(0x27, 0x0570);
	lcdWriteCmd(0x02, 0x0300); /* driving wave form control */
	lcdWriteCmd(0x0B, 0x580C); /* frame cycle control */
	lcdWriteCmd(0x12, 0x0609); /* power control 3 */
	lcdWriteCmd(0x13, 0x3100); /* power control 4 */
	Fill_Screen(WHITE);
}

/**
 * @brief: Sets the ROI that needs to be drawn
 * @param:
 * 		xStart: X-axis Start Point.
 * 		yStart: Y-axis Start Point.
 * 		xEnd:   X-axis End Point.
 * 		yEnd:   Y-axis End Point.
 */
void LCD_SetRegion(uint8_t xStart, uint8_t yStart, uint8_t xEnd, uint8_t yEnd)
{
#if(LCD_Rotation == LCD_Rotate_0)
	lcdSelectReg(0x44);
	lcdWriteData(xEnd + 2);
	lcdWriteData(xStart + 2);

	lcdSelectReg(0x45);
	lcdWriteData(yEnd + 2);
	lcdWriteData(yStart + 2);

	lcdSelectReg(0x21);
	lcdWriteData(yStart + 2);
	lcdWriteData(xStart + 2);

#elif(LCD_Rotation == LCD_Rotate_90)
	lcdSelectReg(0x44);
	lcdWriteData(LCD_Height - yStart + 1);
	lcdWriteData(LCD_Height - yEnd + 1);

	lcdSelectReg(0x45);
	lcdWriteData(LCD_Width - xStart - 1);
	lcdWriteData(LCD_Width - xEnd - 1);

	lcdSelectReg(0x21);
	lcdWriteData(LCD_Width - xStart - 1);
	lcdWriteData(LCD_Height - yStart + 1);
#elif(LCD_Rotation == LCD_Rotate_180)
	lcdSelectReg(0x44);
	lcdWriteData(LCD_Height - xStart + 1);
	lcdWriteData(LCD_Height - xEnd + 1);

	lcdSelectReg(0x45);
	lcdWriteData(LCD_Width - yStart + 1);
	lcdWriteData(LCD_Width - yEnd + 1);

	lcdSelectReg(0x21);
	lcdWriteData(LCD_Height - yStart + 1);
	lcdWriteData(LCD_Width - xStart + 1);
#elif(LCD_Rotation == LCD_Rotate_270)
	lcdSelectReg(0x44);
	lcdWriteData(yEnd + 2);
	lcdWriteData(yStart + 2);

	lcdSelectReg(0x45);
	lcdWriteData(xEnd);
	lcdWriteData(xStart);

	lcdSelectReg(0x21);
	lcdWriteData(xStart);
	lcdWriteData(yStart + 2);
#endif
	lcdSelectReg(0x22);
}

/**
 * @brief: Flush LCD With data.
 * @param:
 * 		xStart: X-axis Start Point.
 * 		yStart: Y-axis Start Point.
 * 		xEnd:   X-axis End Point.
 * 		yEnd:   Y-axis End Point.
 * 		data:   Pointer of data.
 * 		len:    Length of data.
 */
void LCD_Flush(uint8_t xStart, uint8_t yStart, uint8_t xEnd, uint8_t yEnd, uint16_t *data, uint16_t len)
{
#if (Rotation == 0)
	lcdSelectReg(0x44);
	lcdWriteData(xEnd + 2);
	lcdWriteData(xStart + 2);

	lcdSelectReg(0x45);
	lcdWriteData(yEnd + 2);
	lcdWriteData(yStart + 2);

	lcdSelectReg(0x21);
	lcdWriteData(yStart + 2);
	lcdWriteData(xStart + 2);

#elif(Rotation == 1)
	lcdSelectReg(0x44);
	lcdWriteData(LCD_Height - yStart + 1);
	lcdWriteData(LCD_Height - yEnd + 1);

	lcdSelectReg(0x45);
	lcdWriteData(LCD_Width - xStart - 1);
	lcdWriteData(LCD_Width - xEnd - 1);

	lcdSelectReg(0x21);
	lcdWriteData(LCD_Width - xStart - 1);
	lcdWriteData(LCD_Height - yStart + 1);
#elif(Rotation == 2)
	lcdSelectReg(0x44);
	lcdWriteData(LCD_Height - xStart + 1);
	lcdWriteData(LCD_Height - xEnd + 1);

	lcdSelectReg(0x45);
	lcdWriteData(LCD_Width - yStart + 1);
	lcdWriteData(LCD_Width - yEnd + 1);

	lcdSelectReg(0x21);
	lcdWriteData(LCD_Height - yStart + 1);
	lcdWriteData(LCD_Width - xStart + 1);
#elif(Rotation == 3)
	lcdSelectReg(0x44);
	lcdWriteData(yEnd + 2);
	lcdWriteData(yStart + 2);

	lcdSelectReg(0x45);
	lcdWriteData(xEnd);
	lcdWriteData(xStart);

	lcdSelectReg(0x21);
	lcdWriteData(xStart);
	lcdWriteData(yStart + 2);
#endif
	lcdSelectReg(0x22);

	for(uint16_t i = 0; i < len; i++)
	{
		writeData16(*data);
		data ++;
	}
}

/**
 * @brief: Fill the screen
 * @param: Color.
 */
void Fill_Screen(uint16_t color) {
	unsigned int i, m;
	LCD_SetRegion(0, 0, LCD_Width - 1, LCD_Height - 1);
	for (i = 0; i < LCD_Width; i++)
	{
		for (m = 0; m < LCD_Height; m++)
		{
			writeData16(color);
		}
	}
}

/**
 * @brief: Clear the screen.
 */
void LCD_Clear(void)
{
	Fill_Screen(BLACK);
}

void LCD_Write16(uint16_t *DataPtr, uint16_t Length)
{
	for(uint16_t i = 0; i < Length; i++)
	{
		writeData16(*DataPtr);
		DataPtr ++;
	}
}
