/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.8 at Tue Jul 18 19:08:56 2017. */

#include "protob/HwPowerStatusReq.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t HwPowerStatusReq_fields[6] = {
    PB_FIELD(  1, STRING  , REQUIRED, STATIC  , FIRST, HwPowerStatusReq, serialNum, serialNum, 0),
    PB_FIELD(  2, INT32   , REQUIRED, STATIC  , OTHER, HwPowerStatusReq, power, serialNum, 0),
    PB_FIELD(  3, INT32   , REQUIRED, STATIC  , OTHER, HwPowerStatusReq, powerStatus, power, 0),
    PB_FIELD(  4, INT32   , REQUIRED, STATIC  , OTHER, HwPowerStatusReq, sleepMode, powerStatus, 0),
    PB_FIELD(  5, INT32   , REQUIRED, STATIC  , OTHER, HwPowerStatusReq, timing, sleepMode, 0),
    PB_LAST_FIELD
};


/* @@protoc_insertion_point(eof) */
