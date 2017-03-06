#ifndef LANGUAGES_H
#define LANGUAGES_H

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

#define LANGUAGES_ADDRESS   (unsigned int)((unsigned int)DF_LANGUAGES_PAGE*(unsigned int)DF_PAGE_SIZE)
#define LANGUAGES_TOTAL     5

//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------

typedef enum {
    French  = 0,
    English,
    Italian,
    Spanish,
    German,         // 2.4.0
    Unknown = 0xFF
} LANGUAGES;

//------------------------------------------------------------------------------
/// Exported Variables
//------------------------------------------------------------------------------

extern LANGUAGES language;
extern unsigned char *strings_language[LANGUAGES_TOTAL];

typedef enum{
  FirstTime,
  SecondTime  
}OPTIMES;

extern OPTIMES optimes;
//------------------------------------------------------------------------------
/// Exported Functions
//------------------------------------------------------------------------------

void LANGUAGES_Initialize(void);

#endif
