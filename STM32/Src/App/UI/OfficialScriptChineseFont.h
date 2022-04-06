/*
 * OfficialScriptChineseFont.h
 *
 *  Created on: 2022年3月26日
 *      Author: h13
 */

#ifndef APP_UI_OFFICIALSCRIPTCHINESEFONT_H_
#define APP_UI_OFFICIALSCRIPTCHINESEFONT_H_

typedef enum
{
	OfficialScriptChsPing  = 0,      //Chinese 苹
    OfficialScriptChsGuo   = 1,      //Chinese 果
    OfficialScriptChsXiang = 2,      //Chinese 香
    OfficialScriptChsJiao  = 3,      //Chinese 蕉
    OfficialScriptChsMi    = 4,      //Chinese 猕
    OfficialScriptChsHou   = 5,      //Chinese 猴
    OfficialScriptChsTao   = 6,      //Chinese 桃
    OfficialScriptChsNing  = 7,      //Chinese 柠
    OfficialScriptChsMeng  = 8,      //Chinese 檬
    OfficialScriptChsJu    = 9,      //Chinese 橘
    OfficialScriptChsZi    = 10,     //Chinese 子
    OfficialScriptChsLi    = 11,     //Chinese 梨
    OfficialScriptChsHuo   = 12,     //Chinese 火
    OfficialScriptChsLong  = 13,     //Chinese 龙
    OfficialScriptChsXue   = 14,     //Chinese 雪

    OfficialScriptChsYi    = 15,     //Chinese 一
	OfficialScriptChsLiang = 16,     //Chinese 两
    OfficialScriptChsSan   = 17,     //Chinese 三
    OfficialScriptChsSi    = 18,     //Chinese 四
    OfficialScriptChsWu    = 19,     //Chinese 五
    OfficialScriptChsLiu   = 20,     //Chinese 六
    OfficialScriptChsQi    = 21,     //Chinese 七
    OfficialScriptChsGe    = 22,     //Chinese 个
} OfficialScriptChineseFont_t;

uint8_t *GetOfficialScriptChineseFontPtr(OfficialScriptChineseFont_t ChineseCharacterIndex);

#endif /* APP_UI_OFFICIALSCRIPTCHINESEFONT_H_ */
