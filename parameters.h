#ifndef PARAMETERS_H
#define PARAMETERS_H

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------


#define PARAM_IS_OK                     0
#define PARAM_IS_OVERFLOW               1
#define PARAM_IS_UNDERFLOW              2



#define PARAM_INDEX_HOUR                0
#define PARAM_INDEX_MINUTE              1
#define PARAM_INDEX_SECOND              2
#define PARAM_INDEX_HF_SENS             3       // 1.0.24
#define PARAM_INDEX_PIR_SENS            4
#define PARAM_INDEX_US_SENS             5
#define PARAM_INDEX_DAYLIGHTSETPOINT    6
#define PARAM_INDEX_DETECTIONSCHEME     7
#define PARAM_INDEX_INITIAL             8
#define PARAM_INDEX_MAINTAIN            9
#define PARAM_INDEX_RETRIGGER           10
#define PARAM_INDEX_MODE                11
#define PARAM_INDEX_ALERT               12
#define PARAM_INDEX_STANDBYLEVEL        13      // 1.0.24
#define PARAM_INDEX_STANDBYDELAY        14      // 1.0.24
#define PARAM_INDEX_SENDCONDITION       15      // 1.0.24
#define PARAM_INDEX_DEADBAND            16      // 1.0.24
#define PARAM_INDEX_TIMEBASE            17      // 1.0.24
#define PARAM_INDEX_BANDWIDTH           18      // 1.0.24
#define PARAM_INDEX_LIGHTREGULATION     19
#define PARAM_INDEX_LOOPTYPE            20
#define PARAM_INDEX_PROVISIONOFLIGHT    21
#define PARAM_INDEX_YEAR                22      // 2.3.1
#define PARAM_INDEX_MONTH               23      // 2.3.1
#define PARAM_INDEX_DAY                 24      // 2.3.1
#define PARAM_INDEX_SUMMERHOUR          25      // 2.3.1
#define PARAM_INDEX_STARTMONTH          26      // 2.3.1
#define PARAM_INDEX_STARTDAY            27      // 2.3.1
#define PARAM_INDEX_ENDMONTH            28      // 2.3.1
#define PARAM_INDEX_ENDDAY              29      // 2.3.1
#define PARAM_INDEX_COUNTRY             30      // 2.3.1
#define PARAM_INDEX_LONGITUDE           31      // 2.3.1
#define PARAM_INDEX_LATITUDE            32      // 2.3.1
#define PARAM_INDEX_DAYLIGHTFACTOR      33      // read/write only in hidden page
#define PARAM_INDEX_DAYLIGHTFACTORCEN   34      // read/write only in hidden page
#define PARAM_INDEX_NATURALLIGHTFACTOR  35      // read/write only in hidden page
#define PARAM_INDEX_DEVICEIDZIGBEE      36      // read only, writable in hidden page   // 1.0.24
#define PARAM_INDEX_DEVICEIDZIGBEE2     37      // read only, writable in hidden page   // 2.3.1
#define PARAM_INDEX_TYPE                38      // read only
#define PARAM_INDEX_TYPE2               39      // read only    // 2.3.1
#define PARAM_INDEX_FIRMWAREVERSION     40      // read only
#define PARAM_INDEX_ID1                 41      // read only
#define PARAM_INDEX_ID2                 42      // read only
#define PARAM_INDEX_ID3                 43      // read only    // 2.3.1
#define PARAM_INDEX_ID4                 44      // read only    // 2.3.1
#define PARAM_INDEX_REFERENT            45      // read only
#define PARAM_INDEX_MAINGROUP           46      // read only    // 1.0.24
#define PARAM_INDEX_BATTERYINFORMATION  47      // read only    // 1.0.24
#define PARAM_INDEX_DBFIRMWAREVERSION   48      // read only    // 1.0.24
#define PARAM_INDEX_DBFIRMWAREVERSION2  49      // read only    // 2.3.1
#define PARAM_INDEX_DAYLIGHTLEVEL       50      // read only in hidden page, writable when used for calibration
#define PARAM_INDEX_SENSORSTATUS        51      // read only in hidden page
#define PARAM_INDEX_CURRENTPOL          52      // read only in hidden page         // 1.0.24
#define PARAM_INDEX_LOADTYPE            53      // read/write only in hidden page   // 2.2.0
#define PARAM_INDEX_POWERLINEFILTER     54      // read/write only in hidden page   // 2.2.0
#define PARAM_INDEX_BUZZERFEEDBACK      55      // read/write only in hidden page   // 2.2.0
#define PARAM_INDEX_FACTORYLIGHTFACTOR  56      // not implemented in menu          // 2.3.1
#define PARAM_INDEX_GMTSIGN             57      //2.4.1
#define PARAM_INDEX_GMTHOURS            58      //2.4.1
#define PARAM_INDEX_GMTMINUTES          59      //2.4.1
#define PARAM_INDEX_KNX_MASTER_SLAVE    60      //2.4.6
#define PARAM_INDEX_DetectionLed         61
#define PARAM_INDEX_ReactionSpeed        62
#define PARAM_INDEX_OFFset               63
#define PARAM_INDEX_SensorConfig         64///////
#define PARAM_INDEX_DIM_R               65      //2.5.7
#define PARAM_INDEX_DIM_F               66
#define PARAM_INDEX_AUX_MODE            67
#define PARAM_INDEX_AUX_PULE            68
#define PARAM_INDEX_ON_LEVEL            69
#define PARAM_INDEX_EXT_MODE_STATE      70
#define PARAM_INDEX_EXT_FUN_MODE        71
#define PARAM_INDEX_EXT_STANDBY_LEVEL   72
#define PARAM_INDEX_EXT_STANDBY_DELAY   73
#define PARAM_INDEX_EXT_ON_LEVEL        74
#define PARAM_INDEX_OP_TIME             75
#define PARAM_INDEX_WEEK_DAY            76
#define PARAM_INDEX_EXT_START_HOUR      77
#define PARAM_INDEX_EXT_START_MIN       78
#define PARAM_INDEX_EXT_STOP_HOUR       79
#define PARAM_INDEX_EXT_STOP_MIN        80
#define PARAM_INDEX_EXT_SCHEDULD_DAY    81
#define PARAM_INDEX_EXT_REAL_HOUR       82
#define PARAM_INDEX_EXT_REAL_MIN        83
#define PARAM_INDEX_EXT_REAL_SEC        84

