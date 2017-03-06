
#include <string.h>
#include <pwmc\pwmc.h>
#include <usart\usart.h>
#include <pio\pio.h>
#include <aic\aic.h>
#ifndef APP_BAES
#include "time.h"
#include "parameters.h"
#include "nitoo.h"
#endif  // #ifndef APP_BAES
#include "ir.h"
unsigned char xx_createzone;
unsigned char yy_createzone;
unsigned char zone_value;///
unsigned char xx_detector;
unsigned char detec_short_addres;
//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

#define IR_PWM_CPRE (0x5)   //Prescaler is MCK/32, results in 576KHz with MCK being 18.432MHz
#define IR_PWM_CALG (0x0 << 8)  //Waveform is left aligned
#define IR_PWM_CPOL (0x1 << 9)  //Waveform starts at a high level
//#define IR_PWM_CPD  (0x0 << 10) //Writing to PWM_CUPDx will modify the duty cycle at the next period start event
#define IR_PWM_CDTY 5   //
#define IR_PWM_CPRD 16  //

#define IR_US_MR    (AT91C_US_CHRL_8_BITS | AT91C_US_PAR_NONE | AT91C_US_NBSTOP_2_BIT | AT91C_US_INACK)
#define IR_US_BASE  AT91C_BASE_US0
#define IR_US_ID    AT91C_ID_US0

//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------

#ifndef APP_BAES
typedef struct {
    unsigned char TxBuffer[NITOO_MAX_FRAME_LENGTH];
    unsigned char TxBufSize;
    unsigned char RxBuffer[NITOO_MAX_FRAME_LENGTH];
    unsigned char RxBufSize;
} IR;
#else
typedef struct {
    unsigned char TxBuffer[5];
    unsigned char TxBufSize;
    unsigned char RxBuffer[5];
    unsigned char RxBufSize;
} IR;
#endif  // #ifndef APP_BAES

//typedef struct {
//    unsigned windows_value;
//    unsigned corridor_value;
//    unsigned board_value;
//    unsigned XX;
//} view_zone;

//------------------------------------------------------------------------------
/// Local Variables
//------------------------------------------------------------------------------

const Pin pinsIR[] = {PIN_USART0_RXD, PIN_USART0_TXD, PIN_IR_PWM};

static __no_init IR ir;
//
//typedef struct {
//    unsigned windows_value;
//    unsigned corridor_value;
//    unsigned board_value;
//    unsigned XX;
//} VIEW;
//
//
//
 __no_init VIEW view;
 
 __no_init GROUPNUM GroupNum;
 unsigned char RxDisBuffer[NITOO_INDICATION_PARAMETER_FRAME_LENGTH]; 
volatile unsigned char timeout = 0;

#ifdef APP_BAES
char tab_trame_octet[2];

char parite=0;
#endif  // #ifdef APP_BAES

//------------------------------------------------------------------------------
/// Exported Variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Internal Functions
//------------------------------------------------------------------------------

static void _ir_send_frame(void)
{
    IR_US_BASE->US_TCR = 0;
    IR_US_BASE->US_TNCR = 0;

    USART_WriteBuffer(IR_US_BASE, &ir.TxBuffer[0], ir.TxBufSize);

    PWMC_EnableChannel(IR_PWM_CHANNEL);

    while ((IR_US_BASE->US_CSR & (AT91C_US_TXEMPTY | AT91C_US_TXBUFE)) != (AT91C_US_TXEMPTY | AT91C_US_TXBUFE));

    PWMC_DisableChannel(IR_PWM_CHANNEL);
}

static void _ir_timeout()
{
    unsigned int status = AT91C_BASE_US0->US_CSR;

    if ((status & AT91C_US_TIMEOUT) != 0)
    {
        IR_US_BASE->US_CR = AT91C_US_STTTO;
        timeout = 1;
    }
}

#ifdef APP_BAES
void _create_frame_BAES(unsigned char *pFrame,unsigned int Code,unsigned char Command)
{

    tab_trame_octet[1]=Code>>2;

    tab_trame_octet[0]=Code&0x03;
    if((tab_trame_octet[0]==0)||(tab_trame_octet[0]==3))
    {
          parite=0;
    }
    else if((tab_trame_octet[0]==1)||(tab_trame_octet[0]==2))
    {
          parite=1;
    }
    parite=parite<<2;
    Command=Command<<3;
    tab_trame_octet[0]|=Command|parite;
    pFrame[0] =tab_trame_octet[0];
    pFrame[1] =tab_trame_octet[1];
    pFrame[2] =pFrame[0]+pFrame[1];

}
#endif  // #ifdef APP_BAES

//------------------------------------------------------------------------------
/// Exported Functions
//------------------------------------------------------------------------------

