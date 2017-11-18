/* pb_common.c: Common support functions for pb_encode.c and pb_decode.c.
 *
 * 2014 Petteri Aimonen <jpa@kapsi.fi>
 */

#include "protob/MsgUtil.h"
#include <stdint.h>

#define BASE 65521

void int2Bytes(int num, char *intBytes) {
	int i;
	for (i = 0; i < 4; i++) {
		intBytes[i] = (num >> (8 * (4 - i - 1))) & 0xff;
	}
}

 void long2Bytes(uint64_t num, char *longBytes) {
	int i;
	for (i = 0; i < 8; i++) {
		longBytes[i] = (num >> (8 * (8 - i - 1))) & 0xff;
                //printf("longbyte:%d\n",longBytes[i]);
	}
}
/*
unsigned long getAdler32(unsigned char *buf, int len) {

	unsigned long adler = 1;
	unsigned long s1 = adler & 0xffff;
	unsigned long s2 = (adler >> 16) & 0xffff;

	int i;
	for (i = 0; i < len; i++) {
		unsigned char temp = (unsigned char) buf[i];
		s1 = (s1 + temp) % BASE;
		s2 = (s2 + s1) % BASE;
	}
	return (s2 << 16) + s1;
}*/
uint64_t getAdler32(unsigned char *buf, int len) {

        uint64_t adler = 1;
        uint64_t s1 = adler & 0xffff;
        uint64_t s2 = (adler >> 16) & 0xffff;

        int i;
        for (i = 0; i < len; i++) {
        //      unsigned char temp = (unsigned char) buf[i];
                s1 = (s1 + buf[i]) % BASE;
                s2 = (s2 + s1) % BASE;
        }
        return (s2 << 16) + s1;
}


/*
char *random_uuid(char *buf) {
	const char *c = "89ab";
	char *p = buf;
	int n;

	for (n = 0; n < 16; ++n) {
		int b = rand() % 255;

		switch (n) {
		case 6:
			sprintf(p, "4%x", b % 15);
			break;
		case 8:
			sprintf(p, "%c%x", c[rand() % strlen(c)], b % 15);
			break;
		default:
			sprintf(p, "%02x", b);
			break;
		}

		p += 2;

		switch (n) {
		case 3:
		case 5:
		case 7:
		case 9:
			*p++ = '-';
			break;
		}
	}

	*p = 0;

	return buf;
}*/

 int recoverNetworkBytesOrderInt(const char* orderBytes, int begindex) {
	int sourceNumber = 0;
	int i;
	for (i = begindex; i < begindex + 4; i++) {
		sourceNumber <<= 8;
		sourceNumber |= (orderBytes[i] & 0xff);
	}
	return sourceNumber;
}
short int recoverNetworkBytesOrderShort(const char* orderBytes, int begindex) {
	short int sourceNumber = 0;
	int i;
	for (i = begindex; i < begindex + 2; i++) {
		sourceNumber <<= 8;
		sourceNumber |= (orderBytes[i] & 0xff);
	}
	return sourceNumber;
}
 

 uint64_t recoverNetworkBytesOrderLong(const char* orderBytes,
		int begindex) {
	uint64_t sourceNumber = 0;
	int i;
	for (i = begindex; i < begindex + 8; i++) {
		sourceNumber <<= 8;
		sourceNumber |= orderBytes[i] & 0xff;
              // printf("sourceNumber:%d\n",sourceNumber);
	}
	return sourceNumber;
}