#define PARAMS_TOTAL                    65+20      //2.4.6

#define PARAMS_TOTAL_FIRST_VERSION              22
#define PARAMS_TOTAL_SECOND_VERSION             23
#define PARAMS_TOTAL_THIRD_VERSION              25
#define PARAMS_TOTAL_FOURTH_VERSION             25
#define PARAMS_TOTAL_FIFTH_VERSION              37
#define PARAMS_TOTAL_SIXTH_VERSION              40  // 2.3.1
#define PARAMS_TOTAL_SEVENTH_VERSION            41  // 2.3.1


#define PARAM_INDEX_TYPE_FIRST_VERSION          17
#define PARAM_INDEX_TYPE_SECOND_VERSION         18
#define PARAM_INDEX_TYPE_THIRD_VERSION          20
#define PARAM_INDEX_TYPE_FOURTH_VERSION         20
#define PARAM_INDEX_TYPE_FIFTH_VERSION          26
#define PARAM_INDEX_TYPE_SIXTH_VERSION          26  // 2.3.1
#define PARAM_INDEX_TYPE_SEVENTH_VERSION          27  // 2.3.1

#define PARAMS_WRITABLE_TOTAL                   85  // 2.3.1, was 25
#define PARAMS_READABLE_TOTAL                   61



#define PARAM_NUMBER_REFERENT           5
#define PARAM_NUMBER_MAINGROUP          6
#define PARAM_NUMBER_HOUR               10
#define PARAM_NUMBER_MINUTE             11
#define PARAM_NUMBER_SECOND             12
#define PARAM_NUMBER_HF_SENS            19
#define PARAM_NUMBER_PIR_SENS           20
#define PARAM_NUMBER_US_SENS            21
#define PARAM_NUMBER_DAYLIGHTFACTOR     22
#define PARAM_NUMBER_DETECTIONSCHEME    22
#define PARAM_NUMBER_LOOPTYPE           23
#define PARAM_NUMBER_DAYLIGHTFACTORCEN  24
#define PARAM_NUMBER_NATURALLIGHTFACTOR 24
#define PARAM_NUMBER_DAYLIGHTSETPOINT   25
#define PARAM_NUMBER_PROVISIONOFLIGHT   26
#define PARAM_NUMBER_DAYLIGHTLEVEL      27
#define PARAM_NUMBER_LIGHTREGULATION    28
#define PARAM_NUMBER_MODE               29
#define PARAM_NUMBER_INITIAL            30
#define PARAM_NUMBER_MAINTAIN           31
#define PARAM_NUMBER_RETRIGGER          32
#define PARAM_NUMBER_ALERT              34
#define PARAM_NUMBER_STANDBYLEVEL       35
#define PARAM_NUMBER_STANDBYDELAY       36
#define PARAM_NUMBER_SENDCONDITION      40
#define PARAM_NUMBER_DEADBAND           41
#define PARAM_NUMBER_TIMEBASE           42
#define PARAM_NUMBER_BANDWIDTH          43
#define PARAM_NUMBER_TYPE               256
#define PARAM_NUMBER_ID1                257
#define PARAM_NUMBER_ID2                258
#define PARAM_NUMBER_FIRMWAREVERSION    259
#define PARAM_NUMBER_SENSORSTATUS       260
#define PARAM_NUMBER_TESTMODE           261
#define PARAM_NUMBER_CURRENTPOL         262
#define PARAM_NUMBER_BATTERYINFORMATION 263
#define PARAM_NUMBER_DBFIRMWAREVERSION  264
#define PARAM_NUMBER_DEVICEIDZIGBEE     265
#define PARAM_NUMBER_LOADTYPE           266
#define PARAM_NUMBER_POWERLINEFILTER    267
#define PARAM_NUMBER_BUZZERFEEDBACK     268
#define PARAM_NUMBER_FACTORYLIGHTFACTOR 269     // 2.3.1
#define PARAM_NUMBER_YEAR               270     // 2.3.1
#define PARAM_NUMBER_MONTH              271     // 2.3.1
#define PARAM_NUMBER_DAY                272     // 2.3.1
#define PARAM_NUMBER_SUMMERHOUR         273     // 2.3.1
#define PARAM_NUMBER_STARTMONTH         274     // 2.3.1
#define PARAM_NUMBER_STARTDAY           275     // 2.3.1
#define PARAM_NUMBER_ENDMONTH           276     // 2.3.1
#define PARAM_NUMBER_ENDDAY             277     // 2.3.1
#define PARAM_NUMBER_COUNTRY            278     // 2.3.1
#define PARAM_NUMBER_LONGITUDE          279     // 2.3.1
#define PARAM_NUMBER_LATITUDE           280     // 2.3.1
#define PARAM_NUMBER_ID3                281     // 2.3.1
#define PARAM_NUMBER_ID4                282     // 2.3.1
#define PARAM_NUMBER_DBFIRMWAREVERSION2 283     // 2.3.1
#define PARAM_NUMBER_DEVICEIDZIGBEE2    284     // 2.3.1

