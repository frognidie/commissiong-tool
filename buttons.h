#ifndef BUTTONS_H
#define BUTTONS_H

#include <board.h>

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

#define BUTTON_INDEX_NONE       0xFF
#define BUTTON_INDEX_FIRST      0
#define BUTTON_INDEX_MAJOR      0
#define BUTTON_INDEX_UP         1
#define BUTTON_INDEX_LEFT       2
#define BUTTON_INDEX_DOWN       3
#define BUTTON_INDEX_RIGHT      4
#define BUTTON_INDEX_F1         5
#define BUTTON_INDEX_F2         6
#define BUTTON_INDEX_F3         7
#define BUTTON_INDEX_F4         8
#define BUTTON_INDEX_LAST       8

#define BUTTON_TOTAL            9

#define BUTTON_MESSAGE_NONE     0
#define BUTTON_MESSAGE_PUSHING  1
#define BUTTON_MESSAGE_PUSHED   2
#define BUTTON_MESSAGE_PRESSED  3
#define BUTTON_MESSAGE_RELEASED 4

//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------

typedef struct {
    unsigned char message;
    unsigned char index;
    unsigned int time;
} BUTTON_MESSAGE;

//------------------------------------------------------------------------------
/// Exported Variables
//------------------------------------------------------------------------------

extern BUTTON_MESSAGE buttonMessage;

//------------------------------------------------------------------------------
/// Exported Functions
//------------------------------------------------------------------------------

void BUTTON_Configure(void);

void BUTTON_Check(void);

void BUTTON_Acknowledge(void);

void BUTTON_TimerHit(void);

#define BUTTON_GetMessage()         buttonMessage.message

#define BUTTON_GetButtonIndex()     buttonMessage.index

#define BUTTON_GetPressedTime()     buttonMessage.time

#endif
