#ifndef UTILITIES_H
#define UTILITIES_H

//------------------------------------------------------------------------------
/// Exported Variables
//------------------------------------------------------------------------------

//return the number of characters (without the '\0'!!!!!)
unsigned char dectoascii(unsigned char *pString, unsigned int number, unsigned char characters);

void hextoascii(unsigned char *pString, unsigned long long number, unsigned char characters);     // 2.3.2, type of number was unsigned int

//return the number of characters (without the '\0'!!!!!)
unsigned char bcdtoascii(unsigned char *pString, unsigned char number, unsigned char zero_expand);

// 2.4.0
// get the last day of specific year/month, return 0 if the input is out of range
unsigned short getlastday(unsigned short year, unsigned short month);

#endif
