/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.8 at Tue Jul 18 22:49:20 2017. */

#ifndef PB_HWRELEASEVOICEPUSH_PB_H_INCLUDED
#define PB_HWRELEASEVOICEPUSH_PB_H_INCLUDED
#include <protob/pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _HwReleaseVoicePush {
    int32_t voiceId;
    char voicePath[64];
    int32_t shareTime;
/* @@protoc_insertion_point(struct:HwReleaseVoicePush) */
} HwReleaseVoicePush;

/* Default values for struct fields */

/* Initializer values for message structs */
#define HwReleaseVoicePush_init_default          {0, {{NULL}, NULL}, 0}
#define HwReleaseVoicePush_init_zero             {0, {{NULL}, NULL}, 0}

/* Field tags (for use in manual encoding/decoding) */
#define HwReleaseVoicePush_voiceId_tag           1
#define HwReleaseVoicePush_voicePath_tag         2
#define HwReleaseVoicePush_shareTime_tag         3

/* Struct field encoding specification for nanopb */
extern const pb_field_t HwReleaseVoicePush_fields[4];

/* Maximum encoded size of messages (where known) */
/* HwReleaseVoicePush_size depends on runtime parameters */

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define HWRELEASEVOICEPUSH_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
