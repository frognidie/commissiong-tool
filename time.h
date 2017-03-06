#ifndef TIME_H
#define TIME_H

#include <pit\pit.h>

//------------------------------------------------------------------------------
/// Exported Functions
//------------------------------------------------------------------------------

void TIME_Configure(void);

void TIME_Wait(unsigned int delay);

unsigned int TIME_GetTime(void);

#endif
