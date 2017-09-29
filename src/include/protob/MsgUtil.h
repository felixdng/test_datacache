#ifndef _MSGUTIL_H_
#define _MSGUTIL_H_
#include <stdio.h>
#include <stdint.h>


void int2Bytes(int num, char *intBytes) ;

void long2Bytes(uint64_t num, char *longBytes) ;

uint64_t getAdler32(unsigned char *buf, int len) ;

char *random_uuid(char *buf) ;

int recoverNetworkBytesOrderInt(const char* orderBytes, int begindex) ;
short int recoverNetworkBytesOrderShort(const char* orderBytes, int begindex);
uint64_t recoverNetworkBytesOrderLong(const char* orderBytes,int begindex); 
#endif
