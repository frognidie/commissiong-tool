
#include <board.h>
#include <string.h>
#include <pio/pio.h>
#include <memories\spi-flash\spid.h>
#include "dataflash.h"
#include "lcd.h"

static const Pin pinLcdDNC = PIN_LCD_DNC;
static const Pin pinLcdNCS = PIN_LCD_NCS;

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------

unsigned char LCD_IsBusy(Lcd *pLcd)
{
    return SPID_IsBusy(pLcd->pSpid);
}

void LCD_Wait(Lcd *pLcd)
{
    while (LCD_IsBusy(pLcd))
    {
        SPID_Handler(pLcd->pSpid);
    }
}

unsigned char LCD_SendCommand(
    Lcd *pLcd,
    unsigned char cmd,
    unsigned char argument,
    unsigned char cmdSize)
{
    SpidCmd *pCommand;

    if (LCD_IsBusy(pLcd))
    {
        return LCD_ERROR_LOCK;
    }

    PIO_Clear(&pinLcdDNC);

    pLcd->pCmdBuffer[0] = cmd;
    pLcd->pCmdBuffer[1] = argument;

    pCommand = &(pLcd->command);
    pCommand->cmdSize = cmdSize;
    pCommand->pData = 0;
    pCommand->dataSize = 0;
    pCommand->callback = 0;
    pCommand->pArgument = 0;

    if (SPID_SendCommand(pLcd->pSpid, pCommand)) {

        return LCD_ERROR_SPI;
    }

    return 0;
}

unsigned char LCD_SendData(
    Lcd *pLcd,
    unsigned char *pData,
    unsigned short dataSize)
{
    SpidCmd *pCommand;

    if (LCD_IsBusy(pLcd))
    {
        return LCD_ERROR_LOCK;
    }

    PIO_Set(&pinLcdDNC);

    pLcd->pCmdBuffer[0] = 0;
    pLcd->pCmdBuffer[1] = 0;

    pCommand = &(pLcd->command);
    pCommand->cmdSize = 0;
    pCommand->pData = pData;
    pCommand->dataSize = dataSize;
    pCommand->callback = 0;
    pCommand->pArgument = 0;

    if (SPID_SendCommand(pLcd->pSpid, pCommand)) {

        return LCD_ERROR_SPI;
    }

    return 0;
}

unsigned char LCD_Configure(Lcd *pLcd, Spid *pSpid, unsigned char spiCs)
{
    SpidCmd *pCommand;
    unsigned char vbias;

    pLcd->pSpid = pSpid;
    memset(pLcd->pCmdBuffer, 0, 2);

    PIO_Configure(&pinLcdDNC, 1);
    PIO_Configure(&pinLcdNCS, 1);

    pCommand = &(pLcd->command);
    pCommand->pCmd = pLcd->pCmdBuffer;
    pCommand->callback = 0;
    pCommand->pArgument = 0;
    pCommand->spiCs = spiCs;

    DF_Read((unsigned char *)&vbias, 1, LCD_VBIAS_ADDRESS);
    if ((vbias < LCD_VBIAS_MINIMUM) || (vbias > LCD_VBIAS_MAXIMUM))
    {
        vbias = LCD_VBIAS_DEFAULT;
    }

    LCD_SendCommand(pLcd, LCD_SYSTEM_RESET, 0, 1);
    LCD_Wait(pLcd);
    //TBD: Wait(10);
    LCD_SendCommand(pLcd, LCD_SET_TEMP_COMP_10, 0, 1);
    LCD_Wait(pLcd);
    LCD_SendCommand(pLcd, LCD_SET_LINE_RATE_121, 0, 1);
    LCD_Wait(pLcd);
    LCD_SendCommand(pLcd, LCD_SET_PANEL_LOADING_28, 0, 1);
    LCD_Wait(pLcd);
    LCD_SendCommand(pLcd, LCD_SET_PUMP_CONTROL_8X, 0, 1);
    LCD_Wait(pLcd);
    LCD_SendCommand(pLcd, LCD_SET_LCD_BIAS_RATIO_12, 0, 1);
    LCD_Wait(pLcd);
    LCD_SendCommand(pLcd, LCD_SET_VBIAS, vbias, 2);
    LCD_Wait(pLcd);
    LCD_SendCommand(pLcd, LCD_SET_RAM_ADDRESS_AUTOWRAP_EN | LCD_SET_RAM_ADDRESS_AUTOINC_COL | LCD_SET_RAM_ADDRESS_PID_PLUS, 0, 1);
    LCD_Wait(pLcd);
    LCD_SendCommand(pLcd, LCD_SET_LCD_MAPPING_FL_DIS | LCD_SET_LCD_MAPPING_MX_EN | LCD_SET_LCD_MAPPING_MY_EN, 0, 1);
    LCD_Wait(pLcd);
    LCD_SendCommand(pLcd, LCD_SET_LCD_GRAY_SHADE_29PCT, 0, 1);
    LCD_Wait(pLcd);
    LCD_SendCommand(pLcd, LCD_SET_WINDOW_PROG_START_COL, 32, 2);
    LCD_Wait(pLcd);
    LCD_SendCommand(pLcd, LCD_SET_WINDOW_PROG_END_COL, 159, 2);
    LCD_Wait(pLcd);
    LCD_SendCommand(pLcd, LCD_ENABLE_WINDOW_PROGRAM, 0, 1);
    LCD_Wait(pLcd);

    return 0;
}
