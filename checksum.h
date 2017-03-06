#ifndef CHECKSUM_H
#define CHECKSUM_H

unsigned short
CHECKSUM_crc16(unsigned short sum, unsigned char *p, unsigned int len);
unsigned char CHECKSUM_Verify(void);

#endif
