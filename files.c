
#include <stdlib.h>
#include <string.h>
#include "parameters.h"
#include "dataflash.h"
#include "files.h"

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

#define FILES_FAT_ADDRESS   (unsigned int)((unsigned int)DF_FILES_FAT_PAGE*(unsigned int)DF_PAGE_SIZE)

#define FILES_FILE_1_PAGE   1
#define FILES_FILE_2_PAGE   3
#define FILES_FILE_3_PAGE   5
#define FILES_FILE_4_PAGE   7
#define FILES_FILE_5_PAGE   9
#define FILES_FILE_6_PAGE   11
#define FILES_FILE_7_PAGE   13
#define FILES_FILE_8_PAGE   15
#define FILES_FILE_9_PAGE   17
#define FILES_FILE_10_PAGE  19
#define FILES_FILE_11_PAGE  21
#define FILES_FILE_12_PAGE  23
#define FILES_FILE_13_PAGE  28
#define FILES_FILE_14_PAGE  30
#define FILES_FILE_15_PAGE  32
#define FILES_FILE_16_PAGE  34
#define FILES_FILE_17_PAGE  36
#define FILES_FILE_18_PAGE  38
#define FILES_FILE_19_PAGE  40
#define FILES_FILE_20_PAGE  42
#define FILES_FILE_21_PAGE  44
#define FILES_FILE_22_PAGE  46
#define FILES_FILE_23_PAGE  48
#define FILES_FILE_24_PAGE  50
#define FILES_FILE_25_PAGE  52

#define FILES_BACKUP_PAGE   100

#define FILES_FILE_1_ADDRESS    (unsigned int)((unsigned int)FILES_FILE_1_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_2_ADDRESS    (unsigned int)((unsigned int)FILES_FILE_2_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_3_ADDRESS    (unsigned int)((unsigned int)FILES_FILE_3_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_4_ADDRESS    (unsigned int)((unsigned int)FILES_FILE_4_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_5_ADDRESS    (unsigned int)((unsigned int)FILES_FILE_5_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_6_ADDRESS    (unsigned int)((unsigned int)FILES_FILE_6_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_7_ADDRESS    (unsigned int)((unsigned int)FILES_FILE_7_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_8_ADDRESS    (unsigned int)((unsigned int)FILES_FILE_8_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_9_ADDRESS    (unsigned int)((unsigned int)FILES_FILE_9_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_10_ADDRESS   (unsigned int)((unsigned int)FILES_FILE_10_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_11_ADDRESS   (unsigned int)((unsigned int)FILES_FILE_11_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_12_ADDRESS   (unsigned int)((unsigned int)FILES_FILE_12_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_13_ADDRESS   (unsigned int)((unsigned int)FILES_FILE_13_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_14_ADDRESS   (unsigned int)((unsigned int)FILES_FILE_14_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_15_ADDRESS   (unsigned int)((unsigned int)FILES_FILE_15_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_16_ADDRESS   (unsigned int)((unsigned int)FILES_FILE_16_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_17_ADDRESS   (unsigned int)((unsigned int)FILES_FILE_17_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_18_ADDRESS   (unsigned int)((unsigned int)FILES_FILE_18_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_19_ADDRESS   (unsigned int)((unsigned int)FILES_FILE_19_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_20_ADDRESS   (unsigned int)((unsigned int)FILES_FILE_20_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_21_ADDRESS   (unsigned int)((unsigned int)FILES_FILE_21_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_22_ADDRESS   (unsigned int)((unsigned int)FILES_FILE_22_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_23_ADDRESS   (unsigned int)((unsigned int)FILES_FILE_23_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_24_ADDRESS   (unsigned int)((unsigned int)FILES_FILE_24_PAGE*(unsigned int)DF_PAGE_SIZE)*2
#define FILES_FILE_25_ADDRESS   (unsigned int)((unsigned int)FILES_FILE_25_PAGE*(unsigned int)DF_PAGE_SIZE)*2

#define MODIFY_DF_START        0 
#define MODIFY_DF_BACKUP_END   1
#define MODIFY_DF_MOVE_END     2



//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
///        Exported Variables
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
///        Local Functions
//------------------------------------------------------------------------------

#define FILES_SetFileNumber(pFat, number)       (pFat->Number = number)

//Index can be 1 ~ FILES_MAX_NUMBER
#define FILES_GetFilePage(pFat, Index)          (pFat->Page[Index-1])
#define FILES_SetFilePage(pFat, Index, page)    (pFat->Page[Index-1] = page)

#define FILES_GetFileHeaderAddress(pFat, Index) (((unsigned int)FILES_GetFilePage(pFat, Index)*DF_PAGE_SIZE))
#define FILES_GetFileBodyAddress(pFat, Index)   (FILES_GetFileHeaderAddress(pFat, Index)+(unsigned int)sizeof(FILES_FILEHEADER))

const unsigned char File_PageNum[25] = {  FILES_FILE_1_PAGE, FILES_FILE_2_PAGE,FILES_FILE_3_PAGE,FILES_FILE_4_PAGE,FILES_FILE_5_PAGE,
                                          FILES_FILE_6_PAGE, FILES_FILE_7_PAGE,FILES_FILE_8_PAGE,FILES_FILE_9_PAGE,FILES_FILE_10_PAGE,
                                          FILES_FILE_11_PAGE, FILES_FILE_12_PAGE,FILES_FILE_13_PAGE,FILES_FILE_14_PAGE,FILES_FILE_15_PAGE,
                                          FILES_FILE_16_PAGE, FILES_FILE_17_PAGE,FILES_FILE_18_PAGE,FILES_FILE_19_PAGE,FILES_FILE_20_PAGE,
                                          FILES_FILE_21_PAGE, FILES_FILE_22_PAGE,FILES_FILE_23_PAGE,FILES_FILE_24_PAGE,FILES_FILE_25_PAGE,
};
void  ModifyDF( FILES_FAT *pFat );
void  ModifyDF_Backup( FILES_FAT *pFat);
void  ModifyDF_Move( FILES_FAT *pFat);

unsigned char pFile[sizeof(FILES_FILEHEADER)+sizeof(SENSOR_PARAMS)];//lky 1009
//------------------------------------------------------------------------------
///        Exported Functions
//------------------------------------------------------------------------------
void FILES_Initialize(void)
{
    FILES_FAT fat;

    FILES_ReadFAT(&fat);

    switch( fat.Version ){
    case  FILES_FAT_VER_FIRST:
          ModifyDF( &fat );
          break;
    case  FILES_FAT_VER_CURRENT:
          break;
    default:
        fat.Version = FILES_FAT_VER_CURRENT;
        fat.Number = 0;   //meaning no files
        FILES_SaveFAT(&fat);
      break;
    }
}
void  ModifyDF( FILES_FAT *pFat )
{
      switch( pFat->ModifyStep ){
        case MODIFY_DF_START: 
          ModifyDF_Backup(pFat);
          pFat->ModifyStep = MODIFY_DF_BACKUP_END;
          FILES_SaveFAT(pFat);
        case MODIFY_DF_BACKUP_END:
          ModifyDF_Move(pFat);
          pFat->ModifyStep = MODIFY_DF_MOVE_END;
          FILES_SaveFAT(pFat);
        case MODIFY_DF_MOVE_END:
          pFat->Version = FILES_FAT_VER_CURRENT;
          FILES_SaveFAT(pFat);
        default:
          break;
      }
}

