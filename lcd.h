#ifndef LCD_H
#define LCD_H

#include <pio/pio.h>
#include <memories\spi-flash\spid.h>
//#include "dataflash.h"

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

#define LCD_X_LENGTH        128
#define LCD_Y_LENGTH        128
#define LCD_TOTAL_COLUMN    128
#define LCD_TOTAL_PAGE      32
#define LCD_PIXELPERPAGE    4
#define LCD_COLUMN_OFFSET   32

#define LCD_VBIAS_DEFAULT   31
#define LCD_VBIAS_MINIMUM   15
#define LCD_VBIAS_MAXIMUM   55
#define LCD_VBIAS_ADDRESS                   (unsigned int)((unsigned int)DF_LCD_VBIAS_PAGE*(unsigned int)DF_PAGE_SIZE)

/// The LCD driver is currently in use.
#define LCD_ERROR_LOCK         1
/// There was an error with the SPI driver.
#define LCD_ERROR_SPI          2

/// LCD SPI CSR settings given MCK and SPCK.
#define LCD_CSR(mck, spck) (SPID_CSR_DLYBS(mck, 1000) | SPID_CSR_SCBR(mck, spck) | AT91C_SPI_CPOL)

#define LCD_SET_COL_ADDR_LSB                0x00
#define LCD_SET_COL_ADDR_MSB                0x10

#define LCD_SET_TEMP_COMP_5                 0x24
#define LCD_SET_TEMP_COMP_10                0x25
#define LCD_SET_TEMP_COMP_15                0x26
#define LCD_SET_TEMP_COMP_20                0x27

#define LCD_SET_PANEL_LOADING_16            0x28
#define LCD_SET_PANEL_LOADING_21            0x29
#define LCD_SET_PANEL_LOADING_28            0x2A
#define LCD_SET_PANEL_LOADING_38            0x2B

#define LCD_SET_PUMP_CONTROL_EX             0x2C
#define LCD_SET_PUMP_CONTROL_6X             0x2D
#define LCD_SET_PUMP_CONTROL_7X             0x2E
#define LCD_SET_PUMP_CONTROL_8X             0x2F

#define LCD_SET_SCROLL_LSB                  0x40
#define LCD_SET_SCROLL_MSB                  0x50

#define LCD_SET_PAGE_ADDRESS                0x60

#define LCD_SET_VBIAS                       0x81

#define LCD_SET_PARTIAL_DISP_DIS            0x84
#define LCD_SET_PARTIAL_DISP_EN1            0x86
#define LCD_SET_PARTIAL_DISP_EN2            0x87

#define LCD_SET_RAM_ADDRESS_AUTOWRAP_EN     0x89
#define LCD_SET_RAM_ADDRESS_AUTOWRAP_DIS    0x88
#define LCD_SET_RAM_ADDRESS_AUTOINC_COL     0x88
#define LCD_SET_RAM_ADDRESS_AUTOINC_ROW     0x8A
#define LCD_SET_RAM_ADDRESS_PID_PLUS        0x88
#define LCD_SET_RAM_ADDRESS_PID_MINUS       0x8C

#define LCD_SET_FIXED_LINES                 0x90

#define LCD_SET_LINE_RATE_121               0xA0
#define LCD_SET_LINE_RATE_134               0xA1
#define LCD_SET_LINE_RATE_147               0xA2
#define LCD_SET_LINE_RATE_166               0xA3

#define LCD_SET_ALL_PIXEL_ON_EN             0xA5
#define LCD_SET_ALL_PIXEL_ON_DIS            0xA4

#define LCD_SET_INVERSE_DISP_EN             0xA7
#define LCD_SET_INVERSE_DISP_DIS            0xA6

#define LCD_SET_DISP_ENABLE                 0xAF
#define LCD_SET_DISP_DISABLE                0xAE

#define LCD_SET_LCD_MAPPING_FL_EN           0xC1
#define LCD_SET_LCD_MAPPING_FL_DIS          0xC0
#define LCD_SET_LCD_MAPPING_MX_EN           0xC2
#define LCD_SET_LCD_MAPPING_MX_DIS          0xC0
#define LCD_SET_LCD_MAPPING_MY_EN           0xC4
#define LCD_SET_LCD_MAPPING_MY_DIS          0xC0

#define LCD_SET_LCD_GRAY_SHADE_24PCT        0xD0
#define LCD_SET_LCD_GRAY_SHADE_29PCT        0xD1
#define LCD_SET_LCD_GRAY_SHADE_36PCT        0xD2
#define LCD_SET_LCD_GRAY_SHADE_40PCT        0xD3

#define LCD_SYSTEM_RESET                    0xE2

#define LCD_NOP                             0xE3

#define LCD_SET_LCD_BIAS_RATIO_5            0xE8
#define LCD_SET_LCD_BIAS_RATIO_10           0xE9
#define LCD_SET_LCD_BIAS_RATIO_11           0xEA
#define LCD_SET_LCD_BIAS_RATIO_12           0xEB

#define LCD_RESET_CURSOR_UPDATE_MODE        0xEE
#define LCD_SET_CURSOR_UPDATE_MODE          0xEF

#define LCD_SET_COM_END                     0xF1

#define LCD_SET_PARTIAL_DISP_START          0xF2
#define LCD_SET_PARTIAL_DISP_END            0xF3

#define LCD_SET_WINDOW_PROG_START_COL       0xF4
#define LCD_SET_WINDOW_PROG_START_PAGE      0xF5
#define LCD_SET_WINDOW_PROG_END_COL         0xF6
#define LCD_SET_WINDOW_PROG_END_PAGE        0xF7

#define LCD_ENABLE_WINDOW_PROGRAM           0xF9
#define LCD_DISABLE_WINDOW_PROGRAM          0xF8

//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------

typedef struct {
    Spid *pSpid;
    SpidCmd command;
    unsigned char pCmdBuffer[2];
} Lcd;

//------------------------------------------------------------------------------
/// Exported Functions
//------------------------------------------------------------------------------

unsigned char LCD_IsBusy(Lcd *pLcd);

void LCD_Wait(Lcd *pLcd);

unsigned char LCD_SendCommand(
    Lcd *pLcd,
    unsigned char cmd,
    unsigned char argument,
    unsigned char cmdSize);

unsigned char LCD_SendData(
    Lcd *pLcd,
    unsigned char *pData,
    unsigned short dataSize);

unsigned char LCD_Configure(Lcd *pLcd, Spid *pSpid, unsigned char spiCs);

#endif
