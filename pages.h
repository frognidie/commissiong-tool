#ifndef PAGES_H
#define PAGES_H

#include "languages.h"
//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

//#define PAGE_MENUS_FOR_READONLY_ITEMS   1
//#define MANTIS4974
#ifndef APP_BAES
#define PAGE_INDEX_FACTORY   0XFF
// Common pages
#define PAGE_INDEX_MAIN                 0
#define PAGE_INDEX_READ_PARAMETERS      (PAGE_INDEX_MAIN+1)
#define PAGE_INDEX_LIST_PARAMETERS      (PAGE_INDEX_READ_PARAMETERS+1)
#define PAGE_INDEX_SAVEFILE             (PAGE_INDEX_LIST_PARAMETERS+1)
#define PAGE_INDEX_NEWFILE              (PAGE_INDEX_SAVEFILE+1)
#define PAGE_INDEX_REPLACEFILE          (PAGE_INDEX_NEWFILE+1)
#define PAGE_INDEX_PARAMETERS           (PAGE_INDEX_REPLACEFILE+1)
#define PAGE_INDEX_ADVANCEDMODE         (PAGE_INDEX_PARAMETERS+1)
#define PAGE_INDEX_HIDDEN               (PAGE_INDEX_ADVANCEDMODE+1)
#define PAGE_INDEX_DATE                 (PAGE_INDEX_HIDDEN+1)           // 2.4.0
#define PAGE_INDEX_TIMEDELAY            (PAGE_INDEX_DATE+1)
#define PAGE_INDEX_SENSITIVITY          (PAGE_INDEX_TIMEDELAY+1)
#define PAGE_INDEX_CALIBRATION          (PAGE_INDEX_SENSITIVITY+1)
#define PAGE_INDEX_DAYLIGHTSETPOINT     (PAGE_INDEX_CALIBRATION+1)
#define PAGE_INDEX_MODE                 (PAGE_INDEX_DAYLIGHTSETPOINT+1)
#define PAGE_INDEX_DETECTIONSCHEME      (PAGE_INDEX_MODE+1)
#define PAGE_INDEX_ALERT                (PAGE_INDEX_DETECTIONSCHEME+1)
#define PAGE_INDEX_STANDBYLEVEL         (PAGE_INDEX_ALERT+1)
#define PAGE_INDEX_STANDBYDELAY         (PAGE_INDEX_STANDBYLEVEL+1)
#define PAGE_INDEX_SENDCONDITION        (PAGE_INDEX_STANDBYDELAY+1)
#define PAGE_INDEX_DEADBAND             (PAGE_INDEX_SENDCONDITION+1)
#define PAGE_INDEX_TIMEBASE             (PAGE_INDEX_DEADBAND+1)
#define PAGE_INDEX_BANDWIDTH            (PAGE_INDEX_TIMEBASE+1)
#define PAGE_INDEX_SUMMERHOUR           (PAGE_INDEX_BANDWIDTH+1)        // 2.4.0
#define PAGE_INDEX_COUNTRY              (PAGE_INDEX_SUMMERHOUR+1)       // 2.4.0
#define PAGE_INDEX_LIGHTREGULATION      (PAGE_INDEX_COUNTRY+1)
#define PAGE_INDEX_PROVISIONOFLIGHT     (PAGE_INDEX_LIGHTREGULATION+1)
#define PAGE_INDEX_LOOPTYPE             (PAGE_INDEX_PROVISIONOFLIGHT+1)
#define PAGE_INDEX_DAYLIGHTFACTOR       (PAGE_INDEX_LOOPTYPE+1)
#define PAGE_INDEX_DAYLIGHTFACTORCEN    (PAGE_INDEX_DAYLIGHTFACTOR+1)
#define PAGE_INDEX_NATURALLIGHTFACTOR   (PAGE_INDEX_DAYLIGHTFACTORCEN+1)
#define PAGE_INDEX_DAYLIGHTLEVEL        (PAGE_INDEX_NATURALLIGHTFACTOR+1)
#define PAGE_INDEX_SENSORSTATUS         (PAGE_INDEX_DAYLIGHTLEVEL+1)
#define PAGE_INDEX_CURRENTPOL           (PAGE_INDEX_SENSORSTATUS+1)
#define PAGE_INDEX_DEVICEIDZIGBEE       (PAGE_INDEX_CURRENTPOL+1)
#define PAGE_INDEX_OTHERPARAMETERS      (PAGE_INDEX_DEVICEIDZIGBEE+1)
#define PAGE_INDEX_BUZZERFEEDBACK       (PAGE_INDEX_OTHERPARAMETERS+1)
#define PAGE_INDEX_LOADTYPE             (PAGE_INDEX_BUZZERFEEDBACK+1)
#define PAGE_INDEX_POWERLINEFILTER      (PAGE_INDEX_LOADTYPE+1)
#define PAGE_INDEX_TYPE                 (PAGE_INDEX_POWERLINEFILTER+1)
#define PAGE_INDEX_FIRMWAREVERSION      (PAGE_INDEX_TYPE+1)
#define PAGE_INDEX_ID                   (PAGE_INDEX_FIRMWAREVERSION+1)
#define PAGE_INDEX_REFERENT             (PAGE_INDEX_ID+1)
#define PAGE_INDEX_FILES                (PAGE_INDEX_REFERENT+1)
#define PAGE_INDEX_DEFAULTFILES         (PAGE_INDEX_FILES+1)
#define PAGE_INDEX_FILE_PARAMETERS      (PAGE_INDEX_DEFAULTFILES+1)
#define PAGE_INDEX_SENDFILE             (PAGE_INDEX_FILE_PARAMETERS+1)
#define PAGE_INDEX_SENDDEFAULTFILE      (PAGE_INDEX_SENDFILE+1)
#define PAGE_INDEX_DELETEFILE           (PAGE_INDEX_SENDDEFAULTFILE+1)
#define PAGE_INDEX_PNLENTRY             (PAGE_INDEX_DELETEFILE+1)
#define PAGE_INDEX_PNL                  (PAGE_INDEX_PNLENTRY+1)
#define PAGE_INDEX_SETTINGS             (PAGE_INDEX_PNL+1)
#define PAGE_INDEX_LANGUAGES            (PAGE_INDEX_SETTINGS+1)
#define PAGE_INDEX_VERSION              (PAGE_INDEX_LANGUAGES+1)
#define PAGE_INDEX_BATTERY              (PAGE_INDEX_VERSION+1)
#define PAGE_INDEX_KNX_MASTER_SLAVE     ( PAGE_INDEX_BATTERY + 1 )
#define PAGE_INDEX_TIMEZONE             ( PAGE_INDEX_KNX_MASTER_SLAVE + 1 )
#ifndef NO_APPSWITCH
#define PAGE_INDEX_APPSWITCH            (PAGE_INDEX_TIMEZONE+1)
#endif  // #ifndef NO_APPSWITCH
#ifndef NO_APPSWITCH
#define PAGE_INDEX_TEST                 (PAGE_INDEX_APPSWITCH+1)
#else
#define PAGE_INDEX_TEST                 (PAGE_INDEX_TIMEZONE+1)
#endif  // #ifndef NO_APPSWITCH
#define PAGE_INDEX_CONTROL              (PAGE_INDEX_TEST+1)         // 2.3.0
#define PAGE_INDEX_HELP                 (PAGE_INDEX_CONTROL+1)      // 2.3.0

