/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.8 at Sun Jul 16 22:21:43 2017. */

#ifndef PB_HWRELEASEVOICEREQ_PB_H_INCLUDED
#define PB_HWRELEASEVOICEREQ_PB_H_INCLUDED
#include <protob/pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _HwReleaseVoiceReq {
    char serialNum[64];
    int32_t userId;
    char voiceName[64];
    int32_t voiceLength;
    char voiceDesc[64];
    char voicePath[64];
    char voicePic[64];
/* @@protoc_insertion_point(struct:HwReleaseVoiceReq) */
} HwReleaseVoiceReq;

/* Default values for struct fields */

/* Initializer values for message structs */
#define HwReleaseVoiceReq_init_default           {{{NULL}, NULL}, 0, {{NULL}, NULL}, 0, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}}
#define HwReleaseVoiceReq_init_zero              {{{NULL}, NULL}, 0, {{NULL}, NULL}, 0, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}}

/* Field tags (for use in manual encoding/decoding) */
#define HwReleaseVoiceReq_serialNum_tag          1
#define HwReleaseVoiceReq_userId_tag             2
#define HwReleaseVoiceReq_voiceName_tag          3
#define HwReleaseVoiceReq_voiceLength_tag        4
#define HwReleaseVoiceReq_voiceDesc_tag          5
#define HwReleaseVoiceReq_voicePath_tag          6
#define HwReleaseVoiceReq_voicePic_tag           7

/* Struct field encoding specification for nanopb */
extern const pb_field_t HwReleaseVoiceReq_fields[8];

/* Maximum encoded size of messages (where known) */
/* HwReleaseVoiceReq_size depends on runtime parameters */

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define HWRELEASEVOICEREQ_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
