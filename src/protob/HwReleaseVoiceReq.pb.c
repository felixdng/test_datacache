/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.8 at Sun Jul 16 22:21:43 2017. */

#include "protob/HwReleaseVoiceReq.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t HwReleaseVoiceReq_fields[8] = {
    PB_FIELD(  1, STRING  , REQUIRED, STATIC  , FIRST, HwReleaseVoiceReq, serialNum, serialNum, 0),
    PB_FIELD(  2, INT32   , REQUIRED, STATIC  , OTHER, HwReleaseVoiceReq, userId, serialNum, 0),
    PB_FIELD(  3, STRING  , REQUIRED, STATIC  , OTHER, HwReleaseVoiceReq, voiceName, userId, 0),
    PB_FIELD(  4, INT32   , REQUIRED, STATIC  , OTHER, HwReleaseVoiceReq, voiceLength, voiceName, 0),
    PB_FIELD(  5, STRING  , REQUIRED, STATIC  , OTHER, HwReleaseVoiceReq, voiceDesc, voiceLength, 0),
    PB_FIELD(  6, STRING  , REQUIRED, STATIC  , OTHER, HwReleaseVoiceReq, voicePath, voiceDesc, 0),
    PB_FIELD(  7, STRING  , REQUIRED, STATIC  , OTHER, HwReleaseVoiceReq, voicePic, voicePath, 0),
    PB_LAST_FIELD
};


/* @@protoc_insertion_point(eof) */