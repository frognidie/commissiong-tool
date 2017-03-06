
#include <board.h>
#include <aic\aic.h>
//#include <pit\pit.h>
#include "time.h"

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

#define PIT_PERIOD 1000

//------------------------------------------------------------------------------
///         Local variables
//------------------------------------------------------------------------------

static volatile unsigned int time = 0;

//------------------------------------------------------------------------------
///         Exported Functions
//------------------------------------------------------------------------------

void ISR_Pit(void)
{
    unsigned int status;

    // Read the PIT status register
    status = PIT_GetStatus() & AT91C_PITC_PITS;
    if (status != 0) {

        // Read the PIVR to acknowledge interrupt and get number of ticks
        time += (PIT_GetPIVR() >> 20);
    }
}

void TIME_Configure(void)
{
    // Initialize the PIT to the desired frequency
    PIT_Init(PIT_PERIOD, BOARD_MCK / 1000000);
//    AT91C_BASE_PITC->PITC_PIMR = (unsigned int)1152;
//    AT91C_BASE_PITC->PITC_PIMR |= AT91C_PITC_PITEN;

    // Configure interrupt on PIT
    AIC_DisableIT(AT91C_ID_SYS);
    AIC_ConfigureIT(AT91C_ID_SYS, AT91C_AIC_PRIOR_LOWEST, ISR_Pit);
    AIC_EnableIT(AT91C_ID_SYS);
    PIT_EnableIT();

    // Enable the pit
//    PIT_GetPIVR(); //clear PIT_PIVR as a side effect
    PIT_Enable();
}

void TIME_Wait(unsigned int delay)
{
    volatile unsigned int start = time;
    unsigned int elapsed;
    do {
        elapsed = time;
        elapsed -= start;
    }
    while (elapsed < delay);
}

/*
void TIME_Wait(unsigned int delay)
{
    unsigned int target = (unsigned int)(PIT_GetPIIR() + (delay << 20)) >> 20;

    while (((unsigned int)PIT_GetPIIR() >> 20) != target);
}*/

unsigned int TIME_GetTime(void)
{
    return time;
}
