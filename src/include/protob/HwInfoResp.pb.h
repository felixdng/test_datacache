/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.8 at Fri Oct  6 00:20:46 2017. */

#ifndef PB_HWINFORESP_PB_H_INCLUDED
#define PB_HWINFORESP_PB_H_INCLUDED
#include <protob/pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _HwInfoResp_Result {
    int64_t status;
    char nickname[64];
/* @@protoc_insertion_point(struct:HwInfoResp_Result) */
} HwInfoResp_Result;

typedef struct _HwInfoResp {
    int64_t state;
    char msg[64];
    bool has_result;
    HwInfoResp_Result result;
/* @@protoc_insertion_point(struct:HwInfoResp) */
} HwInfoResp;

/* Default values for struct fields */

/* Initializer values for message structs */
#define HwInfoResp_init_default                  {0, "", false, HwInfoResp_Result_init_default}
#define HwInfoResp_Result_init_default           {0, ""}
#define HwInfoResp_init_zero                     {0, "", false, HwInfoResp_Result_init_zero}
#define HwInfoResp_Result_init_zero              {0, ""}

/* Field tags (for use in manual encoding/decoding) */
#define HwInfoResp_Result_status_tag             1
#define HwInfoResp_Result_nickname_tag           2
#define HwInfoResp_state_tag                     1
#define HwInfoResp_msg_tag                       2
#define HwInfoResp_result_tag                    3

/* Struct field encoding specification for nanopb */
extern const pb_field_t HwInfoResp_fields[4];
extern const pb_field_t HwInfoResp_Result_fields[3];

/* Maximum encoded size of messages (where known) */
#define HwInfoResp_size                          156
#define HwInfoResp_Result_size                   77

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define HWINFORESP_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