#define PARAM_NUMBER_GMTSIGN            287      //2.4.1
#define PARAM_NUMBER_GMTHOURS           288      //2.4.1
#define PARAM_NUMBER_GMTMINUTES         289      //2.4.1

#define PARAM_NUMBER_KNX_MASTER_SLAVE   290      //2.4.6
#define PARAM_NUM_DetectionLed          302      //2.5.7
#define PARAM_NUM_ReactionSpeed         303
#define PARAM_NUM_OFFset                304
#define PARAM_NUM_SensorConfig          305
//precir 
#define PARAM_NUMBER_DIM_R              291
#define PARAM_NUMBER_DIM_F              292
#define PARAM_NUMBER_AUX_MODE           293
#define PARAM_NUMBER_AUX_PULSE_TIME     315
#define PARAM_NUMBER_ON_LEVEL           296
#define PARAM_NUMBER_EXT_MODE_STATE     309
#define PARAM_NUMBER_EXT_FUNC_MODE      316
#define PARAM_NUMBER_EXT_STANDBY_LEVEL  317
#define PARAM_NUMBER_EXT_STANDBY_DELAY  318
#define PARAM_NUMBER_EXT_ON_LEVEL       319
#define PARAM_NUMBER_WEEK_DAY           320
#define PARAM_NUMBER_OP_TIME            301

#define PARAM_NUMBER_EXT_START_HOUR     311
#define PARAM_NUMBER_EXT_START_MIN      312
#define PARAM_NUMBER_EXT_STOP_HOUR      313
#define PARAM_NUMBER_EXT_STOP_MIN       314
#define PARAM_NUMBER_EXT_SCHEDULED_DAY  310
#define PARAM_NUMBER_REAL_TIME_HOUR     306
#define PARAM_NUMBER_REAL_TIME_MIN      307
#define PARAM_NUMBER_REAL_TIME_SEC      308

