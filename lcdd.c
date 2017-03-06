
#include <memories\spi-flash\spid.h>
//#include "lcd.h"
//#include "font.h"
#include "lcdd.h"

//------------------------------------------------------------------------------
//         Global variables
//------------------------------------------------------------------------------

__no_init Lcdd lcdd;

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

//unsigned char LcdFrameBuffer[LCD_PAGE][LCD_COLUMN] = {0};

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------

unsigned char LCDD_Initialize(Spid *pSpid, unsigned char spiCs)
{
    unsigned char result;
    Lcd *pLcd = &(lcdd.lcd);

    result = LCD_Configure(pLcd, pSpid, spiCs);

    lcdd.pFont = &FontTrebuchetMS16;

    LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

    lcdd.refresh = 1;

    return result;
}

void LCDD_ClearRect(
    unsigned char xStart,
    unsigned char yStart,
    unsigned char xEnd,
    unsigned char yEnd,
    unsigned char bgColor)
{
    unsigned char quotient, remainder, i;

    for (; yStart<=yEnd; yStart++)
    {
        quotient = yStart/4;
        remainder = yStart%4;

        for (i=xStart; i<=xEnd; i++)
        {
            lcdd.LcddFrameBuffer[quotient][i] &= ~(3 << (remainder << 1));
            lcdd.LcddFrameBuffer[quotient][i] |= (bgColor << (remainder << 1));
        }
    }

    lcdd.refresh = 1;
}

void LCDD_InvertRect(
    unsigned char xStart,
    unsigned char yStart,
    unsigned char xEnd,
    unsigned char yEnd,
    unsigned char bgColor)
{
    unsigned char quotient, remainder, i;

    for (; yStart<=yEnd; yStart++)
    {
        quotient = yStart/4;
        remainder = yStart%4;

        for (i=xStart; i<=xEnd; i++)
        {
            if ((lcdd.LcddFrameBuffer[quotient][i] & (3 << (remainder << 1))) == 0)
            {
                lcdd.LcddFrameBuffer[quotient][i] |= (bgColor << (remainder << 1));
            }
            else
            {
                lcdd.LcddFrameBuffer[quotient][i] &= ~(3 << (remainder << 1));
            }
        }
    }

    lcdd.refresh = 1;
}

unsigned char LCDD_PutChar(
    unsigned char xOffset,
    unsigned char yOffset,
    const unsigned char asciip,
    unsigned char bgColor,
    unsigned char test)
{
    const FONT *pFont = lcdd.pFont;
    const FONT_CHARINFO *pCharInfo = 0;
    unsigned char data, page, offset, i, j;

    pCharInfo = FontGetCharInfo(pFont, asciip);

    if (!test)
    {
        for (j=0; j<pFont->Height; j++)
        {
            page = (yOffset+j)/4;
            offset = ((yOffset+j)%4)<<1;

            for (i=0; i<pCharInfo->Width; i++)
            {
                data = *(pCharInfo->paData + j*pCharInfo->BytesPerLine + i/8);
                data >>= 7-i%8;
                data &= 1;
                if (data)   //this is a character pixel
                {
                    data = 3;
                    if (bgColor)
                    {
                        data = 0;   //reverse display
                    }
                }
                else    //this is a background pixel
                {
                    data = bgColor;
                }
                data <<= offset;
                lcdd.LcddFrameBuffer[page][xOffset+i] &= ~(0x03 << offset);
                lcdd.LcddFrameBuffer[page][xOffset+i] |= data;
            }
        }

        lcdd.refresh = 1;
    }

    return pCharInfo->Distance;
}

