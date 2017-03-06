
#include "utilities.h"

//return the number of characters (without the '\0'!!!!!)
unsigned char dectoascii(unsigned char *pString, unsigned int number, unsigned char characters)
{
    unsigned char bcd[7] = {'0', '0', '0', '0', '0', '0', 0};
    unsigned char result = 0;
    unsigned char *pbcd = &bcd[0];

    if (pString == 0)
    {
        return 0;
    }

    if (number == 0)
    {
        if (characters == 0)
        {
            characters = 1;
        }
    }

    while (number >= 100000)
    {
        number -= 100000;
        bcd[0] += 1;
    }

    while (number >= 10000)
    {
        number -= 10000;
        bcd[1] += 1;
    }

    while (number >= 1000)
    {
        number -= 1000;
        bcd[2] += 1;
    }

    while (number >= 100)
    {
        number -= 100;
        bcd[3] += 1;
    }

    while (number >= 10)
    {
        number -= 10;
        bcd[4] += 1;
    }

    bcd[5] += number;

    if (characters)
    {
        pbcd += 6 - characters;
    }
    else
    {
        while(*pbcd == '0')
        {
            pbcd++;
        }
    }

    while (*pbcd)
    {
        result++;
        *(pString++) = *(pbcd++);
    }

    *pString = 0;

    return result;
}

void hextoascii(unsigned char *pString, unsigned long long number, unsigned char characters)      // 2.3.2, type of number was unsigned int
{
    unsigned char temp;

    *(pString++) = '0';
    *(pString++) = 'x';

    for (unsigned char i=characters; i>0; i--)
    {
        temp = ((number >> 4*(i-1)) & 0xF);

        *pString = temp+0x30;

        if (temp > 9)   //A~F
        {
            *pString += 7;
        }

        pString++;
    }

    *pString = 0;
}

//return the number of characters (without the '\0'!!!!!)
unsigned char bcdtoascii(unsigned char *pString, unsigned char number, unsigned char zero_expand)
{
    unsigned char temp, result;

    result = 1;
    temp = (number >> 4) & 0xF;

    if (temp || zero_expand)
    {
        *(pString++) = temp+0x30;
        result = 2;
    }

    *(pString++) = (number & 0xF)+0x30;

    *pString = 0;

    return result;
}

// 2.4.0
// get the last day of specific year/month, return 0 if the input is out of range
unsigned short getlastday(unsigned short year, unsigned short month)
{
    unsigned short result = 0;

    switch (month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        result = 31;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        result = 30;
        break;
    case 2:
        result = 28;
        if ((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0)))
        {
            result = 29;
        }
    default:
        break;
    }

    return result;
}
