/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.8 at Tue Jul 18 23:52:23 2017. */

#include "protob/HwStartPlayPush.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t HwStartPlayPush_fields[9] = {
    PB_FIELD(  1, INT32   , REQUIRED, STATIC  , FIRST, HwStartPlayPush, voiceId, voiceId, 0),
    PB_FIELD(  2, STRING  , REQUIRED, STATIC  , OTHER, HwStartPlayPush, voicePath, voiceId, 0),
    PB_FIELD(  3, INT32   , REQUIRED, STATIC  , OTHER, HwStartPlayPush, voiceType, voicePath, 0),
    PB_FIELD(  4, INT32   , REQUIRED, STATIC  , OTHER, HwStartPlayPush, relType, voiceType, 0),
    PB_FIELD(  5, STRING  , REQUIRED, STATIC  , OTHER, HwStartPlayPush, relId, relType, 0),
    PB_FIELD(  6, INT32   , REQUIRED, STATIC  , OTHER, HwStartPlayPush, allCount, relId, 0),
    PB_FIELD(  7, INT32   , REQUIRED, STATIC  , OTHER, HwStartPlayPush, userId, allCount, 0),
    PB_FIELD(  8, STRING  , REQUIRED, STATIC  , OTHER, HwStartPlayPush, netPath, userId, 0),
    PB_LAST_FIELD
};


/* @@protoc_insertion_point(eof) */