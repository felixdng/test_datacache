/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.8 at Tue Jul 18 22:37:06 2017. */

#include "protob/HwChangeStatusReq.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t HwChangeStatusReq_fields[4] = {
    PB_FIELD(  1, STRING  , REQUIRED, STATIC  , FIRST, HwChangeStatusReq, serialNum, serialNum, 0),
    PB_FIELD(  2, INT32   , REQUIRED, STATIC  , OTHER, HwChangeStatusReq, state, serialNum, 0),
    PB_FIELD(  3, INT32   , REQUIRED, STATIC  , OTHER, HwChangeStatusReq, reqTime, state, 0),
    PB_LAST_FIELD
};


/* @@protoc_insertion_point(eof) */