#define PARAM_NUMBER_TYPE2              512     // 2.3.1
#define PARAM_NUMBER_ERROR              0xFFFF
//#define PARAM_VALUE_ERROR_RESERVED      0
//#define PARAM_VALUE_ERROR_NOT_IMPLEMENTED   1
//0x0002~0xFFFF for future usage

// 2.3.1
#define PARAM_TYPE_FAMILY_MASK          0x0F00
#define PARAM_TYPE_FAMILY_AUTOSENSOR    0x0000
#define PARAM_TYPE_FAMILY_LIGHTCTRL     0x0200
#define PARAM_TYPE_FAMILY_METERSENSOR   0x0300
#define PARAM_TYPE_FAMILY_AUTOSENSOR2   0x0100  // 2.4.6
// 0x0100, 0x0400...0x0F00 are reserved.
// 2.3.1

#define PARAM_TYPE_CATEGORY_BIT         7
#define PARAM_TYPE_CATEGORY_MASK        (1 << PARAM_TYPE_CATEGORY_BIT)
#define PARAM_TYPE_CATEGORY_STANDALONE  (0 << PARAM_TYPE_CATEGORY_BIT)
#define PARAM_TYPE_CATEGORY_CENTRALIZED (1 << PARAM_TYPE_CATEGORY_BIT)

#define PARAM_TYPE_US_BIT               5
#define PARAM_TYPE_PIR_BIT              4
#define PARAM_TYPE_HF_BIT               3
#define PARAM_TYPE_TECH_MASK            ((1 << PARAM_TYPE_US_BIT) | (1 << PARAM_TYPE_PIR_BIT) | (1 << PARAM_TYPE_HF_BIT))
#define PARAM_TYPE_TECH_NONE            ((0 << PARAM_TYPE_US_BIT) | (0 << PARAM_TYPE_PIR_BIT) | (0 << PARAM_TYPE_HF_BIT))
#define PARAM_TYPE_TECH_HF_ONLY         ((0 << PARAM_TYPE_US_BIT) | (0 << PARAM_TYPE_PIR_BIT) | (1 << PARAM_TYPE_HF_BIT))
#define PARAM_TYPE_TECH_PIR_ONLY        ((0 << PARAM_TYPE_US_BIT) | (1 << PARAM_TYPE_PIR_BIT) | (0 << PARAM_TYPE_HF_BIT))
#define PARAM_TYPE_TECH_US_ONLY         ((1 << PARAM_TYPE_US_BIT) | (0 << PARAM_TYPE_PIR_BIT) | (0 << PARAM_TYPE_HF_BIT))
#define PARAM_TYPE_TECH_DUAL            ((1 << PARAM_TYPE_US_BIT) | (1 << PARAM_TYPE_PIR_BIT) | (0 << PARAM_TYPE_HF_BIT))
#define PARAM_TYPE_TECH_PIR_HF          ((0 << PARAM_TYPE_US_BIT) | (1 << PARAM_TYPE_PIR_BIT) | (1 << PARAM_TYPE_HF_BIT))

// 1.0.24
#define PARAM_TYPE_ZB_BIT               2
#define PARAM_TYPE_ZB_MASK              (1 << PARAM_TYPE_ZB_BIT)
#define PARAM_TYPE_ZB                   (1 << PARAM_TYPE_ZB_BIT)

#define PARAM_TYPE_DGT_BIT              1
#define PARAM_TYPE_LV_BIT               0
#define PARAM_TYPE_CLASS_MASK           ((1 << PARAM_TYPE_DGT_BIT) | (1 << PARAM_TYPE_LV_BIT))
#define PARAM_TYPE_CLASS_LV             (1 << PARAM_TYPE_LV_BIT)
#define PARAM_TYPE_CLASS_DGT            (1 << PARAM_TYPE_DGT_BIT)

#define PARAM_TYPE_INITIAL              0x003A      // Initialize to Digital sensor with all technologies   // 1.0.24
#define PARAM_TYPE_LV_PIR               0x0011
#define PARAM_TYPE_LV_US                0x0021
#define PARAM_TYPE_LV_DUAL              0x0031
#define PARAM_TYPE_LV_HF                0x0009      // 1.0.24
#define PARAM_TYPE_DGT_NONE             0x0002
#define PARAM_TYPE_DGT_PIR              0x0012
#define PARAM_TYPE_DGT_US               0x0022
#define PARAM_TYPE_DGT_DUAL             0x0032
#define PARAM_TYPE_CEN_DL               0x0082      // 1.0.24
#define PARAM_TYPE_CEN_PIR              0x0092
#define PARAM_TYPE_CEN_US               0x00A2
#define PARAM_TYPE_CEN_DUAL             0x00B2
#define PARAM_TYPE_ZB_PIR               0x0014      // 1.0.24
#define PARAM_TYPE_ZB_LV_PIR            0x0015      // 1.0.24
#define PARAM_TYPE_ZB_DUAL              0x0034      // 1.0.24
#define PARAM_TYPE_ZB_LV_DUAL           0x0035      // 2.1
#define PARAM_TYPE_LIGHT_CTRL           0x0201      // 2.3.1
#define PARAM_TYPE_LV_HF_PIR            0x19
#define PARAM_TYPE_ZB_HF_PIR            0x1c
#define PARAM_TYPE_ZB_LV_HF_PIR         0X1D
#define PARAM_TYPE_DGT_HF_PIR           0x1a
#define PARAM_TYPE_CEN_HF_PIR           0x9a

#define PARAM_TYPE_KNX_DAYLIGHT_ONLY                  0x0100
#define PARAM_TYPE_KNX_PIR_ONLY_OCCUPANCY_ONLY        0x0101
#define PARAM_TYPE_KNX_PIR_ONLY_OCCUPANCY_DAYLIGHT    0x0102
#define PARAM_TYPE_SLAVE_KNX_PIR_ONLY_OCCUPANCY_ONLY  0x0103
#define PARAM_TYPE_KNX_PIR_US_OCCUPANCY_ONLY          0x0104
#define PARAM_TYPE_KNX_PIR_US_OCCUPANCY_DAYLIGHT      0x0105
#define PARAM_TYPE_SLAVE_KNX_PIR_US_OCCUPANCY_ONLY    0x0106

//
//
//#define PARAM_TYPE_DALI_MASTER_PIR_US                  0x0108
//#define PARAM_TYPE_DALI_SLAVE_PIR_US                   0x0109
//#define PARAM_TYPE_DALI_NOP_PIR_US                     0x010A
//
//
//#define PARAM_TYPE_DALI_MASTER_PIR                  0x010B
//#define PARAM_TYPE_DALI_SLAVE_PIR                   0x010C
//#define PARAM_TYPE_DALI_NOP_PIR                     0x010D
#define PARAM_TYPE_DALI_NOP                   0x108
#define PARAM_TYPE_DALI_MASTER_DL_PIR_US      0X109
#define PARAM_TYPE_DALI_MASTER_DL_OP_PIR_US   0X10A
#define PARAM_TYPE_DALI_MASTER_OP_PIR_US      0x10B
#define PARAM_TYPE_DALI_SLAVE_OP_PIR_US       0x10C
#define PARAM_TYPE_DALI_MASTER_DL_OP_PIR      0x10D
#define PARAM_TYPE_DALI_MASTER_OP_PIR         0x10E
#define PARAM_TYPE_DALI_SLAVE_OP_PIR          0x10F
#define PARAM_TYPR_DALI_MASTER_DL_PIR         0x110


#define PARAM_TYPE_HF_PRECIR                          0x0107
#define PARAM_TYPE_HF_PRECIR_CLOCK                    0x0111
#define PARAM_TYPE_HF_PRECIR_BILEVEL                  0x0112
#define PARAM_TYPE_HF_PRECIR_ESSENTIAL                0x0113


// 2.3.1
#define PARAM_TYPE2_MCHANNEL_BIT        2
#define PARAM_TYPE2_PULSE_BIT           1
#define PARAM_TYPE2_EYEFUNC_BIT         0
#define PARAM_TYPE2_MCHANNEL_MASK       (1 << PARAM_TYPE2_MCHANNEL_BIT)
#define PARAM_TYPE2_PULSE_MASK          (1 << PARAM_TYPE2_PULSE_BIT)
#define PARAM_TYPE2_EYEFUNC_MASK        (1 << PARAM_TYPE2_EYEFUNC_BIT)
#define PARAM_TYPE2_MCHANNEL            (1 << PARAM_TYPE2_MCHANNEL_BIT)
#define PARAM_TYPE2_PULSE               (1 << PARAM_TYPE2_PULSE_BIT)
#define PARAM_TYPE2_EYEFUNC             (1 << PARAM_TYPE2_EYEFUNC_BIT)
// 2.3.1

//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------

typedef struct {
    unsigned short Number;
    unsigned short Value;
} PARAMETER;

typedef struct {
    PARAMETER Hour;
    PARAMETER Minute;
    PARAMETER Second;
    PARAMETER HfSensitivity;        // 1.0.24
    PARAMETER PirSensitivity;
    PARAMETER UsSensitivity;
    PARAMETER DaylightSetpoint;
    PARAMETER DetectionScheme;
    PARAMETER Initial;
    PARAMETER Maintain;
    PARAMETER Retrigger;
    PARAMETER Mode;
    PARAMETER Alert;
    PARAMETER StandbyLevel;         // 1.0.24
    PARAMETER StandbyDelay;         // 1.0.24
    PARAMETER SendCondition;        // 1.0.24
    PARAMETER DeadBand;             // 1.0.24
    PARAMETER TimeBase;             // 1.0.24
    PARAMETER BandWidth;            // 1.0.24
    PARAMETER LightRegulation;
    PARAMETER LoopType;
    PARAMETER ProvisionOfLight;
    PARAMETER Year;                 // 2.3.1
    PARAMETER Month;                // 2.3.1
    PARAMETER Day;                  // 2.3.1
    PARAMETER SummerHour;           // 2.3.1
    PARAMETER StartMonth;           // 2.3.1
    PARAMETER StartDay;             // 2.3.1
    PARAMETER EndMonth;             // 2.3.1
    PARAMETER EndDay;               // 2.3.1
    PARAMETER Country;              // 2.3.1
    PARAMETER Longitude;            // 2.3.1
    PARAMETER Latitude;             // 2.3.1
    PARAMETER DaylightFactor;       // read write only in hidden page
    PARAMETER DaylightFactorCen;    // read write only in hidden page
    PARAMETER NaturalLightFactor;   // read write only in hidden page
    PARAMETER DeviceIDZigbee;       // read only, writable in hidden page   // 1.0.24
    PARAMETER DeviceIDZigbee2;      // read only, writable in hidden page   // 2.3.1
    PARAMETER Type;                 // read only
    PARAMETER Type2;                // read only    // 2.3.1
    PARAMETER FirmwareVersion;      // read only
    PARAMETER ID1;                  // read only
    PARAMETER ID2;                  // read only
    PARAMETER ID3;                  // read only    // 2.3.1
    PARAMETER ID4;                  // read only    // 2.3.1
    PARAMETER Referent;             // read only
    PARAMETER MainGroup;            // read only    // 1.0.24
    PARAMETER BatteryInformation;   // read only    // 1.0.24
    PARAMETER DBFirmwareVersion;    // read only    // 1.0.24
    PARAMETER DBFirmwareVersion2;   // read only    // 2.3.1
    PARAMETER DaylightLevel;        // read only, writable when used for calibration
    PARAMETER SensorStatus;         // read only in hidden page
    PARAMETER CurrentPOL;           // read only in hidden page     // 1.0.24
    PARAMETER LoadType;             // read/write only in hidden page   // 2.2.0
    PARAMETER PowerLineFilter;      // read/write only in hidden page   // 2.2.0
    PARAMETER BuzzerFeedback;       // read/write only in hidden page   // 2.2.0
    PARAMETER FactoryLightFactor;   // not implemented in menu          // 2.3.1
    PARAMETER GmtSign;              //2.4.1
    PARAMETER GmtHour;              //2.4.1
    PARAMETER GmtMin;              //2.4.1
    PARAMETER KNX_Master_slave;    // 2.4.6
    PARAMETER DetectionLed;        //2.5.7
    PARAMETER ReactionSpeed;       //2.5.7
    PARAMETER OFFset;              //2.5.7
    PARAMETER SensorConfig;         //2.5.7    
    PARAMETER Dim_rise;
    PARAMETER Dim_fal;
    PARAMETER Aux_mode;
    PARAMETER Aux_pul_time;
    PARAMETER On_level;
    PARAMETER Ext_mode_state;
    PARAMETER Ext_func_mode;
    PARAMETER Ext_standby_level;
    PARAMETER Ext_standby_delay;
    PARAMETER Ext_on_level;
    PARAMETER op_time;
    PARAMETER week_day;
    PARAMETER Ext_start_hour;
    PARAMETER Ext_start_min;
    PARAMETER Ext_stop_hour;
    PARAMETER Ext_stop_min;
    PARAMETER Ext_scheduled_day;
    PARAMETER Ext_real_time_hour;
    PARAMETER Ext_real_time_min;
    PARAMETER Ext_real_time_sec;
} SENSOR_PARAMS;

//------------------------------------------------------------------------------
/// Exported Variables
//------------------------------------------------------------------------------

extern __no_init SENSOR_PARAMS sensorParams;
extern const SENSOR_PARAMS *pDefaultFileParams[6];

extern const PARAMETER parameterWalkTest;
extern const PARAMETER parameterEndTimeDelay;
extern const PARAMETER parameterInitialState;

extern const PARAMETER parameterChangeMaster;
extern const PARAMETER parameterChangeSlave;
extern const PARAMETER parameterChangeNop;

extern const unsigned short valuesStandbyLevel[11];
extern const unsigned short valuesStandbyDelay[13];

extern const short valuesLongitude[24];
extern const short valuesLatitude[24];

//------------------------------------------------------------------------------
/// Exported Functions
//------------------------------------------------------------------------------

void PARAMS_ResetToDefault(SENSOR_PARAMS *pParams, unsigned short type);
unsigned char PARAMS_IsParameterAvailable(SENSOR_PARAMS const *pParams, unsigned char Index);
unsigned char PARAMS_ValidateParameterValue(unsigned char Index, unsigned short Value);
unsigned char PARAMS_ValidateDefaultFileValues(const SENSOR_PARAMS *pParams, unsigned char Index, unsigned short Value);
void PARAMS_InterpretStandbyLevel(SENSOR_PARAMS *pParams,int numIndex);
//void PARAMS_InterpretStandbyLevel(SENSOR_PARAMS *pParams);
void PARAMS_InterpretStandbyDelay(SENSOR_PARAMS *pParams);
unsigned char is_KNX_sensor( SENSOR_PARAMS const *_pParams );
unsigned int get_parameters_amount( SENSOR_PARAMS const *_pParams );

//PARAMETER *PARAMS_GetParameter(unsigned char index);
//#define PARAMS_GetParameterPointer(pParams, index)              ((PARAMETER *)pParams + index)
#define PARAMS_GetParameterNumber(pParams, index)               (((PARAMETER *)pParams + index)->Number)
#define PARAMS_GetParameterValue(pParams, index)                (((PARAMETER *)pParams + index)->Value)
//#define PARAMS_GetParameterValuePointer(pParam)                 (&(pParam->Value))
#define PARAMS_SetParameterNumber(pParams, index, number)       (((PARAMETER *)pParams + index)->Number = number)
#define PARAMS_SetParameterValue(pParams, index, value)         (((PARAMETER *)pParams + index)->Value = value)

// 2.3.1
#define PARAMS_IsAutomationSensor(pParams)                      ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE) & PARAM_TYPE_FAMILY_MASK) == PARAM_TYPE_FAMILY_AUTOSENSOR)
#define PARAMS_IsLightController(pParams)                       ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE) & PARAM_TYPE_FAMILY_MASK) == PARAM_TYPE_FAMILY_LIGHTCTRL)
#define PARAMS_IsMeterSensor(pParams)                           ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE) & PARAM_TYPE_FAMILY_MASK) == PARAM_TYPE_FAMILY_METERSENSOR)

#define PARAMS_IsMultiChannelAvailable(pParams)                 (PARAMS_IsParameterAvailable(pParams, PARAM_INDEX_TYPE2) && ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE2) & PARAM_TYPE2_MCHANNEL_MASK) == PARAM_TYPE2_MCHANNEL))
#define PARAMS_IsPulseAvailable(pParams)                        (PARAMS_IsParameterAvailable(pParams, PARAM_INDEX_TYPE2) && ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE2) & PARAM_TYPE2_PULSE_MASK) == PARAM_TYPE2_PULSE))
#define PARAMS_IsEyeFuncAvailable(pParams)                      (PARAMS_IsParameterAvailable(pParams, PARAM_INDEX_TYPE2) && ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE2) & PARAM_TYPE2_EYEFUNC_MASK) == PARAM_TYPE2_EYEFUNC))
// 2.3.1

