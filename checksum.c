
#include "checksum.h"

extern unsigned short const __checksum;

unsigned short
CHECKSUM_crc16(unsigned short sum, unsigned char *p, unsigned int len)
{
    while (len--)
    {
        int i;
        unsigned char byte = *p++;

        for (i = 0; i < 8; ++i)
        {
            unsigned long osum = sum;
            sum <<= 1;
            if (byte & 0x80)
              sum |= 1 ;
            if (osum & 0x8000)
              sum ^= 0x1021;
            byte <<= 1;
        }
    }
    return sum;
}

// dummy function
unsigned char CHECKSUM_Verify(void)
{
    volatile unsigned short sum = __checksum;

//    sum = CHECKSUM_crc16(sum,(unsigned char *)0x100000, (unsigned int)0x10FFFD -(unsigned int)0x100000 +1);

    if (sum != __checksum)
    {
        return 0;
    }

    return 1;
}