void  ModifyDF_Backup( FILES_FAT *pFat)
{
    unsigned char pFile[DF_PAGE_SIZE];
    unsigned char i,page;
  
    for(i=0; i< pFat->Number;i++){
      page = pFat->Page[i];            
      DF_Read(pFile, DF_PAGE_SIZE, (unsigned int)page*DF_PAGE_SIZE);
      
      page +=  (FILES_BACKUP_PAGE-1);
      pFat->Page[i] = page;
      
      DF_Erase((unsigned int)page*DF_PAGE_SIZE);
      DF_Write(pFile, (unsigned int)sizeof(pFile), (unsigned int)page*DF_PAGE_SIZE);
  
//      ptmpsensor = (SENSOR_PARAMS *)pFile;
//      ptmpsensor->GmtSign={PARAM_NUMBER_ERROR, 0},
//      ptmpsensor->GmtHour={PARAM_NUMBER_ERROR, 0};              //2.4.1
//      ptmpsensor->GmtMin={PARAM_NUMBER_ERROR, 0};              //2.4.1
      
//      pFat.Page[i] = FILES_BACKUP_PAGE+page; 
//      DF_Erase((unsigned int)page*DF_PAGE_SIZE);
//      DF_Erase((unsigned int)(page+1)*DF_PAGE_SIZE); 
    }   
                
}
                
void  ModifyDF_Move( FILES_FAT *pFat)
{
  unsigned char pFile[sizeof(FILES_FILEHEADER)+sizeof(SENSOR_PARAMS)];
  unsigned char i,page,backuppage;
//  SENSOR_PARAMS *ptmpsensor;
  unsigned int offset;
  
  page = FILES_BACKUP_PAGE;
//  ptmpsensor = (SENSOR_PARAMS *)&pFile[sizeof(FILES_FILEHEADER)];
  
  for(i=0; i< pFat->Number;i++){
      backuppage = pFat->Page[i];    
   
      DF_Read(pFile, DF_PAGE_SIZE, (unsigned int)backuppage*DF_PAGE_SIZE);
      
//      ptmpsensor->GmtSign.Number = PARAM_NUMBER_ERROR,
  //    ptmpsensor->GmtSign.Value = 0;
    //  ptmpsensor->GmtHour.Number = PARAM_NUMBER_ERROR,
      //ptmpsensor->GmtHour.Value = 0;
      //ptmpsensor->GmtMin.Number = PARAM_NUMBER_ERROR,
      //ptmpsensor->GmtMin.Value = 0;
      
      page = File_PageNum[backuppage-(FILES_BACKUP_PAGE-1)-1];
      pFat->Page[i] = page;

      DF_Erase((unsigned int)page*DF_PAGE_SIZE);
      DF_Erase((unsigned int)(page+1)*DF_PAGE_SIZE);
      
      offset = sizeof(FILES_FILEHEADER)+sizeof(SENSOR_PARAMS)-DF_PAGE_SIZE;
      DF_Write(pFile, DF_PAGE_SIZE, (unsigned int)page*DF_PAGE_SIZE);
      DF_Write(&pFile[DF_PAGE_SIZE], offset, (unsigned int)(page+1)*DF_PAGE_SIZE);
      
  }
}

unsigned char FILES_ReadFAT(FILES_FAT *pFat)
{
    DF_Read((unsigned char *)pFat, (unsigned int)sizeof(FILES_FAT), FILES_FAT_ADDRESS);

    return FILES_GetFileNumber(pFat);
}

unsigned char FILES_SaveFAT(FILES_FAT *pFat)
{
    FILES_FAT tempFAT;

    tempFAT = *pFat;

    DF_Erase(FILES_FAT_ADDRESS);
    DF_Write((unsigned char *)&tempFAT, (unsigned int)sizeof(FILES_FAT), FILES_FAT_ADDRESS);

    return 1;
}

unsigned char FILES_ReadFileName(unsigned char *pName, FILES_FAT *pFat, unsigned char Index)
{
    FILES_FILEHEADER header;

    DF_Read((unsigned char *)&header, sizeof(FILES_FILEHEADER), FILES_GetFileHeaderAddress(pFat, Index));

    strcpy((char *)pName, (char const *)(&header.pName[0]));

    return strlen((char const *)pName);
}

unsigned char FILES_ReadFileParams(SENSOR_PARAMS *pParams, FILES_FAT *pFat, unsigned char Index)
{
    unsigned char size, i, j, index_type;
    FILES_FILEHEADER header;
    PARAMETER *pTempParams;

    DF_Read((unsigned char *)&header, sizeof(FILES_FILEHEADER), FILES_GetFileHeaderAddress(pFat, Index));
    if (header.Version == FILES_FILE_VER_FIRST)
    {
        size = PARAMS_TOTAL_FIRST_VERSION;
        index_type = PARAM_INDEX_TYPE_FIRST_VERSION;
    }
    else if (header.Version == FILES_FILE_VER_SECOND)
    {
        size = PARAMS_TOTAL_SECOND_VERSION;
        index_type = PARAM_INDEX_TYPE_SECOND_VERSION;
    }
    else if (header.Version == FILES_FILE_VER_THIRD)
    {
        size = PARAMS_TOTAL_THIRD_VERSION;
        index_type = PARAM_INDEX_TYPE_THIRD_VERSION;
    }
    else if (header.Version == FILES_FILE_VER_FOURTH)
    {
        size = PARAMS_TOTAL_FOURTH_VERSION;
        index_type = PARAM_INDEX_TYPE_FOURTH_VERSION;
    }
    else if (header.Version == FILES_FILE_VER_FIFTH)
    {
        size = PARAMS_TOTAL_FIFTH_VERSION;
        index_type = PARAM_INDEX_TYPE_FIFTH_VERSION;
    }
    else if (header.Version == FILES_FILE_VER_SIXTH)
    {
        size = PARAMS_TOTAL_SIXTH_VERSION;
        index_type = PARAM_INDEX_TYPE_SIXTH_VERSION;
    }
    else if (header.Version == FILES_FILE_VER_SEVENTH)
    {
        size = PARAMS_TOTAL_SEVENTH_VERSION;
        index_type = PARAM_INDEX_TYPE_SEVENTH_VERSION;
    }

    else if (header.Version == FILES_FILE_VER_CURRENT)
    {
        size = PARAMS_TOTAL;
        index_type = PARAM_INDEX_TYPE;
    }

    if (header.Version == FILES_FILE_VER_CURRENT)
    {
        DF_Read((unsigned char *)pParams, size*sizeof(PARAMETER), FILES_GetFileBodyAddress(pFat, Index));
    }
    else
    {
        pTempParams = malloc(size*sizeof(PARAMETER));

        if (!pTempParams)
        {
            return 0;
        }

        DF_Read((unsigned char *)pTempParams, size*sizeof(PARAMETER), FILES_GetFileBodyAddress(pFat, Index));

        PARAMS_ResetToDefault(pParams, (pTempParams+index_type)->Value);
        for (i=0; i<size; i++)
        {
            for (j=0; j<PARAMS_TOTAL; j++)
            {
//              unsigned char temp;
//            temp=PARAMS_GetParameterNumber(pParams, j);
//                  
//             temp=(pTempParams+i)->Number;
//             if(i==(size-1))
//             {
//             temp=0;
//             }
                  
                if ((j != PARAM_INDEX_DETECTIONSCHEME) && (j != PARAM_INDEX_DAYLIGHTFACTORCEN) && (PARAMS_GetParameterNumber(pParams, j) == (pTempParams+i)->Number))
                {
                    PARAMS_SetParameterValue(pParams, j, (pTempParams+i)->Value);
                }
            }
        }

        free(pTempParams);
    }

    return sizeof(SENSOR_PARAMS);
}

//return 0 if full, page number otherwise
//note: according the  ascending of page number, find the empty page 
unsigned char FILES_FindEmptyPage(FILES_FAT *pFat)
{
    unsigned char i,end,index;
    

    end = FILES_GetFileNumber(pFat);

    for (i=0; i<=24; i++)
    {
        for (index=1; index<=end; index++)
        {
            if (File_PageNum[i] == FILES_GetFilePage(pFat, index)) //if this page is occupied
            {
                break;
            }
        }
        if (index > end) //meaning not found, this page is available
        {
            return File_PageNum[i];
        }
    }

    return 0; //no page available
}

//return 1 if successfully saved or replaced a file, 0 otherwise
#pragma optimize=none
unsigned char FILES_SaveFile(SENSOR_PARAMS *pParams,
                             unsigned char *pName,
                             FILES_FAT *pFat,
                             unsigned char Index)
{
   // unsigned char pFile[sizeof(FILES_FILEHEADER)+sizeof(SENSOR_PARAMS)];
    unsigned char *pPointer = &pFile[0];
    unsigned int i,offset;
    unsigned char page;
    unsigned char replace = 0;
    SENSOR_PARAMS *pTempParams;

    i = FILES_GetFileNumber(pFat);

    if (Index <= i)
    {
        replace = 1;
    }

    if (replace)
    {
        page = FILES_GetFilePage(pFat, Index);
    }
    else
    {
        page = FILES_FindEmptyPage(pFat);
    }

    if (page == 0) //no empty page available, save failed
    {
        return 0;
    }

    *(pPointer++) = FILES_FILE_VER_CURRENT;
    for (i=0; i<FILES_MAX_NAME; i++)
    {
        if (*pName != 0)
        {
            *(pPointer++) = *(pName++);
        }
        else
        {
            break;
        }
    }
    *pPointer = 0;

    pPointer = &pFile[FILES_FILE_FIRST_PARAM_OFFSET];

    pTempParams = (SENSOR_PARAMS *)malloc(sizeof(SENSOR_PARAMS));
    *pTempParams = *pParams;
    PARAMS_SetParameterNumber(pTempParams, PARAM_INDEX_PROVISIONOFLIGHT, PARAM_NUMBER_ERROR);       // Do not save Provision of light
    PARAMS_SetParameterNumber(pTempParams, PARAM_INDEX_DAYLIGHTFACTOR, PARAM_NUMBER_ERROR);       // Do not save Daylight factor
    PARAMS_SetParameterNumber(pTempParams, PARAM_INDEX_DAYLIGHTFACTORCEN, PARAM_NUMBER_ERROR);       // Do not save Daylight factor (cen.)
    PARAMS_SetParameterNumber(pTempParams, PARAM_INDEX_NATURALLIGHTFACTOR, PARAM_NUMBER_ERROR);       // Do not save Natural light factor
//    PARAMS_SetParameterNumber(pTempParams, PARAM_INDEX_DEVICEIDZIGBEE, PARAM_NUMBER_ERROR);       // Do not save DeviceID Zigbee

    for (i=0; i<sizeof(SENSOR_PARAMS); i++)
    {
        *(pPointer++) = *((unsigned char *)pTempParams+i);
    }

    DF_Erase((unsigned int)page*DF_PAGE_SIZE);
    DF_Erase((unsigned int)(page+1)*DF_PAGE_SIZE);
    
    offset = (unsigned int)sizeof(pFile) - DF_PAGE_SIZE;
    DF_Write(pFile, DF_PAGE_SIZE , (unsigned int)page*DF_PAGE_SIZE);
    DF_Write(&pFile[DF_PAGE_SIZE], offset , (unsigned int)(page+1)*DF_PAGE_SIZE);
        
    if (replace == 0)
    {
        //update the FAT if saving new file
        i = FILES_GetFileNumber(pFat)+1;
        FILES_SetFileNumber(pFat, i);
        FILES_SetFilePage(pFat, Index, page);
        FILES_SaveFAT(pFat);
    }

    free(pTempParams);

    return 1;
}

unsigned char FILES_DeleteFile(FILES_FAT *pFat, unsigned char Index)
{
    unsigned char number;
    unsigned char *pTemp;

    number = FILES_GetFileNumber(pFat);

    if ((Index < 1) || (Index > number))
    {
        return 0;
    }

    DF_Erase(FILES_GetFileHeaderAddress(pFat, Index));
    DF_Erase(FILES_GetFileHeaderAddress(pFat, Index)+DF_PAGE_SIZE);

    pTemp = &(pFat->Page[Index-1]);

    for (; Index<number; Index++)
    {
        *pTemp = *(pTemp+1);
        pTemp++;
    }

    *pTemp = 0xFF;

    FILES_SetFileNumber(pFat, number-1);

    FILES_SaveFAT(pFat);

    return 1;
}

//unsigned char FILES_RenameFile()


