/* pb_encode.c -- encode a protobuf using minimal resources
 *
 * 2011 Petteri Aimonen <jpa@kapsi.fi>
 */


#include "Encoder.h"
#include "Decoder.h"
#include <stdint.h>
static const int UUID_LENGTH = 36;
static const int TYPE = 4;
static const int LENGTH = 4;
static const int SN = 4;
static const int ADLER32 = 8;
static const int maxLineLength = 20000;
char uid[37]="57d99d89-caab-482a-a0e9-a0a803eed3ba";
char * encode(int msgType, int sn, unsigned char* data, int length,
		char* destData) {
        int i;
/*        char uuidBytes[37];
	int i;
	random_uuid(uuidBytes);
	printf("uuidbytes: ");
	
	for (i = 0; i < UUID_LENGTH + TYPE + SN + LENGTH + length + ADLER32; ++i) {
		printf(" %d, ", uuidBytes[i]);
	}
	printf("\n");*/
	char typeBytes[TYPE];
	int2Bytes(msgType, typeBytes);
	char snBytes[SN];
	int2Bytes(sn, snBytes);
	char lengthBytes[LENGTH];
	int2Bytes(length, lengthBytes);
	char adlBytes[ADLER32];
	long2Bytes(getAdler32(data, length), adlBytes);

	// copy
	memcpy(destData, uid, UUID_LENGTH);
	memcpy(destData + UUID_LENGTH, typeBytes, TYPE);
	memcpy(destData + UUID_LENGTH + TYPE, snBytes, SN);
	memcpy(destData + UUID_LENGTH + TYPE + SN, lengthBytes, LENGTH);
	memcpy(destData + UUID_LENGTH + TYPE + SN + LENGTH, data, length);
	memcpy(destData + UUID_LENGTH + TYPE + SN + LENGTH + length, adlBytes,
			ADLER32);
	memcpy(destData + UUID_LENGTH + TYPE + SN + LENGTH + length + ADLER32,
			typeBytes, TYPE);
	printf("encode data: ");

	for (i = 0; i < UUID_LENGTH + TYPE + SN + LENGTH + length + ADLER32; ++i) {
		printf(" %d, ", destData[i]);
	}
    
	printf("\n");
	return destData; 
}