unsigned char LCDD_PutString(
    unsigned char xOffset,
    unsigned char yOffset,
    unsigned char maxWidth,
    const unsigned char *pString,
    unsigned char bgColor,
    unsigned char test)
{
    const FONT *pFont = lcdd.pFont;
    unsigned char xCurrent = xOffset;
    unsigned char yCurrent = yOffset;

    if (*pString == 0)
    {
        return 0;
    }

    //remove leading space if any
//    while (*pString == ' ')
//    {
//        pString++;
//    }

    while (*pString)
    {
        if ((maxWidth) && (FontGetNextWordWidth(pFont, pString) > xOffset+maxWidth-xCurrent))
        {
            xCurrent = xOffset;
            yCurrent += pFont->Space;
        }

        if (*pString == ' ')
        {
            xCurrent += LCDD_PutChar(xCurrent, yCurrent, ' ', bgColor, test);
            pString++;
        }
        else
        {
            while ((*pString) && (*pString != ' '))
            {
                xCurrent += LCDD_PutChar(xCurrent, yCurrent, *pString, bgColor, test);
                pString++;
            }
        }
    }

    return (yCurrent-yOffset+pFont->Space);
}

// 2.4.0
// xOffset is start position
unsigned char LCDD_PutScrollString(
    unsigned char xOffset,
    unsigned char yOffset,
    unsigned char maxWdith,
    const unsigned char *pString,
    unsigned char bgColor,
    unsigned char scroll)
{
    unsigned char xCurrent = xOffset;

    if (*pString == 0)
    {
        return 0;
    }

    while (*pString)
    {
        if (xCurrent+LCDD_PutChar(xCurrent, yOffset, *pString, bgColor, 1) <= xOffset+maxWdith)
        {
            xCurrent += LCDD_PutChar(xCurrent, yOffset, *pString, bgColor, 0);
            pString++;
        }
        else
        {
            break;
        }
    }

    if (*pString && scroll)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned char LCDD_PutBitmap(
    unsigned char xOffset,
    unsigned char yOffset,
    const BitmapInfo *pBitmapInfo,
    unsigned char invert)
{
    unsigned char data, page, offset, i, j;

    if (!pBitmapInfo)
    {
        return 0;
    }

    for (j=0; j<pBitmapInfo->Height; j++)
    {
        page = (yOffset+j)/4;
        offset = ((yOffset+j)%4)<<1;

        for (i=0; i<pBitmapInfo->Width; i++)
        {
            data = *(pBitmapInfo->paData + j*pBitmapInfo->BytesPerLine + i/8);
            data >>= 7-i%8;
            if (invert)
            {
                data ^= 1;
            }
            data &= 1;
            data *= 3;
            data <<= offset;
            lcdd.LcddFrameBuffer[page][xOffset+i] &= ~(0x03 << offset);
            lcdd.LcddFrameBuffer[page][xOffset+i] |= data;
        }
    }

    lcdd.refresh = 1;

    return pBitmapInfo->Height;
}

void LCDD_Reset(void)
{
    Lcd *pLcd = &(lcdd.lcd);

    LCD_SendCommand(pLcd, LCD_SYSTEM_RESET, 0, 1);
    LCD_Wait(pLcd);
}

void LCDD_Refresh(void)
{
    Lcd *pLcd = &lcdd.lcd;
    unsigned char *pLcddFrameBuffer = &lcdd.LcddFrameBuffer[0][0];

    //Set column address to 32, very left column
    LCD_SendCommand(pLcd, LCD_SET_COL_ADDR_LSB | 0x00, 0, 1);
    LCD_Wait(pLcd);
    LCD_SendCommand(pLcd, LCD_SET_COL_ADDR_MSB | 0x02, 0, 1);
    LCD_Wait(pLcd);

    //Set page address to 0, very top page
    LCD_SendCommand(pLcd, LCD_SET_PAGE_ADDRESS | 0x00, 0, 1);
    LCD_Wait(pLcd);

    //Write the whole LCDDFrameBuffer to the LCD
    LCD_SendData(pLcd, pLcddFrameBuffer, LCD_TOTAL_PAGE*LCD_TOTAL_COLUMN);
    LCD_Wait(pLcd);

    //Enable display
    LCD_SendCommand(pLcd, LCD_SET_DISP_ENABLE, 0, 1);
    LCD_Wait(pLcd);

    lcdd.refresh = 0;
}
