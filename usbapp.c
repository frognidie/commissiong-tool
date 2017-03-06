
#include <board.h>
#include <string.h>
#include <usb/device/cdc-serial/CDCDSerialDriver.h>
#include <usb/device/cdc-serial/CDCDSerialDriverDescriptors.h>
#include "languages.h"
#include "dataflash.h"
//#include "nitoo.h"        // Commented out @ 1.0.24
#include "checksum.h"
#include "usbapp.h"

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

#define XON     0x11
#define XOFF    0x13

/// Size in bytes of the buffer used for reading data from the USB
#define DATABUFFERSIZE \
    BOARD_USB_ENDPOINTS_MAXPACKETSIZE(CDCDSerialDriverDescriptors_DATAIN)

#define USBAPP_IDLE         0
#define USBAPP_UPGRADING    1

#define USBAPP_FIRMWARE_LENGTH  (unsigned int)0x60000       // 1.0.24, originally was 0xF000
//------------------------------------------------------------------------------
///         Local variables
//------------------------------------------------------------------------------

static unsigned char usbappBuffer[DATABUFFERSIZE];
static unsigned char usbappUpgradeBuffer[DF_PAGE_SIZE];
static unsigned char usbappStage = USBAPP_IDLE;
static __no_init unsigned int usbappDataLength;
static const unsigned char usbappStringUpgradeLM[] = "UPGRADE_LM";
static const unsigned char usbappStringUpgradeEL[] = "UPGRADE_EL";
static __no_init unsigned int usbappStartPage;
static __no_init unsigned int usbappStatusPage;

//------------------------------------------------------------------------------
///        Internal Functions
//------------------------------------------------------------------------------

void _usbDataReceived(unsigned int unused,
                         unsigned char status,
                         unsigned int received,
                         unsigned int remaining)
{
    static unsigned char cyclic;
    unsigned char i;

    if (status == USBD_STATUS_SUCCESS) {
        switch (usbappStage)
        {
        case USBAPP_IDLE:
            if ((received == sizeof(usbappStringUpgradeLM)) \
                  && (!strcmp((char const *)usbappBuffer, (char const *)usbappStringUpgradeLM)))
            {
                usbappBuffer[0] = XON;
                CDCDSerialDriver_Write((void *)usbappBuffer, 1, 0, 0);
                usbappStartPage = DF_UPGRADE_LM_START_PAGE;
                usbappStatusPage = DF_UPGRADE_EL_STATUS_PAGE;
                cyclic = 0;
                usbappDataLength = 0;
                usbappStage = USBAPP_UPGRADING;
            }
            else if ((received == sizeof(usbappStringUpgradeEL)) \
                  && (!strcmp((char const *)usbappBuffer, (char const *)usbappStringUpgradeEL)))
            {
                usbappBuffer[0] = XON;
                CDCDSerialDriver_Write((void *)usbappBuffer, 1, 0, 0);
                usbappStartPage = DF_UPGRADE_EL_START_PAGE;
                usbappStatusPage = DF_UPGRADE_EL_STATUS_PAGE;
                cyclic = 0;
                usbappDataLength = 0;
                usbappStage = USBAPP_UPGRADING;
            }
            break;
        case USBAPP_UPGRADING:
            i = 0;
//            if (received > 0)
//            {
                while ((i < received) && (usbappDataLength < USBAPP_FIRMWARE_LENGTH))
                {
                    usbappDataLength++;
                    usbappUpgradeBuffer[cyclic++] = usbappBuffer[i++];
                    if (cyclic == 0)
                    {
                        DF_Write(usbappUpgradeBuffer, sizeof(usbappUpgradeBuffer), usbappStartPage*DF_PAGE_SIZE+usbappDataLength-sizeof(usbappUpgradeBuffer));
                    }
                };
//            }
            break;
        default:
            break;
        }
        // Start receiving data on the USB
        CDCDSerialDriver_Read(usbappBuffer,
                                DATABUFFERSIZE,
                                (TransferCallback) _usbDataReceived,
                                0);
    }
}

//------------------------------------------------------------------------------
///         Exported Functions
//------------------------------------------------------------------------------

void USBAPP_Initialize(void)
{
    // Start receiving data on the USB
    CDCDSerialDriver_Read(usbappBuffer,
                          DATABUFFERSIZE,
                          (TransferCallback) _usbDataReceived,
                          0);
}

void USBAPP_Handler(void)
{
    static unsigned int timeguard = 0;
    static unsigned int datalength = 0;
    unsigned char TempData;
    unsigned short checksum;
    unsigned short sum;
#if 0
    static unsigned char test[32];
#endif

    if (usbappStage == USBAPP_UPGRADING)
    {
        if (datalength == usbappDataLength)
        {
            if (++timeguard == 0x10000)     // CAUTION
            {
                timeguard = 0;
                datalength = 0;
                if (usbappDataLength == USBAPP_FIRMWARE_LENGTH)
                {
                    //
                    sum = 0;
                    while (usbappDataLength > 2)
                    {
                        AT91C_BASE_WDTC->WDTC_WDCR = (0xA5 << 24) | AT91C_WDTC_WDRSTT;  // 2.1

                        DF_Read(&TempData, 1, usbappStartPage*DF_PAGE_SIZE+USBAPP_FIRMWARE_LENGTH-usbappDataLength);
                        sum = CHECKSUM_crc16(sum, &TempData, 1);
                        usbappDataLength--;
                    }
                    DF_Read((unsigned char *)&checksum, 2, usbappStartPage*DF_PAGE_SIZE+USBAPP_FIRMWARE_LENGTH-2);
                    DF_Erase(usbappStatusPage*DF_PAGE_SIZE);
                    DF_Erase(DF_UPGRADE_FLAG_PAGE*DF_PAGE_SIZE);
                    if (sum == checksum)
                    {
                        DF_Write("Good", sizeof("Good"), usbappStatusPage*DF_PAGE_SIZE);
                        if (usbappStartPage == DF_UPGRADE_LM_START_PAGE)
                        {
                            DF_Write("UPGRADE_LM", sizeof("UPGRADE_LM"), DF_UPGRADE_FLAG_PAGE*DF_PAGE_SIZE);
                            DF_Erase(LANGUAGES_ADDRESS);    // Force language selection page first time after upgrading
                        }
                        else if (usbappStartPage == DF_UPGRADE_EL_START_PAGE)
                        {
                            DF_Write("UPGRADE_EL", sizeof("UPGRADE_EL"), DF_UPGRADE_FLAG_PAGE*DF_PAGE_SIZE);
                            //DF_Erase(LANGUAGES_ADDRESS);    // Force language selection page first time after upgrading
                        }
                        usbappBuffer[0] = XOFF;
                        CDCDSerialDriver_Write((void *)usbappBuffer, 1, 0, 0);
                    }
#if 0
                    DF_Read(test, 32, DF_UPGRADE_FLAG_PAGE*DF_PAGE_SIZE);
                    while (1);
#endif
                }
                usbappStage = USBAPP_IDLE;
            }
        }
        else
        {
            datalength = usbappDataLength;
            timeguard = 0;
        }
    }
}






