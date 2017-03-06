#ifndef LCDD_H
#define LCDD_H

#include <memories\spi-flash\spid.h>
#include "lcd.h"
#include "font.h"
#include "bitmap.h"

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------
#define LCDD_SCROLL_DELAY   500
#define LCDD_SCROLL_DELAY_DALI   1500
//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------

typedef struct {
    Lcd lcd;
    const FONT *pFont;
    unsigned char LcddFrameBuffer[LCD_TOTAL_PAGE][LCD_TOTAL_COLUMN];
    unsigned char refresh;
} Lcdd;

//------------------------------------------------------------------------------
/// Exported Variables
//------------------------------------------------------------------------------
extern Lcdd lcdd;

//------------------------------------------------------------------------------
/// Exported Functions
//------------------------------------------------------------------------------

unsigned char LCDD_Initialize(Spid *pSpid, unsigned char spiCs);

void LCDD_ClearRect(
    unsigned char xStart,
    unsigned char yStart,
    unsigned char xEnd,
    unsigned char yEnd,
    unsigned char bgColor);

void LCDD_InvertRect(
    unsigned char xStart,
    unsigned char yStart,
    unsigned char xEnd,
    unsigned char yEnd,
    unsigned char bgColor);

//return the character width
unsigned char LCDD_PutChar(
    unsigned char xOffset,
    unsigned char yOffset,
    const unsigned char asciip,
    unsigned char bgColor,
    unsigned char test);

//return the string total height
unsigned char LCDD_PutString(
    unsigned char xOffset,
    unsigned char yOffset,
    unsigned char maxWidth,
    const unsigned char *pString,
    unsigned char bgColor,
    unsigned char test);

// 2.4.0
//return whether string length exceeds max width
unsigned char LCDD_PutScrollString(
    unsigned char xOffset,
    unsigned char yOffset,
    unsigned char maxWdith,
    const unsigned char *pString,
    unsigned char bgColor,
    unsigned char scroll);

unsigned char LCDD_PutBitmap(
    unsigned char xOffset,
    unsigned char yOffset,
    const BitmapInfo *pBitmapInfo,
    unsigned char invert);

void LCDD_Reset(void);

void LCDD_Refresh(void);

#endif
