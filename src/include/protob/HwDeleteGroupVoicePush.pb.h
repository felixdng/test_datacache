/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.8 at Sun Jul 16 22:22:10 2017. */

#ifndef PB_HWDELETEGROUPVOICEPUSH_PB_H_INCLUDED
#define PB_HWDELETEGROUPVOICEPUSH_PB_H_INCLUDED
#include <protob/pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _HwDeleteGroupVoicePush {
    int32_t voiceIds;
    pb_callback_t objVoice;
/* @@protoc_insertion_point(struct:HwDeleteGroupVoicePush) */
} HwDeleteGroupVoicePush;

typedef struct _HwDeleteGroupVoicePush_ObjVoice {
    int64_t voiceId;
    bool has_voiceType;
    int64_t voiceType;
/* @@protoc_insertion_point(struct:HwDeleteGroupVoicePush_ObjVoice) */
} HwDeleteGroupVoicePush_ObjVoice;

/* Default values for struct fields */

/* Initializer values for message structs */
#define HwDeleteGroupVoicePush_init_default      {{{NULL}, NULL}, {{NULL}, NULL}}
#define HwDeleteGroupVoicePush_ObjVoice_init_default {0, false, 0}
#define HwDeleteGroupVoicePush_init_zero         {{{NULL}, NULL}, {{NULL}, NULL}}
#define HwDeleteGroupVoicePush_ObjVoice_init_zero {0, false, 0}

/* Field tags (for use in manual encoding/decoding) */
#define HwDeleteGroupVoicePush_voiceIds_tag      1
#define HwDeleteGroupVoicePush_objVoice_tag      2
#define HwDeleteGroupVoicePush_ObjVoice_voiceId_tag 1
#define HwDeleteGroupVoicePush_ObjVoice_voiceType_tag 2

/* Struct field encoding specification for nanopb */
extern const pb_field_t HwDeleteGroupVoicePush_fields[3];
extern const pb_field_t HwDeleteGroupVoicePush_ObjVoice_fields[3];

/* Maximum encoded size of messages (where known) */
/* HwDeleteGroupVoicePush_size depends on runtime parameters */
#define HwDeleteGroupVoicePush_ObjVoice_size     22

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define HWDELETEGROUPVOICEPUSH_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif