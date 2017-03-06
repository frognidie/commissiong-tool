
#include "parameters.h"
#include "DefParaVal.h"
#include "DefParaVal_LV.h"
#include "DefParaVal_DGT.h"
#include "DefParaVal_CEN.h"
#include "DefParaVal_ZIGBEE.h"
#include "DefParaVal_LT_CTRL.h"
#include "DefParaVal_KNX.h"
#include  "DefParaVal_DALI.h"
#include "DefparaVal_PRECIRHF.h"

#include <stdbool.h>



unsigned char IsNewTypeSensor ( unsigned short type );
void  CpyParamsValue( SENSOR_PARAMS *pParams );
unsigned char CheckParameterRange( unsigned char type, unsigned int param_index, unsigned int value);


//------------------------------------------------------------------------------
///        Exported Variables
//------------------------------------------------------------------------------

__no_init SENSOR_PARAMS sensorParams;

const SENSOR_PARAMS *pDefaultFileParams[6] = {&defaultParamsCorridorHall, \
                                              &defaultParamsSmallOfficeCell, \
                                              &defaultParamsMeetingRoom, \
                                              &defaultParamsClassRoom, \
                                              &defaultParamsOpenSpace, \
                                              &defaultParamsToilet};

const PARAMETER parameterWalkTest = {0x0105, 0x0002};
const PARAMETER parameterEndTimeDelay = {0x0105, 0x0001};
const PARAMETER parameterInitialState = {0x0105, 0x0000};
const PARAMETER parameterChangeMaster ={0x0122,0x0000};
const PARAMETER parameterChangeSlave ={0x0122,0x0001};
const PARAMETER parameterChangeNop ={0x0122,0x0002};

const unsigned short valuesStandbyLevel[11] = {1, 2, 3, 5, 10, 15, 20, 30, 50, 75, 100};
const unsigned short valuesStandbyDelay[13] = {0, 5, 10, 20, 30, 61, 65, 70, 75, 80, 90, 120, 255};

// 2.4.0

// 2.4.0
const short valuesLongitude[24] = { 3,              // 0
                                    16,             // 1
                                    151,            // 2
                                    4,              // 3
                                    -43,            // 4
                                    116,            // 5
                                    13,             // 6
                                    13,             // 7
                                    2,              // 8
                                    60,             // 9
                                    0,              // 10
                                    24,             // 11
                                    -4,             // 12
                                    19,             // 13
                                    12,             // 14
                                    -6,             // 15
                                    11,             // 16
                                    21,             // 17
                                    -9,             // 18
                                    38,             // 19
                                    29,             // 20
                                    -74,            // 21
                                    18,             // 22
                                    7               // 23
                                    };

// 2.4.0
const short valuesLatitude[24] = {  38,             // 0
                                    48,             // 1
                                    -34,            // 2
                                    51,             // 3
                                    -23,            // 4
                                    40,             // 5
                                    56,             // 6
                                    53,             // 7
                                    49,             // 8
                                    25,             // 9
                                    51,             // 10
                                    38,             // 11
                                    40,             // 12
                                    47,             // 13
                                    42,             // 14
                                    53,             // 15
                                    60,             // 16
                                    52,             // 17
                                    38,             // 18
                                    56,             // 19
                                    41,             // 20
                                    41,             // 21
                                    59,             // 22
                                    47              // 23
                                    };
//------------------------------------------------------------------------------
///        Exported Functions
//------------------------------------------------------------------------------

void PARAMS_ResetToDefault(SENSOR_PARAMS *pParams, unsigned short type)
{

  if( PARAM_TYPE_LV_PIR == type )
    *pParams = DefParams_LV_PIR;
  else if( PARAM_TYPE_LV_US == type )
    *pParams = DefParams_LV_US; 
  else if( PARAM_TYPE_LV_DUAL == type )
    *pParams = DefParams_LV_PIR_US;  
  else if( PARAM_TYPE_LV_HF == type )
    *pParams = DefParams_LV_HF;  
  else if( PARAM_TYPE_LV_HF_PIR == type )
    *pParams = DefParams_LV_HF_PIR; 
  //////////////////////////////////////////////////
  else if( PARAM_TYPE_DGT_PIR == type )
    *pParams = DefParams_DGT_PIR;  
  else if( PARAM_TYPE_DGT_US == type )
    *pParams = DefParams_DGT_US;  
  else if( PARAM_TYPE_DGT_DUAL == type )
    *pParams = DefParams_DGT_PIR_US;  
  else if( PARAM_TYPE_DGT_HF_PIR == type )
    *pParams = DefParams_DGT_PIR_HF;  
  /////////////////////////////////////////////////
  else if( PARAM_TYPE_CEN_PIR == type )
    *pParams = DefParams_CEN_PIR;  
  else if( PARAM_TYPE_CEN_US == type )
    *pParams = DefParams_CEN_US;  
  else if( PARAM_TYPE_CEN_DUAL == type )
    *pParams = DefParams_CEN_PIR_US;  
  else if( PARAM_TYPE_CEN_HF_PIR == type )
    *pParams = DefParams_CEN_PIR_HF;  
  else if( PARAM_TYPE_CEN_DL == type )
    *pParams = DefParams_CEN_DL; 
  /////////////////////////////////////////////////
  else if( PARAM_TYPE_ZB_LV_PIR == type)
    *pParams = DefParams_LV_ZG_PIR;  
  else if( PARAM_TYPE_ZB_PIR == type )
    *pParams = DefParams_ZG_PIR;  
  else if( PARAM_TYPE_ZB_LV_DUAL == type)
    *pParams = DefParams_LV_ZG_PIR_US;  
  else if( PARAM_TYPE_ZB_DUAL == type )
    *pParams = DefParams_ZG_PIR_US;
  else if( PARAM_TYPE_ZB_LV_HF_PIR == type)
    *pParams = DefParams_LV_ZG_PIR_HF;  
  else if( PARAM_TYPE_ZB_HF_PIR == type )
    *pParams = DefParams_ZG_PIR_HF;
  /////////////////////////////////////////////////  
  else if( PARAM_TYPE_LIGHT_CTRL == type )
    *pParams = DefParams_LT_CTR;  
  else if( PARAM_TYPE_INITIAL == type ){
    *pParams = DefParams_DGT_ALL;  
  }
  else if( PARAM_TYPE_KNX_DAYLIGHT_ONLY == type ){
    *pParams = DefParams_KNX_DAYLIGHT_ONLY;  
  }
  else if( PARAM_TYPE_KNX_PIR_ONLY_OCCUPANCY_ONLY == type ){
    *pParams = DefParams_KNX_PIR_ONLY_OCCUPANCY_ONLY;  
  } 
  else if( PARAM_TYPE_KNX_PIR_ONLY_OCCUPANCY_DAYLIGHT == type ){
    *pParams = DefParams_KNX_PIR_ONLY_OCCUPANCY_DAYLIGHT;  
  } 
  else if( PARAM_TYPE_SLAVE_KNX_PIR_ONLY_OCCUPANCY_ONLY == type ){
    *pParams = DefParams_SLAVE_KNX_PIR_ONLY_OCCUPANCY_ONLY;  
  }
  else if( PARAM_TYPE_KNX_PIR_US_OCCUPANCY_ONLY == type ){
    *pParams = DefParams_KNX_PIR_US_OCCUPANCY_ONLY;  
  }
  else if( PARAM_TYPE_KNX_PIR_US_OCCUPANCY_DAYLIGHT == type ){
    *pParams = DefParams_KNX_PIR_US_OCCUPANCY_DAYLIGHT;  
  }
  else if( PARAM_TYPE_SLAVE_KNX_PIR_US_OCCUPANCY_ONLY == type ){
    *pParams = DefParams_SLAVE_KNX_PIR_US_OCCUPANCY_ONLY;  
  }
  /////////////////////////////////////////////////////////////
  else if(PARAM_TYPE_DALI_NOP==type)
  {
    *pParams = DefParams_DALI_NOP;
  }
  else if(PARAM_TYPE_DALI_MASTER_DL_PIR_US==type)
  {
    *pParams = DefParams_DALI_DL_PIR_US_MASTER;
  }
  else if( PARAM_TYPE_DALI_MASTER_DL_OP_PIR_US ==type)
  {
    *pParams = DefParams_DALI_PIR_US_DL_OP_MASTER;
  }
  else if(PARAM_TYPE_DALI_MASTER_OP_PIR_US==type)
  {
    *pParams = DefParams_DALI_PIR_US_OP_MASTER;
  }
  else if(PARAM_TYPE_DALI_SLAVE_OP_PIR_US ==type)
  {
    *pParams = DefParams_DALI_PIR_US_OP_SLAVE ;
  }
  else if(PARAM_TYPE_DALI_MASTER_DL_OP_PIR  ==type)
  {
    *pParams = DefParams_DALI_PIR_DL_OP_MASTER ;
  } 
    else if(PARAM_TYPE_DALI_MASTER_OP_PIR ==type)
  {
    *pParams = DefParams_DALI_PIR_OP_MASTER ;
  }
  else if(PARAM_TYPE_DALI_SLAVE_OP_PIR  ==type)
  {
    *pParams = DefParams_DALI_PIR_OP_SLAVE ;
  } 
    else if(PARAM_TYPR_DALI_MASTER_DL_PIR  ==type)
  {
    *pParams = DefParams_DALI_PIR_MASTER ;
  } 
   else if( PARAM_TYPE_HF_PRECIR == type ){
    *pParams = DefParams_LV_HF_PRECIR;  
  }
  else if( PARAM_TYPE_HF_PRECIR_CLOCK == type ){
    *pParams = DefParams_LV_HF_PRECIR_clock;  
  }
  else if( PARAM_TYPE_HF_PRECIR_BILEVEL == type ){
    *pParams = DefParams_LV_HF_PRECIR_bilevel;  
  }
  else if( PARAM_TYPE_HF_PRECIR_ESSENTIAL == type ){
    *pParams = DefParams_LV_HF_PRECIR_essential;  
  }

  
  
  
  
  else{
      *pParams = defaultParams;
  }
}

unsigned char PARAMS_IsParameterAvailable(SENSOR_PARAMS const *pParams, unsigned char Index)
{
    if(    ( Index <=get_parameters_amount( pParams ) ) 
        && ( PARAMS_GetParameterNumber(pParams, Index) != PARAM_NUMBER_ERROR ) && ( PARAMS_GetParameterNumber(pParams, Index) != 0 )
      )
    {
         return( true );
    }
    else
    {
        return( false );
    }
}

unsigned char PARAMS_ValidateParameterValue(unsigned char Index, unsigned short Value)
{
    if ((Index == PARAM_INDEX_HOUR) && (Value > 255))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_MINUTE) && (Value > 59))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_SECOND) && (Value > 59))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_HF_SENS) && (Value > 3))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_PIR_SENS) && (Value > 3))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_US_SENS) && (Value > 3))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_DAYLIGHTSETPOINT) && (Value > 255))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_DETECTIONSCHEME) && ((Value < 1) || (Value > 7)))
    {
        return 0;
    }

    if( is_KNX_sensor( &sensorParams ) )
    {
        if( (Index == PARAM_INDEX_INITIAL) && (Value > 5) )
        {
            return 0;
        }    
    }
    else
    {
        if ((Index == PARAM_INDEX_INITIAL) && ((Value < 1) || (Value > 5)))
        {
            return 0;
        }
    }

    if( is_KNX_sensor( &sensorParams ) )
    {
        if( (Index == PARAM_INDEX_MAINTAIN) && (Value > 5) )
        {
            return 0;
        }    
    }
    else
    {
        if ((Index == PARAM_INDEX_MAINTAIN) && ((Value < 1) || (Value > 5)))
        {
            return 0;
        }
    }

    if( is_KNX_sensor( &sensorParams ) )
    {
        if ((Index == PARAM_INDEX_RETRIGGER) && (Value > 5))
        {
            return 0;
        }    
    }
    else
    {
        if ((Index == PARAM_INDEX_RETRIGGER) && (Value > 5))
        {
            return 0;
        }
    }

    if ((Index == PARAM_INDEX_MODE) && ((Value < 1) || (Value > 9)))    // 2.3.0
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_ALERT) && (Value > 3))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_STANDBYLEVEL) && (Value > 255))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_STANDBYDELAY) && (Value > 255))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_SENDCONDITION) && (Value > 3))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_DEADBAND) && ((Value < 1) || (Value > 100)))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_TIMEBASE) && ((Value < 1) || (Value > 59)))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_BANDWIDTH) && ((Value < 1) || (Value > 255)))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_LIGHTREGULATION) && (Value > 1))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_LOOPTYPE) && (Value > 1))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_PROVISIONOFLIGHT) && (Value > 255))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_YEAR) && (Value > 255))       // 2.3.1
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_MONTH) && ((Value == 0) || (Value > 12)))       // 2.3.1
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_DAY) && ((Value == 0) || (Value > 31)))         // 2.3.1
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_SUMMERHOUR) && (Value != 0) && (Value != 1) && (Value != 2) && (Value != 255))        // 2.3.1
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_STARTMONTH) && ((Value == 0) || (Value > 12)))       // 2.3.1
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_STARTDAY) && ((Value == 0) || (Value > 31)))       // 2.3.1
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_ENDMONTH) && ((Value == 0) || (Value > 12)))       // 2.3.1
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_ENDDAY) && ((Value == 0) || (Value > 31)))       // 2.3.1
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_COUNTRY) && (Value > 23))       // 2.3.1
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_LONGITUDE) && ((Value > 0xB4) && (Value < 0xFF4C)))       // 2.3.1
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_LATITUDE) && ((Value > 0x5A) && (Value < 0xFFA6)))       // 2.3.1
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_TYPE) && \
        (Value != PARAM_TYPE_LV_HF) && \
        (Value != PARAM_TYPE_LV_PIR) && \
        (Value != PARAM_TYPE_LV_US) && \
        (Value != PARAM_TYPE_LV_DUAL) && \
//        (Value != PARAM_TYPE_DGT_NONE) &&         // 1.0.24, comment out
        (Value != PARAM_TYPE_DGT_PIR) && \
        (Value != PARAM_TYPE_DGT_US) && \
        (Value != PARAM_TYPE_DGT_DUAL) && \
        (Value != PARAM_TYPE_CEN_DL) && \
        (Value != PARAM_TYPE_CEN_PIR) && \
        (Value != PARAM_TYPE_CEN_US) && \
        (Value != PARAM_TYPE_CEN_DUAL) && \
        (Value != PARAM_TYPE_ZB_PIR) && \
        (Value != PARAM_TYPE_ZB_LV_PIR) && \
        (Value != PARAM_TYPE_ZB_DUAL) && \
        (Value != PARAM_TYPE_ZB_LV_DUAL) && \
        (Value != PARAM_TYPE_LIGHT_CTRL) && \
        (Value != PARAM_TYPE_LV_HF_PIR) && \
        (Value != PARAM_TYPE_ZB_HF_PIR) && \
        (Value != PARAM_TYPE_DGT_HF_PIR) && \
        (Value != PARAM_TYPE_CEN_HF_PIR) && 
        ( PARAM_TYPE_FAMILY_AUTOSENSOR2 != (Value & PARAM_TYPE_FAMILY_MASK) ) 
        )         
    {
        return 0;
    }

    // Type2 value validation       // 2.3.1

    // Firmware Version value validation

    // ID1, ID2, ID3 and ID4 values validation      // 2.3.1

    if ((Index == PARAM_INDEX_REFERENT) && (Value > 0xAF))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_MAINGROUP) && (Value > 255))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_BATTERYINFORMATION) && (Value > 255))
    {
        return 0;
    }

    // Daughter board firmware Version values validation        // 2.3.1 (include Daughter firmware version 2)

    if ((Index == PARAM_INDEX_DEVICEIDZIGBEE) && (Value != 0x0103) && (Value != 0x010C) && (Value != 0x010D) && (Value != 0x010E))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_DEVICEIDZIGBEE2) && (Value != 0x0103) && (Value != 0x010C) && (Value != 0x010D) && (Value != 0x010E))     // 2.3.1
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_DAYLIGHTFACTOR) && ((Value < 1) || (Value > 255)))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_DAYLIGHTFACTORCEN) && ((Value < 1) || (Value > 255)))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_NATURALLIGHTFACTOR) && ((Value < 1) || (Value > 255)))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_DAYLIGHTLEVEL) && (Value > 19999))
    {
        return 0;
    }

    // Sensor Status value validation

    if ((Index == PARAM_INDEX_CURRENTPOL) && (Value > 255))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_LOADTYPE) && (Value > 6))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_POWERLINEFILTER) && (Value > 1))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_BUZZERFEEDBACK) && (Value > 1))
    {
        return 0;
    }

    if ((Index == PARAM_INDEX_FACTORYLIGHTFACTOR) && ((Value == 0) || (Value > 255)))       // 2.3.1
    {
        return 0;
    }

    return 1;
}

unsigned char PARAMS_ValidateDefaultFileValues(const SENSOR_PARAMS *pParams, unsigned char Index, unsigned short Value)
{
    if (PARAMS_SensorHasAllTech(pParams))
    {
        if ((Index == PARAM_INDEX_DETECTIONSCHEME) && ((Value < 1) || (Value > 5)))
        {
            return 0;
        }
        else if ((Index == PARAM_INDEX_INITIAL) && ((Value < 1) || (Value > 5)))
        {
            return 0;
        }
        else if ((Index == PARAM_INDEX_MAINTAIN) && ((Value < 1) || (Value > 5)))
        {
            return 0;
        }
        else if ((Index == PARAM_INDEX_RETRIGGER) && (Value > 5))
        {
            return 0;
        }
    }
    else if (PARAMS_SensorHasPIRandUS(pParams))
    {
        if ((Index == PARAM_INDEX_DETECTIONSCHEME) && ((Value < 1) || (Value > 4)))
        {
            return 0;
        }
        else if ((Index == PARAM_INDEX_INITIAL) && ((Value < 1) || (Value > 4)))
        {
            return 0;
        }
        else if ((Index == PARAM_INDEX_MAINTAIN) && ((Value < 1) || (Value > 4)))
        {
            return 0;
        }
        else if ((Index == PARAM_INDEX_RETRIGGER) && (Value > 4))
        {
            return 0;
        }
    }
    else if (PARAMS_SensorHasPIR(pParams))
    {
        if ((Index == PARAM_INDEX_DETECTIONSCHEME) && (Value != 1))
        {
            return 0;
        }
        else if ((Index == PARAM_INDEX_INITIAL) && (Value != 1))
        {
            return 0;
        }
        else if ((Index == PARAM_INDEX_MAINTAIN) && (Value != 1))
        {
            return 0;
        }
        else if ((Index == PARAM_INDEX_RETRIGGER) && (Value != 0) && (Value != 1))
        {
            return 0;
        }
    }
    else if (PARAMS_SensorHasUS(pParams))
    {
        if ((Index == PARAM_INDEX_DETECTIONSCHEME) && (Value != 2))
        {
            return 0;
        }
        else if ((Index == PARAM_INDEX_INITIAL) && (Value != 2))
        {
            return 0;
        }
        else if ((Index == PARAM_INDEX_MAINTAIN) && (Value != 2))
        {
            return 0;
        }
        else if ((Index == PARAM_INDEX_RETRIGGER) && (Value != 0) && (Value != 2))
        {
            return 0;
        }
    }
    else if (PARAMS_SensorHasHF(pParams))
    {
        if ((Index == PARAM_INDEX_DETECTIONSCHEME) && (Value != 5))
        {
            return 0;
        }
        else if ((Index == PARAM_INDEX_INITIAL) && (Value != 5))
        {
            return 0;
        }
        else if ((Index == PARAM_INDEX_MAINTAIN) && (Value != 5))
        {
            return 0;
        }
        else if ((Index == PARAM_INDEX_RETRIGGER) && (Value != 0) && (Value != 5))
        {
            return 0;
        }
    }

    return 1;
}


void PARAMS_InterpretStandbyLevel(SENSOR_PARAMS *pParams,int numIndex)
{
    unsigned short temp = PARAMS_GetParameterValue(pParams, numIndex);

    if (temp > valuesStandbyLevel[sizeof(valuesStandbyLevel)/sizeof(unsigned short)-1])   // if temp > maximum value, interpret as maximum value.
    {
        temp = valuesStandbyLevel[sizeof(valuesStandbyLevel)/sizeof(unsigned short)-1];
    }

    PARAMS_SetParameterValue(pParams, numIndex, temp);
}
void PARAMS_InterpretStandbyDelay(SENSOR_PARAMS *pParams)
{
    unsigned short temp = PARAMS_GetParameterValue(pParams, PARAM_INDEX_STANDBYDELAY);

    if (temp > valuesStandbyDelay[sizeof(valuesStandbyDelay)/sizeof(unsigned short)-2])     // Interpret 121 to 254 as 255
    {
        temp = valuesStandbyDelay[sizeof(valuesStandbyDelay)/sizeof(unsigned short)-1];
    }

    PARAMS_SetParameterValue(pParams, PARAM_INDEX_STANDBYDELAY, temp);
}

unsigned char is_KNX_sensor( SENSOR_PARAMS const *_pParams )
{
    unsigned char result = false;
  
  if(_pParams->Type.Value<=0x106 && _pParams->Type.Value>=0x100)
  {
     result = true;
  
  }
    
    return( result );
}

unsigned int get_parameters_amount( SENSOR_PARAMS const *_pParams )
{
    unsigned int amount = 0;
    
    if( is_KNX_sensor( _pParams ) )
    {
        amount = 61+4;
    }
    else if(is_PRECIR_sensor( _pParams ))
    {
      amount = 61+4+12+9;
    } 
    else
    {
        amount = 60+4+1;
    }
    
    return( amount );
}

unsigned char is_DALI_sensor(SENSOR_PARAMS const *_pParams)
{
  unsigned char result = false;
  if(_pParams->Type.Value<=0x110 && _pParams->Type.Value>=0x109)
  {
     result = true;
  
  }

return result;

}
unsigned char is_PRECIR_sensor(SENSOR_PARAMS const *_pParams)
{
  unsigned char result = false;
  if(_pParams->Type.Value<=0x113 && _pParams->Type.Value>=0x111)
  {
     result = true;
  
  }
  if(_pParams->Type.Value==0x107)
  {
       result = true;
  }

return result;

}