void IR_Configure(void)
{
    PIO_Configure(pinsIR, PIO_LISTSIZE(pinsIR));

    AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PWMC;
    PWMC_ConfigureChannel(IR_PWM_CHANNEL, IR_PWM_CPRE, IR_PWM_CALG, IR_PWM_CPOL);
    PWMC_SetPeriod(IR_PWM_CHANNEL, IR_PWM_CPRD);
    PWMC_SetDutyCycle(IR_PWM_CHANNEL, IR_PWM_CDTY);

    AT91C_BASE_PMC->PMC_PCER = 1 << IR_US_ID;
    IR_US_BASE->US_IDR = 0xFFFFFFFF;
    USART_Configure(IR_US_BASE, IR_US_MR, 2400, BOARD_MCK);
    USART_SetTransmitterEnabled(IR_US_BASE, 1);
    USART_SetReceiverEnabled(IR_US_BASE, 1);
    AIC_ConfigureIT(IR_US_ID, 0, _ir_timeout);
    AIC_EnableIT(IR_US_ID);
}

#ifndef APP_BAES

void IR_SetSensorParameter(SENSOR_PARAMS *pParams, unsigned char ParamIndex)
{
    ir.TxBufSize = NITOO_CreateSetParameterFrame(ir.TxBuffer, pParams, ParamIndex);

    _ir_send_frame();
}

// return 0 if failed, 1 if good value received, 2 if unknown value, 3 if parameter not implemented
#pragma optimize=none
unsigned char IR_ReadSensorParameter(SENSOR_PARAMS *pParams, unsigned char ParamIndex)
{
    unsigned char result = 1;
    volatile unsigned char temp;
    unsigned short paramvalue;

    ir.TxBufSize = NITOO_CreateReadParameterFrame(ir.TxBuffer, pParams, ParamIndex);

    _ir_send_frame();

    TIME_Wait(1);

    IR_US_BASE->US_RCR = 0;
    IR_US_BASE->US_RNCR = 0;
    timeout = 0;

    temp = IR_US_BASE->US_RHR;

    ir.RxBufSize = NITOO_MAX_FRAME_LENGTH;
    memset(&ir.RxBuffer[0], 0, NITOO_MAX_FRAME_LENGTH);
    USART_ReadBuffer(IR_US_BASE, &ir.RxBuffer[0], ir.RxBufSize);

    IR_US_BASE->US_RTOR = IR_FRAMES_READ_WAIT;

    IR_US_BASE->US_IER = AT91C_US_TIMEOUT;

    while ((IR_US_BASE->US_CSR & AT91C_US_RXBUFF) == 0)
    {
              AT91C_BASE_WDTC->WDTC_WDCR = (0xA5 << 24) | AT91C_WDTC_WDRSTT;

        if (timeout == 1)   //timeout happened
        {
//            result = 0;
            timeout = 0;
            break;
        }
    }

    IR_US_BASE->US_RTOR = 0;

    IR_US_BASE->US_IDR = AT91C_US_TIMEOUT;
    
//    ///test
//        unsigned char TestBuffer[NITOO_MAX_FRAME_LENGTH]=\
//    //{0XAA,0X80,0XFF,0XFF,0XF0,0XF1,0X05,0XFF,0XFF,0X04,0X00,0X00,0X00,0X12};
//    {0XAA,0X80,0XFF,0XFF,0XF0,0Xf1,0X05,0Xff,0Xff,0x05,0x00,0X00,0X00,0Xa3};
//    //0   1    2    3    4     5    6   7    8    9    10    11   12   13
//                                      //xx   yy  zone
//    ///for debug
//    //ir.RxBuffer={0XAA,0X80,0XFF,0XFF,0XF0,0XF1,0X05,0X02,0X00,0X00,0X00,0X00,0X00,0X12};
//    for (temp=0; temp<NITOO_MAX_FRAME_LENGTH; temp++)
//    {
//      ir.RxBuffer[temp] = TestBuffer[temp];
//        
//    }
//    
//   /// test
    
     
    for (temp=0; temp<NITOO_MAX_FRAME_LENGTH; temp++)
    {
        if (ir.RxBuffer[temp] == NITOO_FRAME_PREAMBLE)
        {
            break;
        }
        else
        {
            if (temp >= NITOO_MAX_FRAME_LENGTH - NITOO_INDICATION_PARAMETER_FRAME_LENGTH)   // enough error
            {
                result = 0;
                break;
            }
        }
    }

    if (result == 1) //if data arrived successfully, validate it.
    {
        switch (NITOO_ValidateIndicationParameterFrame(&ir.RxBuffer[temp], pParams, ParamIndex))
        {
        case 0:     // crc error or incorrect frame received
        default:
            result = 0;     // crc error or incorrect frame received
            break;
        case 1:     // parameter value received, extract the value, validate it and store it
            paramvalue = NITOO_GetParameterValueFromIndicationFrame(&ir.RxBuffer[temp]);
            if (PARAMS_ValidateParameterValue(ParamIndex, paramvalue))
            {
                if( PARAMS_IsLightController(pParams ) && (ParamIndex == PARAM_INDEX_HOUR) && ( paramvalue > 23 ) ){
                   result = 2;
                }
                else{
                   PARAMS_SetParameterValue(pParams, ParamIndex, paramvalue);
                }
            }
            else
            {
                result = 2;     // unknown parameter value received
            }
            break;
        case 2:     // parameter ERROR frame received, check the value and do the relevant
            paramvalue = NITOO_GetParameterValueFromIndicationFrame(&ir.RxBuffer[temp]);
            switch (paramvalue)
            {
            case 0:
            default:
                result = 2;     // parameter ERROR frame received, but the value is reserved or for future usage (unknown)
                break;
            case 1:
                PARAMS_SetParameterNumber(pParams, ParamIndex, PARAM_NUMBER_ERROR);
//                PARAMS_SetParameterValue(pParams, ParamIndex, PARAM_VALUE_ERROR_NOT_IMPLEMENTED);
                result = 3;
                break;
            case 5:
              result = 5;
              break;
            }
            break;
        }
    }

    return result;  
}
///
unsigned char IR_ReadCreateZoneInfo(OPTIMES tempLanguage)
{
    unsigned char result = 1;
    volatile unsigned char temp;
    unsigned short paramvalue;

    TIME_Wait(1);

    IR_US_BASE->US_RCR = 0;
    IR_US_BASE->US_RNCR = 0;
    timeout = 0;

    temp = IR_US_BASE->US_RHR;

    ir.RxBufSize = NITOO_MAX_FRAME_LENGTH;
    memset(&ir.RxBuffer[0], 0, NITOO_MAX_FRAME_LENGTH);
    USART_ReadBuffer(IR_US_BASE, &ir.RxBuffer[0], ir.RxBufSize);

    IR_US_BASE->US_RTOR = IR_FRAMES_READ_WAIT;

    IR_US_BASE->US_IER = AT91C_US_TIMEOUT;
    unsigned int buttonTimeStamp=0;
    buttonTimeStamp = TIME_GetTime();
    while ((IR_US_BASE->US_CSR & AT91C_US_RXBUFF) == 0)
    {
              AT91C_BASE_WDTC->WDTC_WDCR = (0xA5 << 24) | AT91C_WDTC_WDRSTT;

        if (timeout == 1 ||  TIME_GetTime()>=buttonTimeStamp+1000)   //timeout happened
        {
//            result = 0;
            timeout = 0;
            break;
        }
    }

    IR_US_BASE->US_RTOR = 0;

    IR_US_BASE->US_IDR = AT91C_US_TIMEOUT;
    
//    unsigned char TestBuffer[NITOO_MAX_FRAME_LENGTH]=\
//    //{0XAA,0X80,0XFF,0XFF,0XF0,0XF1,0X05,0XFF,0XFF,0X04,0X00,0X00,0X00,0X12};
//    {0XAA,0X80,0XFF,0XFF,0XF0,0Xf0,0X3f,0Xff,0Xee,0X00,0X00,0X12};
//    //0   1    2    3    4     5    6   7    8    9    10    11   12   13
//                                      //xx   yy  zone
//    ///for debug
//    //ir.RxBuffer={0XAA,0X80,0XFF,0XFF,0XF0,0XF1,0X05,0X02,0X00,0X00,0X00,0X00,0X00,0X12};
//    for (temp=0; temp<NITOO_MAX_FRAME_LENGTH; temp++)
//    {
//      ir.RxBuffer[temp] = TestBuffer[temp];
//        
//    }
//    
    
#if defined TESTERROR
    //test error code
    unsigned char TestBuffer[NITOO_MAX_FRAME_LENGTH]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00,0x00};
    //{0XAA,0X80,0XFF,0XFF,0XF0,0XF1,0X05,0Xff,0Xff,0X03,0X00,0X00,0X00,0X12};
    //{0XAA,0X80,0XFF,0XFF,0XF0,0XF1,0X05,0XFF,0XFF,0X04,0X00,0X00,0X00,0X12};
    //{0xAA,0x81,0xFF,0xFF,0xF0,0xF0,0x3F,0x07,0x08,0x00,0x00,0x00,0x00,0x3C};
    //0   1    2    3    4     5    6   7    8    9    10    11   12   13
    //xx   yy  zone
    //for debug                                     data2 data3
    //ir.RxBuffer={0XAA,0X80,0XFF,0XFF,0XF0,0XF1,0X05,0Xff,0Xff,0X03,0X00,0X00,0X00,0X12};
    for (temp=0; temp<NITOO_MAX_FRAME_LENGTH; temp++)
    {
      ir.RxBuffer[temp] = TestBuffer[temp];
      
    }
#endif
       int sum;   
    for (temp=0; temp<NITOO_MAX_FRAME_LENGTH; temp++)
    {

       sum = ir.RxBuffer[temp];
        if (ir.RxBuffer[temp] == NITOO_FRAME_PREAMBLE)
        {
            break;
        }
        else
        {
          if(tempLanguage==FirstTime)
          {
            if ((temp >= NITOO_MAX_FRAME_LENGTH - NITOO_NODE_INDEX_FRAME_LENGTH) /*&& (sum != 0)*/)   // 20-14=6 20-12=8 enough error
            {
                result = 0;
                break;
            }
          }
          if(tempLanguage==SecondTime)
          {
            if (temp >= NITOO_MAX_FRAME_LENGTH - NITOO_NODE_INDEX_FRAME_LENGTH)
            {
              result = 10;
              return result;
            }
          }
        }
    }
    if(result==1)
    {
#ifndef TESTERROR    
      if (_verify_frame_crc(ir.RxBuffer, NITOO_INDICATION_PARAMETER_FRAME_LENGTH) == 0)
      {
        result=12;
        return result;
      }
      else
#endif
      {
        if ((ir.RxBuffer[5] == 0xF0) && (ir.RxBuffer[6] == 0x3F))// it is ok
        {
          xx_createzone=ir.RxBuffer[7];
          yy_createzone=ir.RxBuffer[8];
          zone_value=ir.RxBuffer[9];
          result = 10;
          return result;
        }
        
        if ((ir.RxBuffer[5] == CODE_INDICATION_MOTION_SENSOR_PARAMETER) && \
          (ir.RxBuffer[6] == CMD_INDICATION_MOTION_SENSOR_PARAMETER))
        {
          if ((ir.RxBuffer[7] == 0xFF) &&(ir.RxBuffer[8] == (0xFF00) >> 8))
          {
            if ((ir.RxBuffer[9] == 0x03) && (ir.RxBuffer[10] ==(0x00)))
              result= 1;
            else if ((ir.RxBuffer[9] == 0x04) && (ir.RxBuffer[10] ==(0x00)))
              result= 2;
            else if ((ir.RxBuffer[9] == 0x06) && (ir.RxBuffer[10] ==(0x00)))
              result= 6;
            else if ((ir.RxBuffer[9] == 0x07) && (ir.RxBuffer[10] ==(0x00)))
              result= 7;
            else if ((ir.RxBuffer[9] == 0x08) && (ir.RxBuffer[10] ==(0x00)))
              result= 8;
            else
              result=3;
          }
          else if ((ir.RxBuffer[7] == 0xFE) && (ir.RxBuffer[8] ==(0xFF00) >> 8))
          {
            if ((ir.RxBuffer[9] == 0x02) && (ir.RxBuffer[10] ==(0x00)))
              result= 4;
            else if ((ir.RxBuffer[9] == 0x03) && (ir.RxBuffer[10] ==(0x00)))
              result= 5;
            else
              result= 9; 
          }
          else
          {
            result= 11;
          }
        }
        else
        {
          result= 0;
        }
        
      }
    }


    return result;
}



unsigned char IR_Read_UNIT_DESCRIPTION_Info(unsigned char num)
{
    unsigned char result = 1;
    volatile unsigned char temp;
    unsigned short paramvalue;

    TIME_Wait(1);

    IR_US_BASE->US_RCR = 0;
    IR_US_BASE->US_RNCR = 0;
    timeout = 0;

    temp = IR_US_BASE->US_RHR;

    ir.RxBufSize = NITOO_MAX_FRAME_LENGTH;
    memset(&ir.RxBuffer[0], 0, NITOO_MAX_FRAME_LENGTH);
    USART_ReadBuffer(IR_US_BASE, &ir.RxBuffer[0], ir.RxBufSize);

    IR_US_BASE->US_RTOR = IR_FRAMES_READ_WAIT;

    IR_US_BASE->US_IER = AT91C_US_TIMEOUT;

    while ((IR_US_BASE->US_CSR & AT91C_US_RXBUFF) == 0)
    {
              AT91C_BASE_WDTC->WDTC_WDCR = (0xA5 << 24) | AT91C_WDTC_WDRSTT;

        if (timeout == 1)   //timeout happened
        {
//            result = 0;
            timeout = 0;
            break;
        }
    }

    IR_US_BASE->US_RTOR = 0;

    IR_US_BASE->US_IDR = AT91C_US_TIMEOUT;
    
//   unsigned char TestBuffer[NITOO_MAX_FRAME_LENGTH]=\
////    //{0XAA,0X80,0XFF,0XFF,0XF0,0XF1,0X05,0XFF,0XFF,0X04,0X00,0X00,0X00,0X12};
//    {0XAA,0X80,0XFF,0XFF,0XF0,0Xb0,0X0d,0X12,0X64,0X12};
//    //0   1    2    3    4     5    6   7    8    9    10    11   12   13
//    
//    ///for debug
//    //ir.RxBuffer={0XAA,0X80,0XFF,0XFF,0XF0,0XF1,0X05,0X02,0X00,0X00,0X00,0X00,0X00,0X12};
//    for (temp=0; temp<NITOO_MAX_FRAME_LENGTH; temp++)
//    {
//      ir.RxBuffer[temp] = TestBuffer[temp];
//        
//    }
//    
//    ir.RxBuffer[8]= ir.RxBuffer[8]+num;
//    
    
    
    
    for (temp=0; temp<NITOO_MAX_FRAME_LENGTH; temp++)
    {
        if (ir.RxBuffer[temp] == NITOO_FRAME_PREAMBLE)
        {
            break;
        }
        else
        {
            if (temp >= NITOO_MAX_FRAME_LENGTH - NITOO_UNIT_DESCIPTION_STATUS)   // 20-14=6 20-12=8 enough error
            {
                result = 0;
                break;
            }
        }
    }
    if(result==1)
    {
      if (_verify_frame_crc(ir.RxBuffer, NITOO_UNIT_DESCIPTION_STATUS) == 0)
      {
        result= 0;
      }
      else
      {
        if ((ir.RxBuffer[5] == 0xB0) && (ir.RxBuffer[6] == 0x0D))// it is ok
        {
          switch(num){
          //case 0x01:view.windows_value=0x65;
          case 0x01:view.windows_value=ir.RxBuffer[8];
                    break;
          case 0x02:view.corridor_value=ir.RxBuffer[8];
          //case 0x02:view.corridor_value=0x65;
                    break;
          case 0x03:view.board_value=ir.RxBuffer[8];
          //case 0x03:view.board_value=0x66;
                    break;         
           }
          result = 2;
          return result;
        }
        else
          result =3;
        
      }
    }


    return result;
}

////
unsigned char IR_Read_CONTROL_GEAR_Info(unsigned char num)
{
    unsigned char result = 1;
    volatile unsigned char temp;
    unsigned short paramvalue;

    TIME_Wait(1);

    IR_US_BASE->US_RCR = 0;
    IR_US_BASE->US_RNCR = 0;
    timeout = 0;

    temp = IR_US_BASE->US_RHR;

    ir.RxBufSize = NITOO_MAX_FRAME_LENGTH;
    memset(&ir.RxBuffer[0], 0, NITOO_MAX_FRAME_LENGTH);
    USART_ReadBuffer(IR_US_BASE, &ir.RxBuffer[0], ir.RxBufSize);

    IR_US_BASE->US_RTOR = IR_FRAMES_READ_WAIT;

    IR_US_BASE->US_IER = AT91C_US_TIMEOUT;

    while ((IR_US_BASE->US_CSR & AT91C_US_RXBUFF) == 0)
    {
              AT91C_BASE_WDTC->WDTC_WDCR = (0xA5 << 24) | AT91C_WDTC_WDRSTT;

        if (timeout == 1)   //timeout happened
        {
//            result = 0;
            timeout = 0;
            break;
        }
    }

    IR_US_BASE->US_RTOR = 0;

    IR_US_BASE->US_IDR = AT91C_US_TIMEOUT;
    
//    unsigned char TestBuffer[NITOO_MAX_FRAME_LENGTH]=\
//    //{0XAA,0X80,0XFF,0XFF,0XF0,0XF1,0X05,0XFF,0XFF,0X04,0X00,0X00,0X00,0X12};
//    {0XAA,0X80,0XFF,0XFF,0XF0,0Xb0,0X42,0X01,0Xee,0X12};
//    //0   1    2    3    4     5    6   7    8    9    10    11   12   13
//    int i;
//    for(i=0;i<NITOO_MAX_FRAME_LENGTH;i++)
//    {
//      ir.RxBuffer[i]=TestBuffer[i];
//    }
    
    
    for (temp=0; temp<NITOO_MAX_FRAME_LENGTH; temp++)
    {
        if (ir.RxBuffer[temp] == NITOO_FRAME_PREAMBLE)
        {
            break;
        }
        else
        {
            if (temp >= NITOO_MAX_FRAME_LENGTH - NITOO_UNIT_DESCIPTION_STATUS)   // 20-14=6 20-12=8 enough error
            {
                result = 0;
                break;
            }
        }
    }
    if(result==1)
    {
      if (_verify_frame_crc(ir.RxBuffer, NITOO_NODE_INDEX_FRAME_LENGTH) == 0 )
      {
        result= 0;
      }
      else
      {
        if ((ir.RxBuffer[5] == 0xB0) && (ir.RxBuffer[6] == 0x42))// it is ok
        {
//          view.XX=ir.RxBuffer[7];
//          result = 2;
//          return result;
          
          switch(num){
          case 0x01:view.win_ballast=ir.RxBuffer[7];
          break;
          case 0x02:view.cori_ballast=ir.RxBuffer[7];
          break;
          case 0x04:view.board_ballast=ir.RxBuffer[7];
          break;         
          }
          result = 2;
          return result;
        }
        
      }
    }


    return result;
}

////

unsigned char IR_ReadDetectorInfo(void)
{
    unsigned char result = 1;
    volatile unsigned char temp;
    unsigned short paramvalue;
    unsigned int buttonTimeStamp;
    TIME_Wait(1);

    IR_US_BASE->US_RCR = 0;
    IR_US_BASE->US_RNCR = 0;
    timeout = 0;

    temp = IR_US_BASE->US_RHR;

    ir.RxBufSize = NITOO_MAX_FRAME_LENGTH;
    memset(&ir.RxBuffer[0], 0, NITOO_MAX_FRAME_LENGTH);
    USART_ReadBuffer(IR_US_BASE, &ir.RxBuffer[0], ir.RxBufSize);

    IR_US_BASE->US_RTOR = IR_FRAMES_READ_WAIT;

    IR_US_BASE->US_IER = AT91C_US_TIMEOUT;

    while ((IR_US_BASE->US_CSR & AT91C_US_RXBUFF) == 0)
    {
              AT91C_BASE_WDTC->WDTC_WDCR = (0xA5 << 24) | AT91C_WDTC_WDRSTT;

        if (timeout == 1 ||  TIME_GetTime()>=buttonTimeStamp+1000)   //timeout happened
        {
//            result = 0;
            timeout = 0;
            break;
        }
    }

    IR_US_BASE->US_RTOR = 0;

    IR_US_BASE->US_IDR = AT91C_US_TIMEOUT;
    
    
//    //TEST array
//    unsigned char TestBuffer[NITOO_MAX_FRAME_LENGTH]=\
//    //{0XAA,0X80,0XFF,0XFF,0XF0,0XF1,0X05,0XFF,0XFF,0X04,0X00,0X00,0X00,0X12};  //error code
//    {0XAA,0X80,0XFF,0XFF,0XF0,0Xb0,0X4C,0Xff, 0Xff, 0X12};       // ok for group Num
//    //0   1    2    3    4     5    6     7    8    9    10    11   12   13
//                                      //data2 data3 crc
//                                             // group
//    
//    ///for debug
//    //ir.RxBuffer={0XAA,0X80,0XFF,0XFF,0XF0,0XF1,0X05,0X02,0X00,0X00,0X00,0X00,0X00,0X12};
//    for (temp=0; temp<NITOO_MAX_FRAME_LENGTH; temp++)
//    {
//      ir.RxBuffer[temp] = TestBuffer[temp];
//        
//    }  
    
    for (temp=0; temp<NITOO_MAX_FRAME_LENGTH; temp++)
    {
        if (ir.RxBuffer[temp] == NITOO_FRAME_PREAMBLE)
        {
            break;
        }
        else
        {
            if (temp >= NITOO_MAX_FRAME_LENGTH - NITOO_NODE_INDEX_FRAME_LENGTH)   // 20-14=6 20-12=8 enough error
            {
                result = 0;
                break;
            }
        }
    }
    if(result==1)
    {//for crc check
      if (_verify_frame_crc(ir.RxBuffer, NITOO_NODE_INDEX_FRAME_LENGTH) == 0 && _verify_frame_crc(ir.RxBuffer, NITOO_INDICATION_PARAMETER_FRAME_LENGTH) == 0)
      {
        result= 5;
      }
      else
      {
        if ((ir.RxBuffer[5] == 0xB0) && (ir.RxBuffer[6] == 0x4C))// it is ok
        {
          detec_short_addres=ir.RxBuffer[7];
          zone_value=ir.RxBuffer[8];
          result = 10;
          if( zone_value==0xFF)
            result = 11;
          return result;
        }
        
        if ((ir.RxBuffer[5] == CODE_INDICATION_MOTION_SENSOR_PARAMETER) && \
          (ir.RxBuffer[6] == CMD_INDICATION_MOTION_SENSOR_PARAMETER))
        {
          if ((ir.RxBuffer[7] == 0xFF) &&(ir.RxBuffer[8] == (0xFF00) >> 8))
          {
            if ((ir.RxBuffer[9] == 0x03) && (ir.RxBuffer[10] ==(0x00)))
              result= 1;
           else if ((ir.RxBuffer[9] == 0x04) && (ir.RxBuffer[10] ==(0x00)))
              result= 2;
            else
              result=3;
          }
          else if ((ir.RxBuffer[7] == 0xFE) && (ir.RxBuffer[8] ==(0xFF00) >> 8))
          {
            if ((ir.RxBuffer[9] == 0x02) && (ir.RxBuffer[10] ==(0x00)))
              result= 4;
            else
              result= 3; 
          }
          else
          {
            result= 3;
          }
        }
        else
        {
          result= 0;
        }
        
      }
    }


    return result;
}

#pragma optimize=none
unsigned char IR_ReadGroupNum(unsigned char DATAx,unsigned char flag,unsigned char InOrEx)
{
    unsigned char result = 1;
    volatile unsigned char temp;
    unsigned short paramvalue;
    unsigned int buttonTimeStamp=0;
    TIME_Wait(1);

    IR_US_BASE->US_RCR = 0;
    IR_US_BASE->US_RNCR = 0;
    timeout = 0;

    temp = IR_US_BASE->US_RHR;

    ir.RxBufSize = NITOO_MAX_FRAME_LENGTH;
    memset(&ir.RxBuffer[0], 0, NITOO_MAX_FRAME_LENGTH);
    USART_ReadBuffer(IR_US_BASE, &ir.RxBuffer[0], ir.RxBufSize);

    IR_US_BASE->US_RTOR = IR_FRAMES_READ_WAIT;

    IR_US_BASE->US_IER = AT91C_US_TIMEOUT;

    while ((IR_US_BASE->US_CSR & AT91C_US_RXBUFF) == 0)
    {
              AT91C_BASE_WDTC->WDTC_WDCR = (0xA5 << 24) | AT91C_WDTC_WDRSTT;

        if (timeout == 1)//||  TIME_GetTime()>=buttonTimeStamp+2000)   //timeout happened
        {
//            result = 0;
            timeout = 0;
            break;
        }
    }

    IR_US_BASE->US_RTOR = 0;

    IR_US_BASE->US_IDR = AT91C_US_TIMEOUT;
    
    
//    //TEST array
//    unsigned char TestBuffer[NITOO_MAX_FRAME_LENGTH]=\
//    //{0XAA,0X80,0XFF,0XFF,0XF0,0XF1,0X05,0XFF,0XFF,0X04,0X00,0X00,0X00,0X12};  //error code
//    {0XAA,0X80,0XFF,0XFF,0XF0,0Xf0,0X45,0XFF, 0X00, 0X00, 0x7f, 0x22};       // ok for group Num
//    //0   1    2    3    4     5    6   7       8    9    10    11   
//                                      //data2 data3 data4 data5 crc
//    //RESPONSE_DEVICE_GROUP_NUMBER	0xF0	0x45	MaskL	Mask 	Mask	MaskH   CRC
//    ///for debug
//    for (temp=0; temp<NITOO_MAX_FRAME_LENGTH; temp++)
//    {
//      ir.RxBuffer[temp] = TestBuffer[temp];
//        
//    }
//    

    
    for (temp=0; temp<NITOO_MAX_FRAME_LENGTH; temp++)
    {
        if (ir.RxBuffer[temp] == NITOO_FRAME_PREAMBLE)
        {
            break;
        }
        else
        {
            if (temp >= NITOO_MAX_FRAME_LENGTH - NITOO_INDICATION_PARAMETER_FRAME_LENGTH)//   // 20-14=6 20-12=8 enough error
            {
                result = 4;
                break;
            }
        }
    }
    if(result==1)
    {//for crc check
      if ( _verify_frame_crc(ir.RxBuffer, NITOO_INDICATION_PARAMETER_FRAME_LENGTH) == 0)
      {
        result= 5;
      }
      else
      {
        if ((ir.RxBuffer[5] == 0xf0) && (ir.RxBuffer[6] == 0x45))// it is ok
        {
          if(InOrEx==0)
          {
            switch (flag)
            {
            case 2:
              if((DATAx & ir.RxBuffer[7])>0 )
              {//ok
                result = 10;
              }
              else
                result = 1;
              break;
            case 3:
              if((DATAx & ir.RxBuffer[8])>0 )
              {//ok
                result = 10;
              }
              else
                result = 1;
              break;
            case 4:
              if((DATAx & ir.RxBuffer[9])>0 )
              {//ok
                result = 10;
              }
              else
                result = 1;
              break;
            case 5:
              if((DATAx & ir.RxBuffer[10])>0 )
              {//ok
                result = 10;
              }
              else
                result = 1;
            default:
              result = 2;
            }
          }
          ////////////
          if(InOrEx==1)
          {
            switch (flag)
            {
            case 2:
              if((DATAx & ir.RxBuffer[7])==0 )
              {//ok
                result = 10;
              }
              else
                result = 1;
              break;
            case 3:
              if((DATAx & ir.RxBuffer[8])==0 )
              {//ok
                result = 10;
              }
              else
                result = 1;
              break;
            case 4:
              if((DATAx & ir.RxBuffer[9])==0 )
              {//ok
                result = 10;
              }
              else
                result = 1;
              break;
            case 5:
              if((DATAx & ir.RxBuffer[10])==0 )
              {//ok
                result = 10;
              }
              else
                result = 1;
            default:
              result = 2;
            }
          }
      }
      
      
      if ((ir.RxBuffer[5] == CODE_INDICATION_MOTION_SENSOR_PARAMETER) && \
        (ir.RxBuffer[6] == CMD_INDICATION_MOTION_SENSOR_PARAMETER))
      {
        if ((ir.RxBuffer[7] == 0xFF) &&(ir.RxBuffer[8] == (0xFF00) >> 8))
        {
          if ((ir.RxBuffer[9] == 0x05) && (ir.RxBuffer[10] ==(0x00)))
            result= 8;
          
        }
        if ((ir.RxBuffer[7] == 0xFE) && (ir.RxBuffer[8] ==(0xFF00) >> 8))
        {
          if ((ir.RxBuffer[9] == 0x03) && (ir.RxBuffer[10] ==(0x00)))
            result= 14;

        }
        
      }
     }
    }

    return result;
}




unsigned char IR_ReadAllGroupNum(void)
{
    unsigned char result = 1;
    volatile unsigned char temp;
    unsigned short paramvalue;

    TIME_Wait(1);

    IR_US_BASE->US_RCR = 0;
    IR_US_BASE->US_RNCR = 0;
    timeout = 0;

    temp = IR_US_BASE->US_RHR;

    ir.RxBufSize = NITOO_MAX_FRAME_LENGTH;
    memset(&ir.RxBuffer[0], 0, NITOO_MAX_FRAME_LENGTH);
    USART_ReadBuffer(IR_US_BASE, &ir.RxBuffer[0], ir.RxBufSize);

    IR_US_BASE->US_RTOR = IR_FRAMES_READ_WAIT;

    IR_US_BASE->US_IER = AT91C_US_TIMEOUT;
    unsigned int buttonTimeStamp=0;
    buttonTimeStamp = TIME_GetTime();
    while ((IR_US_BASE->US_CSR & AT91C_US_RXBUFF) == 0)
    {
              AT91C_BASE_WDTC->WDTC_WDCR = (0xA5 << 24) | AT91C_WDTC_WDRSTT;

        if (timeout == 1 ||  TIME_GetTime()>=buttonTimeStamp+1000)   //timeout happened
        {
//            result = 0;
            timeout = 0;
            break;
        }
    }

    IR_US_BASE->US_RTOR = 0;

    IR_US_BASE->US_IDR = AT91C_US_TIMEOUT;
    
    
//    //TEST array
//    unsigned char TestBuffer[NITOO_MAX_FRAME_LENGTH]=\
//    //{0XAA,0X80,0XFF,0XFF,0XF0,0XF1,0X05,0XFF,0XFF,0X04,0X00,0X00,0X00,0X12};  //error code
//    {0XAA,0X80,0XFF,0XFF,0XF0,0Xf0,0X45,0X21, 0X01, 0X01, 0x80, 0x22};       // ok for group Num
//    //0   1    2    3    4     5    6    7       8    9    10    11   
//                                      //data2 data3 data4 data5 crc
//    //RESPONSE_DEVICE_GROUP_NUMBER	0xF0	0x45	MaskL	Mask 	Mask	MaskH   CRC
//    ///for debug
//    ir.RxBuffer[7]=TestBuffer[7];
//    ir.RxBuffer[8]=TestBuffer[8];
//    ir.RxBuffer[9]=TestBuffer[9];
//    ir.RxBuffer[10]=TestBuffer[10];
//    for (temp=0; temp<NITOO_MAX_FRAME_LENGTH; temp++)
//    {
//      ir.RxBuffer[temp] = TestBuffer[temp];
//        
//    }
    

    
    for (temp=0; temp<NITOO_MAX_FRAME_LENGTH; temp++)
    {
        if (ir.RxBuffer[temp] == NITOO_FRAME_PREAMBLE)
        {
            break;
        }
        else
        {
            if (temp >= NITOO_MAX_FRAME_LENGTH - NITOO_NODE_INDEX_FRAME_LENGTH)//   // 20-14=6 20-12=8 enough error
            {
                result = 0;
                break;
            }
        }
    }
    if(result==1)
    {//for crc check
      if (_verify_frame_crc(ir.RxBuffer, NITOO_INDICATION_PARAMETER_FRAME_LENGTH) == 0)
      {
        result= 5;
      }
      else
      {
        if ((ir.RxBuffer[5] == 0xf0) && (ir.RxBuffer[6] == 0x45))// it is ok
        {
          int i;
          
          for(i=0;i<=7;i++)
          {            
            if((ir.RxBuffer[7]>>i) & 0x01>0)
            {
              GroupNum.GroupTmp[GroupNum.Index]=i;
              GroupNum.Index++;
            }
          }
          for(i=8;i<=15;i++)
          {            
            if((ir.RxBuffer[8]>>(i-8)) & 0x01>0)
            {
              GroupNum.GroupTmp[GroupNum.Index]=i;
              GroupNum.Index++;
            }
          }
          for(i=16;i<=23;i++)
          {            
            if((ir.RxBuffer[9]>>(i-16)) & 0x01>0)
            {
              GroupNum.GroupTmp[GroupNum.Index]=i;
              GroupNum.Index++;
            }
          }
          for(i=24;i<=31;i++)
          {            
            if((ir.RxBuffer[10]>>(i-24)) & 0x01>0)
            {
              GroupNum.GroupTmp[GroupNum.Index]=i;
              GroupNum.Index++;
            }
          }
            
          
          result = 10;
        }
      }
    }
    
    return result;
}



void IR_PnLGroup(unsigned char Channel, unsigned char Code, unsigned char Command)          // 2.3.0
{
    ir.TxBufSize = NITOO_CreateExtendedCommandFrame(ir.TxBuffer, Channel, Code, Command);       // 2.3.0

    _ir_send_frame();
}

void IR_DirectlySetSensorParameter(const PARAMETER *pParameter)
{
    ir.TxBufSize = NITOO_CreateDirectlySetParameterFrame(ir.TxBuffer, pParameter);

    _ir_send_frame();
}


void IR_ReadNBofControlGearParameter(unsigned char Channel,unsigned char Code,  unsigned char Command,unsigned char MaskL,unsigned char MaskH)
{
    ir.TxBufSize = NITOO_CreateReadNBofControlGearFrame(ir.TxBuffer, \
                                                  Channel, \
                                                  Code, \
                                                  Command,\
                                                  MaskL,\
                                                  MaskH);

    _ir_send_frame();
}
void IR_SetGroupNum(unsigned char Channel,unsigned char Code,  unsigned char Command,unsigned char MaskL,unsigned char MaskLH,unsigned char MaskHL,unsigned char MaskH,unsigned char data6)
{
  ir.TxBufSize = NITOO_CreateSetDeviceGroupNum(ir.TxBuffer,Channel,Code,Command,MaskL,MaskLH,MaskHL,MaskH,data6);

  _ir_send_frame();
}
void IR_DirectlySetAreaOnOFF(unsigned char Channel,unsigned char Code)
{
    ir.TxBufSize = NITOO_CreateDirectlySetAreaOnOFF(ir.TxBuffer,Channel,Code);

    _ir_send_frame();
}

#else

void IR_BAES(unsigned int Code, unsigned char Command)
{
    ir.TxBufSize = 3;
    _create_frame_BAES(ir.TxBuffer, Code, Command);

    _ir_send_frame();
}

#endif  // #ifndef APP_BAES
