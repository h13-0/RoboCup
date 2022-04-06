/*
 * SSD1283.h
 *
 *  Created on: 2021Äê6ÔÂ4ÈÕ
 *      Author: h13
 */

#ifndef SSD1283_SSD1283_H_
#define SSD1283_SSD1283_H_

#include <stdint.h>

/**
 * @group: Colors
 */
#define RED  		(0xf800)
#define GREEN		(0x07e0)
#define BLUE 		(0x001f)
#define SBLUE 	    (0x251F)
#define WHITE		(0xffff)
#define BLACK		(0x0000)
#define YELLOW      (0xFFE0)
#define GRAY0       (0xEF7D)
#define GRAY1       (0x8410)
#define GRAY2       (0x4208)

/**
 * @brief: Initializes the LCD
 * @note:  **Considering the possibility of multiple SPI devices, please manually initialize SPI before initializing LCD!!**
 */
void LCD_Init(void);

/**
 * @brief: Sets the ROI that needs to be drawn
 * @param:
 * 		xStart: X-axis Start Point.
 * 		yStart: Y-axis Start Point.
 * 		xEnd:   X-axis End Point.
 * 		yEnd:   Y-axis End Point.
 */
void LCD_SetRegion(uint8_t xStart, uint8_t yStart, uint8_t xEnd, uint8_t yEnd);

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
void LCD_Flush(uint8_t xStart, uint8_t yStart, uint8_t xEnd, uint8_t yEnd, uint16_t *data, uint16_t len);

/**
 * @brief: Fill the screen
 * @param: Color.
 */
void Fill_Screen(uint16_t color);

/**
 * @brief: Clear the screen.
 */
void LCD_Clear(void);

void LCD_Write16(uint16_t *DataPtr, uint16_t Length);

#endif /* PLATFORM_SSD1283_SSD1283_H_ */
