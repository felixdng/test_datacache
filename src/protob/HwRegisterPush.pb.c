/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.8 at Fri Sep  8 02:12:55 2017. */

#include "protob/HwRegisterPush.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t HwRegisterPush_fields[4] = {
    PB_FIELD(  1, INT32   , REQUIRED, STATIC  , FIRST, HwRegisterPush, userId, userId, 0),
    PB_FIELD(  2, STRING  , OPTIONAL, CALLBACK, OTHER, HwRegisterPush, nickname, userId, 0),
    PB_FIELD(  3, BOOL    , OPTIONAL, STATIC  , OTHER, HwRegisterPush, needReset, nickname, 0),
    PB_LAST_FIELD
};


/* @@protoc_insertion_point(eof) */
