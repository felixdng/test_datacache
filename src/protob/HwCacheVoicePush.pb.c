/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.8 at Mon Jul 17 01:36:29 2017. */

#include "protob/HwCacheVoicePush.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t HwCacheVoicePush_fields[2] = {
    PB_FIELD(  1, MESSAGE , REPEATED, CALLBACK, FIRST, HwCacheVoicePush, objVoice, objVoice, &HwCacheVoicePush_ObjVoice_fields),
    PB_LAST_FIELD
};

const pb_field_t HwCacheVoicePush_ObjVoice_fields[4] = {
    PB_FIELD(  1, INT32   , REQUIRED, STATIC  , FIRST, HwCacheVoicePush_ObjVoice, voiceId, voiceId, 0),
    PB_FIELD(  2, STRING  , REQUIRED, STATIC, OTHER, HwCacheVoicePush_ObjVoice, voicePath, voiceId, 0),
    PB_FIELD(  3, INT32   , REQUIRED, STATIC  , OTHER, HwCacheVoicePush_ObjVoice, voiceType, voicePath, 0),
    PB_LAST_FIELD
};


/* @@protoc_insertion_point(eof) */