#define PARAMS_IsStandaloneSensor(pParams)                      ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE) & PARAM_TYPE_CATEGORY_MASK) == PARAM_TYPE_CATEGORY_STANDALONE)
#define PARAMS_IsCentralizedSensor(pParams)                     ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE) & PARAM_TYPE_CATEGORY_MASK) == PARAM_TYPE_CATEGORY_CENTRALIZED)

#define PARAMS_SensorHasNoTech(pParams)                         ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE) & PARAM_TYPE_TECH_MASK) == PARAM_TYPE_TECH_NONE)
#define PARAMS_SensorHasHF(pParams)                             (PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE) & PARAM_TYPE_TECH_HF_ONLY)     // 2.3.0
#define PARAMS_SensorHasPIR(pParams)                            (PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE) & PARAM_TYPE_TECH_PIR_ONLY)    // 2.3.0
#define PARAMS_SensorHasUS(pParams)                             (PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE) & PARAM_TYPE_TECH_US_ONLY)     // 2.3.0
#define PARAMS_SensorHasPIRandUS(pParams)                       ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE) & PARAM_TYPE_TECH_DUAL) == PARAM_TYPE_TECH_DUAL)      // 1.0.24
#define PARAMS_SensorHasPIRandHF(pParams)                       ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE) & PARAM_TYPE_TECH_PIR_HF) == PARAM_TYPE_TECH_PIR_HF)      // 1.0.24
#define PARAMS_SensorHasAllTech(pParams)                        ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE) & PARAM_TYPE_TECH_MASK) == PARAM_TYPE_TECH_MASK)      // 1.0.24
#define PARAMS_SensorIsHFOnly(pParams)                          ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE) & PARAM_TYPE_TECH_MASK) == PARAM_TYPE_TECH_HF_ONLY)      // 1.0.24

//new dali type
#define PARAMS_IsDALI_NOP(pParams)                              ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE))==PARAM_TYPE_DALI_NOP) 
#define PARAMS_IsDALI_MASTER_DL_PIR_US(pParams)                 ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE))==PARAM_TYPE_DALI_MASTER_DL_PIR_US) 
#define PARAMS_IsDALI_MASTER_DL_OP_PIR_US(pParams)              ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE))==PARAM_TYPE_DALI_MASTER_DL_OP_PIR_US) 
#define PARAMS_IsDALI_MASTER_OP_PIR_US(pParams)                 ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE))==PARAM_TYPE_DALI_MASTER_OP_PIR_US) 
#define PARAMS_IsDALI_SLAVE_OP_PIR_US(pParams)                   ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE))==PARAM_TYPE_DALI_SLAVE_OP_PIR_US) 
#define PARAMS_IsDALI_MASTER_DL_OP_PIR(pParams)               ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE))==PARAM_TYPE_DALI_MASTER_DL_OP_PIR) 
#define PARAMS_IsDALI_MASTER_OP_PIR(pParams)                     ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE))==PARAM_TYPE_DALI_MASTER_OP_PIR) 
#define PARAMS_IsDALI_SLAVE_OP_PIR(pParams)                      ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE))==PARAM_TYPE_DALI_SLAVE_OP_PIR ) 
#define PARAMS_IsDALI_MASTER_PIR_DL(pParams)                      ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE))==PARAM_TYPR_DALI_MASTER_DL_PIR )


// 1.0.24
//#define PARAMS_IsZigbeeSensor(pParams)                          ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE) & PARAM_TYPE_ZB_MASK) == PARAM_TYPE_ZB)
#define PARAMS_IsZigbeeSensor(pParams)                          (PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE)==PARAM_TYPE_ZB_PIR || PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE)==PARAM_TYPE_ZB_LV_PIR || PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE)==PARAM_TYPE_ZB_DUAL || PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE)==PARAM_TYPE_ZB_LV_DUAL || PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE)==PARAM_TYPE_ZB_HF_PIR  || PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE)==PARAM_TYPE_ZB_LV_HF_PIR)
#define PARAMS_IsLineVoltageSensor(pParams)                     ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE) & PARAM_TYPE_CLASS_MASK) == PARAM_TYPE_CLASS_LV)
#define PARAMS_IsDigitalSensor(pParams)                         ((PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE) & PARAM_TYPE_CLASS_MASK) == PARAM_TYPE_CLASS_DGT)

#endif

extern  const unsigned char *KNX_sensor_type_name[];
unsigned char is_DALI_sensor(SENSOR_PARAMS const *_pParams);
unsigned char is_PRECIR_sensor(SENSOR_PARAMS const *_pParams);