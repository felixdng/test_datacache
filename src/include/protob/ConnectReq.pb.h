/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.8 at Thu Jun 22 02:20:39 2017. */

#ifndef PB_CONNECTREQ_PB_H_INCLUDED
#define PB_CONNECTREQ_PB_H_INCLUDED
#include <protob/pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _ConnectReq {
    char serialNum[64];
    bool has_isReset;
    bool isReset;
    bool has_versionCode;
    int32_t versionCode;
/* @@protoc_insertion_point(struct:ConnectReq) */
} ConnectReq;

/* Default values for struct fields */

/* Initializer values for message structs */
#define ConnectReq_init_default                  {{{NULL}, NULL}, false, 0, false, 0}
#define ConnectReq_init_zero                     {{{NULL}, NULL}, false, 0, false, 0}

/* Field tags (for use in manual encoding/decoding) */
#define ConnectReq_serialNum_tag                 1
#define ConnectReq_isReset_tag                   2
#define ConnectReq_versionCode_tag               3

/* Struct field encoding specification for nanopb */
extern const pb_field_t ConnectReq_fields[4];

/* Maximum encoded size of messages (where known) */
/* ConnectReq_size depends on runtime parameters */

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define CONNECTREQ_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif