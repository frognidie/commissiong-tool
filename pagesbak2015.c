
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dataflash.h"
#include "utilities.h"
#include "battery.h"
#include "buttons.h"
#include "font.h"
#include "lcdd.h"
#include "languages.h"
#include "strings.h"
#include "bitmap.h"
#ifndef APP_BAES
#include "parameters.h"
#endif  // #ifndef APP_BAES
#include "ir.h"
#include "time.h"
#include "pages.h"
#ifndef APP_BAES
#include "files.h"
#endif  // #ifndef APP_BAES
#include "version.h"
#include "country.h"
#include  <stdbool.h>

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

#define PAGE_LINE_INDENT            1
//#define PAGE_NORMAL_COLOR           0
#define PAGE_HIGHLIGHT_COLOR        3
#define PAGE_ARROW_NAV_X_POS        LCD_X_LENGTH-BITMAP_ARROW_NAV_WIDTH

#define PAGE_FUNCKEY_AREA_HEIGHT    20
#define PAGE_FUNCKEY_Y_START        (LCD_Y_LENGTH-PAGE_FUNCKEY_AREA_HEIGHT+2)
#define PAGE_FUNCKEY_Y_END          (LCD_Y_LENGTH-2)
#define PAGE_FUNCKEY_1_WIDTH        30
#define PAGE_FUNCKEY_2_WIDTH        31
#define PAGE_FUNCKEY_3_WIDTH        31
#define PAGE_FUNCKEY_4_WIDTH        31
#define PAGE_FUNCKEY_1_X_START      1
#define PAGE_FUNCKEY_1_X_END        (PAGE_FUNCKEY_1_X_START+PAGE_FUNCKEY_1_WIDTH-1)
#define PAGE_FUNCKEY_2_X_START      (PAGE_FUNCKEY_1_X_END+2)
#define PAGE_FUNCKEY_2_X_END        (PAGE_FUNCKEY_2_X_START+PAGE_FUNCKEY_2_WIDTH-1)
#define PAGE_FUNCKEY_3_X_START      (PAGE_FUNCKEY_2_X_END+2)
#define PAGE_FUNCKEY_3_X_END        (PAGE_FUNCKEY_3_X_START+PAGE_FUNCKEY_3_WIDTH-1)
#define PAGE_FUNCKEY_4_X_START      (PAGE_FUNCKEY_3_X_END+2)
#define PAGE_FUNCKEY_4_X_END        (PAGE_FUNCKEY_4_X_START+PAGE_FUNCKEY_4_WIDTH-1)

#define PAGE_PARAM_READ_RETRY       3

//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------

typedef struct {
    const unsigned char *pString;
    unsigned char PageHandlerIndex;
} PAGE_MENU_ITEM;

typedef struct {
    unsigned char Width;
    unsigned char Height;
    void *pData;
} PAGE_ITEM;

typedef struct {
    unsigned char yStart;
    unsigned char yEnd;
} PAGE_ITEM_RECT;

typedef struct {
    unsigned char const *pName;
    unsigned char *pValue;
    unsigned char ValueType;    // 0: string    1: icon
} PAGE_READ_PARAMS;

typedef struct {
    const unsigned char *pString;
    unsigned char PageHandlerIndex;
} PAGE_PARAMS;

typedef struct {
    unsigned char ParamIndex;
    unsigned char Updated;
    unsigned short Max;
    unsigned short Min;
} PAGE_PARAMETER;

typedef struct {
    unsigned char ButtonIndex;
    unsigned char RectStartX;
    unsigned char RectStartY;
    unsigned char RectEndX;
    unsigned char RectEndY;
} PAGE_BUTTON_RECT;

typedef struct {
    unsigned char RectStartX;
    unsigned char RectEndX;
} PAGE_FUNCKEY_RECT;

typedef struct {
    void *pItem;
    unsigned char itemType; // 0 refers to bitmap, 1 refers to string
} PAGE_FUNCKEY_ITEM;

#ifndef APP_BAES
typedef struct {
    unsigned char pFileName[FILES_MAX_NAME+1];
} PAGE_FILE_PARAMS;
#endif // #ifndef APP_BAES

//------------------------------------------------------------------------------
//         Local Variables
//------------------------------------------------------------------------------

//extern  view_zone view;
extern __no_init VIEW view;
extern __no_init GROUPNUM GroupNum;

static unsigned char pageInitialize = 1;

static unsigned char pageUpdate = 0;

static unsigned char pageSwap = 0;

static unsigned char pageButtonActive = 0;

static const PAGE_FUNCKEY_RECT pageFunckeyRect[4] = {
    {PAGE_FUNCKEY_1_X_START, PAGE_FUNCKEY_1_X_END},
    {PAGE_FUNCKEY_2_X_START, PAGE_FUNCKEY_2_X_END},
    {PAGE_FUNCKEY_3_X_START, PAGE_FUNCKEY_3_X_END},
    {PAGE_FUNCKEY_4_X_START, PAGE_FUNCKEY_4_X_END}
};

static PAGE_FUNCKEY_ITEM pageFunckeyItems[4] = {0};

#ifndef APP_BAES
static unsigned char pageParent = 0;

static unsigned char argForSubPage = 0;
static unsigned char argForSubPage2 = 0;

static unsigned char pageAvailability = 1;

static unsigned char pageForceRead = 0;

static unsigned char pageScroll = 0;    // 2.4.0

static unsigned int pageTimeStamp = 0xffffffff;     // 2.4.0
#else
const unsigned char Info_Digits[10][13] = {
    {11,1,12,2,7,2,9,1,10,2,7,0,2},
    {0,0,5,2,7,2,3,0,0,0,0,0,0},
    {4,1,12,2,9,0,6,1,10,2,11,1,0},
    {4,1,12,2,13,2,9,1,4,0,4,1,0},
    {5,0,5,2,13,2,3,0,0,0,10,1,2},
    {11,1,6,0,12,2,9,1,4,0,10,1,2},
    {11,1,6,0,12,2,9,1,10,2,14,1,2},
    {4,1,12,2,7,2,3,0,0,0,0,0,0},
    {11,1,12,2,13,2,9,1,10,2,14,1,2},
    {11,1,12,2,13,2,9,1,4,0,10,1,2},
};
#endif

//------------------------------------------------------------------------------
///        Exported Variables
//------------------------------------------------------------------------------

PAGE_HANDLER pageHandler[] = {
#ifndef APP_BAES
    MainPageHandler,                //PAGE_INDEX_MAIN
    ReadParametersPageHandler,      //PAGE_INDEX_READ_PARAMETERS
    ListParametersPageHandler,      //PAGE_INDEX_LIST_PARAMETERS
    SaveFilePageHandler,            //PAGE_INDEX_SAVEFILE
    NewFilePageHandler,             //PAGE_INDEX_NEWFILE
    ReplaceFilePageHandler,         //PAGE_INDEX_REPLACEFILE
    ParametersPageHandler,          //PAGE_INDEX_PARAMETERS
    AdvancedModePageHandler,        //PAGE_INDEX_ADVANCEDMODE
    HiddenPageHandler,              //PAGE_INDEX_HIDDEN
    DatePageHandler,                //PAGE_INDEX_DATE           // 2.4.0
    TimedelayPageHandler,           //PAGE_INDEX_TIMEDELAY
    SensitivityPageHandler,         //PAGE_INDEX_SENSITIVITY
    CalibrationPageHandler,         //PAGE_INDEX_CALIBRATION
    DaylightSetpointPageHandler,    //PAGE_INDEX_DAYLIGHTSETPOINT
    ModePageHandler,                //PAGE_INDEX_MODE
    DetectionschemePageHandler,     //PAGE_INDEX_DETECTIONSCHEME
    AlertPageHandler,               //PAGE_INDEX_ALERT
    StandbylevelPageHandler,        //PAGE_INDEX_STANDBYLEVEL
    StandbydelayPageHandler,        //PAGE_INDEX_STANDBYDELAY
    SendconditionPageHandler,       //PAGE_INDEX_SENDCONDITION
    DeadbandPageHandler,            //PAGE_INDEX_DEADBAND
    TimebasePageHandler,            //PAGE_INDEX_TIMEBASE
    BandwidthPageHandler,           //PAGE_INDEX_BANDWIDTH
    SummerhourPageHandler,          //PAGE_INDEX_SUMMERHOUR     // 2.4.0
    CountryPageHandler,             //PAGE_INDEX_COUNTRY        // 2.4.0
    LightregulationPageHandler,     //PAGE_INDEX_LIGHTREGULATION
    ProvisionoflightPageHandler,    //PAGE_INDEX_PROVISIONOFLIGHT
    LooptypePageHandler,            //PAGE_INDEX_LOOPTYPE
    DaylightFactorPageHandler,      //PAGE_INDEX_DAYLIGHTFACTOR
    DaylightFactorCenPageHandler,   //PAGE_INDEX_DAYLIGHTFACTORCEN
    NaturalLightFactorPageHandler,  //PAGE_INDEX_NATURALLIGHTFACTOR
    DaylightLevelPageHandler,       //PAGE_INDEX_DAYLIGHTLEVEL
    SensorStatusPageHandler,        //PAGE_INDEX_SENSORSTATUS
    CurrentPOLPageHandler,          //PAGE_INDEX_CURRENTPOL
    DeviceIDZigbeePageHandler,      //PAGE_INDEX_DEVICEIDZIGBEE
    OtherparametersPageHandler,     //PAGE_INDEX_OTHERPARAMETERS
    BuzzerfeedbackPageHandler,      //PAGE_INDEX_BUZZERFEEDBACK
    LoadtypePageHandler,            //PAGE_INDEX_LOADTYPE
    PowerlinefilterPageHandler,     //PAGE_INDEX_POWERLINEFILTER
    TypePageHandler,                //PAGE_INDEX_TYPE
    FirmwareVersionPageHandler,     //PAGE_INDEX_FIRMWAREVERSION
    IDPageHandler,                  //PAGE_INDEX_ID
    ReferentPageHandler,            //PAGE_INDEX_REFERENT
    FilesPageHandler,               //PAGE_INDEX_FILES
    DefaultFilesPageHandler,        //PAGE_INDEX_DEFAULTFILES
    FileParametersPageHandler,      //PAGE_INDEX_FILE_PARAMETERS
    SendFilePageHandler,            //PAGE_INDEX_SENDFILE
    SendDefaultFilePageHandler,     //PAGE_INDEX_SENDDEFAULTFILE
    DeleteFilePageHandler,          //PAGE_INDEX_DELETEFILE
    PnLEntryPageHandler,            //PAGE_INDEX_PNLENTRY
    PnLPageHandler,                 //PAGE_INDEX_PNL
    SettingsPageHandler,            //PAGE_INDEX_SETTINGS
    LanguagesPageHandler,           //PAGE_INDEX_LANGUAGES
    VersionPageHandler,             //PAGE_INDEX_VERSION
    BatteryPageHandler,             //PAGE_INDEX_BATTERY
    KNX_master_slave_PageHandler,    // PAGE_INDEX_KNX_MASTER_SLAVE
    TimeZonePageHandler,            //PAGE_INDEX_TIMEZONE
#ifndef NO_APPSWITCH
    AppSwitchPageHandler,           //PAGE_INDEX_APPSWITCH
#endif  // #ifndef NO_APPSWITCH
    TestPageHandler,                //PAGE_INDEX_TEST
    ControlPageHandler,             //PAGE_INDEX_CONTROL        // 2.3.0
    HelpPageHandler,                //PAGE_INDEX_HELP
    DaliMasterPageHandler,          //PAGE_INDEX_DALI_MASTER
    DaliSlavePageHandler,           //PAGE_INDEX_DALI_SLAVE
    DaliNopPageHandler,             //PAGE_INDEX_DALI_NOP    
    DaliControlGearHandler,          //PAGE_CONTROL_GEAR,
    DaliDetectorHandler,             //PAGE_DETECTOR,  
    DaliChangeTypeHandler,           //PAGE_CHANGE_TYPE,                                             
    DaliCreateZoneHandler,           //PAGE_CREATE_ZONE
    DaliViewAreaHandler,             //PAGE_VIEW_AREA  
    DaliCreateGroupHandler,           //PAGE_CREATE_GROUP
    DaliChooseZoneHandler,            //PAGE_CHOOSE_ZONE  
    DALIOffSetHandler,               //PAGE_INDEX_OFFSET;
    DALIDetectionLEDHandler,         //PAGE_INDEX_Detection_LED;
    DALISensorConfigHandler,         //PAGE_INDEX_SensorConfig;  
    DALIdetectorCreateGroupHandler, //PAGE_INDEX_DetectorCreateGroup     
    DALIDetectorFinishCreate,       //PAGE_INDEX_DetectorFinishCreate
    DALIDetectorAddSlave,           //PAGE_INDEX_ADDSLAVE  
    DALIZoneViewWindows,             // PAGE_INDEX_WINDOWS,
    DALIZoneViewCorridor,            // PAGE_INDEX_CORRIDOR,
    DALIZoneViewBoard,            //  PAGE_INDEX_BOARD 
    DaliViewAreaCorridHandler ,            //PAGE_VIEW_AREA_CORRID 
    DaliChooseZoneCorridHandler,     //PAGE_CHOOSE_ZONE_CORRID
#ifdef ROOM_CONTROL
    RoomControlPageHandler,         //PAGE_INDEX_ROOMCONTROL
#endif
#else
    MainPageHandler,                //PAGE_INDEX_MAIN
    AdresserPageHandler,            //PAGE_INDEX_ADRESSER
    TesterPageHandler,              //PAGE_INDEX_TESTER
    SettingsPageHandler,            //PAGE_INDEX_SETTINGS
    VersionPageHandler,             //PAGE_INDEX_VERSION
    BatteryPageHandler,             //PAGE_INDEX_BATTERY
#ifndef NO_APPSWITCH
    AppSwitchPageHandler,           //PAGE_INDEX_APPSWITCH
#endif  // #ifndef NO_APPSWITCH
#endif  // #ifndef APP_BAES
};

unsigned char pageCurrent = PAGE_INDEX_MAIN;
unsigned char pagePowerOff = 0;
extern const unsigned char *KNX_sensor_master_slave_menu_item_langeage_string[];
extern const unsigned char *KNX_sensor_master_slave_title_langeage_string[];
extern const unsigned char *KNX_sensor_master_langeage_string[];
extern const unsigned char *KNX_sensor_slave_langeage_string[];

//extern unsigned char xx_createzone;

unsigned summer_hour_check( void );

//------------------------------------------------------------------------------
///        Internal Functions
//------------------------------------------------------------------------------
// return the central xpos of string
unsigned char _figure_string_center_x_pos(unsigned char start, unsigned char end, const unsigned char *pString)
{
    unsigned char result = 0;

    while (*pString)
    {
        result += LCDD_PutChar(0, 0, *pString, 0, 1);
        pString++;
    }

    // 2.4.0
    if (start+end >= result)
    {
        return ((start+end-result)>>1);
    }
    else
    {
        return start;
    }
}

void _common_battery_condition(void)
{
    if (pagePowerOff)
    {
        pagePowerOff = 0;
        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_LOWBATTERY+language]), 56, 0, strings[STRINGS_INDEX_LOWBATTERY+language], 0, 0);
        pageSwap = 1;
        pageCurrent = PAGE_INDEX_MAIN;
    }
}

void _draw_funckey_area_frame(void)
{
    LCDD_ClearRect(0, LCD_Y_LENGTH-PAGE_FUNCKEY_AREA_HEIGHT, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 3);
    for (unsigned char i=0; i<4; i++)
    {
        LCDD_ClearRect(pageFunckeyRect[i].RectStartX, PAGE_FUNCKEY_Y_START, pageFunckeyRect[i].RectEndX, PAGE_FUNCKEY_Y_END, 0);
    }
}

#define _figure_icon_center_pos(start, end, length) ((start+end-length)>>1)

void _draw_funckey_icon(unsigned char xStart, unsigned char xEnd, const BitmapInfo *pBitmapInfo)
{
    unsigned char x,y;

    x = _figure_icon_center_pos(xStart, xEnd, pBitmapInfo->Width);
    y = _figure_icon_center_pos(PAGE_FUNCKEY_Y_START, PAGE_FUNCKEY_Y_END, pBitmapInfo->Height);
    LCDD_PutBitmap(x, y, pBitmapInfo, 0);
}

void _put_funckey_string(unsigned char xStart, unsigned char xEnd, const unsigned char *pString)
{
    unsigned char x;

    x = _figure_string_center_x_pos(xStart, xEnd, pString);
    LCDD_PutString(x, PAGE_FUNCKEY_Y_START, 0, pString, 0, 0);
}

void _common_funckey_initializer(void)
{
    _draw_funckey_area_frame();
    for (unsigned char i=0; i<4; i++)
    {
        if (pageFunckeyItems[i].pItem)
        {
            if (pageFunckeyItems[i].itemType == 0)    // 0 refers to bitmap
            {
                _draw_funckey_icon(pageFunckeyRect[i].RectStartX, pageFunckeyRect[i].RectEndX, (const BitmapInfo *)pageFunckeyItems[i].pItem);
            }
            else    // string
            {
                _put_funckey_string(pageFunckeyRect[i].RectStartX, pageFunckeyRect[i].RectEndX, (const unsigned char *)pageFunckeyItems[i].pItem);
            }
        }
    }
}

void _common_funckey_handler(void)
{
    unsigned char i;

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHING) && (pageButtonActive == 0))
    {
        pageButtonActive = 1;
        i = BUTTON_GetButtonIndex();
        if ((i >= BUTTON_INDEX_F1) && (i <= BUTTON_INDEX_F4) && pageFunckeyItems[i-BUTTON_INDEX_F1].pItem)
        {
            LCDD_InvertRect(pageFunckeyRect[i-BUTTON_INDEX_F1].RectStartX, PAGE_FUNCKEY_Y_START, pageFunckeyRect[i-BUTTON_INDEX_F1].RectEndX, PAGE_FUNCKEY_Y_END, PAGE_HIGHLIGHT_COLOR);
        }
    }
    else if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        pageButtonActive = 0;
        i = BUTTON_GetButtonIndex();
        if ((i >= BUTTON_INDEX_F1) && (i <= BUTTON_INDEX_F4) && pageFunckeyItems[i-BUTTON_INDEX_F1].pItem)
        {
            LCDD_InvertRect(pageFunckeyRect[i-BUTTON_INDEX_F1].RectStartX, PAGE_FUNCKEY_Y_START, pageFunckeyRect[i-BUTTON_INDEX_F1].RectEndX, PAGE_FUNCKEY_Y_END, PAGE_HIGHLIGHT_COLOR);
        }
    }
}

#ifndef APP_BAES
#define FUNCKEY_NONE        0
#define FUNCKEY_READ_ONLY   1
#define FUNCKEY_WRITE_ONLY  2
#define FUNCKEY_READ_WRITE  3
#define FUNCKEY_READ_MASK   1
#define FUNCKEY_WRITE_MASK  2
void _param_page_funckey_initializer(unsigned char availability)
{
    memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
    pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
    if (availability & FUNCKEY_READ_MASK)
    {
        pageFunckeyItems[1].pItem = (void *)&BitmapInfo_ReadSensor;
    }
    if (availability & FUNCKEY_WRITE_MASK)
    {
        pageFunckeyItems[2].pItem = (void *)&BitmapInfo_WriteSensor;
    }
    _common_funckey_initializer();
}
void _param_pageDali_funckey_initializer(unsigned char availability)
{
    memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
    pageFunckeyItems[0].pItem = (void *)&BitmapInfo_BackHome;
    if (availability & FUNCKEY_READ_MASK)
    {
        pageFunckeyItems[1].pItem = (void *)&BitmapInfo_ReadSensor;
    }
    if (availability & FUNCKEY_WRITE_MASK)
    {
        pageFunckeyItems[2].pItem = (void *)&BitmapInfo_WriteSensor;
    }
    _common_funckey_initializer();
}






void _adjust_param_value(PAGE_PARAMETER *pPageParam, signed short step)
{
    signed short value;
    volatile signed short max,min;
    
    value = PARAMS_GetParameterValue(&sensorParams, pPageParam->ParamIndex);
    value += step;
    
    min = (signed short)(pPageParam->Min);
    max = (signed short)(pPageParam->Max);
    
    if (value < min )
    {
        value = pPageParam->Max;
    }
    else if (value > max )
    {
        value = pPageParam->Min;
    }

    PARAMS_SetParameterValue(&sensorParams, pPageParam->ParamIndex, (unsigned short)value);
}

// return 1 if successfully read, 0 otherwise.
unsigned char _page_sub_read_parameters(PAGE_PARAMETER *pPageParam, unsigned char number)
{
#define PAGE_SUBREADPARAMS_RECT_X_START     8
#define PAGE_SUBREADPARAMS_RECT_Y_START     20
#define PAGE_SUBREADPARAMS_RECT_X_END       (LCD_X_LENGTH-PAGE_SUBREADPARAMS_RECT_X_START-1)
#define PAGE_SUBREADPARAMS_BODY_INDENT      4
#define PAGE_SUBREADPARAMS_BODY_X_START     (PAGE_SUBREADPARAMS_RECT_X_START+PAGE_SUBREADPARAMS_BODY_INDENT)
#define PAGE_SUBREADPARAMS_BODY_X_LENGTH    (PAGE_SUBREADPARAMS_RECT_X_END-PAGE_SUBREADPARAMS_BODY_INDENT-PAGE_SUBREADPARAMS_BODY_X_START)
#define PAGE_SUBREADPARAMS_BODY_Y_START     (PAGE_SUBREADPARAMS_RECT_Y_START+PAGE_SUBREADPARAMS_BODY_INDENT)

    unsigned short tempType;
    unsigned char retry = 0;
    unsigned char yOffset;
    static unsigned char result;
    unsigned char const *string = NULL;
    unsigned char currentParameter;
    unsigned char read = 0;

    tempType = PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_TYPE);
    while (retry < PAGE_PARAM_READ_RETRY)
    {
        if (pageForceRead || (result = IR_ReadSensorParameter(&sensorParams, PARAM_INDEX_TYPE)))
        {
            retry = 0;
            switch (result)
            {
            case 1:
                if (pageForceRead == 0)
                {
                    TIME_Wait(200);
                }
                else
                {
                    pageForceRead = 0;
                }
                 
                if (tempType != PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_TYPE))
                {
                    PARAMS_ResetToDefault(&sensorParams, PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_TYPE));
                    pageForceRead = 1;
                }
//                string = NULL;
                break;
            case 2:
                string = strings[STRINGS_INDEX_UNKNOWNVALUE+language];
                break;
            case 3:
                string = strings[STRINGS_INDEX_NOTIMPLEMENTED+language];
                break;
            default:
                string = strings[STRINGS_INDEX_ERRORREADING+language];
                break;
            }
            break;
        }
        else
        {
            if (++retry < PAGE_PARAM_READ_RETRY)
            {
                TIME_Wait(200);
            }
            else
            {
                string = strings[STRINGS_INDEX_ERRORREADING+language];
            }
        }
    }

    if ((result == 1) && (pageForceRead == 0))
    {
        while (number > 0)
        {
            currentParameter = (pPageParam+number-1)->ParamIndex;
            if (PARAMS_IsParameterAvailable(&sensorParams, currentParameter))
            {
                read = 1;   // Indicate that at least 1 parameter has been tried to read.
                while (retry < PAGE_PARAM_READ_RETRY)
                {
                    if (result = IR_ReadSensorParameter(&sensorParams, currentParameter))
                    {
                        retry = 0;
                        switch (result)
                        {
                        case 1:
                            //string = NULL;
                            if (currentParameter == PARAM_INDEX_STANDBYLEVEL)
                            {
                                PARAMS_InterpretStandbyLevel(&sensorParams);
                            }
                            else if (currentParameter == PARAM_INDEX_STANDBYDELAY)
                            {
                                PARAMS_InterpretStandbyDelay(&sensorParams);
                            }
                            break;
                        case 2:
                            string = strings[STRINGS_INDEX_UNKNOWNVALUE+language];
                            break;
                        case 3:
//                            string = strings[STRINGS_INDEX_NOTIMPLEMENTED+language];
                            break;
                        default:
                            string = strings[STRINGS_INDEX_ERRORREADING+language];
                            break;
                        }
                        if (--number > 0)
                        {
                            TIME_Wait(200);
                        }
                        break;
                    }
                    else
                    {
                        if (++retry < PAGE_PARAM_READ_RETRY)
                        {
                            TIME_Wait(200);
                        }
                    }
                }

                if (retry >= PAGE_PARAM_READ_RETRY)
                {
                    string = strings[STRINGS_INDEX_ERRORREADING+language];
                    break;
                }
            }
            else
            {
                number--;
            }
        }

        if (read == 0)  // None of the parameter(s) is implemented.
        {
            string = strings[STRINGS_INDEX_NOTIMPLEMENTED+language];
        }
    }
    
    if (string != NULL)
    {
        if( PARAM_NUMBER_TYPE2 == PARAMS_GetParameterNumber( &sensorParams, currentParameter ) )
        {
            PARAMS_SetParameterNumber(&sensorParams, PARAM_INDEX_TYPE2, PARAM_NUMBER_ERROR ); 
        }
        else
        {
            yOffset = PAGE_SUBREADPARAMS_BODY_Y_START+LCDD_PutString(PAGE_SUBREADPARAMS_BODY_X_START, PAGE_SUBREADPARAMS_BODY_Y_START, PAGE_SUBREADPARAMS_BODY_X_LENGTH, string, 3, 1)+PAGE_SUBREADPARAMS_BODY_INDENT;
            LCDD_ClearRect(PAGE_SUBREADPARAMS_RECT_X_START, PAGE_SUBREADPARAMS_RECT_Y_START, PAGE_SUBREADPARAMS_RECT_X_END, yOffset, 3);
            LCDD_PutString(PAGE_SUBREADPARAMS_BODY_X_START, PAGE_SUBREADPARAMS_BODY_Y_START, PAGE_SUBREADPARAMS_BODY_X_LENGTH, string, 3, 0);
            LCDD_Refresh();     // Have to do this here.
            TIME_Wait(2000);
            result = 0;            
        }
    }

    pageUpdate = 0;
    pageSwap = 1;

    return result;
}

void _page_sub_set_parameters(PAGE_PARAMETER *pPageParam, unsigned char number)
{
    for (unsigned char i=0; i<number; i++)
    {
        if (PARAMS_IsParameterAvailable(&sensorParams, (pPageParam+i)->ParamIndex))
        {
            IR_SetSensorParameter(&sensorParams, (pPageParam+i)->ParamIndex);
//            (pPageParam+i)->Updated = 1;

           TIME_Wait(25);
        }
    }
}

void _list_parameters(SENSOR_PARAMS const *pSensorParams, unsigned char DisplayReadOnlyItems, unsigned char Swap)
{
#define PARAMETERS_AMOUNT_MAX           36      // 2.4.0, was 29
#define PARAMETERS_VALUE_LENGTH_MAX     22
#define PARAMETERS_LINE_1               16
#define PARAMETERS_LINE_HEIGHT          16
#define PARAMETERS_LINE_SPACE           6
#define PARAMETERS_LINE_2               (PARAMETERS_LINE_1 + PARAMETERS_LINE_HEIGHT + PARAMETERS_LINE_SPACE)
#define PARAMETERS_LINE_3               (PARAMETERS_LINE_2 + PARAMETERS_LINE_HEIGHT + PARAMETERS_LINE_SPACE)
#define PARAMETERS_LINE_4               (PARAMETERS_LINE_3 + PARAMETERS_LINE_HEIGHT + PARAMETERS_LINE_SPACE)

    static unsigned char pageSubUpdate = 0;
    static unsigned char currentItem = 0;
    static unsigned char totalItems = 0;
    static PAGE_READ_PARAMS *pReadParameters = 0;
    PAGE_READ_PARAMS *pParametersTemp;
    static unsigned char *pValueStrings = 0;
    unsigned char *pValuesTemp;
    static unsigned char displaySelection = 1;  //0: display names, 1: display values
    unsigned char firstDisplayedItem;

    if (pageInitialize)
    {
        LCDD_ClearRect(0, PARAMETERS_LINE_1, LCD_X_LENGTH-1, LCD_Y_LENGTH-PAGE_FUNCKEY_AREA_HEIGHT-1, 0);

        currentItem = 0;

        displaySelection = 1;

        pageScroll = 0;  // 2.4.0

        pReadParameters = (PAGE_READ_PARAMS *)malloc(PARAMETERS_AMOUNT_MAX*sizeof(unsigned char *)*2);
        pParametersTemp = pReadParameters;

        pValueStrings = (unsigned char *)malloc(PARAMETERS_AMOUNT_MAX*PARAMETERS_VALUE_LENGTH_MAX);
        pValuesTemp = pValueStrings;

        // Date     // 2.4.0
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_YEAR) && PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_MONTH) && PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_DAY))
        {
            pParametersTemp->pName = strings[STRINGS_INDEX_DATE+language];
            pParametersTemp->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            pValuesTemp += dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_DAY), 2);
            strcpy((char *)pValuesTemp, " / ");
            pValuesTemp += 3;
            pValuesTemp += dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_MONTH), 2);
            strcpy((char *)pValuesTemp, " / ");
            pValuesTemp += 3;
            pValuesTemp += (dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_YEAR)+2000, 4) + 1);
        }

        // Time delay
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_HOUR) || PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_MINUTE) || PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_SECOND))
        {
            if (PARAMS_IsLightController(pSensorParams))    // 2.4.0
            {
                pParametersTemp->pName = strings[STRINGS_INDEX_HOUR+language];
            }
            else
            {
                pParametersTemp->pName = strings[STRINGS_INDEX_TIMEDELAY+language];
            }

            // 2.3.1
            if (PARAMS_IsPulseAvailable(pSensorParams) && \
                ((PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_HOUR) && ((PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_HOUR) | PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_MINUTE) | PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_SECOND)) == 0)) || \
                 ((PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_MINUTE) | PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_SECOND)) == 0)))
            {
                pParametersTemp->pValue = (unsigned char *)&BitmapInfo_Pulse_S;
                pParametersTemp->ValueType = 1; // icon
            }
            else
            {
                pParametersTemp->pValue = pValuesTemp;
                pParametersTemp->ValueType = 0; // string
                if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_HOUR))
                {
                    pValuesTemp += dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_HOUR), 0); //hour value
                    if (language == German)     // 2.4.0
                    {
                        strcpy((char *)pValuesTemp, " Std ");
                        pValuesTemp += 5; //strlen((char const *)" Std ");
                    }
                    else
                    {
                        strcpy((char *)pValuesTemp, " H ");
                        pValuesTemp += 3; //strlen((char const *)" H ");
                    }
                }
                if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_MINUTE))
                {
                    pValuesTemp += dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_MINUTE), 0); //minute value
                    strcpy((char *)pValuesTemp, " Min ");
                    pValuesTemp += 5; //strlen((char const *)" Min ");
                }
                if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_SECOND))
                {
                    pValuesTemp += dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_SECOND), 0); //second value
                    if (language == German)     // 2.4.0
                    {
                        strcpy((char *)pValuesTemp, " Sek");
                    }
                    else
                    {
                        strcpy((char *)pValuesTemp, " Sec");
                    }
                    pValuesTemp += 5; //strlen((char const *)" Sec") + 1;
                }
            }
            pParametersTemp++;
        }

        // HF sensitivity
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_HF_SENS))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_SENSITIVITY+language];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            strcpy((char *)pValuesTemp, "HF > ");
            pValuesTemp += 5; //strlen((char const *)"HF > ");
            strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_LOW+STRINGS_TOTAL_LANGUAGES*(unsigned char)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_HF_SENS)+language]);
            pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
        }

        // PIR sensitivity
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_PIR_SENS))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_SENSITIVITY+language];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            strcpy((char *)pValuesTemp, "PIR > ");
            pValuesTemp += 6; //strlen((char const *)"PIR > ");
            strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_LOW+STRINGS_TOTAL_LANGUAGES*(unsigned char)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_PIR_SENS)+language]);
            pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
        }

        // US sensitivity
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_US_SENS))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_SENSITIVITY+language];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            strcpy((char *)pValuesTemp, "US  > ");
            pValuesTemp += 6; //strlen((char const *)"US  > ");
            strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_LOW+STRINGS_TOTAL_LANGUAGES*(unsigned char)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_US_SENS)+language]);
            pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
        }

        // Daylight setpoint
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_DAYLIGHTSETPOINT))
        {
            if (PARAMS_IsLightController(pSensorParams))    // 2.4.0
            {
                (pParametersTemp)->pName = strings[STRINGS_INDEX_SETPOINT ];
            }
            else
            {
                (pParametersTemp)->pName = strings[STRINGS_INDEX_DAYLIGHTSETPOINT ];
            }
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            pValuesTemp += dectoascii(pValuesTemp, 5*PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_DAYLIGHTSETPOINT), 0);
            if (PARAMS_SensorIsHFOnly(&sensorParams) )     // 2.2.0
            {
                pValuesTemp++;
            }
            else
            {
                strcpy((char *)pValuesTemp, " Lux");
                pValuesTemp += 5;   //strlen((char const *)" Lux") + 1;
            }
        }

        // Mode
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_MODE))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_MODE ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_MODE+STRINGS_TOTAL_LANGUAGES*(unsigned char)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_MODE) ]);
            pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
        }

        // Detection scheme
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_DETECTIONSCHEME))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_DETECTIONSCHEME ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
//            strcpy((char *)pValuesTemp, "Detection");       // Todo
//            pValuesTemp += strlen((char const *)pValuesTemp);
//            strcpy((char *)pValuesTemp, " > ");
//            pValuesTemp += 3;
            strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_DISABLED+STRINGS_TOTAL_LANGUAGES*(unsigned char)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_DETECTIONSCHEME) ]);
            pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
        }

        // Detection scheme initial
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_INITIAL))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_DETECTIONSCHEME ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_INITIAL ]);
            pValuesTemp += strlen((char const *)pValuesTemp);
            strcpy((char *)pValuesTemp, " > ");
            pValuesTemp += 3;
            strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_DISABLED+STRINGS_TOTAL_LANGUAGES*(unsigned char)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_INITIAL) ]);
            pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
        }

        // Detection scheme maintain
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_MAINTAIN))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_DETECTIONSCHEME ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_MAINTAIN ]);
            pValuesTemp += strlen((char const *)pValuesTemp);
            strcpy((char *)pValuesTemp, " > ");
            pValuesTemp += 3;
            strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_DISABLED+STRINGS_TOTAL_LANGUAGES*(unsigned char)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_MAINTAIN) ]);
            pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
        }

        // Detection scheme retrigger
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_RETRIGGER))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_DETECTIONSCHEME ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_RETRIGGER ]);
            pValuesTemp += strlen((char const *)pValuesTemp);
            strcpy((char *)pValuesTemp, " > ");
            pValuesTemp += 3;
            strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_DISABLED+STRINGS_TOTAL_LANGUAGES*(unsigned char)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_RETRIGGER) ]);
            pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
        }

        // Alert
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_ALERT))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_ALERT ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            if (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_ALERT) == (unsigned short)0x2)  //0x2 refers to audible alert
            {
                strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_AUDIBLE ]);
            }
            else
            {
                strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_DISABLED ]);
            }
            pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
        }

        // Standby level
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_STANDBYLEVEL))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_STANDBYLEVEL ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            pValuesTemp += dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_STANDBYLEVEL), 0);
            *pValuesTemp++ = '%';
            *pValuesTemp++ = '\0';
        }

        // Standby delay
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_STANDBYDELAY))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_STANDBYDELAY ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            if (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_STANDBYDELAY) == 0)
            {
                strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_NOSTANDBY ]);
                pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
            }
            else if (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_STANDBYDELAY) == 255)
            {
                strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_UNLIMITED ]);
                pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
            }
            else if (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_STANDBYDELAY) <= 60)
            {
                pValuesTemp += dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_STANDBYDELAY), 0);
                strcpy((char *)pValuesTemp, " Sec");
                pValuesTemp += 5;
            }
            else if (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_STANDBYDELAY) <= 120)
            {
                pValuesTemp += dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_STANDBYDELAY)-60, 0);
                strcpy((char *)pValuesTemp, " Min");
                pValuesTemp += 5;
            }
        }

        // Send condition
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_SENDCONDITION))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_SENDCONDITION ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_CYCLICAL+STRINGS_TOTAL_LANGUAGES*(unsigned char)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_SENDCONDITION) ]);
            pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
        }

        // Dead band
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_DEADBAND))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_DEADBAND ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            pValuesTemp += dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_DEADBAND), 0);
            *pValuesTemp++ = '%';
            *pValuesTemp++ = '\0';
        }

        // Time base
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_TIMEBASE))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_TIMEBASE ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            pValuesTemp += dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_TIMEBASE), 2);
            strcpy((char *)pValuesTemp, " Min");
            pValuesTemp += 5;
        }

        // Bandwidth
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_BANDWIDTH))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_BANDWIDTH ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            pValuesTemp += dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_BANDWIDTH), 0);
            strcpy((char *)pValuesTemp, " Mes/Min");
            pValuesTemp += 9;
        }

        // Summer hour  // 2.4.0
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_SUMMERHOUR))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_SUMMERHOUR ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            if (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_SUMMERHOUR) <= 2)
            {
                strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_NOCHOICE+STRINGS_TOTAL_LANGUAGES*(unsigned char)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_SUMMERHOUR) ]);
            }
            else
            {
                strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_NONE ]);
            }

            pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
        }

        // Start day    // 2.4.0
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_SUMMERHOUR) && (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_SUMMERHOUR) == 0))   // 0 means No choice. Do we need to check parameter availability of Start month and Start day?
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_STARTDAY ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            pValuesTemp += dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_STARTDAY), 2);
            strcpy((char *)pValuesTemp, " / ");
            pValuesTemp += 3;
            pValuesTemp += (dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_STARTMONTH), 2) + 1);
        }

        // End day    // 2.4.0
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_SUMMERHOUR) && (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_SUMMERHOUR) == 0))   // 0 means No choice. Do we need to check parameter availability of End month and End day?
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_ENDDAY ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            pValuesTemp += dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_ENDDAY), 2);
            strcpy((char *)pValuesTemp, " / ");
            pValuesTemp += 3;
            pValuesTemp += (dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_ENDMONTH), 2) + 1);
        }

        // Country  // 2.4.0
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_COUNTRY))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_COUNTRY ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            strcpy((char *)pValuesTemp, (char const *)valuesCountry[PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_COUNTRY)*5 ]);
            pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
        }

        // Longitude    // 2.4.0
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_LONGITUDE))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_LONGITUDE ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            if (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_LONGITUDE) == 0)
            {
                *(pValuesTemp++) = '0';
                *(pValuesTemp++) = '\0';
            }
            else if (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_LONGITUDE) < 0x8000)
            {
                *(pValuesTemp++) = '+';
                pValuesTemp += (dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_LONGITUDE), 0) + 1);
            }
            else
            {
                *(pValuesTemp++) = '-';
                pValuesTemp += (dectoascii(pValuesTemp, (0xFFFF - (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_LONGITUDE) - 1)), 0) + 1);
            }
        }

        // Latitude     // 2.4.0
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_LATITUDE))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_LATITUDE ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            if (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_LATITUDE) == 0)
            {
                *(pValuesTemp++) = '0';
                *(pValuesTemp++) = '\0';
            }
            else if (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_LATITUDE) < 0x8000)
            {
                *(pValuesTemp++) = '+';
                pValuesTemp += (dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_LATITUDE), 0) + 1);
            }
            else
            {
                *(pValuesTemp++) = '-';
                pValuesTemp += (dectoascii(pValuesTemp, (0xFFFF - (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_LATITUDE) - 1)), 0) + 1);
            }
        }

        // Light regulation
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_LIGHTREGULATION))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_LIGHTREGULATION ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            if (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_LIGHTREGULATION))
            {
                strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_ENABLED ]);
            }
            else
            {
                strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_DISABLED ]);
            }
            pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
        }

        // Provision of light
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_PROVISIONOFLIGHT))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_PROVISIONOFLIGHT ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            if (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_PROVISIONOFLIGHT))
            {
                pValuesTemp += dectoascii(pValuesTemp, 5*PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_PROVISIONOFLIGHT), 0);
                 if (PARAMS_SensorIsHFOnly(&sensorParams) )     // 2.2.0
                {
                    pValuesTemp++;
                }
                else
                {
                    strcpy((char *)pValuesTemp, " Lux");
                    pValuesTemp += 5;   //strlen((char const *)" Lux") + 1;
                }
            }
            else    // 2.2.0
            {
                strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_AUTO ]);      // 2.4.0
                pValuesTemp += (strlen((char const *)pValuesTemp) + 1);         // 2.4.0
            }
        }

        // Loop type
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_LOOPTYPE))
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_LOOPTYPE ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_CLOSELOOP+STRINGS_TOTAL_LANGUAGES*(unsigned char)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_LOOPTYPE) ]);
            pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
        }

        // Type
        if (DisplayReadOnlyItems)
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_TYPE ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            if( is_KNX_sensor( &sensorParams ) )
            {
                 strcpy((char *)pValuesTemp, (char const *)KNX_sensor_type_name[(sensorParams.Type.Value & 0x000F)*LANGUAGES_TOTAL ]);
                 pValuesTemp += (strlen((char const *)pValuesTemp) + 1);                
            }
            else
            {
                if (PARAMS_IsLightController(pSensorParams)){    // 2.4.0
                    strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_ALGORITHMIC ]);
                    pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
                }
                else{
                    if (PARAMS_IsLineVoltageSensor(pSensorParams)){
                      strcpy((char *)pValuesTemp, "LV ");
                      pValuesTemp += 3;
                    }
                    else if (PARAMS_IsDigitalSensor(pSensorParams)){
                      if (PARAMS_IsStandaloneSensor(pSensorParams)){
                        strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_DIGITAL ]);
                        pValuesTemp += strlen((char const *)pValuesTemp);
    //                  strcpy((char *)pValuesTemp, "DIGITAL ");
    
      //                  pValuesTemp += 8;
                        
                      }
                      else{    // Centralized sensor
                        strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_CENTRALIZED ]);
                        pValuesTemp += strlen((char const *)pValuesTemp);
                      }
                    }
                    if (PARAMS_IsZigbeeSensor(pSensorParams)){
                      strcpy((char *)pValuesTemp, "Zigbee ");
                      pValuesTemp += 7;
                    }
                    strcpy((char *)pValuesTemp, "> ");
                    pValuesTemp += 2;
                    
                    if (PARAMS_SensorHasPIRandUS(pSensorParams)){
                      strcpy((char *)pValuesTemp, "PIR+US");
                      pValuesTemp += 7;
                    }
                    else if (PARAMS_SensorHasPIRandHF(pSensorParams)){
                      strcpy((char *)pValuesTemp, "PIR+HF");
                      pValuesTemp += 7;
                    }                
                    else if (PARAMS_SensorHasHF(pSensorParams)){
                      strcpy((char *)pValuesTemp, "HF");
                      pValuesTemp += 3;
                    }
                    else if (PARAMS_SensorHasPIR(pSensorParams)){
                      strcpy((char *)pValuesTemp, "PIR");
                      pValuesTemp += 4;
                    }
                    else if (PARAMS_SensorHasUS(pSensorParams)){
                      strcpy((char *)pValuesTemp, "US");
                      pValuesTemp += 3;
                    }
                    else{    // Sensor has no tech, must be daylight sensor
                      strcpy((char *)pValuesTemp, "DL");
                      pValuesTemp += 3;
                    }
                }
            }
        }

        // ID   // 2.3.2, include ID3 and ID4 for Zigbee sensors
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_ID1) && PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_ID2) && DisplayReadOnlyItems)
        {
            (pParametersTemp)->pName = "ID";
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_ID3) && PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_ID4))
            {
                hextoascii(pValuesTemp, (((unsigned long long)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_ID4) << 48) | ((unsigned long long)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_ID3) << 32) | ((unsigned long long)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_ID2) << 16) | (unsigned long long)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_ID1)), 16);
                pValuesTemp += 19;
            }
            else
            {
                hextoascii(pValuesTemp, (((unsigned int)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_ID2) << 16) | (unsigned int)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_ID1)), 8);
                pValuesTemp += 11;
            }
        }

        // DeviceID Zigbee
        if ((PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_DEVICEIDZIGBEE) || PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_DEVICEIDZIGBEE2)) && DisplayReadOnlyItems)    // 2.4.0
        {
//            (pParametersTemp)->pName = "DeviceID Zigbee";   // 2.4.0
            (pParametersTemp)->pName = strings[STRINGS_INDEX_DEVICE_ID ];

            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            // 2.4.0
            if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_DEVICEIDZIGBEE))
            {
                hextoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_DEVICEIDZIGBEE), 4);
                pValuesTemp += 6;
                if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_DEVICEIDZIGBEE2))
                {
                    *(pValuesTemp++) = ' ';
                    *(pValuesTemp++) = ' ';
                }
                else
                {
                    pValuesTemp++;
                }
            }
            if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_DEVICEIDZIGBEE2))
            {
                hextoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_DEVICEIDZIGBEE2), 4);
                pValuesTemp += 7;
            }
        }

/* 2.4.0
        // 2.3.2
        // DeviceID Zigbee 2
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_DEVICEIDZIGBEE2) && DisplayReadOnlyItems)
        {
            (pParametersTemp)->pName = "DeviceID Zigbee 2";
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            hextoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_DEVICEIDZIGBEE2), 4);
            pValuesTemp += 7;
        }
*/
        
    //dd    // " Master / Slave " 
        if ( PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_KNX_MASTER_SLAVE ) )
        {
            (pParametersTemp)->pName = KNX_sensor_master_slave_menu_item_langeage_string[language];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            if (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_KNX_MASTER_SLAVE))
            {
                //strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_ENABLED ]);
                strcpy((char *)pValuesTemp, KNX_sensor_slave_langeage_string[language]);
            }
            else
            {
                //strcpy((char *)pValuesTemp, (char const *)strings[STRINGS_INDEX_DISABLED ]);
                strcpy((char *)pValuesTemp, KNX_sensor_master_langeage_string[language]);
            }
            pValuesTemp += (strlen((char const *)pValuesTemp) + 1);
        }

        // Firmware version
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_FIRMWAREVERSION) && DisplayReadOnlyItems)
        {
            if( is_KNX_sensor( &sensorParams ) )
            {
                (pParametersTemp)->pName = strings[STRINGS_INDEX_FIRMWARE ];            
            }
            else
            {
                if (PARAMS_IsZigbeeSensor(pSensorParams))
                {
    //                (pParametersTemp)->pName = "Controller firmware";
                   (pParametersTemp)->pName = strings[STRINGS_INDEX_CONTROLLER_FIM ];
    
                }
                else
                {
    //                (pParametersTemp)->pName = "Firmware";
                   (pParametersTemp)->pName = strings[STRINGS_INDEX_FIRMWARE ];
    
                }
            }
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            pValuesTemp += bcdtoascii(pValuesTemp, (unsigned char)(PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_FIRMWAREVERSION) >> 8), 0);
            strcpy((char *)pValuesTemp, ".");
            pValuesTemp++;
            pValuesTemp += (bcdtoascii(pValuesTemp, (unsigned char)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_FIRMWAREVERSION), 1) + 1);
        }

        // Referent
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_REFERENT) && DisplayReadOnlyItems)
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_REFERENT ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            pValuesTemp += dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_REFERENT), 0);
            pValuesTemp++;
        }

        // Group
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_MAINGROUP) && DisplayReadOnlyItems)
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_GROUP ];
            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            pValuesTemp += dectoascii(pValuesTemp, PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_MAINGROUP), 0);
            pValuesTemp++;
        }

        // Daughterboard firmware version   // 2.3.2, include DBFIRMWAREVERSION2 (MAJOR.MINOR) and DBFIRMWAREVERSION (.REVISION.FIELDTEST)
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_DBFIRMWAREVERSION) && DisplayReadOnlyItems)
        {
//            (pParametersTemp)->pName = "Dau. board firmware";
            (pParametersTemp)->pName = strings[STRINGS_INDEX_DAUGHTER_BOARD ];

            (pParametersTemp)->pValue = pValuesTemp;
            pParametersTemp->ValueType = 0; // string
            pParametersTemp++;
            if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_DBFIRMWAREVERSION2))
            {
                pValuesTemp += bcdtoascii(pValuesTemp, (unsigned char)(PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_DBFIRMWAREVERSION2) >> 8), 0);
                strcpy((char *)pValuesTemp, ".");
                pValuesTemp++;
                pValuesTemp += bcdtoascii(pValuesTemp, (unsigned char)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_DBFIRMWAREVERSION2), 1);
                strcpy((char *)pValuesTemp, ".");
                pValuesTemp++;
                pValuesTemp += bcdtoascii(pValuesTemp, (unsigned char)(PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_DBFIRMWAREVERSION) >> 8), 1);
                strcpy((char *)pValuesTemp, ".");
                pValuesTemp++;
            }
            else
            {
                pValuesTemp += bcdtoascii(pValuesTemp, (unsigned char)(PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_DBFIRMWAREVERSION) >> 8), 0);
                strcpy((char *)pValuesTemp, ".");
                pValuesTemp++;
            }
            pValuesTemp += (bcdtoascii(pValuesTemp, (unsigned char)PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_DBFIRMWAREVERSION), 1) + 1);
        }

        // Battery information
        if (PARAMS_IsParameterAvailable(pSensorParams, PARAM_INDEX_BATTERYINFORMATION) && ((PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_BATTERYINFORMATION) & 0x08) == 0) && DisplayReadOnlyItems)
        {
            (pParametersTemp)->pName = strings[STRINGS_INDEX_BATTERY ];
            switch (PARAMS_GetParameterValue(pSensorParams, PARAM_INDEX_BATTERYINFORMATION) & 0xF0)
            {
            case 0x00:
                pParametersTemp->pValue = (unsigned char *)&BitmapInfo_Battery[0];
                break;
            case 0x40:
                pParametersTemp->pValue = (unsigned char *)&BitmapInfo_Battery[1];
                break;
            case 0x80:
                pParametersTemp->pValue = (unsigned char *)&BitmapInfo_Battery[2];
                break;
            case 0xA0:
            default:
                pParametersTemp->pValue = (unsigned char *)&BitmapInfo_Battery[3];
                break;
            }
            pParametersTemp->ValueType = 1; // icon
            pParametersTemp++;
        }

        totalItems = pParametersTemp - pReadParameters;

        pageSubUpdate = 1;
    }

    if (pageSubUpdate)
    {
        firstDisplayedItem = 0;

        if (currentItem > 2)
        {
            firstDisplayedItem = currentItem - 2;

            if (currentItem == totalItems - 1)
            {
                firstDisplayedItem = totalItems - 4;
            }
        }

        if (totalItems >= 1)
        {
            LCDD_ClearRect(0, PARAMETERS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PARAMETERS_LINE_1+PARAMETERS_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 0) ? 3:0);
            if ((pReadParameters+firstDisplayedItem)->ValueType == 0)   // string
            {
                // 2.4.0
//                LCDD_PutString(PAGE_LINE_INDENT, PARAMETERS_LINE_1, 0, *((unsigned char **)(pReadParameters+firstDisplayedItem)+displaySelection), (currentItem - firstDisplayedItem == 0) ? 3:0, 0);
                if (LCDD_PutScrollString(PAGE_LINE_INDENT, PARAMETERS_LINE_1, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), *((unsigned char **)(pReadParameters+firstDisplayedItem)+displaySelection)+((currentItem - firstDisplayedItem == 0)?pageScroll:0), (currentItem - firstDisplayedItem == 0)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 0)?1:0))
                {
                    pageScroll++;
                    pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
                }
            }
            else    // icon
            {
                if (displaySelection == 0)
                {
                    // 2.4.0
//                    LCDD_PutString(PAGE_LINE_INDENT, PARAMETERS_LINE_1, 0, *((unsigned char **)(pReadParameters+firstDisplayedItem)), (currentItem - firstDisplayedItem == 0) ? 3:0, 0);
                    if (LCDD_PutScrollString(PAGE_LINE_INDENT, PARAMETERS_LINE_1, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), *((unsigned char **)(pReadParameters+firstDisplayedItem))+((currentItem - firstDisplayedItem == 0)?pageScroll:0), (currentItem - firstDisplayedItem == 0)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 0)?1:0))
                    {
                        pageScroll++;
                        pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
                    }
                }
                else
                {
                    LCDD_PutBitmap(PAGE_LINE_INDENT, PARAMETERS_LINE_1, (const BitmapInfo *)(pReadParameters+firstDisplayedItem)->pValue, (currentItem - firstDisplayedItem == 0) ? 1:0);
                }
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PARAMETERS_LINE_1, (currentItem - firstDisplayedItem == 0) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 2)
        {
            LCDD_ClearRect(0, PARAMETERS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PARAMETERS_LINE_2+PARAMETERS_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 1) ? 3:0);
            if ((pReadParameters+firstDisplayedItem+1)->ValueType == 0)   // string
            {
                // 2.4.0
//                LCDD_PutString(PAGE_LINE_INDENT, PARAMETERS_LINE_2, 0, *((unsigned char **)(pReadParameters+firstDisplayedItem+1)+displaySelection), (currentItem - firstDisplayedItem == 1) ? 3:0, 0);
                if (LCDD_PutScrollString(PAGE_LINE_INDENT, PARAMETERS_LINE_2, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), *((unsigned char **)(pReadParameters+firstDisplayedItem+1)+displaySelection)+((currentItem - firstDisplayedItem == 1)?pageScroll:0), (currentItem - firstDisplayedItem == 1)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 1)?1:0))
                {
                    pageScroll++;
                    pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
                }
            }
            else    // icon
            {
                if (displaySelection == 0)
                {
                    // 2.4.0
//                    LCDD_PutString(PAGE_LINE_INDENT, PARAMETERS_LINE_2, 0, *((unsigned char **)(pReadParameters+firstDisplayedItem+1)), (currentItem - firstDisplayedItem == 1) ? 3:0, 0);
                    if (LCDD_PutScrollString(PAGE_LINE_INDENT, PARAMETERS_LINE_2, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), *((unsigned char **)(pReadParameters+firstDisplayedItem+1))+((currentItem - firstDisplayedItem == 1)?pageScroll:0), (currentItem - firstDisplayedItem == 1)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 1)?1:0))
                    {
                        pageScroll++;
                        pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
                    }
                }
                else
                {
                    LCDD_PutBitmap(PAGE_LINE_INDENT, PARAMETERS_LINE_2, (const BitmapInfo *)(pReadParameters+firstDisplayedItem+1)->pValue, (currentItem - firstDisplayedItem == 1) ? 1:0);
                }
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PARAMETERS_LINE_2, (currentItem - firstDisplayedItem == 1) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 3)
        {
            LCDD_ClearRect(0, PARAMETERS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PARAMETERS_LINE_3+PARAMETERS_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 2) ? 3:0);
            if ((pReadParameters+firstDisplayedItem+2)->ValueType == 0)   // string
            {
                // 2.4.0
//                LCDD_PutString(PAGE_LINE_INDENT, PARAMETERS_LINE_3, 0, *((unsigned char **)(pReadParameters+firstDisplayedItem+2)+displaySelection), (currentItem - firstDisplayedItem == 2) ? 3:0, 0);
                if (LCDD_PutScrollString(PAGE_LINE_INDENT, PARAMETERS_LINE_3, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), *((unsigned char **)(pReadParameters+firstDisplayedItem+2)+displaySelection)+((currentItem - firstDisplayedItem == 2)?pageScroll:0), (currentItem - firstDisplayedItem == 2)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 2)?1:0))
                {
                    pageScroll++;
                    pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
                }
            }
            else    // icon
            {
                if (displaySelection == 0)
                {
                    // 2.4.0
//                    LCDD_PutString(PAGE_LINE_INDENT, PARAMETERS_LINE_3, 0, *((unsigned char **)(pReadParameters+firstDisplayedItem+2)), (currentItem - firstDisplayedItem == 2) ? 3:0, 0);
                    if (LCDD_PutScrollString(PAGE_LINE_INDENT, PARAMETERS_LINE_3, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), *((unsigned char **)(pReadParameters+firstDisplayedItem+2))+((currentItem - firstDisplayedItem == 2)?pageScroll:0), (currentItem - firstDisplayedItem == 2)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 2)?1:0))
                    {
                        pageScroll++;
                        pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
                    }
                }
                else
                {
                    LCDD_PutBitmap(PAGE_LINE_INDENT, PARAMETERS_LINE_3, (const BitmapInfo *)(pReadParameters+firstDisplayedItem+2)->pValue, (currentItem - firstDisplayedItem == 2) ? 1:0);
                }
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PARAMETERS_LINE_3, (currentItem - firstDisplayedItem == 2) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 4)
        {
            LCDD_ClearRect(0, PARAMETERS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PARAMETERS_LINE_4+PARAMETERS_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 3) ? 3:0);
            if ((pReadParameters+firstDisplayedItem+3)->ValueType == 0)   // string
            {
                // 2.4.0
//                LCDD_PutString(PAGE_LINE_INDENT, PARAMETERS_LINE_4, 0, *((unsigned char **)(pReadParameters+firstDisplayedItem+3)+displaySelection), (currentItem - firstDisplayedItem == 3) ? 3:0, 0);
                if (LCDD_PutScrollString(PAGE_LINE_INDENT, PARAMETERS_LINE_4, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), *((unsigned char **)(pReadParameters+firstDisplayedItem+3)+displaySelection)+((currentItem - firstDisplayedItem == 3)?pageScroll:0), (currentItem - firstDisplayedItem == 3)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 3)?1:0))
                {
                    pageScroll++;
                    pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
                }
            }
            else    // icon
            {
                if (displaySelection == 0)
                {
                    // 2.4.0
//                    LCDD_PutString(PAGE_LINE_INDENT, PARAMETERS_LINE_4, 0, *((unsigned char **)(pReadParameters+firstDisplayedItem+3)), (currentItem - firstDisplayedItem == 3) ? 3:0, 0);
                    if (LCDD_PutScrollString(PAGE_LINE_INDENT, PARAMETERS_LINE_4, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), *((unsigned char **)(pReadParameters+firstDisplayedItem+3))+((currentItem - firstDisplayedItem == 3)?pageScroll:0), (currentItem - firstDisplayedItem == 3)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 3)?1:0))
                    {
                        pageScroll++;
                        pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
                    }
                }
                else
                {
                    LCDD_PutBitmap(PAGE_LINE_INDENT, PARAMETERS_LINE_4, (const BitmapInfo *)(pReadParameters+firstDisplayedItem+3)->pValue, (currentItem - firstDisplayedItem == 3) ? 1:0);
                }
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PARAMETERS_LINE_4, (currentItem - firstDisplayedItem == 3) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        pageSubUpdate = 0;
    }

    if (BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = totalItems-1;
            }
            else
            {
                currentItem--;
            }

//            BUTTON_Acknowledge();
            pageScroll = 0;  // 2.4.0
            pageSubUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == totalItems-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

//            BUTTON_Acknowledge();
            pageScroll = 0;  // 2.4.0
            pageSubUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            displaySelection ^= 1;

//            BUTTON_Acknowledge();
            pageScroll = 0;  // 2.4.0
            pageSubUpdate = 1;
        }
    }

    // 2.4.0
    if (TIME_GetTime() >= pageTimeStamp)
    {
        pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
        pageSubUpdate = 1;
    }

    if (Swap)
    {
        if (pReadParameters)
        {
            free(pReadParameters);
        }

        if (pValueStrings)
        {
            free(pValueStrings);
        }
    }
}

/*
void _list_files(PAGE_FILES *pFiles, FILES_FAT *pFat)
{
    for (unsigned char i=1; i<=FILES_GetFileNumber(pFat); i++)
    {
        FILES_ReadFileName(&(pFiles[i-1].pFileName[0]), pFat, i);
        pFiles++;
    }
}
*/

void _draw_rectangular_for_value(void)
{
#define PAGE_VALUE_Y_POS                46
#define PAGE_RECTANGULAR_LENGTH         108
#define PAGE_RECTANGULAR_HEIGHT         20
#define PAGE_RECTANGULAR_X_POS          ((LCD_X_LENGTH-PAGE_RECTANGULAR_LENGTH)/2)
#define PAGE_RECTANGULAR_Y_POS          44

    unsigned char i,j;

    for (j=PAGE_RECTANGULAR_Y_POS/4; j<(PAGE_RECTANGULAR_Y_POS+PAGE_RECTANGULAR_HEIGHT)/4; j++)
    {
        for (i=PAGE_RECTANGULAR_X_POS; i<PAGE_RECTANGULAR_X_POS+PAGE_RECTANGULAR_LENGTH; i++)
        {
            lcdd.LcddFrameBuffer[j][i] = Bitmap_Rectangular[(j-PAGE_RECTANGULAR_Y_POS/4)*PAGE_RECTANGULAR_LENGTH+i-PAGE_RECTANGULAR_X_POS];
        }
    }
}
#else

void _draw_Digit(unsigned char x, unsigned char y, unsigned char digit)
{
  unsigned char c = 0;
    LCDD_PutBitmap(x, y, &BitmapInfo_Digit[Info_Digits[digit][0]], c);
    LCDD_PutBitmap(x+8, y, &BitmapInfo_Digit[Info_Digits[digit][1]], c);
    LCDD_PutBitmap(x+16, y, &BitmapInfo_Digit[Info_Digits[digit][2]], c);
    LCDD_PutBitmap(x+16, y+8, &BitmapInfo_Digit[Info_Digits[digit][3]], c);
    LCDD_PutBitmap(x+16, y+16, &BitmapInfo_Digit[Info_Digits[digit][4]], c);
    LCDD_PutBitmap(x+16, y+24, &BitmapInfo_Digit[Info_Digits[digit][5]], c);
    LCDD_PutBitmap(x+16, y+32, &BitmapInfo_Digit[Info_Digits[digit][6]], c);
    LCDD_PutBitmap(x+8, y+32, &BitmapInfo_Digit[Info_Digits[digit][7]], c);
    LCDD_PutBitmap(x, y+32, &BitmapInfo_Digit[Info_Digits[digit][8]], c);
    LCDD_PutBitmap(x, y+24, &BitmapInfo_Digit[Info_Digits[digit][9]], c);
    LCDD_PutBitmap(x, y+16, &BitmapInfo_Digit[Info_Digits[digit][10]], c);
    LCDD_PutBitmap(x+8, y+16, &BitmapInfo_Digit[Info_Digits[digit][11]], c);
    LCDD_PutBitmap(x, y+8, &BitmapInfo_Digit[Info_Digits[digit][12]], c);
}

#endif  // #ifndef APP_BAES

//------------------------------------------------------------------------------
///        Exported Functions
//------------------------------------------------------------------------------

#ifndef APP_BAES
void MainPageHandler(void)
{
#define PAGE_MAIN_MAXIMUMITEMS              6
#define PAGE_MAIN_BATTERY_BODY_LENGTH       19
#define PAGE_MAIN_BATTERY_BODY_HEIGHT       10
#define PAGE_MAIN_BATTERY_BODY_X_START      (LCD_X_LENGTH-2-PAGE_MAIN_BATTERY_BODY_LENGTH)
#define PAGE_MAIN_BATTERY_BODY_Y_START      2
#define PAGE_MAIN_BATTERY_NIPPLE_LENGTH     2
#define PAGE_MAIN_BATTERY_NIPPLE_HEIGHT     6
#define PAGE_MAIN_BATTERY_NIPPLE_X_START    (PAGE_MAIN_BATTERY_BODY_X_START-PAGE_MAIN_BATTERY_NIPPLE_LENGTH)
#define PAGE_MAIN_BATTERY_NIPPLE_Y_START    4
#define PAGE_MAIN_BATTERY_FUEL_X_START      (PAGE_MAIN_BATTERY_BODY_X_START+2)
#define PAGE_MAIN_BATTERY_FUEL_Y_START      (PAGE_MAIN_BATTERY_BODY_Y_START+2)
#define PAGE_MAIN_BATTERY_FUEL_LENGTH       15
#define PAGE_MAIN_BATTERY_FUEL_HEIGHT       6
#define PAGE_MAIN_LINE_1                    16
#define PAGE_MAIN_LINE_HEIGHT               16
#define PAGE_MAIN_LINE_SPACE                6
#define PAGE_MAIN_LINE_2                    (PAGE_MAIN_LINE_1 + PAGE_MAIN_LINE_HEIGHT + PAGE_MAIN_LINE_SPACE)
#define PAGE_MAIN_LINE_3                    (PAGE_MAIN_LINE_2 + PAGE_MAIN_LINE_HEIGHT + PAGE_MAIN_LINE_SPACE)
#define PAGE_MAIN_LINE_4                    (PAGE_MAIN_LINE_3 + PAGE_MAIN_LINE_HEIGHT + PAGE_MAIN_LINE_SPACE)

    static PAGE_MENU_ITEM *pMenuItems;
    static unsigned char currentItem = 0;
    static unsigned char totalItems;
    static unsigned char pageMainBattery = 0;
    static PAGE_PARAMETER *pParameter;      // 2.4.0
    unsigned char firstDisplayedItem;

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[1].pItem = (void *)&BitmapInfo_ReadSensor;
        pageFunckeyItems[2].pItem = (void *)&BitmapInfo_ReadType;       // 2.4.0
        pageFunckeyItems[3].pItem = (void *)&BitmapInfo_QuestionMark;
        _common_funckey_initializer();

        // 2.4.0
        pParameter = (PAGE_PARAMETER *)malloc(2*sizeof(PAGE_PARAMETER));
        pParameter->ParamIndex = PARAM_INDEX_TYPE2;
        (pParameter+1)->ParamIndex = PARAM_INDEX_TYPE;      // Intend to put Type to make sure at least one parameter is read.

        pMenuItems = (PAGE_MENU_ITEM *)malloc(PAGE_MAIN_MAXIMUMITEMS*sizeof(PAGE_MENU_ITEM));

        totalItems = 0;

        // 2.4.0
        if (PARAMS_IsLightController(&sensorParams))
        {
            pMenuItems->pString = strings[STRINGS_INDEX_BASICPARAMETERS ];
            pMenuItems->PageHandlerIndex = PAGE_INDEX_PARAMETERS;
            totalItems++;
        }
        else
        {
            if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_HOUR) || \
                PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MINUTE) || \
                PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_SECOND) || \
                PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_HF_SENS) || \
                PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_PIR_SENS) || \
                PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_US_SENS) || \
                PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DAYLIGHTSETPOINT) || \
                PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DETECTIONSCHEME) || \
                PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_INITIAL) || \
                PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MAINTAIN) || \
                PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_RETRIGGER) || \
                PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MODE) || \
                PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_ALERT) || \
                PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_STANDBYLEVEL) || \
                PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_STANDBYDELAY) || \
                PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_SENDCONDITION) || \
                PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DEADBAND) || \
                PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_TIMEBASE) || \
                  PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DetectionLed) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_BANDWIDTH)
                )
            {
                pMenuItems->pString = strings[STRINGS_INDEX_PARAMETERS ];
                pMenuItems->PageHandlerIndex = PAGE_INDEX_PARAMETERS;
                totalItems++;   // 2.4.0, was totalItems = 1;
            }
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DAYLIGHTFACTOR) || \
            PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DAYLIGHTFACTORCEN) || \
            PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_NATURALLIGHTFACTOR) || \
            PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_LIGHTREGULATION) || \
            PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_PROVISIONOFLIGHT) || \
            PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_LOOPTYPE) || \
            PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_KNX_MASTER_SLAVE )
            )
        {
            (pMenuItems+totalItems)->pString = strings[STRINGS_INDEX_ADVANCEDMODE ];
            (pMenuItems+totalItems)->PageHandlerIndex = PAGE_INDEX_ADVANCEDMODE;
            totalItems++;
        }
        (pMenuItems+totalItems)->pString = strings[STRINGS_INDEX_FILES ];
        (pMenuItems+totalItems)->PageHandlerIndex = PAGE_INDEX_FILES;
        totalItems++;
        // 2.3.0

        
         if (PARAMS_IsDALI_MASTER_DL_PIR_US(&sensorParams) || PARAMS_IsDALI_MASTER_DL_OP_PIR_US(&sensorParams) || PARAMS_IsDALI_MASTER_OP_PIR_US(&sensorParams) || \
             PARAMS_IsDALI_MASTER_DL_OP_PIR(&sensorParams) || \
             PARAMS_IsDALI_MASTER_OP_PIR(&sensorParams) || PARAMS_IsDALI_MASTER_PIR_DL(&sensorParams))  //2.5.7
        {
            (pMenuItems+totalItems)->pString ="DALI";
            (pMenuItems+totalItems)->PageHandlerIndex = PAGE_INDEX_DALI_MASTER;//PAGE_INDEX_DALI_MASTER
            totalItems++;
        }
         if (PARAMS_IsDALI_SLAVE_OP_PIR_US(&sensorParams) || PARAMS_IsDALI_SLAVE_OP_PIR(&sensorParams))  //2.5.7
        {
            (pMenuItems+totalItems)->pString ="DALI";
            (pMenuItems+totalItems)->PageHandlerIndex = PAGE_INDEX_DALI_SLAVE;//PAGE_INDEX_DALI_SLAVE
            totalItems++;
        }
        
        if (PARAMS_IsDALI_NOP(&sensorParams))  //2.5.7
        {
          (pMenuItems+totalItems)->pString ="DALI";
          (pMenuItems+totalItems)->PageHandlerIndex = PAGE_CHANGE_TYPE;//PAGE_INDEX_DALI_NOP
          totalItems++;
        }
        if ((!PARAMS_IsZigbeeSensor(&sensorParams)) && \
          (!PARAMS_IsLightController(&sensorParams)))     // 2.4.0
        {
          (pMenuItems+totalItems)->pString = strings[STRINGS_INDEX_PNL ];
          (pMenuItems+totalItems)->PageHandlerIndex = PAGE_INDEX_PNLENTRY;
          totalItems++;
        }

        (pMenuItems+totalItems)->pString = strings[STRINGS_INDEX_TEST ];        // 2.4.0
        (pMenuItems+totalItems)->PageHandlerIndex = PAGE_INDEX_TEST;
        totalItems++;
        (pMenuItems+totalItems)->pString = strings[STRINGS_INDEX_SETTINGS ];
        (pMenuItems+totalItems)->PageHandlerIndex = PAGE_INDEX_SETTINGS;
        totalItems++;

        LCDD_ClearRect(PAGE_MAIN_BATTERY_NIPPLE_X_START, PAGE_MAIN_BATTERY_NIPPLE_Y_START, PAGE_MAIN_BATTERY_NIPPLE_X_START+PAGE_MAIN_BATTERY_NIPPLE_LENGTH-1, PAGE_MAIN_BATTERY_NIPPLE_Y_START+PAGE_MAIN_BATTERY_NIPPLE_HEIGHT-1, 3);
        LCDD_ClearRect(PAGE_MAIN_BATTERY_BODY_X_START, PAGE_MAIN_BATTERY_BODY_Y_START, PAGE_MAIN_BATTERY_BODY_X_START+PAGE_MAIN_BATTERY_BODY_LENGTH-1, PAGE_MAIN_BATTERY_BODY_Y_START+PAGE_MAIN_BATTERY_BODY_HEIGHT-1, 3);

        // 2.3.0
        if (currentItem > totalItems)
        {
            currentItem = totalItems;
        }

        pageScroll = 0; // 2.4.0

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        firstDisplayedItem = 1;

        if (currentItem > 3)
        {
            firstDisplayedItem = currentItem - 2;

            if (currentItem == totalItems)
            {
                firstDisplayedItem = totalItems - 3;
            }
        }

        if (totalItems >= 1)
        {
            LCDD_ClearRect(0, PAGE_MAIN_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_MAIN_LINE_1+PAGE_MAIN_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 0) ? PAGE_HIGHLIGHT_COLOR:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_MAIN_LINE_1, 0, (pMenuItems+firstDisplayedItem-1)->pString, (currentItem - firstDisplayedItem == 0) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_MAIN_LINE_1, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), (pMenuItems+firstDisplayedItem-1)->pString+((currentItem - firstDisplayedItem == 0)?pageScroll:0), (currentItem - firstDisplayedItem == 0)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 0)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_MAIN_LINE_1, (currentItem - firstDisplayedItem == 0) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 2)
        {
            LCDD_ClearRect(0, PAGE_MAIN_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_MAIN_LINE_2+PAGE_MAIN_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 1) ? PAGE_HIGHLIGHT_COLOR:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_MAIN_LINE_2, 0, (pMenuItems+firstDisplayedItem)->pString, (currentItem - firstDisplayedItem == 1) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_MAIN_LINE_2, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), (pMenuItems+firstDisplayedItem)->pString+((currentItem - firstDisplayedItem == 1)?pageScroll:0), (currentItem - firstDisplayedItem == 1)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 1)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_MAIN_LINE_2, (currentItem - firstDisplayedItem == 1) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 3)
        {
            LCDD_ClearRect(0, PAGE_MAIN_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_MAIN_LINE_3+PAGE_MAIN_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 2) ? PAGE_HIGHLIGHT_COLOR:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_MAIN_LINE_3, 0, (pMenuItems+firstDisplayedItem+1)->pString, (currentItem - firstDisplayedItem == 2) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_MAIN_LINE_3, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), (pMenuItems+firstDisplayedItem+1)->pString+((currentItem - firstDisplayedItem == 2)?pageScroll:0), (currentItem - firstDisplayedItem == 2)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 2)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_MAIN_LINE_3, (currentItem - firstDisplayedItem == 2) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 4)
        {
            LCDD_ClearRect(0, PAGE_MAIN_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_MAIN_LINE_4+PAGE_MAIN_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 3) ? PAGE_HIGHLIGHT_COLOR:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_MAIN_LINE_4, 0, (pMenuItems+firstDisplayedItem+2)->pString, (currentItem - firstDisplayedItem == 3) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_MAIN_LINE_4, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), (pMenuItems+firstDisplayedItem+2)->pString+((currentItem - firstDisplayedItem == 3)?pageScroll:0), (currentItem - firstDisplayedItem == 3)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 3)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_MAIN_LINE_4, (currentItem - firstDisplayedItem == 3) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        LCDD_ClearRect(PAGE_MAIN_BATTERY_FUEL_X_START-1, PAGE_MAIN_BATTERY_FUEL_Y_START-1, PAGE_MAIN_BATTERY_FUEL_X_START+PAGE_MAIN_BATTERY_FUEL_LENGTH, PAGE_MAIN_BATTERY_FUEL_Y_START+PAGE_MAIN_BATTERY_FUEL_HEIGHT, 0);
        if (pageMainBattery)
        {
            LCDD_ClearRect(PAGE_MAIN_BATTERY_FUEL_X_START+PAGE_MAIN_BATTERY_FUEL_LENGTH-pageMainBattery, PAGE_MAIN_BATTERY_FUEL_Y_START, PAGE_MAIN_BATTERY_FUEL_X_START+PAGE_MAIN_BATTERY_FUEL_LENGTH-1, PAGE_MAIN_BATTERY_FUEL_Y_START+PAGE_MAIN_BATTERY_FUEL_HEIGHT-1, 2);
        }
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if ((currentItem == 0) || (currentItem == 1))
            {
                currentItem = totalItems;
            }
            else
            {
                currentItem--;
            }

            pageScroll = 0; // 2.4.0
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == totalItems)
            {
                currentItem = 1;
            }
            else
            {
                currentItem++;
            }

            pageScroll = 0; // 2.4.0
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            if (currentItem)
            {
                pageCurrent = (pMenuItems+currentItem-1)->PageHandlerIndex;
                
                if(pageCurrent==PAGE_INDEX_DALI_SLAVE)
                {
                  GroupNum.Index=0;
                  for(int i=0;i<32;i++)
                    GroupNum.GroupTmp[i]=0;
                }

                pageSwap = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F2)
        {
            pageCurrent = PAGE_INDEX_READ_PARAMETERS;

            pageSwap = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F3) || pageForceRead)    // 2.4.0
        {
            if (PARAMS_IsLineVoltageSensor(&sensorParams) && (!PARAMS_IsZigbeeSensor(&sensorParams)) && (!PARAMS_SensorHasHF(&sensorParams)))
            {
                PARAMS_SetParameterNumber(&sensorParams, PARAM_INDEX_TYPE2, PARAM_NUMBER_TYPE2);
            }
            else if( is_KNX_sensor( &sensorParams ) )
            {
                PARAMS_SetParameterNumber(&sensorParams, PARAM_INDEX_TYPE2, PARAM_NUMBER_TYPE2);
            }

            _page_sub_read_parameters(pParameter, 2);
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F4)
        {
            pageCurrent = PAGE_INDEX_HELP;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    if (pageMainBattery != BATTERY_GetCurrentLevel())
    {
        pageMainBattery = BATTERY_GetCurrentLevel();
        pageUpdate = 1;
    }

    // 2.4.0
    if (TIME_GetTime() >= pageTimeStamp)
    {
        pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
        pageUpdate = 1;
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pMenuItems)
        {
            free(pMenuItems);
        }

        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}
#else
void MainPageHandler(void)
{
#define PAGE_MAIN_TOTALITEMS                4     // 6
#define PAGE_MAIN_BATTERY_BODY_LENGTH       19
#define PAGE_MAIN_BATTERY_BODY_HEIGHT       10
#define PAGE_MAIN_BATTERY_BODY_X_START      (LCD_X_LENGTH-2-PAGE_MAIN_BATTERY_BODY_LENGTH)
#define PAGE_MAIN_BATTERY_BODY_Y_START      2
#define PAGE_MAIN_BATTERY_NIPPLE_LENGTH     2
#define PAGE_MAIN_BATTERY_NIPPLE_HEIGHT     6
#define PAGE_MAIN_BATTERY_NIPPLE_X_START    (PAGE_MAIN_BATTERY_BODY_X_START-PAGE_MAIN_BATTERY_NIPPLE_LENGTH)
#define PAGE_MAIN_BATTERY_NIPPLE_Y_START    4
#define PAGE_MAIN_BATTERY_FUEL_X_START      (PAGE_MAIN_BATTERY_BODY_X_START+2)
#define PAGE_MAIN_BATTERY_FUEL_Y_START      (PAGE_MAIN_BATTERY_BODY_Y_START+2)
#define PAGE_MAIN_BATTERY_FUEL_LENGTH       15
#define PAGE_MAIN_BATTERY_FUEL_HEIGHT       6
#define PAGE_MAIN_LINE_1                    22
#define PAGE_MAIN_LINE_HEIGHT               16
#define PAGE_MAIN_LINE_SPACE                4
#define PAGE_MAIN_LINE_2                    (PAGE_MAIN_LINE_1 + PAGE_MAIN_LINE_HEIGHT + PAGE_MAIN_LINE_SPACE)
#define PAGE_MAIN_LINE_3                    (PAGE_MAIN_LINE_2 + PAGE_MAIN_LINE_HEIGHT + PAGE_MAIN_LINE_SPACE)
#define PAGE_MAIN_LINE_4                    (PAGE_MAIN_LINE_3 + PAGE_MAIN_LINE_HEIGHT + PAGE_MAIN_LINE_SPACE)
#define NB_REPEAT_BMP     2

    static PAGE_MENU_ITEM *pMenuItems;
    static unsigned char currentItem = 0;
    static unsigned char pageMainBattery = 0;
    unsigned char firstDisplayedItem;

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);
	    LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "MENU"), 0, 0, "MENU", 0, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[3].pItem = (void *)&BitmapInfo_tool;
        _common_funckey_initializer();

        pMenuItems = (PAGE_MENU_ITEM *)malloc(PAGE_MAIN_TOTALITEMS*sizeof(PAGE_MENU_ITEM));

        pMenuItems->pString = strings[STRINGS_INDEX_ADRESSER];
        pMenuItems->PageHandlerIndex = PAGE_INDEX_ADRESSER;
        (pMenuItems+1)->pString = strings[STRINGS_INDEX_TESTER];
        (pMenuItems+1)->PageHandlerIndex = PAGE_INDEX_TESTER;
        (pMenuItems+2)->pString = strings[STRINGS_INDEX_TESTADRESSAGE];
        (pMenuItems+2)->PageHandlerIndex = PAGE_INDEX_MAIN;
        (pMenuItems+3)->pString = strings[STRINGS_INDEX_EFFACER_ADRESSE];
        (pMenuItems+3)->PageHandlerIndex = PAGE_INDEX_MAIN;

        LCDD_ClearRect(PAGE_MAIN_BATTERY_NIPPLE_X_START, PAGE_MAIN_BATTERY_NIPPLE_Y_START, PAGE_MAIN_BATTERY_NIPPLE_X_START+PAGE_MAIN_BATTERY_NIPPLE_LENGTH-1, PAGE_MAIN_BATTERY_NIPPLE_Y_START+PAGE_MAIN_BATTERY_NIPPLE_HEIGHT-1, 3);
        LCDD_ClearRect(PAGE_MAIN_BATTERY_BODY_X_START, PAGE_MAIN_BATTERY_BODY_Y_START, PAGE_MAIN_BATTERY_BODY_X_START+PAGE_MAIN_BATTERY_BODY_LENGTH-1, PAGE_MAIN_BATTERY_BODY_Y_START+PAGE_MAIN_BATTERY_BODY_HEIGHT-1, 3);

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        firstDisplayedItem = 1;

        if (currentItem > 4)
        {
            firstDisplayedItem = currentItem - 2;

            if (currentItem == PAGE_MAIN_TOTALITEMS)
            {
                firstDisplayedItem = PAGE_MAIN_TOTALITEMS - 4;
            }
        }

       if (currentItem >=3 )  pageFunckeyItems[1].pItem = (void *)&BitmapInfo_SendData;
                         else pageFunckeyItems[1].pItem = NULL;
            _common_funckey_initializer();

        LCDD_ClearRect(0, PAGE_MAIN_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_MAIN_LINE_1+PAGE_MAIN_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 0) ? PAGE_HIGHLIGHT_COLOR:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_MAIN_LINE_1, 0, (pMenuItems+firstDisplayedItem-1)->pString, (currentItem - firstDisplayedItem == 0) ? PAGE_HIGHLIGHT_COLOR:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_MAIN_LINE_1, (currentItem - firstDisplayedItem == 0) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_MAIN_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_MAIN_LINE_2+PAGE_MAIN_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 1) ? PAGE_HIGHLIGHT_COLOR:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_MAIN_LINE_2, 0, (pMenuItems+firstDisplayedItem)->pString, (currentItem - firstDisplayedItem == 1) ? PAGE_HIGHLIGHT_COLOR:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_MAIN_LINE_2, (currentItem - firstDisplayedItem == 1) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_MAIN_LINE_3, LCD_X_LENGTH-1, PAGE_MAIN_LINE_3+PAGE_MAIN_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 2) ? PAGE_HIGHLIGHT_COLOR:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_MAIN_LINE_3, 0, (pMenuItems+firstDisplayedItem+1)->pString, (currentItem - firstDisplayedItem == 2) ? PAGE_HIGHLIGHT_COLOR:0, 0);

        LCDD_ClearRect(0, PAGE_MAIN_LINE_4, LCD_X_LENGTH-1, PAGE_MAIN_LINE_4+PAGE_MAIN_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 3) ? PAGE_HIGHLIGHT_COLOR:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_MAIN_LINE_4, 0, (pMenuItems+firstDisplayedItem+2)->pString, (currentItem - firstDisplayedItem == 3) ? PAGE_HIGHLIGHT_COLOR:0, 0);

        LCDD_ClearRect(PAGE_MAIN_BATTERY_FUEL_X_START-1, PAGE_MAIN_BATTERY_FUEL_Y_START-1, PAGE_MAIN_BATTERY_FUEL_X_START+PAGE_MAIN_BATTERY_FUEL_LENGTH, PAGE_MAIN_BATTERY_FUEL_Y_START+PAGE_MAIN_BATTERY_FUEL_HEIGHT, 0);
        if (pageMainBattery)
        {
            LCDD_ClearRect(PAGE_MAIN_BATTERY_FUEL_X_START+PAGE_MAIN_BATTERY_FUEL_LENGTH-pageMainBattery, PAGE_MAIN_BATTERY_FUEL_Y_START, PAGE_MAIN_BATTERY_FUEL_X_START+PAGE_MAIN_BATTERY_FUEL_LENGTH-1, PAGE_MAIN_BATTERY_FUEL_Y_START+PAGE_MAIN_BATTERY_FUEL_HEIGHT-1, 2);
        }
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
         if ((currentItem == 0) || (currentItem == 1))
            {
                currentItem = PAGE_MAIN_TOTALITEMS;
            }
            else
            {
                currentItem--;
            }
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
         if (currentItem == PAGE_MAIN_TOTALITEMS)
            {
                currentItem = 1;
            }
            else
            {
                currentItem++;
            }
            pageUpdate = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)||(BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR))
        {
         if (currentItem)
            {
                pageCurrent = (pMenuItems+currentItem-1)->PageHandlerIndex;
                pageSwap = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F2)
        {
         if (currentItem == 3)
            {
            IR_BAES(0x3ff,0x05);       // Test adressage
            }

         if (currentItem == 4)
            {
            IR_BAES(0x3ff,0x02);      // Effacement
            }

         if (currentItem >=3 )
            {
            for(int cpt=0;cpt<NB_REPEAT_BMP;cpt++)
              {
              LCDD_ClearRect(0, 16, LCD_X_LENGTH-1, LCD_Y_LENGTH-25, 0);
              for(int cpt2=0;cpt2<3;cpt2++)
                {
                LCDD_PutBitmap(((LCD_X_LENGTH-1)/2)-25, (LCD_Y_LENGTH-1)/2, &BitmapInfo_SendData_big[cpt2], 0);
                LCDD_Refresh();
                TIME_Wait(200);
                }
              }
            LCDD_ClearRect(0, 16, LCD_X_LENGTH-1, LCD_Y_LENGTH-25, 0);
            }
         pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F4)
        {
          pageCurrent = PAGE_INDEX_SETTINGS;
          pageSwap = 1;
        }
      //        BUTTON_Acknowledge();
    }

    if (pageMainBattery != BATTERY_GetCurrentLevel())
      {
        pageMainBattery = BATTERY_GetCurrentLevel();
          pageUpdate = 1;
      }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pMenuItems)
        {
            free(pMenuItems);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}
#endif  // #ifndef APP_BAES

#ifndef APP_BAES
void ReadParametersPageHandler(void)
{
#define PAGE_READPARAMS_RECT_X_START    8
#define PAGE_READPARAMS_RECT_Y_START    20
#define PAGE_READPARAMS_RECT_X_END      (LCD_X_LENGTH - 8 )
#define PAGE_READPARAMS_BODY_INDENT     4
#define PAGE_READPARAMS_BODY_X_START    (PAGE_READPARAMS_RECT_X_START+PAGE_READPARAMS_BODY_INDENT)
#define PAGE_READPARAMS_BODY_X_LENGTH   (PAGE_READPARAMS_RECT_X_END-PAGE_READPARAMS_BODY_INDENT-PAGE_READPARAMS_BODY_X_START)
#define PAGE_READPARAMS_BODY_Y_START    (PAGE_READPARAMS_RECT_Y_START+PAGE_READPARAMS_BODY_INDENT)
#define PAGE_READPARAMS_PGBAR_Y_SPAN    4
#define PAGE_READPARAMS_PGBAR_HEIGHT    12
#define PAGE_READPARAMS_PGBAR_LENGTH    (PAGE_READPARAMS_RECT_X_END-PAGE_READPARAMS_RECT_X_START-3)
#define PAGE_READPARAMS_PGBAR_STEP      ( 2 ) // a step of the progressbar of reading all parameter; only for displaying 

    static unsigned char currentParameter;
    static unsigned char xOffset;
    static unsigned char yOffset;
    static unsigned char retry;
    static unsigned char const *string;
    static unsigned char result;
    //static unsigned char *tempstring;
    unsigned char error;
    unsigned int para_amount_in_array = 0;

    if (pageInitialize)
    {
        pageInitialize = 0;

        currentParameter = PARAM_INDEX_TYPE;

        xOffset = PAGE_READPARAMS_RECT_X_START+2;
        yOffset = PAGE_READPARAMS_BODY_Y_START+LCDD_PutString(PAGE_READPARAMS_BODY_X_START, PAGE_READPARAMS_BODY_Y_START, PAGE_READPARAMS_BODY_X_LENGTH, strings[STRINGS_INDEX_WAITREADING ], 3, 1)+PAGE_READPARAMS_BODY_INDENT;
        LCDD_ClearRect(PAGE_READPARAMS_RECT_X_START, PAGE_READPARAMS_RECT_Y_START, PAGE_READPARAMS_RECT_X_END, yOffset, 3);
        LCDD_PutString(PAGE_READPARAMS_BODY_X_START, PAGE_READPARAMS_BODY_Y_START, PAGE_READPARAMS_BODY_X_LENGTH, strings[STRINGS_INDEX_WAITREADING ], 3, 0);
        yOffset += PAGE_READPARAMS_PGBAR_Y_SPAN;

        retry = 0;

        string = NULL;

        result = 1;     // Initial to good result

        //tempstring = (unsigned char *)malloc(sizeof(unsigned char) * 10);

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        error = 0;
        
        para_amount_in_array = get_parameters_amount( &sensorParams );

        if (currentParameter == PARAM_INDEX_TYPE)
        {
            if (result = IR_ReadSensorParameter(&sensorParams, PARAM_INDEX_TYPE))
            {
                retry = 0;
                switch (result)
                {
                case 1:     // Reading successfully
                    PARAMS_ResetToDefault(&sensorParams, PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_TYPE));
                    currentParameter = 0;
                    string = NULL;
                    xOffset += PAGE_READPARAMS_PGBAR_STEP;
                    if (xOffset > PAGE_READPARAMS_RECT_X_END-2)
                    {
                        xOffset = PAGE_READPARAMS_RECT_X_END-2;
                    }
                    LCDD_ClearRect(PAGE_READPARAMS_RECT_X_START, yOffset, PAGE_READPARAMS_RECT_X_END, yOffset+PAGE_READPARAMS_PGBAR_HEIGHT-1, 3);
                    LCDD_ClearRect(PAGE_READPARAMS_RECT_X_START+1, yOffset+1, PAGE_READPARAMS_RECT_X_END-1, yOffset+PAGE_READPARAMS_PGBAR_HEIGHT-2, 0);
                    LCDD_ClearRect(PAGE_READPARAMS_RECT_X_START+2, yOffset+2, xOffset, yOffset+PAGE_READPARAMS_PGBAR_HEIGHT-3, 3);
                    break;
                case 2:
                    error = 1;
                    string = strings[STRINGS_INDEX_UNKNOWNVALUE ];
                    break;
                case 3:
                    error = 1;
                    string = strings[STRINGS_INDEX_NOTIMPLEMENTED ];
                    break;
                }
            }
            else    // Reading failure
            {
                if (++retry >= PAGE_PARAM_READ_RETRY)
                {
                    error = 1;
                    string = strings[STRINGS_INDEX_ERRORREADING ];
                }
            }
        }
        else
        {
                // In orde to be compatible with old sensor, these parameters are bypassed during "full reading".
                // Harry 2014-04-14 according to the test on firmware of 048806 that is given by william 
                if (    (PARAMS_IsParameterAvailable(&sensorParams, currentParameter))
                     && ( PARAM_INDEX_CURRENTPOL != currentParameter )  
                     && ( PARAM_INDEX_SENSORSTATUS != currentParameter )
                     && ( PARAM_INDEX_BUZZERFEEDBACK != currentParameter )
                     && ( PARAM_INDEX_FACTORYLIGHTFACTOR != currentParameter )
                   )
                {
                    if (result = IR_ReadSensorParameter(&sensorParams, currentParameter))
                    {
                        retry = 0;
                        switch(result)
                        {
                        case 1:
                            string = NULL;
                            if (currentParameter == PARAM_INDEX_STANDBYLEVEL)
                            {
                                PARAMS_InterpretStandbyLevel(&sensorParams);
                            }
                            else if (currentParameter == PARAM_INDEX_STANDBYDELAY)
                            {
                                PARAMS_InterpretStandbyDelay(&sensorParams);
                            }
                            break;
                        case 2:
                            string = strings[STRINGS_INDEX_UNKNOWNVALUE ];
                            break;
                        case 3:
                          if( PARAM_INDEX_TYPE2 != currentParameter ){
                            string = strings[STRINGS_INDEX_NOTIMPLEMENTED ];
                          }
                          else{
                            string = NULL;                            
                          }
                            break;
                        }
                        currentParameter++;
                        xOffset += PAGE_READPARAMS_PGBAR_STEP;
                        if (xOffset > PAGE_READPARAMS_RECT_X_END-2)
                        {
                            xOffset = PAGE_READPARAMS_RECT_X_END-2;
                        }
                        LCDD_ClearRect(PAGE_READPARAMS_RECT_X_START+2, yOffset+2, xOffset, yOffset+PAGE_READPARAMS_PGBAR_HEIGHT-3, 3);
                        if (currentParameter == PARAM_INDEX_TYPE)
                        {
                            currentParameter++;
                            xOffset += PAGE_READPARAMS_PGBAR_STEP;
                            if (xOffset > PAGE_READPARAMS_RECT_X_END-2)
                            {
                                xOffset = PAGE_READPARAMS_RECT_X_END-2;
                            }
                            LCDD_ClearRect(PAGE_READPARAMS_RECT_X_START+2, yOffset+2, xOffset, yOffset+PAGE_READPARAMS_PGBAR_HEIGHT-3, 3);
                        }
                        if (currentParameter >= para_amount_in_array )
                        {
                            LCDD_ClearRect(PAGE_READPARAMS_RECT_X_START+2, yOffset+2, PAGE_READPARAMS_RECT_X_END-2, yOffset+PAGE_READPARAMS_PGBAR_HEIGHT-3, 3);
                            pageUpdate = 0;
                            pageCurrent = PAGE_INDEX_LIST_PARAMETERS;
                            pageSwap = 1;
                        }
                    }
                    else
                    {
                        if (++retry >= PAGE_PARAM_READ_RETRY)
                        {
                            // In orde to be compatible with old sensor, continue to read the following parameter even when fail to read TYPE2
                            // Harry 2014-04-14 according to the test on firmware of 048806 that is given by william                             
                            if( PARAM_INDEX_TYPE2 != currentParameter )
                            {
                                error = 1;
                                string = strings[STRINGS_INDEX_ERRORREADING ];
                            }
                        }
                    }
                }
                else
                {
                    retry = 0;
                    currentParameter++;
                    xOffset += PAGE_READPARAMS_PGBAR_STEP;
                    if (xOffset > PAGE_READPARAMS_RECT_X_END-2)
                    {
                        xOffset = PAGE_READPARAMS_RECT_X_END-2;
                    }
                    LCDD_ClearRect(PAGE_READPARAMS_RECT_X_START+2, yOffset+2, xOffset, yOffset+PAGE_READPARAMS_PGBAR_HEIGHT-3, 3);
                    if (currentParameter == PARAM_INDEX_TYPE)
                    {
                        currentParameter++;
                        xOffset += PAGE_READPARAMS_PGBAR_STEP;
                        if (xOffset > PAGE_READPARAMS_RECT_X_END-2)
                        {
                            xOffset = PAGE_READPARAMS_RECT_X_END-2;
                        }
                        LCDD_ClearRect(PAGE_READPARAMS_RECT_X_START+2, yOffset+2, xOffset, yOffset+PAGE_READPARAMS_PGBAR_HEIGHT-3, 3);
                    }
                    if (currentParameter >= para_amount_in_array )
                    {
                        LCDD_ClearRect(PAGE_READPARAMS_RECT_X_START+2, yOffset+2, PAGE_READPARAMS_RECT_X_END-2, yOffset+PAGE_READPARAMS_PGBAR_HEIGHT-3, 3);
                        pageUpdate = 0;
                        pageCurrent = PAGE_INDEX_LIST_PARAMETERS;
                        pageSwap = 1;
                    }
                }
        }

        if (error)
        {
            PARAMS_ResetToDefault(&sensorParams, PARAM_TYPE_INITIAL);      // Reset to default when reading failed
            yOffset -= PAGE_READPARAMS_PGBAR_Y_SPAN;
            LCDD_ClearRect(PAGE_READPARAMS_RECT_X_START, PAGE_READPARAMS_RECT_Y_START, PAGE_READPARAMS_RECT_X_END, yOffset, 3);
            LCDD_PutString(PAGE_READPARAMS_BODY_X_START, PAGE_READPARAMS_BODY_Y_START, PAGE_READPARAMS_BODY_X_LENGTH, string, 3, 0);
            LCDD_Refresh();     // Have to do this here.
            TIME_Wait(2000);
            pageUpdate = 0;
            pageCurrent = PAGE_INDEX_MAIN;
            pageSwap = 1;
        }
        else
        {
            if (string)
            {
                yOffset -= PAGE_READPARAMS_PGBAR_Y_SPAN;
                LCDD_ClearRect(PAGE_READPARAMS_RECT_X_START, PAGE_READPARAMS_RECT_Y_START, PAGE_READPARAMS_RECT_X_END, yOffset, 3);
                LCDD_PutString(PAGE_READPARAMS_BODY_X_START, PAGE_READPARAMS_BODY_Y_START, PAGE_READPARAMS_BODY_X_LENGTH, string, 3, 0);
                LCDD_Refresh();     // Have to do this here.
                yOffset += PAGE_READPARAMS_PGBAR_Y_SPAN;
            }
            TIME_Wait(100);
        }
    }

    if (((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED)) && (pageUpdate == 0))
    {

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        //if (tempstring)
        //{
        //    free(tempstring);
        //}

        pageSwap = 0;
        pageInitialize = 1;
    }
}

// 2.4.0
void  TimeZonePageHandler( void )
{
#define PAGE_TIMEZONE_TOTALITEMS        3
#define PAGE_TIMEZONE_BODY_Y_POS        PAGE_VALUE_Y_POS
#define PAGE_TIMEZONE_BODY_X_POS        43  //    
#define PAGE_TIMEZONE_SIGN_X_POS        42  //    45
#define PAGE_TIMEZONE_HOUR_X_POS        54  //    65
#define PAGE_TIMEZONE_MINUTE_X_POS      80

    static unsigned char currentItem;
    static PAGE_PARAMETER *pParameter;
    unsigned char temp[5];

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_GMTSIGN) && PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_GMTMINUTES) && PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_GMTMINUTES);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_GMT ]), 0, 0, strings[STRINGS_INDEX_GMT ], 0, 0);

       if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

            currentItem = 0;

            pParameter = (PAGE_PARAMETER *)malloc(3*sizeof(PAGE_PARAMETER));

            pParameter->ParamIndex = PARAM_INDEX_GMTSIGN;
            pParameter->Max = 1;
            pParameter->Min = 0;

            (pParameter+1)->ParamIndex = PARAM_INDEX_GMTHOURS;
            (pParameter+1)->Max = 23;
            (pParameter+1)->Min = 0;

            (pParameter+2)->ParamIndex = PARAM_INDEX_GMTMINUTES;
            (pParameter+2)->Max = 59;
            (pParameter+2)->Min = 0;

            _draw_rectangular_for_value();
            LCDD_PutString(13, PAGE_TIMEZONE_BODY_Y_POS, 0, "GMT:", PAGE_HIGHLIGHT_COLOR, 0);
            LCDD_PutString(69, PAGE_TIMEZONE_BODY_Y_POS, 0, "H", PAGE_HIGHLIGHT_COLOR, 0);
            LCDD_PutString(95, PAGE_TIMEZONE_BODY_Y_POS, 0, "Min", PAGE_HIGHLIGHT_COLOR, 0);

            pageUpdate = 1;
        }
        else
        {
            _draw_rectangular_for_value();
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {


        temp[0] = PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_GMTSIGN);
        if( temp[0] > 1 ){
           temp[0] = 1;
           PARAMS_SetParameterNumber(&sensorParams, PARAM_INDEX_GMTSIGN, PARAM_NUMBER_GMTSIGN); 
        }
        if( temp[0] )
          temp[0]='+';
        else
          temp[0]='-';
        temp[1] = '\0';
        LCDD_PutString(PAGE_TIMEZONE_SIGN_X_POS, PAGE_TIMEZONE_BODY_Y_POS, 0, &temp[0], (currentItem == 0)?0:PAGE_HIGHLIGHT_COLOR, 0);
        dectoascii(&temp[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_GMTHOURS), 2);
        LCDD_PutString(PAGE_TIMEZONE_HOUR_X_POS, PAGE_TIMEZONE_BODY_Y_POS, 0, &temp[0], (currentItem == 1)?0:PAGE_HIGHLIGHT_COLOR, 0);
        dectoascii(&temp[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_GMTMINUTES), 2);
        LCDD_PutString(PAGE_TIMEZONE_MINUTE_X_POS, PAGE_TIMEZONE_BODY_Y_POS, 0, &temp[0], (currentItem == 2)?0:PAGE_HIGHLIGHT_COLOR, 0);

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
                _adjust_param_value(pParameter+currentItem, 1);

                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
                _adjust_param_value(pParameter+currentItem, -1);

                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            if (pageAvailability)
            {
                if (currentItem == PAGE_TIMEZONE_TOTALITEMS-1)
                {
                    currentItem = 0;
                }
                else
                {
                    currentItem++;
                }

                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT)
        {
            if (pageAvailability)
            {
                if (currentItem == 0)
                {
                    currentItem = PAGE_TIMEZONE_TOTALITEMS-1;
                }
                else
                {
                    currentItem--;
                }

                pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
                _page_sub_read_parameters(pParameter, PAGE_TIMEZONE_TOTALITEMS);

//                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
                _page_sub_set_parameters(pParameter, PAGE_TIMEZONE_TOTALITEMS);

//                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }
    }
    else if (pageAvailability && (BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x3F) == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            _adjust_param_value(pParameter+currentItem, 1);

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            _adjust_param_value(pParameter+currentItem, -1);

            pageUpdate = 1;
        }
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }

}

void ListParametersPageHandler(void)
{
    _list_parameters(&sensorParams, 1, 0);

    if (pageInitialize)
    {
        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, PARAMETERS_LINE_1-1, 0);
        LCDD_ClearRect(0, PARAMETERS_LINE_4+PARAMETERS_LINE_HEIGHT+PARAMETERS_LINE_SPACE, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        pageFunckeyItems[3].pItem = (void *)&BitmapInfo_SaveFile;
        _common_funckey_initializer();

        pageInitialize = 0;

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;
            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F4)
        {
            pageParent = PAGE_INDEX_LIST_PARAMETERS;
            pageCurrent = PAGE_INDEX_SAVEFILE;
            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        _list_parameters(&sensorParams, 1, 1);
        pageSwap = 0;
        pageInitialize = 1;
    }
}

void SaveFilePageHandler(void)
{
//#define PAGE_SAVEFILE_LINE_1            16
//#define PAGE_SAVEFILE_LINE_HEIGHT       16
//#define PAGE_SAVEFILE_LINE_SPACE        6
//#define PAGE_SAVEFILE_LINE_2            (PAGE_SAVEFILE_LINE_1 + PAGE_SAVEFILE_LINE_HEIGHT + PAGE_SAVEFILE_LINE_SPACE)
//#define PAGE_SAVEFILE_LINE_3            (PAGE_SAVEFILE_LINE_2 + PAGE_SAVEFILE_LINE_HEIGHT + PAGE_SAVEFILE_LINE_SPACE)
//#define PAGE_SAVEFILE_LINE_4            (PAGE_SAVEFILE_LINE_3 + PAGE_SAVEFILE_LINE_HEIGHT + PAGE_SAVEFILE_LINE_SPACE)
#define PAGE_FILES_LINE_1            16
#define PAGE_FILES_LINE_HEIGHT       16
#define PAGE_FILES_LINE_SPACE        6
#define PAGE_FILES_LINE_2            (PAGE_FILES_LINE_1 + PAGE_FILES_LINE_HEIGHT + PAGE_FILES_LINE_SPACE)
#define PAGE_FILES_LINE_3            (PAGE_FILES_LINE_2 + PAGE_FILES_LINE_HEIGHT + PAGE_FILES_LINE_SPACE)
#define PAGE_FILES_LINE_4            (PAGE_FILES_LINE_3 + PAGE_FILES_LINE_HEIGHT + PAGE_FILES_LINE_SPACE)

    static PAGE_FILE_PARAMS *pFileParams;
    static FILES_FAT *pFat;
    static unsigned char currentItem = 0;
    static unsigned char totalItems;
    static unsigned char newFileAvailable;
    static unsigned char firstDisplayedItem;

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        _common_funckey_initializer();

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_SAVEFILE ]), 0, 0, strings[STRINGS_INDEX_SAVEFILE ], 0, 0);

//        currentItem = 0;

        pFat = (FILES_FAT *)malloc(sizeof(FILES_FAT));

        FILES_ReadFAT(pFat);

        totalItems = FILES_GetFileNumber(pFat);

        newFileAvailable = 0;
        if (totalItems < FILES_MAX_NUMBER)
        {
            totalItems += 1; // Plus 1 for new file
            newFileAvailable = 1;
        }

        if (currentItem > totalItems - 1)
        {
            currentItem = totalItems - 1;
        }

        pFileParams = (PAGE_FILE_PARAMS *)malloc((FILES_MAX_NUMBER)*sizeof(PAGE_FILE_PARAMS));    //use FILES_MAX_NUMBER for worst case testing

        for (unsigned char i=1; i<=FILES_GetFileNumber(pFat); i++)
        {
            FILES_ReadFileName(pFileParams[i-1].pFileName, pFat, i);
        }

        if (newFileAvailable)
        {
            strcpy((char *)(pFileParams[totalItems-1].pFileName), (char const *)strings[STRINGS_INDEX_NEWFILE ]);
        }

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        firstDisplayedItem = 0;

//        if (totalItems > 4)
//        {
        if (currentItem > 2)
        {
            firstDisplayedItem = currentItem - 2;

            if (currentItem == totalItems - 1)
            {
                firstDisplayedItem = totalItems - 4;
            }
        }
//        }

        if (totalItems >= 1)
        {
            LCDD_ClearRect(0, PAGE_FILES_LINE_1, LCD_X_LENGTH-1, PAGE_FILES_LINE_1+PAGE_FILES_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 0) ? 3:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_FILES_LINE_1, 0, (pFileParams+firstDisplayedItem)->pFileName, (currentItem - firstDisplayedItem == 0) ? 3:0, 0);
            LCDD_PutString(PAGE_ARROW_NAV_X_POS-2, PAGE_FILES_LINE_1, 0, "OK", (currentItem - firstDisplayedItem == 0) ? 3:0, 0);
//            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_FILES_LINE_1, (currentItem - firstDisplayedItem == 0) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 2)
        {
            LCDD_ClearRect(0, PAGE_FILES_LINE_2, LCD_X_LENGTH-1, PAGE_FILES_LINE_2+PAGE_FILES_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 1) ? 3:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_FILES_LINE_2, 0, (pFileParams+firstDisplayedItem+1)->pFileName, (currentItem - firstDisplayedItem == 1) ? 3:0, 0);
            LCDD_PutString(PAGE_ARROW_NAV_X_POS-2, PAGE_FILES_LINE_2, 0, "OK", (currentItem - firstDisplayedItem == 1) ? 3:0, 0);
//            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_FILES_LINE_2, (currentItem - firstDisplayedItem == 1) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 3)
        {
            LCDD_ClearRect(0, PAGE_FILES_LINE_3, LCD_X_LENGTH-1, PAGE_FILES_LINE_3+PAGE_FILES_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 2) ? 3:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_FILES_LINE_3, 0, (pFileParams+firstDisplayedItem+2)->pFileName, (currentItem - firstDisplayedItem == 2) ? 3:0, 0);
            LCDD_PutString(PAGE_ARROW_NAV_X_POS-2, PAGE_FILES_LINE_3, 0, "OK", (currentItem - firstDisplayedItem == 2) ? 3:0, 0);
//            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_FILES_LINE_3, (currentItem - firstDisplayedItem == 2) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 4)
        {
            LCDD_ClearRect(0, PAGE_FILES_LINE_4, LCD_X_LENGTH-1, PAGE_FILES_LINE_4+PAGE_FILES_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 3) ? 3:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_FILES_LINE_4, 0, (pFileParams+firstDisplayedItem+3)->pFileName, (currentItem - firstDisplayedItem == 3) ? 3:0, 0);
            LCDD_PutString(PAGE_ARROW_NAV_X_POS-2, PAGE_FILES_LINE_4, 0, "OK", (currentItem - firstDisplayedItem == 3) ? 3:0, 0);
//            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_FILES_LINE_4, (currentItem - firstDisplayedItem == 3) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = totalItems-1;
            }
            else
            {
                currentItem--;
            }

//            BUTTON_Acknowledge();
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == totalItems-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

//            BUTTON_Acknowledge();
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR)
        {
            if (newFileAvailable && (currentItem == totalItems-1))  // To create a new file
            {
                argForSubPage = currentItem - firstDisplayedItem;
                argForSubPage2 = 0;     // Indicating to save a new file
                pageCurrent = PAGE_INDEX_NEWFILE;
            }
            else // To replace an existing file
            {
                argForSubPage = currentItem + 1;    // index of save the file at
                pageCurrent = PAGE_INDEX_REPLACEFILE;
            }
            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = pageParent;
            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pFileParams)
        {
            free(pFileParams);
        }

        if (pFat)
        {
            free(pFat);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void NewFilePageHandler(void)
{
#define PAGE_NEWFILE_CURSORFLASH    500     // 500mS
//#define PAGE_NEWFILE_LINE_INDENT    1
#define PAGE_NEWFILE_RECT_X_START   8
#define PAGE_NEWFILE_RECT_Y_START   20
#define PAGE_NEWFILE_RECT_X_END     (LCD_X_LENGTH-PAGE_NEWFILE_RECT_X_START-1)
#define PAGE_NEWFILE_BODY_INDENT    4
#define PAGE_NEWFILE_BODY_X_START   (PAGE_NEWFILE_RECT_X_START+PAGE_NEWFILE_BODY_INDENT)
#define PAGE_NEWFILE_BODY_X_LENGTH  (PAGE_NEWFILE_RECT_X_END-PAGE_NEWFILE_BODY_INDENT-PAGE_NEWFILE_BODY_X_START)
#define PAGE_NEWFILE_BODY_Y_START   (PAGE_NEWFILE_RECT_Y_START+PAGE_NEWFILE_BODY_INDENT)

    static SENSOR_PARAMS *pParams;
    static unsigned char *pFileName;
    static unsigned char index;
    static unsigned char xOffset, yOffset;
    static unsigned char invertCursor;
    static unsigned int timeStamp;
    unsigned char *pCurrent;
    FILES_FAT fat;
    unsigned char yEnd;

    if (pageInitialize)
    {
        pageInitialize = 0;

        index = 0;
        xOffset = PAGE_LINE_INDENT;
        yOffset = PAGE_FILES_LINE_1 + argForSubPage*(PAGE_FILES_LINE_HEIGHT+PAGE_FILES_LINE_SPACE);

        LCDD_ClearRect(0, yOffset, LCD_X_LENGTH-1, yOffset+PAGE_FILES_LINE_HEIGHT-1, 0);
        LCDD_PutString(PAGE_ARROW_NAV_X_POS-2, yOffset, 0, "OK", 0, 0);
//        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, yOffset, &BitmapInfo_ArrowNav[0], 0);

        pParams = (SENSOR_PARAMS *)malloc(sizeof(SENSOR_PARAMS));
        pFileName = (unsigned char *)malloc(FILES_MAX_NAME+1);
        memset(pFileName, '\0', FILES_MAX_NAME+1);

        invertCursor = 1;

        timeStamp = TIME_GetTime() + PAGE_NEWFILE_CURSORFLASH;

        pageUpdate = 1;
    }

    pCurrent = pFileName+index;

    if (pageUpdate)
    {
        LCDD_ClearRect(xOffset, yOffset, xOffset+FONT_CHAR_MAX_WIDTH-1, yOffset+PAGE_FILES_LINE_HEIGHT, 0);
        LCDD_PutChar(xOffset, yOffset, (*pCurrent == '\0')?' ':*pCurrent, invertCursor?3:0, 0);
        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || ((BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x7F) == 0)))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (*pCurrent == '\0')
            {
                if (index == 0) // digit is not allowed for the first character
                {
                    *pCurrent = 'A';
                }
                else
                {
                    *pCurrent = '0';
                }
            }
            else if (*pCurrent == '9')
            {
                *pCurrent = 'A';
            }
            else if (*pCurrent == 'Z')
            {
                *pCurrent = '_';
            }
            else if (*pCurrent == '_')
            {
                *pCurrent = 'a';
            }
            else if (*pCurrent == 'z')
            {
                if (index == 0) // digit is not allowed for the first character
                {
                    *pCurrent = 'A';
                }
                else
                {
                    *pCurrent = '0';
                }
            }
            else
            {
                (*pCurrent)++;
            }

//            BUTTON_Acknowledge();
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (*pCurrent == '\0')
            {
                *pCurrent = 'z';
            }
            else if (*pCurrent == 'a')
            {
                *pCurrent = '_';
            }
            else if (*pCurrent == '_')
            {
                *pCurrent = 'Z';
            }
            else if (*pCurrent == 'A')
            {
                if (index == 0) // digit is not allowed for the first character
                {
                    *pCurrent = 'z';
                }
                else
                {
                    *pCurrent = '9';
                }
            }
            else if (*pCurrent == '0')
            {
                *pCurrent = 'z';
            }
            else
            {
                (*pCurrent)--;
            }

//            BUTTON_Acknowledge();
            pageUpdate = 1;
        }
    }

    if  ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT)
        {
            if (index > 0)
            {
                LCDD_ClearRect(xOffset, yOffset, xOffset+FONT_CHAR_MAX_WIDTH-1, yOffset+PAGE_FILES_LINE_HEIGHT, 0);
                *(pCurrent--) = '\0';
                xOffset -= FontGetCharWidth(&FontTrebuchetMS16, *pCurrent);
                index--;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            if ((*pCurrent != '\0') && (index < FILES_MAX_NAME-1) && (xOffset < PAGE_ARROW_NAV_X_POS-22))
            {
                LCDD_PutChar(xOffset, yOffset, *pCurrent, 0, 0);
                xOffset += FontGetCharWidth(&FontTrebuchetMS16, *pCurrent);
                *(pCurrent+1) = *pCurrent;
                index++;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR)
        {
            if ((index > 0) || (*pCurrent != '\0'))
            {
                FILES_ReadFAT(&fat);
                if (argForSubPage2 == 0)    // Indicating to save a new file
                {
                    if (FILES_SaveFile(&sensorParams, pFileName, &fat, FILES_GetFileNumber(&fat)+1))
                    {
                        yEnd = PAGE_NEWFILE_BODY_Y_START+LCDD_PutString(PAGE_NEWFILE_BODY_X_START, PAGE_NEWFILE_BODY_Y_START, PAGE_NEWFILE_BODY_X_LENGTH, strings[STRINGS_INDEX_FILESAVED ], 3, 1);
                        LCDD_ClearRect(PAGE_NEWFILE_RECT_X_START, PAGE_NEWFILE_RECT_Y_START, PAGE_NEWFILE_RECT_X_END, yEnd+PAGE_NEWFILE_BODY_INDENT, 3);
                        LCDD_PutString(PAGE_NEWFILE_BODY_X_START, PAGE_NEWFILE_BODY_Y_START, PAGE_NEWFILE_BODY_X_LENGTH, strings[STRINGS_INDEX_FILESAVED ], 3, 0);
                        LCDD_Refresh();     // Have to do this here.
                        TIME_Wait(2000);
                    }
                    pageCurrent = PAGE_INDEX_SAVEFILE;
                }
                else    // Renaming an existing file, the index was saved in argForSubPage2
                {
                    if (FILES_ReadFileParams(pParams, &fat, argForSubPage2))    // Read the existing parameters
                    {
                        if (FILES_SaveFile(pParams, pFileName, &fat, argForSubPage2))   // Save the file back with new file name
                        {
                            // Might want to display something?
                        }
                    }
                    pageCurrent = PAGE_INDEX_FILES;
                }
                pageSwap = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            if (argForSubPage2 == 0)    // Saving a new file, back to save file page
            {
                pageCurrent = PAGE_INDEX_SAVEFILE;
            }
            else        // Renaming an existing file, back to files page
            {
                pageCurrent = PAGE_INDEX_FILES;
            }
            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    if (pageUpdate == 1)
    {
        invertCursor = 1;
        timeStamp = TIME_GetTime() + PAGE_NEWFILE_CURSORFLASH;
    }
    else if (TIME_GetTime() >= timeStamp)
    {
        invertCursor ^= 1;
        timeStamp = TIME_GetTime() + PAGE_NEWFILE_CURSORFLASH;
        pageUpdate = 1;
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParams)
        {
            free(pParams);
        }

        if (pFileName)
        {
            free(pFileName);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void ReplaceFilePageHandler(void)
{
#define PAGE_REPLACEFILE_RECT_X_START   8
#define PAGE_REPLACEFILE_RECT_Y_START   20
#define PAGE_REPLACEFILE_RECT_X_END     (LCD_X_LENGTH-PAGE_REPLACEFILE_RECT_X_START-1)
#define PAGE_REPLACEFILE_BODY_INDENT    4
#define PAGE_REPLACEFILE_BODY_X_START   (PAGE_REPLACEFILE_RECT_X_START+PAGE_REPLACEFILE_BODY_INDENT)
#define PAGE_REPLACEFILE_BODY_X_LENGTH  (PAGE_REPLACEFILE_RECT_X_END-PAGE_REPLACEFILE_BODY_INDENT-PAGE_REPLACEFILE_BODY_X_START)
#define PAGE_REPLACEFILE_BODY_Y_START   (PAGE_REPLACEFILE_RECT_Y_START+PAGE_REPLACEFILE_BODY_INDENT)

    static FILES_FAT *pFat;
    static unsigned char *pFileName;
    unsigned char yEnd;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pFat = (FILES_FAT *)malloc(sizeof(FILES_FAT));
        pFileName = malloc(FILES_MAX_NAME+1);

        FILES_ReadFAT(pFat);
        FILES_ReadFileName(pFileName, pFat, argForSubPage);

        yEnd = PAGE_REPLACEFILE_BODY_Y_START+LCDD_PutString(PAGE_REPLACEFILE_BODY_X_START, PAGE_REPLACEFILE_BODY_Y_START, PAGE_REPLACEFILE_BODY_X_LENGTH, strings[STRINGS_INDEX_REPLACEFILE ], 3, 1);
        LCDD_ClearRect(PAGE_REPLACEFILE_RECT_X_START, PAGE_REPLACEFILE_RECT_Y_START, PAGE_REPLACEFILE_RECT_X_END, yEnd+PAGE_REPLACEFILE_BODY_INDENT, 3);
        LCDD_PutString(PAGE_REPLACEFILE_BODY_X_START, PAGE_REPLACEFILE_BODY_Y_START, PAGE_REPLACEFILE_BODY_X_LENGTH, strings[STRINGS_INDEX_REPLACEFILE ], 3, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[1].pItem = (void *)strings[STRINGS_INDEX_YES ];
        pageFunckeyItems[1].itemType = 1;
        pageFunckeyItems[2].pItem = (void *)strings[STRINGS_INDEX_NO_1 ];
        pageFunckeyItems[2].itemType = 1;
        _common_funckey_initializer();
    }

    if (pageUpdate)
    {
        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F2)
        {
            if (FILES_SaveFile(&sensorParams, pFileName, pFat, argForSubPage))
            {
                yEnd = PAGE_REPLACEFILE_BODY_Y_START+LCDD_PutString(PAGE_REPLACEFILE_BODY_X_START, PAGE_REPLACEFILE_BODY_Y_START, PAGE_REPLACEFILE_BODY_X_LENGTH, strings[STRINGS_INDEX_FILESAVED ], 3, 1);
                LCDD_ClearRect(PAGE_REPLACEFILE_RECT_X_START, PAGE_REPLACEFILE_RECT_Y_START, PAGE_REPLACEFILE_RECT_X_END, yEnd+PAGE_REPLACEFILE_BODY_INDENT, 3);
                LCDD_PutString(PAGE_REPLACEFILE_BODY_X_START, PAGE_REPLACEFILE_BODY_Y_START, PAGE_REPLACEFILE_BODY_X_LENGTH, strings[STRINGS_INDEX_FILESAVED ], 3, 0);
                LCDD_Refresh();     // Have to do this here.
                TIME_Wait(2000);
            }
            pageCurrent = PAGE_INDEX_SAVEFILE;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            pageCurrent = PAGE_INDEX_SAVEFILE;
        }
        pageSwap = 1;

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pFat)
        {
            free(pFat);
        }

        if (pFileName)
        {
            free(pFileName);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void ParametersPageHandler(void)
{
#define PAGE_PARAMETERS_MAXIMUMITEMS    12      // 1.0.24, was 6
#define PAGE_PARAMETERS_LINE_1          16
#define PAGE_PARAMETERS_LINE_HEIGHT     16
#define PAGE_PARAMETERS_LINE_SPACE      6
#define PAGE_PARAMETERS_LINE_2          (PAGE_PARAMETERS_LINE_1 + PAGE_PARAMETERS_LINE_HEIGHT + PAGE_PARAMETERS_LINE_SPACE)
#define PAGE_PARAMETERS_LINE_3          (PAGE_PARAMETERS_LINE_2 + PAGE_PARAMETERS_LINE_HEIGHT + PAGE_PARAMETERS_LINE_SPACE)
#define PAGE_PARAMETERS_LINE_4          (PAGE_PARAMETERS_LINE_3 + PAGE_PARAMETERS_LINE_HEIGHT + PAGE_PARAMETERS_LINE_SPACE)

    static PAGE_PARAMS *pPageParams;
    PAGE_PARAMS *pTempParams;
    static unsigned char currentItem = 0;
    static unsigned char totalItems;
    unsigned char firstDisplayedItem;

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        pageFunckeyItems[3].pItem = (void *)&BitmapInfo_SaveFile;
        _common_funckey_initializer();

        pPageParams = (PAGE_PARAMS *)malloc(PAGE_PARAMETERS_MAXIMUMITEMS * sizeof(PAGE_PARAMS));
        pTempParams = pPageParams;

        totalItems = 0;

        // 2.4.0
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_YEAR) && PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MONTH) && PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DAY))
        {
            pTempParams->pString = strings[STRINGS_INDEX_DATE ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_DATE;
            pTempParams++;
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_HOUR) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MINUTE) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_SECOND))
        {
            if (PARAMS_IsLightController(&sensorParams))    // 2.4.0
            {
                pTempParams->pString = strings[STRINGS_INDEX_HOUR ];
            }
            else
            {
                pTempParams->pString = strings[STRINGS_INDEX_TIMEDELAY ];
            }
            pTempParams->PageHandlerIndex = PAGE_INDEX_TIMEDELAY;
            pTempParams++;
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_HF_SENS) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_PIR_SENS) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_US_SENS))
        {
            pTempParams->pString = strings[STRINGS_INDEX_SENSITIVITY ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_SENSITIVITY;
            pTempParams++;
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DAYLIGHTSETPOINT))
        {
            if (PARAMS_IsLightController(&sensorParams))    // 2.4.0
            {
                pTempParams->pString = strings[STRINGS_INDEX_SETPOINT ];
            }
            else
            {
                pTempParams->pString = strings[STRINGS_INDEX_DAYLIGHTSETPOINT ];
            }
            pTempParams->PageHandlerIndex = PAGE_INDEX_DAYLIGHTSETPOINT;
            pTempParams++;
        }
        if (PARAMS_IsDALI_MASTER_DL_PIR_US(&sensorParams) || PARAMS_IsDALI_MASTER_DL_OP_PIR_US(&sensorParams)|| PARAMS_IsDALI_MASTER_OP_PIR_US(&sensorParams)) 
        {
          if(PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_OFFset))
          {
            pTempParams->pString = "offset";
            pTempParams->PageHandlerIndex = PAGE_INDEX_OFFSET;
            pTempParams++;       
          }
        }
        
        if (PARAMS_IsDALI_MASTER_DL_OP_PIR_US(&sensorParams)||PARAMS_IsDALI_MASTER_OP_PIR_US(&sensorParams)||PARAMS_IsDALI_SLAVE_OP_PIR_US(&sensorParams)||PARAMS_IsDALI_MASTER_DL_OP_PIR(&sensorParams)||PARAMS_IsDALI_MASTER_OP_PIR(&sensorParams)||PARAMS_IsDALI_SLAVE_OP_PIR(&sensorParams) ||PARAMS_IsDALI_MASTER_PIR_DL(&sensorParams) )  //2.5.7
        {  if(PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DetectionLed))
         {
            pTempParams->pString = "Detection LED";
            pTempParams->PageHandlerIndex = PAGE_INDEX_Detection_LED;
            pTempParams++;  
          }
        }
        if (PARAMS_IsDALI_MASTER_DL_PIR_US(&sensorParams)||PARAMS_IsDALI_MASTER_DL_OP_PIR_US(&sensorParams)||PARAMS_IsDALI_MASTER_DL_OP_PIR(&sensorParams)||PARAMS_IsDALI_MASTER_OP_PIR(&sensorParams)||PARAMS_IsDALI_MASTER_PIR_DL(&sensorParams))  
        {
          if(PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_SensorConfig))
          {
            pTempParams->pString = "Sensor Config";
            pTempParams->PageHandlerIndex = PAGE_INDEX_SensorConfig;
            pTempParams++; 
          }
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MODE))
        {
            pTempParams->pString = strings[STRINGS_INDEX_MODE ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_MODE;
            pTempParams++;
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DETECTIONSCHEME) || 
            (    PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_INITIAL) 
              || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MAINTAIN) 
              || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_RETRIGGER)))
        {
            pTempParams->pString = strings[STRINGS_INDEX_DETECTIONSCHEME ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_DETECTIONSCHEME;
            pTempParams++;
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_ALERT))
        {
            pTempParams->pString = strings[STRINGS_INDEX_ALERT ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_ALERT;
            pTempParams++;
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_STANDBYLEVEL))
        {
            pTempParams->pString = strings[STRINGS_INDEX_STANDBYLEVEL ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_STANDBYLEVEL;
            pTempParams++;
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_STANDBYDELAY))
        {
            pTempParams->pString = strings[STRINGS_INDEX_STANDBYDELAY ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_STANDBYDELAY;
            pTempParams++;
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_SENDCONDITION))
        {
            pTempParams->pString = strings[STRINGS_INDEX_SENDCONDITION ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_SENDCONDITION;
            pTempParams++;
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DEADBAND))
        {
            pTempParams->pString = strings[STRINGS_INDEX_DEADBAND ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_DEADBAND;
            pTempParams++;
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_TIMEBASE))
        {
            pTempParams->pString = strings[STRINGS_INDEX_TIMEBASE ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_TIMEBASE;
            pTempParams++;
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_BANDWIDTH))
        {
            pTempParams->pString = strings[STRINGS_INDEX_BANDWIDTH ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_BANDWIDTH;
            pTempParams++;
        }
        // 2.4.0
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_SUMMERHOUR))     // Do we need to check Start month, Start Day, End month and End day?
        {
            pTempParams->pString = strings[STRINGS_INDEX_SUMMERHOUR ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_SUMMERHOUR;
            pTempParams++;
        }
        // 2.4.0
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_COUNTRY))        // Do we need to check Longitude and Latitude?
        {
            pTempParams->pString = strings[STRINGS_INDEX_COUNTRY ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_COUNTRY;
            pTempParams++;
        }
        // 2.4.0
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_GMTSIGN))        // Do we need to check Longitude and Latitude?
        {
            pTempParams->pString = strings[STRINGS_INDEX_GMT ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_TIMEZONE;
            pTempParams++;
        }

        totalItems = pTempParams - pPageParams;

        if (currentItem >= totalItems)
        {
            currentItem = totalItems-1;
        }

        pageScroll = 0;  // 2.4.0

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        firstDisplayedItem = 0;

        if (currentItem > 2)
        {
            firstDisplayedItem = currentItem - 2;

            if (currentItem == totalItems - 1)
            {
                firstDisplayedItem = totalItems - 4;
            }
        }

        if (totalItems >= 1)
        {
            LCDD_ClearRect(0, PAGE_PARAMETERS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_PARAMETERS_LINE_1+PAGE_PARAMETERS_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 0) ? PAGE_HIGHLIGHT_COLOR:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_PARAMETERS_LINE_1, 0, (pPageParams+firstDisplayedItem)->pString, (currentItem - firstDisplayedItem == 0) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_PARAMETERS_LINE_1, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), (pPageParams+firstDisplayedItem)->pString+((currentItem - firstDisplayedItem == 0)?pageScroll:0), (currentItem - firstDisplayedItem == 0)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 0)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_PARAMETERS_LINE_1, (currentItem - firstDisplayedItem == 0) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 2)
        {
            LCDD_ClearRect(0, PAGE_PARAMETERS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_PARAMETERS_LINE_2+PAGE_PARAMETERS_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 1) ? PAGE_HIGHLIGHT_COLOR:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_PARAMETERS_LINE_2, 0, (pPageParams+firstDisplayedItem+1)->pString, (currentItem - firstDisplayedItem == 1) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_PARAMETERS_LINE_2, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), (pPageParams+firstDisplayedItem+1)->pString+((currentItem - firstDisplayedItem == 1)?pageScroll:0), (currentItem - firstDisplayedItem == 1)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 1)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_PARAMETERS_LINE_2, (currentItem - firstDisplayedItem == 1) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 3)
        {
            LCDD_ClearRect(0, PAGE_PARAMETERS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_PARAMETERS_LINE_3+PAGE_PARAMETERS_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 2) ? PAGE_HIGHLIGHT_COLOR:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_PARAMETERS_LINE_3, 0, (pPageParams+firstDisplayedItem+2)->pString, (currentItem - firstDisplayedItem == 2) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_PARAMETERS_LINE_3, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), (pPageParams+firstDisplayedItem+2)->pString+((currentItem - firstDisplayedItem == 2)?pageScroll:0), (currentItem - firstDisplayedItem == 2)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 2)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_PARAMETERS_LINE_3, (currentItem - firstDisplayedItem == 2) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 4)
        {
            LCDD_ClearRect(0, PAGE_PARAMETERS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_PARAMETERS_LINE_4+PAGE_PARAMETERS_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 3) ? PAGE_HIGHLIGHT_COLOR:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_PARAMETERS_LINE_4, 0, (pPageParams+firstDisplayedItem+3)->pString, (currentItem - firstDisplayedItem == 3) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_PARAMETERS_LINE_4, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), (pPageParams+firstDisplayedItem+3)->pString+((currentItem - firstDisplayedItem == 3)?pageScroll:0), (currentItem - firstDisplayedItem == 3)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 3)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_PARAMETERS_LINE_4, (currentItem - firstDisplayedItem == 3) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = totalItems-1;
            }
            else
            {
                currentItem--;
            }

            pageScroll = 0;  // 2.4.0
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == totalItems-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

            pageScroll = 0;  // 2.4.0
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            pageCurrent = (pPageParams+currentItem)->PageHandlerIndex;

            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F4)
        {
            pageParent = PAGE_INDEX_PARAMETERS;
            pageCurrent = PAGE_INDEX_SAVEFILE;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    // 2.4.0
    if (TIME_GetTime() >= pageTimeStamp)
    {
        pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
        pageUpdate = 1;
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pPageParams)
        {
            free(pPageParams);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void AdvancedModePageHandler(void)
{
#define PAGE_ADVANCEDMODE_MAXIMUMITEMS    5     // 1.0.24, was 10
#define PAGE_ADVANCEDMODE_LINE_1          16
#define PAGE_ADVANCEDMODE_LINE_HEIGHT     16
#define PAGE_ADVANCEDMODE_LINE_SPACE      6
#define PAGE_ADVANCEDMODE_LINE_2          (PAGE_ADVANCEDMODE_LINE_1 + PAGE_ADVANCEDMODE_LINE_HEIGHT + PAGE_ADVANCEDMODE_LINE_SPACE)
#define PAGE_ADVANCEDMODE_LINE_3          (PAGE_ADVANCEDMODE_LINE_2 + PAGE_ADVANCEDMODE_LINE_HEIGHT + PAGE_ADVANCEDMODE_LINE_SPACE)
#define PAGE_ADVANCEDMODE_LINE_4          (PAGE_ADVANCEDMODE_LINE_3 + PAGE_ADVANCEDMODE_LINE_HEIGHT + PAGE_ADVANCEDMODE_LINE_SPACE)

    static PAGE_PARAMS *pPageParams;
    PAGE_PARAMS *pTempParams;
    static unsigned char currentItem = 0;
    static unsigned char totalItems;
    unsigned char firstDisplayedItem;

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        pageFunckeyItems[3].pItem = (void *)&BitmapInfo_SaveFile;
        _common_funckey_initializer();

        pPageParams = (PAGE_PARAMS *)malloc(PAGE_ADVANCEDMODE_MAXIMUMITEMS * sizeof(PAGE_PARAMS));
        pTempParams = pPageParams;

        totalItems = 0;

        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DAYLIGHTFACTOR) || \
            PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DAYLIGHTFACTORCEN) || \
            PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_NATURALLIGHTFACTOR) || \
            PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_FACTORYLIGHTFACTOR)
            )
        {
            pTempParams->pString = strings[STRINGS_INDEX_CALIBRATION ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_CALIBRATION;
            pTempParams++;
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_LIGHTREGULATION))
        {
            pTempParams->pString = strings[STRINGS_INDEX_LIGHTREGULATION ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_LIGHTREGULATION;
            pTempParams++;
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_PROVISIONOFLIGHT))
        {
            pTempParams->pString = strings[STRINGS_INDEX_PROVISIONOFLIGHT ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_PROVISIONOFLIGHT;
            pTempParams++;
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_LOOPTYPE))
        {
            pTempParams->pString = strings[STRINGS_INDEX_LOOPTYPE ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_LOOPTYPE;
            pTempParams++;
        }
        
        if( is_KNX_sensor( &sensorParams ) )
        {
            if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_KNX_MASTER_SLAVE ) )
            {
                pTempParams->pString = KNX_sensor_master_slave_menu_item_langeage_string[language];
                pTempParams->PageHandlerIndex = PAGE_INDEX_KNX_MASTER_SLAVE;
                pTempParams++;             
            }
           
        }

        totalItems = pTempParams - pPageParams;

        if (currentItem >= totalItems)
        {
            currentItem = totalItems-1;
        }

        pageScroll = 0;  // 2.4.0

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        firstDisplayedItem = 0;

        if (currentItem > 2)
        {
            firstDisplayedItem = currentItem - 2;

            if (currentItem == totalItems - 1)
            {
                firstDisplayedItem = totalItems - 4;
            }
        }

        if (totalItems >= 1)
        {
            LCDD_ClearRect(0, PAGE_ADVANCEDMODE_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_ADVANCEDMODE_LINE_1+PAGE_ADVANCEDMODE_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 0) ? PAGE_HIGHLIGHT_COLOR:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_ADVANCEDMODE_LINE_1, 0, (pPageParams+firstDisplayedItem)->pString, (currentItem - firstDisplayedItem == 0) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_ADVANCEDMODE_LINE_1, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), (pPageParams+firstDisplayedItem)->pString+((currentItem - firstDisplayedItem == 0)?pageScroll:0), (currentItem - firstDisplayedItem == 0)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 0)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_ADVANCEDMODE_LINE_1, (currentItem - firstDisplayedItem == 0) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 2)
        {
            LCDD_ClearRect(0, PAGE_ADVANCEDMODE_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_ADVANCEDMODE_LINE_2+PAGE_ADVANCEDMODE_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 1) ? PAGE_HIGHLIGHT_COLOR:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_ADVANCEDMODE_LINE_2, 0, (pPageParams+firstDisplayedItem+1)->pString, (currentItem - firstDisplayedItem == 1) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_ADVANCEDMODE_LINE_2, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), (pPageParams+firstDisplayedItem+1)->pString+((currentItem - firstDisplayedItem == 1)?pageScroll:0), (currentItem - firstDisplayedItem == 1)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 1)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_ADVANCEDMODE_LINE_2, (currentItem - firstDisplayedItem == 1) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 3)
        {
            LCDD_ClearRect(0, PAGE_ADVANCEDMODE_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_ADVANCEDMODE_LINE_3+PAGE_ADVANCEDMODE_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 2) ? PAGE_HIGHLIGHT_COLOR:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_ADVANCEDMODE_LINE_3, 0, (pPageParams+firstDisplayedItem+2)->pString, (currentItem - firstDisplayedItem == 2) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_ADVANCEDMODE_LINE_3, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), (pPageParams+firstDisplayedItem+2)->pString+((currentItem - firstDisplayedItem == 2)?pageScroll:0), (currentItem - firstDisplayedItem == 2)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 2)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_ADVANCEDMODE_LINE_3, (currentItem - firstDisplayedItem == 2) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 4)
        {
            LCDD_ClearRect(0, PAGE_ADVANCEDMODE_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_ADVANCEDMODE_LINE_4+PAGE_ADVANCEDMODE_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 3) ? PAGE_HIGHLIGHT_COLOR:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_ADVANCEDMODE_LINE_4, 0, (pPageParams+firstDisplayedItem+3)->pString, (currentItem - firstDisplayedItem == 3) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_ADVANCEDMODE_LINE_4, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), (pPageParams+firstDisplayedItem+3)->pString+((currentItem - firstDisplayedItem == 3)?pageScroll:0), (currentItem - firstDisplayedItem == 3)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 3)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_ADVANCEDMODE_LINE_4, (currentItem - firstDisplayedItem == 3) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = totalItems-1;
            }
            else
            {
                currentItem--;
            }

            pageScroll = 0;  // 2.4.0
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == totalItems-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

            pageScroll = 0;  // 2.4.0
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            pageCurrent = (pPageParams+currentItem)->PageHandlerIndex;

            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F4)
        {
            pageParent = PAGE_INDEX_ADVANCEDMODE;
            pageCurrent = PAGE_INDEX_SAVEFILE;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    // 2.4.0
    if (TIME_GetTime() >= pageTimeStamp)
    {
        pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
        pageUpdate = 1;
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pPageParams)
        {
            free(pPageParams);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void HiddenPageHandler(void)
{
#ifdef ROOM_CONTROL
#define PAGE_HIDDEN_MAXIMUMITEMS    9
#else
#define PAGE_HIDDEN_MAXIMUMITEMS    8
#endif
#define PAGE_HIDDEN_LINE_1          16
#define PAGE_HIDDEN_LINE_HEIGHT     16
#define PAGE_HIDDEN_LINE_SPACE      6
#define PAGE_HIDDEN_LINE_2          (PAGE_HIDDEN_LINE_1 + PAGE_HIDDEN_LINE_HEIGHT + PAGE_HIDDEN_LINE_SPACE)
#define PAGE_HIDDEN_LINE_3          (PAGE_HIDDEN_LINE_2 + PAGE_HIDDEN_LINE_HEIGHT + PAGE_HIDDEN_LINE_SPACE)
#define PAGE_HIDDEN_LINE_4          (PAGE_HIDDEN_LINE_3 + PAGE_HIDDEN_LINE_HEIGHT + PAGE_HIDDEN_LINE_SPACE)

    static PAGE_PARAMS *pPageParams;
    PAGE_PARAMS *pTempParams;
    static unsigned char currentItem = 0;
    static unsigned char totalItems;
    unsigned char firstDisplayedItem;

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        _common_funckey_initializer();

        pPageParams = (PAGE_PARAMS *)malloc(PAGE_HIDDEN_MAXIMUMITEMS * sizeof(PAGE_PARAMS));
        pTempParams = pPageParams;

//        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DAYLIGHTFACTOR))
//        {
            pTempParams->pString = strings[STRINGS_INDEX_DAYLIGHTFACTOR ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_DAYLIGHTFACTOR;
            pTempParams++;
//        }
//        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_NATURALLIGHTFACTOR))
//        {
            pTempParams->pString = strings[STRINGS_INDEX_NATURALLIGHTFACTOR ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_NATURALLIGHTFACTOR;
            pTempParams++;
//        }
//        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DAYLIGHTFACTORCEN))
//        {
            pTempParams->pString = strings[STRINGS_INDEX_DAYLIGHTFACTORCEN ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_DAYLIGHTFACTORCEN;
            pTempParams++;
//        }
//        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DAYLIGHTLEVEL))
//        {
            pTempParams->pString = strings[STRINGS_INDEX_DAYLIGHTLEVEL ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_DAYLIGHTLEVEL;
            pTempParams++;
//        }
//        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_SENSORSTATUS))
//        {
            pTempParams->pString = strings[STRINGS_INDEX_SENSORSTATUS ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_SENSORSTATUS;
            pTempParams++;
//        }
            pTempParams->pString = strings[STRINGS_INDEX_CURRENTPOL ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_CURRENTPOL;
            pTempParams++;
#ifdef ROOM_CONTROL
        pTempParams->pString = strings[STRINGS_INDEX_ROOMCONTROL ];
        pTempParams->PageHandlerIndex = PAGE_INDEX_ROOMCONTROL;
        pTempParams++;
#endif
            pTempParams->pString = strings[STRINGS_INDEX_OTHERPARAMETERS ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_OTHERPARAMETERS;
            pTempParams++;

          
            pTempParams->pString = strings[STRINGS_INDEX_DEVICE_ID ];
            pTempParams->PageHandlerIndex = PAGE_INDEX_DEVICEIDZIGBEE;
            pTempParams++;

        totalItems = pTempParams - pPageParams;

        if (currentItem >= totalItems)
        {
            currentItem = totalItems-1;
        }

        pageScroll = 0;  // 2.4.0

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        firstDisplayedItem = 0;

        if (currentItem > 2)
        {
            firstDisplayedItem = currentItem - 2;

            if (currentItem == totalItems - 1)
            {
                firstDisplayedItem = totalItems - 4;
            }
        }

        if (totalItems >= 1)
        {
            LCDD_ClearRect(0, PAGE_HIDDEN_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_HIDDEN_LINE_1+PAGE_HIDDEN_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 0) ? PAGE_HIGHLIGHT_COLOR:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_HIDDEN_LINE_1, 0, (pPageParams+firstDisplayedItem)->pString, (currentItem - firstDisplayedItem == 0) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_HIDDEN_LINE_1, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), (pPageParams+firstDisplayedItem)->pString+((currentItem - firstDisplayedItem == 0)?pageScroll:0), (currentItem - firstDisplayedItem == 0)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 0)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_HIDDEN_LINE_1, (currentItem - firstDisplayedItem == 0) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 2)
        {
            LCDD_ClearRect(0, PAGE_HIDDEN_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_HIDDEN_LINE_2+PAGE_HIDDEN_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 1) ? PAGE_HIGHLIGHT_COLOR:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_HIDDEN_LINE_2, 0, (pPageParams+firstDisplayedItem+1)->pString, (currentItem - firstDisplayedItem == 1) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_HIDDEN_LINE_2, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), (pPageParams+firstDisplayedItem+1)->pString+((currentItem - firstDisplayedItem == 1)?pageScroll:0), (currentItem - firstDisplayedItem == 1)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 1)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_HIDDEN_LINE_2, (currentItem - firstDisplayedItem == 1) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 3)
        {
            LCDD_ClearRect(0, PAGE_HIDDEN_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_HIDDEN_LINE_3+PAGE_HIDDEN_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 2) ? PAGE_HIGHLIGHT_COLOR:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_HIDDEN_LINE_3, 0, (pPageParams+firstDisplayedItem+2)->pString, (currentItem - firstDisplayedItem == 2) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_HIDDEN_LINE_3, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), (pPageParams+firstDisplayedItem+2)->pString+((currentItem - firstDisplayedItem == 2)?pageScroll:0), (currentItem - firstDisplayedItem == 2)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 2)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_HIDDEN_LINE_3, (currentItem - firstDisplayedItem == 2) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 4)
        {
            LCDD_ClearRect(0, PAGE_HIDDEN_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_HIDDEN_LINE_4+PAGE_HIDDEN_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 3) ? PAGE_HIGHLIGHT_COLOR:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_HIDDEN_LINE_4, 0, (pPageParams+firstDisplayedItem+3)->pString, (currentItem - firstDisplayedItem == 3) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_HIDDEN_LINE_4, (PAGE_ARROW_NAV_X_POS-PAGE_LINE_INDENT), (pPageParams+firstDisplayedItem+3)->pString+((currentItem - firstDisplayedItem == 3)?pageScroll:0), (currentItem - firstDisplayedItem == 3)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 3)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_HIDDEN_LINE_4, (currentItem - firstDisplayedItem == 3) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = totalItems-1;
            }
            else
            {
                currentItem--;
            }

            pageScroll = 0;  // 2.4.0
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == totalItems-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

            pageScroll = 0;  // 2.4.0
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            pageCurrent = (pPageParams+currentItem)->PageHandlerIndex;

            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_SETTINGS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    // 2.4.0
    if (TIME_GetTime() >= pageTimeStamp)
    {
        pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
        pageUpdate = 1;
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pPageParams)
        {
            free(pPageParams);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

// 2.4.0
void DatePageHandler(void)
{
#define PAGE_DATE_TOTALITEMS        3
#define PAGE_DATE_BODY_Y_POS        PAGE_VALUE_Y_POS
#define PAGE_DATE_BODY_X_POS        43
#define PAGE_DATE_DAY_X_POS         28
#define PAGE_DATE_MONTH_X_POS       52
#define PAGE_DATE_YEAR_X_POS        76

    static unsigned char currentItem;
    static PAGE_PARAMETER *pParameter;
    unsigned char temp[5];

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_YEAR) && PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MONTH) && PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DAY);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_DATE ]), 0, 0, strings[STRINGS_INDEX_DATE ], 0, 0);

        if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

            currentItem = 0;

            pParameter = (PAGE_PARAMETER *)malloc(3*sizeof(PAGE_PARAMETER));

            pParameter->ParamIndex = PARAM_INDEX_DAY;
            //pParameter->Max = 31;
            pParameter->Min = 1;

            (pParameter+1)->ParamIndex = PARAM_INDEX_MONTH;
            (pParameter+1)->Max = 12;
            (pParameter+1)->Min = 1;

            (pParameter+2)->ParamIndex = PARAM_INDEX_YEAR;
            (pParameter+2)->Max = 255;
            (pParameter+2)->Min = 0;

            _draw_rectangular_for_value();
            LCDD_PutString(PAGE_DATE_BODY_X_POS, PAGE_DATE_BODY_Y_POS, 0, "/      /", PAGE_HIGHLIGHT_COLOR, 0);

            pageUpdate = 1;
        }
        else
        {
            _draw_rectangular_for_value();
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        pParameter->Max = getlastday(PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_YEAR)+2000, PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_MONTH));
        if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_DAY) > pParameter->Max)
        {
            PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_DAY, pParameter->Max);
        }

        dectoascii(&temp[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_DAY), 2);
        LCDD_PutString(PAGE_DATE_DAY_X_POS, PAGE_DATE_BODY_Y_POS, 0, &temp[0], (currentItem == 0)?0:PAGE_HIGHLIGHT_COLOR, 0);
        dectoascii(&temp[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_MONTH), 2);
        LCDD_PutString(PAGE_DATE_MONTH_X_POS, PAGE_DATE_BODY_Y_POS, 0, &temp[0], (currentItem == 1)?0:PAGE_HIGHLIGHT_COLOR, 0);
        dectoascii(&temp[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_YEAR)+2000, 4);
        LCDD_PutString(PAGE_DATE_YEAR_X_POS, PAGE_DATE_BODY_Y_POS, 0, &temp[0], (currentItem == 2)?0:PAGE_HIGHLIGHT_COLOR, 0);

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
                _adjust_param_value(pParameter+currentItem, 1);

                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
                _adjust_param_value(pParameter+currentItem, -1);

                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            if (pageAvailability)
            {
                if (currentItem == PAGE_DATE_TOTALITEMS-1)
                {
                    currentItem = 0;
                }
                else
                {
                    currentItem++;
                }

                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT)
        {
            if (pageAvailability)
            {
                if (currentItem == 0)
                {
                    currentItem = PAGE_DATE_TOTALITEMS-1;
                }
                else
                {
                    currentItem--;
                }

                pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
                _page_sub_read_parameters(pParameter, PAGE_DATE_TOTALITEMS);

//                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
                _page_sub_set_parameters(pParameter, PAGE_DATE_TOTALITEMS);

//                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }
    }
    else if (pageAvailability && (BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x3F) == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            _adjust_param_value(pParameter+currentItem, 1);

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            _adjust_param_value(pParameter+currentItem, -1);

            pageUpdate = 1;
        }
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void  TimedelayPageHandler(void)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
{
//#define PAGE_TIMEDELAY_TITLE_X_POS  36
#define PAGE_TIMEDELAY_BODY_X_POS_3 36
#define PAGE_TIMEDELAY_BODY_X_POS_2 45
#define PAGE_TIMEDELAY_BODY_Y_POS   PAGE_VALUE_Y_POS
#define PAGE_TIMEDELAY_HOUR_X_POS   15
#define PAGE_TIMEDELAY_MIN_X_POS_3  46
#define PAGE_TIMEDELAY_MIN_X_POS_2  30
#define PAGE_TIMEDELAY_SEC_X_POS_3  81
#define PAGE_TIMEDELAY_SEC_X_POS_2  65
#define PAGE_TIMEDELAY_PULSE_X_POS  40      // 2.3.0
#define PAGE_TIMEDELAY_PULSE_Y_POS  38      // 2.3.0
#define PAGE_TIMEDELAY_PULSE_WIDTH  48      // 2.3.0
#define PAGE_TIMEDELAY_PULSE_HEIGHT 32      // 2.3.0

    static unsigned char currentItem;
    static unsigned char totalItems;
    static unsigned char xHour, xMin, xSec;
    static PAGE_PARAMETER *pParameter;
    static PAGE_PARAMETER *pParamTemp;
    static unsigned char pointerOffset;
    unsigned char pHour[4];
    unsigned char pMin[3];
    unsigned char pSec[3];
    unsigned char BcdBits;
    unsigned int  timedelay = 0;
    
    
    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_HOUR) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MINUTE) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_SECOND);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        if (PARAMS_IsLightController(&sensorParams))    // 2.4.0
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_HOUR ]), 0, 0, strings[STRINGS_INDEX_HOUR ], 0, 0);
        }
        else
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_TIMEDELAY ]), 0, 0, strings[STRINGS_INDEX_TIMEDELAY ], 0, 0);
        }

//        _draw_rectangular_for_value();    // 2.3.0

        if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_READ_WRITE);
    
            currentItem = 0;
    
            pParameter = (PAGE_PARAMETER *)malloc(4*sizeof(PAGE_PARAMETER));    // 2.3.1, was 3
    
            pParameter->ParamIndex = PARAM_INDEX_HOUR;
            if( is_KNX_sensor( &sensorParams ) )
            {
                  pParameter->Max = 17;        
            }
            else
            {
                if (PARAMS_IsLightController(&sensorParams))    // 2.4.0
                  pParameter->Max = 23;
                else
                  pParameter->Max = 255;
            }
            pParameter->Min = 0;
    //        pParameter->Updated = 1;
    
            (pParameter+1)->ParamIndex = PARAM_INDEX_MINUTE;
            (pParameter+1)->Max = 59;
            (pParameter+1)->Min = 0;
    //        (pParameter+1)->Updated = 1;
    
            (pParameter+2)->ParamIndex = PARAM_INDEX_SECOND;
            (pParameter+2)->Max = 59;
    //        (pParameter+2)->Min = 0;      // 2.4.0
    //        (pParameter+2)->Updated = 1;
    
            // 2.3.1, hide one more parameter Type2 in the list in order to read to determine if Pulse available to display, not included in totalItems
            (pParameter+3)->ParamIndex = PARAM_INDEX_TYPE2;
    
            // 2.3.0
            if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_HOUR))
            {
                totalItems = 3;
                pointerOffset = 0;
            }
            else
            {
                totalItems = 2;
                pointerOffset = 1;
            }
    
            pParamTemp = pParameter+currentItem+pointerOffset;
    
            pageUpdate = 1;
        }
        else
        {
            _draw_rectangular_for_value();      // 2.3.0
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        // 2.4.0
        (pParameter+2)->Min = 0;
        if ((PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_TYPE) == PARAM_TYPE_DGT_PIR) || \
            (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_TYPE) == PARAM_TYPE_DGT_US) || \
            (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_TYPE) == PARAM_TYPE_DGT_DUAL))
        {
            if ((PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_HOUR) == 0) && \
                (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_MINUTE) == 0))
            {
                (pParameter+2)->Min = 15;
                if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_SECOND) < 15)
                {
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_SECOND, 15);
                }
            }
        }

        // 2.3.1
        LCDD_ClearRect(0, PAGE_TIMEDELAY_PULSE_Y_POS, LCD_X_LENGTH-1, PAGE_TIMEDELAY_PULSE_Y_POS+PAGE_TIMEDELAY_PULSE_HEIGHT-1, 0);

        if (PARAMS_IsPulseAvailable(&sensorParams) && \
            ((PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_HOUR) && ((PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_HOUR) | PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_MINUTE) | PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_SECOND)) == 0)) || \
             ((PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_MINUTE) | PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_SECOND)) == 0)))
        {
            LCDD_PutBitmap(PAGE_TIMEDELAY_PULSE_X_POS, PAGE_TIMEDELAY_PULSE_Y_POS, &BitmapInfo_Pulse, 0);
        }
        else
        {
            _draw_rectangular_for_value();

            if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_HOUR))
            {
                if (language == German)     // 2.4.0
                {
                    LCDD_PutString(PAGE_TIMEDELAY_BODY_X_POS_3, PAGE_TIMEDELAY_BODY_Y_POS, 0, "St     Min      Sek", PAGE_HIGHLIGHT_COLOR, 0);
                }
                else
                {
                    LCDD_PutString(PAGE_TIMEDELAY_BODY_X_POS_3, PAGE_TIMEDELAY_BODY_Y_POS, 0, "H      Min      Sec", PAGE_HIGHLIGHT_COLOR, 0);
                }
                xHour = PAGE_TIMEDELAY_HOUR_X_POS;
                xMin = PAGE_TIMEDELAY_MIN_X_POS_3;
                xSec = PAGE_TIMEDELAY_SEC_X_POS_3;
            }
            else
            {
                if (language == German)     // 2.4.0
                {
                    LCDD_PutString(PAGE_TIMEDELAY_BODY_X_POS_2, PAGE_TIMEDELAY_BODY_Y_POS, 0, "Min      Sek", PAGE_HIGHLIGHT_COLOR, 0);
                }
                else
                {
                    LCDD_PutString(PAGE_TIMEDELAY_BODY_X_POS_2, PAGE_TIMEDELAY_BODY_Y_POS, 0, "Min      Sec", PAGE_HIGHLIGHT_COLOR, 0);
                }
                xMin = PAGE_TIMEDELAY_MIN_X_POS_2;
                xSec = PAGE_TIMEDELAY_SEC_X_POS_2;
            }

            if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_HOUR))   // && (pParameter->Updated))
            {
//                pParameter->Updated = 0;
              if (PARAMS_IsLightController(&sensorParams))    // 2.4.0
                  BcdBits = 2;
              else
                  BcdBits = 3;
                
              dectoascii(&pHour[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_HOUR), BcdBits);
                LCDD_PutString(xHour, PAGE_TIMEDELAY_BODY_Y_POS, 0, &pHour[0], (currentItem == totalItems-3)?0:PAGE_HIGHLIGHT_COLOR, 0);
            }

//            if ((pParameter+1)->Updated)
            {
//                (pParameter+1)->Updated = 0;
                dectoascii(&pMin[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_MINUTE), 2);
                LCDD_PutString(xMin, PAGE_TIMEDELAY_BODY_Y_POS, 0, &pMin[0], (currentItem == totalItems-2)?0:PAGE_HIGHLIGHT_COLOR, 0);
            }

//            if ((pParameter+2)->Updated)
            {
//                (pParameter+2)->Updated = 0;
                dectoascii(&pSec[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_SECOND), 2);
                LCDD_PutString(xSec, PAGE_TIMEDELAY_BODY_Y_POS, 0, &pSec[0], (currentItem == totalItems-1)?0:PAGE_HIGHLIGHT_COLOR, 0);
            }
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
                _adjust_param_value(pParamTemp, 1);
                
                timedelay =    (unsigned int )(sensorParams.Hour.Value)*3600UL 
                             + (unsigned int )(sensorParams.Minute.Value)*60UL
                             + (unsigned int )(sensorParams.Second.Value);
                
                if( is_KNX_sensor( &sensorParams ) )
                {
                    if( timedelay >= (18UL*3600UL) )
                    {
                        sensorParams.Hour.Value = 17;
                        sensorParams.Minute.Value = 59;
                        sensorParams.Second.Value = 59;
                    }
                    else if( timedelay < 5 )
                    {
                        sensorParams.Hour.Value = 0;
                        sensorParams.Minute.Value = 0;
                        sensorParams.Second.Value = 5;                        
                    }
                }

//              pParamTemp->Updated = 1;
                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            
                _adjust_param_value(pParamTemp, -1);
                timedelay =    (unsigned int )(sensorParams.Hour.Value)*3600UL
                             + (unsigned int )(sensorParams.Minute.Value)*60UL
                             + (unsigned int )(sensorParams.Second.Value);
                
                if( is_KNX_sensor( &sensorParams ) )
                {
                    if( timedelay < 5 )
                    {
                        sensorParams.Hour.Value = 0;
                        sensorParams.Minute.Value = 0;
                        sensorParams.Second.Value = 5;
                    }
                    else if( timedelay >= (18UL*3600UL) )
                    {
                        sensorParams.Hour.Value = 17;
                        sensorParams.Minute.Value = 59;
                        sensorParams.Second.Value = 59;
                    }
                    
                    
                }
                
//              pParamTemp->Updated = 1;
                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            if (pageAvailability)
            {
            if (currentItem == totalItems-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

//            pParamTemp->Updated = 1;
            pParamTemp = pParameter+currentItem+pointerOffset;
//            pParamTemp->Updated = 1;
            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT)
        {
            if (pageAvailability)
            {
            if (currentItem == 0)
            {
                currentItem = totalItems-1;
            }
            else
            {
                currentItem--;
            }

//            pParamTemp->Updated = 1;
            pParamTemp = pParameter+currentItem+pointerOffset;
//            pParamTemp->Updated = 1;
            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
                // 2.3.1, reset Type2 parameter number in the profile for reading Line Voltage PIR and/or US sensors.
                if (PARAMS_IsLineVoltageSensor(&sensorParams) && (!PARAMS_IsZigbeeSensor(&sensorParams)) && (!PARAMS_SensorHasHF(&sensorParams)))
                {
                    PARAMS_SetParameterNumber(&sensorParams, PARAM_INDEX_TYPE2, PARAM_NUMBER_TYPE2);
                }

            _page_sub_read_parameters(pParameter, 4);       // 2.3.1, was 3

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
              if( ( pointerOffset == 0) && ( IR_ReadSensorParameter(&sensorParams, PARAM_INDEX_TYPE) == 1 ) 
                && ( PARAMS_IsLightController(&sensorParams)) ){
                   
                 if( PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_HOUR) < 24 ){  
                    _page_sub_set_parameters(pParameter+pointerOffset, totalItems);              
                }
              }
              else{
                _page_sub_set_parameters(pParameter+pointerOffset, totalItems);
              }
//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }
    else if (pageAvailability && (BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x3F) == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            _adjust_param_value(pParamTemp, 1);

//            pParamTemp->Updated = 1;
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            _adjust_param_value(pParamTemp, -1);

//            pParamTemp->Updated = 1;
            pageUpdate = 1;
        }
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void SensitivityPageHandler(void)
{
//#define PAGE_SENSITIVITY_TITLE_X_POS        36
#define PAGE_SENSITIVITY_MAXIMUMITEMS       3
#define PAGE_SENSITIVITY_NAME_X_POS         4
#define PAGE_SENSITIVITY_VALUE_X_POS        40
#define PAGE_SENSITIVITY_BRACKET_X_POS      114
#define PAGE_SENSITIVITY_BODY_Y_POS_1       18
#define PAGE_SENSITIVITY_BODY_Y_POS_DELTA   22
#define PAGE_SENSITIVITY_BODY_Y_POS_2       (PAGE_SENSITIVITY_BODY_Y_POS_1 + PAGE_SENSITIVITY_BODY_Y_POS_DELTA)
#define PAGE_SENSITIVITY_BODY_Y_POS_3       (PAGE_SENSITIVITY_BODY_Y_POS_2 + PAGE_SENSITIVITY_BODY_Y_POS_DELTA)

    static const PAGE_ITEM_RECT rect[3] = {{PAGE_SENSITIVITY_BODY_Y_POS_1-1, PAGE_SENSITIVITY_BODY_Y_POS_1+16}, {PAGE_SENSITIVITY_BODY_Y_POS_2-1, PAGE_SENSITIVITY_BODY_Y_POS_2+16}, {PAGE_SENSITIVITY_BODY_Y_POS_3-1, PAGE_SENSITIVITY_BODY_Y_POS_3+16}};
    static unsigned char previousItem;
    static unsigned char currentItem;
    static unsigned char totalItems;
    static PAGE_PARAMETER *pParameter;
    unsigned char yOffset, i;
    PAGE_PARAMETER *pTempParameter;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_HF_SENS) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_PIR_SENS) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_US_SENS);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_SENSITIVITY ]), 0, 0, strings[STRINGS_INDEX_SENSITIVITY ], 0, 0);

        if (pageAvailability)
        {
        _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

        previousItem = 0xFF;
        currentItem = 0;
        totalItems = 0;
        pParameter = (PAGE_PARAMETER *)malloc(PAGE_SENSITIVITY_MAXIMUMITEMS*sizeof(PAGE_PARAMETER));
        pTempParameter = pParameter;
        yOffset = PAGE_SENSITIVITY_BODY_Y_POS_1;
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_PIR_SENS))
        {
            totalItems++;
            pTempParameter->ParamIndex = PARAM_INDEX_PIR_SENS;
            pTempParameter->Max = 3;
            pTempParameter->Min = 0;
            pTempParameter++;
            LCDD_PutString(PAGE_SENSITIVITY_NAME_X_POS, yOffset, 0, "PIR", 0, 0);
            yOffset += PAGE_SENSITIVITY_BODY_Y_POS_DELTA;
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_US_SENS))
        {
            totalItems++;
            pTempParameter->ParamIndex = PARAM_INDEX_US_SENS;
            pTempParameter->Max = 3;
            pTempParameter->Min = 0;
            pTempParameter++;
            LCDD_PutString(PAGE_SENSITIVITY_NAME_X_POS, yOffset, 0, "US", 0, 0);
            yOffset += PAGE_SENSITIVITY_BODY_Y_POS_DELTA;
        }
        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_HF_SENS))
        {
            totalItems++;
            pTempParameter->ParamIndex = PARAM_INDEX_HF_SENS;
            pTempParameter->Max = 3;
            pTempParameter->Min = 0;
            LCDD_PutString(PAGE_SENSITIVITY_NAME_X_POS, yOffset, 0, "HF", 0, 0);
        }

        pageUpdate = 1;
        }
        else
        {
            _draw_rectangular_for_value();
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        if (currentItem != previousItem)
        {
            if (previousItem != 0xFF)
            {
                LCDD_InvertRect(0, rect[previousItem].yStart, LCD_X_LENGTH-1, rect[previousItem].yEnd, PAGE_HIGHLIGHT_COLOR);
            }
            LCDD_InvertRect(0, rect[currentItem].yStart, LCD_X_LENGTH-1, rect[currentItem].yEnd, PAGE_HIGHLIGHT_COLOR);

            previousItem = currentItem;
        }

        yOffset = PAGE_SENSITIVITY_BODY_Y_POS_1;
        for (i=0; i<totalItems; i++)
        {
            LCDD_ClearRect(PAGE_SENSITIVITY_VALUE_X_POS-4, yOffset, LCD_X_LENGTH-2, yOffset+15, 0);
            LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS, yOffset, 0, strings[STRINGS_INDEX_LOW+STRINGS_TOTAL_LANGUAGES*PARAMS_GetParameterValue(&sensorParams, (pParameter+i)->ParamIndex) ], 0, 0);      // 2.4.0
            LCDD_PutString(PAGE_SENSITIVITY_BRACKET_X_POS, yOffset, 0, "<>", 0, 0);
            yOffset += PAGE_SENSITIVITY_BODY_Y_POS_DELTA;
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            if (currentItem == 0)
            {
                currentItem = totalItems-1;
            }
            else
            {
                currentItem--;
            }

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            if (currentItem == totalItems-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter+currentItem, 1);

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter+currentItem, -1);

            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
                // Temporarily reset to read all sensitivity parameters
                pParameter->ParamIndex = PARAM_INDEX_PIR_SENS;
                (pParameter+1)->ParamIndex = PARAM_INDEX_US_SENS;
                (pParameter+2)->ParamIndex = PARAM_INDEX_HF_SENS;

                _page_sub_read_parameters(pParameter, PAGE_SENSITIVITY_MAXIMUMITEMS);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, totalItems);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void CalibrationPageHandler(void)
{
//#define PAGE_CALIBRATION_TITLE_X_POS        36
#define PAGE_CALIBRATION_BODY_X_POS         40
#define PAGE_CALIBRATION_BODY_Y_POS         PAGE_VALUE_Y_POS

    static PAGE_PARAMETER *pParameter;
    static unsigned char ConfirmPrompt;     // 0: normal display    1: prompt for confirmation
    unsigned char pDaylightlevel[10];   // 2.2.0, was 6
    static unsigned char DispWarningFlag = 0;
    
    if (pageInitialize)
    {
        pageInitialize = 0;

        ConfirmPrompt = 0;
        
        DispWarningFlag = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DAYLIGHTFACTOR) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DAYLIGHTFACTORCEN) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_NATURALLIGHTFACTOR);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_CALIBRATION ]), 0, 0, strings[STRINGS_INDEX_CALIBRATION ], 0, 0);

        _draw_rectangular_for_value();

        if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_WRITE_ONLY);

            pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

            pParameter->ParamIndex = PARAM_INDEX_DAYLIGHTLEVEL;
            pParameter->Max = 19999;
            pParameter->Min = 0;

            pageUpdate = 1;
        }
        else
        {
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }
    
    if( DispWarningFlag ){
      if (language == German){     // 2.4.0
        if( TIME_GetTime() >= pageTimeStamp ){
          LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-21, 0);       
          LCDD_PutString(0, 0, LCD_X_LENGTH, strings[STRINGS_INDEX_CALIBRATIONPROMPT ]+162, 0, 0);
          DispWarningFlag = 0;
        }
      }
    }
    if (pageUpdate)
    {
        if (ConfirmPrompt)
        {
            DispWarningFlag = 1;
            pageTimeStamp = TIME_GetTime() + 5000;

            LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);
            LCDD_PutString(0, 0, LCD_X_LENGTH, strings[STRINGS_INDEX_CALIBRATIONPROMPT ], 0, 0);
          
            memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
            pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
            pageFunckeyItems[1].pItem = (void *)strings[STRINGS_INDEX_YES ];
            pageFunckeyItems[1].itemType = 1;
            pageFunckeyItems[2].pItem = (void *)strings[STRINGS_INDEX_NO_1 ];
            pageFunckeyItems[2].itemType = 1;
            _common_funckey_initializer();
        }
        else
        {
            dectoascii(&pDaylightlevel[0], 5*PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_DAYLIGHTLEVEL), 5);
            if (PARAMS_SensorIsHFOnly(&sensorParams) )     // 2.2.0
            {
            }
            else
            {
                strcpy((char *)&pDaylightlevel[5], " Lux");
            }
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, &pDaylightlevel[0]), PAGE_CALIBRATION_BODY_Y_POS, 0, &pDaylightlevel[0], PAGE_HIGHLIGHT_COLOR, 0);   // 2.2.0
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability && (ConfirmPrompt == 0))
            {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability && (ConfirmPrompt == 0))
            {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F2)
        {
            if (pageAvailability && (ConfirmPrompt == 1))
            {
                _page_sub_set_parameters(pParameter, 1);

                pageSwap = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
                if (ConfirmPrompt)
                {
                    ConfirmPrompt = 0;

                    pageCurrent = PAGE_INDEX_ADVANCEDMODE;

                    pageSwap = 1;
                }
                else
                {
                    ConfirmPrompt = 1;

                    pageUpdate = 1;
                }
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            ConfirmPrompt = 0;

            pageCurrent = PAGE_INDEX_ADVANCEDMODE;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }
    else if (pageAvailability && (ConfirmPrompt == 0) && (BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x3F) == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void DaylightSetpointPageHandler(void)
{
//#define PAGE_DAYLIGHTSETPOINT_TITLE_X_POS   20
#define PAGE_DAYLIGHTSETPOINT_BODY_X_POS    40
#define PAGE_DAYLIGHTSETPOINT_BODY_Y_POS    PAGE_VALUE_Y_POS
#define PAGE_DAYLIGHTSETPOINT_EYE_X_POS     40      // 2.3.0
#define PAGE_DAYLIGHTSETPOINT_EYE_Y_POS     36      // 2.3.0
#define PAGE_DAYLIGHTSETPOINT_EYE_WIDTH     48      // 2.3.0
#define PAGE_DAYLIGHTSETPOINT_EYE_HEIGHT    32      // 2.3.0

    static PAGE_PARAMETER *pParameter;
    unsigned char pDaylightsetpoint[9];     // 2.2.0, was 5

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DAYLIGHTSETPOINT);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        if (PARAMS_IsLightController(&sensorParams))    // 2.4.0
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_SETPOINT ]), 0, 0, strings[STRINGS_INDEX_SETPOINT ], 0, 0);
        }
        else
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_DAYLIGHTSETPOINT ]), 0, 0, strings[STRINGS_INDEX_DAYLIGHTSETPOINT ], 0, 0);
        }

//        _draw_rectangular_for_value();    // 2.3.0

        if (pageAvailability)
        {
        _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

        pParameter = (PAGE_PARAMETER *)malloc(2*sizeof(PAGE_PARAMETER));        // 2.3.1, was 1

        pParameter->ParamIndex = PARAM_INDEX_DAYLIGHTSETPOINT;
        pParameter->Max = 0xFF;
        pParameter->Min = 0;

        // 2.3.1, hide one more parameter Type2 in the list in order to read to determine if Pulse available to display
        (pParameter+1)->ParamIndex = PARAM_INDEX_TYPE2;

        pageUpdate = 1;
        }
        else
        {
            _draw_rectangular_for_value();      // 2.3.0
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        // 2.3.0
        LCDD_ClearRect(0, PAGE_DAYLIGHTSETPOINT_EYE_Y_POS, LCD_X_LENGTH-1, PAGE_DAYLIGHTSETPOINT_EYE_Y_POS+PAGE_DAYLIGHTSETPOINT_EYE_HEIGHT-1, 0);

        if (PARAMS_IsEyeFuncAvailable(&sensorParams) && (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_DAYLIGHTSETPOINT) == 0))   // 2.3.1
        {
            for (unsigned char j=PAGE_DAYLIGHTSETPOINT_EYE_Y_POS/4; j<(PAGE_DAYLIGHTSETPOINT_EYE_Y_POS+PAGE_DAYLIGHTSETPOINT_EYE_HEIGHT)/4; j++)
            {
                for (unsigned char i=PAGE_DAYLIGHTSETPOINT_EYE_X_POS; i<PAGE_DAYLIGHTSETPOINT_EYE_X_POS+PAGE_DAYLIGHTSETPOINT_EYE_WIDTH; i++)
                {
                    lcdd.LcddFrameBuffer[j][i] = Bitmap_Eye[(j-PAGE_DAYLIGHTSETPOINT_EYE_Y_POS/4)*PAGE_DAYLIGHTSETPOINT_EYE_WIDTH+i-PAGE_DAYLIGHTSETPOINT_EYE_X_POS];
                }
            }
        }
        else
        {
            _draw_rectangular_for_value();

            dectoascii(&pDaylightsetpoint[0], 5*PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_DAYLIGHTSETPOINT), 4);
            if (PARAMS_SensorIsHFOnly(&sensorParams) )     // 2.2.0
            {
            }
            else
            {
                strcpy((char *)&pDaylightsetpoint[4], " Lux");
            }
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, &pDaylightsetpoint[0]), PAGE_DAYLIGHTSETPOINT_BODY_Y_POS, 0, &pDaylightsetpoint[0], PAGE_HIGHLIGHT_COLOR, 0);    // 2.2.0
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
                // 2.3.1, reset Type2 parameter number in the profile for reading Line Voltage PIR and/or US sensors.
                if (PARAMS_IsLineVoltageSensor(&sensorParams) && (!PARAMS_IsZigbeeSensor(&sensorParams)) && (!PARAMS_SensorHasHF(&sensorParams)))
                {
                    PARAMS_SetParameterNumber(&sensorParams, PARAM_INDEX_TYPE2, PARAM_NUMBER_TYPE2);
                }

            _page_sub_read_parameters(pParameter, 2);       // 2.3.1, was 1

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }
    else if (pageAvailability && (BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x3F) == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

//void ModePageHandler(void)
void DALISensorConfigHandler(void)
{
//#define PAGE_MODE_TITLE_X_POS       48
#define PAGE_MODE_NAME_X_POS        2
#define PAGE_MODE_VALUE_X_POS       112
#define PAGE_MODE_BODY_Y_POS_1      18
#define PAGE_MODE_BODY_Y_POS_2      40
#define PAGE_MODE_BODY_Y_POS_3      62
#define PAGE_MODE_BODY_Y_POS_4      84

    static const PAGE_ITEM_RECT rect[4] = {{PAGE_MODE_BODY_Y_POS_1-1, PAGE_MODE_BODY_Y_POS_1+16}, {PAGE_MODE_BODY_Y_POS_2-1, PAGE_MODE_BODY_Y_POS_2+16}, {PAGE_MODE_BODY_Y_POS_3-1, PAGE_MODE_BODY_Y_POS_3+16}, {PAGE_MODE_BODY_Y_POS_4-1, PAGE_MODE_BODY_Y_POS_4+16}};
//    static unsigned char previousItem;    // 2.3.0
    static unsigned char currentItem;
    static unsigned char totalItems;
    static PAGE_PARAMETER *pParameter;
    static unsigned char mode_value;
    const unsigned char possible_values[3]={0x00,0x01,0x02};    // 2.3.0
    unsigned char i;
//    const unsigned char *string;      // 2.3.0
    const unsigned char * string[]={"Daylight","Occupancy","Daylight+Occupancy"};
    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_SensorConfig);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "Sensor Configration"), 0, 0, "Sensor Configration", 0, 0);

        if (pageAvailability)
        {
          _param_page_funckey_initializer(FUNCKEY_READ_WRITE);
          
          pParameter = (PAGE_PARAMETER *)malloc(2*sizeof(PAGE_PARAMETER));        // 2.3.1, was 1
          pParameter->ParamIndex = PARAM_INDEX_SensorConfig;
          mode_value = PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_SensorConfig);
          
          // 2.3.1, hide one more parameter Type2 in the list in order to read to determine if Pulse available to display
          // (pParameter+1)->ParamIndex = PARAM_INDEX_TYPE2;
         // possible_values[totalItems] = 3;
          totalItems=3;
          currentItem = 0;
          for (i=0; i<totalItems; i++)
          {
            if (mode_value == possible_values[i])
            {
              currentItem = i;
              break;
            }
          }
          
          pageScroll = 0;  // 2.4.0
          
          pageUpdate = 1;
        }
        else
        {
            _draw_rectangular_for_value();
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
      
      for (i=0; i<totalItems; i++)
      {
        LCDD_ClearRect(0, rect[i].yStart, LCD_X_LENGTH-1, rect[i].yEnd, (i == currentItem) ? PAGE_HIGHLIGHT_COLOR:0);
        if (LCDD_PutScrollString(PAGE_MODE_NAME_X_POS, rect[i].yStart+1, (PAGE_MODE_VALUE_X_POS-2-PAGE_MODE_NAME_X_POS), string[i]+((i == currentItem)?pageScroll:0), (i == currentItem)?PAGE_HIGHLIGHT_COLOR:0, (i == currentItem)?1:0))
        {
          pageScroll++;
          pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
        }
      // LCDD_PutScrollString(PAGE_MODE_NAME_X_POS, rect[i].yStart+1, (PAGE_MODE_VALUE_X_POS-2-PAGE_MODE_NAME_X_POS), string[i], (i == currentItem)?PAGE_HIGHLIGHT_COLOR:0, (i == currentItem)?1:0);
        
        LCDD_ClearRect(PAGE_MODE_VALUE_X_POS-2, rect[i].yStart+1, LCD_X_LENGTH-2, rect[i].yEnd-1, 0);
        LCDD_PutBitmap(PAGE_MODE_VALUE_X_POS, rect[i].yStart+1, (mode_value == possible_values[i])?&BitmapInfo_Validation[1]:&BitmapInfo_Validation[0], 0);
      }
      
      pageUpdate = 0;
    }
    
    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            if (currentItem == 0)
            {
                currentItem = totalItems-1;
            }
            else
            {
                currentItem--;
            }
                        pageScroll = 0;
            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            if (currentItem == totalItems-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }
            pageScroll = 0;
            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR)
        {
            if (pageAvailability)
            {
            mode_value = possible_values[currentItem];
            PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_SensorConfig, mode_value);
            pageScroll = 0;
            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
                // 2.3.1, reset Type2 parameter number in the profile for reading Line Voltage PIR and/or US sensors.
                if (PARAMS_IsLineVoltageSensor(&sensorParams) && (!PARAMS_IsZigbeeSensor(&sensorParams)) && (!PARAMS_SensorHasHF(&sensorParams)))
                {
                    PARAMS_SetParameterNumber(&sensorParams, PARAM_INDEX_TYPE2, PARAM_NUMBER_TYPE2);
                }

            _page_sub_read_parameters(pParameter, 2);       // 2.3.1, was 1

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    // 2.4.0
    if (TIME_GetTime() >= pageTimeStamp)
    {
        pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
        pageUpdate = 1;
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void DetectionschemePageHandler(void)
{
//#define PAGE_DETECTIONSCHEME_TITLE_X_POS    20
#define PAGE_DETECTIONSCHEME_MAXIMUMITEMS   3
#define PAGE_DETECTIONSCHEME_STA_ITEMS      3
#define PAGE_DETECTIONSCHEME_CEN_ITEMS      1
#define PAGE_DETECTIONSCHEME_NAME_X_POS     1
#define PAGE_DETECTIONSCHEME_VALUE_X_POS    60
#define PAGE_DETECTIONSCHEME_BRACKET_X_POS  115
#define PAGE_DETECTIONSCHEME_BODY_Y_POS_1   18
#define PAGE_DETECTIONSCHEME_BODY_Y_POS_2   40
#define PAGE_DETECTIONSCHEME_BODY_Y_POS_3   62

    static unsigned char currentItem;
    static unsigned char totalItems;
    static PAGE_PARAMETER *pParameter;
    unsigned  short value;
    static unsigned char pageScroll_1;

    if (pageInitialize)
    {
        pageInitialize = 0;        
        pageScroll = 0;  // 2.4.0
        pageScroll_1 = 0;        
        pageAvailability =  PARAMS_IsParameterAvailable( &sensorParams, PARAM_INDEX_DETECTIONSCHEME ) || (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_INITIAL)  || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MAINTAIN) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_RETRIGGER));
        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_DETECTIONSCHEME ]), 0, 0, strings[STRINGS_INDEX_DETECTIONSCHEME ], 0, 0);
        if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_READ_WRITE);   
            currentItem = 0;   
            pParameter = (PAGE_PARAMETER *)malloc(PAGE_DETECTIONSCHEME_MAXIMUMITEMS*sizeof(PAGE_PARAMETER));            
            if( is_KNX_sensor( &sensorParams ) )
            {
                if( 0x0100 == sensorParams.Type.Value )
                {
                    totalItems = 0;
                }
                else if( 0x0101 == sensorParams.Type.Value )
                {
                    totalItems = 0;
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_INITIAL) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_INITIAL;
                        pParameter->Min = 0;
                        pParameter->Max = 1;
                        pParameter += 1;
                    }
                    
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MAINTAIN ) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_MAINTAIN;
                        pParameter->Min = 1;
                        pParameter->Max = 1;
                        pParameter += 1;
                    }
    
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_RETRIGGER ) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_RETRIGGER;
                        pParameter->Min = 0;
                        pParameter->Max = 0;
                        pParameter += 1;
                    }
                    
                    pParameter -= totalItems;
                }
                else if( 0x0102 == sensorParams.Type.Value )
                {
                    totalItems = 0;
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_INITIAL) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_INITIAL;
                        pParameter->Min = 0;
                        pParameter->Max = 1;
                        pParameter += 1;
                    }
                    
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MAINTAIN ) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_MAINTAIN;
                        pParameter->Min = 1;
                        pParameter->Max = 1;
                        pParameter += 1;
                    }
    
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_RETRIGGER ) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_RETRIGGER;
                        pParameter->Min = 0;
                        pParameter->Max = 0;
                        pParameter += 1;
                    }
                    
                    pParameter -= totalItems;
                }
                
                else if( 0x0103 == sensorParams.Type.Value )
                {
                    totalItems = 0;
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_INITIAL) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_INITIAL;
                        pParameter->Min = 0;
                        pParameter->Max = 1;
                        pParameter += 1;
                    }
                    
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MAINTAIN ) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_MAINTAIN;
                        pParameter->Min = 1;
                        pParameter->Max = 1;
                        pParameter += 1;
                    }
    
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_RETRIGGER ) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_RETRIGGER;
                        pParameter->Min = 0;
                        pParameter->Max = 0;
                        pParameter += 1;
                    }
                    
                    pParameter -= totalItems;
                }
                
                else if( 0x0104 == sensorParams.Type.Value )
                {
                    totalItems = 0;
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_INITIAL) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_INITIAL;
                        pParameter->Min = 0;
                        pParameter->Max = 4;
                        pParameter += 1;
                    }
                    
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MAINTAIN ) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_MAINTAIN;
                        pParameter->Min = 1;
                        pParameter->Max = 4;
                        pParameter += 1;
                    }
    
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_RETRIGGER ) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_RETRIGGER;
                        pParameter->Min = 0;
                        pParameter->Max = 0;
                        pParameter += 1;
                    }
                    
                    pParameter -= totalItems;
                }
                
                else if( 0x0105 == sensorParams.Type.Value )
                {
                    totalItems = 0;
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_INITIAL) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_INITIAL;
                        pParameter->Min = 0;
                        pParameter->Max = 4;
                        pParameter += 1;
                    }
                    
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MAINTAIN ) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_MAINTAIN;
                        pParameter->Min = 1;
                        pParameter->Max = 4;
                        pParameter += 1;
                    }
    
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_RETRIGGER ) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_RETRIGGER;
                        pParameter->Min = 0;
                        pParameter->Max = 0;
                        pParameter += 1;
                    }
                    
                    pParameter -= totalItems;
                }

               else if( 0x0106 == sensorParams.Type.Value )
                {
                    totalItems = 0;
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_INITIAL) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_INITIAL;
                        pParameter->Min = 0;
                        pParameter->Max = 4;
                        pParameter += 1;
                    }
                    
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MAINTAIN ) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_MAINTAIN;
                        pParameter->Min = 1;
                        pParameter->Max = 4;
                        pParameter += 1;
                    }
    
                    if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_RETRIGGER ) )
                    {
                        totalItems += 1;
                        pParameter->ParamIndex = PARAM_INDEX_RETRIGGER;
                        pParameter->Min = 0;
                        pParameter->Max = 0;
                        pParameter += 1;
                    }
                    
                    pParameter -= totalItems;
                }
                
                pageUpdate = 1;
            }
            else if(sensorParams.Type.Value>=0x108 && sensorParams.Type.Value<=0x10F)
            {
              if( 0x10A == sensorParams.Type.Value )
              {
                totalItems = 0;
                if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_INITIAL) )
                {
                  totalItems += 1;
                  pParameter->ParamIndex = PARAM_INDEX_INITIAL;
                  pParameter->Min = 0;
                  pParameter->Max = 4;
                  pParameter += 1;
                }
                
                if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MAINTAIN ) )
                {
                  totalItems += 1;
                  pParameter->ParamIndex = PARAM_INDEX_MAINTAIN;
                  pParameter->Min = 1;
                  pParameter->Max = 4;
                  pParameter += 1;
                }
                pParameter -= totalItems;
              }
              if( 0x10B == sensorParams.Type.Value )
              {
                totalItems = 0;
                if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_INITIAL) )
                {
                  totalItems += 1;
                  pParameter->ParamIndex = PARAM_INDEX_INITIAL;
                  pParameter->Min = 0;
                  pParameter->Max = 4;
                  pParameter += 1;
                }
                
                if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MAINTAIN ) )
                {
                  totalItems += 1;
                  pParameter->ParamIndex = PARAM_INDEX_MAINTAIN;
                  pParameter->Min = 1;
                  pParameter->Max = 4;
                  pParameter += 1;
                }
                pParameter -= totalItems;
              }
              if( 0x10C == sensorParams.Type.Value )
              {
                totalItems = 0;
                if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_INITIAL) )
                {
                  totalItems += 1;
                  pParameter->ParamIndex = PARAM_INDEX_INITIAL;
                  pParameter->Min = 0;
                  pParameter->Max = 4;
                  pParameter += 1;
                }
                
                if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MAINTAIN ) )
                {
                  totalItems += 1;
                  pParameter->ParamIndex = PARAM_INDEX_MAINTAIN;
                  pParameter->Min = 1;
                  pParameter->Max = 4;
                  pParameter += 1;
                }
                pParameter -= totalItems;
              }
              if( 0x10D == sensorParams.Type.Value )
              {
                totalItems = 0;
                if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_INITIAL) )
                {
                  totalItems += 1;
                  pParameter->ParamIndex = PARAM_INDEX_INITIAL;
                  pParameter->Min = 0;
                  pParameter->Max = 1;
                  pParameter += 1;
                }
                
                if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MAINTAIN ) )
                {
                  totalItems += 1;
                  pParameter->ParamIndex = PARAM_INDEX_MAINTAIN;
                  pParameter->Min = 1;
                  pParameter->Max = 1;
                  pParameter += 1;
                }
                pParameter -= totalItems;
              }
              if( 0x10E == sensorParams.Type.Value )
              {
                totalItems = 0;
                if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_INITIAL) )
                {
                  totalItems += 1;
                  pParameter->ParamIndex = PARAM_INDEX_INITIAL;
                  pParameter->Min = 0;
                  pParameter->Max = 1;
                  pParameter += 1;
                }
                
                if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MAINTAIN ) )
                {
                  totalItems += 1;
                  pParameter->ParamIndex = PARAM_INDEX_MAINTAIN;
                  pParameter->Min = 1;
                  pParameter->Max = 1;
                  pParameter += 1;
                }
                pParameter -= totalItems;
              }
              if( 0x10F == sensorParams.Type.Value )
              {
                totalItems = 0;
                if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_INITIAL) )
                {
                  totalItems += 1;
                  pParameter->ParamIndex = PARAM_INDEX_INITIAL;
                  pParameter->Min = 0;
                  pParameter->Max = 1;
                  pParameter += 1;
                }
                
                if( PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MAINTAIN ) )
                {
                  totalItems += 1;
                  pParameter->ParamIndex = PARAM_INDEX_MAINTAIN;
                  pParameter->Min = 1;
                  pParameter->Max = 1;
                  pParameter += 1;
                }
                pParameter -= totalItems;
              }
                pageUpdate = 1;
}
            else
            {    
                if (PARAMS_IsStandaloneSensor(&sensorParams))
                {
                    totalItems = PAGE_DETECTIONSCHEME_STA_ITEMS;
        
                    pParameter->ParamIndex = PARAM_INDEX_INITIAL;
                    (pParameter+1)->ParamIndex = PARAM_INDEX_MAINTAIN;
                    (pParameter+2)->ParamIndex = PARAM_INDEX_RETRIGGER;
                    if (PARAMS_SensorHasAllTech(&sensorParams))     //us+pir+hf
                    {
                        pParameter->Min = 1;
                        pParameter->Max = 7;
                        (pParameter+1)->Min = 1;
                        (pParameter+1)->Max = 7;
                        (pParameter+2)->Min = 0;              //0 is disable
                        (pParameter+2)->Max = 7;
                    }
                    else if (PARAMS_SensorHasPIRandUS(&sensorParams))//pir+us
                    {
                        pParameter->Min = 1;
                        pParameter->Max = 4;
                        (pParameter+1)->Min = 1;
                        (pParameter+1)->Max = 4;
                        (pParameter+2)->Min = 0;
                        (pParameter+2)->Max = 4;
                    }
                    else if (PARAMS_SensorHasPIRandHF(&sensorParams))//pir+hf
                    {
                        pParameter->Min = 1;
                        pParameter->Max = 7;
                        (pParameter+1)->Min = 1;
                        (pParameter+1)->Max = 7;
                        (pParameter+2)->Min = 0;
                        (pParameter+2)->Max = 7;
                    }
                    
                    else if (PARAMS_SensorHasPIR(&sensorParams))
                    {
                        pParameter->Min = 1;
                        pParameter->Max = 1;
                        (pParameter+1)->Min = 1;
                        (pParameter+1)->Max = 1;
                        (pParameter+2)->Min = 0;
                        (pParameter+2)->Max = 1;
                    }
                    else if (PARAMS_SensorHasUS(&sensorParams))
                    {
                        pParameter->Min = 2;
                        pParameter->Max = 2;
                        (pParameter+1)->Min = 2;
                        (pParameter+1)->Max = 2;
                        (pParameter+2)->Min = 0;
                        (pParameter+2)->Max = 2;
                    }
                    else if (PARAMS_SensorHasHF(&sensorParams))
                    {
                        pParameter->Min = 5;
                        pParameter->Max = 5;
                        (pParameter+1)->Min = 5;
                        (pParameter+1)->Max = 5;
                        (pParameter+2)->Min = 0;
                        (pParameter+2)->Max = 5;
                    }
        //            pParameter->Updated = 1;
        //            (pParameter+1)->Updated = 1;
        //            (pParameter+2)->Updated = 1;
        
        //            LCDD_PutString(PAGE_DETECTIONSCHEME_NAME_X_POS, PAGE_DETECTIONSCHEME_BODY_Y_POS_1, 0, strings[STRINGS_INDEX_INITIAL ], 0, 0);
        //            LCDD_PutString(PAGE_DETECTIONSCHEME_NAME_X_POS, PAGE_DETECTIONSCHEME_BODY_Y_POS_2, 0, strings[STRINGS_INDEX_MAINTAIN ], 0, 0);
        //            LCDD_PutString(PAGE_DETECTIONSCHEME_NAME_X_POS, PAGE_DETECTIONSCHEME_BODY_Y_POS_3, 0, strings[STRINGS_INDEX_RETRIGGER ], 0, 0);
                }
                
                else    // Centralized sensor
                {
                    totalItems = PAGE_DETECTIONSCHEME_CEN_ITEMS;    // Initial to the case for centralized sensor
        
                    pParameter->ParamIndex = PARAM_INDEX_DETECTIONSCHEME;   // Multiplex with PARAM_INDEX_DETECTIONSCHEME
                    pParameter->Min = 1;
                    pParameter->Max = 1;
                    if (PARAMS_SensorHasPIRandUS(&sensorParams))
                    {
                        pParameter->Max = 4;
                    }
                    else if (PARAMS_SensorHasUS(&sensorParams))
                    {
                        pParameter->Min = 2;
                        pParameter->Max = 2;
                    }
                    else if (PARAMS_SensorHasPIRandHF(&sensorParams))//pir+hf
                    {
                        pParameter->Max = 7;
                    }
        
        //            pParameter->Updated = 1;
        
        //            LCDD_PutString(PAGE_DETECTIONSCHEME_NAME_X_POS, PAGE_DETECTIONSCHEME_BODY_Y_POS_1, 0, "Detection", 0, 0);       // Todo
                }
                
        
                pageScroll = 0; // 2.4.0
        
                pageUpdate = 1;
            }
        }
        else
        {
            _draw_rectangular_for_value();
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        // 2.4.0
        LCDD_ClearRect(0, PAGE_DETECTIONSCHEME_BODY_Y_POS_1-1, LCD_X_LENGTH-1, PAGE_DETECTIONSCHEME_BODY_Y_POS_1+16, (currentItem == 0)?PAGE_HIGHLIGHT_COLOR:0);
        LCDD_ClearRect(0, PAGE_DETECTIONSCHEME_BODY_Y_POS_2-1, LCD_X_LENGTH-1, PAGE_DETECTIONSCHEME_BODY_Y_POS_2+16, (currentItem == 1)?PAGE_HIGHLIGHT_COLOR:0);
        LCDD_ClearRect(0, PAGE_DETECTIONSCHEME_BODY_Y_POS_3-1, LCD_X_LENGTH-1, PAGE_DETECTIONSCHEME_BODY_Y_POS_3+16, (currentItem == 2)?PAGE_HIGHLIGHT_COLOR:0);

        if (totalItems >= 1)
        {
            LCDD_ClearRect(PAGE_DETECTIONSCHEME_VALUE_X_POS-2, PAGE_DETECTIONSCHEME_BODY_Y_POS_1, LCD_X_LENGTH-2, PAGE_DETECTIONSCHEME_BODY_Y_POS_1+15, 0);
            if (PARAMS_IsStandaloneSensor(&sensorParams))
            {
                // 2.4.0
                if (LCDD_PutScrollString(PAGE_DETECTIONSCHEME_NAME_X_POS, PAGE_DETECTIONSCHEME_BODY_Y_POS_1, (PAGE_DETECTIONSCHEME_VALUE_X_POS-2-PAGE_DETECTIONSCHEME_NAME_X_POS), strings[STRINGS_INDEX_INITIAL ]+((currentItem == 0)?pageScroll:0), (currentItem == 0)?PAGE_HIGHLIGHT_COLOR:0, (currentItem == 0)?1:0))
                {
                    pageScroll++;
                    pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
                }
                LCDD_PutString(PAGE_DETECTIONSCHEME_VALUE_X_POS, PAGE_DETECTIONSCHEME_BODY_Y_POS_1, 0, strings[STRINGS_INDEX_DISABLED+STRINGS_TOTAL_LANGUAGES*PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_INITIAL) ], 0, 0);      // 2.4.0
            }
            else
            {
                LCDD_PutString(PAGE_DETECTIONSCHEME_VALUE_X_POS, PAGE_DETECTIONSCHEME_BODY_Y_POS_1, 0, strings[STRINGS_INDEX_DISABLED+STRINGS_TOTAL_LANGUAGES*PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_DETECTIONSCHEME) ], 0, 0);      // 2.4.0
            }
            if (pParameter->Min != pParameter->Max)
            {
                LCDD_PutString(PAGE_DETECTIONSCHEME_BRACKET_X_POS, PAGE_DETECTIONSCHEME_BODY_Y_POS_1, 0, "<>", 0, 0);
            }
        }

        if (totalItems >= 2)
        {
            LCDD_ClearRect(PAGE_DETECTIONSCHEME_VALUE_X_POS-2, PAGE_DETECTIONSCHEME_BODY_Y_POS_2, LCD_X_LENGTH-2, PAGE_DETECTIONSCHEME_BODY_Y_POS_2+15, 0);
            // 2.4.0
            if (LCDD_PutScrollString(PAGE_DETECTIONSCHEME_NAME_X_POS, PAGE_DETECTIONSCHEME_BODY_Y_POS_2, (PAGE_DETECTIONSCHEME_VALUE_X_POS-2-PAGE_DETECTIONSCHEME_NAME_X_POS), strings[STRINGS_INDEX_MAINTAIN ]+((currentItem == 1)?pageScroll:0), (currentItem == 1)?PAGE_HIGHLIGHT_COLOR:0, (currentItem == 1)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            LCDD_PutString(PAGE_DETECTIONSCHEME_VALUE_X_POS, PAGE_DETECTIONSCHEME_BODY_Y_POS_2, 0, strings[STRINGS_INDEX_DISABLED+STRINGS_TOTAL_LANGUAGES*PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_MAINTAIN) ], 0, 0);     // 2.4.0
            if ((pParameter+1)->Min != (pParameter+1)->Max)
            {
                LCDD_PutString(PAGE_DETECTIONSCHEME_BRACKET_X_POS, PAGE_DETECTIONSCHEME_BODY_Y_POS_2, 0, "<>", 0, 0);
            }
        }

        if (totalItems >= 3)
        {
            LCDD_ClearRect(PAGE_DETECTIONSCHEME_VALUE_X_POS-2, PAGE_DETECTIONSCHEME_BODY_Y_POS_3, LCD_X_LENGTH-2, PAGE_DETECTIONSCHEME_BODY_Y_POS_3+15, 0);
            // 2.4.0
            if (LCDD_PutScrollString(PAGE_DETECTIONSCHEME_NAME_X_POS, PAGE_DETECTIONSCHEME_BODY_Y_POS_3, (PAGE_DETECTIONSCHEME_VALUE_X_POS-2-PAGE_DETECTIONSCHEME_NAME_X_POS), strings[STRINGS_INDEX_RETRIGGER ]+((currentItem == 2)?pageScroll:0), (currentItem == 2)?PAGE_HIGHLIGHT_COLOR:0, (currentItem == 2)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
            if (language >= Italian){      // 2.4.0, was Spanish            
              if (LCDD_PutScrollString(PAGE_DETECTIONSCHEME_VALUE_X_POS, PAGE_DETECTIONSCHEME_BODY_Y_POS_3, LCD_X_LENGTH-PAGE_DETECTIONSCHEME_VALUE_X_POS-12, strings[STRINGS_INDEX_DISABLED+STRINGS_TOTAL_LANGUAGES*PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_RETRIGGER) ]+pageScroll_1, 0, 1))
              {
                pageScroll_1++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
              }
            }
            else{
              LCDD_PutString(PAGE_DETECTIONSCHEME_VALUE_X_POS, PAGE_DETECTIONSCHEME_BODY_Y_POS_3, 0, strings[STRINGS_INDEX_DISABLED+STRINGS_TOTAL_LANGUAGES*PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_RETRIGGER) ], 0, 0);    // 2.4.0
            }
            if ((pParameter+2)->Min != (pParameter+2)->Max)
            {
                LCDD_PutString(PAGE_DETECTIONSCHEME_BRACKET_X_POS, PAGE_DETECTIONSCHEME_BODY_Y_POS_3, 0, "<>", 0, 0);
            }
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            if (currentItem == totalItems-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }
            pageScroll_1 = 0;
            pageScroll = 0;  // 2.4.0
            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            if (currentItem == 0)
            {
                currentItem = totalItems-1;
            }
            else
            {
                currentItem--;
            }

            pageScroll_1 = 0;
            pageScroll = 0;  // 2.4.0
            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT) || (BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT))
        {
            pageScroll_1 = 0;
            if (pageAvailability)
            {
                if( is_KNX_sensor( &sensorParams ) ) 
                {
                  
                    if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
                    {
                        _adjust_param_value(pParameter+currentItem, 1);
                    }
                    else
                    {
                        _adjust_param_value(pParameter+currentItem, -1);
                    }
                  
//                    if (PARAMS_GetParameterValue(&sensorParams, (pParameter+currentItem)->ParamIndex) == (pParameter+currentItem)->Min)
//                    {
//                        PARAMS_SetParameterValue(&sensorParams, (pParameter+currentItem)->ParamIndex, (pParameter+currentItem)->Max);
//                    }
//                    else
//                    {
//                        PARAMS_SetParameterValue(&sensorParams, (pParameter+currentItem)->ParamIndex, (pParameter+currentItem)->Min);
//                    }
                  
                }
                else if(sensorParams.Type.Value>=0x108 && sensorParams.Type.Value<=0x10F)
                {
                  if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
                  {
                    _adjust_param_value(pParameter+currentItem, 1);
                  }
                  else
                  {
                    _adjust_param_value(pParameter+currentItem, -1);
                  }
                  
                  
                }
                else
                {
                    if (PARAMS_SensorHasAllTech(&sensorParams) || PARAMS_SensorHasPIRandUS(&sensorParams)){
                        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT){
                            _adjust_param_value(pParameter+currentItem, 1);
                        }
                        else{
                            _adjust_param_value(pParameter+currentItem, -1);
                        }
                    }
                    else if (PARAMS_SensorHasPIRandHF(&sensorParams)){
                        value = PARAMS_GetParameterValue(&sensorParams, (pParameter+currentItem)->ParamIndex);
                        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT){
                           switch( value ){
                           case 0:
                             value = 1;
                             break;                   
                           case 1:
                             value = 5;
                             break;
                           case 5:
                             value = 6;
                             break;
                           case 6:
                             value = 7;
                             break;
                           case 7:
                             value = (pParameter+currentItem)->Min;
                             break;                   
                           }
                        }
                        else{
                           switch( value ){
                           case 7:
                             value = 6;
                             break;
                           case 6:
                               value = 5;
                             break;
                           case 5:
                             value = 1;
                             break;
                           case 1:
                               if( 0 == (pParameter+currentItem)->Min) 
                                value = 0;
                               else
                                 value = 7;
                             break;
                           case  0:
                              value = 7;   
                              break;
                           }
                        }
                        PARAMS_SetParameterValue(&sensorParams, (pParameter+currentItem)->ParamIndex, value);
                  }
                  else{
                    if (PARAMS_GetParameterValue(&sensorParams, (pParameter+currentItem)->ParamIndex) == (pParameter+currentItem)->Min)
                    {
                        PARAMS_SetParameterValue(&sensorParams, (pParameter+currentItem)->ParamIndex, (pParameter+currentItem)->Max);
                    }
                    else
                    {
                        PARAMS_SetParameterValue(&sensorParams, (pParameter+currentItem)->ParamIndex, (pParameter+currentItem)->Min);
                    }
                  }
                }
              
              pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
            _page_sub_read_parameters(pParameter, totalItems);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, totalItems);   // 1.0.24, was _page_sub_set_parameters(pParameter, 3);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    // 2.4.0
    if (TIME_GetTime() >= pageTimeStamp)
    {
        pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
        pageUpdate = 1;
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void AlertPageHandler(void)
{
//#define PAGE_ALERT_TITLE_X_POS    48
#define PAGE_ALERT_NAME_X_POS     2
#define PAGE_ALERT_VALUE_X_POS    106
#define PAGE_ALERT_BODY_Y_POS     18

    static PAGE_PARAMETER *pParameter;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_ALERT);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_ALERT ]), 0, 0, strings[STRINGS_INDEX_ALERT ], 0, 0);

        if (pageAvailability)
        {
        _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

        pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

        pParameter->ParamIndex = PARAM_INDEX_ALERT;

        LCDD_PutString(PAGE_ALERT_NAME_X_POS, PAGE_ALERT_BODY_Y_POS, 0, strings[STRINGS_INDEX_AUDIBLE ], 0, 0);
        LCDD_InvertRect(0, PAGE_ALERT_BODY_Y_POS-1, LCD_X_LENGTH-1, PAGE_ALERT_BODY_Y_POS+16, PAGE_HIGHLIGHT_COLOR);

        pageUpdate = 1;
        }
        else
        {
            _draw_rectangular_for_value();
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        LCDD_ClearRect(PAGE_ALERT_VALUE_X_POS-2, PAGE_ALERT_BODY_Y_POS, LCD_X_LENGTH-2, PAGE_ALERT_BODY_Y_POS+15, 0);
        if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_ALERT) == 2)    //2 refers to audible alert
        {
            LCDD_PutBitmap(_figure_icon_center_pos(PAGE_ALERT_VALUE_X_POS, LCD_X_LENGTH-2, BitmapInfo_Validation[1].Width), PAGE_ALERT_BODY_Y_POS, &BitmapInfo_Validation[1], 0);
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR)
        {
            if (pageAvailability)
            {
            if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_ALERT) == 2)
            {
                PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_ALERT, 0);
            }
            else
            {
                PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_ALERT, 2);
            }

            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
            _page_sub_read_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void StandbylevelPageHandler(void)
{
#define PAGE_STANDBYLEVEL_BODY_Y_POS    PAGE_VALUE_Y_POS

    static PAGE_PARAMETER *pParameter;
    unsigned char pStandbylevel[6], i;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_STANDBYLEVEL);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_STANDBYLEVEL ]), 0, 0, strings[STRINGS_INDEX_STANDBYLEVEL ], 0, 0);

        _draw_rectangular_for_value();

        if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

            pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

            pParameter->ParamIndex = PARAM_INDEX_STANDBYLEVEL;
            pParameter->Max = sizeof(valuesStandbyLevel)/sizeof(unsigned short)-1;
            pParameter->Min = 0;

            pageUpdate = 1;
        }
        else
        {
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        _draw_rectangular_for_value();

        memset(&pStandbylevel[0], 0, sizeof(pStandbylevel));
        i = dectoascii(&pStandbylevel[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYLEVEL), 0);
        pStandbylevel[i++] = ' ';
        pStandbylevel[i] = '%';
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, &pStandbylevel[0]), PAGE_STANDBYLEVEL_BODY_Y_POS, 0, &pStandbylevel[0], PAGE_HIGHLIGHT_COLOR, 0);

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
                if (valuesStandbyLevel[pParameter->Max] <= PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYLEVEL))
                {
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_STANDBYLEVEL, valuesStandbyLevel[pParameter->Min]);
                }
                else
                {
                    for (i=pParameter->Min; i<=pParameter->Max; i++)
                    {
                        if (valuesStandbyLevel[i] > PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYLEVEL))
                        {
                            break;
                        }
                    }
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_STANDBYLEVEL, valuesStandbyLevel[i]);
                }

                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
                if (valuesStandbyLevel[pParameter->Min] >= PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYLEVEL))
                {
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_STANDBYLEVEL, valuesStandbyLevel[pParameter->Max]);
                }
                else
                {
                    for (i=pParameter->Max; i>=pParameter->Min; i--)
                    {
                        if (valuesStandbyLevel[i] < PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYLEVEL))
                        {
                            break;
                        }
                    }
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_STANDBYLEVEL, valuesStandbyLevel[i]);
                }

                pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
            _page_sub_read_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }
    else if (pageAvailability && (BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x3F) == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (valuesStandbyLevel[pParameter->Max] <= PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYLEVEL))
            {
                PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_STANDBYLEVEL, valuesStandbyLevel[pParameter->Min]);
            }
            else
            {
                for (i=pParameter->Min; i<=pParameter->Max; i++)
                {
                    if (valuesStandbyLevel[i] > PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYLEVEL))
                    {
                        break;
                    }
                }
                PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_STANDBYLEVEL, valuesStandbyLevel[i]);
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (valuesStandbyLevel[pParameter->Min] >= PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYLEVEL))
            {
                PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_STANDBYLEVEL, valuesStandbyLevel[pParameter->Max]);
            }
            else
            {
                for (i=pParameter->Max; i>=pParameter->Min; i--)
                {
                    if (valuesStandbyLevel[i] < PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYLEVEL))
                    {
                        break;
                    }
                }
                PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_STANDBYLEVEL, valuesStandbyLevel[i]);
            }

            pageUpdate = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void StandbydelayPageHandler(void)
{
#define PAGE_STANDBYDELAY_BODY_X_POS    40
#define PAGE_STANDBYDELAY_BODY_Y_POS    PAGE_VALUE_Y_POS

    static PAGE_PARAMETER *pParameter;
    unsigned char pStandbydelay[20], i;
    unsigned short tempvalue;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_STANDBYDELAY);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_STANDBYDELAY ]), 0, 0, strings[STRINGS_INDEX_STANDBYDELAY ], 0, 0);

        _draw_rectangular_for_value();

        if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

            pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

            pParameter->ParamIndex = PARAM_INDEX_STANDBYDELAY;
            pParameter->Max = sizeof(valuesStandbyDelay)/sizeof(unsigned short)-1;
            pParameter->Min = 0;

            pageUpdate = 1;
        }
        else
        {
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        _draw_rectangular_for_value();

        memset(&pStandbydelay[0], 0, sizeof(pStandbydelay));
        tempvalue = PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYDELAY);
        if (tempvalue == 0)
        {
            strcpy((char *)&pStandbydelay[0], (char const *)strings[STRINGS_INDEX_NOSTANDBY ]);
        }
        else if (tempvalue == 255)
        {
            strcpy((char *)&pStandbydelay[0], (char const *)strings[STRINGS_INDEX_UNLIMITED ]);
        }
        else if (tempvalue <= 60)
        {
            i = dectoascii(&pStandbydelay[0], tempvalue, 0);
            pStandbydelay[i++] = ' ';
            pStandbydelay[i] = 's';
        }
        else if (tempvalue <= 120)
        {
            tempvalue -= 60;
            i = dectoascii(&pStandbydelay[0], tempvalue, 0);
            pStandbydelay[i++] = ' ';
            pStandbydelay[i++] = 'm';
            pStandbydelay[i] = 'n';
        }
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, &pStandbydelay[0]), PAGE_STANDBYDELAY_BODY_Y_POS, 0, &pStandbydelay[0], PAGE_HIGHLIGHT_COLOR, 0);

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
                if (valuesStandbyDelay[pParameter->Max] <= PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYDELAY))
                {
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_STANDBYDELAY, valuesStandbyDelay[pParameter->Min]);
                }
                else
                {
                    for (i=pParameter->Min; i<=pParameter->Max; i++)
                    {
                        if (valuesStandbyDelay[i] > PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYDELAY))
                        {
                            break;
                        }
                    }
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_STANDBYDELAY, valuesStandbyDelay[i]);
                }

                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
                if (valuesStandbyDelay[pParameter->Min] >= PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYDELAY))
                {
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_STANDBYDELAY, valuesStandbyDelay[pParameter->Max]);
                }
                else
                {
                    for (i=pParameter->Max; i>=pParameter->Min; i--)
                    {
                        if (valuesStandbyDelay[i] < PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYDELAY))
                        {
                            break;
                        }
                    }
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_STANDBYDELAY, valuesStandbyDelay[i]);
                }

                pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
            _page_sub_read_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }
    else if (pageAvailability && (BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x3F) == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (valuesStandbyDelay[pParameter->Max] <= PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYDELAY))
            {
                PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_STANDBYDELAY, valuesStandbyDelay[pParameter->Min]);
            }
            else
            {
                for (i=pParameter->Min; i<=pParameter->Max; i++)
                {
                    if (valuesStandbyDelay[i] > PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYDELAY))
                    {
                        break;
                    }
                }
                PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_STANDBYDELAY, valuesStandbyDelay[i]);
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (valuesStandbyDelay[pParameter->Min] >= PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYDELAY))
            {
                PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_STANDBYDELAY, valuesStandbyDelay[pParameter->Max]);
            }
            else
            {
                for (i=pParameter->Max; i>=pParameter->Min; i--)
                {
                    if (valuesStandbyDelay[i] < PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STANDBYDELAY))
                    {
                        break;
                    }
                }
                PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_STANDBYDELAY, valuesStandbyDelay[i]);
            }

            pageUpdate = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void SendconditionPageHandler(void)
{
#define PAGE_SENDCONDITION_TOTAL_ITEMS      4
#define PAGE_SENDCONDITION_NAME_X_POS        2
#define PAGE_SENDCONDITION_NAME_MAX_LENGTH  108
#define PAGE_SENDCONDITION_VALUE_X_POS       112
#define PAGE_SENDCONDITION_BODY_Y_POS_1      18
#define PAGE_SENDCONDITION_BODY_Y_POS_2      40
#define PAGE_SENDCONDITION_BODY_Y_POS_3      62
#define PAGE_SENDCONDITION_BODY_Y_POS_4      84

    static const PAGE_ITEM_RECT rect[4] = {{PAGE_SENDCONDITION_BODY_Y_POS_1-1, PAGE_SENDCONDITION_BODY_Y_POS_1+16}, {PAGE_SENDCONDITION_BODY_Y_POS_2-1, PAGE_SENDCONDITION_BODY_Y_POS_2+16}, {PAGE_SENDCONDITION_BODY_Y_POS_3-1, PAGE_SENDCONDITION_BODY_Y_POS_3+16}, {PAGE_SENDCONDITION_BODY_Y_POS_4-1, PAGE_SENDCONDITION_BODY_Y_POS_4+16}};
    static unsigned char previousItem;
    static unsigned char currentItem;
    static PAGE_PARAMETER *pParameter;
    unsigned char i;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_SENDCONDITION);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_SENDCONDITION ]), 0, 0, strings[STRINGS_INDEX_SENDCONDITION ], 0, 0);

        if (pageAvailability)
        {
        _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

        pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));
        pParameter->ParamIndex = PARAM_INDEX_SENDCONDITION;
        pParameter->Max = 3;
        pParameter->Min = 0;

        previousItem = 0xFF;
        currentItem = PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_SENDCONDITION);

        LCDD_PutString(PAGE_SENDCONDITION_NAME_X_POS, rect[0].yStart+1, PAGE_SENDCONDITION_NAME_MAX_LENGTH, strings[STRINGS_INDEX_CYCLICAL ], 0, 0);
        LCDD_PutString(PAGE_SENDCONDITION_NAME_X_POS, rect[1].yStart+1, PAGE_SENDCONDITION_NAME_MAX_LENGTH, strings[STRINGS_INDEX_ONCHANGE ], 0, 0);
        LCDD_PutString(PAGE_SENDCONDITION_NAME_X_POS, rect[2].yStart+1, PAGE_SENDCONDITION_NAME_MAX_LENGTH, strings[STRINGS_INDEX_ONREQUEST ], 0, 0);
        LCDD_PutString(PAGE_SENDCONDITION_NAME_X_POS, rect[3].yStart+1, PAGE_SENDCONDITION_NAME_MAX_LENGTH, strings[STRINGS_INDEX_CHANGEORCYCLICAL ], 0, 0);

        pageUpdate = 1;
        }
        else
        {
            _draw_rectangular_for_value();
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        if (currentItem != previousItem)
        {
            if (previousItem != 0xFF)
            {
                LCDD_InvertRect(0, rect[previousItem].yStart, LCD_X_LENGTH-1, rect[previousItem].yEnd, PAGE_HIGHLIGHT_COLOR);
            }
            LCDD_InvertRect(0, rect[currentItem].yStart, LCD_X_LENGTH-1, rect[currentItem].yEnd, PAGE_HIGHLIGHT_COLOR);

            previousItem = currentItem;
        }

        for (i=0; i<PAGE_SENDCONDITION_TOTAL_ITEMS; i++)
        {
            LCDD_ClearRect(PAGE_SENDCONDITION_VALUE_X_POS-2, rect[i].yStart+1, LCD_X_LENGTH-2, rect[i].yEnd-1, 0);
            LCDD_PutBitmap(PAGE_SENDCONDITION_VALUE_X_POS, rect[i].yStart+1, (i == PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_SENDCONDITION))?&BitmapInfo_Validation[1]:&BitmapInfo_Validation[0], 0);
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            if (currentItem == 0)
            {
                currentItem = PAGE_SENDCONDITION_TOTAL_ITEMS-1;
            }
            else
            {
                currentItem--;
            }

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            if (currentItem == PAGE_SENDCONDITION_TOTAL_ITEMS-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR)
        {
            if (pageAvailability)
            {
            PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_SENDCONDITION, currentItem);

            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
            _page_sub_read_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void DeadbandPageHandler(void)
{
#define PAGE_DEADBAND_BODY_Y_POS    PAGE_VALUE_Y_POS

    static PAGE_PARAMETER *pParameter;
    unsigned char pDeadband[8], i;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DEADBAND);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_DEADBAND ]), 0, 0, strings[STRINGS_INDEX_DEADBAND ], 0, 0);

        if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

            pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

            pParameter->ParamIndex = PARAM_INDEX_DEADBAND;
            pParameter->Max = 100;
            pParameter->Min = 1;

            pageUpdate = 1;
        }
        else
        {
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        _draw_rectangular_for_value();

        memset(&pDeadband[0], 0, sizeof(pDeadband));
        i = 0;
        pDeadband[i++] = 177;     // +/-
        pDeadband[i++] = ' ';
        i += dectoascii(&pDeadband[2], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_DEADBAND), 0);
        pDeadband[i++] = ' ';
        pDeadband[i] = '%';
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, &pDeadband[0]), PAGE_DEADBAND_BODY_Y_POS, 0, &pDeadband[0], PAGE_HIGHLIGHT_COLOR, 0);

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
            _page_sub_read_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }
    else if (pageAvailability && (BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x3F) == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void TimebasePageHandler(void)
{
#define PAGE_TIMEBASE_BODY_Y_POS    PAGE_VALUE_Y_POS

    static PAGE_PARAMETER *pParameter;
    unsigned char pTimebase[7], i;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_TIMEBASE);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_TIMEBASE ]), 0, 0, strings[STRINGS_INDEX_TIMEBASE ], 0, 0);

        if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

            pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

            pParameter->ParamIndex = PARAM_INDEX_TIMEBASE;
            pParameter->Max = 59;
            pParameter->Min = 1;

            pageUpdate = 1;
        }
        else
        {
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        _draw_rectangular_for_value();

        memset(&pTimebase[0], 0, sizeof(pTimebase));
        i = dectoascii(&pTimebase[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_TIMEBASE), 0);
        pTimebase[i++] = ' ';
        pTimebase[i++] = 'M';
        pTimebase[i++] = 'i';
        pTimebase[i] = 'n';
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, &pTimebase[0]), PAGE_TIMEBASE_BODY_Y_POS, 0, &pTimebase[0], PAGE_HIGHLIGHT_COLOR, 0);

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
            _page_sub_read_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }
    else if (pageAvailability && (BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x3F) == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void BandwidthPageHandler(void)
{
#define PAGE_BANDWIDTH_BODY_Y_POS    PAGE_VALUE_Y_POS

    static PAGE_PARAMETER *pParameter;
    unsigned char pBandwidth[12], i;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_BANDWIDTH);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_BANDWIDTH ]), 0, 0, strings[STRINGS_INDEX_BANDWIDTH ], 0, 0);

        if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

            pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

            pParameter->ParamIndex = PARAM_INDEX_BANDWIDTH;
            pParameter->Max = 255;
            pParameter->Min = 1;

            pageUpdate = 1;
        }
        else
        {
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        _draw_rectangular_for_value();

        memset(&pBandwidth[0], 0, sizeof(pBandwidth));
        i = dectoascii(&pBandwidth[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_BANDWIDTH), 0);
        pBandwidth[i++] = ' ';
        pBandwidth[i++] = 'M';
        pBandwidth[i++] = 'e';
        pBandwidth[i++] = 's';
        pBandwidth[i++] = '/';
        pBandwidth[i++] = 'M';
        pBandwidth[i++] = 'i';
        pBandwidth[i] = 'n';
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, &pBandwidth[0]), PAGE_BANDWIDTH_BODY_Y_POS, 0, &pBandwidth[0], PAGE_HIGHLIGHT_COLOR, 0);

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
            _page_sub_read_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }
    else if (pageAvailability && (BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x3F) == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

// 2.4.0
void SummerhourPageHandler(void)
{
#define PAGE_SUMMERHOUR_MAXIMUMITEMS        5
#define PAGE_SUMMERHOUR_RECT_GEO_X_START    10
#define PAGE_SUMMERHOUR_RECT_GEO_X_END      (LCD_X_LENGTH - 1 - PAGE_SUMMERHOUR_RECT_GEO_X_START)
#define PAGE_SUMMERHOUR_RECT_GEO_Y_START    20
#define PAGE_SUMMERHOUR_RECT_GEO_Y_END      (PAGE_SUMMERHOUR_RECT_GEO_Y_START + 19)
#define PAGE_SUMMERHOUR_GEO_X_POS           (PAGE_SUMMERHOUR_RECT_GEO_X_START + 4)
#define PAGE_SUMMERHOUR_GEO_Y_POS           (PAGE_SUMMERHOUR_RECT_GEO_Y_START + 2)
#define PAGE_SUMMERHOUR_BRACKET_X_POS       (PAGE_SUMMERHOUR_RECT_GEO_X_END - 4 - 6 - 6)
#define PAGE_SUMMERHOUR_BRACKET_Y_POS       PAGE_SUMMERHOUR_GEO_Y_POS
#define PAGE_SUMMERHOUR_DAY_VALUES_Y_POS    PAGE_VALUE_Y_POS
#define PAGE_SUMMERHOUR_STARTDAY_X_POS      20
#define PAGE_SUMMERHOUR_SLASH_1_X_POS       33
#define PAGE_SUMMERHOUR_STARTMONTH_X_POS    40
#define PAGE_SUMMERHOUR_ENDDAY_X_POS        75
#define PAGE_SUMMERHOUR_SLASH_2_X_POS       88
#define PAGE_SUMMERHOUR_ENDMONTH_X_POS      95
#define PAGE_SUMMERHOUR_DAY_TEXTS_Y_POS     64
#define PAGE_SUMMERHOUR_CURSORFLASH        500

    static unsigned char currentItem;
    static unsigned char totalItems;
    static PAGE_PARAMETER *pParameter;
    static const unsigned char geo_values[4] = {0, 1, 2, 255};      // 0: No choice, 1: Europe, 2: USA, 255: None
    static unsigned char geo_values_idx;
    static unsigned char invertCursor;
    static unsigned int timeStamp;
    unsigned char temp[2];


    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_SUMMERHOUR);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_SUMMERHOUR ]), 0, 0, strings[STRINGS_INDEX_SUMMERHOUR ], 0, 0);

        if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

            currentItem = 0;

            pParameter = (PAGE_PARAMETER *)malloc(PAGE_SUMMERHOUR_MAXIMUMITEMS*sizeof(PAGE_PARAMETER));

            pParameter->ParamIndex = PARAM_INDEX_SUMMERHOUR;

            (pParameter+1)->ParamIndex = PARAM_INDEX_STARTDAY;
            (pParameter+1)->Min = 1;

            (pParameter+2)->ParamIndex = PARAM_INDEX_STARTMONTH;
            (pParameter+2)->Max = 12;
            (pParameter+2)->Min = 1;

            (pParameter+3)->ParamIndex = PARAM_INDEX_ENDDAY;
            (pParameter+3)->Min = 1;

            (pParameter+4)->ParamIndex = PARAM_INDEX_ENDMONTH;
            (pParameter+4)->Max = 12;
            (pParameter+4)->Min = 1;

            for (geo_values_idx = 0; geo_values_idx < sizeof(geo_values); geo_values_idx++)
            {
                if (geo_values[geo_values_idx] == PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_SUMMERHOUR))
                {
                    break;
                }
            }

            invertCursor = 1;

            timeStamp = TIME_GetTime() + PAGE_SUMMERHOUR_CURSORFLASH;

            pageUpdate = 1;
        }
        else
        {
            _draw_rectangular_for_value();
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        totalItems = 1;
        if (geo_values_idx == 0)     // Geographic location is no choice
        {
            totalItems = PAGE_SUMMERHOUR_MAXIMUMITEMS;
        }

        (pParameter+1)->Max = getlastday(2012, PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STARTMONTH));
        if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STARTDAY) > (pParameter+1)->Max)
        {
            PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_STARTDAY, (pParameter+1)->Max);
        }

        (pParameter+3)->Max = getlastday(2012, PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_ENDMONTH));
        if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_ENDDAY) > (pParameter+3)->Max)
        {
            PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_ENDDAY, (pParameter+3)->Max);
        }

        LCDD_ClearRect(0, 16, LCD_X_LENGTH-1, LCD_Y_LENGTH-25, 0);

        LCDD_ClearRect(PAGE_SUMMERHOUR_RECT_GEO_X_START, PAGE_SUMMERHOUR_RECT_GEO_Y_START, PAGE_SUMMERHOUR_RECT_GEO_X_END, PAGE_SUMMERHOUR_RECT_GEO_Y_END, (currentItem == 0)?PAGE_HIGHLIGHT_COLOR:0);
        LCDD_ClearRect(PAGE_SUMMERHOUR_RECT_GEO_X_START+1, PAGE_SUMMERHOUR_RECT_GEO_Y_START+1, PAGE_SUMMERHOUR_RECT_GEO_X_END-1, PAGE_SUMMERHOUR_RECT_GEO_Y_END-1, 0);
        LCDD_PutString(PAGE_SUMMERHOUR_GEO_X_POS, PAGE_SUMMERHOUR_GEO_Y_POS, 0, strings[STRINGS_INDEX_NOCHOICE+geo_values_idx*STRINGS_TOTAL_LANGUAGES ], 0, 0);
        if (currentItem == 0)
        {
            LCDD_PutString(PAGE_SUMMERHOUR_BRACKET_X_POS, PAGE_SUMMERHOUR_BRACKET_Y_POS, 0, "<>", 0, 0);
        }

        if (totalItems > 1)
        {
            _draw_rectangular_for_value();

            dectoascii(&temp[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STARTDAY), 2);
            LCDD_PutString(PAGE_SUMMERHOUR_STARTDAY_X_POS, PAGE_SUMMERHOUR_DAY_VALUES_Y_POS, 0, &temp[0], ((currentItem == 1)&&invertCursor)?0:PAGE_HIGHLIGHT_COLOR, 0);
            LCDD_PutString(PAGE_SUMMERHOUR_SLASH_1_X_POS, PAGE_SUMMERHOUR_DAY_VALUES_Y_POS, 0, "/", PAGE_HIGHLIGHT_COLOR, 0);
            dectoascii(&temp[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STARTMONTH), 2);
            LCDD_PutString(PAGE_SUMMERHOUR_STARTMONTH_X_POS, PAGE_SUMMERHOUR_DAY_VALUES_Y_POS, 0, &temp[0], ((currentItem == 2)&&invertCursor)?0:PAGE_HIGHLIGHT_COLOR, 0);

            dectoascii(&temp[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_ENDDAY), 2);
            LCDD_PutString(PAGE_SUMMERHOUR_ENDDAY_X_POS, PAGE_SUMMERHOUR_DAY_VALUES_Y_POS, 0, &temp[0], ((currentItem == 3)&&invertCursor)?0:PAGE_HIGHLIGHT_COLOR, 0);
            LCDD_PutString(PAGE_SUMMERHOUR_SLASH_2_X_POS, PAGE_SUMMERHOUR_DAY_VALUES_Y_POS, 0, "/", PAGE_HIGHLIGHT_COLOR, 0);
            dectoascii(&temp[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_ENDMONTH), 2);
            LCDD_PutString(PAGE_SUMMERHOUR_ENDMONTH_X_POS, PAGE_SUMMERHOUR_DAY_VALUES_Y_POS, 0, &temp[0], ((currentItem == 4)&&invertCursor)?0:PAGE_HIGHLIGHT_COLOR, 0);

            LCDD_PutString(_figure_string_center_x_pos(PAGE_SUMMERHOUR_RECT_GEO_X_START, LCD_X_LENGTH/2-1, strings[STRINGS_INDEX_STARTDAY ]), PAGE_SUMMERHOUR_DAY_TEXTS_Y_POS, 0, strings[STRINGS_INDEX_STARTDAY ], 0, 0);
            LCDD_PutString(_figure_string_center_x_pos(LCD_X_LENGTH/2, PAGE_SUMMERHOUR_RECT_GEO_X_END, strings[STRINGS_INDEX_ENDDAY ]), PAGE_SUMMERHOUR_DAY_TEXTS_Y_POS, 0, strings[STRINGS_INDEX_ENDDAY ], 0, 0);
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
                if (currentItem == 0)
                {
                    currentItem = totalItems - 1;
                }
                else
                {
                    _adjust_param_value(pParameter+currentItem, 1);
                    // according to [88230 0003370] on mantis, this modification need rollback because the reported
                    // bug was indetified as not a bug.
//                    if( false == summer_hour_check() )
//                    {
//                        _adjust_param_value(pParameter+currentItem, -1);
//                    }
                    
                }

                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
                if (currentItem == 0)
                {
                    if (totalItems > 1)
                    {
                        currentItem = 1;
                    }
                }
                else
                {
                    _adjust_param_value(pParameter+currentItem, -1);
                    // according to [88230 0003370] on mantis, this modification need rollback because the reported
                    // bug was indetified as not a bug.
//                    if( false == summer_hour_check() ) 
//                    {
//                        _adjust_param_value(pParameter+currentItem, 1 );
//                    }
                }

                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            if (pageAvailability)
            {
                if (currentItem == 0)
                {
                    if (geo_values_idx == sizeof(geo_values) - 1)
                    {
                        geo_values_idx = 0;
                    }
                    else
                    {
                        geo_values_idx++;
                    }
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_SUMMERHOUR, geo_values[geo_values_idx]);
                }
                else
                {
                    if (currentItem == totalItems - 1)
                    {
                        currentItem = 0;
                    }
                    else
                    {
                        currentItem++;
                    }
                }

                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT)
        {
            if (pageAvailability)
            {
                if (currentItem == 0)
                {
                    if (geo_values_idx == 0)
                    {
                        geo_values_idx = sizeof(geo_values) - 1;
                    }
                    else
                    {
                        geo_values_idx--;
                    }
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_SUMMERHOUR, geo_values[geo_values_idx]);
                }
                else
                {
                    if (currentItem == 1)
                    {
                        currentItem = 0;
                    }
                    else
                    {
                        currentItem--;
                    }
                }

                pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
                _page_sub_read_parameters(pParameter, PAGE_SUMMERHOUR_MAXIMUMITEMS);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
                _page_sub_set_parameters(pParameter, totalItems);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }
    else if (pageAvailability && (BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x3F) == 0))
    {
        if (currentItem > 0)
        {
            if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
            {
                _adjust_param_value(pParameter+currentItem, 1);
                  // according to [88230 0003370] on mantis, this modification need rollback because the reported
                  // bug was indetified as not a bug.
//                if( false == summer_hour_check() )
//                {
//                    _adjust_param_value(pParameter+currentItem, -1);
//                }

                pageUpdate = 1;
            }
            else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
            {
                _adjust_param_value(pParameter+currentItem, -1);
                  // according to [88230 0003370] on mantis, this modification need rollback because the reported
                  // bug was indetified as not a bug.
//                if( false == summer_hour_check() )
//                {
//                    _adjust_param_value(pParameter+currentItem, 1);
//                }

                pageUpdate = 1;
            }
        }
    }

    if (currentItem > 0)
    {
        if (pageUpdate == 1)
        {
            invertCursor = 1;
            timeStamp = TIME_GetTime() + PAGE_SUMMERHOUR_CURSORFLASH;
        }
        else if (TIME_GetTime() >= timeStamp)
        {
            invertCursor ^= 1;
            timeStamp = TIME_GetTime() + PAGE_SUMMERHOUR_CURSORFLASH;
            pageUpdate = 1;
        }
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

// 2.4.0
void CountryPageHandler(void)
{
#define PAGE_COUNTRY_TOTALITEMS             3
#define PAGE_COUNTRY_RECT_CNTY_X_START      10
#define PAGE_COUNTRY_RECT_CNTY_X_END        (LCD_X_LENGTH - 1 - PAGE_COUNTRY_RECT_CNTY_X_START)
#define PAGE_COUNTRY_RECT_CNTY_Y_START      20
#define PAGE_COUNTRY_RECT_CNTY_Y_END        (PAGE_COUNTRY_RECT_CNTY_Y_START + 19)
#define PAGE_COUNTRY_CNTY_X_POS             (PAGE_COUNTRY_RECT_CNTY_X_START + 4)
#define PAGE_COUNTRY_CNTY_Y_POS             (PAGE_COUNTRY_RECT_CNTY_Y_START + 2)
#define PAGE_COUNTRY_BRACKET_CNTY_X_POS     (PAGE_COUNTRY_RECT_CNTY_X_END - 4 - 6 - 6)
#define PAGE_COUNTRY_BRACKET_CNTY_Y_POS     PAGE_COUNTRY_CNTY_Y_POS
#define PAGE_COUNTRY_RECT_LONG_X_START      48
#define PAGE_COUNTRY_RECT_LONG_X_END        PAGE_COUNTRY_RECT_CNTY_X_END
#define PAGE_COUNTRY_RECT_LONG_Y_START      (PAGE_COUNTRY_RECT_CNTY_Y_END + 5)
#define PAGE_COUNTRY_RECT_LONG_Y_END        (PAGE_COUNTRY_RECT_LONG_Y_START + 19)
#define PAGE_COUNTRY_LONG_X_POS             (PAGE_COUNTRY_RECT_LONG_X_START + 4)
#define PAGE_COUNTRY_LONG_Y_POS             (PAGE_COUNTRY_RECT_LONG_Y_START + 2)
#define PAGE_COUNTRY_BRACKET_LONG_X_POS     PAGE_COUNTRY_BRACKET_CNTY_X_POS
#define PAGE_COUNTRY_BRACKET_LONG_Y_POS     PAGE_COUNTRY_LONG_Y_POS
#define PAGE_COUNTRY_TEXT_LONG_X_POS        PAGE_COUNTRY_CNTY_X_POS
#define PAGE_COUNTRY_TEXT_LONG_Y_POS        PAGE_COUNTRY_LONG_Y_POS
#define PAGE_COUNTRY_RECT_LAT_X_START       48
#define PAGE_COUNTRY_RECT_LAT_X_END         PAGE_COUNTRY_RECT_CNTY_X_END
#define PAGE_COUNTRY_RECT_LAT_Y_START       (PAGE_COUNTRY_RECT_LONG_Y_END + 5)
#define PAGE_COUNTRY_RECT_LAT_Y_END         (PAGE_COUNTRY_RECT_LAT_Y_START + 19)
#define PAGE_COUNTRY_LAT_X_POS              (PAGE_COUNTRY_RECT_LAT_X_START + 4)
#define PAGE_COUNTRY_LAT_Y_POS              (PAGE_COUNTRY_RECT_LAT_Y_START + 2)
#define PAGE_COUNTRY_BRACKET_LAT_X_POS      PAGE_COUNTRY_BRACKET_CNTY_X_POS
#define PAGE_COUNTRY_BRACKET_LAT_Y_POS      PAGE_COUNTRY_LAT_Y_POS
#define PAGE_COUNTRY_TEXT_LAT_X_POS         PAGE_COUNTRY_CNTY_X_POS
#define PAGE_COUNTRY_TEXT_LAT_Y_POS         PAGE_COUNTRY_LAT_Y_POS

    static unsigned char currentItem;
    static PAGE_PARAMETER *pParameter;
    unsigned char tempString[6];
    unsigned short tempValue;
    unsigned char i;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_COUNTRY);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_COUNTRY ]), 0, 0, strings[STRINGS_INDEX_COUNTRY ], 0, 0);

        if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

            currentItem = 0;

            pParameter = (PAGE_PARAMETER *)malloc(PAGE_COUNTRY_TOTALITEMS*sizeof(PAGE_PARAMETER));

            pParameter->ParamIndex = PARAM_INDEX_COUNTRY;
            pParameter->Max = 23;
            pParameter->Min = 0;

            (pParameter+1)->ParamIndex = PARAM_INDEX_LONGITUDE;
            (pParameter+1)->Max = 180;
            (pParameter+1)->Min = 0xFF4C;   // -180

            (pParameter+2)->ParamIndex = PARAM_INDEX_LATITUDE;
            (pParameter+2)->Max = 180;
            (pParameter+2)->Min = 0xFF4C;   // -180

            LCDD_PutString(PAGE_COUNTRY_TEXT_LONG_X_POS, PAGE_COUNTRY_TEXT_LONG_Y_POS, 0, "Long:", 0, 0);
            LCDD_PutString(PAGE_COUNTRY_TEXT_LAT_X_POS, PAGE_COUNTRY_TEXT_LAT_Y_POS, 0, "Lat:", 0, 0);

            pageUpdate = 1;
        }
        else
        {
            _draw_rectangular_for_value();
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        LCDD_ClearRect(PAGE_COUNTRY_RECT_CNTY_X_START, PAGE_COUNTRY_RECT_CNTY_Y_START, PAGE_COUNTRY_RECT_CNTY_X_END, PAGE_COUNTRY_RECT_CNTY_Y_END, (currentItem == 0)?PAGE_HIGHLIGHT_COLOR:0);
        LCDD_ClearRect(PAGE_COUNTRY_RECT_CNTY_X_START+1, PAGE_COUNTRY_RECT_CNTY_Y_START+1, PAGE_COUNTRY_RECT_CNTY_X_END-1, PAGE_COUNTRY_RECT_CNTY_Y_END-1, 0);
        LCDD_PutString(PAGE_COUNTRY_CNTY_X_POS, PAGE_COUNTRY_CNTY_Y_POS, 0, valuesCountry[PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_COUNTRY)*5 ], 0, 0);
        if (currentItem == 0)
        {
            LCDD_PutString(PAGE_COUNTRY_BRACKET_CNTY_X_POS, PAGE_COUNTRY_BRACKET_CNTY_Y_POS, 0, "<>", 0, 0);
        }

        tempValue = PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_LONGITUDE);
        if (tempValue == 0)
        {
            i = 0;
        }
        else if (tempValue < 0x8000)
        {
            tempString[0] = '+';
            i = 1;
        }
        else
        {
            tempValue = 0xFFFF - tempValue;
            tempValue++;
            tempString[0] = '-';
            i = 1;
        }
        i += dectoascii(&tempString[i], tempValue, 0);
        tempString[i++] = stringCountry[0];
        tempString[i] = '\0';
        LCDD_ClearRect(PAGE_COUNTRY_RECT_LONG_X_START, PAGE_COUNTRY_RECT_LONG_Y_START, PAGE_COUNTRY_RECT_LONG_X_END, PAGE_COUNTRY_RECT_LONG_Y_END, (currentItem == 1)?PAGE_HIGHLIGHT_COLOR:0);
        LCDD_ClearRect(PAGE_COUNTRY_RECT_LONG_X_START+1, PAGE_COUNTRY_RECT_LONG_Y_START+1, PAGE_COUNTRY_RECT_LONG_X_END-1, PAGE_COUNTRY_RECT_LONG_Y_END-1, 0);
        LCDD_PutString(PAGE_COUNTRY_LONG_X_POS, PAGE_COUNTRY_LONG_Y_POS, 0, &tempString[0], 0, 0);
        if (currentItem == 1)
        {
            LCDD_PutString(PAGE_COUNTRY_BRACKET_LONG_X_POS, PAGE_COUNTRY_BRACKET_LONG_Y_POS, 0, "<>", 0, 0);
        }

        tempValue = PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_LATITUDE);
        if (tempValue == 0)
        {
            i = 0;
        }
        else if (tempValue < 0x8000)
        {
            tempString[0] = '+';
            i = 1;
        }
        else
        {
            tempValue = 0xFFFF - tempValue;
            tempValue++;
            tempString[0] = '-';
            i = 1;
        }
        i += dectoascii(&tempString[i], tempValue, 0);
        tempString[i++] = stringCountry[0];
        tempString[i] = '\0';
        LCDD_ClearRect(PAGE_COUNTRY_RECT_LAT_X_START, PAGE_COUNTRY_RECT_LAT_Y_START, PAGE_COUNTRY_RECT_LAT_X_END, PAGE_COUNTRY_RECT_LAT_Y_END, (currentItem == 2)?PAGE_HIGHLIGHT_COLOR:0);
        LCDD_ClearRect(PAGE_COUNTRY_RECT_LAT_X_START+1, PAGE_COUNTRY_RECT_LAT_Y_START+1, PAGE_COUNTRY_RECT_LAT_X_END-1, PAGE_COUNTRY_RECT_LAT_Y_END-1, 0);
        LCDD_PutString(PAGE_COUNTRY_LAT_X_POS, PAGE_COUNTRY_LAT_Y_POS, 0, &tempString[0], 0, 0);
        if (currentItem == 2)
        {
            LCDD_PutString(PAGE_COUNTRY_BRACKET_LAT_X_POS, PAGE_COUNTRY_BRACKET_LAT_Y_POS, 0, "<>", 0, 0);
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
                if (currentItem == 0)
                {
                    currentItem = PAGE_COUNTRY_TOTALITEMS-1;
                }
                else
                {
                    currentItem--;
                }

                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
                if (currentItem == PAGE_COUNTRY_TOTALITEMS-1)
                {
                    currentItem = 0;
                }
                else
                {
                    currentItem++;
                }

                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            if (pageAvailability)
            {
                _adjust_param_value(pParameter+currentItem, 1);

                if (currentItem == 0)
                {
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_LONGITUDE, valuesLongitude[PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_COUNTRY)]);
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_LATITUDE, valuesLatitude[PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_COUNTRY)]);
                }

                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT)
        {
            if (pageAvailability)
            {
                _adjust_param_value(pParameter+currentItem, -1);

                if (currentItem == 0)
                {
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_LONGITUDE, valuesLongitude[PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_COUNTRY)]);
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_LATITUDE, valuesLatitude[PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_COUNTRY)]);
                }

                pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
                _page_sub_read_parameters(pParameter, PAGE_COUNTRY_TOTALITEMS);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
                _page_sub_set_parameters(pParameter, PAGE_COUNTRY_TOTALITEMS);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }
    else if (pageAvailability && (BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x3F) == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            _adjust_param_value(pParameter+currentItem, 1);

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT)
        {
            _adjust_param_value(pParameter+currentItem, -1);

            pageUpdate = 1;
        }
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void LightregulationPageHandler(void)
{
//#define PAGE_LIGHTREGULATION_TITLE_X_POS    20
  #define PAGE_LIGHTREGULATION_NAME_X_POS     2
  #define PAGE_LIGHTREGULATION_VALUE_X_POS    106
  #define PAGE_LIGHTREGULATION_BODY_Y_POS     18
  static const PAGE_ITEM_RECT rect[1] = {18+22-1, 18+22+16};
  static unsigned char  currentItem=0;
  static unsigned char currentItem_x;
  const unsigned char *string_speed[]={"very fast","fast","normal","low","very low"};
  static PAGE_PARAMETER *pParameter;
  unsigned char speed_value;  
    if (pageInitialize)
    {
        pageInitialize = 0;
        currentItem=0;
        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_LIGHTREGULATION);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_LIGHTREGULATION ]), 0, 0, strings[STRINGS_INDEX_LIGHTREGULATION ], 0, 0);

        if (pageAvailability)
        {
          _param_page_funckey_initializer(FUNCKEY_READ_WRITE);
          
          pParameter = (PAGE_PARAMETER *)malloc(2*sizeof(PAGE_PARAMETER));
          pParameter->ParamIndex = PARAM_INDEX_LIGHTREGULATION;
          
          if(PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_ReactionSpeed))
          {  
            //(pParameter+1) = (PAGE_PARAMETER *)malloc(1*sizeof(PAGE_PARAMETER));
            (pParameter+1)->ParamIndex = PARAM_INDEX_ReactionSpeed;
          }

//          LCDD_PutString(PAGE_LIGHTREGULATION_NAME_X_POS, PAGE_LIGHTREGULATION_BODY_Y_POS, 0, strings[STRINGS_INDEX_LIGHTREGULATION ], 0, 0);
//          LCDD_InvertRect(0, PAGE_LIGHTREGULATION_BODY_Y_POS-1, LCD_X_LENGTH-1, PAGE_LIGHTREGULATION_BODY_Y_POS+16, PAGE_HIGHLIGHT_COLOR);
          
          pageUpdate = 1;
        }
        else
        {
            _draw_rectangular_for_value();
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
        
        
        if(PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_ReactionSpeed))
        { 
          //(pParameter+1)->ParamIndex = PARAM_INDEX_ReactionSpeed;
          LCDD_ClearRect(0, rect[0].yStart, LCD_X_LENGTH-1, rect[0].yEnd, 0);
          LCDD_PutString(0, 18+22, 0, "Reaction speed", 0, 0);
        //  LCDD_InvertRect(0, rect[currentItem].yStart, LCD_X_LENGTH-1, rect[currentItem].yEnd, PAGE_HIGHLIGHT_COLOR);         
             
          LCDD_ClearRect(40+40-4, 18+22, LCD_X_LENGTH-2-80, 18+22+15, 0);
          LCDD_PutString(40+40, 18+22, 0, string_speed[0], 0, 0);      // 2.4.0
          LCDD_PutString(PAGE_SENSITIVITY_BRACKET_X_POS, 18+22, 0, "<>", 0, 0);       
        
        }
    }

    if (pageUpdate)
    {
       if(PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_LIGHTREGULATION))
        {
           LCDD_ClearRect(0, PAGE_LIGHTREGULATION_BODY_Y_POS-1, LCD_X_LENGTH, PAGE_LIGHTREGULATION_BODY_Y_POS+17, 0);    
          //  LCDD_ClearRect(PAGE_LIGHTREGULATION_VALUE_X_POS-2, PAGE_LIGHTREGULATION_BODY_Y_POS, LCD_X_LENGTH-2, PAGE_LIGHTREGULATION_BODY_Y_POS+15, 0);
          LCDD_PutString(2, PAGE_LIGHTREGULATION_BODY_Y_POS, 0, strings[STRINGS_INDEX_LIGHTREGULATION ], 0, 0);
          LCDD_InvertRect(2, PAGE_LIGHTREGULATION_BODY_Y_POS-1, LCD_X_LENGTH-1, PAGE_LIGHTREGULATION_BODY_Y_POS+16+1, PAGE_HIGHLIGHT_COLOR);
          //LCDD_InvertRect(0, PAGE_LIGHTREGULATION_BODY_Y_POS-1, LCD_X_LENGTH-1, PAGE_LIGHTREGULATION_BODY_Y_POS+16, PAGE_HIGHLIGHT_COLOR);
          LCDD_ClearRect(80+10, 18, LCD_X_LENGTH-1-1,18+16, 0);    
        }
      
      
        if(PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_ReactionSpeed))
      {
        //LCDD_ClearRect(PAGE_LIGHTREGULATION_VALUE_X_POS-2, PAGE_LIGHTREGULATION_BODY_Y_POS, LCD_X_LENGTH-2, PAGE_LIGHTREGULATION_BODY_Y_POS+15, 0);
        LCDD_ClearRect(0, PAGE_LIGHTREGULATION_BODY_Y_POS-1, LCD_X_LENGTH, PAGE_LIGHTREGULATION_BODY_Y_POS+17, 0);      
        if(currentItem==0)
        {
          //  LCDD_ClearRect(PAGE_LIGHTREGULATION_VALUE_X_POS-2, PAGE_LIGHTREGULATION_BODY_Y_POS, LCD_X_LENGTH-2, PAGE_LIGHTREGULATION_BODY_Y_POS+15, 0);
          LCDD_PutString(2, PAGE_LIGHTREGULATION_BODY_Y_POS, 0, strings[STRINGS_INDEX_LIGHTREGULATION ], 0, 0);
          LCDD_InvertRect(2, PAGE_LIGHTREGULATION_BODY_Y_POS-1, LCD_X_LENGTH-1, PAGE_LIGHTREGULATION_BODY_Y_POS+16+1, PAGE_HIGHLIGHT_COLOR);
          //LCDD_InvertRect(0, PAGE_LIGHTREGULATION_BODY_Y_POS-1, LCD_X_LENGTH-1, PAGE_LIGHTREGULATION_BODY_Y_POS+16, PAGE_HIGHLIGHT_COLOR);
          LCDD_ClearRect(80+10, 18, LCD_X_LENGTH-1-1,18+16, 0);      
          
          LCDD_ClearRect(0, rect[0].yStart, LCD_X_LENGTH-1, rect[0].yEnd, 0);
          LCDD_PutString(0, 18+22, 0, "Reaction speed", 0, 0);         
          LCDD_PutString(40+40, 18+22, 0, string_speed[currentItem_x], 0, 0);      // 2.4.0
          // LCDD_PutString(PAGE_SENSITIVITY_BRACKET_X_POS, 18+22, 0, "<>", 0, 0);  
        }
        if(currentItem==1)
        {
          LCDD_ClearRect(0, 18, LCD_X_LENGTH-1,18+22, 0);
          LCDD_PutString(PAGE_LIGHTREGULATION_NAME_X_POS, PAGE_LIGHTREGULATION_BODY_Y_POS, 0, strings[STRINGS_INDEX_LIGHTREGULATION ], 0, 0);
          LCDD_ClearRect(0, rect[0].yStart, LCD_X_LENGTH-1, rect[0].yEnd, 0);
          LCDD_PutString(0, 18+22, 0, "Reaction speed", 0, 0); 
          LCDD_InvertRect(0, rect[0].yStart, LCD_X_LENGTH-1, rect[0].yEnd, PAGE_HIGHLIGHT_COLOR); 
          LCDD_PutString(40+40, 18+22, 0, string_speed[currentItem_x], 0, 0);      // 2.4.0
          //LCDD_PutString(PAGE_SENSITIVITY_BRACKET_X_POS, 18+22, 0, "<>", 0, 0);          

        }
      }
        
       // LCDD_ClearRect(PAGE_LIGHTREGULATION_VALUE_X_POS-2, PAGE_LIGHTREGULATION_BODY_Y_POS, LCD_X_LENGTH-2, PAGE_LIGHTREGULATION_BODY_Y_POS+15, 0);
         if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_LIGHTREGULATION) == 1)    //1 refers to light regulation enabled
        {
           LCDD_PutBitmap(_figure_icon_center_pos(PAGE_LIGHTREGULATION_VALUE_X_POS, LCD_X_LENGTH-2, BitmapInfo_Validation[1].Width), PAGE_LIGHTREGULATION_BODY_Y_POS, &BitmapInfo_Validation[1], 0);
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR)
        {
          if(PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_LIGHTREGULATION)) 
            {
            if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_LIGHTREGULATION) == 1)
            {
                PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_LIGHTREGULATION, 0);
            }
            else
            {
                PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_LIGHTREGULATION, 1);
            }

            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
          
            if (pageAvailability)
            {
              if(PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_ReactionSpeed))
              {
                if(currentItem==1)
                {
                  _page_sub_read_parameters(pParameter, 3);
                  currentItem_x=PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_LIGHTREGULATION);
                }
                if(currentItem==0)
                  _page_sub_read_parameters(pParameter, 2);   
              }else
                 _page_sub_read_parameters(pParameter, 2);               
              pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {

          if(PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_ReactionSpeed))         
          {
            if(currentItem==1)
            {
              switch(currentItem_x){
              case 0:
                speed_value=0x00;
                break;
              case 1:
                speed_value=0x01;
                break;
              case 2:
                speed_value=0x02;            
                break;
              case 3:
                speed_value=0x03;  
                break;
              case 4:
                speed_value=0x04;  
                break;
              case 5:
                speed_value=0x05;  
                break;}
              PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_ReactionSpeed, speed_value);
              _page_sub_set_parameters(pParameter+1, 1);
            }
          }
          if(currentItem==0)
          {    
            if(PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_LIGHTREGULATION)) 
             _page_sub_set_parameters(pParameter, 1);
          }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_ADVANCEDMODE;

            pageSwap = 1;
        }
        else if(BUTTON_GetButtonIndex() ==BUTTON_INDEX_RIGHT)
        {
            if(PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_ReactionSpeed))
            {
             if (currentItem_x == 0)
            {
                currentItem_x = 5-1;
            }
            else
            {
                currentItem_x--;
            }
            pageUpdate = 1;
            }
        }
         else if(BUTTON_GetButtonIndex() ==BUTTON_INDEX_LEFT)
        {
            if(PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_ReactionSpeed))
            {
             if (currentItem_x == 5-1)
            {
                currentItem_x = 0;
            }
            else
            {
                currentItem_x++;
            }
            pageUpdate = 1;
            }
        }
         else if(BUTTON_GetButtonIndex() ==BUTTON_INDEX_UP)
        {
          if(PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_ReactionSpeed))
          {
            if (currentItem == 0)
            {
              currentItem = 2-1;
            }
            else
            {
              currentItem--;
            }
            pageUpdate = 1;
          }
        }
         else if(BUTTON_GetButtonIndex() ==BUTTON_INDEX_DOWN)
         {
           if(PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_ReactionSpeed))
           {
             if (currentItem == 2-1)
             {
               currentItem = 0;
             }
             else
             {
               currentItem++;
             }
             pageUpdate = 1;
            }
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void ProvisionoflightPageHandler(void)
{
//#define PAGE_PROVISIONOFLIGHT_TITLE_X_POS   20
#define PAGE_PROVISIONOFLIGHT_BODY_X_POS    40
#define PAGE_PROVISIONOFLIGHT_BODY_Y_POS    PAGE_VALUE_Y_POS

    static PAGE_PARAMETER *pParameter;
    unsigned char pProvisionoflight[9];     // 2.2.0, was 5

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_PROVISIONOFLIGHT);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_PROVISIONOFLIGHT ]), 0, 0, strings[STRINGS_INDEX_PROVISIONOFLIGHT ], 0, 0);

        if (pageAvailability)
        {
        _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

        pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

        pParameter->ParamIndex = PARAM_INDEX_PROVISIONOFLIGHT;
        pParameter->Max = 0xFF;
        pParameter->Min = 0;

        pageUpdate = 1;
        }
        else
        {
            _draw_rectangular_for_value();
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        _draw_rectangular_for_value();

        if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_PROVISIONOFLIGHT))
        {
            dectoascii(&pProvisionoflight[0], 5*PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_PROVISIONOFLIGHT), 4);
            if (PARAMS_SensorIsHFOnly(&sensorParams) )     // 2.2.0
            {
            }
            else
            {
                strcpy((char *)&pProvisionoflight[4], " Lux");
            }
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, &pProvisionoflight[0]), PAGE_PROVISIONOFLIGHT_BODY_Y_POS, 0, &pProvisionoflight[0], PAGE_HIGHLIGHT_COLOR, 0);    // 2.2.0
        }
        else
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_AUTO ]), PAGE_PROVISIONOFLIGHT_BODY_Y_POS, 0, strings[STRINGS_INDEX_AUTO ], PAGE_HIGHLIGHT_COLOR, 0);       // 2.4.0
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
            _page_sub_read_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_ADVANCEDMODE;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }
    else if (pageAvailability && (BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x3F) == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void LooptypePageHandler(void)
{
//#define PAGE_LOOPTYPE_TITLE_X_POS   20
#define PAGE_LOOPTYPE_BODY_X_POS    40
#define PAGE_LOOPTYPE_BODY_Y_POS    PAGE_VALUE_Y_POS

    static PAGE_PARAMETER *pParameter;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_LOOPTYPE);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_LOOPTYPE ]), 0, 0, strings[STRINGS_INDEX_LOOPTYPE ], 0, 0);

        _draw_rectangular_for_value();

        if (pageAvailability)
        {
        _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

        pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

        pParameter->ParamIndex = PARAM_INDEX_LOOPTYPE;

        pageUpdate = 1;
        }
        else
        {
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        _draw_rectangular_for_value();

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_CLOSELOOP+STRINGS_TOTAL_LANGUAGES*PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_LOOPTYPE) ])+1, PAGE_LOOPTYPE_BODY_Y_POS, 0, strings[STRINGS_INDEX_CLOSELOOP+STRINGS_TOTAL_LANGUAGES*PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_LOOPTYPE) ], PAGE_HIGHLIGHT_COLOR, 0);

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR)
        {
            if (pageAvailability)
            {
            if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_LOOPTYPE) == 1)
            {
                PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_LOOPTYPE, 0);
            }
            else
            {
                PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_LOOPTYPE, 1);
            }

            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
            _page_sub_read_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_ADVANCEDMODE;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void DaylightFactorPageHandler(void)
{
#define PAGE_DAYLIGHTFACTOR_BODY_Y_POS    PAGE_VALUE_Y_POS

    static PAGE_PARAMETER *pParameter;
    unsigned char pValueString[8];
    unsigned char index;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DAYLIGHTFACTOR);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_DAYLIGHTFACTOR ]), 0, 0, strings[STRINGS_INDEX_DAYLIGHTFACTOR ], 0, 0);

//        _draw_rectangular_for_value();

        pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

        pParameter->ParamIndex = PARAM_INDEX_DAYLIGHTFACTOR;
        pParameter->Max = 0xFF;
        pParameter->Min = 1;

        if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_READ_WRITE);
        }
        else
        {
            _param_page_funckey_initializer(FUNCKEY_READ_ONLY);
        }

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        memset(&pValueString[0], 0, sizeof(pValueString));
        index = 0;

        if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_DAYLIGHTFACTOR) == 0)
        {
            pValueString[0] = '0';
        }
        else
        {
            index += dectoascii(&pValueString[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_DAYLIGHTFACTOR)/10, 0);
            pValueString[index++] = '.';
            dectoascii(&pValueString[index], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_DAYLIGHTFACTOR)%10, 0);
        }

        _draw_rectangular_for_value();

        if (pageAvailability)
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, &pValueString[0]), PAGE_DAYLIGHTFACTOR_BODY_Y_POS, 0, &pValueString[0], PAGE_HIGHLIGHT_COLOR, 0);
        }
        else
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
        }
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            _page_sub_read_parameters(pParameter, 1);

//            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }
    else if (pageAvailability && (BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x3F) == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void DaylightFactorCenPageHandler(void)
{
#define PAGE_DAYLIGHTFACTORCEN_BODY_Y_POS   PAGE_VALUE_Y_POS

    static PAGE_PARAMETER *pParameter;
    unsigned char pValueString[8];
    unsigned char index;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DAYLIGHTFACTORCEN);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_DAYLIGHTFACTORCEN ]), 0, 0, strings[STRINGS_INDEX_DAYLIGHTFACTORCEN ], 0, 0);

//        _draw_rectangular_for_value();

        pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

        pParameter->ParamIndex = PARAM_INDEX_DAYLIGHTFACTORCEN;
        pParameter->Max = 0xFF;
        pParameter->Min = 1;

        if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_READ_WRITE);
        }
        else
        {
            _param_page_funckey_initializer(FUNCKEY_READ_ONLY);
        }

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        memset(&pValueString[0], 0, sizeof(pValueString));
        index = 0;

        if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_DAYLIGHTFACTORCEN) == 0)
        {
            pValueString[0] = '0';
        }
        else
        {
            index += dectoascii(&pValueString[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_DAYLIGHTFACTORCEN)/10, 0);
            pValueString[index++] = '.';
            dectoascii(&pValueString[index], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_DAYLIGHTFACTORCEN)%10, 0);
        }

        _draw_rectangular_for_value();

        if (pageAvailability)
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, &pValueString[0]), PAGE_DAYLIGHTFACTORCEN_BODY_Y_POS, 0, &pValueString[0], PAGE_HIGHLIGHT_COLOR, 0);
        }
        else
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
        }
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            _page_sub_read_parameters(pParameter, 1);

//            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }
    else if (pageAvailability && (BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x3F) == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void NaturalLightFactorPageHandler(void)
{
#define PAGE_NATURALLIGHTFACTOR_BODY_Y_POS    PAGE_VALUE_Y_POS

    static PAGE_PARAMETER *pParameter;
    unsigned char pValueString[8];
    unsigned char index;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_NATURALLIGHTFACTOR);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_NATURALLIGHTFACTOR ]), 0, 0, strings[STRINGS_INDEX_NATURALLIGHTFACTOR ], 0, 0);

//        _draw_rectangular_for_value();

        pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

        pParameter->ParamIndex = PARAM_INDEX_NATURALLIGHTFACTOR;
        pParameter->Max = 0xFF;
        pParameter->Min = 1;

        if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_READ_WRITE);
        }
        else
        {
            _param_page_funckey_initializer(FUNCKEY_READ_ONLY);
        }

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        memset(&pValueString[0], 0, sizeof(pValueString));
        index = 0;

        if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_NATURALLIGHTFACTOR) == 0)
        {
            pValueString[0] = '0';
        }
        else
        {
            index += dectoascii(&pValueString[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_NATURALLIGHTFACTOR)/10, 0);
            pValueString[index++] = '.';
            dectoascii(&pValueString[index], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_NATURALLIGHTFACTOR)%10, 0);
        }

        _draw_rectangular_for_value();

        if (pageAvailability)
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, &pValueString[0]), PAGE_NATURALLIGHTFACTOR_BODY_Y_POS, 0, &pValueString[0], PAGE_HIGHLIGHT_COLOR, 0);
        }
        else
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
        }
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            _page_sub_read_parameters(pParameter, 1);

//            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }
    else if (pageAvailability && (BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x3F) == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void DaylightLevelPageHandler(void)
{
#define PAGE_DAYLIGHTLEVEL_BODY_X_POS         40
#define PAGE_DAYLIGHTLEVEL_BODY_Y_POS         PAGE_VALUE_Y_POS

    static PAGE_PARAMETER *pParameter;
    unsigned char pDaylightlevel[10];   // 2.2.0, was 6

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DAYLIGHTLEVEL);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_DAYLIGHTLEVEL ]), 0, 0, strings[STRINGS_INDEX_DAYLIGHTLEVEL ], 0, 0);

        _draw_rectangular_for_value();

        _param_page_funckey_initializer(FUNCKEY_READ_ONLY);

        pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

        pParameter->ParamIndex = PARAM_INDEX_DAYLIGHTLEVEL;
        pParameter->Max = 19999;
        pParameter->Min = 0;

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        if (pageAvailability)
        {
            dectoascii(&pDaylightlevel[0], 5*PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_DAYLIGHTLEVEL), 5);
            if (PARAMS_SensorIsHFOnly(&sensorParams) )     // 2.2.0
            {
            }
            else
            {
                strcpy((char *)&pDaylightlevel[5], " Lux");
            }
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, &pDaylightlevel[0]), PAGE_DAYLIGHTLEVEL_BODY_Y_POS, 0, &pDaylightlevel[0], PAGE_HIGHLIGHT_COLOR, 0);     // 2.2.0
        }
        else
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
        }
        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            _page_sub_read_parameters(pParameter, 1);

//            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void SensorStatusPageHandler(void)
{
#define PAGE_SENSORSTATUS_STRING_MAX_LENGTH 48
#define PAGE_SENSORSTATUS_TOTAL_ITEMS       9
#define PAGE_SENSORSTATUS_LINE_1            16
#define PAGE_SENSORSTATUS_LINE_HEIGHT       16
#define PAGE_SENSORSTATUS_LINE_SPACE        6
#define PAGE_SENSORSTATUS_LINE_2            (PAGE_SENSORSTATUS_LINE_1 + PAGE_SENSORSTATUS_LINE_HEIGHT + PAGE_SENSORSTATUS_LINE_SPACE)
#define PAGE_SENSORSTATUS_LINE_3            (PAGE_SENSORSTATUS_LINE_2 + PAGE_SENSORSTATUS_LINE_HEIGHT + PAGE_SENSORSTATUS_LINE_SPACE)
#define PAGE_SENSORSTATUS_LINE_4            (PAGE_SENSORSTATUS_LINE_3 + PAGE_SENSORSTATUS_LINE_HEIGHT + PAGE_SENSORSTATUS_LINE_SPACE)

    // total items are 7
    static PAGE_PARAMETER *pParameter;
//    static const PAGE_ITEM_RECT rect[4] = {{PAGE_SENSORSTATUS_BODY_Y_POS_1-1, PAGE_SENSORSTATUS_BODY_Y_POS_1+16}, {PAGE_SENSORSTATUS_BODY_Y_POS_2-1, PAGE_SENSORSTATUS_BODY_Y_POS_2+16}, {PAGE_SENSORSTATUS_BODY_Y_POS_3-1, PAGE_SENSORSTATUS_BODY_Y_POS_3+16}, {PAGE_SENSORSTATUS_BODY_Y_POS_4-1, PAGE_SENSORSTATUS_BODY_Y_POS_4+16}};
    static unsigned char *pStrings;
    static unsigned char currentItem;
    static unsigned char data_available = 0;
    unsigned char firstDisplayedItem;
    unsigned short temp;
    unsigned short index;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_SENSORSTATUS);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_SENSORSTATUS ]), 0, 0, strings[STRINGS_INDEX_SENSORSTATUS ], 0, 0);

        _param_page_funckey_initializer(FUNCKEY_READ_ONLY);

        pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

        pParameter->ParamIndex = PARAM_INDEX_SENSORSTATUS;

        if (pageAvailability)
        {
        pStrings = (unsigned char *)malloc(PAGE_SENSORSTATUS_TOTAL_ITEMS*PAGE_SENSORSTATUS_STRING_MAX_LENGTH);

        memset(pStrings, '\0', PAGE_SENSORSTATUS_TOTAL_ITEMS*PAGE_SENSORSTATUS_STRING_MAX_LENGTH);

        currentItem = 0;

        if (data_available)
        {
            pageUpdate = 1;

            // fill pStrings[0][0] ~ pStrings[0][PAGE_SENSORSTATUS_STRING_MAX_LENGTH-1]
            temp = (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_SENSORSTATUS)>>8)&0x007F;
            index = 0;
            strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_LOAD ]);
            index += strlen((const char *)&pStrings[index]);
            pStrings[index++] = ' ';
            switch(temp)
            {
            case 0:
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_OFF ]);      // 2.4.0
                break;
            case 100:
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_ON ]);       // 2.4.0
                break;
            default:
                strcpy((char *)&pStrings[index], "DIM ");
                index += 4;
                index += dectoascii(&pStrings[index], temp, 0);
                pStrings[index] = '%';
                break;
            }

            temp = PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_SENSORSTATUS);

            // fill pStrings[1][0] ~ pStrings[1][PAGE_SENSORSTATUS_STRING_MAX_LENGTH-1]
            index = PAGE_SENSORSTATUS_STRING_MAX_LENGTH;
            strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_SUNDETECTION ]);
            index += strlen((const char *)&pStrings[index]);
            pStrings[index++] = ' ';
            if (temp & 0x0080)
            {
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_ACTIVE ]);
            }
            else
            {
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_INACTIVE ]);
            }

            // fill pStrings[2][0] ~ pStrings[2][PAGE_SENSORSTATUS_STRING_MAX_LENGTH-1]
            index = 2*PAGE_SENSORSTATUS_STRING_MAX_LENGTH;
            if (temp & 0x0040)
            {
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_STANDBYMODE ]);
            }
            else
            {
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_NOSTANDBY ]);
            }

            // fill pStrings[3][0] ~ pStrings[3][PAGE_SENSORSTATUS_STRING_MAX_LENGTH-1]
            index = 3*PAGE_SENSORSTATUS_STRING_MAX_LENGTH;
            if (temp & 0x0020)
            {
//                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_ACTIVE ]);
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_REGULATION_U ]);
            }
            else
            {
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_NO_2 ]);
                index += strlen((const char *)&pStrings[index]);
                pStrings[index++] = ' ';
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_REGULATION_L ]);
            }

            // fill pStrings[4][0] ~ pStrings[4][PAGE_SENSORSTATUS_STRING_MAX_LENGTH-1]
            index = 4*PAGE_SENSORSTATUS_STRING_MAX_LENGTH;
            if (temp & 0x0010)
            {
//                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_ACTIVE ]);
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_DEROGATION_U ]);
            }
            else
            {
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_NO_2 ]);
                index += strlen((const char *)&pStrings[index]);
                pStrings[index++] = ' ';
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_DEROGATION_L ]);
            }

            // fill pStrings[5][0] ~ pStrings[5][PAGE_SENSORSTATUS_STRING_MAX_LENGTH-1]
            index = 5*PAGE_SENSORSTATUS_STRING_MAX_LENGTH;
            if (temp & 0x0008)
            {
//                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_ACTIVE ]);
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_DETECTION_U ]);
            }
            else
            {
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_NO_2 ]);
                index += strlen((const char *)&pStrings[index]);
                pStrings[index++] = ' ';
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_DETECTION_L ]);
            }

            // fill pStrings[5][0] ~ pStrings[5][PAGE_SENSORSTATUS_STRING_MAX_LENGTH-1]
            index = 6*PAGE_SENSORSTATUS_STRING_MAX_LENGTH;
            strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_TIMEDELAY ]);
            index += strlen((const char *)&pStrings[index]);
            pStrings[index++] = ' ';
            if (temp & 0x0004)
            {
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_ON ]);       // 2.4.0
            }
            else
            {
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_OFF ]);      // 2.4.0
            }

            // fill pStrings[7][0] ~ pStrings[7][PAGE_SENSORSTATUS_STRING_MAX_LENGTH-1]
            index = 7*PAGE_SENSORSTATUS_STRING_MAX_LENGTH;
            strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_WALKTHROUGH ]);
            index += strlen((const char *)&pStrings[index]);
            pStrings[index++] = ' ';
            if (temp & 0x0002)
            {
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_ACTIVE ]);
            }
            else
            {
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_INACTIVE ]);
            }

            // fill pStrings[8][0] ~ pStrings[8][PAGE_SENSORSTATUS_STRING_MAX_LENGTH-1]
            index = 8*PAGE_SENSORSTATUS_STRING_MAX_LENGTH;
            strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_RETRIGGER ]);
            index += strlen((const char *)&pStrings[index]);
            pStrings[index++] = ' ';
            if (temp & 0x0001)
            {
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_ACTIVE ]);
            }
            else
            {
                strcpy((char *)&pStrings[index], (char const *)strings[STRINGS_INDEX_INACTIVE ]);
            }

            pageScroll = 0; // 2.4.0
        }
        }
        else
        {
            _draw_rectangular_for_value();
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
        }
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        firstDisplayedItem = 0;

#if (PAGE_SENSORSTATUS_TOTAL_ITEMS > 4)
        if (currentItem > 2)
        {
            firstDisplayedItem = currentItem - 2;

            if (currentItem == PAGE_SENSORSTATUS_TOTAL_ITEMS - 1)
            {
                firstDisplayedItem = PAGE_SENSORSTATUS_TOTAL_ITEMS - 4;
            }
        }
#endif

        if (PAGE_SENSORSTATUS_TOTAL_ITEMS >= 1)
        {
            LCDD_ClearRect(0, PAGE_SENSORSTATUS_LINE_1, LCD_X_LENGTH-1, PAGE_SENSORSTATUS_LINE_1+PAGE_SENSORSTATUS_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 0) ? 3:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_SENSORSTATUS_LINE_1, 0, (pStrings+firstDisplayedItem*PAGE_SENSORSTATUS_STRING_MAX_LENGTH), (currentItem - firstDisplayedItem == 0) ? 3:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_SENSORSTATUS_LINE_1, (LCD_X_LENGTH-2-PAGE_LINE_INDENT), (pStrings+firstDisplayedItem*PAGE_SENSORSTATUS_STRING_MAX_LENGTH)+((currentItem - firstDisplayedItem == 0)?pageScroll:0), (currentItem - firstDisplayedItem == 0)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 0)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
        }

        if (PAGE_SENSORSTATUS_TOTAL_ITEMS >= 2)
        {
            LCDD_ClearRect(0, PAGE_SENSORSTATUS_LINE_2, LCD_X_LENGTH-1, PAGE_SENSORSTATUS_LINE_2+PAGE_SENSORSTATUS_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 1) ? 3:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_SENSORSTATUS_LINE_2, 0, (pStrings+(firstDisplayedItem+1)*PAGE_SENSORSTATUS_STRING_MAX_LENGTH), (currentItem - firstDisplayedItem == 1) ? 3:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_SENSORSTATUS_LINE_2, (LCD_X_LENGTH-2-PAGE_LINE_INDENT), (pStrings+(firstDisplayedItem+1)*PAGE_SENSORSTATUS_STRING_MAX_LENGTH)+((currentItem - firstDisplayedItem == 1)?pageScroll:0), (currentItem - firstDisplayedItem == 1)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 1)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
        }

        if (PAGE_SENSORSTATUS_TOTAL_ITEMS >= 3)
        {
            LCDD_ClearRect(0, PAGE_SENSORSTATUS_LINE_3, LCD_X_LENGTH-1, PAGE_SENSORSTATUS_LINE_3+PAGE_SENSORSTATUS_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 2) ? 3:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_SENSORSTATUS_LINE_3, 0, (pStrings+(firstDisplayedItem+2)*PAGE_SENSORSTATUS_STRING_MAX_LENGTH), (currentItem - firstDisplayedItem == 2) ? 3:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_SENSORSTATUS_LINE_3, (LCD_X_LENGTH-2-PAGE_LINE_INDENT), (pStrings+(firstDisplayedItem+2)*PAGE_SENSORSTATUS_STRING_MAX_LENGTH)+((currentItem - firstDisplayedItem == 2)?pageScroll:0), (currentItem - firstDisplayedItem == 2)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 2)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
        }

        if (PAGE_SENSORSTATUS_TOTAL_ITEMS >= 4)
        {
            LCDD_ClearRect(0, PAGE_SENSORSTATUS_LINE_4, LCD_X_LENGTH-1, PAGE_SENSORSTATUS_LINE_4+PAGE_SENSORSTATUS_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 3) ? 3:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_SENSORSTATUS_LINE_4, 0, (pStrings+(firstDisplayedItem+3)*PAGE_SENSORSTATUS_STRING_MAX_LENGTH), (currentItem - firstDisplayedItem == 3) ? 3:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_SENSORSTATUS_LINE_4, (LCD_X_LENGTH-2-PAGE_LINE_INDENT), (pStrings+(firstDisplayedItem+3)*PAGE_SENSORSTATUS_STRING_MAX_LENGTH)+((currentItem - firstDisplayedItem == 3)?pageScroll:0), (currentItem - firstDisplayedItem == 3)?PAGE_HIGHLIGHT_COLOR:0, (currentItem - firstDisplayedItem == 3)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
        }
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability && data_available)
            {
            if (currentItem == 0)
            {
                currentItem = PAGE_SENSORSTATUS_TOTAL_ITEMS-1;
            }
            else
            {
                currentItem--;
            }

            pageScroll = 0;  // 2.4.0
            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability && data_available)
            {
            if (currentItem == PAGE_SENSORSTATUS_TOTAL_ITEMS-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

            pageScroll = 0;  // 2.4.0
            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (_page_sub_read_parameters(pParameter, 1))
            {
                data_available = 1;
            }
            else
            {
                data_available = 0;
            }

//            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            data_available = 0;

            pageCurrent = PAGE_INDEX_HIDDEN;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    // 2.4.0
    if (data_available && (TIME_GetTime() >= pageTimeStamp))
    {
        pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
        pageUpdate = 1;
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        if (pStrings)
        {
            free(pStrings);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void CurrentPOLPageHandler(void)
{
#define PAGE_CURRENTPOL_BODY_Y_POS         PAGE_VALUE_Y_POS

    static PAGE_PARAMETER *pParameter;
    unsigned char pCurrentPOL[9], i;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_CURRENTPOL);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        // 2.4.0
//        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_CURRENTPOL ]), 0, 0, strings[STRINGS_INDEX_CURRENTPOL ], 0, 0);

//        _draw_rectangular_for_value();

        pageScroll = 0;  // 2.4.0

        pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

        pParameter->ParamIndex = PARAM_INDEX_CURRENTPOL;
        pParameter->Max = 255;
        pParameter->Min = 0;

        _param_page_funckey_initializer(FUNCKEY_READ_ONLY);

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        // 2.4.0
        if (LCDD_PutScrollString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_CURRENTPOL ]), 0, LCD_X_LENGTH, strings[STRINGS_INDEX_CURRENTPOL ]+pageScroll, 0, 1))
        {
            pageScroll++;
            pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
        }

        memset(&pCurrentPOL[0], 0, sizeof(pCurrentPOL));
        i = dectoascii(&pCurrentPOL[0], 5*PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_CURRENTPOL), 0);
        if (PARAMS_SensorHasHF(&sensorParams) && PARAMS_IsLineVoltageSensor(&sensorParams))     // 2.2.0
        {
        }
        else
        {
            pCurrentPOL[i++] = ' ';
            pCurrentPOL[i++] = 'L';
            pCurrentPOL[i++] = 'u';
            pCurrentPOL[i] = 'x';
        }

        _draw_rectangular_for_value();

        if (pageAvailability)
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, &pCurrentPOL[0]), PAGE_CURRENTPOL_BODY_Y_POS, 0, &pCurrentPOL[0], PAGE_HIGHLIGHT_COLOR, 0);
        }
        else
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_CURRENTPOL_BODY_Y_POS, 0, " N/A ", 0, 0);
        }
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            _page_sub_read_parameters(pParameter, 1);

//            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    // 2.4.0
    if (TIME_GetTime() >= pageTimeStamp)
    {
        pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
        pageUpdate = 1;
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void DeviceIDZigbeePageHandler(void)
{
#define PAGE_DEVICEZIGBEE_TOTALITEMS        2       // 2.4.0
#define PAGE_DEVICEIDZIGBEE_BODY_Y_POS         PAGE_VALUE_Y_POS

    static PAGE_PARAMETER *pParameter;
    unsigned char pDeviceIDZigbee[15];      // 2.4.0, was 6
    unsigned char i;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DEVICEIDZIGBEE) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DEVICEIDZIGBEE2);       // 2.4.0

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "DeviceID Zigbee"), 0, 0, "DeviceID Zigbee", 0, 0);

//        _draw_rectangular_for_value();

        pParameter = (PAGE_PARAMETER *)malloc(PAGE_DEVICEZIGBEE_TOTALITEMS*sizeof(PAGE_PARAMETER));     // 2.4.0

        pParameter->ParamIndex = PARAM_INDEX_DEVICEIDZIGBEE2;       // 2.4.0
        (pParameter+1)->ParamIndex = PARAM_INDEX_DEVICEIDZIGBEE;

        _param_page_funckey_initializer(FUNCKEY_READ_ONLY);     // 2.4.0

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        _draw_rectangular_for_value();

        if (pageAvailability)
        {
            memset(&pDeviceIDZigbee[0], 0, sizeof(pDeviceIDZigbee));
            i = 0;

            if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DEVICEIDZIGBEE))
            {
                hextoascii(&pDeviceIDZigbee[i], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_DEVICEIDZIGBEE), 4);
                i += 6;
                if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DEVICEIDZIGBEE2))
                {
                    pDeviceIDZigbee[i++] = ' ';   // 2.4.0
                    pDeviceIDZigbee[i++] = ' ';   // 2.4.0
                }
            }
            if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DEVICEIDZIGBEE2))
            {
                hextoascii(&pDeviceIDZigbee[i], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_DEVICEIDZIGBEE2), 4);   // 2.4.0
            }

            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, &pDeviceIDZigbee[0]), PAGE_DEVICEIDZIGBEE_BODY_Y_POS, 0, &pDeviceIDZigbee[0], PAGE_HIGHLIGHT_COLOR, 0);
        }
        else
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_DEVICEIDZIGBEE_BODY_Y_POS, 0, " N/A ", 0, 0);
        }
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            PARAMS_SetParameterNumber(&sensorParams, PARAM_INDEX_DEVICEIDZIGBEE, PARAM_NUMBER_DEVICEIDZIGBEE);
            PARAMS_SetParameterNumber(&sensorParams, PARAM_INDEX_DEVICEIDZIGBEE2, PARAM_NUMBER_DEVICEIDZIGBEE2);

            _page_sub_read_parameters(pParameter, PAGE_DEVICEZIGBEE_TOTALITEMS);

//            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void OtherparametersPageHandler(void)
{
#define PAGE_OTHERPARAMETERS_TOTALITEMS      3
#define PAGE_OTHERPARAMETERS_LINE_1          16
#define PAGE_OTHERPARAMETERS_LINE_HEIGHT     16
#define PAGE_OTHERPARAMETERS_LINE_SPACE      6
#define PAGE_OTHERPARAMETERS_LINE_2          (PAGE_OTHERPARAMETERS_LINE_1 + PAGE_OTHERPARAMETERS_LINE_HEIGHT + PAGE_OTHERPARAMETERS_LINE_SPACE)
#define PAGE_OTHERPARAMETERS_LINE_3          (PAGE_OTHERPARAMETERS_LINE_2 + PAGE_OTHERPARAMETERS_LINE_HEIGHT + PAGE_OTHERPARAMETERS_LINE_SPACE)
#define PAGE_OTHERPARAMETERS_LINE_4          (PAGE_OTHERPARAMETERS_LINE_3 + PAGE_OTHERPARAMETERS_LINE_HEIGHT + PAGE_OTHERPARAMETERS_LINE_SPACE)

    static PAGE_PARAMS *pPageParams;
    PAGE_PARAMS *pTempParams;
    static unsigned char currentItem = 0;
    unsigned char firstDisplayedItem;

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        _common_funckey_initializer();

        pPageParams = (PAGE_PARAMS *)malloc(PAGE_OTHERPARAMETERS_TOTALITEMS * sizeof(PAGE_PARAMS));
        pTempParams = pPageParams;

        pTempParams->pString = strings[STRINGS_INDEX_BUZZERFEEDBACK ];
        pTempParams->PageHandlerIndex = PAGE_INDEX_BUZZERFEEDBACK;
        pTempParams++;

        pTempParams->pString = strings[STRINGS_INDEX_LOADTYPE ];
        pTempParams->PageHandlerIndex = PAGE_INDEX_LOADTYPE;
        pTempParams++;

        pTempParams->pString = strings[STRINGS_INDEX_POWERLINEFILTER ];
        pTempParams->PageHandlerIndex = PAGE_INDEX_POWERLINEFILTER;
        pTempParams++;

        if (currentItem >= PAGE_OTHERPARAMETERS_TOTALITEMS)
        {
            currentItem = PAGE_OTHERPARAMETERS_TOTALITEMS-1;
        }

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        firstDisplayedItem = 0;

#if (PAGE_OTHERPARAMETERS_TOTALITEMS > 4)
        if (currentItem > 2)
        {
            firstDisplayedItem = currentItem - 2;

            if (currentItem == PAGE_OTHERPARAMETERS_TOTALITEMS - 1)
            {
                firstDisplayedItem = PAGE_OTHERPARAMETERS_TOTALITEMS - 4;
            }
        }
#endif

        if (PAGE_OTHERPARAMETERS_TOTALITEMS >= 1)
        {
            LCDD_ClearRect(0, PAGE_OTHERPARAMETERS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_OTHERPARAMETERS_LINE_1+PAGE_OTHERPARAMETERS_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 0) ? PAGE_HIGHLIGHT_COLOR:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_OTHERPARAMETERS_LINE_1, 0, (pPageParams+firstDisplayedItem)->pString, (currentItem - firstDisplayedItem == 0) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_OTHERPARAMETERS_LINE_1, (currentItem - firstDisplayedItem == 0) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (PAGE_OTHERPARAMETERS_TOTALITEMS >= 2)
        {
            LCDD_ClearRect(0, PAGE_OTHERPARAMETERS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_OTHERPARAMETERS_LINE_2+PAGE_OTHERPARAMETERS_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 1) ? PAGE_HIGHLIGHT_COLOR:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_OTHERPARAMETERS_LINE_2, 0, (pPageParams+firstDisplayedItem+1)->pString, (currentItem - firstDisplayedItem == 1) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_OTHERPARAMETERS_LINE_2, (currentItem - firstDisplayedItem == 1) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (PAGE_OTHERPARAMETERS_TOTALITEMS >= 3)
        {
            LCDD_ClearRect(0, PAGE_OTHERPARAMETERS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_OTHERPARAMETERS_LINE_3+PAGE_OTHERPARAMETERS_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 2) ? PAGE_HIGHLIGHT_COLOR:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_OTHERPARAMETERS_LINE_3, 0, (pPageParams+firstDisplayedItem+2)->pString, (currentItem - firstDisplayedItem == 2) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_OTHERPARAMETERS_LINE_3, (currentItem - firstDisplayedItem == 2) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (PAGE_OTHERPARAMETERS_TOTALITEMS >= 4)
        {
            LCDD_ClearRect(0, PAGE_OTHERPARAMETERS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_OTHERPARAMETERS_LINE_4+PAGE_OTHERPARAMETERS_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 3) ? PAGE_HIGHLIGHT_COLOR:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_OTHERPARAMETERS_LINE_4, 0, (pPageParams+firstDisplayedItem+3)->pString, (currentItem - firstDisplayedItem == 3) ? PAGE_HIGHLIGHT_COLOR:0, 0);
            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_OTHERPARAMETERS_LINE_4, (currentItem - firstDisplayedItem == 3) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = PAGE_OTHERPARAMETERS_TOTALITEMS-1;
            }
            else
            {
                currentItem--;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == PAGE_OTHERPARAMETERS_TOTALITEMS-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            pageCurrent = (pPageParams+currentItem)->PageHandlerIndex;

            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pPageParams)
        {
            free(pPageParams);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void BuzzerfeedbackPageHandler(void)
{
#define PAGE_BUZZERFEEDBACK_NAME_X_POS  2
#define PAGE_BUZZERFEEDBACK_VALUE_X_POS 106
#define PAGE_BUZZERFEEDBACK_BODY_Y_POS  18

    static PAGE_PARAMETER *pParameter;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_BUZZERFEEDBACK);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_BUZZERFEEDBACK ]), 0, 0, strings[STRINGS_INDEX_BUZZERFEEDBACK ], 0, 0);

        pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

        pParameter->ParamIndex = PARAM_INDEX_BUZZERFEEDBACK;

        if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

            LCDD_PutString(PAGE_BUZZERFEEDBACK_NAME_X_POS, PAGE_BUZZERFEEDBACK_BODY_Y_POS, 0, strings[STRINGS_INDEX_BUZZERFEEDBACK ], 0, 0);
            LCDD_InvertRect(0, PAGE_BUZZERFEEDBACK_BODY_Y_POS-1, LCD_X_LENGTH-1, PAGE_BUZZERFEEDBACK_BODY_Y_POS+16, PAGE_HIGHLIGHT_COLOR);

            pageUpdate = 1;
        }
        else
        {
            _param_page_funckey_initializer(FUNCKEY_READ_ONLY);

            _draw_rectangular_for_value();
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            //pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        LCDD_ClearRect(PAGE_BUZZERFEEDBACK_VALUE_X_POS-2, PAGE_BUZZERFEEDBACK_BODY_Y_POS, LCD_X_LENGTH-2, PAGE_BUZZERFEEDBACK_BODY_Y_POS+15, 0);
        if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_BUZZERFEEDBACK) == 1)    //1 refers to buzzer feedback enabled
        {
            LCDD_PutBitmap(_figure_icon_center_pos(PAGE_BUZZERFEEDBACK_VALUE_X_POS, LCD_X_LENGTH-2, BitmapInfo_Validation[1].Width), PAGE_BUZZERFEEDBACK_BODY_Y_POS, &BitmapInfo_Validation[1], 0);
        }
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR)
        {
            if (pageAvailability)
            {
                if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_BUZZERFEEDBACK) == 1)
                {
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_BUZZERFEEDBACK, 0);
                }
                else
                {
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_BUZZERFEEDBACK, 1);
                }

                pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            _page_sub_read_parameters(pParameter, 1);

//            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
                _page_sub_set_parameters(pParameter, 1);

//                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_OTHERPARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void LoadtypePageHandler(void)
{
    static PAGE_PARAMETER *pParameter;
    unsigned char xpos;
    const unsigned char *ptmpstr;

    if (pageInitialize)
    {
     pageScroll = 0;                    

      pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_LOADTYPE);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_LOADTYPE ]), 0, 0, strings[STRINGS_INDEX_LOADTYPE ], 0, 0);

        pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

        pParameter->ParamIndex = PARAM_INDEX_LOADTYPE;
        pParameter->Max = 6;
        pParameter->Min = 0;

        if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_READ_WRITE);
        }
        else
        {
            _param_page_funckey_initializer(FUNCKEY_READ_ONLY);
        }

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        _draw_rectangular_for_value();

        if (pageAvailability)
        {
          
        ptmpstr = strings[STRINGS_INDEX_LOADTYPE+STRINGS_TOTAL_LANGUAGES*((unsigned char)PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_LOADTYPE)+1) ];
        xpos = _figure_string_center_x_pos(PAGE_RECTANGULAR_X_POS+3, PAGE_RECTANGULAR_X_POS+PAGE_RECTANGULAR_LENGTH+2, strings[STRINGS_INDEX_LOADTYPE+STRINGS_TOTAL_LANGUAGES*((unsigned char)PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_LOADTYPE)+1) ]);

        
        if (LCDD_PutScrollString( xpos, PAGE_VALUE_Y_POS,PAGE_RECTANGULAR_LENGTH-5,ptmpstr+pageScroll,PAGE_HIGHLIGHT_COLOR,1)){
            pageScroll++;
            pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
        }
          
//            LCDD_PutString(xpos, PAGE_VALUE_Y_POS, 0,ptmpstr , PAGE_HIGHLIGHT_COLOR, 0);
        }
        else
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
        }
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
                _adjust_param_value(pParameter, 1);

                pageUpdate = 1;
                pageScroll = 0;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
                _adjust_param_value(pParameter, -1);
                pageScroll = 0;
                pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            _page_sub_read_parameters(pParameter, 1);

//            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
                _page_sub_set_parameters(pParameter, 1);

//                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_OTHERPARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (TIME_GetTime() >= pageTimeStamp)
    {
        pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
        pageUpdate = 1;
    }
    
    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void PowerlinefilterPageHandler(void)
{
#define PAGE_POWERLINEFILTER_NAME_X_POS  2
#define PAGE_POWERLINEFILTER_VALUE_X_POS 106
#define PAGE_POWERLINEFILTER_BODY_Y_POS  18

    static PAGE_PARAMETER *pParameter;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_POWERLINEFILTER);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_POWERLINEFILTER ]), 0, 0, strings[STRINGS_INDEX_POWERLINEFILTER ], 0, 0);

        pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

        pParameter->ParamIndex = PARAM_INDEX_POWERLINEFILTER;

        if (pageAvailability)
        {
            _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

            LCDD_PutString(PAGE_POWERLINEFILTER_NAME_X_POS, PAGE_POWERLINEFILTER_BODY_Y_POS, 0, strings[STRINGS_INDEX_POWERLINEFILTER ], 0, 0);
            LCDD_InvertRect(0, PAGE_POWERLINEFILTER_BODY_Y_POS-1, LCD_X_LENGTH-1, PAGE_POWERLINEFILTER_BODY_Y_POS+16, PAGE_HIGHLIGHT_COLOR);

            pageUpdate = 1;
        }
        else
        {
            _param_page_funckey_initializer(FUNCKEY_READ_ONLY);

            _draw_rectangular_for_value();
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            //pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        LCDD_ClearRect(PAGE_POWERLINEFILTER_VALUE_X_POS-2, PAGE_POWERLINEFILTER_BODY_Y_POS, LCD_X_LENGTH-2, PAGE_POWERLINEFILTER_BODY_Y_POS+15, 0);
        if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_POWERLINEFILTER) == 1)    //1 refers to buzzer feedback enabled
        {
            LCDD_PutBitmap(_figure_icon_center_pos(PAGE_POWERLINEFILTER_VALUE_X_POS, LCD_X_LENGTH-2, BitmapInfo_Validation[1].Width), PAGE_POWERLINEFILTER_BODY_Y_POS, &BitmapInfo_Validation[1], 0);
        }
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR)
        {
            if (pageAvailability)
            {
                if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_POWERLINEFILTER) == 1)
                {
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_POWERLINEFILTER, 0);
                }
                else
                {
                    PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_POWERLINEFILTER, 1);
                }

                pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            _page_sub_read_parameters(pParameter, 1);

//            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
                _page_sub_set_parameters(pParameter, 1);

//                pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_OTHERPARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void TypePageHandler(void)
{
}

void FirmwareVersionPageHandler(void)
{
}

void IDPageHandler(void)
{
}

void ReferentPageHandler(void)
{
}

void FilesPageHandler(void)
{
//#define PAGE_FILES_LINE_1            16
//#define PAGE_FILES_LINE_HEIGHT       16
//#define PAGE_FILES_LINE_SPACE        6
//#define PAGE_FILES_LINE_2            (PAGE_FILES_LINE_1 + PAGE_FILES_LINE_HEIGHT + PAGE_FILES_LINE_SPACE)
//#define PAGE_FILES_LINE_3            (PAGE_FILES_LINE_2 + PAGE_FILES_LINE_HEIGHT + PAGE_FILES_LINE_SPACE)
//#define PAGE_FILES_LINE_4            (PAGE_FILES_LINE_3 + PAGE_FILES_LINE_HEIGHT + PAGE_FILES_LINE_SPACE)

    static PAGE_FILE_PARAMS *pFileParams;
    static unsigned char currentItem = 0;
    static unsigned char totalItems;
    static unsigned char firstDisplayedItem;
    FILES_FAT fat;

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_FILES ]), 0, 0, strings[STRINGS_INDEX_FILES ], 0, 0);

        FILES_ReadFAT(&fat);

        totalItems = FILES_GetFileNumber(&fat)+1;   // plus 1 for default files

        if (currentItem >= totalItems)
        {
            currentItem = totalItems - 1;
        }

        pFileParams = (PAGE_FILE_PARAMS *)malloc((FILES_MAX_NUMBER)*sizeof(PAGE_FILE_PARAMS));    //use FILES_MAX_NUMBER for worst case testing

        for (unsigned char i=1; i<totalItems; i++)
        {
            FILES_ReadFileName(pFileParams[i-1].pFileName, &fat, i);
        }

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        firstDisplayedItem = 0;

//        if (totalItems > 4)
//        {
        if (currentItem > 2)
        {
            firstDisplayedItem = currentItem - 2;

            if (currentItem == totalItems - 1)
            {
                firstDisplayedItem = totalItems - 4;
            }
        }
//        }

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        if (currentItem > 0)
        {
            pageFunckeyItems[1].pItem = (void *)&BitmapInfo_ReadFile;
            pageFunckeyItems[2].pItem = (void *)&BitmapInfo_WriteSensor;
            pageFunckeyItems[3].pItem = (void *)&BitmapInfo_DeleteFile;
        }
        _common_funckey_initializer();

        if (totalItems >= 1)
        {
            LCDD_ClearRect(0, PAGE_FILES_LINE_1, LCD_X_LENGTH-1, PAGE_FILES_LINE_1+PAGE_FILES_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 0) ? 3:0);
            if (firstDisplayedItem == 0)    // to display "Default files"
            {
                LCDD_PutString(PAGE_LINE_INDENT, PAGE_FILES_LINE_1, 0, strings[STRINGS_INDEX_DEFAULTFILES ], (currentItem - firstDisplayedItem == 0) ? 3:0, 0);
                LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_FILES_LINE_1, (currentItem - firstDisplayedItem == 0) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
            }
            else
            {
                LCDD_PutString(PAGE_LINE_INDENT, PAGE_FILES_LINE_1, 0, (pFileParams+firstDisplayedItem-1)->pFileName, (currentItem - firstDisplayedItem == 0) ? 3:0, 0);
                LCDD_PutString(PAGE_ARROW_NAV_X_POS-2, PAGE_FILES_LINE_1, 0, "OK", (currentItem - firstDisplayedItem == 0) ? 3:0, 0);
            }
        }

        if (totalItems >= 2)
        {
            LCDD_ClearRect(0, PAGE_FILES_LINE_2, LCD_X_LENGTH-1, PAGE_FILES_LINE_2+PAGE_FILES_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 1) ? 3:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_FILES_LINE_2, 0, (pFileParams+firstDisplayedItem)->pFileName, (currentItem - firstDisplayedItem == 1) ? 3:0, 0);
            LCDD_PutString(PAGE_ARROW_NAV_X_POS-2, PAGE_FILES_LINE_2, 0, "OK", (currentItem - firstDisplayedItem == 1) ? 3:0, 0);
//            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_FILES_LINE_2, (currentItem - firstDisplayedItem == 1) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 3)
        {
            LCDD_ClearRect(0, PAGE_FILES_LINE_3, LCD_X_LENGTH-1, PAGE_FILES_LINE_3+PAGE_FILES_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 2) ? 3:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_FILES_LINE_3, 0, (pFileParams+firstDisplayedItem+1)->pFileName, (currentItem - firstDisplayedItem == 2) ? 3:0, 0);
            LCDD_PutString(PAGE_ARROW_NAV_X_POS-2, PAGE_FILES_LINE_3, 0, "OK", (currentItem - firstDisplayedItem == 2) ? 3:0, 0);
//            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_FILES_LINE_3, (currentItem - firstDisplayedItem == 2) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        if (totalItems >= 4)
        {
            LCDD_ClearRect(0, PAGE_FILES_LINE_4, LCD_X_LENGTH-1, PAGE_FILES_LINE_4+PAGE_FILES_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 3) ? 3:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_FILES_LINE_4, 0, (pFileParams+firstDisplayedItem+2)->pFileName, (currentItem - firstDisplayedItem == 3) ? 3:0, 0);
            LCDD_PutString(PAGE_ARROW_NAV_X_POS-2, PAGE_FILES_LINE_4, 0, "OK", (currentItem - firstDisplayedItem == 3) ? 3:0, 0);
//            LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_FILES_LINE_4, (currentItem - firstDisplayedItem == 3) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
                if (currentItem == 0)
                {
                    currentItem = totalItems - 1;
                }
                else
                {
                    currentItem--;
                }

//            BUTTON_Acknowledge();
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
                if (currentItem == totalItems - 1)
                {
                    currentItem = 0;
                }
                else
                {
                    currentItem++;
                }

//            BUTTON_Acknowledge();
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            if (currentItem == 0)   // enter Default files page
            {
                pageCurrent = PAGE_INDEX_DEFAULTFILES;
                pageSwap = 1;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR)
        {
            if (currentItem > 0)
            {
                argForSubPage = currentItem - firstDisplayedItem;
                argForSubPage2 = currentItem;
                pageCurrent = PAGE_INDEX_NEWFILE;   // To rename file
                pageSwap = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;
            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F2)
        {
            if (currentItem > 0)
            {
                argForSubPage = currentItem;
                pageCurrent = PAGE_INDEX_FILE_PARAMETERS;
                pageSwap = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (currentItem > 0)
            {
                argForSubPage = currentItem;
                pageCurrent = PAGE_INDEX_SENDFILE;
                pageSwap = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F4)
        {
            if (currentItem > 0)
            {
                argForSubPage = currentItem;
                pageCurrent = PAGE_INDEX_DELETEFILE;
                pageSwap = 1;
            }
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pFileParams)
        {
            free(pFileParams);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void DefaultFilesPageHandler(void)
{
#define PAGE_DEFAULTFILES_TOTALITEMS    12

    static unsigned char currentItem = 0;
    static unsigned char firstDisplayedItem;

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_DEFAULTFILES ]), 0, 0, strings[STRINGS_INDEX_DEFAULTFILES ], 0, 0);

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        firstDisplayedItem = 0;

#if (PAGE_DEFAULTFILES_TOTALITEMS > 4)
        if (currentItem > 2)
        {
            firstDisplayedItem = currentItem - 2;

            if (currentItem == PAGE_DEFAULTFILES_TOTALITEMS - 1)
            {
                firstDisplayedItem = PAGE_DEFAULTFILES_TOTALITEMS - 4;
            }
        }
#endif

        _param_page_funckey_initializer(FUNCKEY_WRITE_ONLY);

        if (PAGE_DEFAULTFILES_TOTALITEMS >= 1)
        {
            LCDD_ClearRect(0, PAGE_FILES_LINE_1, LCD_X_LENGTH-1, PAGE_FILES_LINE_1+PAGE_FILES_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 0) ? 3:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_FILES_LINE_1, 0, strings[STRINGS_INDEX_CORRIDORHALLAUTO+STRINGS_TOTAL_LANGUAGES*firstDisplayedItem ], (currentItem - firstDisplayedItem == 0) ? 3:0, 0);
        }

        if (PAGE_DEFAULTFILES_TOTALITEMS >= 2)
        {
            LCDD_ClearRect(0, PAGE_FILES_LINE_2, LCD_X_LENGTH-1, PAGE_FILES_LINE_2+PAGE_FILES_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 1) ? 3:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_FILES_LINE_2, 0, strings[STRINGS_INDEX_CORRIDORHALLAUTO+STRINGS_TOTAL_LANGUAGES*(firstDisplayedItem+1) ], (currentItem - firstDisplayedItem == 1) ? 3:0, 0);
        }

        if (PAGE_DEFAULTFILES_TOTALITEMS >= 3)
        {
            LCDD_ClearRect(0, PAGE_FILES_LINE_3, LCD_X_LENGTH-1, PAGE_FILES_LINE_3+PAGE_FILES_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 2) ? 3:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_FILES_LINE_3, 0, strings[STRINGS_INDEX_CORRIDORHALLAUTO+STRINGS_TOTAL_LANGUAGES*(firstDisplayedItem+2) ], (currentItem - firstDisplayedItem == 2) ? 3:0, 0);
        }

        if (PAGE_DEFAULTFILES_TOTALITEMS >= 4)
        {
            LCDD_ClearRect(0, PAGE_FILES_LINE_4, LCD_X_LENGTH-1, PAGE_FILES_LINE_4+PAGE_FILES_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 3) ? 3:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_FILES_LINE_4, 0, strings[STRINGS_INDEX_CORRIDORHALLAUTO+STRINGS_TOTAL_LANGUAGES*(firstDisplayedItem+3) ], (currentItem - firstDisplayedItem == 3) ? 3:0, 0);
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
                if (currentItem == 0)
                {
                    currentItem = PAGE_DEFAULTFILES_TOTALITEMS - 1;
                }
                else
                {
                    currentItem--;
                }

//            BUTTON_Acknowledge();
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
                if (currentItem == PAGE_DEFAULTFILES_TOTALITEMS - 1)
                {
                    currentItem = 0;
                }
                else
                {
                    currentItem++;
                }

//            BUTTON_Acknowledge();
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_FILES;
            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            argForSubPage = currentItem / 2;
            argForSubPage2 = currentItem % 2;
            pageCurrent = PAGE_INDEX_SENDDEFAULTFILE;
            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }
}

void FileParametersPageHandler(void)
{
    static SENSOR_PARAMS *pParams;
    FILES_FAT fat;

    if (pageInitialize)
    {
        pParams = (SENSOR_PARAMS *)malloc(sizeof(SENSOR_PARAMS));
        FILES_ReadFAT(&fat);
        FILES_ReadFileParams(pParams, &fat, argForSubPage);
    }

    _list_parameters(pParams, 0, 0);

    if (pageInitialize)
    {
        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, PARAMETERS_LINE_1-1, 0);
        LCDD_ClearRect(0, PARAMETERS_LINE_4+PARAMETERS_LINE_HEIGHT+PARAMETERS_LINE_SPACE, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        _common_funckey_initializer();

        pageInitialize = 0;

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_FILES;
            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        _list_parameters(pParams, 0, 1);
        if(pParams)
        {
            free(pParams);
        }
        pageSwap = 0;
        pageInitialize = 1;
    }
}

void SendFilePageHandler(void)
{
#define PAGE_SENDFILE_RECT_X_START   8
#define PAGE_SENDFILE_RECT_Y_START   16
#define PAGE_SENDFILE_RECT_X_END     (LCD_X_LENGTH-PAGE_SENDFILE_RECT_X_START-1)
#define PAGE_SENDFILE_BODY_INDENT    4
#define PAGE_SENDFILE_BODY_X_START   (PAGE_SENDFILE_RECT_X_START+PAGE_SENDFILE_BODY_INDENT)
#define PAGE_SENDFILE_BODY_X_LENGTH  (PAGE_SENDFILE_RECT_X_END-PAGE_SENDFILE_BODY_INDENT-PAGE_SENDFILE_BODY_X_START)
#define PAGE_SENDFILE_BODY_Y_START   (PAGE_SENDFILE_RECT_Y_START+PAGE_SENDFILE_BODY_INDENT)
#define PAGE_SENDFILE_PGBAR_Y_SPAN   4
#define PAGE_SENDFILE_PGBAR_HEIGHT   12
#define PAGE_SENDFILE_PGBAR_LENGTH   (PAGE_SENDFILE_RECT_X_END-PAGE_SENDFILE_RECT_X_START-3)
#define PAGE_SENDFILE_PGBAR_STEP     (PAGE_SENDFILE_PGBAR_LENGTH/PARAMS_WRITABLE_TOTAL)

    static SENSOR_PARAMS *pParams;
    static unsigned char currentParameter;
    static unsigned char xOffset;
    static unsigned char yOffset;
    FILES_FAT fat;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pParams = (SENSOR_PARAMS *)malloc(sizeof(SENSOR_PARAMS));

        FILES_ReadFAT(&fat);
        FILES_ReadFileParams(pParams, &fat, argForSubPage);

        currentParameter = 0;

        xOffset = PAGE_SENDFILE_RECT_X_START+2;
        yOffset = PAGE_SENDFILE_BODY_Y_START+LCDD_PutString(PAGE_SENDFILE_BODY_X_START, PAGE_SENDFILE_BODY_Y_START, PAGE_SENDFILE_BODY_X_LENGTH, strings[STRINGS_INDEX_WAITSENDING ], 3, 1)+PAGE_SENDFILE_BODY_INDENT;
        LCDD_ClearRect(PAGE_SENDFILE_RECT_X_START, PAGE_SENDFILE_RECT_Y_START, PAGE_SENDFILE_RECT_X_END, yOffset, 3);
        yOffset += PAGE_SENDFILE_PGBAR_Y_SPAN;
        LCDD_PutString(PAGE_SENDFILE_BODY_X_START, PAGE_SENDFILE_BODY_Y_START, PAGE_SENDFILE_BODY_X_LENGTH, strings[STRINGS_INDEX_WAITSENDING ], 3, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        pageFunckeyItems[1].pItem = (void *)strings[STRINGS_INDEX_YES ];
        pageFunckeyItems[1].itemType = 1;
        pageFunckeyItems[2].pItem = (void *)strings[STRINGS_INDEX_NO_1 ];
        pageFunckeyItems[2].itemType = 1;
        _common_funckey_initializer();
    }

    if (pageUpdate)
    {
        if (currentParameter < PARAMS_WRITABLE_TOTAL)
        {
            if (PARAMS_IsParameterAvailable(pParams, currentParameter))
            {
                IR_SetSensorParameter(pParams, currentParameter);
                TIME_Wait(25);
            }
            xOffset+= PAGE_SENDFILE_PGBAR_STEP;
            if (xOffset > PAGE_SENDFILE_RECT_X_END-2)
            {
                xOffset = PAGE_SENDFILE_RECT_X_END-2;
            }
            LCDD_ClearRect(PAGE_SENDFILE_RECT_X_START+2, yOffset+2, xOffset, yOffset+PAGE_SENDFILE_PGBAR_HEIGHT-3, 3);
            currentParameter++;
        }
        else
        {
            LCDD_ClearRect(PAGE_SENDFILE_RECT_X_START+2, yOffset+2, PAGE_SENDFILE_RECT_X_END-2, yOffset+PAGE_SENDFILE_PGBAR_HEIGHT-3, 3);
            pageUpdate = 0;
            pageCurrent = PAGE_INDEX_FILES;
            pageSwap = 1;
        }
    }

    _common_funckey_handler();

    if (((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED)) && (pageUpdate == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F2)
        {
            LCDD_ClearRect(PAGE_SENDFILE_RECT_X_START, yOffset, PAGE_SENDFILE_RECT_X_END, yOffset+PAGE_SENDFILE_PGBAR_HEIGHT-1, 3);
            LCDD_ClearRect(PAGE_SENDFILE_RECT_X_START+1, yOffset+1, PAGE_SENDFILE_RECT_X_END-1, yOffset+PAGE_SENDFILE_PGBAR_HEIGHT-2, 0);
            pageUpdate = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F1) || (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3))
        {
            pageCurrent = PAGE_INDEX_FILES;
            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParams)
        {
            free(pParams);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void SendDefaultFilePageHandler(void)
{
    static SENSOR_PARAMS *pParams;
    static unsigned char currentParameter;
    static unsigned char xOffset;
    static unsigned char yOffset;
    static unsigned char const *string;
    unsigned char retry, result, i;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pParams = (SENSOR_PARAMS *)malloc(sizeof(SENSOR_PARAMS));

        currentParameter = 0;

        xOffset = PAGE_SENDFILE_RECT_X_START+2;
        yOffset = PAGE_SENDFILE_BODY_Y_START+LCDD_PutString(PAGE_SENDFILE_BODY_X_START, PAGE_SENDFILE_BODY_Y_START, PAGE_SENDFILE_BODY_X_LENGTH, strings[STRINGS_INDEX_WAITSENDING ], 3, 1)+PAGE_SENDFILE_BODY_INDENT;
        LCDD_ClearRect(PAGE_SENDFILE_RECT_X_START, PAGE_SENDFILE_RECT_Y_START, PAGE_SENDFILE_RECT_X_END, yOffset, 3);
        yOffset += PAGE_SENDFILE_PGBAR_Y_SPAN;
        LCDD_PutString(PAGE_SENDFILE_BODY_X_START, PAGE_SENDFILE_BODY_Y_START, PAGE_SENDFILE_BODY_X_LENGTH, strings[STRINGS_INDEX_WAITSENDING ], 3, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        pageFunckeyItems[1].pItem = (void *)strings[STRINGS_INDEX_YES ];
        pageFunckeyItems[1].itemType = 1;
        pageFunckeyItems[2].pItem = (void *)strings[STRINGS_INDEX_NO_1 ];
        pageFunckeyItems[2].itemType = 1;
        _common_funckey_initializer();
    }

    if (pageUpdate)
    {
        if (currentParameter < PARAMS_WRITABLE_TOTAL)
        {
            if (PARAMS_IsParameterAvailable(pParams, currentParameter))
            {
                IR_SetSensorParameter(pParams, currentParameter);
                TIME_Wait(25);
            }
            xOffset+= PAGE_SENDFILE_PGBAR_STEP;
            if (xOffset > PAGE_SENDFILE_RECT_X_END-2)
            {
                xOffset = PAGE_SENDFILE_RECT_X_END-2;
            }
            LCDD_ClearRect(PAGE_SENDFILE_RECT_X_START+2, yOffset+2, xOffset, yOffset+PAGE_SENDFILE_PGBAR_HEIGHT-3, 3);
            currentParameter++;
        }
        else
        {
            LCDD_ClearRect(PAGE_SENDFILE_RECT_X_START+2, yOffset+2, PAGE_SENDFILE_RECT_X_END-2, yOffset+PAGE_SENDFILE_PGBAR_HEIGHT-3, 3);
            pageUpdate = 0;
            pageCurrent = PAGE_INDEX_FILES;
            pageSwap = 1;
        }
    }

    _common_funckey_handler();

    if (((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED)) && (pageUpdate == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F2)
        {
            retry = 0;
            string = NULL;
            PARAMS_SetParameterNumber(pParams, PARAM_INDEX_TYPE, PARAM_NUMBER_TYPE);
            while (retry < PAGE_PARAM_READ_RETRY)
            {
                result = IR_ReadSensorParameter(pParams, PARAM_INDEX_TYPE);
                if (result == 1)    // reading successfully
                {
                    if (PARAMS_IsLineVoltageSensor(pParams))    // What about LV Zigbee sensor?
                    {
                        PARAMS_ResetToDefault(pParams, PARAMS_GetParameterValue(pParams, PARAM_INDEX_TYPE));
                        for (i=0; i<PARAMS_WRITABLE_TOTAL; i++)
                        {
                            if (!PARAMS_IsParameterAvailable(pDefaultFileParams[argForSubPage], i))
                            {
                                PARAMS_SetParameterNumber(pParams, i, PARAM_NUMBER_ERROR);
                            }
                            else
                            {
                                if (PARAMS_IsParameterAvailable(pParams, i) && PARAMS_ValidateDefaultFileValues(pParams, i, PARAMS_GetParameterValue(pDefaultFileParams[argForSubPage], i)))
                                {
                                    if ((i == PARAM_INDEX_MODE) && (argForSubPage2 == 1))   // manual mode, override the defaul value
                                    {
                                        PARAMS_SetParameterValue(pParams, i, 3);    // Manual on/Auto off
                                    }
                                    else
                                    {
                                        PARAMS_SetParameterValue(pParams, i, PARAMS_GetParameterValue(pDefaultFileParams[argForSubPage], i));
                                    }
                                }
                            }
                        }
                        break;
                    }
                    else
                    {
                        string = strings[STRINGS_INDEX_DEFAULTFILESNA ];
                        break;
                    }
                }
                else if (result == 2)
                {
                    string = strings[STRINGS_INDEX_UNKNOWNVALUE ];
                    break;
                }
                else if (result == 3)
                {
                    string = strings[STRINGS_INDEX_NOTIMPLEMENTED ];
                    break;
                }
                else
                {
                    if (++retry >= PAGE_PARAM_READ_RETRY)
                    {
                        string = strings[STRINGS_INDEX_ERRORREADING ];
                        break;
                    }
                    else
                    {
                        TIME_Wait(200);
                    }
                }
            }

            if (string)     // Error happened, display the message and quit
            {
                yOffset -= PAGE_SENDFILE_PGBAR_Y_SPAN;  // step back to bottom line of message displaying rectangular
                LCDD_ClearRect(PAGE_SENDFILE_RECT_X_START, PAGE_SENDFILE_RECT_Y_START, PAGE_SENDFILE_RECT_X_END, yOffset, 3);
                yOffset = PAGE_SENDFILE_BODY_Y_START+LCDD_PutString(PAGE_SENDFILE_BODY_X_START, PAGE_SENDFILE_BODY_Y_START, PAGE_SENDFILE_BODY_X_LENGTH, string, 3, 1)+PAGE_SENDFILE_BODY_INDENT;   // Expand the rect if new message occupies more area
                LCDD_ClearRect(PAGE_SENDFILE_RECT_X_START, PAGE_SENDFILE_RECT_Y_START, PAGE_SENDFILE_RECT_X_END, yOffset, 3);
                LCDD_PutString(PAGE_SENDFILE_BODY_X_START, PAGE_SENDFILE_BODY_Y_START, PAGE_SENDFILE_BODY_X_LENGTH, string, 3, 0);
                LCDD_Refresh();
                TIME_Wait(2000);
                pageUpdate = 0;
                pageCurrent = PAGE_INDEX_DEFAULTFILES;
                pageSwap = 1;
            }
            else    // Type validated correctly, start sending parameters
            {
                LCDD_ClearRect(PAGE_SENDFILE_RECT_X_START, yOffset, PAGE_SENDFILE_RECT_X_END, yOffset+PAGE_SENDFILE_PGBAR_HEIGHT-1, 3);
                LCDD_ClearRect(PAGE_SENDFILE_RECT_X_START+1, yOffset+1, PAGE_SENDFILE_RECT_X_END-1, yOffset+PAGE_SENDFILE_PGBAR_HEIGHT-2, 0);
                pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F1) || (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3))
        {
            pageCurrent = PAGE_INDEX_DEFAULTFILES;
            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParams)
        {
            free(pParams);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

void DeleteFilePageHandler(void)
{
#define PAGE_DELETEFILE_RECT_X_START   8
#define PAGE_DELETEFILE_RECT_Y_START   20
#define PAGE_DELETEFILE_RECT_X_END     (LCD_X_LENGTH-PAGE_DELETEFILE_RECT_X_START-1)
#define PAGE_DELETEFILE_BODY_INDENT    4
#define PAGE_DELETEFILE_BODY_X_START   (PAGE_DELETEFILE_RECT_X_START+PAGE_DELETEFILE_BODY_INDENT)
#define PAGE_DELETEFILE_BODY_X_LENGTH  (PAGE_DELETEFILE_RECT_X_END-PAGE_DELETEFILE_BODY_INDENT-PAGE_DELETEFILE_BODY_X_START)
#define PAGE_DELETEFILE_BODY_Y_START   (PAGE_DELETEFILE_RECT_Y_START+PAGE_DELETEFILE_BODY_INDENT)

    FILES_FAT fat;
    unsigned char yEnd;

    if (pageInitialize)
    {
        pageInitialize = 0;

        yEnd = PAGE_DELETEFILE_BODY_Y_START+LCDD_PutString(PAGE_DELETEFILE_BODY_X_START, PAGE_DELETEFILE_BODY_Y_START, PAGE_DELETEFILE_BODY_X_LENGTH, strings[STRINGS_INDEX_DELETEFILE ], 3, 1);
        LCDD_ClearRect(PAGE_DELETEFILE_RECT_X_START, PAGE_DELETEFILE_RECT_Y_START, PAGE_DELETEFILE_RECT_X_END, yEnd+PAGE_DELETEFILE_BODY_INDENT, 3);
        LCDD_PutString(PAGE_DELETEFILE_BODY_X_START, PAGE_DELETEFILE_BODY_Y_START, PAGE_DELETEFILE_BODY_X_LENGTH, strings[STRINGS_INDEX_DELETEFILE ], 3, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[1].pItem = (void *)strings[STRINGS_INDEX_YES ];
        pageFunckeyItems[1].itemType = 1;
        pageFunckeyItems[2].pItem = (void *)strings[STRINGS_INDEX_NO_1 ];
        pageFunckeyItems[2].itemType = 1;
        _common_funckey_initializer();
    }

    if (pageUpdate)
    {
        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F2)
        {
            FILES_ReadFAT(&fat);
            FILES_DeleteFile(&fat, argForSubPage);
            pageCurrent = PAGE_INDEX_FILES;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            pageCurrent = PAGE_INDEX_FILES;
        }
        pageSwap = 1;

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }
}

void PnLEntryPageHandler(void)
{
#define PAGE_PNLENTRY_BODY_X_POS         36
#define PAGE_PNLENTRY_BODY_Y_POS         48
#define PAGE_PNLENTRY_LINE_HEIGHT       16
#define PAGE_PNLENTRY_LINE_SPACE        6
#define PAGE_PNLENTRY_LINE_1            16
#define PAGE_PNLENTRY_LINE_2            (PAGE_PNLENTRY_LINE_1+PAGE_PNLENTRY_LINE_HEIGHT+PAGE_PNLENTRY_LINE_SPACE)
#define PAGE_PNLENTRY_BODY_Y_POS        48
#define PAGE_PNLENTRY_CNTDOWN_ERASE     10  // 10 seconds

    static unsigned char currentItem = 0;
    static unsigned char cntdown;
    unsigned char i;
    unsigned char ascii[4] = {'\0'};

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_PNL ]), 0, 0, strings[STRINGS_INDEX_PNL ], 0, 0);

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        LCDD_ClearRect(0, PAGE_PNLENTRY_LINE_1, LCD_X_LENGTH-1, PAGE_PNLENTRY_LINE_1+PAGE_PNLENTRY_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_PNLENTRY_LINE_1, 0, strings[STRINGS_INDEX_PNLGROUP ], (currentItem == 0)?3:0, 0);

        LCDD_ClearRect(0, PAGE_PNLENTRY_LINE_2, LCD_X_LENGTH-1, PAGE_PNLENTRY_LINE_2+PAGE_PNLENTRY_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_PNLENTRY_LINE_2, 0, strings[STRINGS_INDEX_ADDPERIPHERAL ], (currentItem == 1)?3:0, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        if (currentItem == 0)   // redraw the function key area for "PnL Group"
        {
            pageFunckeyItems[2].pItem = (void *)strings[STRINGS_INDEX_ERASE ];
            pageFunckeyItems[2].itemType = 1;
            pageFunckeyItems[3].pItem = (void *)strings[STRINGS_INDEX_LEARN ];      // 2.4.0
            pageFunckeyItems[3].itemType = 1;
        }
        else if (currentItem == 1)  // redraw the function key area for "Add peripheral"
        {
            pageFunckeyItems[2].pItem = (void *)&BitmapInfo_WriteSensor;
        }
        _common_funckey_initializer();
    }

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHING) && (pageButtonActive == 0))
    {
        if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_UP) || (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN))
        {
            pageButtonActive = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageButtonActive = 1;
            LCDD_InvertRect(pageFunckeyRect[0].RectStartX, PAGE_FUNCKEY_Y_START, pageFunckeyRect[0].RectEndX, PAGE_FUNCKEY_Y_END, PAGE_HIGHLIGHT_COLOR);
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F3) && (currentItem == 0))
        {
            pageButtonActive = 1;
            LCDD_ClearRect(pageFunckeyRect[2].RectStartX, PAGE_FUNCKEY_Y_START, pageFunckeyRect[2].RectEndX, PAGE_FUNCKEY_Y_END, 0);
            cntdown = PAGE_PNLENTRY_CNTDOWN_ERASE;
            dectoascii(&ascii[0], cntdown, 0);
            for (i=0; i<sizeof(ascii); i++)
            {
                if (ascii[i] == '\0')
                {
                    break;
                }
            }
            ascii[i] = '\"';
            _put_funckey_string(pageFunckeyRect[2].RectStartX, pageFunckeyRect[2].RectEndX, &ascii[0]);
            LCDD_InvertRect(pageFunckeyRect[2].RectStartX, PAGE_FUNCKEY_Y_START, pageFunckeyRect[2].RectEndX, PAGE_FUNCKEY_Y_END, PAGE_HIGHLIGHT_COLOR);
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F3) && (currentItem == 1))
        {
            pageButtonActive = 1;
            LCDD_InvertRect(pageFunckeyRect[2].RectStartX, PAGE_FUNCKEY_Y_START, pageFunckeyRect[2].RectEndX, PAGE_FUNCKEY_Y_END, PAGE_HIGHLIGHT_COLOR);
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F4) && (currentItem == 0))
        {
            pageButtonActive = 1;
            LCDD_InvertRect(pageFunckeyRect[3].RectStartX, PAGE_FUNCKEY_Y_START, pageFunckeyRect[3].RectEndX, PAGE_FUNCKEY_Y_END, PAGE_HIGHLIGHT_COLOR);
        }
    }
    else if (((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED)) && pageButtonActive)
    {
        pageButtonActive = 0;

        if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_UP) || (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN))
        {
            currentItem = 1 - currentItem;

//            BUTTON_Acknowledge();
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            LCDD_InvertRect(pageFunckeyRect[0].RectStartX, PAGE_FUNCKEY_Y_START, pageFunckeyRect[0].RectEndX, PAGE_FUNCKEY_Y_END, PAGE_HIGHLIGHT_COLOR);
            pageCurrent = PAGE_INDEX_MAIN;
            pageSwap = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F3) && (currentItem == 0))
        {
            LCDD_ClearRect(pageFunckeyRect[2].RectStartX, PAGE_FUNCKEY_Y_START, pageFunckeyRect[2].RectEndX, PAGE_FUNCKEY_Y_END, 0);
            _put_funckey_string(pageFunckeyRect[2].RectStartX, pageFunckeyRect[2].RectEndX, strings[STRINGS_INDEX_ERASE ]);
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F3) && (currentItem == 1))
        {
            LCDD_InvertRect(pageFunckeyRect[2].RectStartX, PAGE_FUNCKEY_Y_START, pageFunckeyRect[2].RectEndX, PAGE_FUNCKEY_Y_END, PAGE_HIGHLIGHT_COLOR);
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_ADD_PERIPHERAL, CMD_ADD_PERIPHERAL);        // 2.3.0
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F4) && (currentItem == 0))
        {
            LCDD_InvertRect(pageFunckeyRect[3].RectStartX, PAGE_FUNCKEY_Y_START, pageFunckeyRect[3].RectEndX, PAGE_FUNCKEY_Y_END, PAGE_HIGHLIGHT_COLOR);
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_START_GROUP_PNL, CMD_START_GROUP_PNL);
            pageCurrent = PAGE_INDEX_PNL;
            pageSwap = 1;
        }
    }
    else if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3) && (currentItem == 0) && (BUTTON_GetPressedTime()%1000 == 0) && pageButtonActive)
    {
        LCDD_ClearRect(pageFunckeyRect[2].RectStartX, PAGE_FUNCKEY_Y_START, pageFunckeyRect[2].RectEndX, PAGE_FUNCKEY_Y_END, 0);
        if (--cntdown)
        {
            dectoascii(&ascii[0], cntdown, 0);
            for (i=0; i<sizeof(ascii); i++)
            {
                if (ascii[i] == '\0')
                {
                    break;
                }
            }
            ascii[i] = '\"';
            _put_funckey_string(pageFunckeyRect[2].RectStartX, pageFunckeyRect[2].RectEndX, &ascii[0]);
            LCDD_InvertRect(pageFunckeyRect[2].RectStartX, PAGE_FUNCKEY_Y_START, pageFunckeyRect[2].RectEndX, PAGE_FUNCKEY_Y_END, PAGE_HIGHLIGHT_COLOR);
        }
        else
        {
            pageButtonActive = 0;
            _put_funckey_string(pageFunckeyRect[2].RectStartX, pageFunckeyRect[2].RectEndX, strings[STRINGS_INDEX_ERASE ]);

            BUTTON_Acknowledge();

            // Send NITOO frame of erase PnL group
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_ERASE_GROUP_PNL, CMD_ERASE_GROUP_PNL);

            LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1-PAGE_FUNCKEY_AREA_HEIGHT, 0);
            for (i=0; i<16; i++)
            {
                LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_DELETEGROUP ]), PAGE_PNLENTRY_BODY_Y_POS, 0, strings[STRINGS_INDEX_DELETEGROUP ], 0, 0);
                LCDD_Refresh();
                TIME_Wait(52);
                LCDD_ClearRect(0, PAGE_PNLENTRY_BODY_Y_POS, LCD_X_LENGTH-1, PAGE_PNLENTRY_BODY_Y_POS+15, 0);
                LCDD_Refresh();
                TIME_Wait(242);
            }
            pageCurrent = PAGE_INDEX_PNLENTRY;
            pageSwap = 1;
        }
    }

    _common_battery_condition();

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }
}

void PnLPageHandler(void)
{
#define PAGE_PNL_TITLE_X_POS            44
#define PAGE_PNL_RECT_SPACE             3
#define PAGE_PNL_RECT_LENGTH            25
#define PAGE_PNL_RECT_INCLUDE_X_START   51
#define PAGE_PNL_RECT_INCLUDE_X_END     (PAGE_PNL_RECT_INCLUDE_X_START+PAGE_PNL_RECT_LENGTH-1)
#define PAGE_PNL_RECT_INCLUDE_Y_START   17
#define PAGE_PNL_RECT_INCLUDE_Y_END     (PAGE_PNL_RECT_INCLUDE_Y_START+PAGE_PNL_RECT_LENGTH-1)
#define PAGE_PNL_RECT1_X_START          (PAGE_PNL_RECT_INCLUDE_X_START-PAGE_PNL_RECT_SPACE-PAGE_PNL_RECT_LENGTH)
#define PAGE_PNL_RECT1_X_END            (PAGE_PNL_RECT1_X_START+PAGE_PNL_RECT_LENGTH-1)
#define PAGE_PNL_RECT1_Y_START          (PAGE_PNL_RECT_INCLUDE_Y_END+PAGE_PNL_RECT_SPACE+1)
#define PAGE_PNL_RECT1_Y_END            (PAGE_PNL_RECT1_Y_START+PAGE_PNL_RECT_LENGTH-1)
#define PAGE_PNL_RECT_NEXT_X_START      PAGE_PNL_RECT_INCLUDE_X_START
#define PAGE_PNL_RECT_NEXT_X_END        PAGE_PNL_RECT_INCLUDE_X_END
#define PAGE_PNL_RECT_NEXT_Y_START      PAGE_PNL_RECT1_Y_START
#define PAGE_PNL_RECT_NEXT_Y_END        PAGE_PNL_RECT1_Y_END
#define PAGE_PNL_RECT2_X_START          (PAGE_PNL_RECT_INCLUDE_X_END+PAGE_PNL_RECT_SPACE+1)
#define PAGE_PNL_RECT2_X_END            (PAGE_PNL_RECT2_X_START+PAGE_PNL_RECT_LENGTH-1)
#define PAGE_PNL_RECT2_Y_START          PAGE_PNL_RECT1_Y_START
#define PAGE_PNL_RECT2_Y_END            PAGE_PNL_RECT1_Y_END
#define PAGE_PNL_RECT_EXCLUDE_X_START   PAGE_PNL_RECT_INCLUDE_X_START
#define PAGE_PNL_RECT_EXCLUDE_X_END     PAGE_PNL_RECT_INCLUDE_X_END
#define PAGE_PNL_RECT_EXCLUDE_Y_START   (PAGE_PNL_RECT1_Y_END+PAGE_PNL_RECT_SPACE+1)
#define PAGE_PNL_RECT_EXCLUDE_Y_END     (PAGE_PNL_RECT_EXCLUDE_Y_START+PAGE_PNL_RECT_LENGTH-1)
#define PAGE_PNL_CHAR_INCLUDE_X_POS     ((PAGE_PNL_RECT_INCLUDE_X_START+PAGE_PNL_RECT_INCLUDE_X_END)/2-2)
#define PAGE_PNL_CHAR_INCLUDE_Y_POS     ((PAGE_PNL_RECT_INCLUDE_Y_START+PAGE_PNL_RECT_INCLUDE_Y_END)/2-8)
#define PAGE_PNL_CHAR_NEXT_X_POS        ((PAGE_PNL_RECT_NEXT_X_START+PAGE_PNL_RECT_NEXT_X_END)/2-2)
#define PAGE_PNL_CHAR_NEXT_Y_POS        ((PAGE_PNL_RECT_NEXT_Y_START+PAGE_PNL_RECT_NEXT_Y_END)/2-8)
#define PAGE_PNL_CHAR_EXCLUDE_X_POS     ((PAGE_PNL_RECT_EXCLUDE_X_START+PAGE_PNL_RECT_EXCLUDE_X_END)/2-2)
#define PAGE_PNL_CHAR_EXCLUDE_Y_POS     ((PAGE_PNL_RECT_EXCLUDE_Y_START+PAGE_PNL_RECT_EXCLUDE_Y_END)/2-8)
#define PAGE_PNL_CNTDOWN_INCLUDE        2   // 2 seconds

    static unsigned int timeStamp;
    static unsigned char learnDisplay = 0;
    static unsigned char cntdown;
    unsigned char i;
    unsigned char ascii[4] = {'\0'};

    static const PAGE_BUTTON_RECT rects[3] = {
        {BUTTON_INDEX_UP, PAGE_PNL_RECT_INCLUDE_X_START, PAGE_PNL_RECT_INCLUDE_Y_START, PAGE_PNL_RECT_INCLUDE_X_END, PAGE_PNL_RECT_INCLUDE_Y_END},
        {BUTTON_INDEX_MAJOR, PAGE_PNL_RECT_NEXT_X_START, PAGE_PNL_RECT_NEXT_Y_START, PAGE_PNL_RECT_NEXT_X_END, PAGE_PNL_RECT_NEXT_Y_END},
        {BUTTON_INDEX_DOWN, PAGE_PNL_RECT_EXCLUDE_X_START, PAGE_PNL_RECT_EXCLUDE_Y_START, PAGE_PNL_RECT_EXCLUDE_X_END, PAGE_PNL_RECT_EXCLUDE_Y_END},
        };

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)strings[STRINGS_INDEX_END ];
        pageFunckeyItems[0].itemType = 1;
        _common_funckey_initializer();

        LCDD_ClearRect(PAGE_PNL_RECT1_X_START, PAGE_PNL_RECT1_Y_START, PAGE_PNL_RECT1_X_END, PAGE_PNL_RECT1_Y_END, 3);
        LCDD_ClearRect(PAGE_PNL_RECT1_X_START+1, PAGE_PNL_RECT1_Y_START+1, PAGE_PNL_RECT1_X_END-1, PAGE_PNL_RECT1_Y_END-1, 0);
        LCDD_ClearRect(PAGE_PNL_RECT2_X_START, PAGE_PNL_RECT2_Y_START, PAGE_PNL_RECT2_X_END, PAGE_PNL_RECT2_Y_END, 3);
        LCDD_ClearRect(PAGE_PNL_RECT2_X_START+1, PAGE_PNL_RECT2_Y_START+1, PAGE_PNL_RECT2_X_END-1, PAGE_PNL_RECT2_Y_END-1, 0);

        for (i=0; i<3; i++)
        {
            LCDD_ClearRect(rects[i].RectStartX, rects[i].RectStartY, rects[i].RectEndX, rects[i].RectEndY, 3);
            LCDD_ClearRect(rects[i].RectStartX+1, rects[i].RectStartY+1, rects[i].RectEndX-1, rects[i].RectEndY-1, 0);
        }

        LCDD_PutChar(PAGE_PNL_CHAR_INCLUDE_X_POS, PAGE_PNL_CHAR_INCLUDE_Y_POS, '+', 0, 0);
        LCDD_PutString(_figure_string_center_x_pos(PAGE_PNL_RECT_NEXT_X_START, PAGE_PNL_RECT_NEXT_X_END, "OK"), PAGE_PNL_CHAR_NEXT_Y_POS, 0, "OK", 0, 0);
        LCDD_PutChar(PAGE_PNL_CHAR_EXCLUDE_X_POS, PAGE_PNL_CHAR_EXCLUDE_Y_POS, '-', 0, 0);

        learnDisplay = 0;

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        timeStamp = TIME_GetTime()+1000;

        if (learnDisplay)
        {
            LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, 15, 0);
        }
        else
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_LEARN ]), 0, 0, strings[STRINGS_INDEX_LEARN ], 0, 0);
        }
        learnDisplay = 1-learnDisplay;
    }

    if (TIME_GetTime() >= timeStamp)
    {
        pageUpdate = 1;
    }

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHING) && (pageButtonActive == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            pageButtonActive = 1;
            LCDD_ClearRect(rects[0].RectStartX+1, rects[0].RectStartY+1, rects[0].RectEndX-1, rects[0].RectEndY-1, 0);
            cntdown = PAGE_PNL_CNTDOWN_INCLUDE;
            dectoascii(&ascii[0], cntdown, 0);
            for (i=0; i<sizeof(ascii); i++)
            {
                if (ascii[i] == '\0')
                {
                    break;
                }
            }
            ascii[i] = '\"';
            LCDD_PutString(PAGE_PNL_CHAR_INCLUDE_X_POS, PAGE_PNL_CHAR_INCLUDE_Y_POS, 0, &ascii[0], 0, 0);
            LCDD_InvertRect(rects[0].RectStartX+1, rects[0].RectStartY+1, rects[0].RectEndX-1, rects[0].RectEndY-1, PAGE_HIGHLIGHT_COLOR);
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR)
        {
            pageButtonActive = 1;
            LCDD_InvertRect(rects[1].RectStartX+1, rects[1].RectStartY+1, rects[1].RectEndX-1, rects[1].RectEndY-1, PAGE_HIGHLIGHT_COLOR);
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            pageButtonActive = 1;
            LCDD_InvertRect(rects[2].RectStartX+1, rects[2].RectStartY+1, rects[2].RectEndX-1, rects[2].RectEndY-1, PAGE_HIGHLIGHT_COLOR);
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageButtonActive = 1;
            LCDD_InvertRect(pageFunckeyRect[0].RectStartX, PAGE_FUNCKEY_Y_START, pageFunckeyRect[0].RectEndX, PAGE_FUNCKEY_Y_END, PAGE_HIGHLIGHT_COLOR);
        }
    }
    else if (((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED)) && pageButtonActive)
    {
        pageButtonActive = 0;

        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            LCDD_ClearRect(rects[0].RectStartX+1, rects[0].RectStartY+1, rects[0].RectEndX-1, rects[0].RectEndY-1, 0);
            LCDD_PutChar(PAGE_PNL_CHAR_INCLUDE_X_POS, PAGE_PNL_CHAR_INCLUDE_Y_POS, '+', 0, 0);
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR)
        {
            LCDD_InvertRect(rects[1].RectStartX+1, rects[1].RectStartY+1, rects[1].RectEndX-1, rects[1].RectEndY-1, PAGE_HIGHLIGHT_COLOR);
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_NEXT_ACTUATOR, CMD_NEXT_ACTUATOR);
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            LCDD_InvertRect(rects[2].RectStartX+1, rects[2].RectStartY+1, rects[2].RectEndX-1, rects[2].RectEndY-1, PAGE_HIGHLIGHT_COLOR);
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_EXCLUDE, CMD_EXCLUDE);
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            LCDD_InvertRect(pageFunckeyRect[0].RectStartX, PAGE_FUNCKEY_Y_START, pageFunckeyRect[0].RectEndX, PAGE_FUNCKEY_Y_END, PAGE_HIGHLIGHT_COLOR);
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_END_GROUP_PNL, CMD_END_GROUP_PNL);
            pageCurrent = PAGE_INDEX_PNLENTRY;
            pageSwap = 1;
        }
    }
    else if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP) && (BUTTON_GetPressedTime()%1000 == 0) && pageButtonActive)
    {
        LCDD_ClearRect(rects[0].RectStartX+1, rects[0].RectStartY+1, rects[0].RectEndX-1, rects[0].RectEndY-1, 0);
        if (--cntdown)
        {
            dectoascii(&ascii[0], cntdown, 0);
            for (i=0; i<sizeof(ascii); i++)
            {
                if (ascii[i] == '\0')
                {
                    break;
                }
            }
            ascii[i] = '\"';
            LCDD_PutString(PAGE_PNL_CHAR_INCLUDE_X_POS, PAGE_PNL_CHAR_INCLUDE_Y_POS, 0, &ascii[0], 0, 0);
            LCDD_InvertRect(rects[0].RectStartX+1, rects[0].RectStartY+1, rects[0].RectEndX-1, rects[0].RectEndY-1, PAGE_HIGHLIGHT_COLOR);
        }
        else
        {
            pageButtonActive = 0;
            LCDD_PutChar(PAGE_PNL_CHAR_INCLUDE_X_POS, PAGE_PNL_CHAR_INCLUDE_Y_POS, '+', 0, 0);

            // Send NITOO frame of including
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_INCLUDE, CMD_INCLUDE);

            for (i=0; i<8; i++)
            {
                LCDD_InvertRect(rects[0].RectStartX+1, rects[0].RectStartY+1, rects[0].RectEndX-1, rects[0].RectEndY-1, PAGE_HIGHLIGHT_COLOR);
                LCDD_Refresh();
                TIME_Wait(242);
            }
        }
    }

    _common_battery_condition();

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }
}
#else
void AdresserPageHandler(void)//PAGE_CREATION_ZONE
{

	#define PAGE_MAIN_BATTERY_BODY_LENGTH       19
	#define PAGE_MAIN_BATTERY_BODY_HEIGHT       10
	#define PAGE_MAIN_BATTERY_BODY_X_START      (LCD_X_LENGTH-2-PAGE_MAIN_BATTERY_BODY_LENGTH)
	#define PAGE_MAIN_BATTERY_BODY_Y_START      2
	#define PAGE_MAIN_BATTERY_NIPPLE_LENGTH     2
	#define PAGE_MAIN_BATTERY_NIPPLE_HEIGHT     6
	#define PAGE_MAIN_BATTERY_NIPPLE_X_START    (PAGE_MAIN_BATTERY_BODY_X_START-PAGE_MAIN_BATTERY_NIPPLE_LENGTH)
	#define PAGE_MAIN_BATTERY_NIPPLE_Y_START    4
	#define PAGE_MAIN_BATTERY_FUEL_X_START      (PAGE_MAIN_BATTERY_BODY_X_START+2)
	#define PAGE_MAIN_BATTERY_FUEL_Y_START      (PAGE_MAIN_BATTERY_BODY_Y_START+2)
	#define PAGE_MAIN_BATTERY_FUEL_LENGTH       15
	#define PAGE_MAIN_BATTERY_FUEL_HEIGHT       6
    #define NB_REPEAT_BMP     2

    unsigned char diz;
    unsigned char uni;
    unsigned char cen;
    unsigned char mil;
    static unsigned char pageMainBattery = 0;
    static unsigned char currentItem = 3;
    unsigned char commande;
    static unsigned int nombredadresses;

    if(nombredadresses>1022)
        {
        nombredadresses=1022;
        }
        else
        {
        mil=nombredadresses/1000;
        cen=(nombredadresses-(mil*1000))/100;
        diz=(nombredadresses-(mil*1000)-(cen*100))/10;
        uni=nombredadresses-(mil*1000)-(cen*100)-(diz*10);
        }

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);
	LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "ADRESSER"), 0, 0, "ADRESSER", 0, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        pageFunckeyItems[1].pItem = (void *)&BitmapInfo_SendData;
        pageFunckeyItems[2].pItem = (void *)&BitmapInfo_DeleteData;
        pageFunckeyItems[3].pItem = (void *)&BitmapInfo_ResetData;

        _common_funckey_initializer();

        LCDD_ClearRect(PAGE_MAIN_BATTERY_NIPPLE_X_START, PAGE_MAIN_BATTERY_NIPPLE_Y_START, PAGE_MAIN_BATTERY_NIPPLE_X_START+PAGE_MAIN_BATTERY_NIPPLE_LENGTH-1, PAGE_MAIN_BATTERY_NIPPLE_Y_START+PAGE_MAIN_BATTERY_NIPPLE_HEIGHT-1, 3);
        LCDD_ClearRect(PAGE_MAIN_BATTERY_BODY_X_START, PAGE_MAIN_BATTERY_BODY_Y_START, PAGE_MAIN_BATTERY_BODY_X_START+PAGE_MAIN_BATTERY_BODY_LENGTH-1, PAGE_MAIN_BATTERY_BODY_Y_START+PAGE_MAIN_BATTERY_BODY_HEIGHT-1, 3);

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
       pageUpdate = 0;

       LCDD_ClearRect(0, 16, LCD_X_LENGTH-1, LCD_Y_LENGTH-25, 0);

       LCDD_ClearRect(4 + (currentItem * 30), 34 , 4 + (currentItem * 30) + 27 , 34 + 47  , 2);
       LCDD_ClearRect(6 + (currentItem * 30), 36 , 4 + (currentItem * 30) + 25 , 34 + 45  , 0);

       LCDD_PutBitmap(14 + (currentItem * 30), 26 , &BitmapInfo_Digit[5], 0);
       _draw_Digit(6, 38, mil);
       _draw_Digit(36, 38, cen);
       _draw_Digit(66, 38, diz);
       _draw_Digit(96, 38, uni);
       LCDD_PutBitmap(14 + (currentItem * 30) ,82 , &BitmapInfo_Digit[3], 0);

       LCDD_ClearRect(PAGE_MAIN_BATTERY_FUEL_X_START-1, PAGE_MAIN_BATTERY_FUEL_Y_START-1, PAGE_MAIN_BATTERY_FUEL_X_START+PAGE_MAIN_BATTERY_FUEL_LENGTH, PAGE_MAIN_BATTERY_FUEL_Y_START+PAGE_MAIN_BATTERY_FUEL_HEIGHT, 0);
       if (pageMainBattery)
        {
            LCDD_ClearRect(PAGE_MAIN_BATTERY_FUEL_X_START+PAGE_MAIN_BATTERY_FUEL_LENGTH-pageMainBattery, PAGE_MAIN_BATTERY_FUEL_Y_START, PAGE_MAIN_BATTERY_FUEL_X_START+PAGE_MAIN_BATTERY_FUEL_LENGTH-1, PAGE_MAIN_BATTERY_FUEL_Y_START+PAGE_MAIN_BATTERY_FUEL_HEIGHT-1, 2);
        }
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED)/* || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED)*/)
    {
        if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_UP))
        {
          if (currentItem == 3)
           {
             if ((uni<9)&&(nombredadresses < 1022)) uni++;
             else uni = 0;
           }
          if (currentItem == 2)
           {
             if ((diz<9)&&(nombredadresses < 1012)) diz++;
             else diz = 0;
           }
          if (currentItem == 1)
           {
             if ((cen<9)&&(nombredadresses < 922)) cen++;
             else cen = 0;
           }
          if (currentItem == 0)
           {
             if ((mil==0)&&(nombredadresses < 22)) mil++;
             else mil = 0;
           }

          nombredadresses = (mil*1000) + (cen*100) + (diz*10) + uni;
          pageUpdate = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN))
        {
          if (currentItem == 3)
           {
            if (nombredadresses < 1020)
              {
               if (uni>0) uni--;
                    else uni = 9;
              }
           }
          if (currentItem == 2)
           {
            if (nombredadresses < 1000)
                {
                if (diz>0) diz--;
                else diz = 9;
                }
            else
                {
                if (diz>0) diz--;
                else diz = 2;
                }
           }
          if (currentItem == 1)
           {
            if (nombredadresses < 1000)
              {
                 if (cen>0) cen--;
                     else cen = 9;
              }
           }
          if (currentItem == 0)
           {
             if (mil == 1) mil--;
             else if (nombredadresses > 1000) mil = 1;
           }

          nombredadresses = (mil*1000) + (cen*100) + (diz*10) + uni;
          pageUpdate = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT))
        {
              if(currentItem>0)
              {
                    currentItem--;
              }
              else currentItem=0;
              pageUpdate = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT))
        {
              if(currentItem<3)
              {
                    currentItem++;
              }
              else currentItem=3;
              pageUpdate = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR))
        {

        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;
            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F2)
        {
            commande=0x01;
            IR_BAES(nombredadresses,commande);

            for(int cpt=0;cpt<NB_REPEAT_BMP;cpt++)
              {
              LCDD_ClearRect(0, 16, LCD_X_LENGTH-1, LCD_Y_LENGTH-25, 0);
              for(int cpt2=0;cpt2<3;cpt2++)
                {
                LCDD_PutBitmap(((LCD_X_LENGTH-1)/2)-25, (LCD_Y_LENGTH-1)/2, &BitmapInfo_SendData_big[cpt2], 0);
                LCDD_Refresh();
                TIME_Wait(200);
                }
              }
            LCDD_ClearRect(0, 16, LCD_X_LENGTH-1, LCD_Y_LENGTH-25, 0);
            pageUpdate = 1;
        }

        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)    // Effacement
        {
            commande=0x02;
            IR_BAES(nombredadresses,commande);

            for(int cpt=0;cpt<NB_REPEAT_BMP;cpt++)
              {
              LCDD_ClearRect(0, 16, LCD_X_LENGTH-1, LCD_Y_LENGTH-25, 0);
              LCDD_PutString(PAGE_LINE_INDENT+35, 16 , 0, "Effacement", 0, 0);
              for(int cpt2=0;cpt2<3;cpt2++)
                {
                LCDD_PutBitmap(((LCD_X_LENGTH-1)/2)-25, (LCD_Y_LENGTH-1)/2, &BitmapInfo_DeleteData_big[cpt2], 0);
                LCDD_Refresh();
                TIME_Wait(175);
                }
              TIME_Wait(200);
              }
            LCDD_ClearRect(0, 16, LCD_X_LENGTH-1, LCD_Y_LENGTH-25, 0);
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F4)
        {
            nombredadresses = 0 ;
            currentItem = 3;
            pageSwap = 1;
        }
    }
    if (pageMainBattery != BATTERY_GetCurrentLevel())
    {
        pageMainBattery = BATTERY_GetCurrentLevel();
        pageUpdate = 1;
    }

    _common_battery_condition();

    if (pageSwap)
    {
        //free(pZonesItems);
        pageSwap = 0;
        pageInitialize = 1;
    }
}

void TesterPageHandler(void)// PAGE TEST Adresse
{

    #define PAGE_MAIN_BATTERY_BODY_LENGTH       19
    #define PAGE_MAIN_BATTERY_BODY_HEIGHT       10
    #define PAGE_MAIN_BATTERY_BODY_X_START      (LCD_X_LENGTH-2-PAGE_MAIN_BATTERY_BODY_LENGTH)
    #define PAGE_MAIN_BATTERY_BODY_Y_START      2
    #define PAGE_MAIN_BATTERY_NIPPLE_LENGTH     2
    #define PAGE_MAIN_BATTERY_NIPPLE_HEIGHT     6
    #define PAGE_MAIN_BATTERY_NIPPLE_X_START    (PAGE_MAIN_BATTERY_BODY_X_START-PAGE_MAIN_BATTERY_NIPPLE_LENGTH)
    #define PAGE_MAIN_BATTERY_NIPPLE_Y_START    4
    #define PAGE_MAIN_BATTERY_FUEL_X_START      (PAGE_MAIN_BATTERY_BODY_X_START+2)
    #define PAGE_MAIN_BATTERY_FUEL_Y_START      (PAGE_MAIN_BATTERY_BODY_Y_START+2)
    #define PAGE_MAIN_BATTERY_FUEL_LENGTH       15
    #define PAGE_MAIN_BATTERY_FUEL_HEIGHT       6
    #define NON 1
    #define OUI 0

    unsigned char diz;
    unsigned char uni;
    unsigned char cen;
    unsigned char mil;
    static unsigned char pageMainBattery = 0;
    static unsigned char currentItem = 3;
    unsigned char commande;
    static unsigned int nombredadresses;

    if(nombredadresses>1022)
        {
        nombredadresses=1022;
        }
        else
        {
        mil=nombredadresses/1000;
        cen=(nombredadresses-(mil*1000))/100;
        diz=(nombredadresses-(mil*1000)-(cen*100))/10;
        uni=nombredadresses-(mil*1000)-(cen*100)-(diz*10);
        }

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);
	LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "TESTER"), 0, 0, "TESTER", 0, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
         pageFunckeyItems[1].pItem = (void *)&BitmapInfo_SendData;
        pageFunckeyItems[2].pItem = (void *)&BitmapInfo_DeleteData;
        pageFunckeyItems[3].pItem = (void *)&BitmapInfo_ResetData;

        _common_funckey_initializer();

        LCDD_ClearRect(PAGE_MAIN_BATTERY_NIPPLE_X_START, PAGE_MAIN_BATTERY_NIPPLE_Y_START, PAGE_MAIN_BATTERY_NIPPLE_X_START+PAGE_MAIN_BATTERY_NIPPLE_LENGTH-1, PAGE_MAIN_BATTERY_NIPPLE_Y_START+PAGE_MAIN_BATTERY_NIPPLE_HEIGHT-1, 3);
        LCDD_ClearRect(PAGE_MAIN_BATTERY_BODY_X_START, PAGE_MAIN_BATTERY_BODY_Y_START, PAGE_MAIN_BATTERY_BODY_X_START+PAGE_MAIN_BATTERY_BODY_LENGTH-1, PAGE_MAIN_BATTERY_BODY_Y_START+PAGE_MAIN_BATTERY_BODY_HEIGHT-1, 3);

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
       pageUpdate = 0;

       LCDD_ClearRect(0, 16, LCD_X_LENGTH-1, LCD_Y_LENGTH-25, 0);

       LCDD_ClearRect(4 + (currentItem * 30), 34 , 4 + (currentItem * 30) + 27 , 34 + 47  , 2);
       LCDD_ClearRect(6 + (currentItem * 30), 36 , 4 + (currentItem * 30) + 25 , 34 + 45  , 0);

        LCDD_PutBitmap(14 + (currentItem * 30), 26 , &BitmapInfo_Digit[5], 0);
       _draw_Digit(6, 38, mil);
       _draw_Digit(36, 38, cen);
       _draw_Digit(66, 38, diz);
       _draw_Digit(96, 38, uni);
       LCDD_PutBitmap(14 + (currentItem * 30) ,82 , &BitmapInfo_Digit[3], 0);

       LCDD_ClearRect(PAGE_MAIN_BATTERY_FUEL_X_START-1, PAGE_MAIN_BATTERY_FUEL_Y_START-1, PAGE_MAIN_BATTERY_FUEL_X_START+PAGE_MAIN_BATTERY_FUEL_LENGTH, PAGE_MAIN_BATTERY_FUEL_Y_START+PAGE_MAIN_BATTERY_FUEL_HEIGHT, 0);
       if (pageMainBattery)
        {
            LCDD_ClearRect(PAGE_MAIN_BATTERY_FUEL_X_START+PAGE_MAIN_BATTERY_FUEL_LENGTH-pageMainBattery, PAGE_MAIN_BATTERY_FUEL_Y_START, PAGE_MAIN_BATTERY_FUEL_X_START+PAGE_MAIN_BATTERY_FUEL_LENGTH-1, PAGE_MAIN_BATTERY_FUEL_Y_START+PAGE_MAIN_BATTERY_FUEL_HEIGHT-1, 2);
        }
    }
    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED)/* || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED)*/)
    {
        if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_UP))
        {
          if (currentItem == 3)
           {
             if ((uni<9)&&(nombredadresses < 1022)) uni++;
             else uni = 0;
           }
          if (currentItem == 2)
           {
             if ((diz<9)&&(nombredadresses < 1012)) diz++;
             else diz = 0;
           }
          if (currentItem == 1)
           {
             if ((cen<9)&&(nombredadresses < 922)) cen++;
             else cen = 0;
           }
          if (currentItem == 0)
           {
             if ((mil==0)&&(nombredadresses < 22)) mil++;
             else mil = 0;
           }

          nombredadresses = (mil*1000) + (cen*100) + (diz*10) + uni;
          pageUpdate = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN))
        {
          if (currentItem == 3)
           {
            if (nombredadresses < 1020)
              {
               if (uni>0) uni--;
                    else uni = 9;
              }
           }
          if (currentItem == 2)
           {
            if (nombredadresses < 1000)
             {
              if (diz>0) diz--;
              else diz = 9;
             }
             else
              {
               if (diz>0) diz--;
               else diz = 2;
              }           }
          if (currentItem == 1)
           {
              if (nombredadresses < 1000)
              {
               if (cen>0) cen--;
                   else cen = 9;
              }
           }
          if (currentItem == 0)
           {
             if (mil == 1) mil--;
             else if (nombredadresses > 1000) mil = 1;
           }

          nombredadresses = (mil*1000) + (cen*100) + (diz*10) + uni;
          pageUpdate = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT))
        {
              if(currentItem>0)
              {
                currentItem--;
              }
              else currentItem=0;
              pageUpdate = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT))
        {
              if(currentItem<3)
              {
                currentItem++;
              }
              else currentItem=3;
              pageUpdate = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR))
        {
    //        commande=0x01;
    //        IR_BAES(nombredadresses,commande);
    //        pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
           pageCurrent = PAGE_INDEX_MAIN;
           pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F2)
        {
                //    Commande Test adresse
         IR_BAES(nombredadresses, 0x04);

         for(int cpt=0;cpt<NB_REPEAT_BMP;cpt++)
            {
              LCDD_ClearRect(0, 16, LCD_X_LENGTH-1, LCD_Y_LENGTH-25, 0);
              for(int cpt2=0;cpt2<3;cpt2++)
              {
                LCDD_PutBitmap(((LCD_X_LENGTH-1)/2)-25, (LCD_Y_LENGTH-1)/2, &BitmapInfo_SendData_big[cpt2], 0);
                LCDD_Refresh();
                TIME_Wait(200);
              }
            }
         LCDD_ClearRect(0, 16, LCD_X_LENGTH-1, LCD_Y_LENGTH-25, 0);
         pageUpdate = 1;
        }
       else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            commande=0x02;                    // Commande effacement
            IR_BAES(nombredadresses,commande);

            for(int cpt=0;cpt<NB_REPEAT_BMP;cpt++)
              {
              LCDD_ClearRect(0, 16, LCD_X_LENGTH-1, LCD_Y_LENGTH-25, 0);
              LCDD_PutString(PAGE_LINE_INDENT+35, 16 , 0, "Effacement", 0, 0);
              for(int cpt2=0;cpt2<3;cpt2++)
                {
                LCDD_PutBitmap(((LCD_X_LENGTH-1)/2)-25, (LCD_Y_LENGTH-1)/2, &BitmapInfo_DeleteData_big[cpt2], 0);
                LCDD_Refresh();
                TIME_Wait(175);
                }
              TIME_Wait(200);
              }
            LCDD_ClearRect(0, 16, LCD_X_LENGTH-1, LCD_Y_LENGTH-25, 0);
            pageUpdate = 1;
        }
       else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F4)
        {
           nombredadresses = 0 ;
           currentItem = 3;
           pageSwap = 1;
        }
    }
    if (pageMainBattery != BATTERY_GetCurrentLevel())
    {
        pageMainBattery = BATTERY_GetCurrentLevel();
        pageUpdate = 1;
    }

    _common_battery_condition();

    if (pageSwap)
    {
        //free(pZonesItems);
        pageSwap = 0;
        pageInitialize = 1;
    }
}
#endif  // #ifndef APP_BAES

void SettingsPageHandler(void)
{
//#define PAGE_SETTINGS_TITLE_X_POS    40
#ifndef NO_APPSWITCH
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   4
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   3
    #endif  // #ifndef APP_BAES
#else
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   3
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#endif  // #ifndef NO_APPSWITCH
#define PAGE_SETTINGS_LINE_HEIGHT    16
#define PAGE_SETTINGS_LINE_SPACE     6
#define PAGE_SETTINGS_LINE_1         16
#define PAGE_SETTINGS_LINE_2         (PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_3         (PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_4         (PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)

    static unsigned char currentItem = 0;
    static const unsigned char targetPage[PAGE_SETTINGS_MAXIMUMITEMS] = {
#ifndef APP_BAES
        PAGE_INDEX_LANGUAGES,
#endif  // #ifndef APP_BAES
        PAGE_INDEX_VERSION,
        PAGE_INDEX_BATTERY,
#ifndef NO_APPSWITCH
        PAGE_INDEX_APPSWITCH,
#endif  // #ifndef NO_APPSWITCH
    };
#ifndef APP_BAES
    static unsigned char enterHiddenPage;
    static unsigned int timeStamp;
#endif  // #ifndef APP_BAES

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        _common_funckey_initializer();

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_SETTINGS ]), 0, 0, strings[STRINGS_INDEX_SETTINGS ], 0, 0);

//        currentItem = 0;

#ifndef APP_BAES
        enterHiddenPage = 0;

        timeStamp = 0xffffffff; // Initial timeStamp to infinity
#endif  // #ifndef APP_BAES

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
#ifndef APP_BAES
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, strings[STRINGS_INDEX_LANGUAGES ], (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, strings[STRINGS_INDEX_VERSION ], (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, strings[STRINGS_INDEX_BATTERY ], (currentItem == 2)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 3)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_4, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 3)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_4, (currentItem == 3)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#else
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, strings[STRINGS_INDEX_VERSION ], (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, strings[STRINGS_INDEX_BATTERY ], (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 2)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#endif  // #ifndef APP_BAES

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == PAGE_SETTINGS_MAXIMUMITEMS-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = PAGE_SETTINGS_MAXIMUMITEMS-1;
            }
            else
            {
                currentItem--;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            pageCurrent = targetPage[currentItem];

            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }

#ifndef APP_BAES
        if (((BUTTON_GetButtonIndex() == BUTTON_INDEX_F3) && (enterHiddenPage == 0)) || \
            ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) && (enterHiddenPage == 1)) || \
            ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F4) && (enterHiddenPage == 2))    \
           )
        {
            enterHiddenPage++;
            timeStamp = TIME_GetTime() + 2000;      // Set timeStamp to 2 seconds after
        }
        else
        {
            enterHiddenPage = 0;
            timeStamp = 0xffffffff;     // Reset timeStamp to infinity
        }
#endif  // #ifndef APP_BAES

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

#ifndef APP_BAES
    if (TIME_GetTime() >= timeStamp)
    {
        if (enterHiddenPage == 3)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;
            pageSwap = 1;
        }
        else
        {
            enterHiddenPage = 0;
            timeStamp = 0xffffffff;     // Reset timeStamp to infinity
        }
    }
#endif  // #ifndef APP_BAES

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }
}

#ifndef APP_BAES
// 2.4.0 rewrite
void LanguagesPageHandler(void)
{
#define PAGE_LANGUAGES_TOTALITEMS       5
#define PAGE_LANGUAGES_NAME_X_POS       2
#define PAGE_LANGUAGES_VALUE_X_POS      106
#define PAGE_LANGUAGES_LINE_HEIGHT      16
#define PAGE_LANGUAGES_LINE_SPACE       6
#define PAGE_LANGUAGES_LINE_1           17
#define PAGE_LANGUAGES_LINE_2           (PAGE_LANGUAGES_LINE_1+PAGE_LANGUAGES_LINE_HEIGHT+PAGE_LANGUAGES_LINE_SPACE)
#define PAGE_LANGUAGES_LINE_3           (PAGE_LANGUAGES_LINE_2+PAGE_LANGUAGES_LINE_HEIGHT+PAGE_LANGUAGES_LINE_SPACE)
#define PAGE_LANGUAGES_LINE_4           (PAGE_LANGUAGES_LINE_3+PAGE_LANGUAGES_LINE_HEIGHT+PAGE_LANGUAGES_LINE_SPACE)

    static const unsigned char ver[] = {'V','e','r',':',' ',VERSION_NUMBER,'\0'};
    static unsigned char currentItem;
    static unsigned char tempLanguage;
    unsigned char firstDisplayedItem;

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        _common_funckey_initializer();

        if (language == Unknown)
        {
            pageParent = PAGE_INDEX_MAIN;
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, ver), 0, 0, ver, 0, 0);
        }
        else
        {
            pageParent = PAGE_INDEX_SETTINGS;
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_LANGUAGES ]), 0, 0, strings[STRINGS_INDEX_LANGUAGES ], 0, 0);
        }

        currentItem = language;
        tempLanguage = currentItem;

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        firstDisplayedItem = 0;

#if (PAGE_LANGUAGES_TOTALITEMS > 4)
        if ((currentItem != Unknown) && (currentItem > 2))
        {
            firstDisplayedItem = currentItem - 2;

            if (currentItem == PAGE_LANGUAGES_TOTALITEMS - 1)
            {
                firstDisplayedItem = PAGE_LANGUAGES_TOTALITEMS - 4;
            }
        }
#endif

        if (PAGE_LANGUAGES_TOTALITEMS >= 1)
        {
            LCDD_ClearRect(0, PAGE_LANGUAGES_LINE_1-1, LCD_X_LENGTH-1, PAGE_LANGUAGES_LINE_1+16, (currentItem == firstDisplayedItem)?PAGE_HIGHLIGHT_COLOR:0);
            LCDD_PutString(PAGE_LANGUAGES_NAME_X_POS, PAGE_LANGUAGES_LINE_1, 0, strings_language[firstDisplayedItem], (currentItem == firstDisplayedItem)?PAGE_HIGHLIGHT_COLOR:0, 0);
            LCDD_ClearRect(PAGE_LANGUAGES_VALUE_X_POS-2, PAGE_LANGUAGES_LINE_1, LCD_X_LENGTH-2, PAGE_LANGUAGES_LINE_1+15, 0);
            if (tempLanguage == firstDisplayedItem)
            {
                LCDD_PutBitmap(_figure_icon_center_pos(PAGE_LANGUAGES_VALUE_X_POS, LCD_X_LENGTH-2, BitmapInfo_Validation[1].Width), PAGE_LANGUAGES_LINE_1, &BitmapInfo_Validation[1], 0);
            }
            else
            {
                LCDD_PutBitmap(_figure_icon_center_pos(PAGE_LANGUAGES_VALUE_X_POS, LCD_X_LENGTH-2, BitmapInfo_Validation[0].Width), PAGE_LANGUAGES_LINE_1, &BitmapInfo_Validation[0], 0);
            }
        }

        if (PAGE_LANGUAGES_TOTALITEMS >= 2)
        {
            LCDD_ClearRect(0, PAGE_LANGUAGES_LINE_2-1, LCD_X_LENGTH-1, PAGE_LANGUAGES_LINE_2+16, (currentItem == firstDisplayedItem+1)?PAGE_HIGHLIGHT_COLOR:0);
            LCDD_PutString(PAGE_LANGUAGES_NAME_X_POS, PAGE_LANGUAGES_LINE_2, 0, strings_language[firstDisplayedItem+1], (currentItem == firstDisplayedItem+1)?PAGE_HIGHLIGHT_COLOR:0, 0);
            LCDD_ClearRect(PAGE_LANGUAGES_VALUE_X_POS-2, PAGE_LANGUAGES_LINE_2, LCD_X_LENGTH-2, PAGE_LANGUAGES_LINE_2+15, 0);
            if (tempLanguage == firstDisplayedItem+1)
            {
                LCDD_PutBitmap(_figure_icon_center_pos(PAGE_LANGUAGES_VALUE_X_POS, LCD_X_LENGTH-2, BitmapInfo_Validation[1].Width), PAGE_LANGUAGES_LINE_2, &BitmapInfo_Validation[1], 0);
            }
            else
            {
                LCDD_PutBitmap(_figure_icon_center_pos(PAGE_LANGUAGES_VALUE_X_POS, LCD_X_LENGTH-2, BitmapInfo_Validation[0].Width), PAGE_LANGUAGES_LINE_2, &BitmapInfo_Validation[0], 0);
            }
        }

        if (PAGE_LANGUAGES_TOTALITEMS >= 3)
        {
            LCDD_ClearRect(0, PAGE_LANGUAGES_LINE_3-1, LCD_X_LENGTH-1, PAGE_LANGUAGES_LINE_3+16, (currentItem == firstDisplayedItem+2)?PAGE_HIGHLIGHT_COLOR:0);
            LCDD_PutString(PAGE_LANGUAGES_NAME_X_POS, PAGE_LANGUAGES_LINE_3, 0, strings_language[firstDisplayedItem+2], (currentItem == firstDisplayedItem+2)?PAGE_HIGHLIGHT_COLOR:0, 0);
            LCDD_ClearRect(PAGE_LANGUAGES_VALUE_X_POS-2, PAGE_LANGUAGES_LINE_3, LCD_X_LENGTH-2, PAGE_LANGUAGES_LINE_3+15, 0);
            if (tempLanguage == firstDisplayedItem+2)
            {
                LCDD_PutBitmap(_figure_icon_center_pos(PAGE_LANGUAGES_VALUE_X_POS, LCD_X_LENGTH-2, BitmapInfo_Validation[1].Width), PAGE_LANGUAGES_LINE_3, &BitmapInfo_Validation[1], 0);
            }
            else
            {
                LCDD_PutBitmap(_figure_icon_center_pos(PAGE_LANGUAGES_VALUE_X_POS, LCD_X_LENGTH-2, BitmapInfo_Validation[0].Width), PAGE_LANGUAGES_LINE_3, &BitmapInfo_Validation[0], 0);
            }
        }

        if (PAGE_LANGUAGES_TOTALITEMS >= 4)
        {
            LCDD_ClearRect(0, PAGE_LANGUAGES_LINE_4-1, LCD_X_LENGTH-1, PAGE_LANGUAGES_LINE_4+16, (currentItem == firstDisplayedItem+3)?PAGE_HIGHLIGHT_COLOR:0);
            LCDD_PutString(PAGE_LANGUAGES_NAME_X_POS, PAGE_LANGUAGES_LINE_4, 0, strings_language[firstDisplayedItem+3], (currentItem == firstDisplayedItem+3)?PAGE_HIGHLIGHT_COLOR:0, 0);
            LCDD_ClearRect(PAGE_LANGUAGES_VALUE_X_POS-2, PAGE_LANGUAGES_LINE_4, LCD_X_LENGTH-2, PAGE_LANGUAGES_LINE_4+15, 0);
            if (tempLanguage == firstDisplayedItem+3)
            {
                LCDD_PutBitmap(_figure_icon_center_pos(PAGE_LANGUAGES_VALUE_X_POS, LCD_X_LENGTH-2, BitmapInfo_Validation[1].Width), PAGE_LANGUAGES_LINE_4, &BitmapInfo_Validation[1], 0);
            }
            else
            {
                LCDD_PutBitmap(_figure_icon_center_pos(PAGE_LANGUAGES_VALUE_X_POS, LCD_X_LENGTH-2, BitmapInfo_Validation[0].Width), PAGE_LANGUAGES_LINE_4, &BitmapInfo_Validation[0], 0);
            }
        }

        if (tempLanguage != Unknown)
        {
            pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
            _common_funckey_initializer();
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if ((currentItem == French) || (currentItem == Unknown))
            {
                currentItem = German;
            }
            else
            {
                currentItem--;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if ((currentItem == German) || (currentItem == Unknown))
            {
                currentItem = French;
            }
            else
            {
                currentItem++;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR)
        {
            tempLanguage = currentItem;

            pageUpdate = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F1) && (tempLanguage != Unknown))
        {
            language = (LANGUAGES)tempLanguage;
//            DF_Erase(LANGUAGES_ADDRESS);
            DF_Write(&tempLanguage, 1, LANGUAGES_ADDRESS);

            pageCurrent = pageParent;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }
}
#endif  // #ifndef APP_BAES

void VersionPageHandler(void)
{
//#define PAGE_VERSION_TITLE_X_POS    36
#define PAGE_VERSION_BODY_Y_POS     20

    static const unsigned char version[] = {'F','i','r','m','w','a','r','e',':',' ',VERSION_NUMBER,'\0'};

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        _common_funckey_initializer();

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_VERSION ]), 0, 0, strings[STRINGS_INDEX_VERSION ], 0, 0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_VERSION_BODY_Y_POS, 0, version, 0, 0);
    }

    if (pageUpdate)
    {
        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_SETTINGS;

            pageSwap = 1;
        }
    }

    _common_battery_condition();

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }
}

void BatteryPageHandler(void)
{
#define PAGE_BATTERY_TOP_SECTION_HEIGHT     16
#define PAGE_BATTERY_LEVEL_LENGTH           70
#define PAGE_BATTERY_LEVEL_HEIGHT           26
#define PAGE_BATTERY_LEVEL_X_POS            ((LCD_X_LENGTH-PAGE_BATTERY_LEVEL_LENGTH)/2)
#define PAGE_BATTERY_LEVEL_Y_POS            (((LCD_Y_LENGTH-PAGE_BATTERY_TOP_SECTION_HEIGHT-PAGE_FUNCKEY_AREA_HEIGHT-PAGE_BATTERY_LEVEL_HEIGHT)/2+PAGE_BATTERY_TOP_SECTION_HEIGHT)/4*4)
#define PAGE_BATTERY_NIPPLE_LENGTH          4
#define PAGE_BATTERY_NIPPLE_HEIGHT          16
#define PAGE_BATTERY_NIPPLE_X_POS           PAGE_BATTERY_LEVEL_X_POS
#define PAGE_BATTERY_NIPPLE_Y_POS           (PAGE_BATTERY_LEVEL_Y_POS+(PAGE_BATTERY_LEVEL_HEIGHT-PAGE_BATTERY_NIPPLE_HEIGHT)/2)
#define PAGE_BATTERY_BODY_LENGTH            (PAGE_BATTERY_LEVEL_LENGTH-PAGE_BATTERY_NIPPLE_LENGTH)
#define PAGE_BATTERY_BODY_HEIGHT            PAGE_BATTERY_LEVEL_HEIGHT
#define PAGE_BATTERY_BODY_X_POS             (PAGE_BATTERY_LEVEL_X_POS+PAGE_BATTERY_NIPPLE_LENGTH)
#define PAGE_BATTERY_BODY_Y_POS             PAGE_BATTERY_LEVEL_Y_POS
#define PAGE_BATTERY_FUEL_LENGTH            (PAGE_BATTERY_BODY_LENGTH-6)
#define PAGE_BATTERY_FUEL_HEIGHT            (PAGE_BATTERY_BODY_HEIGHT-6)
#define PAGE_BATTERY_FUEL_X_POS             (PAGE_BATTERY_BODY_X_POS+3)
#define PAGE_BATTERY_FUEL_Y_POS             (PAGE_BATTERY_BODY_Y_POS+3)
#define PAGE_BATTERY_EXCLAMATION_LENGTH     44
#define PAGE_BATTERY_EXCLAMATION_HEIGHT     40
#define PAGE_BATTERY_EXCLAMATION_X_POS      ((LCD_X_LENGTH-PAGE_BATTERY_EXCLAMATION_LENGTH)/2)
#define PAGE_BATTERY_EXCLAMATION_Y_POS      (((LCD_Y_LENGTH-PAGE_BATTERY_TOP_SECTION_HEIGHT-PAGE_FUNCKEY_AREA_HEIGHT-PAGE_BATTERY_EXCLAMATION_HEIGHT)/2+PAGE_BATTERY_TOP_SECTION_HEIGHT)/4*4)

    static unsigned char pageBatteryLevel = 0;
    static unsigned int pageBatteryTimeStamp;

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, LCD_Y_LENGTH-PAGE_FUNCKEY_AREA_HEIGHT-1, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        _common_funckey_initializer();

        pageBatteryTimeStamp = TIME_GetTime() + 1000;

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-PAGE_FUNCKEY_AREA_HEIGHT-1, 0);

        if ((pageBatteryLevel <= BATTERY_LOW_LEVEL) && (!BATTERY_IsCharging()) && (TIME_GetTime() >= pageBatteryTimeStamp))
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_LOWBATTERY ]), 0, 0, strings[STRINGS_INDEX_LOWBATTERY ], 0, 0);

            for (unsigned char j=PAGE_BATTERY_EXCLAMATION_Y_POS/4; j<(PAGE_BATTERY_EXCLAMATION_Y_POS+PAGE_BATTERY_EXCLAMATION_HEIGHT)/4; j++)
            {
                for (unsigned char i=PAGE_BATTERY_EXCLAMATION_X_POS; i<PAGE_BATTERY_EXCLAMATION_X_POS+PAGE_BATTERY_EXCLAMATION_LENGTH; i++)
                {
                    lcdd.LcddFrameBuffer[j][i] = Bitmap_Exclamation[(j-PAGE_BATTERY_EXCLAMATION_Y_POS/4)*PAGE_BATTERY_EXCLAMATION_LENGTH+i-PAGE_BATTERY_EXCLAMATION_X_POS];
                }
            }
        }
        else
        {
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_BATTERYLEVEL ]), 0, 0, strings[STRINGS_INDEX_BATTERYLEVEL ], 0, 0);

            LCDD_ClearRect(PAGE_BATTERY_NIPPLE_X_POS, PAGE_BATTERY_NIPPLE_Y_POS, PAGE_BATTERY_NIPPLE_X_POS+PAGE_BATTERY_NIPPLE_LENGTH-1, PAGE_BATTERY_NIPPLE_Y_POS+PAGE_BATTERY_NIPPLE_HEIGHT-1, 3);
            LCDD_ClearRect(PAGE_BATTERY_BODY_X_POS, PAGE_BATTERY_BODY_Y_POS, PAGE_BATTERY_BODY_X_POS+PAGE_BATTERY_BODY_LENGTH-1, PAGE_BATTERY_BODY_Y_POS+PAGE_BATTERY_BODY_HEIGHT-1, 3);
            LCDD_ClearRect(PAGE_BATTERY_BODY_X_POS+2, PAGE_BATTERY_BODY_Y_POS+2, PAGE_BATTERY_BODY_X_POS+PAGE_BATTERY_BODY_LENGTH-3, PAGE_BATTERY_BODY_Y_POS+PAGE_BATTERY_BODY_HEIGHT-3, 0);
            if (pageBatteryLevel)
            {
                LCDD_ClearRect(PAGE_BATTERY_FUEL_X_POS+PAGE_BATTERY_FUEL_LENGTH-(PAGE_BATTERY_FUEL_LENGTH/(BATTERY_TOTALLEVELS-1)*pageBatteryLevel), PAGE_BATTERY_FUEL_Y_POS, PAGE_BATTERY_FUEL_X_POS+PAGE_BATTERY_FUEL_LENGTH-1, PAGE_BATTERY_FUEL_Y_POS+PAGE_BATTERY_FUEL_HEIGHT-1, 2);
            }
        }
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_SETTINGS;

            pageSwap = 1;
        }
    }

    if (pageBatteryLevel != BATTERY_GetCurrentLevel())
    {
        pageBatteryLevel = BATTERY_GetCurrentLevel();
        pageUpdate = 1;
    }

    if (TIME_GetTime() == pageBatteryTimeStamp)
    {
        pageUpdate = 1;
    }

    _common_battery_condition();

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }
}

#ifndef NO_APPSWITCH
void AppSwitchPageHandler(void)
{
#define PAGE_APPSWITCH_RECT_X_START     8
#define PAGE_APPSWITCH_RECT_Y_START     20
#define PAGE_APPSWITCH_RECT_X_END       (LCD_X_LENGTH-PAGE_APPSWITCH_RECT_X_START-1)
#define PAGE_APPSWITCH_BODY_INDENT      4
#define PAGE_APPSWITCH_BODY_X_START     (PAGE_APPSWITCH_RECT_X_START+PAGE_APPSWITCH_BODY_INDENT)
#define PAGE_APPSWITCH_BODY_X_LENGTH    (PAGE_APPSWITCH_RECT_X_END-PAGE_APPSWITCH_BODY_INDENT-PAGE_APPSWITCH_BODY_X_START)
#define PAGE_APPSWITCH_BODY_Y_START     (PAGE_APPSWITCH_RECT_Y_START+PAGE_APPSWITCH_BODY_INDENT)

    static unsigned char yOffset;
    static unsigned char appStatus;
    unsigned char pString[20];

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

#ifndef APP_BAES
        DF_Read(pString, sizeof(pString), DF_UPGRADE_EL_STATUS_PAGE*DF_PAGE_SIZE);
#else
        DF_Read(pString, sizeof(pString), DF_UPGRADE_LM_STATUS_PAGE*DF_PAGE_SIZE);
#endif  // #ifndef APP_BAES

        yOffset = PAGE_APPSWITCH_BODY_Y_START+LCDD_PutString(PAGE_APPSWITCH_BODY_X_START, PAGE_APPSWITCH_BODY_Y_START, PAGE_APPSWITCH_BODY_X_LENGTH, strings[STRINGS_INDEX_APPSWITCHPROMPT ], 3, 1)+PAGE_APPSWITCH_BODY_INDENT;
        LCDD_ClearRect(PAGE_APPSWITCH_RECT_X_START, PAGE_APPSWITCH_RECT_Y_START, PAGE_APPSWITCH_RECT_X_END, yOffset, 3);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        if (!strcmp((const char *)pString, "Good"))
        {
            appStatus = 1;
            LCDD_PutString(PAGE_APPSWITCH_BODY_X_START, PAGE_APPSWITCH_BODY_Y_START, PAGE_APPSWITCH_BODY_X_LENGTH, strings[STRINGS_INDEX_APPSWITCHPROMPT ], 3, 0);
            pageFunckeyItems[1].pItem = (void *)strings[STRINGS_INDEX_YES ];
            pageFunckeyItems[1].itemType = 1;
            pageFunckeyItems[2].pItem = (void *)strings[STRINGS_INDEX_NO_1 ];
            pageFunckeyItems[2].itemType = 1;
        }
        else
        {
            appStatus = 0;
            LCDD_PutString(PAGE_APPSWITCH_BODY_X_START, PAGE_APPSWITCH_BODY_Y_START, PAGE_APPSWITCH_BODY_X_LENGTH, strings[STRINGS_INDEX_NOAPPFOUND ], 3, 0);
            pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        }
        _common_funckey_initializer();

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F1) && (appStatus == 0))
        {
            pageCurrent = PAGE_INDEX_SETTINGS;

            pageSwap = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) && (appStatus == 1))
        {
            DF_Erase(DF_UPGRADE_FLAG_PAGE*DF_PAGE_SIZE);
#ifndef APP_BAES
            DF_Write("UPGRADE_EL", sizeof("UPGRADE_EL"), DF_UPGRADE_FLAG_PAGE*DF_PAGE_SIZE);
#else
            DF_Write("UPGRADE_LM", sizeof("UPGRADE_LM"), DF_UPGRADE_FLAG_PAGE*DF_PAGE_SIZE);
#endif  // #ifndef APP_BAES

            LCDD_ClearRect(PAGE_APPSWITCH_RECT_X_START, PAGE_APPSWITCH_RECT_Y_START, PAGE_APPSWITCH_RECT_X_END, yOffset, 3);
            LCDD_PutString(PAGE_APPSWITCH_BODY_X_START, PAGE_APPSWITCH_BODY_Y_START, PAGE_APPSWITCH_BODY_X_LENGTH, strings[STRINGS_INDEX_RESTART ], 3, 0);
            LCDD_Refresh();
            TIME_Wait(2000);

            pageCurrent = PAGE_INDEX_SETTINGS;

            pageSwap = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F3) && (appStatus == 1))
        {
            pageCurrent = PAGE_INDEX_SETTINGS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();


    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }
}
#endif  // #ifndef NO_APPSWITCH

#ifndef APP_BAES
void TestPageHandler(void)
{
#define PAGE_TEST_STRING_MAX_LENGTH     30
//#define PAGE_TEST_TOTAL_ITEMS           5     // 2.3.0
#define PAGE_TEST_MAXIMUM_ITEMS         5       // 2.3.0
#define PAGE_TEST_LINE_1                16
#define PAGE_TEST_LINE_HEIGHT           16
#define PAGE_TEST_LINE_SPACE            6
#define PAGE_TEST_LINE_2                (PAGE_TEST_LINE_1+PAGE_TEST_LINE_HEIGHT+PAGE_TEST_LINE_SPACE)
#define PAGE_TEST_LINE_3                (PAGE_TEST_LINE_2+PAGE_TEST_LINE_HEIGHT+PAGE_TEST_LINE_SPACE)
#define PAGE_TEST_LINE_4                (PAGE_TEST_LINE_3+PAGE_TEST_LINE_HEIGHT+PAGE_TEST_LINE_SPACE)

    static unsigned char currentItem = 0;   // 2.3.0
    static unsigned char totalItems;    // 2.3.0
    static unsigned char const *pStrings[PAGE_TEST_MAXIMUM_ITEMS];
    unsigned char firstDisplayedItem;

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

//        _param_page_funckey_initializer(FUNCKEY_WRITE_ONLY);      // 2.3.0

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_TEST ]), 0, 0, strings[STRINGS_INDEX_TEST ], 0, 0);     // 2.4.0

//        currentItem = 0;      // 2.3.0

        pStrings[0] = strings[STRINGS_INDEX_WALKTEST ];
        pStrings[1] = strings[STRINGS_INDEX_ENDTIMEDELAY ];
        pStrings[2] = strings[STRINGS_INDEX_INITIALSTATE ];
        // 2.3.0
        if (PARAMS_IsZigbeeSensor(&sensorParams))
        {
            pStrings[3] = strings[STRINGS_INDEX_CONTROL ];
            totalItems = 4;
        }
        else
        {
            pStrings[3] = strings[STRINGS_INDEX_ON ];       // 2.4.0
            pStrings[4] = strings[STRINGS_INDEX_OFF ];      // 2.4.0
            totalItems = 5;
        }

        // 2.3.0
        if (currentItem > totalItems - 1)
        {
            currentItem = totalItems - 1;
        }

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        firstDisplayedItem = 0;

//#if (PAGE_TEST_TOTAL_ITEMS > 4)   // 2.3.0
        if (currentItem > 2)
        {
            firstDisplayedItem = currentItem - 2;

            if (currentItem == totalItems - 1)  // 2.3.0
            {
                firstDisplayedItem = totalItems - 4;    // 2.3.0
            }
        }
//#endif    // 2.3.0

        // 2.3.0
        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        if (!((currentItem == 3) && PARAMS_IsZigbeeSensor(&sensorParams)))
        {
            pageFunckeyItems[2].pItem = (void *)&BitmapInfo_WriteSensor;
        }
        _common_funckey_initializer();
        // 2.3.0

        if (totalItems >= 1)    // 2.3.0
        {
            LCDD_ClearRect(0, PAGE_TEST_LINE_1, LCD_X_LENGTH-1, PAGE_TEST_LINE_1+PAGE_TEST_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 0)?3:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_TEST_LINE_1, 0, pStrings[firstDisplayedItem], (currentItem - firstDisplayedItem == 0)?3:0, 0);
            // 2.3.0
            if ((firstDisplayedItem == 3) && PARAMS_IsZigbeeSensor(&sensorParams))
            {
                LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_TEST_LINE_1, (currentItem - firstDisplayedItem == 0) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
            }
            // 2.3.0
        }

        if (totalItems >= 2)    // 2.3.0
        {
            LCDD_ClearRect(0, PAGE_TEST_LINE_2, LCD_X_LENGTH-1, PAGE_TEST_LINE_2+PAGE_TEST_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 1)?3:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_TEST_LINE_2, 0, pStrings[firstDisplayedItem+1], (currentItem - firstDisplayedItem == 1)?3:0, 0);
            // 2.3.0
            if ((firstDisplayedItem+1 == 3) && PARAMS_IsZigbeeSensor(&sensorParams))
            {
                LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_TEST_LINE_2, (currentItem - firstDisplayedItem == 1) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
            }
            // 2.3.0
        }

        if (totalItems >= 3)    // 2.3.0
        {
            LCDD_ClearRect(0, PAGE_TEST_LINE_3, LCD_X_LENGTH-1, PAGE_TEST_LINE_3+PAGE_TEST_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 2)?3:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_TEST_LINE_3, 0, pStrings[firstDisplayedItem+2], (currentItem - firstDisplayedItem == 2)?3:0, 0);
            // 2.3.0
            if ((firstDisplayedItem+2 == 3) && PARAMS_IsZigbeeSensor(&sensorParams))
            {
                LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_TEST_LINE_3, (currentItem - firstDisplayedItem == 2) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
            }
            // 2.3.0
        }

        if (totalItems >= 4)    // 2.3.0
        {
            LCDD_ClearRect(0, PAGE_TEST_LINE_4, LCD_X_LENGTH-1, PAGE_TEST_LINE_4+PAGE_TEST_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 3)?3:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_TEST_LINE_4, 0, pStrings[firstDisplayedItem+3], (currentItem - firstDisplayedItem == 3)?3:0, 0);
            // 2.3.0
            if ((firstDisplayedItem+3 == 3) && PARAMS_IsZigbeeSensor(&sensorParams))
            {
                LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_TEST_LINE_4, (currentItem - firstDisplayedItem == 3) ? &BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
            }
            // 2.3.0
        }
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == totalItems-1)    // 2.3.0
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = totalItems-1;     // 2.3.0
            }
            else
            {
                currentItem--;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }
        // 2.3.0
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            if ((currentItem == 3) && PARAMS_IsZigbeeSensor(&sensorParams))   // enter Zigbee sensor control page
            {
                pageCurrent = PAGE_INDEX_CONTROL;
                pageSwap = 1;
            }

            pageUpdate = 1;     // What for?
        }   // 2.3.0
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            switch(currentItem)
            {
            case 0:
                IR_DirectlySetSensorParameter(&parameterWalkTest);
                break;
            case 1:
                IR_DirectlySetSensorParameter(&parameterEndTimeDelay);
                break;
            case 2:
                IR_DirectlySetSensorParameter(&parameterInitialState);
                break;
            case 3:
                // 2.3.0
                if (!PARAMS_IsZigbeeSensor(&sensorParams))
                {
                    IR_PnLGroup(NITOO_LM058A_NODE, CODE_ON, 0);
                }   // 2.3.0
                break;
            case 4:
                IR_PnLGroup(NITOO_LM058A_NODE, CODE_OFF, 0);
                // fall over
            default:
                break;
            }
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }
}

// 2.3.0
void ControlPageHandler(void)
{
#define PAGE_CONTROL_STRING_MAX_LENGTH     30
#define PAGE_CONTROL_TOTAL_ITEMS           2
#define PAGE_CONTROL_LINE_1                16
#define PAGE_CONTROL_LINE_HEIGHT           16
#define PAGE_CONTROL_LINE_SPACE            6
#define PAGE_CONTROL_LINE_2                (PAGE_CONTROL_LINE_1+PAGE_CONTROL_LINE_HEIGHT+PAGE_CONTROL_LINE_SPACE)
#define PAGE_CONTROL_LINE_3                (PAGE_CONTROL_LINE_2+PAGE_CONTROL_LINE_HEIGHT+PAGE_CONTROL_LINE_SPACE)
#define PAGE_CONTROL_LINE_4                (PAGE_CONTROL_LINE_3+PAGE_CONTROL_LINE_HEIGHT+PAGE_CONTROL_LINE_SPACE)

    static unsigned char currentItem = 0;

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        pageFunckeyItems[2].pItem = (void *)strings[STRINGS_INDEX_ON ];     // 2.4.0
        pageFunckeyItems[2].itemType = 1;
        pageFunckeyItems[3].pItem = (void *)strings[STRINGS_INDEX_OFF ];    // 2.4.0
        pageFunckeyItems[3].itemType = 1;
        _common_funckey_initializer();

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_CONTROL ]), 0, 0, strings[STRINGS_INDEX_CONTROL ], 0, 0);

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        LCDD_ClearRect(0, PAGE_CONTROL_LINE_1, LCD_X_LENGTH-1, PAGE_CONTROL_LINE_1+PAGE_CONTROL_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_CONTROL_LINE_1, 0, strings[STRINGS_INDEX_LIGHTS ], (currentItem == 0)?3:0, 0);

        LCDD_ClearRect(0, PAGE_CONTROL_LINE_2, LCD_X_LENGTH-1, PAGE_CONTROL_LINE_2+PAGE_CONTROL_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_CONTROL_LINE_2, 0, strings[STRINGS_INDEX_AUXILIARIES ], (currentItem == 1)?3:0, 0);
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == PAGE_CONTROL_TOTAL_ITEMS-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = PAGE_CONTROL_TOTAL_ITEMS-1;
            }
            else
            {
                currentItem--;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_TEST;

            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            switch(currentItem)
            {
            case 0:
                IR_PnLGroup(NITOO_LM058A_UNIT_ZB_LG, CODE_ON, 0);
                break;
            case 1:
                IR_PnLGroup(NITOO_LM058A_UNIT_ZB_AG, CODE_ON, 0);
                // fall over
            default:
                break;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F4)
        {
            switch(currentItem)
            {
            case 0:
                IR_PnLGroup(NITOO_LM058A_UNIT_ZB_LG, CODE_OFF, 0);
                break;
            case 1:
                IR_PnLGroup(NITOO_LM058A_UNIT_ZB_AG, CODE_OFF, 0);
                // fall over
            default:
                break;
            }
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }
}   // 2.3.0

void HelpPageHandler(void)
{
#define PAGE_HELP_BODY_Y_POS        20

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        memset(pageFunckeyItems, 0, sizeof(pageFunckeyItems));
        pageFunckeyItems[0].pItem = (void *)&BitmapInfo_ArrowReturn;
        _common_funckey_initializer();

        LCDD_PutString(PAGE_LINE_INDENT, PAGE_HELP_BODY_Y_POS, LCD_X_LENGTH-2, strings[STRINGS_INDEX_HELP ], 0, 0);
    }

    if (pageUpdate)
    {
        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }
    }

    _common_battery_condition();

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }
}

#ifdef ROOM_CONTROL
void RoomControlPageHandler(void)
{
#define PAGE_ROOMCONTROL_TOTAL_ITEMS           2
#define PAGE_ROOMCONTROL_LINE_1                16
#define PAGE_ROOMCONTROL_LINE_HEIGHT           16
#define PAGE_ROOMCONTROL_LINE_SPACE            6
#define PAGE_ROOMCONTROL_LINE_2                (PAGE_ROOMCONTROL_LINE_1+PAGE_ROOMCONTROL_LINE_HEIGHT+PAGE_ROOMCONTROL_LINE_SPACE)
#define PAGE_ROOMCONTROL_LINE_3                (PAGE_ROOMCONTROL_LINE_2+PAGE_ROOMCONTROL_LINE_HEIGHT+PAGE_ROOMCONTROL_LINE_SPACE)
#define PAGE_ROOMCONTROL_LINE_4                (PAGE_ROOMCONTROL_LINE_3+PAGE_ROOMCONTROL_LINE_HEIGHT+PAGE_ROOMCONTROL_LINE_SPACE)

    static unsigned char currentItem;
    static unsigned char const *pStrings[PAGE_ROOMCONTROL_TOTAL_ITEMS];
    unsigned char firstDisplayedItem;

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        _param_page_funckey_initializer(FUNCKEY_WRITE_ONLY);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_ROOMCONTROL ]), 0, 0, strings[STRINGS_INDEX_ROOMCONTROL ], 0, 0);

        currentItem = 0;

        pStrings[0] = strings[STRINGS_INDEX_REINITIALIZEROOM ];
        pStrings[1] = strings[STRINGS_INDEX_RETURNTOFACTORY ];

        pageScroll = 0;  // 2.4.0

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
        pageUpdate = 0;

        firstDisplayedItem = 0;

#if (PAGE_ROOMCONTROL_TOTAL_ITEMS > 4)
        if (currentItem > 2)
        {
            firstDisplayedItem = currentItem - 2;

            if (currentItem == PAGE_ROOMCONTROL_TOTAL_ITEMS - 1)
            {
                firstDisplayedItem = PAGE_ROOMCONTROL_TOTAL_ITEMS - 4;
            }
        }
#endif

        if (PAGE_ROOMCONTROL_TOTAL_ITEMS >= 1)
        {
            LCDD_ClearRect(0, PAGE_ROOMCONTROL_LINE_1, LCD_X_LENGTH-1, PAGE_ROOMCONTROL_LINE_1+PAGE_ROOMCONTROL_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 0)?3:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_ROOMCONTROL_LINE_1, 0, pStrings[firstDisplayedItem], (currentItem - firstDisplayedItem == 0)?3:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_ROOMCONTROL_LINE_1, (LCD_X_LENGTH-PAGE_LINE_INDENT), pStrings[firstDisplayedItem]+((currentItem - firstDisplayedItem == 0)?pageScroll:0), (currentItem - firstDisplayedItem == 0)?3:0, (currentItem - firstDisplayedItem == 0)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
        }

        if (PAGE_ROOMCONTROL_TOTAL_ITEMS >= 2)
        {
            LCDD_ClearRect(0, PAGE_ROOMCONTROL_LINE_2, LCD_X_LENGTH-1, PAGE_ROOMCONTROL_LINE_2+PAGE_ROOMCONTROL_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 1)?3:0);
            // 2.4.0
//            LCDD_PutString(PAGE_LINE_INDENT, PAGE_ROOMCONTROL_LINE_2, 0, pStrings[firstDisplayedItem+1], (currentItem - firstDisplayedItem == 1)?3:0, 0);
            if (LCDD_PutScrollString(PAGE_LINE_INDENT, PAGE_ROOMCONTROL_LINE_2, (LCD_X_LENGTH-PAGE_LINE_INDENT), pStrings[firstDisplayedItem+1]+((currentItem - firstDisplayedItem == 1)?pageScroll:0), (currentItem - firstDisplayedItem == 1)?3:0, (currentItem - firstDisplayedItem == 1)?1:0))
            {
                pageScroll++;
                pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
            }
        }

        if (PAGE_ROOMCONTROL_TOTAL_ITEMS >= 3)
        {
            LCDD_ClearRect(0, PAGE_ROOMCONTROL_LINE_3, LCD_X_LENGTH-1, PAGE_ROOMCONTROL_LINE_3+PAGE_ROOMCONTROL_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 2)?3:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_ROOMCONTROL_LINE_3, 0, pStrings[firstDisplayedItem+2], (currentItem - firstDisplayedItem == 2)?3:0, 0);
        }

        if (PAGE_ROOMCONTROL_TOTAL_ITEMS >= 4)
        {
            LCDD_ClearRect(0, PAGE_ROOMCONTROL_LINE_4, LCD_X_LENGTH-1, PAGE_ROOMCONTROL_LINE_4+PAGE_ROOMCONTROL_LINE_HEIGHT-1, (currentItem - firstDisplayedItem == 3)?3:0);
            LCDD_PutString(PAGE_LINE_INDENT, PAGE_ROOMCONTROL_LINE_4, 0, pStrings[firstDisplayedItem+3], (currentItem - firstDisplayedItem == 3)?3:0, 0);
        }
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == PAGE_ROOMCONTROL_TOTAL_ITEMS-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

            pageScroll = 0;  // 2.4.0
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = PAGE_ROOMCONTROL_TOTAL_ITEMS-1;
            }
            else
            {
                currentItem--;
            }

            pageScroll = 0;  // 2.4.0
            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;

            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            switch(currentItem)
            {
            case 0:
                IR_PnLGroup(NITOO_LM058A_NODE, CODE_RESET_ROOM_CONTROL, CMD_RESET_ROOM_CONTROL);
                break;
            case 1:
                IR_PnLGroup(NITOO_LM058A_NODE, CODE_MOTION_SENSOR_RETURN_TO_FACTORY_CONFIG, CMD_MOTION_SENSOR_RETURN_TO_FACTORY_CONFIG);
                // fall over
            default:
                break;
            }
        }

//        BUTTON_Acknowledge();
    }

    // 2.4.0
    if (TIME_GetTime() >= pageTimeStamp)
    {
        pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
        pageUpdate = 1;
    }

    _common_battery_condition();

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }
}
#endif  // #ifdef ROOM_CONTROL
#endif  // #ifndef APP_BAES


void KNX_master_slave_PageHandler( void )
{
#define PAGE_ALERT_NAME_X_POS     2
#define PAGE_ALERT_VALUE_X_POS    106
#define PAGE_ALERT_BODY_Y_POS     18

    static PAGE_PARAMETER *pParameter;

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_KNX_MASTER_SLAVE );

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, KNX_sensor_master_slave_title_langeage_string[language]), 0, 0, KNX_sensor_master_slave_title_langeage_string[language], 0, 0);

        if (pageAvailability)
        {
            _param_page_funckey_initializer( FUNCKEY_READ_ONLY );

            pParameter = (PAGE_PARAMETER *)malloc(sizeof(PAGE_PARAMETER));

            pParameter->ParamIndex = PARAM_INDEX_KNX_MASTER_SLAVE;

            LCDD_PutString(PAGE_ALERT_NAME_X_POS, PAGE_ALERT_BODY_Y_POS, 0, KNX_sensor_master_langeage_string[language], 0, 0);
            
            LCDD_InvertRect(0, PAGE_ALERT_BODY_Y_POS-1, LCD_X_LENGTH-1, PAGE_ALERT_BODY_Y_POS+16, PAGE_HIGHLIGHT_COLOR);

            pageUpdate = 1;
        }
        else
        {
            _draw_rectangular_for_value();
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        LCDD_ClearRect(PAGE_ALERT_VALUE_X_POS-2, PAGE_ALERT_BODY_Y_POS, LCD_X_LENGTH-2, PAGE_ALERT_BODY_Y_POS+15, 0);
        if (PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_KNX_MASTER_SLAVE) == 0 )    // 0 reflect MASTER mode
        {
            LCDD_PutBitmap(_figure_icon_center_pos(PAGE_ALERT_VALUE_X_POS, LCD_X_LENGTH-2, BitmapInfo_Validation[1].Width), PAGE_ALERT_BODY_Y_POS, &BitmapInfo_Validation[1], 0);
        }

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {

        if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
                _page_sub_read_parameters( pParameter, 1 );
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_ADVANCEDMODE;

            pageSwap = 1;
        }
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}

unsigned summer_hour_check( void )
{
    unsigned int start_date,start_month,end_date,end_month; 
    unsigned int check_result;
    
    start_date = PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STARTDAY );
    start_month = PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_STARTMONTH );
    end_date =  PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_ENDDAY );
    end_month =  PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_ENDMONTH );
    if(  (start_month > end_month)
       ||((start_month == end_month) && (start_date >= end_date))
       ) 
    {
        check_result = false;
    }
    else
    {
        check_result = true;
    }
    
    return( check_result );
    
}




void DaliMasterPageHandler(void)
{
//#define PAGE_SETTINGS_TITLE_X_POS    40
#ifndef NO_APPSWITCH
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   4
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   3
    #endif  // #ifndef APP_BAES
#else
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   3
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#endif  // #ifndef NO_APPSWITCH
#define PAGE_SETTINGS_LINE_HEIGHT    16
#define PAGE_SETTINGS_LINE_SPACE     6
#define PAGE_SETTINGS_LINE_1         16
#define PAGE_SETTINGS_LINE_2         (PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_3         (PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_4         (PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)

    static unsigned char currentItem = 0;
    static const unsigned char targetPage[PAGE_SETTINGS_MAXIMUMITEMS] = {
        PAGE_CONTROL_GEAR,//PAGE_CONTROL_GEAR
        PAGE_DETECTOR,  //
        PAGE_CHANGE_TYPE,
    };
#ifndef APP_BAES
    static unsigned char enterHiddenPage;
    static unsigned int timeStamp;
#endif  // #ifndef APP_BAES

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);
        _param_pageDali_funckey_initializer(FUNCKEY_NONE);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "Master"), 0, 0, "Master", 0, 0);

//        currentItem = 0;

#ifndef APP_BAES
        enterHiddenPage = 0;

        timeStamp = 0xffffffff; // Initial timeStamp to infinity
#endif  // #ifndef APP_BAES

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
#ifndef APP_BAES
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, "Control gear", (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "Detector", (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, "Change", (currentItem == 2)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 3)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_4, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 3)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_4, (currentItem == 3)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#else
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, strings[STRINGS_INDEX_VERSION ], (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, strings[STRINGS_INDEX_BATTERY ], (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 2)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#endif  // #ifndef APP_BAES

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == PAGE_SETTINGS_MAXIMUMITEMS-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = PAGE_SETTINGS_MAXIMUMITEMS-1;
            }
            else
            {
                currentItem--;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            pageCurrent = targetPage[currentItem];

            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }

#ifndef APP_BAES
        if (((BUTTON_GetButtonIndex() == BUTTON_INDEX_F3) && (enterHiddenPage == 0)) || \
            ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) && (enterHiddenPage == 1)) || \
            ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F4) && (enterHiddenPage == 2))    \
           )
        {
            enterHiddenPage++;
            timeStamp = TIME_GetTime() + 2000;      // Set timeStamp to 2 seconds after
        }
        else
        {
            enterHiddenPage = 0;
            timeStamp = 0xffffffff;     // Reset timeStamp to infinity
        }
#endif  // #ifndef APP_BAES

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

#ifndef APP_BAES
    if (TIME_GetTime() >= timeStamp)
    {
        if (enterHiddenPage == 3)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;
            pageSwap = 1;
        }
        else
        {
            enterHiddenPage = 0;
            timeStamp = 0xffffffff;     // Reset timeStamp to infinity
        }
    }
#endif  // #ifndef APP_BAES

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }
}





void DaliSlavePageHandler(void)
{
#define PAGE_SENSITIVITY_MAXIMUMITEMS       3
#define PAGE_SENSITIVITY_NAME_X_POS         4
#define PAGE_SENSITIVITY_VALUE_X_POS        40
#define PAGE_SENSITIVITY_BRACKET_X_POS      114
#define PAGE_SENSITIVITY_BODY_Y_POS_1       18
#define PAGE_SENSITIVITY_BODY_Y_POS_DELTA   22
#define PAGE_SENSITIVITY_BODY_Y_POS_2       (PAGE_SENSITIVITY_BODY_Y_POS_1 + PAGE_SENSITIVITY_BODY_Y_POS_DELTA)
#define PAGE_SENSITIVITY_BODY_Y_POS_3       (PAGE_SENSITIVITY_BODY_Y_POS_2 + PAGE_SENSITIVITY_BODY_Y_POS_DELTA)

  static const PAGE_ITEM_RECT rect[1] = {{PAGE_SENSITIVITY_BODY_Y_POS_1-1, PAGE_SENSITIVITY_BODY_Y_POS_1+16}};//{PAGE_SENSITIVITY_BODY_Y_POS_2-1, PAGE_SENSITIVITY_BODY_Y_POS_2+16}, {PAGE_SENSITIVITY_BODY_Y_POS_3-1, PAGE_SENSITIVITY_BODY_Y_POS_3+16}};
   // static unsigned char previousItem;
    static unsigned char currentItem;
    static unsigned char totalItems=4;
    static PAGE_PARAMETER *pParameter;
    unsigned char yOffset, i;
    PAGE_PARAMETER *pTempParameter;
    yOffset = PAGE_SENSITIVITY_BODY_Y_POS_1;
    const unsigned char *ErrorStrings[]={"    ","W+B","C+B","W+C+B","W+C"}; 
    const unsigned char *strings[]={"Windows","Corridor","Board","Exclude"}; 
    static unsigned char currentItem_x;
   const unsigned char *PString;
       unsigned char pNum[2];
    if (pageInitialize)
    {
        pageInitialize = 0;
//        GroupNum.Index=0;
//        for(int i=0;i<32;i++)
//          GroupNum.GroupTmp[i]=0;

        pageAvailability = 1;//PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_HF_SENS) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_PIR_SENS) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_US_SENS);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

      //  LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1,0, 0, "Choose Zone", 0, 0);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "Slave"), 0, 0, "Slave", 0, 0); 
        
        if (pageAvailability)
        {
          _param_pageDali_funckey_initializer(FUNCKEY_READ_WRITE);
          currentItem = 0;
          pParameter = (PAGE_PARAMETER *)malloc(PAGE_SENSITIVITY_MAXIMUMITEMS*sizeof(PAGE_PARAMETER));
          pTempParameter = pParameter;
          yOffset = PAGE_SENSITIVITY_BODY_Y_POS_1;
          pageUpdate = 1;
        }
        
    }


    if (pageUpdate)
    {
      
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "View", (currentItem == 1)?3:0, 0);
       // LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, "Earase from this Group", (currentItem == 2)?3:0, 0);               
        
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 3)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_4, 0, "Earase All masters", (currentItem == 3)?3:0, 0);  
             
        if (currentItem == 0)
        {
          LCDD_ClearRect(PAGE_SENSITIVITY_NAME_X_POS, rect[currentItem].yStart, LCD_X_LENGTH-1, rect[currentItem].yEnd, 0);
          LCDD_PutString(PAGE_SENSITIVITY_NAME_X_POS, yOffset, 0, "Group", 0, 0);
          LCDD_InvertRect(PAGE_SENSITIVITY_NAME_X_POS, rect[currentItem].yStart, LCD_X_LENGTH-1, rect[currentItem].yEnd, PAGE_HIGHLIGHT_COLOR);           
        }
       else 
        {
           LCDD_ClearRect(PAGE_SENSITIVITY_NAME_X_POS, rect[0].yStart, LCD_X_LENGTH-1, rect[0].yEnd, 0);
           LCDD_PutString(PAGE_SENSITIVITY_NAME_X_POS, 18, 0, "Group", 0, 0);
        }
        

 //PString=strings[0];
        
         for (i=0; i<1; i++)
        {
            LCDD_ClearRect(PAGE_SENSITIVITY_VALUE_X_POS-4, 18, LCD_X_LENGTH-2, yOffset+15, 0);
            //LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS, 18, 0, strings[currentItem_x], 0, 0);      // 2.4.0
            if(GroupNum.Index>0)
            { 
              dectoascii(&pNum[0],GroupNum.GroupTmp[currentItem_x], 2);
              LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS, 18, 0,"Group", 0, 0); 
              LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS+35, 18, 0, &pNum[0], 0, 0); 
            }
            //LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS, 18, 0, PString, 0, 0); 
            LCDD_PutString(PAGE_SENSITIVITY_BRACKET_X_POS, 18, 0, "<>", 0, 0);           
        }
       pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
          if (pageAvailability)
          {
            if (currentItem == 0)
            {
              currentItem = totalItems-1;
            }
            else
            {
              currentItem--;
            }
            
            pageUpdate = 1;
          }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
          if (pageAvailability)
          {
            if (currentItem == totalItems-1)
            {
              currentItem = 0;
            }
            else
            {
              currentItem++;
            }
            
            pageUpdate = 1;
          }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
          if(currentItem==0 && GroupNum.Index>=1)
          {
            if (currentItem_x == 0)
            {
              currentItem_x = GroupNum.Index-1;
            }
            else
            {
              currentItem_x--;
            }
            
            //pageUpdate = 1;
             dectoascii(&pNum[0],GroupNum.GroupTmp[currentItem_x],2);
            LCDD_ClearRect(PAGE_SENSITIVITY_VALUE_X_POS-4, 18, LCD_X_LENGTH-2, yOffset+15, 0);
            LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS, 18, 0,"Group", 0, 0); 
            LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS+35, 18, 0, &pNum[0], 0, 0); 
            LCDD_PutString(PAGE_SENSITIVITY_BRACKET_X_POS, 18, 0, "<>", 0, 0);
          }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT)
        {
          if(currentItem==0 && GroupNum.Index>=1)
          {
            if (currentItem_x == (GroupNum.Index-1))
            {
              currentItem_x = 0;
            }
            else
            {
              currentItem_x++;
            }
            
            // pageUpdate = 1;
            //LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS, 18, 0, strings[currentItem_x], 0, 0);      // 2.4.0
            dectoascii(&pNum[0],GroupNum.GroupTmp[currentItem_x],2);
            LCDD_ClearRect(PAGE_SENSITIVITY_VALUE_X_POS-4, 18, LCD_X_LENGTH-2, yOffset+15, 0);
            LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS, 18, 0,"Group", 0, 0); 
            LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS+35, 18, 0,&pNum[0], 0, 0); 
            LCDD_PutString(PAGE_SENSITIVITY_BRACKET_X_POS, 18, 0, "<>", 0, 0);  
          }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F3))
        {
          if(currentItem==1)            
          {//indentify
              IR_ReadNBofControlGearParameter(NITOO_LM058A_NODE,0xB0,0x48,20,GroupNum.GroupTmp[currentItem_x]);
          }
          if(currentItem==2)            
          {
            unsigned char GroupBeSelect;
            GroupBeSelect=GroupNum.GroupTmp[currentItem_x];
//            if((GroupBeSelect>=0)&&(GroupBeSelect<=7))
//            {
//              IR_SetGroupNum(NITOO_LM058A_NODE,0xF0,0x4D,GroupBeSelect, 0x00, 0x00, 0x00);  
//            } 
//            if((GroupBeSelect>=8)&&(GroupBeSelect<=15))
//            {
//              IR_SetGroupNum(NITOO_LM058A_NODE,0xF0,0x4D,0x00,GroupBeSelect, 0x00, 0x00);  
//            }
//            if((GroupBeSelect>=16)&&(GroupBeSelect<=23))
//            {
//              IR_SetGroupNum(NITOO_LM058A_NODE,0xF0,0x4D,0x00,0x00,GroupBeSelect, 0x00);  
//            }
//            if((GroupBeSelect>=24)&&(GroupBeSelect<=31))
//            {
//              IR_SetGroupNum(NITOO_LM058A_NODE,0xF0,0x4D,0x00,0x00,0x00, GroupBeSelect);  
//            }
            DataGroup.DATA2=0;
            DataGroup.DATA3=0;
            DataGroup.DATA4=0;
            DataGroup.DATA5=0;
            DataGroup.flag=0;
            DataGroup=ConverGroup2Data(GroupBeSelect,DataGroup);
            IR_SetGroupNum(NITOO_LM058A_NODE,0xF0,0x46,DataGroup.DATA2, DataGroup.DATA3, DataGroup.DATA4,DataGroup.DATA5);            
            _page_read_GroupNumAll();
            for(int i=0;i<GroupNum.Index;i++)
            {      
              if(GroupNum.GroupTmp[i]==GroupBeSelect)
              {
                WaringBlock("WARNING","RESET ERROR"); 
                TIME_Wait(2000);
              }
            }
            currentItem_x=0;
          }
          if(currentItem==3)
          {
              IR_PnLGroup(NITOO_LM058A_NODE,0x70,0x4A);
          }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F2)
        {
          if(currentItem==1)            
          {//QUERY_D           
            IR_PnLGroup(NITOO_LM058A_NODE,0x70,0x44);
            GroupNum.Index=0;
            for(int i=0;i<32;i++)
              GroupNum.GroupTmp[i]=0;
            
            _page_read_GroupNumAll();
            currentItem_x=0;
          }

        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }


}



void DaliNopPageHandler(void)
{
//#define PAGE_SETTINGS_TITLE_X_POS    40
#ifndef NO_APPSWITCH
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   4
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   3
    #endif  // #ifndef APP_BAES
#else
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   3
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#endif  // #ifndef NO_APPSWITCH
#define PAGE_SETTINGS_LINE_HEIGHT    16
#define PAGE_SETTINGS_LINE_SPACE     6
#define PAGE_SETTINGS_LINE_1         16
#define PAGE_SETTINGS_LINE_2         (PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_3         (PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_4         (PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)

    static unsigned char currentItem = 0;
    static const unsigned char targetPage[PAGE_SETTINGS_MAXIMUMITEMS] = {
#ifndef APP_BAES
        PAGE_INDEX_LANGUAGES,
#endif  // #ifndef APP_BAES
        PAGE_INDEX_VERSION,
        PAGE_INDEX_BATTERY,
#ifndef NO_APPSWITCH
        PAGE_INDEX_APPSWITCH,
#endif  // #ifndef NO_APPSWITCH
    };
#ifndef APP_BAES
    static unsigned char enterHiddenPage;
    static unsigned int timeStamp;
#endif  // #ifndef APP_BAES

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);
       _param_pageDali_funckey_initializer(FUNCKEY_NONE);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "NOP"), 0, 0, strings[STRINGS_INDEX_SETTINGS ], 0, 0);

//        currentItem = 0;

#ifndef APP_BAES
        enterHiddenPage = 0;

        timeStamp = 0xffffffff; // Initial timeStamp to infinity
#endif  // #ifndef APP_BAES

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
#ifndef APP_BAES
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, "Change", (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

//        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
//        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "Detector", (currentItem == 1)?3:0, 0);
//        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
//
//        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
//        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, "Change", (currentItem == 2)?3:0, 0);
//        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 3)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_4, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 3)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_4, (currentItem == 3)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#else
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, strings[STRINGS_INDEX_VERSION ], (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, strings[STRINGS_INDEX_BATTERY ], (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 2)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#endif  // #ifndef APP_BAES

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == PAGE_SETTINGS_MAXIMUMITEMS-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = PAGE_SETTINGS_MAXIMUMITEMS-1;
            }
            else
            {
                currentItem--;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            pageCurrent = targetPage[currentItem];

            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }

#ifndef APP_BAES
        if (((BUTTON_GetButtonIndex() == BUTTON_INDEX_F3) && (enterHiddenPage == 0)) || \
            ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) && (enterHiddenPage == 1)) || \
            ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F4) && (enterHiddenPage == 2))    \
           )
        {
            enterHiddenPage++;
            timeStamp = TIME_GetTime() + 2000;      // Set timeStamp to 2 seconds after
        }
        else
        {
            enterHiddenPage = 0;
            timeStamp = 0xffffffff;     // Reset timeStamp to infinity
        }
#endif  // #ifndef APP_BAES

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

#ifndef APP_BAES
    if (TIME_GetTime() >= timeStamp)
    {
        if (enterHiddenPage == 3)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;
            pageSwap = 1;
        }
        else
        {
            enterHiddenPage = 0;
            timeStamp = 0xffffffff;     // Reset timeStamp to infinity
        }
    }
#endif  // #ifndef APP_BAES

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }
}
void DaliControlGearHandler(void)
{
//#define PAGE_SETTINGS_TITLE_X_POS    40
#ifndef NO_APPSWITCH
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   4
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   3
    #endif  // #ifndef APP_BAES
#else
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   3
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#endif  // #ifndef NO_APPSWITCH
#define PAGE_SETTINGS_LINE_HEIGHT    16
#define PAGE_SETTINGS_LINE_SPACE     6
#define PAGE_SETTINGS_LINE_1         16
#define PAGE_SETTINGS_LINE_2         (PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_3         (PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_4         (PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)

    static unsigned char currentItem = 0;
    static const unsigned char targetPage[PAGE_SETTINGS_MAXIMUMITEMS-1] = {
        PAGE_CREATE_ZONE,
        PAGE_VIEW_AREA,  
    };
#ifndef APP_BAES
    static unsigned char enterHiddenPage;
    static unsigned int timeStamp;
#endif  // #ifndef APP_BAES

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);
        _param_pageDali_funckey_initializer(FUNCKEY_NONE);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "CONTROL GEAR"), 0, 0, "CONTROL GEAR", 0, 0);

//        currentItem = 0;

#ifndef APP_BAES
        enterHiddenPage = 0;

        timeStamp = 0xffffffff; // Initial timeStamp to infinity
#endif  // #ifndef APP_BAES

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
#ifndef APP_BAES
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, "Create gear", (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "view area", (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, "erase", (currentItem == 2)?3:0, 0);
       // LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 3)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_4, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 3)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_4, (currentItem == 3)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#else
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, strings[STRINGS_INDEX_VERSION ], (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, strings[STRINGS_INDEX_BATTERY ], (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 2)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#endif  // #ifndef APP_BAES

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == PAGE_SETTINGS_MAXIMUMITEMS-1)
            {
                currentItem = 0;
                _draw_funckey_area_frame();
            }
            else
            {
                currentItem++;
            }
            
            if(currentItem==2)
            {
              _param_pageDali_funckey_initializer(FUNCKEY_WRITE_ONLY);  
            }
            else
            {
              _param_pageDali_funckey_initializer(FUNCKEY_NONE);
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = PAGE_SETTINGS_MAXIMUMITEMS-1;
            }
            else
            {
                currentItem--;
            }
            if(currentItem==2)
            {
              _param_pageDali_funckey_initializer(FUNCKEY_WRITE_ONLY);
              
            }
            else
            {
              _param_pageDali_funckey_initializer(FUNCKEY_NONE);
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
           /// pageCurrent = targetPage[currentItem];
            if((sensorParams.Type.Value==PARAM_TYPE_DALI_MASTER_DL_PIR_US) || (sensorParams.Type.Value==PARAM_TYPE_DALI_MASTER_DL_OP_PIR_US) || (sensorParams.Type.Value==PARAM_TYPE_DALI_MASTER_OP_PIR_US) )
            {//dual
              if(currentItem==1)
              {
                pageCurrent=PAGE_VIEW_AREA;
              }     
              else
                 pageCurrent = targetPage[currentItem];
            }
            else if((sensorParams.Type.Value==PARAM_TYPE_DALI_MASTER_DL_OP_PIR) || (sensorParams.Type.Value==PARAM_TYPE_DALI_MASTER_OP_PIR)|| (sensorParams.Type.Value==PARAM_TYPR_DALI_MASTER_DL_PIR) )
            {//corri
              if(currentItem==1)
              {
                pageCurrent=PAGE_VIEW_AREA_CORRID;
              }
              else
                 pageCurrent = targetPage[currentItem];
            }
            else
              pageCurrent = targetPage[currentItem];
              pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }
        
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F2)
        {
            //IR_PnLGroup(NITOO_LM058A_NODE, CODE_ERASE_GROUP_PNL, CMD_DELETE_ALL_CONTROL_GEAR);
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_START_GROUP_PNL, CMD_DELETE_ALL_CONTROL_GEAR);
        }
        
    }

    _common_battery_condition();

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }

}
void DaliDetectorHandler(void)
{
//#define PAGE_SETTINGS_TITLE_X_POS    40
#ifndef NO_APPSWITCH
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   4
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   3
    #endif  // #ifndef APP_BAES
#else
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   3
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#endif  // #ifndef NO_APPSWITCH
#define PAGE_SETTINGS_LINE_HEIGHT    16
#define PAGE_SETTINGS_LINE_SPACE     6
#define PAGE_SETTINGS_LINE_1         16
#define PAGE_SETTINGS_LINE_2         (PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_3         (PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_4         (PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)

    static unsigned char currentItem = 0;

#ifndef APP_BAES
    static unsigned char enterHiddenPage;
    static unsigned int timeStamp;
#endif  // #ifndef APP_BAES

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "Master"), 0, 0, "Master", 0, 0);
       _param_pageDali_funckey_initializer(FUNCKEY_WRITE_ONLY);
//        currentItem = 0;

#ifndef APP_BAES
        enterHiddenPage = 0;

        timeStamp = 0xffffffff; // Initial timeStamp to infinity
#endif  // #ifndef APP_BAES

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
#ifndef APP_BAES
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, "Create group", (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "Group start blink", (currentItem == 1)?3:0, 0);
        //LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, "Group stop blink", (currentItem == 2)?3:0, 0);
       // LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 3)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_4, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 3)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_4, (currentItem == 3)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#else
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, strings[STRINGS_INDEX_VERSION ], (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, strings[STRINGS_INDEX_BATTERY ], (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 2)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#endif  // #ifndef APP_BAES

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == PAGE_SETTINGS_MAXIMUMITEMS-1)
            {
                currentItem = 0;
                _draw_funckey_area_frame();
            }
            else
            {
                currentItem++;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = PAGE_SETTINGS_MAXIMUMITEMS-1;

            }
            else
            {
                currentItem--;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
          if(currentItem==0)
            pageCurrent = PAGE_INDEX_DetectorCreateGroup;

            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }
        
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F2)
        {
           // IR_PnLGroup(NITOO_LM058A_NODE, CODE_ERASE_GROUP_PNL, CMD_DELETE_ALL_CONTROL_GEAR);
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
           if(currentItem==1)
           {
             IR_ReadNBofControlGearParameter(NITOO_LM058A_NODE,0xB0,0x47,0x14,0x00);      
           }
            if(currentItem==2)
           {
             IR_ReadNBofControlGearParameter(NITOO_LM058A_NODE,0xB0,0x47,0x00,0x00);      
           }
           
        }
        
    }

    _common_battery_condition();

#ifndef APP_BAES
    if (TIME_GetTime() >= timeStamp)
    {
        if (enterHiddenPage == 3)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;
            pageSwap = 1;
        }
        else
        {
            enterHiddenPage = 0;
            timeStamp = 0xffffffff;     // Reset timeStamp to infinity
        }
    }
#endif  // #ifndef APP_BAES

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }



}

void DALIdetectorCreateGroupHandler(void)
{
//#define PAGE_SETTINGS_TITLE_X_POS    40
#ifndef NO_APPSWITCH
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   4
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#else
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#endif  // #ifndef NO_APPSWITCH
#define PAGE_SETTINGS_LINE_HEIGHT    16
#define PAGE_SETTINGS_LINE_SPACE     6
#define PAGE_SETTINGS_LINE_1         16
#define PAGE_SETTINGS_LINE_2         (PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_3         (PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_4         (PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)

    static unsigned char currentItem = 0;

#ifndef APP_BAES
    static unsigned char enterHiddenPage;
    static unsigned int timeStamp;
#endif  // #ifndef APP_BAES

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        _param_pageDali_funckey_initializer(FUNCKEY_READ_WRITE);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "Master"), 0, 0, "Master", 0, 0);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "CREATE GROUP"), PAGE_SETTINGS_LINE_1, 0, "CREATE GROUP", 0, 0);

//        currentItem = 0;

#ifndef APP_BAES
        enterHiddenPage = 0;

        timeStamp = 0xffffffff; // Initial timeStamp to infinity
#endif  // #ifndef APP_BAES

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
#ifndef APP_BAES
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "start", (currentItem == 0)?3:0, 0);
       // LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, "stop", (currentItem == 1)?3:0, 0);
       // LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

      
    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 3)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_4, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 3)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_4, (currentItem == 3)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#else
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, strings[STRINGS_INDEX_VERSION ], (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, strings[STRINGS_INDEX_BATTERY ], (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 2)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#endif  // #ifndef APP_BAES

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == PAGE_SETTINGS_MAXIMUMITEMS-1)
            {
                currentItem = 0;
               // _draw_funckey_area_frame();
            }
            else
            {
                currentItem++;

            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = PAGE_SETTINGS_MAXIMUMITEMS-1;

            }
            else
            {
                currentItem--;

            }

            pageUpdate = 1;
        }

        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }
        
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)//read
        {
          if(currentItem==0)
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_ERASE_GROUP_PNL, CMD_CONTROL_SENSOR_CREATE_GROUP);
          if(currentItem==1)
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_ERASE_GROUP_PNL, CMD_CANCEL_LEARNING);
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F2)
        {  
          if(currentItem==0)
          {
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_ERASE_GROUP_PNL, CMD_QUERY_DEVICE_SHORT_ADDRESS);
            
            _page_read_detector_info();
          }
        }
        
    }

    _common_battery_condition();


    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }

}

void DALIDetectorFinishCreate(void)
{
  //#define PAGE_SETTINGS_TITLE_X_POS    40
#ifndef NO_APPSWITCH
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   4
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#else
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#endif  // #ifndef NO_APPSWITCH
#define PAGE_SETTINGS_LINE_HEIGHT    16
#define PAGE_SETTINGS_LINE_SPACE     6
#define PAGE_SETTINGS_LINE_1         16
#define PAGE_SETTINGS_LINE_2         (PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_3         (PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_4         (PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)

    static unsigned char currentItem = 1;

#ifndef APP_BAES
    static unsigned char enterHiddenPage;
    static unsigned int timeStamp;
#endif  // #ifndef APP_BAES

    unsigned char pSec[3];
    
    
    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);
        _param_pageDali_funckey_initializer(FUNCKEY_NONE);

        // _param_page_funckey_initializer(FUNCKEY_READ_WRITE);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "Master"), 0, 0, "Master", 0, 0);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "Create Group"), PAGE_SETTINGS_LINE_1, 0, "Create Group", 0, 0);        
        
        LCDD_PutString(10, PAGE_SETTINGS_LINE_2, 0, "GROUP", 0, 0);

//        currentItem = 0;

#ifndef APP_BAES
        enterHiddenPage = 0;

        timeStamp = 0xffffffff; // Initial timeStamp to infinity
#endif  // #ifndef APP_BAES

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
#ifndef APP_BAES
        //LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
      //  LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "start", (currentItem == 0)?3:0, 0);
       // LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, "add slave", (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        
    //    LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        //LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, xx_createzone+"of"+yy_createzone+"control gear", (currentItem == 1)?3:0, 0);
      
//         LCDD_PutString(20-5, PAGE_SETTINGS_LINE_4, 0, "    of        Control Gear", 0, 0);
//         dectoascii(&pMin[0],xx_createzone, 0);
//         LCDD_PutString(5, PAGE_SETTINGS_LINE_4, 0, &pMin[0],PAGE_HIGHLIGHT_COLOR, 0);
        
         dectoascii(&pSec[0],zone_value, 0);
         LCDD_PutString(100, PAGE_SETTINGS_LINE_2, 0, &pSec[0],PAGE_HIGHLIGHT_COLOR, 0);       
        
    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 3)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_4, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 3)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_4, (currentItem == 3)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#else
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, strings[STRINGS_INDEX_VERSION ], (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, strings[STRINGS_INDEX_BATTERY ], (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 2)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#endif  // #ifndef APP_BAES

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }
         else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            pageCurrent =PAGE_INDEX_ADDSLAVE;

            pageSwap = 1;
        }
    
    }

    _common_battery_condition();

#ifndef APP_BAES
    if (TIME_GetTime() >= timeStamp)
    {
        if (enterHiddenPage == 3)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;
            pageSwap = 1;
        }
        else
        {
            enterHiddenPage = 0;
            timeStamp = 0xffffffff;     // Reset timeStamp to infinity
        }
    }
#endif  // #ifndef APP_BAES

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }

}

 DATAGroup DataGroup;
void DALIDetectorAddSlave(void)
{
   //#define PAGE_SETTINGS_TITLE_X_POS    40
#ifndef NO_APPSWITCH
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   4
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#else
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#endif  // #ifndef NO_APPSWITCH
#define PAGE_SETTINGS_LINE_HEIGHT    16
#define PAGE_SETTINGS_LINE_SPACE     6
#define PAGE_SETTINGS_LINE_1         16
#define PAGE_SETTINGS_LINE_2         (PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_3         (PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_4         (PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)

    static unsigned char currentItem = 0;

#ifndef APP_BAES
    static unsigned char enterHiddenPage;
    static unsigned int timeStamp;
#endif  // #ifndef APP_BAES

    unsigned char pSec[3];
 //  static struct DATAGroup test[32];
    DataGroup.DATA2=0;
    DataGroup.DATA3=0;
    DataGroup.DATA4=0;
    DataGroup.DATA5=0;
    DataGroup.flag=0;
    if (pageInitialize)
    {
        pageInitialize = 0;
        _param_page_funckey_initializer(FUNCKEY_READ_WRITE);
        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        _param_pageDali_funckey_initializer(FUNCKEY_WRITE_ONLY);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "Add slave"), 0, 0, "Add slave", 0, 0);
       // LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "Create Group"), PAGE_SETTINGS_LINE_1, 0, "Create Group", 0, 0);        
        
        LCDD_PutString(10, PAGE_SETTINGS_LINE_2, 0, "GROUP", 0, 0);

//        currentItem = 0;

#ifndef APP_BAES
        enterHiddenPage = 0;

        timeStamp = 0xffffffff; // Initial timeStamp to infinity
#endif  // #ifndef APP_BAES

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
#ifndef APP_BAES
        //LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
      //  LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "start", (currentItem == 0)?3:0, 0);
       // LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, "Include", (currentItem == 0)?3:0, 0);
  
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_4, 0, "Exclude", (currentItem == 1)?3:0, 0);

         dectoascii(&pSec[0],zone_value, 0);
         LCDD_PutString(100, PAGE_SETTINGS_LINE_2, 0, &pSec[0],PAGE_HIGHLIGHT_COLOR, 0);       
        
    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 3)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_4, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 3)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_4, (currentItem == 3)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#else
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, strings[STRINGS_INDEX_VERSION ], (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, strings[STRINGS_INDEX_BATTERY ], (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 2)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#endif  // #ifndef APP_BAES

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }
        
        
        if(BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
         // zone_value=0x10;
          if(zone_value<=31)
          DataGroup=ConverGroup2Data(zone_value,DataGroup);
          
          if(currentItem==0)
          {//include group           
            //SET_DEVICE_GROUP_NUMBER+ MASK 32 bits  
            IR_SetGroupNum(NITOO_LM058A_NODE,0xF0,0x46,DataGroup.DATA2, DataGroup.DATA3, DataGroup.DATA4,DataGroup.DATA5);
            if(DataGroup.flag==2)
              _page_read_GroupNum(DataGroup.DATA2,2,0);
            if(DataGroup.flag==3)
              _page_read_GroupNum(DataGroup.DATA3,DataGroup.flag,0);
            if(DataGroup.flag==4)
              _page_read_GroupNum(DataGroup.DATA4,DataGroup.flag,0);
            if(DataGroup.flag==5)
              _page_read_GroupNum(DataGroup.DATA5,DataGroup.flag,0);
          }
          
          if(currentItem==1)
          {//exclude group
            //SET_DEVICE_GROUP_NUMBER+ MASK 32 bits  
            IR_SetGroupNum(NITOO_LM058A_NODE,0xF0,0x4D,DataGroup.DATA2, DataGroup.DATA3, DataGroup.DATA4,DataGroup.DATA5);
            if(DataGroup.flag==2)
              _page_read_GroupNum(DataGroup.DATA2,DataGroup.flag,1);
            if(DataGroup.flag==3)
              _page_read_GroupNum(DataGroup.DATA3,DataGroup.flag,1);
            if(DataGroup.flag==4)
              _page_read_GroupNum(DataGroup.DATA4,DataGroup.flag,1);
            if(DataGroup.flag==5)
              _page_read_GroupNum(DataGroup.DATA5,DataGroup.flag,1);
          }
        
        }
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == PAGE_SETTINGS_MAXIMUMITEMS-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = PAGE_SETTINGS_MAXIMUMITEMS-1;
            }
            else
            {
                currentItem--;
            }

            pageUpdate = 1;
        }
    
    }

    _common_battery_condition();

#ifndef APP_BAES
    if (TIME_GetTime() >= timeStamp)
    {
        if (enterHiddenPage == 3)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;
            pageSwap = 1;
        }
        else
        {
            enterHiddenPage = 0;
            timeStamp = 0xffffffff;     // Reset timeStamp to infinity
        }
    }
#endif  // #ifndef APP_BAES

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }
}

DATAGroup ConverGroup2Data(unsigned char groupNum,DATAGroup A)
{
  float data=0;
  //struct DATAGroup A;
  
  if((groupNum>=0)&&(groupNum<=7))
  {
    A.flag=2;
    data=pow(2,groupNum);   
    A.DATA2=(unsigned char)data;
//    if(groupNum==0)
//      A.DATA2=0;
    
  } 
  if((groupNum>=8)&&(groupNum<=15))
  {
    A.flag=3;
    groupNum-=8;
    data=pow(2,groupNum);   
    A.DATA3=(unsigned char)data;
  }
  if((groupNum>=16)&&(groupNum<=23))
  {
    A.flag=4;
    groupNum-=16;
    data=pow(2,groupNum);   
    A.DATA4=(unsigned char)data;
  }
  if((groupNum>=24)&&(groupNum<=31))
  {
    A.flag=5;
    groupNum-=24;
    data=pow(2,groupNum);   
    A.DATA5=(unsigned char)data;
  }
  return A;
}
void DaliChangeTypeHandler(void)
{
  
  //#define PAGE_SETTINGS_TITLE_X_POS    40
#ifndef NO_APPSWITCH
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   4
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   3
    #endif  // #ifndef APP_BAES
#else
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   3
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#endif  // #ifndef NO_APPSWITCH
#define PAGE_SETTINGS_LINE_HEIGHT    16
#define PAGE_SETTINGS_LINE_SPACE     6
#define PAGE_SETTINGS_LINE_1         16
#define PAGE_SETTINGS_LINE_2         (PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_3         (PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_4         (PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)

    static unsigned char currentItem = 0;
    //             unsigned char const *string ="sbsb";
#ifndef APP_BAES
    static unsigned char enterHiddenPage;
    static unsigned int timeStamp;
#endif  // #ifndef APP_BAES

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        _param_pageDali_funckey_initializer(FUNCKEY_READ_WRITE);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "Change type"), 0, 0, "Change type", 0, 0);

//        currentItem = 0;

#ifndef APP_BAES
        enterHiddenPage = 0;

        timeStamp = 0xffffffff; // Initial timeStamp to infinity
#endif  // #ifndef APP_BAES

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
#ifndef APP_BAES
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, "Master", (currentItem == 0)?3:0, 0);
      //  LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "Slave", (currentItem == 1)?3:0, 0);
     //   LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, "Nop", (currentItem == 2)?3:0, 0);
    //    LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 3)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_4, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 3)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_4, (currentItem == 3)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#else
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, strings[STRINGS_INDEX_VERSION ], (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, strings[STRINGS_INDEX_BATTERY ], (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 2)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#endif  // #ifndef APP_BAES

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
      if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
      {
        if (currentItem == PAGE_SETTINGS_MAXIMUMITEMS-1)
        {
          currentItem = 0;
        }
        else
        {
          currentItem++;
        }
        
        pageUpdate = 1;
      }
      else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
      {
        if (currentItem == 0)
        {
          currentItem = PAGE_SETTINGS_MAXIMUMITEMS-1;
        }
        else
        {
          currentItem--;
        }
        
        pageUpdate = 1;
      }
      else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
      {
        pageCurrent = PAGE_INDEX_MAIN;
        
        pageSwap = 1;
      }
      
      else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
      {
        if(currentItem==0)
        {
          //slave to master
          if(PARAMS_IsDALI_SLAVE_OP_PIR_US(&sensorParams) || PARAMS_IsDALI_SLAVE_OP_PIR(&sensorParams))
          {
            //      unsigned char yOffset;
            //      static unsigned char result;             
            WaringBlock("WARNING","RESET PRODUCT"); 
          }
          TIME_Wait(2000);
          pageSwap =  1;
          IR_DirectlySetSensorParameter(&parameterChangeMaster);
          
        }
        if(currentItem==1)
        {
          //master to slave
          if (PARAMS_IsDALI_MASTER_DL_PIR_US(&sensorParams) || PARAMS_IsDALI_MASTER_DL_OP_PIR_US(&sensorParams) || PARAMS_IsDALI_MASTER_OP_PIR_US(&sensorParams) || \
            PARAMS_IsDALI_MASTER_DL_OP_PIR_US(&sensorParams) || \
              PARAMS_IsDALI_MASTER_OP_PIR(&sensorParams)||PARAMS_IsDALI_MASTER_PIR_DL(&sensorParams))
          {
            //      unsigned char yOffset;
            //      static unsigned char result;             
            WaringBlock("WARNING","RESET PRODUCT"); 
          }
          TIME_Wait(2000);
          pageSwap =  1;
          IR_DirectlySetSensorParameter(&parameterChangeSlave);
          
        }
        if(currentItem==2)
        {        
          IR_DirectlySetSensorParameter(&parameterChangeNop);
        }
        
      }

    }
    
    if (pageSwap)
    {
      pageSwap = 0;
      pageInitialize = 1;
    }
   
}


void WaringBlock(unsigned char const *string_1, unsigned char const *string_2)
{

  LCDD_ClearRect(PAGE_READPARAMS_RECT_X_START, PAGE_READPARAMS_RECT_Y_START, PAGE_READPARAMS_RECT_X_END, 70, 3);
  LCDD_PutString(PAGE_READPARAMS_BODY_X_START, PAGE_READPARAMS_BODY_Y_START, PAGE_READPARAMS_BODY_X_LENGTH, string_1, 3, 0);
  LCDD_PutString(PAGE_READPARAMS_BODY_X_START, PAGE_READPARAMS_BODY_Y_START+20, PAGE_READPARAMS_BODY_X_LENGTH, string_2, 3, 0);
  LCDD_Refresh();     // Have to do this here.
  
}

void DaliCreateZoneHandler(void)
{
//#define PAGE_SETTINGS_TITLE_X_POS    40
#ifndef NO_APPSWITCH
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   4
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#else
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#endif  // #ifndef NO_APPSWITCH
#define PAGE_SETTINGS_LINE_HEIGHT    16
#define PAGE_SETTINGS_LINE_SPACE     6
#define PAGE_SETTINGS_LINE_1         16
#define PAGE_SETTINGS_LINE_2         (PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_3         (PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_4         (PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)

    static unsigned char currentItem = 0;

#ifndef APP_BAES
    static unsigned char enterHiddenPage;
    static unsigned int timeStamp;
#endif  // #ifndef APP_BAES

    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        _param_pageDali_funckey_initializer(FUNCKEY_READ_WRITE);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "CONTROL GEAR"), 0, 0, "CONTROL GEAR", 0, 0);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "CREATE ZONE"), PAGE_SETTINGS_LINE_1, 0, "CREATE ZONE", 0, 0);

//        currentItem = 0;

#ifndef APP_BAES
        enterHiddenPage = 0;

        timeStamp = 0xffffffff; // Initial timeStamp to infinity
#endif  // #ifndef APP_BAES

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
#ifndef APP_BAES
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "start", (currentItem == 0)?3:0, 0);
       // LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, "stop", (currentItem == 1)?3:0, 0);
       // LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

      
    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 3)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_4, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 3)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_4, (currentItem == 3)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#else
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, strings[STRINGS_INDEX_VERSION ], (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, strings[STRINGS_INDEX_BATTERY ], (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 2)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#endif  // #ifndef APP_BAES

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == PAGE_SETTINGS_MAXIMUMITEMS-1)
            {
                currentItem = 0;
               // _draw_funckey_area_frame();
            }
            else
            {
                currentItem++;
//                if(currentItem == PAGE_SETTINGS_MAXIMUMITEMS-1)
//                {
//                  _param_page_funckey_initializer(FUNCKEY_READ_WRITE);
//                
//                }
//                else
//                {
//                  _draw_funckey_area_frame();
//                }
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = PAGE_SETTINGS_MAXIMUMITEMS-1;

            }
            else
            {
                currentItem--;

            }

            pageUpdate = 1;
        }

        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }
        
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)//read
        {
          if(currentItem==0)
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_ERASE_GROUP_PNL, CMD_CONTROL_GEAR_CREATE_GROUP);
          if(currentItem==1)
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_ERASE_GROUP_PNL, CMD_CANCEL_LEARNING);
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F2)
        {  
          if(currentItem==0)
          {
            //int temp;
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_ERASE_GROUP_PNL, CMD_QUERY_NEXT_CONTROL_GEAR);
//            currentItem=xx_createzone;
//            currentItem=yy_createzone;
//            currentItem=zone_value;
            
            if(_page_read_zone_info()==10)
               pageCurrent = PAGE_CREATE_GROUP;
          }
        }
        
    }

    _common_battery_condition();

#ifndef APP_BAES
    if (TIME_GetTime() >= timeStamp)
    {
        if (enterHiddenPage == 3)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;
            pageSwap = 1;
        }
        else
        {
            enterHiddenPage = 0;
            timeStamp = 0xffffffff;     // Reset timeStamp to infinity
        }
    }
#endif  // #ifndef APP_BAES

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }

}


void DaliCreateGroupHandler(void)
{
//#define PAGE_SETTINGS_TITLE_X_POS    40
#ifndef NO_APPSWITCH
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   4
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#else
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#endif  // #ifndef NO_APPSWITCH
#define PAGE_SETTINGS_LINE_HEIGHT    16
#define PAGE_SETTINGS_LINE_SPACE     6
#define PAGE_SETTINGS_LINE_1         16
#define PAGE_SETTINGS_LINE_2         (PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_3         (PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_4         (PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)

    static unsigned char currentItem = 1;

#ifndef APP_BAES
    static unsigned char enterHiddenPage;
    static unsigned int timeStamp;
#endif  // #ifndef APP_BAES

    unsigned char pMin[3];
    unsigned char pSec[3];
    
    
    if (pageInitialize)
    {
        pageInitialize = 0;

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        _param_pageDali_funckey_initializer(FUNCKEY_READ_ONLY);

        // _param_page_funckey_initializer(FUNCKEY_READ_WRITE);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "CONTROL GEAR"), 0, 0, "CONTROL GEAR", 0, 0);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "End of addressing"), PAGE_SETTINGS_LINE_1, 0, "End of addressing", 0, 0);

//        currentItem = 0;

#ifndef APP_BAES
        enterHiddenPage = 0;

        timeStamp = 0xffffffff; // Initial timeStamp to infinity
#endif  // #ifndef APP_BAES

        pageUpdate = 1;
    }

    if (pageUpdate)
    {
#ifndef APP_BAES
        //LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
      //  LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "start", (currentItem == 0)?3:0, 0);
       // LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, "Choose zone", (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
        
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        //LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, xx_createzone+"of"+yy_createzone+"control gear", (currentItem == 1)?3:0, 0);
      
         LCDD_PutString(20-5, PAGE_SETTINGS_LINE_4, 0, "    of        Control Gear", 0, 0);
         dectoascii(&pMin[0],xx_createzone, 0);
         LCDD_PutString(5, PAGE_SETTINGS_LINE_4, 0, &pMin[0],PAGE_HIGHLIGHT_COLOR, 0);
        
         dectoascii(&pSec[0],yy_createzone, 0);
         LCDD_PutString(50-5-5, PAGE_SETTINGS_LINE_4, 0, &pSec[0],PAGE_HIGHLIGHT_COLOR, 0);       
        
    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 3)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_4, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 3)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_4, (currentItem == 3)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#else
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, strings[STRINGS_INDEX_VERSION ], (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, strings[STRINGS_INDEX_BATTERY ], (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 2)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#endif  // #ifndef APP_BAES

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }
         else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
          
          if(PARAMS_IsDALI_MASTER_DL_PIR_US(&sensorParams)||PARAMS_IsDALI_MASTER_DL_OP_PIR_US(&sensorParams)||PARAMS_IsDALI_MASTER_OP_PIR_US(&sensorParams))
          {
            pageCurrent =PAGE_CHOOSE_ZONE;
          }
          
          
          if(PARAMS_IsDALI_MASTER_DL_OP_PIR(&sensorParams)||PARAMS_IsDALI_MASTER_OP_PIR(&sensorParams)||PARAMS_IsDALI_MASTER_PIR_DL(&sensorParams))
          {
            pageCurrent =PAGE_CHOOSE_ZONE_CORRID;
          
          }
        
          
            pageSwap = 1;
        }
    
    }

    _common_battery_condition();

#ifndef APP_BAES
    if (TIME_GetTime() >= timeStamp)
    {
        if (enterHiddenPage == 3)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;
            pageSwap = 1;
        }
        else
        {
            enterHiddenPage = 0;
            timeStamp = 0xffffffff;     // Reset timeStamp to infinity
        }
    }
#endif  // #ifndef APP_BAES

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
    }

}

void DaliChooseZoneCorridHandler(void)
{
#define PAGE_SENSITIVITY_MAXIMUMITEMS       3
#define PAGE_SENSITIVITY_NAME_X_POS         4
#define PAGE_SENSITIVITY_VALUE_X_POS        40
#define PAGE_SENSITIVITY_BRACKET_X_POS      114
#define PAGE_SENSITIVITY_BODY_Y_POS_1       18
#define PAGE_SENSITIVITY_BODY_Y_POS_DELTA   22
#define PAGE_SENSITIVITY_BODY_Y_POS_2       (PAGE_SENSITIVITY_BODY_Y_POS_1 + PAGE_SENSITIVITY_BODY_Y_POS_DELTA)
#define PAGE_SENSITIVITY_BODY_Y_POS_3       (PAGE_SENSITIVITY_BODY_Y_POS_2 + PAGE_SENSITIVITY_BODY_Y_POS_DELTA)

  static const PAGE_ITEM_RECT rect[1] = {{PAGE_SENSITIVITY_BODY_Y_POS_1-1, PAGE_SENSITIVITY_BODY_Y_POS_1+16}};//{PAGE_SENSITIVITY_BODY_Y_POS_2-1, PAGE_SENSITIVITY_BODY_Y_POS_2+16}, {PAGE_SENSITIVITY_BODY_Y_POS_3-1, PAGE_SENSITIVITY_BODY_Y_POS_3+16}};
   // static unsigned char previousItem;
    static unsigned char currentItem;
    static unsigned char totalItems=3;
    static PAGE_PARAMETER *pParameter;
    unsigned char yOffset, i;
    PAGE_PARAMETER *pTempParameter;
    yOffset = PAGE_SENSITIVITY_BODY_Y_POS_1;
    const unsigned char *ErrorStrings[]={"    ","error"}; 
    const unsigned char *strings[]={"Include","Exclude"}; 
    static unsigned char currentItem_x;
   const unsigned char *PString;
       unsigned char pMin[3];
    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = 1;//PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_HF_SENS) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_PIR_SENS) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_US_SENS);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

      //  LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1,0, 0, "Choose Zone", 0, 0);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "Choose Zone Corrid"), 0, 0, "Choose Zone Corrid", 0, 0); 
 

        if (pageAvailability)
        {
        _param_pageDali_funckey_initializer(FUNCKEY_READ_WRITE);
          currentItem = 0;
          pParameter = (PAGE_PARAMETER *)malloc(PAGE_SENSITIVITY_MAXIMUMITEMS*sizeof(PAGE_PARAMETER));
          pTempParameter = pParameter;
          yOffset = PAGE_SENSITIVITY_BODY_Y_POS_1;
          pageUpdate = 1;
        }
    }


    if (pageUpdate)
    {
         switch(zone_value)
         {
         case 0x00:
           PString=ErrorStrings[0];
           break;
         case 0x01:
           PString=strings[0];
           currentItem_x=0;
           break;

         case 0x05:
           PString=strings[1];
           currentItem_x=1;
           break;               
         
         default:
           PString=NULL;
           break;  
         }
      
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "Next", (currentItem == 1)?3:0, 0);
       // LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, "Previous", (currentItem == 2)?3:0, 0);               
        
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, 0);
        LCDD_PutString(20-5, PAGE_SETTINGS_LINE_4, 0, "    of        Control Gear", 0, 0);
        
        dectoascii(&pMin[0],xx_createzone, 0);
        LCDD_PutString(5, PAGE_SETTINGS_LINE_4, 0, &pMin[0],PAGE_HIGHLIGHT_COLOR, 0);
        
        dectoascii(&pMin[0],yy_createzone, 0);
        LCDD_PutString(50-5-5, PAGE_SETTINGS_LINE_4, 0, &pMin[0],PAGE_HIGHLIGHT_COLOR, 0);      
        
        if (currentItem == 0)
        {

          LCDD_ClearRect(PAGE_SENSITIVITY_NAME_X_POS, rect[currentItem].yStart, LCD_X_LENGTH-1, rect[currentItem].yEnd, 0);
          LCDD_PutString(PAGE_SENSITIVITY_NAME_X_POS, yOffset, 0, "ZONE", 0, 0);
          LCDD_InvertRect(PAGE_SENSITIVITY_NAME_X_POS, rect[currentItem].yStart, LCD_X_LENGTH-1, rect[currentItem].yEnd, PAGE_HIGHLIGHT_COLOR); 
           
        }
       else 
        {
           LCDD_ClearRect(PAGE_SENSITIVITY_NAME_X_POS, rect[0].yStart, LCD_X_LENGTH-1, rect[0].yEnd, 0);
           LCDD_PutString(PAGE_SENSITIVITY_NAME_X_POS, 18, 0, "ZONE", 0, 0);
        }
        

 //PString=strings[0];
         for (i=0; i<1; i++)
        {
            LCDD_ClearRect(PAGE_SENSITIVITY_VALUE_X_POS-4, 18, LCD_X_LENGTH-2, yOffset+15, 0);
            //LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS, 18, 0, strings[currentItem_x], 0, 0);      // 2.4.0
            LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS, 18, 0, PString, 0, 0); 
            LCDD_PutString(PAGE_SENSITIVITY_BRACKET_X_POS, 18, 0, "<>", 0, 0);           
        }
       pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            if (currentItem == 0)
            {
                currentItem = totalItems-1;
            }
            else
            {
                currentItem--;
            }

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            if (currentItem == totalItems-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
          if(currentItem==0)
          {
            if (currentItem_x == 0)
            {
              currentItem_x = 1;
            }
            else
            {
              currentItem_x--;
            }
            
            //pageUpdate = 1;
            LCDD_ClearRect(PAGE_SENSITIVITY_VALUE_X_POS-4, 18, LCD_X_LENGTH-2, yOffset+15, 0);
            LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS, 18, 0, strings[currentItem_x], 0, 0); 
            LCDD_PutString(PAGE_SENSITIVITY_BRACKET_X_POS, 18, 0, "<>", 0, 0);
          }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT)
        {
          if(currentItem==0)
          {
            if (currentItem_x == 1)
            {
              currentItem_x = 0;
            }
            else
            {
              currentItem_x++;
            }
            
            // pageUpdate = 1;
            //LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS, 18, 0, strings[currentItem_x], 0, 0);      // 2.4.0
            LCDD_ClearRect(PAGE_SENSITIVITY_VALUE_X_POS-4, 18, LCD_X_LENGTH-2, yOffset+15, 0);
            LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS, 18, 0, strings[currentItem_x], 0, 0); 
            LCDD_PutString(PAGE_SENSITIVITY_BRACKET_X_POS, 18, 0, "<>", 0, 0);  
          }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
          if(currentItem==1)            
          {
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_ERASE_GROUP_PNL, CMD_QUERY_NEXT_CONTROL_GEAR);
             _page_read_zone_info();  
          }
          if(currentItem==2)
          {
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_ERASE_GROUP_PNL, CMD_QUERY_PREVIOUS_CONTROL_GEAR);
             _page_read_zone_info();  
          }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
           if(currentItem==0)            
           {
             if(currentItem_x==0x01)
             {//exclude
               IR_ReadNBofControlGearParameter(NITOO_LM058A_NODE,0xB0,0x3C,0x05,0x00);
             }
             if(currentItem_x==0x00)
             {//include
               IR_ReadNBofControlGearParameter(NITOO_LM058A_NODE,0xB0,0x3B,0x01,0x00);            
             }

             TIME_Wait(100);
             IR_PnLGroup(NITOO_LM058A_NODE, CODE_ERASE_GROUP_PNL, CMD_QUERY_NEXT_CONTROL_GEAR);
             _page_read_zone_info();    
             
           }

        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {//cancel
           IR_PnLGroup(NITOO_LM058A_NODE, CODE_ERASE_GROUP_PNL, CMD_CANCEL_LEARNING);
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }

}
void DaliChooseZoneHandler(void)
{
#define PAGE_SENSITIVITY_MAXIMUMITEMS       3
#define PAGE_SENSITIVITY_NAME_X_POS         4
#define PAGE_SENSITIVITY_VALUE_X_POS        40
#define PAGE_SENSITIVITY_BRACKET_X_POS      114
#define PAGE_SENSITIVITY_BODY_Y_POS_1       18
#define PAGE_SENSITIVITY_BODY_Y_POS_DELTA   22
#define PAGE_SENSITIVITY_BODY_Y_POS_2       (PAGE_SENSITIVITY_BODY_Y_POS_1 + PAGE_SENSITIVITY_BODY_Y_POS_DELTA)
#define PAGE_SENSITIVITY_BODY_Y_POS_3       (PAGE_SENSITIVITY_BODY_Y_POS_2 + PAGE_SENSITIVITY_BODY_Y_POS_DELTA)

  static const PAGE_ITEM_RECT rect[1] = {{PAGE_SENSITIVITY_BODY_Y_POS_1-1, PAGE_SENSITIVITY_BODY_Y_POS_1+16}};//{PAGE_SENSITIVITY_BODY_Y_POS_2-1, PAGE_SENSITIVITY_BODY_Y_POS_2+16}, {PAGE_SENSITIVITY_BODY_Y_POS_3-1, PAGE_SENSITIVITY_BODY_Y_POS_3+16}};
   // static unsigned char previousItem;
    static unsigned char currentItem;
    static unsigned char totalItems=3;
    static PAGE_PARAMETER *pParameter;
    unsigned char yOffset, i;
    PAGE_PARAMETER *pTempParameter;
    yOffset = PAGE_SENSITIVITY_BODY_Y_POS_1;
    const unsigned char *ErrorStrings[]={"    ","W+B","C+B","W+C+B","W+C"}; 
    const unsigned char *strings[]={"Windows","Corridor","Board","Exclude"}; 
    static unsigned char currentItem_x;
   const unsigned char *PString;
       unsigned char pMin[3];
    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = 1;//PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_HF_SENS) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_PIR_SENS) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_US_SENS);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

      //  LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1,0, 0, "Choose Zone", 0, 0);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "Choose Zone"), 0, 0, "Choose Zone", 0, 0); 
 

        if (pageAvailability)
        {
        _param_pageDali_funckey_initializer(FUNCKEY_READ_WRITE);
          currentItem = 0;
          pParameter = (PAGE_PARAMETER *)malloc(PAGE_SENSITIVITY_MAXIMUMITEMS*sizeof(PAGE_PARAMETER));
          pTempParameter = pParameter;
          yOffset = PAGE_SENSITIVITY_BODY_Y_POS_1;
          pageUpdate = 1;
        }
    }


    if (pageUpdate)
    {
         switch(zone_value)
         {
         case 0x00:
           PString=ErrorStrings[0];
           break;
         case 0x01:
           PString=strings[0];
           currentItem_x=0;
           break;
         case 0x02:
           PString=strings[1];
           currentItem_x=1;
           break;
         case 0x03:
           PString=ErrorStrings[4];
           break;
         case 0x04:
           PString=strings[2];
           currentItem_x=2;
           break;
         case 0x05:
           PString=ErrorStrings[1];
           break;               
         case 0x06:
           PString=ErrorStrings[2];
           break;      
         case 0x07:
           PString=ErrorStrings[3];
           break;             
         default:
           PString=NULL;
           break;  
         }
      
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "Next", (currentItem == 1)?3:0, 0);
       // LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, "Previous", (currentItem == 2)?3:0, 0);               
        
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, 0);
        LCDD_PutString(20-5, PAGE_SETTINGS_LINE_4, 0, "    of        Control Gear", 0, 0);
        
        dectoascii(&pMin[0],xx_createzone, 0);
        LCDD_PutString(5, PAGE_SETTINGS_LINE_4, 0, &pMin[0],PAGE_HIGHLIGHT_COLOR, 0);
        
        dectoascii(&pMin[0],yy_createzone, 0);
        LCDD_PutString(50-5-5, PAGE_SETTINGS_LINE_4, 0, &pMin[0],PAGE_HIGHLIGHT_COLOR, 0);      
        
        if (currentItem == 0)
        {

          LCDD_ClearRect(PAGE_SENSITIVITY_NAME_X_POS, rect[currentItem].yStart, LCD_X_LENGTH-1, rect[currentItem].yEnd, 0);
          LCDD_PutString(PAGE_SENSITIVITY_NAME_X_POS, yOffset, 0, "ZONE", 0, 0);
          LCDD_InvertRect(PAGE_SENSITIVITY_NAME_X_POS, rect[currentItem].yStart, LCD_X_LENGTH-1, rect[currentItem].yEnd, PAGE_HIGHLIGHT_COLOR); 
           
        }
       else 
        {
           LCDD_ClearRect(PAGE_SENSITIVITY_NAME_X_POS, rect[0].yStart, LCD_X_LENGTH-1, rect[0].yEnd, 0);
           LCDD_PutString(PAGE_SENSITIVITY_NAME_X_POS, 18, 0, "ZONE", 0, 0);
        }
        

 //PString=strings[0];
         for (i=0; i<1; i++)
        {
            LCDD_ClearRect(PAGE_SENSITIVITY_VALUE_X_POS-4, 18, LCD_X_LENGTH-2, yOffset+15, 0);
            //LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS, 18, 0, strings[currentItem_x], 0, 0);      // 2.4.0
            LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS, 18, 0, PString, 0, 0); 
            LCDD_PutString(PAGE_SENSITIVITY_BRACKET_X_POS, 18, 0, "<>", 0, 0);           
        }
       pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            if (currentItem == 0)
            {
                currentItem = totalItems-1;
            }
            else
            {
                currentItem--;
            }

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            if (currentItem == totalItems-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
          if(currentItem==0)
          {
            if (currentItem_x == 0)
            {
              currentItem_x = 3;
            }
            else
            {
              currentItem_x--;
            }
            
            //pageUpdate = 1;
            LCDD_ClearRect(PAGE_SENSITIVITY_VALUE_X_POS-4, 18, LCD_X_LENGTH-2, yOffset+15, 0);
            LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS, 18, 0, strings[currentItem_x], 0, 0); 
            LCDD_PutString(PAGE_SENSITIVITY_BRACKET_X_POS, 18, 0, "<>", 0, 0);
          }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT)
        {
          if(currentItem==0)
          {
            if (currentItem_x == 3)
            {
              currentItem_x = 0;
            }
            else
            {
              currentItem_x++;
            }
            
            // pageUpdate = 1;
            //LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS, 18, 0, strings[currentItem_x], 0, 0);      // 2.4.0
            LCDD_ClearRect(PAGE_SENSITIVITY_VALUE_X_POS-4, 18, LCD_X_LENGTH-2, yOffset+15, 0);
            LCDD_PutString(PAGE_SENSITIVITY_VALUE_X_POS, 18, 0, strings[currentItem_x], 0, 0); 
            LCDD_PutString(PAGE_SENSITIVITY_BRACKET_X_POS, 18, 0, "<>", 0, 0);  
          }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
          if(currentItem==1)            
          {
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_ERASE_GROUP_PNL, CMD_QUERY_NEXT_CONTROL_GEAR);
             _page_read_zone_info();  
          }
          if(currentItem==2)
          {
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_ERASE_GROUP_PNL, CMD_QUERY_PREVIOUS_CONTROL_GEAR);
             _page_read_zone_info();  
          }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
           if(currentItem==0)            
           {
             if(currentItem_x==0x03)
             {//exclude
               IR_ReadNBofControlGearParameter(NITOO_LM058A_NODE,0xB0,0x3C,0x05,0x00);
             }
             else
             {//include
               switch(currentItem_x)
               {
               case 0x00:
                 IR_ReadNBofControlGearParameter(NITOO_LM058A_NODE,0xB0,0x3B,0x01,0x00);
                 break;
               case 0x01:
                 IR_ReadNBofControlGearParameter(NITOO_LM058A_NODE,0xB0,0x3B,0x02,0x00);            
                 break;
               case 0x02:
                 IR_ReadNBofControlGearParameter(NITOO_LM058A_NODE,0xB0,0x3B,0x04,0x00);              
                 break;
               }                     
               
             }

             TIME_Wait(100);
             IR_PnLGroup(NITOO_LM058A_NODE, CODE_ERASE_GROUP_PNL, CMD_QUERY_NEXT_CONTROL_GEAR);
             _page_read_zone_info();    
             
           }

        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
          
           IR_PnLGroup(NITOO_LM058A_NODE, CODE_ERASE_GROUP_PNL, CMD_CANCEL_LEARNING);
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }


}

void DaliViewAreaCorridHandler(void)
{
#ifndef NO_APPSWITCH
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   4
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   0
    #endif  // #ifndef APP_BAES
#else
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   3
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#endif  // #ifndef NO_APPSWITCH
#define PAGE_SETTINGS_LINE_HEIGHT    16
#define PAGE_SETTINGS_LINE_SPACE     6
#define PAGE_SETTINGS_LINE_1         16
#define PAGE_SETTINGS_LINE_2         (PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_3         (PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_4         (PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
    static unsigned char firstDisplay = 0;
    static unsigned char currentItem = 0;
    unsigned char pDaylightsetpoint[9]; 
#ifndef APP_BAES
    static unsigned char enterHiddenPage;
    static unsigned int timeStamp;
#endif  // #ifndef APP_BAES

    if (pageInitialize)
    {      
        pageInitialize = 0;
        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);
        _param_pageDali_funckey_initializer(FUNCKEY_READ_ONLY);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "VIEW ZONE"), 0, 0, "VIEW ZONE", 0, 0);
#ifndef APP_BAES
        enterHiddenPage = 0;

        timeStamp = 0xffffffff; // Initial timeStamp to infinity
#endif  // #ifndef APP_BAES

        pageUpdate = 0;
    }
if(firstDisplay==1)
{   if (pageUpdate)
    {
#ifndef APP_BAES


        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "Corridor", 3, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);


         
        if(view.corridor_value==0x65)
        {
          LCDD_PutString(100-30, PAGE_SETTINGS_LINE_2, 0,"ON", PAGE_HIGHLIGHT_COLOR, 0); 
        }
        else if(view.corridor_value==0x66)
        {
          LCDD_PutString(100-30, PAGE_SETTINGS_LINE_2, 0,"OFF", PAGE_HIGHLIGHT_COLOR, 0);         
        }
        else{
        
        dectoascii(&pDaylightsetpoint[0],view.corridor_value, 3);
        strcpy((char *)&pDaylightsetpoint[3], "%");
        LCDD_PutString(100-30, PAGE_SETTINGS_LINE_2, 0, &pDaylightsetpoint[0], PAGE_HIGHLIGHT_COLOR, 0);  
        }
            
    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 3)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_4, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 3)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_4, (currentItem == 3)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#else
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, strings[STRINGS_INDEX_VERSION ], (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, strings[STRINGS_INDEX_BATTERY ], (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 2)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#endif  // #ifndef APP_BAES

        pageUpdate = 0;
    }
}
    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
         if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            pageCurrent = PAGE_INDEX_CORRIDOR;

            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }
        
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F2)
        {
          if(!firstDisplay)
          {
            int i=0;
            int result_read;
            for(i=1;i<2;i++)
            {
              IR_PnLGroup(i,CODE_ANNOUNCE_ID,CMD_UNIT_DESCRIPTION_REQUEST);
              TIME_Wait(2);
              result_read=IR_Read_UNIT_DESCRIPTION_Info(i);
              if( result_read!=2)
                break;
              
            }
            if(result_read==2)
            {
              IR_ReadNBofControlGearParameter(NITOO_LM058A_NODE,CODE_OPEN_LEARNING,0x41,0x01,0x00);
              result_read= IR_Read_CONTROL_GEAR_Info();
              if( result_read==2)
              {
                firstDisplay=1;
                pageUpdate = 1;
              }
            }
          }
          else
          {
              int result_read;
              IR_PnLGroup(currentItem,CODE_ANNOUNCE_ID,CMD_UNIT_DESCRIPTION_REQUEST);
              result_read=IR_Read_UNIT_DESCRIPTION_Info(currentItem);
              if( result_read==2)
              {
//                if(currentItem==0)
//                  IR_ReadNBofControlGearParameter(NITOO_LM058A_NODE,CODE_OPEN_LEARNING,0x41,0x01,0x00);
//                if(currentItem==1)
                  IR_ReadNBofControlGearParameter(NITOO_LM058A_NODE,CODE_OPEN_LEARNING,0x41,0x02,0x00);
//                if(currentItem==2)               
//                  IR_ReadNBofControlGearParameter(NITOO_LM058A_NODE,CODE_OPEN_LEARNING,0x41,0x04,0x00);
                
                result_read= IR_Read_CONTROL_GEAR_Info();
                if( result_read==2)
                {
                  firstDisplay=1;
                  pageUpdate = 1;
                }
                
              }
          }
          
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_START_GROUP_PNL, CMD_CONTROL_GEAR_CREATE_GROUP);
        }
        
    }

    _common_battery_condition();

#ifndef APP_BAES
    if (TIME_GetTime() >= timeStamp)
    {
        if (enterHiddenPage == 3)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;
            pageSwap = 1;
        }
        else
        {
            enterHiddenPage = 0;
            timeStamp = 0xffffffff;     // Reset timeStamp to infinity
        }
    }
#endif  // #ifndef APP_BAES

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
        firstDisplay=0;
    }
}



void DaliViewAreaHandler(void)
{
//#define PAGE_SETTINGS_TITLE_X_POS    40
#ifndef NO_APPSWITCH
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   4
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   3
    #endif  // #ifndef APP_BAES
#else
    #ifndef APP_BAES
    #define PAGE_SETTINGS_MAXIMUMITEMS   3
    #else
    #define PAGE_SETTINGS_MAXIMUMITEMS   2
    #endif  // #ifndef APP_BAES
#endif  // #ifndef NO_APPSWITCH
#define PAGE_SETTINGS_LINE_HEIGHT    16
#define PAGE_SETTINGS_LINE_SPACE     6
#define PAGE_SETTINGS_LINE_1         16
#define PAGE_SETTINGS_LINE_2         (PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_3         (PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
#define PAGE_SETTINGS_LINE_4         (PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT+PAGE_SETTINGS_LINE_SPACE)
    static unsigned char firstDisplay = 0;
    static unsigned char currentItem = 0;
    static const unsigned char targetPage[PAGE_SETTINGS_MAXIMUMITEMS+1] = {
        PAGE_INDEX_WINDOWS,
        PAGE_INDEX_CORRIDOR,
        PAGE_INDEX_BOARD
    };
        unsigned char pDaylightsetpoint[9]; 
       // unsigned char pDaylightsetpoint1[9]; 
       // unsigned char pDaylightsetpoint2[9]; 
#ifndef APP_BAES
    static unsigned char enterHiddenPage;
    static unsigned int timeStamp;
#endif  // #ifndef APP_BAES

    if (pageInitialize)
    {      
        pageInitialize = 0;
        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);
        _param_pageDali_funckey_initializer(FUNCKEY_READ_ONLY);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "VIEW ZONE"), 0, 0, "VIEW ZONE", 0, 0);

//        currentItem = 0;

#ifndef APP_BAES
        enterHiddenPage = 0;

        timeStamp = 0xffffffff; // Initial timeStamp to infinity
#endif  // #ifndef APP_BAES

        pageUpdate = 0;
    }
if(firstDisplay==1)
{   if (pageUpdate)
    {
#ifndef APP_BAES
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, "Windows", (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "Corridor", (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, "Board", (currentItem == 2)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        if(view.windows_value==0x65)
        {
          LCDD_PutString(100-30, PAGE_SETTINGS_LINE_1, 0,"ON", PAGE_HIGHLIGHT_COLOR, 0); 
        }
        else if(view.windows_value==0x66)
        {
          LCDD_PutString(100-30, PAGE_SETTINGS_LINE_1, 0,"OFF", PAGE_HIGHLIGHT_COLOR, 0);         
        }
        else{
          dectoascii(&pDaylightsetpoint[0],view.windows_value, 3);
          strcpy((char *)&pDaylightsetpoint[3], "%");
          LCDD_PutString(100-30, PAGE_SETTINGS_LINE_1, 0, &pDaylightsetpoint[0], PAGE_HIGHLIGHT_COLOR, 0);  
        }
         
        if(view.corridor_value==0x65)
        {
          LCDD_PutString(100-30, PAGE_SETTINGS_LINE_2, 0,"ON", PAGE_HIGHLIGHT_COLOR, 0); 
        }
        else if(view.corridor_value==0x66)
        {
          LCDD_PutString(100-30, PAGE_SETTINGS_LINE_2, 0,"OFF", PAGE_HIGHLIGHT_COLOR, 0);         
        }
        else{
        
        dectoascii(&pDaylightsetpoint[0],view.corridor_value, 3);
        strcpy((char *)&pDaylightsetpoint[3], "%");
        LCDD_PutString(100-30, PAGE_SETTINGS_LINE_2, 0, &pDaylightsetpoint[0], PAGE_HIGHLIGHT_COLOR, 0);  
        }
        
         if(view.board_value==0x65)
        {
          LCDD_PutString(100-30, PAGE_SETTINGS_LINE_3, 0,"ON", PAGE_HIGHLIGHT_COLOR, 0); 
        }
        else if(view.board_value==0x66)
        {
          LCDD_PutString(100-30, PAGE_SETTINGS_LINE_3, 0,"OFF", PAGE_HIGHLIGHT_COLOR, 0);         
        }
        else{
          dectoascii(&pDaylightsetpoint[0],view.board_value, 3);
          strcpy((char *)&pDaylightsetpoint[3], "%");
          LCDD_PutString(100-30,  PAGE_SETTINGS_LINE_3, 0, &pDaylightsetpoint[0], PAGE_HIGHLIGHT_COLOR, 0);  
        }
         LCDD_PutString(20-5, PAGE_SETTINGS_LINE_4, 0, "    of Control Gear", 0, 0);
         dectoascii(&pDaylightsetpoint[0],view.XX, 3);
         LCDD_PutString(5, PAGE_SETTINGS_LINE_4, 0, &pDaylightsetpoint[0],PAGE_HIGHLIGHT_COLOR, 0);
        
        
    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 3)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_4, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 3)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_4, (currentItem == 3)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#else
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_1, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_1+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 0)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_1, 0, strings[STRINGS_INDEX_VERSION ], (currentItem == 0)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, strings[STRINGS_INDEX_BATTERY ], (currentItem == 1)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_2, (currentItem == 1)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

    #ifndef NO_APPSWITCH
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, strings[STRINGS_INDEX_APPSWITCH ], (currentItem == 2)?3:0, 0);
        LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_3, (currentItem == 2)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);
    #endif  // #ifndef NO_APPSWITCH
#endif  // #ifndef APP_BAES

        pageUpdate = 0;
    }
}
    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED))
    {
      if(firstDisplay==1)
      {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (currentItem == PAGE_SETTINGS_MAXIMUMITEMS-1)
            {
                currentItem = 0;
                //_draw_funckey_area_frame();
            }
            else
            {
                currentItem++;
            }

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (currentItem == 0)
            {
                currentItem = PAGE_SETTINGS_MAXIMUMITEMS-1;

            }
            else
            {
                currentItem--;
            }

            pageUpdate = 1;
        }
      }
         if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            pageCurrent = targetPage[currentItem];

            pageSwap = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }
        
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F2)
        {
          if(!firstDisplay)
          {
            int i=1;
            int result_read;
            for(i=1;i<4;i++)
            {
              IR_PnLGroup(i,CODE_ANNOUNCE_ID,CMD_UNIT_DESCRIPTION_REQUEST);
              result_read=IR_Read_UNIT_DESCRIPTION_Info(i);
              if( result_read!=2)
                break;
              
            }
            if(result_read==2)
            {
              IR_ReadNBofControlGearParameter(NITOO_LM058A_NODE,CODE_OPEN_LEARNING,0x41,0x01,0x00);
              result_read= IR_Read_CONTROL_GEAR_Info();
              if( result_read==2)
              {
                firstDisplay=1;
                pageUpdate = 1;
              }
            }
          }
          else
          {
              int result_read;
              IR_PnLGroup(currentItem+1,CODE_ANNOUNCE_ID,CMD_UNIT_DESCRIPTION_REQUEST);
              result_read=IR_Read_UNIT_DESCRIPTION_Info(currentItem);
              if( result_read==2)
              {
                if(currentItem==0)
                  IR_ReadNBofControlGearParameter(NITOO_LM058A_NODE,CODE_OPEN_LEARNING,0x41,0x01,0x00);
                if(currentItem==1)
                  IR_ReadNBofControlGearParameter(NITOO_LM058A_NODE,CODE_OPEN_LEARNING,0x41,0x02,0x00);
                if(currentItem==2)               
                  IR_ReadNBofControlGearParameter(NITOO_LM058A_NODE,CODE_OPEN_LEARNING,0x41,0x04,0x00);
                
                result_read= IR_Read_CONTROL_GEAR_Info();
                if( result_read==2)
                {
                  firstDisplay=1;
                  pageUpdate = 1;
                }
                
              }
          }
          
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            IR_PnLGroup(NITOO_LM058A_NODE, CODE_START_GROUP_PNL, CMD_CONTROL_GEAR_CREATE_GROUP);
        }
        
    }

    _common_battery_condition();

#ifndef APP_BAES
    if (TIME_GetTime() >= timeStamp)
    {
        if (enterHiddenPage == 3)
        {
            pageCurrent = PAGE_INDEX_HIDDEN;
            pageSwap = 1;
        }
        else
        {
            enterHiddenPage = 0;
            timeStamp = 0xffffffff;     // Reset timeStamp to infinity
        }
    }
#endif  // #ifndef APP_BAES

    if (pageSwap)
    {
        pageSwap = 0;
        pageInitialize = 1;
        firstDisplay=0;
    }

}
unsigned char _page_read_GroupNumAll(void)
{
#define PAGE_SUBREADPARAMS_RECT_X_START     8
#define PAGE_SUBREADPARAMS_RECT_Y_START     20
#define PAGE_SUBREADPARAMS_RECT_X_END       (LCD_X_LENGTH-PAGE_SUBREADPARAMS_RECT_X_START-1)
#define PAGE_SUBREADPARAMS_BODY_INDENT      4
#define PAGE_SUBREADPARAMS_BODY_X_START     (PAGE_SUBREADPARAMS_RECT_X_START+PAGE_SUBREADPARAMS_BODY_INDENT)
#define PAGE_SUBREADPARAMS_BODY_X_LENGTH    (PAGE_SUBREADPARAMS_RECT_X_END-PAGE_SUBREADPARAMS_BODY_INDENT-PAGE_SUBREADPARAMS_BODY_X_START)
#define PAGE_SUBREADPARAMS_BODY_Y_START     (PAGE_SUBREADPARAMS_RECT_Y_START+PAGE_SUBREADPARAMS_BODY_INDENT)

    unsigned char retry = 0;
    unsigned char yOffset;
    static unsigned char result;
    unsigned char const *string = NULL;
    unsigned char currentParameter;
    const unsigned char *string_zone[]={"code not right"};
    const unsigned char* string_fuck={"crc error"};
     while (retry < PAGE_PARAM_READ_RETRY)
    {
        if (pageForceRead || (result = IR_ReadAllGroupNum()))
        {
            retry = 0;
            switch (result)
            {
            case 1:
                string= string_zone[0];
                break;
            case 5:
                string = string_fuck;
                break;    
                
            case 10:
               // pageCurrent = PAGE_CREATE_GROUP;
                break;
            default:
                string = strings[STRINGS_INDEX_ERRORREADING ];
                break;
            }
           break;  
        }
        else
        {
            if (++retry < PAGE_PARAM_READ_RETRY)
            {
                TIME_Wait(200);
            }
            else
            {
              
                //string=string_fuck;
                string = strings[STRINGS_INDEX_ERRORREADING ];
            }
        }
    }
    //string = string_zone[0];
    if (string != NULL)
    {
        if( PARAM_NUMBER_TYPE2 == PARAMS_GetParameterNumber( &sensorParams, currentParameter ) )
        {
            PARAMS_SetParameterNumber(&sensorParams, PARAM_INDEX_TYPE2, PARAM_NUMBER_ERROR ); 
        }
        else
        {
            yOffset = PAGE_SUBREADPARAMS_BODY_Y_START+LCDD_PutString(PAGE_SUBREADPARAMS_BODY_X_START, PAGE_SUBREADPARAMS_BODY_Y_START, PAGE_SUBREADPARAMS_BODY_X_LENGTH, string, 3, 1)+PAGE_SUBREADPARAMS_BODY_INDENT;
            LCDD_ClearRect(PAGE_SUBREADPARAMS_RECT_X_START, PAGE_SUBREADPARAMS_RECT_Y_START, PAGE_SUBREADPARAMS_RECT_X_END, yOffset, 3);
            LCDD_PutString(PAGE_SUBREADPARAMS_BODY_X_START, PAGE_SUBREADPARAMS_BODY_Y_START, PAGE_SUBREADPARAMS_BODY_X_LENGTH, string, 3, 0);
            LCDD_Refresh();     // Have to do this here.
            TIME_Wait(2000);
            result = 0;            
        }
    }

    pageUpdate = 0;
    pageSwap = 1;

    return result;
}

unsigned char _page_read_zone_info(void)
{
#define PAGE_SUBREADPARAMS_RECT_X_START     8
#define PAGE_SUBREADPARAMS_RECT_Y_START     20
#define PAGE_SUBREADPARAMS_RECT_X_END       (LCD_X_LENGTH-PAGE_SUBREADPARAMS_RECT_X_START-1)
#define PAGE_SUBREADPARAMS_BODY_INDENT      4
#define PAGE_SUBREADPARAMS_BODY_X_START     (PAGE_SUBREADPARAMS_RECT_X_START+PAGE_SUBREADPARAMS_BODY_INDENT)
#define PAGE_SUBREADPARAMS_BODY_X_LENGTH    (PAGE_SUBREADPARAMS_RECT_X_END-PAGE_SUBREADPARAMS_BODY_INDENT-PAGE_SUBREADPARAMS_BODY_X_START)
#define PAGE_SUBREADPARAMS_BODY_Y_START     (PAGE_SUBREADPARAMS_RECT_Y_START+PAGE_SUBREADPARAMS_BODY_INDENT)

    unsigned char retry = 0;
    unsigned char yOffset;
    static unsigned char result;
    unsigned char const *string = NULL;
    unsigned char currentParameter;
    const unsigned char *string_zone[]={"out of range short address","out of range Group address","error","in progress"};
    const unsigned char* string_fuck={"crc error"};
     while (retry < PAGE_PARAM_READ_RETRY)
    {
        if (pageForceRead || (result = IR_ReadCreateZoneInfo()))
        {
            retry = 0;
            switch (result)
            {
            case 1:
                string= string_zone[0];
                break;
            case 2:
                string = string_zone[1];
                break;
            case 3:
                string = string_zone[2];
                break;
            case 4:
                string = string_zone[3];
                break;
            case 5:
                string = string_fuck;
                break;    
                
            case 10:
               // pageCurrent = PAGE_CREATE_GROUP;
                break;
            default:
                string = strings[STRINGS_INDEX_ERRORREADING ];
                break;
            }
           break;  
        }
        else
        {
            if (++retry < PAGE_PARAM_READ_RETRY)
            {
                TIME_Wait(200);
            }
            else
            {
              
                //string=string_fuck;
                string = strings[STRINGS_INDEX_ERRORREADING ];
            }
        }
    }
    //string = string_zone[0];
    if (string != NULL)
    {
        if( PARAM_NUMBER_TYPE2 == PARAMS_GetParameterNumber( &sensorParams, currentParameter ) )
        {
            PARAMS_SetParameterNumber(&sensorParams, PARAM_INDEX_TYPE2, PARAM_NUMBER_ERROR ); 
        }
        else
        {
            yOffset = PAGE_SUBREADPARAMS_BODY_Y_START+LCDD_PutString(PAGE_SUBREADPARAMS_BODY_X_START, PAGE_SUBREADPARAMS_BODY_Y_START, PAGE_SUBREADPARAMS_BODY_X_LENGTH, string, 3, 1)+PAGE_SUBREADPARAMS_BODY_INDENT;
            LCDD_ClearRect(PAGE_SUBREADPARAMS_RECT_X_START, PAGE_SUBREADPARAMS_RECT_Y_START, PAGE_SUBREADPARAMS_RECT_X_END, yOffset, 3);
            LCDD_PutString(PAGE_SUBREADPARAMS_BODY_X_START, PAGE_SUBREADPARAMS_BODY_Y_START, PAGE_SUBREADPARAMS_BODY_X_LENGTH, string, 3, 0);
            LCDD_Refresh();     // Have to do this here.
            TIME_Wait(2000);
            result = 0;            
        }
    }

    pageUpdate = 0;
    pageSwap = 1;

    return result;
}


unsigned char _page_read_detector_info(void)
{
#define PAGE_SUBREADPARAMS_RECT_X_START     8
#define PAGE_SUBREADPARAMS_RECT_Y_START     20
#define PAGE_SUBREADPARAMS_RECT_X_END       (LCD_X_LENGTH-PAGE_SUBREADPARAMS_RECT_X_START-1)
#define PAGE_SUBREADPARAMS_BODY_INDENT      4
#define PAGE_SUBREADPARAMS_BODY_X_START     (PAGE_SUBREADPARAMS_RECT_X_START+PAGE_SUBREADPARAMS_BODY_INDENT)
#define PAGE_SUBREADPARAMS_BODY_X_LENGTH    (PAGE_SUBREADPARAMS_RECT_X_END-PAGE_SUBREADPARAMS_BODY_INDENT-PAGE_SUBREADPARAMS_BODY_X_START)
#define PAGE_SUBREADPARAMS_BODY_Y_START     (PAGE_SUBREADPARAMS_RECT_Y_START+PAGE_SUBREADPARAMS_BODY_INDENT)

    unsigned char retry = 0;
    unsigned char yOffset;
    static unsigned char result;
    unsigned char const *string = NULL;
    unsigned char currentParameter;
    const unsigned char *string_zone[]={"out of range short address","out of range Group address","error","in progress"};
    const unsigned char* string_fuck[]={"CRC error"};
     while (retry < PAGE_PARAM_READ_RETRY)
    {
        if (pageForceRead || (result = IR_ReadDetectorInfo()))
        {
            retry = 0;
            switch (result)
            {
            case 1:
                string= string_zone[0];
                break;
            case 2:
                string = string_zone[1];
                break;
            case 3:
                string = string_zone[2];
                break;
            case 4:
                string = string_zone[3];
                break;
            case 5:
                string = string_fuck[0];
                break;
            case 10:
                pageCurrent = PAGE_INDEX_DetectorFinishCreate;
                break;
            default:
                string = strings[STRINGS_INDEX_ERRORREADING ];
                break;
            }
           break;  
        }
        else
        {
            if (++retry < PAGE_PARAM_READ_RETRY)
            {
                TIME_Wait(200);
            }
            else
            {
              
                //string=string_fuck;
                string = strings[STRINGS_INDEX_ERRORREADING ];
            }
        }
    }
    //string = string_zone[0];
    if (string != NULL)
    {
        if( PARAM_NUMBER_TYPE2 == PARAMS_GetParameterNumber( &sensorParams, currentParameter ) )
        {
            PARAMS_SetParameterNumber(&sensorParams, PARAM_INDEX_TYPE2, PARAM_NUMBER_ERROR ); 
        }
        else
        {
            yOffset = PAGE_SUBREADPARAMS_BODY_Y_START+LCDD_PutString(PAGE_SUBREADPARAMS_BODY_X_START, PAGE_SUBREADPARAMS_BODY_Y_START, PAGE_SUBREADPARAMS_BODY_X_LENGTH, string, 3, 1)+PAGE_SUBREADPARAMS_BODY_INDENT;
            LCDD_ClearRect(PAGE_SUBREADPARAMS_RECT_X_START, PAGE_SUBREADPARAMS_RECT_Y_START, PAGE_SUBREADPARAMS_RECT_X_END, yOffset, 3);
            LCDD_PutString(PAGE_SUBREADPARAMS_BODY_X_START, PAGE_SUBREADPARAMS_BODY_Y_START, PAGE_SUBREADPARAMS_BODY_X_LENGTH, string, 3, 0);
            LCDD_Refresh();     // Have to do this here.
            TIME_Wait(2000);
            result = 0;            
        }
    }

    pageUpdate = 0;
    pageSwap = 1;

    return result;
}

unsigned char _page_read_GroupNum(unsigned char DATAx,unsigned char flag,unsigned char InOrEx)
{
#define PAGE_SUBREADPARAMS_RECT_X_START     8
#define PAGE_SUBREADPARAMS_RECT_Y_START     20
#define PAGE_SUBREADPARAMS_RECT_X_END       (LCD_X_LENGTH-PAGE_SUBREADPARAMS_RECT_X_START-1)
#define PAGE_SUBREADPARAMS_BODY_INDENT      4
#define PAGE_SUBREADPARAMS_BODY_X_START     (PAGE_SUBREADPARAMS_RECT_X_START+PAGE_SUBREADPARAMS_BODY_INDENT)
#define PAGE_SUBREADPARAMS_BODY_X_LENGTH    (PAGE_SUBREADPARAMS_RECT_X_END-PAGE_SUBREADPARAMS_BODY_INDENT-PAGE_SUBREADPARAMS_BODY_X_START)
#define PAGE_SUBREADPARAMS_BODY_Y_START     (PAGE_SUBREADPARAMS_RECT_Y_START+PAGE_SUBREADPARAMS_BODY_INDENT)

    unsigned char retry = 0;
    unsigned char yOffset;
    static unsigned char result;
    unsigned char const *string = NULL;
    unsigned char currentParameter;
    const unsigned char *string_zone[]={"Set error","flag error"};
    const unsigned char* string_fuck[]={"CRC error"};
     while (retry < PAGE_PARAM_READ_RETRY)
    {
        if (pageForceRead || (result = IR_ReadGroupNum(DATAx,flag,InOrEx)))
        {
            retry = 0;
            switch (result)
            {
            case 1:
                string= string_zone[0];
                break;
            case 2:
                string = string_zone[1];
                break;
            case 3:
                string = string_zone[2];
                break;
            case 4:
                string = string_zone[3];
                break;
            case 5:
                string = string_fuck[0];
                break;
            case 10:
               // pageCurrent = PAGE_INDEX_DetectorFinishCreate;
                break;
            default:
                string = strings[STRINGS_INDEX_ERRORREADING ];
                break;
            }
           break;  
        }
        else
        {
            if (++retry < PAGE_PARAM_READ_RETRY)
            {
                TIME_Wait(200);
            }
            else
            {
              
                //string=string_fuck;
                string = strings[STRINGS_INDEX_ERRORREADING ];
            }
        }
    }
    //string = string_zone[0];
    if (string != NULL)
    {
        if( PARAM_NUMBER_TYPE2 == PARAMS_GetParameterNumber( &sensorParams, currentParameter ) )
        {
            PARAMS_SetParameterNumber(&sensorParams, PARAM_INDEX_TYPE2, PARAM_NUMBER_ERROR ); 
        }
        else
        {
            yOffset = PAGE_SUBREADPARAMS_BODY_Y_START+LCDD_PutString(PAGE_SUBREADPARAMS_BODY_X_START, PAGE_SUBREADPARAMS_BODY_Y_START, PAGE_SUBREADPARAMS_BODY_X_LENGTH, string, 3, 1)+PAGE_SUBREADPARAMS_BODY_INDENT;
            LCDD_ClearRect(PAGE_SUBREADPARAMS_RECT_X_START, PAGE_SUBREADPARAMS_RECT_Y_START, PAGE_SUBREADPARAMS_RECT_X_END, yOffset, 3);
            LCDD_PutString(PAGE_SUBREADPARAMS_BODY_X_START, PAGE_SUBREADPARAMS_BODY_Y_START, PAGE_SUBREADPARAMS_BODY_X_LENGTH, string, 3, 0);
            LCDD_Refresh();     // Have to do this here.
            TIME_Wait(2000);
            result = 0;            
        }
    }

    pageUpdate = 0;
    pageSwap = 1;

    return result;
}




void DALIOffSetHandler(void)
{
  //#define PAGE_DAYLIGHTSETPOINT_TITLE_X_POS   20
#define PAGE_DAYLIGHTSETPOINT_BODY_X_POS    40
#define PAGE_DAYLIGHTSETPOINT_BODY_Y_POS    PAGE_VALUE_Y_POS
#define PAGE_DAYLIGHTSETPOINT_EYE_X_POS     40      // 2.3.0
#define PAGE_DAYLIGHTSETPOINT_EYE_Y_POS     36      // 2.3.0
#define PAGE_DAYLIGHTSETPOINT_EYE_WIDTH     48      // 2.3.0
#define PAGE_DAYLIGHTSETPOINT_EYE_HEIGHT    32      // 2.3.0

    static PAGE_PARAMETER *pParameter;
    unsigned char pDaylightsetpoint[9];     // 2.2.0, was 5

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_OFFset);
        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "Offset"), 0, 0, "Offset", 0, 0);
        
        if (pageAvailability)
        {
          _param_page_funckey_initializer(FUNCKEY_READ_WRITE);
          
          pParameter = (PAGE_PARAMETER *)malloc(2*sizeof(PAGE_PARAMETER));        // 2.3.1, was 1
          
          pParameter->ParamIndex = PARAM_INDEX_OFFset;
          pParameter->Max = 0x64;
          pParameter->Min = 0;
          
          pageUpdate = 1;
        }
        else
        {
            _draw_rectangular_for_value();      // 2.3.0
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        // 2.3.0
         LCDD_ClearRect(0, PAGE_DAYLIGHTSETPOINT_EYE_Y_POS, LCD_X_LENGTH-1, PAGE_DAYLIGHTSETPOINT_EYE_Y_POS+PAGE_DAYLIGHTSETPOINT_EYE_HEIGHT-1, 0);
         _draw_rectangular_for_value();

            dectoascii(&pDaylightsetpoint[0], PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_OFFset), 3);
            {
                strcpy((char *)&pDaylightsetpoint[3], "%");
            }
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, &pDaylightsetpoint[0]), PAGE_DAYLIGHTSETPOINT_BODY_Y_POS, 0, &pDaylightsetpoint[0], PAGE_HIGHLIGHT_COLOR, 0);    // 2.2.0
      

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)//++
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT)//--
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {

            _page_sub_read_parameters(pParameter, 1);       // 2.3.1, was 1

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }
    else if (pageAvailability && (BUTTON_GetMessage() == BUTTON_MESSAGE_PRESSED) && ((BUTTON_GetPressedTime() & 0x3F) == 0))
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_RIGHT)
        {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT)
        {
            _adjust_param_value(pParameter, -1);

            pageUpdate = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter!=NULL)
        {
            free(pParameter);
            pParameter=NULL;
        }

        pageSwap = 0;
        pageInitialize = 1;
    }

}

void DALIDetectionLEDHandler(void)
{
  //#define PAGE_DAYLIGHTSETPOINT_TITLE_X_POS   20
#define PAGE_DAYLIGHTSETPOINT_BODY_X_POS    40
#define PAGE_DAYLIGHTSETPOINT_BODY_Y_POS    PAGE_VALUE_Y_POS
#define PAGE_DAYLIGHTSETPOINT_EYE_X_POS     40      // 2.3.0
#define PAGE_DAYLIGHTSETPOINT_EYE_Y_POS     36      // 2.3.0
#define PAGE_DAYLIGHTSETPOINT_EYE_WIDTH     48      // 2.3.0
#define PAGE_DAYLIGHTSETPOINT_EYE_HEIGHT    32      // 2.3.0

    static PAGE_PARAMETER *pParameter;
    unsigned char pDaylightsetpoint[3];     // 2.2.0, was 5
    unsigned short temp;
    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_DetectionLed);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);


       LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "Detection LED"), 0, 0, "Detection LED", 0, 0);

        if (pageAvailability)
        {
        _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

        pParameter = (PAGE_PARAMETER *)malloc(2*sizeof(PAGE_PARAMETER));        // 2.3.1, was 1

        pParameter->ParamIndex = PARAM_INDEX_DetectionLed;
        pParameter->Max = 0x01;
        pParameter->Min = 0;

        pageUpdate = 1;
        }
        else
        {
            _draw_rectangular_for_value();      // 2.3.0
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        // 2.3.0
         LCDD_ClearRect(0, PAGE_DAYLIGHTSETPOINT_EYE_Y_POS, LCD_X_LENGTH-1, PAGE_DAYLIGHTSETPOINT_EYE_Y_POS+PAGE_DAYLIGHTSETPOINT_EYE_HEIGHT-1, 0);
         _draw_rectangular_for_value();
         temp=PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_DetectionLed);
         if (temp)
          strcpy((char *)&pDaylightsetpoint[0], "ON");
         else
          strcpy((char *)&pDaylightsetpoint[0], "OFF");          
         LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, &pDaylightsetpoint[0]), PAGE_DAYLIGHTSETPOINT_BODY_Y_POS, 0, &pDaylightsetpoint[0], PAGE_HIGHLIGHT_COLOR, 0);    // 2.2.0
      

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR)//++
        {
            if (pageAvailability)
            {
            _adjust_param_value(pParameter, 1);

            pageUpdate = 1;
            }
        }

        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
                // 2.3.1, reset Type2 parameter number in the profile for reading Line Voltage PIR and/or US sensors.
                if (PARAMS_IsLineVoltageSensor(&sensorParams) && (!PARAMS_IsZigbeeSensor(&sensorParams)) && (!PARAMS_SensorHasHF(&sensorParams)))
                {
                    PARAMS_SetParameterNumber(&sensorParams, PARAM_INDEX_TYPE2, PARAM_NUMBER_TYPE2);
                }

            _page_sub_read_parameters(pParameter, 2);       // 2.3.1, was 1

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }




    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }

}
void ModePageHandler(void)
{
  //#define PAGE_MODE_TITLE_X_POS       48
#define PAGE_MODE_NAME_X_POS        2
#define PAGE_MODE_VALUE_X_POS       112
#define PAGE_MODE_BODY_Y_POS_1      18
#define PAGE_MODE_BODY_Y_POS_2      40
#define PAGE_MODE_BODY_Y_POS_3      62
#define PAGE_MODE_BODY_Y_POS_4      84

    static const PAGE_ITEM_RECT rect[4] = {{PAGE_MODE_BODY_Y_POS_1-1, PAGE_MODE_BODY_Y_POS_1+16}, {PAGE_MODE_BODY_Y_POS_2-1, PAGE_MODE_BODY_Y_POS_2+16}, {PAGE_MODE_BODY_Y_POS_3-1, PAGE_MODE_BODY_Y_POS_3+16}, {PAGE_MODE_BODY_Y_POS_4-1, PAGE_MODE_BODY_Y_POS_4+16}};
//    static unsigned char previousItem;    // 2.3.0
    static unsigned char currentItem;
    static unsigned char totalItems;
    static PAGE_PARAMETER *pParameter;
    static unsigned char mode_value;
    static unsigned char possible_values[5];    // 2.3.0
    unsigned char i, firstDisplayedItem;
//    const unsigned char *string;      // 2.3.0

    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MODE);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, strings[STRINGS_INDEX_MODE ]), 0, 0, strings[STRINGS_INDEX_MODE ], 0, 0);

        if (pageAvailability)
        {
        _param_page_funckey_initializer(FUNCKEY_READ_WRITE);

        pParameter = (PAGE_PARAMETER *)malloc(2*sizeof(PAGE_PARAMETER));        // 2.3.1, was 1
        pParameter->ParamIndex = PARAM_INDEX_MODE;
        mode_value = PARAMS_GetParameterValue(&sensorParams, PARAM_INDEX_MODE);

        // 2.3.1, hide one more parameter Type2 in the list in order to read to determine if Pulse available to display
        (pParameter+1)->ParamIndex = PARAM_INDEX_TYPE2;

        possible_values[0] = 1;
        totalItems = 1;

        if (PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_INITIAL) && \
            PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_MAINTAIN) && \
            PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_RETRIGGER)
            )
        {
            possible_values[totalItems] = 2;
            totalItems++;
        }

        possible_values[totalItems] = 3;
        totalItems++;

        // 2.3.0
        if ((PARAMS_IsLineVoltageSensor(&sensorParams) && PARAMS_IsZigbeeSensor(&sensorParams)) || PARAMS_IsMultiChannelAvailable(&sensorParams) || PARAMS_IsDigitalSensor(&sensorParams))      // 2.3.1
        {
            possible_values[totalItems] = 5;
            totalItems++;
        }

        // 2.3.0
        if (PARAMS_IsZigbeeSensor(&sensorParams))
        {
            possible_values[totalItems] = 6;
            totalItems++;
        }

//        previousItem = 0xFF;      // 2.3.0
        currentItem = 0;
        for (i=0; i<totalItems; i++)
        {
            if (mode_value == possible_values[i])
            {
                currentItem = i;
                break;
            }
        }

        /* 2.3.0
        for (i=0; i<totalItems; i++)
        {
            switch (possible_values[i])
            {
            case 1:
                string = strings[STRINGS_INDEX_AUTOONOFF ];
                break;
            case 2:
                string = strings[STRINGS_INDEX_AUTOWALKTHRU ];
                break;
            case 3:
                string = strings[STRINGS_INDEX_MANUALONAUTOOFF ];
                break;
            case 5:
                string = strings[STRINGS_INDEX_PARTIALONGROUPOFF ];
                break;
            default:
                string = NULL;
                break;
            }

            if (string)
            {
                LCDD_PutString(PAGE_MODE_NAME_X_POS, rect[i].yStart+1, 0, string, 0, 0);
            }
        }
        2.3.0 */

        pageScroll = 0;  // 2.4.0

        pageUpdate = 1;
        }
        else
        {
            _draw_rectangular_for_value();
            _param_page_funckey_initializer(FUNCKEY_NONE);
            LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, " N/A "), PAGE_VALUE_Y_POS, 0, " N/A ", 0, 0);
            pageForceRead = 0;
        }
    }

    if (pageUpdate)
    {
        firstDisplayedItem = 0;

        if (currentItem > 2)
        {
            firstDisplayedItem = currentItem - 2;

            if (currentItem == totalItems - 1)
            {
                firstDisplayedItem = totalItems - 4;
            }
        }

        for (i=firstDisplayedItem; i<firstDisplayedItem+4; i++)
        {
            if (i < totalItems)
            {
                LCDD_ClearRect(0, rect[i-firstDisplayedItem].yStart, LCD_X_LENGTH-1, rect[i-firstDisplayedItem].yEnd, (i == currentItem) ? PAGE_HIGHLIGHT_COLOR:0);
                // 2.4.0
//                LCDD_PutString(PAGE_MODE_NAME_X_POS, rect[i-firstDisplayedItem].yStart+1, 0, strings[STRINGS_INDEX_MODE+possible_values[i]*STRINGS_TOTAL_LANGUAGES ], (i == currentItem) ? PAGE_HIGHLIGHT_COLOR:0, 0);
                if (LCDD_PutScrollString(PAGE_MODE_NAME_X_POS, rect[i-firstDisplayedItem].yStart+1, (PAGE_MODE_VALUE_X_POS-2-PAGE_MODE_NAME_X_POS), strings[STRINGS_INDEX_MODE+possible_values[i]*STRINGS_TOTAL_LANGUAGES ]+((i == currentItem)?pageScroll:0), (i == currentItem)?PAGE_HIGHLIGHT_COLOR:0, (i == currentItem)?1:0))
                {
                    pageScroll++;
                    pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
                }
                LCDD_ClearRect(PAGE_MODE_VALUE_X_POS-2, rect[i-firstDisplayedItem].yStart+1, LCD_X_LENGTH-2, rect[i-firstDisplayedItem].yEnd-1, 0);
                LCDD_PutBitmap(PAGE_MODE_VALUE_X_POS, rect[i-firstDisplayedItem].yStart+1, (mode_value == possible_values[i])?&BitmapInfo_Validation[1]:&BitmapInfo_Validation[0], 0);
            }
        }

        /* 2.3.0
        if (currentItem != previousItem)
        {
            if (previousItem != 0xFF)
            {
                LCDD_InvertRect(0, rect[previousItem].yStart, LCD_X_LENGTH-1, rect[previousItem].yEnd, PAGE_HIGHLIGHT_COLOR);
            }
            LCDD_InvertRect(0, rect[currentItem].yStart, LCD_X_LENGTH-1, rect[currentItem].yEnd, PAGE_HIGHLIGHT_COLOR);

            previousItem = currentItem;
        }

        for (i=0; i<totalItems; i++)
        {
            LCDD_ClearRect(PAGE_MODE_VALUE_X_POS-2, rect[i].yStart+1, LCD_X_LENGTH-2, rect[i].yEnd-1, 0);
            LCDD_PutBitmap(PAGE_MODE_VALUE_X_POS, rect[i].yStart+1, (mode_value == possible_values[i])?&BitmapInfo_Validation[1]:&BitmapInfo_Validation[0], 0);
        }
        2.3.0 */

        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            if (currentItem == 0)
            {
                currentItem = totalItems-1;
            }
            else
            {
                currentItem--;
            }

            pageScroll = 0;  // 2.4.0
            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            if (currentItem == totalItems-1)
            {
                currentItem = 0;
            }
            else
            {
                currentItem++;
            }

            pageScroll = 0;  // 2.4.0
            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_MAJOR)
        {
            if (pageAvailability)
            {
            mode_value = possible_values[currentItem];
            PARAMS_SetParameterValue(&sensorParams, PARAM_INDEX_MODE, mode_value);

            pageScroll = 0;  // 2.4.0
            pageUpdate = 1;
            }
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {
            if (pageAvailability)
            {
                // 2.3.1, reset Type2 parameter number in the profile for reading Line Voltage PIR and/or US sensors.
                if (PARAMS_IsLineVoltageSensor(&sensorParams) && (!PARAMS_IsZigbeeSensor(&sensorParams)) && (!PARAMS_SensorHasHF(&sensorParams)))
                {
                    PARAMS_SetParameterNumber(&sensorParams, PARAM_INDEX_TYPE2, PARAM_NUMBER_TYPE2);
                }

            _page_sub_read_parameters(pParameter, 2);       // 2.3.1, was 1

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
            if (pageAvailability)
            {
            _page_sub_set_parameters(pParameter, 1);

//            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_PARAMETERS;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    // 2.4.0
    if (TIME_GetTime() >= pageTimeStamp)
    {
        pageTimeStamp = TIME_GetTime() + LCDD_SCROLL_DELAY;
        pageUpdate = 1;
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }

}

void DALIZoneViewWindows(void)
{

#define PAGE_SENSITIVITY_MAXIMUMITEMS       3
#define PAGE_SENSITIVITY_NAME_X_POS         4
#define PAGE_SENSITIVITY_VALUE_X_POS        40
#define PAGE_SENSITIVITY_BRACKET_X_POS      114
#define PAGE_SENSITIVITY_BODY_Y_POS_1       18
#define PAGE_SENSITIVITY_BODY_Y_POS_DELTA   22
#define PAGE_SENSITIVITY_BODY_Y_POS_2       (PAGE_SENSITIVITY_BODY_Y_POS_1 + PAGE_SENSITIVITY_BODY_Y_POS_DELTA)
#define PAGE_SENSITIVITY_BODY_Y_POS_3       (PAGE_SENSITIVITY_BODY_Y_POS_2 + PAGE_SENSITIVITY_BODY_Y_POS_DELTA)
    PAGE_PARAMETER *pTempParameter;
   // static unsigned char previousItem;
    static unsigned char currentItem=1;
    static unsigned char totalItems=2;
    static PAGE_PARAMETER *pParameter;
    unsigned char yOffset;
    yOffset = PAGE_SENSITIVITY_BODY_Y_POS_1;
    unsigned char pDaylightsetpoint[9]; 
    
    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = 1;//PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_HF_SENS) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_PIR_SENS) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_US_SENS);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

      //  LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1,0, 0, "Choose Zone", 0, 0);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "Windows Zone"), 0, 0, "Windows Zone", 0, 0); 
 

        if (pageAvailability)
        {
        _param_pageDali_funckey_initializer(FUNCKEY_WRITE_ONLY);
          currentItem = 1;
          pParameter = (PAGE_PARAMETER *)malloc(PAGE_SENSITIVITY_MAXIMUMITEMS*sizeof(PAGE_PARAMETER));
          pTempParameter = pParameter;
          yOffset = PAGE_SENSITIVITY_BODY_Y_POS_1;
          pageUpdate = 1;
        }
    }


    if (pageUpdate)
    {
      
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "ON", (currentItem == 1)?3:0, 0);
       // LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, "OFF", (currentItem == 2)?3:0, 0);               
        
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, 0);

        LCDD_PutString(20-5, PAGE_SETTINGS_LINE_4, 0, "    of Control Gear", 0, 0);
        dectoascii(&pDaylightsetpoint[0],view.XX, 3);
        LCDD_PutString(5, PAGE_SETTINGS_LINE_4, 0, &pDaylightsetpoint[0],PAGE_HIGHLIGHT_COLOR, 0);
  
        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            if (currentItem == 1)
            {
                currentItem = totalItems;
            }
            else
            {
                currentItem--;
            }

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            if (currentItem == totalItems)
            {
                currentItem = 1;
            }
            else
            {
                currentItem++;
            }

            pageUpdate = 1;
            }
        }

        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT)
        {

            pageUpdate = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {

        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
          if(currentItem==1)
          {
            IR_DirectlySetAreaOnOFF(0x01,0x01);
            TIME_Wait(20);
            IR_PnLGroup(0x01,CODE_ANNOUNCE_ID,CMD_UNIT_DESCRIPTION_REQUEST);
            IR_Read_UNIT_DESCRIPTION_Info(0x00);
          }
          if(currentItem==2)
          {
            IR_DirectlySetAreaOnOFF(0x01,0x02);
            TIME_Wait(20);
            IR_PnLGroup(0x01,CODE_ANNOUNCE_ID,CMD_UNIT_DESCRIPTION_REQUEST);
            IR_Read_UNIT_DESCRIPTION_Info(0x00);
          }

        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }

}

void DALIZoneViewCorridor(void)
{
  
#define PAGE_SENSITIVITY_MAXIMUMITEMS       3
#define PAGE_SENSITIVITY_NAME_X_POS         4
#define PAGE_SENSITIVITY_VALUE_X_POS        40
#define PAGE_SENSITIVITY_BRACKET_X_POS      114
#define PAGE_SENSITIVITY_BODY_Y_POS_1       18
#define PAGE_SENSITIVITY_BODY_Y_POS_DELTA   22
#define PAGE_SENSITIVITY_BODY_Y_POS_2       (PAGE_SENSITIVITY_BODY_Y_POS_1 + PAGE_SENSITIVITY_BODY_Y_POS_DELTA)
#define PAGE_SENSITIVITY_BODY_Y_POS_3       (PAGE_SENSITIVITY_BODY_Y_POS_2 + PAGE_SENSITIVITY_BODY_Y_POS_DELTA)
    PAGE_PARAMETER *pTempParameter;
   // static unsigned char previousItem;
    static unsigned char currentItem=1;
    static unsigned char totalItems=2;
    static PAGE_PARAMETER *pParameter;
    unsigned char yOffset;
    yOffset = PAGE_SENSITIVITY_BODY_Y_POS_1;
    unsigned char pDaylightsetpoint[9]; 
    
    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = 1;//PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_HF_SENS) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_PIR_SENS) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_US_SENS);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

      //  LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1,0, 0, "Choose Zone", 0, 0);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "Corridor Zone"), 0, 0, "Corridor Zone", 0, 0); 
 

        if (pageAvailability)
        {
         _param_pageDali_funckey_initializer(FUNCKEY_WRITE_ONLY);
          currentItem = 1;
          pParameter = (PAGE_PARAMETER *)malloc(PAGE_SENSITIVITY_MAXIMUMITEMS*sizeof(PAGE_PARAMETER));
          pTempParameter = pParameter;
          yOffset = PAGE_SENSITIVITY_BODY_Y_POS_1;
          pageUpdate = 1;
        }
    }


    if (pageUpdate)
    {
      
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "ON", (currentItem == 1)?3:0, 0);
       // LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, "OFF", (currentItem == 2)?3:0, 0);               
        
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, 0);

        LCDD_PutString(20-5, PAGE_SETTINGS_LINE_4, 0, "    of Control Gear", 0, 0);
        dectoascii(&pDaylightsetpoint[0],view.XX, 3);
        LCDD_PutString(5, PAGE_SETTINGS_LINE_4, 0, &pDaylightsetpoint[0],PAGE_HIGHLIGHT_COLOR, 0);
  
        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            if (currentItem == 1)
            {
                currentItem = totalItems;
            }
            else
            {
                currentItem--;
            }

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            if (currentItem == totalItems)
            {
                currentItem = 1;
            }
            else
            {
                currentItem++;
            }

            pageUpdate = 1;
            }
        }

        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT)
        {

            pageUpdate = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {

        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
          if(currentItem==1)
          {
            if((sensorParams.Type.Value==PARAM_TYPE_DALI_MASTER_DL_PIR_US) || (sensorParams.Type.Value==PARAM_TYPE_DALI_MASTER_DL_OP_PIR_US) || (sensorParams.Type.Value==PARAM_TYPE_DALI_MASTER_OP_PIR_US))
            { IR_DirectlySetAreaOnOFF(0x02,0x01);
            TIME_Wait(20);
            IR_PnLGroup(0x02,CODE_ANNOUNCE_ID,CMD_UNIT_DESCRIPTION_REQUEST);
            IR_Read_UNIT_DESCRIPTION_Info(0x01);
            }
            else
            {
              IR_DirectlySetAreaOnOFF(0x01,0x01);
              TIME_Wait(20);
              IR_PnLGroup(0x01,CODE_ANNOUNCE_ID,CMD_UNIT_DESCRIPTION_REQUEST);
              IR_Read_UNIT_DESCRIPTION_Info(0x01);
            }
            
          }
          if(currentItem==2)
          {
            if((sensorParams.Type.Value==PARAM_TYPE_DALI_MASTER_DL_PIR_US) || (sensorParams.Type.Value==PARAM_TYPE_DALI_MASTER_DL_OP_PIR_US) || (sensorParams.Type.Value==PARAM_TYPE_DALI_MASTER_OP_PIR_US))            
            { IR_DirectlySetAreaOnOFF(0x02,0x02);
            TIME_Wait(20);
            IR_PnLGroup(0x02,CODE_ANNOUNCE_ID,CMD_UNIT_DESCRIPTION_REQUEST);
            IR_Read_UNIT_DESCRIPTION_Info(0x01);
            }
            else
            {
              IR_DirectlySetAreaOnOFF(0x01,0x02);
              TIME_Wait(20);
              IR_PnLGroup(0x01,CODE_ANNOUNCE_ID,CMD_UNIT_DESCRIPTION_REQUEST);
              IR_Read_UNIT_DESCRIPTION_Info(0x01);
            
            }
          }
          

        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }

}
void DALIZoneViewBoard(void)
{
  
#define PAGE_SENSITIVITY_MAXIMUMITEMS       3
#define PAGE_SENSITIVITY_NAME_X_POS         4
#define PAGE_SENSITIVITY_VALUE_X_POS        40
#define PAGE_SENSITIVITY_BRACKET_X_POS      114
#define PAGE_SENSITIVITY_BODY_Y_POS_1       18
#define PAGE_SENSITIVITY_BODY_Y_POS_DELTA   22
#define PAGE_SENSITIVITY_BODY_Y_POS_2       (PAGE_SENSITIVITY_BODY_Y_POS_1 + PAGE_SENSITIVITY_BODY_Y_POS_DELTA)
#define PAGE_SENSITIVITY_BODY_Y_POS_3       (PAGE_SENSITIVITY_BODY_Y_POS_2 + PAGE_SENSITIVITY_BODY_Y_POS_DELTA)
    PAGE_PARAMETER *pTempParameter;
   // static unsigned char previousItem;
    static unsigned char currentItem=1;
    static unsigned char totalItems=2;
    static PAGE_PARAMETER *pParameter;
    unsigned char yOffset;
    yOffset = PAGE_SENSITIVITY_BODY_Y_POS_1;
    unsigned char pDaylightsetpoint[9]; 
    
    if (pageInitialize)
    {
        pageInitialize = 0;

        pageAvailability = 1;//PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_HF_SENS) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_PIR_SENS) || PARAMS_IsParameterAvailable(&sensorParams, PARAM_INDEX_US_SENS);

        LCDD_ClearRect(0, 0, LCD_X_LENGTH-1, LCD_Y_LENGTH-1, 0);

      //  LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1,0, 0, "Choose Zone", 0, 0);
        LCDD_PutString(_figure_string_center_x_pos(0, LCD_X_LENGTH-1, "Board Zone"), 0, 0, "Board Zone", 0, 0); 
 

        if (pageAvailability)
        {
          _param_page_funckey_initializer(FUNCKEY_WRITE_ONLY);
          currentItem = 1;
          pParameter = (PAGE_PARAMETER *)malloc(PAGE_SENSITIVITY_MAXIMUMITEMS*sizeof(PAGE_PARAMETER));
          pTempParameter = pParameter;
          yOffset = PAGE_SENSITIVITY_BODY_Y_POS_1;
          pageUpdate = 1;
        }
    }


    if (pageUpdate)
    {
      
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_2, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_2+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 1)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_2, 0, "ON", (currentItem == 1)?3:0, 0);
       // LCDD_PutBitmap(PAGE_ARROW_NAV_X_POS, PAGE_SETTINGS_LINE_1, (currentItem == 0)?&BitmapInfo_ArrowNav[1]:&BitmapInfo_ArrowNav[0], 0);

        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_3, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_3+PAGE_SETTINGS_LINE_HEIGHT-1, (currentItem == 2)?3:0);
        LCDD_PutString(PAGE_LINE_INDENT, PAGE_SETTINGS_LINE_3, 0, "OFF", (currentItem == 2)?3:0, 0);               
        
        LCDD_ClearRect(0, PAGE_SETTINGS_LINE_4, PAGE_ARROW_NAV_X_POS-1, PAGE_SETTINGS_LINE_4+PAGE_SETTINGS_LINE_HEIGHT-1, 0);

        LCDD_PutString(20-5, PAGE_SETTINGS_LINE_4, 0, "    of Control Gear", 0, 0);
        dectoascii(&pDaylightsetpoint[0],view.XX, 3);
        LCDD_PutString(5, PAGE_SETTINGS_LINE_4, 0, &pDaylightsetpoint[0],PAGE_HIGHLIGHT_COLOR, 0);
  
        pageUpdate = 0;
    }

    _common_funckey_handler();

    if ((BUTTON_GetMessage() == BUTTON_MESSAGE_PUSHED) || (BUTTON_GetMessage() == BUTTON_MESSAGE_RELEASED) || pageForceRead)
    {
        if (BUTTON_GetButtonIndex() == BUTTON_INDEX_UP)
        {
            if (pageAvailability)
            {
            if (currentItem == 1)
            {
                currentItem = totalItems;
            }
            else
            {
                currentItem--;
            }

            pageUpdate = 1;
            }
        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_DOWN)
        {
            if (pageAvailability)
            {
            if (currentItem == totalItems)
            {
                currentItem = 1;
            }
            else
            {
                currentItem++;
            }

            pageUpdate = 1;
            }
        }

        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_LEFT)
        {

            pageUpdate = 1;
        }
        else if ((BUTTON_GetButtonIndex() == BUTTON_INDEX_F2) || pageForceRead)
        {

        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F3)
        {
          if(currentItem==1)
          {
            IR_DirectlySetAreaOnOFF(0x03,0x01);
            TIME_Wait(20);
            IR_PnLGroup(0x03,CODE_ANNOUNCE_ID,CMD_UNIT_DESCRIPTION_REQUEST);
            IR_Read_UNIT_DESCRIPTION_Info(0x02);
          }
          if(currentItem==2)
          {
            IR_DirectlySetAreaOnOFF(0x03,0x02);
            TIME_Wait(20);
            IR_PnLGroup(0x03,CODE_ANNOUNCE_ID,CMD_UNIT_DESCRIPTION_REQUEST);
            IR_Read_UNIT_DESCRIPTION_Info(0x02);
          }

        }
        else if (BUTTON_GetButtonIndex() == BUTTON_INDEX_F1)
        {
            pageCurrent = PAGE_INDEX_MAIN;

            pageSwap = 1;
        }

//        BUTTON_Acknowledge();
    }

    _common_battery_condition();

    if (pageSwap)
    {
        if (pParameter)
        {
            free(pParameter);
        }

        pageSwap = 0;
        pageInitialize = 1;
    }
}
