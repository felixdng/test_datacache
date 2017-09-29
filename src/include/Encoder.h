#ifndef _ENCODER_H_
#define _ENCODER_H_
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <iconv.h>
#include <strings.h>
#include <stdlib.h>

#include "protob/MsgUtil.h"


char * encode(int msgType, int sn, unsigned char* data, int length,char* destData) ;

#endif
