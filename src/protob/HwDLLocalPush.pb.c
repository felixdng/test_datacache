/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.8 at Sun Jul 16 23:12:22 2017. */

#include "protob/HwDLLocalPush.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t HwDLLocalPush_fields[3] = {
    PB_FIELD(  1, MESSAGE , REPEATED, CALLBACK, FIRST, HwDLLocalPush, objVoice, objVoice, &HwDLLocalPush_ObjVoice_fields),
    PB_FIELD(  2, INT32   , REQUIRED, STATIC  , OTHER, HwDLLocalPush, dlType, objVoice, 0),
    PB_LAST_FIELD
};

const pb_field_t HwDLLocalPush_ObjVoice_fields[4] = {
    PB_FIELD(  1, INT32   , REQUIRED, STATIC  , FIRST, HwDLLocalPush_ObjVoice, voiceId, voiceId, 0),
    PB_FIELD(  2, STRING  , REQUIRED, STATIC  , OTHER, HwDLLocalPush_ObjVoice, netPath, voiceId, 0),
    PB_FIELD(  3, STRING  , REQUIRED, STATIC  , OTHER, HwDLLocalPush_ObjVoice, localPath, netPath, 0),
    PB_LAST_FIELD
};


/* @@protoc_insertion_point(eof) */