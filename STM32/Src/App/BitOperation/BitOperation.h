/*
 * BitOperation.h
 *
 *  CReated on: 2021Äê6ÔÂ8ÈÕ
 *      Author: h13
 */

#ifndef APP_BITOPERATION_BITOPERATION_H_
#define APP_BITOPERATION_BITOPERATION_H_

/**
 * @group: 8Bit Variable Operations.
 */
#define Get8Bit0(var) (var&0x80)
#define Get8Bit1(var) (var&0x40)
#define Get8Bit2(var) (var&0x20)
#define Get8Bit3(var) (var&0x10)
#define Get8Bit4(var) (var&0x08)
#define Get8Bit5(var) (var&0x04)
#define Get8Bit6(var) (var&0x02)
#define Get8Bit7(var) (var&0x01)

#define Set8Bit0(var) (var|=0x80)
#define Set8Bit1(var) (var|=0x40)
#define Set8Bit2(var) (var|=0x20)
#define Set8Bit3(var) (var|=0x10)
#define Set8Bit4(var) (var|=0x08)
#define Set8Bit5(var) (var|=0x04)
#define Set8Bit6(var) (var|=0x02)
#define Set8Bit7(var) (var|=0x01)

#define Reset8Bit0(var) (var&=0x7f)
#define Reset8Bit1(var) (var&=0xbf)
#define Reset8Bit2(var) (var&=0xdf)
#define Reset8Bit3(var) (var&=0xef)
#define Reset8Bit4(var) (var&=0xf7)
#define Reset8Bit5(var) (var&=0xfb)
#define Reset8Bit6(var) (var&=0xfd)
#define Reset8Bit7(var) (var&=0xfe)

#endif /* APP_BITOPERATION_BITOPERATION_H_ */
