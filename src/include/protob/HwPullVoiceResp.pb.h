/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.8 at Wed Sep 27 16:58:48 2017. */

#ifndef PB_HWPULLVOICERESP_PB_H_INCLUDED
#define PB_HWPULLVOICERESP_PB_H_INCLUDED
#include <protob/pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _HwPullVoiceResp {
    pb_callback_t objVoice;
    int64_t thisReqTime;
/* @@protoc_insertion_point(struct:HwPullVoiceResp) */
} HwPullVoiceResp;

typedef struct _HwPullVoiceResp_ObjVoice {
    int64_t voiceId;
    char voicePath[128];
    int64_t shareTime;
    bool has_voiceType;
    int64_t voiceType;
/* @@protoc_insertion_point(struct:HwPullVoiceResp_ObjVoice) */
} HwPullVoiceResp_ObjVoice;

/* Default values for struct fields */

/* Initializer values for message structs */
#define HwPullVoiceResp_init_default             {{{NULL}, NULL}, 0}
#define HwPullVoiceResp_ObjVoice_init_default    {0, "", 0, false, 0}
#define HwPullVoiceResp_init_zero                {{{NULL}, NULL}, 0}
#define HwPullVoiceResp_ObjVoice_init_zero       {0, "", 0, false, 0}

/* Field tags (for use in manual encoding/decoding) */
#define HwPullVoiceResp_objVoice_tag             1
#define HwPullVoiceResp_thisReqTime_tag          2
#define HwPullVoiceResp_ObjVoice_voiceId_tag     1
#define HwPullVoiceResp_ObjVoice_voicePath_tag   2
#define HwPullVoiceResp_ObjVoice_shareTime_tag   3
#define HwPullVoiceResp_ObjVoice_voiceType_tag   4

/* Struct field encoding specification for nanopb */
extern const pb_field_t HwPullVoiceResp_fields[3];
extern const pb_field_t HwPullVoiceResp_ObjVoice_fields[5];

/* Maximum encoded size of messages (where known) */
/* HwPullVoiceResp_size depends on runtime parameters */
#define HwPullVoiceResp_ObjVoice_size            164

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define HWPULLVOICERESP_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
