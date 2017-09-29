/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.8 at Tue Jul 18 23:52:23 2017. */

#ifndef PB_HWSTARTPLAYPUSH_PB_H_INCLUDED
#define PB_HWSTARTPLAYPUSH_PB_H_INCLUDED
#include <protob/pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _HwStartPlayPush {
    int32_t voiceId;
    char voicePath[64];
    int32_t voiceType;
    int32_t relType;
    char relId[64];
    int32_t allCount;
    bool has_userId;
    int32_t userId;
    char netPath[64];
/* @@protoc_insertion_point(struct:HwStartPlayPush) */
} HwStartPlayPush;

/* Default values for struct fields */

/* Initializer values for message structs */
#define HwStartPlayPush_init_default             {0, {{NULL}, NULL}, 0, 0, {{NULL}, NULL}, 0, false, 0, {{NULL}, NULL}}
#define HwStartPlayPush_init_zero                {0, {{NULL}, NULL}, 0, 0, {{NULL}, NULL}, 0, false, 0, {{NULL}, NULL}}

/* Field tags (for use in manual encoding/decoding) */
#define HwStartPlayPush_voiceId_tag              1
#define HwStartPlayPush_voicePath_tag            2
#define HwStartPlayPush_voiceType_tag            3
#define HwStartPlayPush_relType_tag              4
#define HwStartPlayPush_relId_tag                5
#define HwStartPlayPush_allCount_tag             6
#define HwStartPlayPush_userId_tag               7
#define HwStartPlayPush_netPath_tag              8

/* Struct field encoding specification for nanopb */
extern const pb_field_t HwStartPlayPush_fields[9];

/* Maximum encoded size of messages (where known) */
/* HwStartPlayPush_size depends on runtime parameters */

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define HWSTARTPLAYPUSH_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
