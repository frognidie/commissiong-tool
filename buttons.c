
#include <board.h>
#include <pio\pio.h>
//#include "main.h"
#include "time.h"
#include "lcdd.h"
//#include "pages.h"
#include "buttons.h"

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

#define BUTTON_DEBOUNCE_TIME    5
#define BUTTON_PRESS_TIME       500

#define BUTTON_BKLITOFF_TIME    30000
#define BUTTON_POWEROFF_TIME    300000

//------------------------------------------------------------------------------
//         Global variables
//------------------------------------------------------------------------------

__no_init BUTTON_MESSAGE buttonMessage;
extern const Pin pinVbus;
extern const Pin pinVinDis;
extern const Pin pinsLcdBKLIT;

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

static const Pin pinsButtons[] = {PINS_PUSHBUTTONS};
static __no_init unsigned int buttonTimeStamp;

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------

void BUTTON_Configure(void)
{
    PIO_Configure(pinsButtons, PIO_LISTSIZE(pinsButtons));

    buttonMessage.message = BUTTON_MESSAGE_NONE;
    buttonMessage.index = BUTTON_INDEX_NONE;
    buttonMessage.time = 0;

    buttonTimeStamp = TIME_GetTime();
}

void BUTTON_Check(void)
{
    static unsigned char time = 0;
    static unsigned char ignore = 0;
    unsigned char poweroff = 0;

    if (buttonMessage.index == BUTTON_INDEX_NONE)
    {
        for (unsigned char i=BUTTON_INDEX_FIRST; i<=BUTTON_INDEX_LAST; i++)
        {
            if (PIO_Get(&pinsButtons[i]) == 0)
            {
                buttonMessage.index = i;
                break;
            }
        }
    }
    else
    {
        if (PIO_Get(&pinsButtons[buttonMessage.index]) == 0)
        {
            if (time < BUTTON_DEBOUNCE_TIME)
            {
                time++;
            }
        }
        else
        {
            if (time > 0)
            {
                time--;
            }
        }

        if (time == BUTTON_DEBOUNCE_TIME)
        {
            if ((ignore == 0) && (PIO_Get(&pinsLcdBKLIT)))  // If the backlight was OFF, just turn on the backlight, ignore button messages
            {
                ignore = 1;
            }

            PIO_Clear(&pinsLcdBKLIT);
            buttonTimeStamp = TIME_GetTime();

            if (!ignore)
            {
                buttonMessage.time++;

                if (buttonMessage.time > BUTTON_PRESS_TIME)
                {
                    buttonMessage.message = BUTTON_MESSAGE_PRESSED;
                }
                else
                {
                    buttonMessage.message = BUTTON_MESSAGE_PUSHING;
                }
            }
        }
        else if (time == 0)
        {
            ignore = 0;

            if (buttonMessage.time >= BUTTON_PRESS_TIME)
            {
                buttonMessage.message = BUTTON_MESSAGE_RELEASED;
            }
            else if (buttonMessage.time > 0)
            {
                buttonMessage.message = BUTTON_MESSAGE_PUSHED;
            }
            else
            {
                BUTTON_Acknowledge();
            }
        }
    }

    if (buttonMessage.message == BUTTON_MESSAGE_NONE)
    {
        if (PIO_Get(&pinVbus) == 1)
        {
            if (TIME_GetTime() >= buttonTimeStamp + BUTTON_POWEROFF_TIME)
            {
                poweroff = 1;
            }
            else if (TIME_GetTime() >= buttonTimeStamp + BUTTON_BKLITOFF_TIME)
            {
                PIO_Set(&pinsLcdBKLIT);
            }
        }
        else
        {
            buttonTimeStamp = TIME_GetTime();
        }
    }

    if ((PIO_Get(&pinVbus) == 1) && (buttonMessage.message == BUTTON_MESSAGE_PRESSED) && (buttonMessage.index == BUTTON_INDEX_MAJOR) && (buttonMessage.time >= 1000))
    {
       poweroff = 1;
    }

    /*
    if (PIO_Get(&pinVbus) == 1)
    {
        if (buttonMessage.message == BUTTON_MESSAGE_NONE)
        {
            if (TIME_GetTime() >= buttonTimeStamp + BUTTON_POWEROFF_TIME)
            {
                poweroff = 1;
            }
            else if (TIME_GetTime() >= buttonTimeStamp + BUTTON_BKLITOFF_TIME)
            {
                PIO_Set(&pinsLcdBKLIT);
            }
        }
        else
        {
            buttonTimeStamp = TIME_GetTime();
            PIO_Clear(&pinsLcdBKLIT);

            if ((buttonMessage.message == BUTTON_MESSAGE_PRESSED) && (buttonMessage.index == BUTTON_INDEX_MAJOR) && (buttonMessage.time >= 1000) && (pageCurrent != PAGE_INDEX_PNL))
            {
                poweroff = 1;
            }
        }
    }
    */

    if (poweroff)
    {
        LCDD_Reset();
        TIME_Wait(10);
        PIO_Configure(&pinVinDis, 1);
        while(1);
    }
}

void BUTTON_Acknowledge(void)
{
    buttonMessage.message = BUTTON_MESSAGE_NONE;
    buttonMessage.index = BUTTON_INDEX_NONE;
    buttonMessage.time = 0;
}

void BUTTON_TimerHit(void)
{
    buttonTimeStamp = TIME_GetTime();
}
