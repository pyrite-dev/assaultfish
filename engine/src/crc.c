/* crc32.c -- compute the CRC-32 of a data stream
 * Copyright (C) 1995-1998 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include <GearSrc/CRC.h>

GSU32 GSCRC32(void* input, unsigned int len) {
	int	       i, j;
	GSU32	       byte, crc, mask;
	unsigned char* message = input;

	i   = 0;
	crc = 0xFFFFFFFF;
	while(i < len) {
		byte = message[i]; // Get next byte.
		crc  = crc ^ byte;
		for(j = 7; j >= 0; j--) { // Do eight times.
			mask = -(crc & 1);
			crc  = (crc >> 1) ^ (0xEDB88320 & mask);
		}
		i = i + 1;
	}
	return ~crc;
}