#define PAGE_INDEX_DALI_MASTER           (PAGE_INDEX_HELP+1)
#define PAGE_INDEX_DALI_SLAVE            (PAGE_INDEX_DALI_MASTER+1)
#define PAGE_INDEX_DALI_NOP              (PAGE_INDEX_DALI_SLAVE+1)

#define PAGE_CONTROL_GEAR               (PAGE_INDEX_DALI_NOP+1)
#define PAGE_DETECTOR                   (PAGE_CONTROL_GEAR+1)
#define PAGE_CHANGE_TYPE                (PAGE_DETECTOR+1)

#define PAGE_CREATE_ZONE                (PAGE_CHANGE_TYPE+1)
#define PAGE_VIEW_AREA                   (PAGE_CREATE_ZONE+1)
#define PAGE_CREATE_GROUP                (PAGE_VIEW_AREA+1)
#define PAGE_CHOOSE_ZONE                 (PAGE_CREATE_GROUP+1)

#define   PAGE_INDEX_OFFSET               (PAGE_CHOOSE_ZONE+1)
#define   PAGE_INDEX_Detection_LED         (PAGE_INDEX_OFFSET+1)
#define   PAGE_INDEX_SensorConfig          (PAGE_INDEX_Detection_LED+1)

#define  PAGE_INDEX_DetectorCreateGroup   (PAGE_INDEX_SensorConfig+1)
#define  PAGE_INDEX_DetectorFinishCreate   (PAGE_INDEX_DetectorCreateGroup+1)
#define  PAGE_INDEX_ADDSLAVE                (PAGE_INDEX_DetectorFinishCreate+1)


#define  PAGE_INDEX_WINDOWS               (PAGE_INDEX_ADDSLAVE+1)
#define  PAGE_INDEX_CORRIDOR              (PAGE_INDEX_WINDOWS+1)
#define  PAGE_INDEX_BOARD                  (PAGE_INDEX_CORRIDOR+1)
#define  PAGE_VIEW_AREA_CORRID            (PAGE_INDEX_BOARD+1)
#define  PAGE_CHOOSE_ZONE_CORRID           (PAGE_VIEW_AREA_CORRID+1)
#define   PAGE_INDEX_NOGROUP               PAGE_CHOOSE_ZONE_CORRID+1

#define PAGE_INDEX_DIMSLOPE             (PAGE_INDEX_NOGROUP+1)
#define PAGE_INDEX_AUXINPUT             (PAGE_INDEX_DIMSLOPE+1)
#define PAGE_INDEX_ONLEVEL              (PAGE_INDEX_AUXINPUT+1)
#define PAGE_INDEX_HOUR                 (PAGE_INDEX_ONLEVEL+1)
#define PAGE_INDEX_EXTENDMODE           (PAGE_INDEX_HOUR+1)
#define PAGE_INDEX_EXTENDMODESATE        (PAGE_INDEX_EXTENDMODE+1)
#define PAGE_INDEX_EXTENDFUNC           (PAGE_INDEX_EXTENDMODESATE+1)
#define PAGE_INDEX_EXTENDSTANDBY        (PAGE_INDEX_EXTENDFUNC+1)
#define PAGE_INDEX_EXTENDDELAY          (PAGE_INDEX_EXTENDSTANDBY+1)
#define PAGE_INDEX_EXTENDONLEVEL         (PAGE_INDEX_EXTENDDELAY+1)
#define PAGE_INDEX_OPTIME                 (PAGE_INDEX_EXTENDONLEVEL+1)
#define PAGE_INDEX_EXTENDSCHEDULED        (PAGE_INDEX_OPTIME+1)
#ifdef ROOM_CONTROL
#define PAGE_INDEX_ROOMCONTROL          (PAGE_INDEX_EXTENDSCHEDULED+1)
#endif

#else

#define PAGE_INDEX_MAIN                 0
#define PAGE_INDEX_ADRESSER             (PAGE_INDEX_MAIN+1)
#define PAGE_INDEX_TESTER               (PAGE_INDEX_ADRESSER+1)
#define PAGE_INDEX_SETTINGS             (PAGE_INDEX_TESTER+1)
#define PAGE_INDEX_VERSION              (PAGE_INDEX_SETTINGS+1)
#define PAGE_INDEX_BATTERY              (PAGE_INDEX_VERSION+1)
#ifndef NO_APPSWITCH
#define PAGE_INDEX_APPSWITCH            (PAGE_INDEX_BATTERY+1)
#endif  // #ifndef NO_APPSWITCH

#endif  // #ifndef APP_BAES

//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------

typedef void (*PAGE_HANDLER)(void);

//------------------------------------------------------------------------------
/// Exported Variables
//------------------------------------------------------------------------------

extern PAGE_HANDLER pageHandler[];

extern unsigned char pageCurrent;
extern unsigned char pagePowerOff;

//------------------------------------------------------------------------------
/// Exported Functions
//------------------------------------------------------------------------------

#ifndef APP_BAES

void MainPageHandler(void);
void ReadParametersPageHandler(void);
void TimeZonePageHandler( void );
void ListParametersPageHandler(void);
void SaveFilePageHandler(void);
void NewFilePageHandler(void);
void ReplaceFilePageHandler(void);
void ParametersPageHandler(void);
void AdvancedModePageHandler(void);
void HiddenPageHandler(void);
void DatePageHandler(void);                 // 2.4.0
void TimedelayPageHandler(void);
void SensitivityPageHandler(void);
void CalibrationPageHandler(void);
void DaylightSetpointPageHandler(void);
void ModePageHandler(void);
void DetectionschemePageHandler(void);
void AlertPageHandler(void);
void StandbylevelPageHandler(void);
void StandbydelayPageHandler(void);
void SendconditionPageHandler(void);
void DeadbandPageHandler(void);
void TimebasePageHandler(void);
void BandwidthPageHandler(void);
void SummerhourPageHandler(void);           // 2.4.0
void CountryPageHandler(void);              // 2.4.0
void LightregulationPageHandler(void);
void ProvisionoflightPageHandler(void);
void LooptypePageHandler(void);
void DaylightFactorPageHandler(void);
void DaylightFactorCenPageHandler(void);
void NaturalLightFactorPageHandler(void);
void DaylightLevelPageHandler(void);
void SensorStatusPageHandler(void);
void CurrentPOLPageHandler(void);
void DeviceIDZigbeePageHandler(void);
void OtherparametersPageHandler(void);
void BuzzerfeedbackPageHandler(void);
void LoadtypePageHandler(void);
void PowerlinefilterPageHandler(void);
void TypePageHandler(void);
void FirmwareVersionPageHandler(void);
void IDPageHandler(void);
void ReferentPageHandler(void);
void FilesPageHandler(void);
void DefaultFilesPageHandler(void);
void FileParametersPageHandler(void);
void SendFilePageHandler(void);
void SendDefaultFilePageHandler(void);
void DeleteFilePageHandler(void);
void PnLEntryPageHandler(void);
void PnLPageHandler(void);
void SettingsPageHandler(void);
void LanguagesPageHandler(void);
void VersionPageHandler(void);
void BatteryPageHandler(void);
void KNX_master_slave_PageHandler(void);
void DimmerSlopePageHandler(void);
void AuxiliaryInputPageHandler(void);
void OnLevelPageHandler(void);
void ExtendModePageHandler(void);
void ExtendModeStatePageHandler(void);
void ExtendModeFunctionPageHandler(void);
void ExtendStandbyPageHandler(void);
void ExtendStandbyDelayHandler(void);
void ExtendOnLevelPageHandler(void);
void OperationTimeOnPageHandler(void);
void HourPageHandled();
void ExtendSchedulePageHandler();

unsigned char func_pos2value(unsigned char currentItem);
#ifndef NO_APPSWITCH
void AppSwitchPageHandler(void);
#endif  // #ifndef NO_APPSWITCH
void TestPageHandler(void);
void ControlPageHandler(void);          // 2.3.0
void HelpPageHandler(void);

void DaliMasterPageHandler(void);
void DaliSlavePageHandler(void);
void DaliNopPageHandler(void);

void DaliControlGearHandler(void);    
void DaliDetectorHandler(void);       
void DaliChangeTypeHandler(void);


void DaliCreateZoneHandler(void);
void DaliViewAreaHandler(void);
void DaliCreateGroupHandler(void);
void DaliChooseZoneHandler(void);

void DALIOffSetHandler(void);
void DALIDetectionLEDHandler(void);
void DALISensorConfigHandler(void);

void DALIdetectorCreateGroupHandler(void);
void DALIDetectorFinishCreate(void);
void DALIDetectorAddSlave(void);

void DALIZoneViewWindows(void);            
void DALIZoneViewCorridor(void);            
void DALIZoneViewBoard(void); 
void DaliViewAreaCorridHandler(void);
void DaliChooseZoneCorridHandler(void);
void DALINoGroupHandler(void);
void WaringBlock(unsigned char const *string_1, unsigned char const *string_2);
unsigned char _page_read_zone_info(OPTIMES tempLanguage);
unsigned char _page_read_detector_info(void);
unsigned char _page_read_GroupNum(unsigned char DATAx,unsigned char flag,unsigned char InOrEx);
unsigned char _page_read_GroupNumAll(void);

typedef struct DATAGroup{
  unsigned char DATA2;
  unsigned char DATA3;
  unsigned char DATA4;
  unsigned char DATA5;
  unsigned char flag;
}DATAGroup;

extern DATAGroup DataGroup;
DATAGroup ConverGroup2Data(unsigned char groupNum,DATAGroup A);

#ifdef ROOM_CONTROL
void RoomControlPageHandler(void);
#endif

#else

void MainPageHandler(void);
void AdresserPageHandler(void);
void TesterPageHandler(void);
void SettingsPageHandler(void);
void VersionPageHandler(void);
void BatteryPageHandler(void);
#ifndef NO_APPSWITCH
void AppSwitchPageHandler(void);
#endif  // #ifndef NO_APPSWITCH

#endif  // #ifndef APP_BAES

#endif
