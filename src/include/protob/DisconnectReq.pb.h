/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.8 at Wed Jul 12 20:03:02 2017. */

#ifndef PB_DISCONNECTREQ_PB_H_INCLUDED
#define PB_DISCONNECTREQ_PB_H_INCLUDED
#include <protob/pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _DisconnectReq {
    char serialNum[64];
/* @@protoc_insertion_point(struct:DisconnectReq) */
} DisconnectReq;

/* Default values for struct fields */

/* Initializer values for message structs */
#define DisconnectReq_init_default               {{{NULL}, NULL}}
#define DisconnectReq_init_zero                  {{{NULL}, NULL}}

/* Field tags (for use in manual encoding/decoding) */
#define DisconnectReq_serialNum_tag              1

/* Struct field encoding specification for nanopb */
extern const pb_field_t DisconnectReq_fields[2];

/* Maximum encoded size of messages (where known) */
/* DisconnectReq_size depends on runtime parameters */

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define DISCONNECTREQ_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif