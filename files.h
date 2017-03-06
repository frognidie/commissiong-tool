#ifndef FILES_H
#define FILES_H

#include "parameters.h"
#include "dataflash.h"

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

#define FILES_MAX_NUMBER            25

#define FILES_MAX_NAME              15

#define FILES_FAT_VERSION_OFFSET        0
#define FILES_FAT_NUMBER_OFFSET         1
#define FILES_FAT_FIRST_ADDRESS_OFFSET  2

#define FILES_FILE_VERSION_OFFSET       0
#define FILES_FILE_NAME_OFFSET          (FILES_FILE_VERSION_OFFSET+1)
#define FILES_FILE_FIRST_PARAM_OFFSET   (FILES_FILE_NAME_OFFSET+FILES_MAX_NAME+1)

#define FILES_FAT_VER_REVISION      1
#define FILES_FAT_VER_FIRST         0x10
#define FILES_FAT_VER_SECOND        (FILES_FAT_VER_FIRST+FILES_FAT_VER_REVISION)
#define FILES_FAT_VER_CURRENT       FILES_FAT_VER_SECOND

// File version update notes:
// 0x10: First version
// 0x11: Revision 1, add "Natural light factor" to Sensor parameters
// 0x12: Revision 2, add "Detection scheme" and "Daylight factor (CEN)" for centralized system
// 0x13: Revision 3, see "ADDON FOR LM058a V1.1.doc"
// 0x14: Revision 4, see "ADDON FOR LM058a V2.1.doc"
// 0x15: Revision 5, see "ADDON II FOR LM058a V2.4.doc"
// 0x16: Revision 6, see "ADDON III FOR LM058a V2.8.doc"
// 0x17: Revision 7, see "ADDON III FOR LM058a V2.10.doc"

#define FILES_FILE_VER_REVISION     1
#define FILES_FILE_VER_FIRST        0x10
#define FILES_FILE_VER_SECOND       (FILES_FILE_VER_FIRST+FILES_FILE_VER_REVISION)
#define FILES_FILE_VER_THIRD        (FILES_FILE_VER_SECOND+FILES_FILE_VER_REVISION)
#define FILES_FILE_VER_FOURTH       (FILES_FILE_VER_THIRD+FILES_FILE_VER_REVISION)
#define FILES_FILE_VER_FIFTH        (FILES_FILE_VER_FOURTH+FILES_FILE_VER_REVISION)
#define FILES_FILE_VER_SIXTH        (FILES_FILE_VER_FIFTH+FILES_FILE_VER_REVISION)
#define FILES_FILE_VER_SEVENTH      (FILES_FILE_VER_SIXTH+FILES_FILE_VER_REVISION)      // 2.4.0
#define FILES_FILE_VER_EIGHTH       (FILES_FILE_VER_SEVENTH+FILES_FILE_VER_REVISION)      // 2.4.1

#define FILES_FILE_VER_CURRENT      FILES_FILE_VER_EIGHTH      // 2.4.1


//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------

typedef struct {
    unsigned char Version;
    unsigned char Number;
    unsigned char Page[FILES_MAX_NUMBER];
    unsigned char ModifyStep;
} FILES_FAT;

typedef struct {
    unsigned char Version;
    unsigned char pName[FILES_MAX_NAME+1];
} FILES_FILEHEADER;

//------------------------------------------------------------------------------
///        Exported Variables
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
///        Exported Functions
//------------------------------------------------------------------------------

void FILES_Initialize(void);

unsigned char FILES_ReadFAT(FILES_FAT *pFat);
unsigned char FILES_SaveFAT(FILES_FAT *pFat);
unsigned char FILES_ReadFileName(unsigned char *pName, FILES_FAT *pFat, unsigned char Index);
unsigned char FILES_ReadFileParams(SENSOR_PARAMS *pParams, FILES_FAT *pFat, unsigned char Index);

//return 0 if full, page number otherwise
unsigned char FILES_FindEmptyPage(FILES_FAT *pFat);

//return 1 if successfully saved or replaced a file, 0 otherwise
unsigned char FILES_SaveFile(SENSOR_PARAMS *pParams,
                             unsigned char *pName,
                             FILES_FAT *pFat,
                             unsigned char Index);

unsigned char FILES_DeleteFile(FILES_FAT *pFat, unsigned char Index);

#define FILES_GetFileNumber(pFat)               ((pFat)->Number)



#endif
