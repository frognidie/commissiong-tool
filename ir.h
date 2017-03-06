#ifndef IR_H
#define IR_H

#include <pwmc\pwmc.h>
#include <usart\usart.h>
#include "nitoo.h"
#include "languages.h"
extern unsigned char xx_createzone;
extern unsigned char yy_createzone;
extern unsigned char zone_value;
extern unsigned char detec_short_addres;
typedef struct {
  unsigned windows_value;
    unsigned corridor_value;
    unsigned board_value;
    unsigned XX;
    unsigned win_ballast;
    unsigned cori_ballast;
    unsigned board_ballast;
} VIEW;

typedef struct {
  unsigned char GroupTmp[32];
  unsigned char Index;
  
}GROUPNUM;
extern unsigned char xx_detector;

extern unsigned char RxDisBuffer[NITOO_INDICATION_PARAMETER_FRAME_LENGTH];
//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------
//#define TESTERROR   //for debug error code
#define IR_FRAMES_SEND_INTERVAL     (25<<20)    //for comparing with PIT_PIVR, refer to 25mS
#define IR_FRAMES_READ_WAIT         (48)       //in a period of 1/2400, refer to 20mS
//#define IR_FRAMES_READ_WAIT         (4800)       //in a period of 1/2400, refer to 2000mS

//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/// Exported Variables
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
/// Exported Functions
//------------------------------------------------------------------------------


void IR_Configure(void);
#ifndef APP_BAES
void IR_SetSensorParameter(SENSOR_PARAMS *pParams, unsigned char ParamIndex);
unsigned char IR_ReadSensorParameter(SENSOR_PARAMS *pParams, unsigned char ParamIndex);
void IR_PnLGroup(unsigned char Channel, unsigned char Code, unsigned char Command);         // 2.3.0
void IR_DirectlySetSensorParameter(const PARAMETER *pParameter);
unsigned char IR_ReadCreateZoneInfo(OPTIMES tempLanguage);
unsigned char IR_Read_CONTROL_GEAR_Info(unsigned char num);
unsigned char IR_ReadDetectorInfo(void);
void IR_DirectlySetAreaOnOFF(unsigned char Channel,unsigned char Code);
unsigned char IR_Read_UNIT_DESCRIPTION_Info(unsigned char num);
unsigned char IR_ReadGroupNum(unsigned char DATAx,unsigned char flag,unsigned char InOrEx);
void IR_ReadNBofControlGearParameter(unsigned char Channel,unsigned char Code,  unsigned char Command,unsigned char MaskL,unsigned char MaskH);
void IR_SetGroupNum(unsigned char Channel,unsigned char Code,  unsigned char Command,unsigned char MaskL,unsigned char MaskLH,unsigned char MaskHL,unsigned char MaskH,unsigned char data6);
#else
void IR_BAES(unsigned int Code, unsigned char Command);
#endif  // #ifndef APP_BAES

#endif
