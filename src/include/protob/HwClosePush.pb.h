/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.8 at Tue Jul 18 19:08:31 2017. */

#ifndef PB_HWCLOSEPUSH_PB_H_INCLUDED
#define PB_HWCLOSEPUSH_PB_H_INCLUDED
#include <protob/pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _HwClosePush {
    int32_t timing;
/* @@protoc_insertion_point(struct:HwClosePush) */
} HwClosePush;

/* Default values for struct fields */

/* Initializer values for message structs */
#define HwClosePush_init_default                 {0}
#define HwClosePush_init_zero                    {0}

/* Field tags (for use in manual encoding/decoding) */
#define HwClosePush_timing_tag                   1

/* Struct field encoding specification for nanopb */
extern const pb_field_t HwClosePush_fields[2];

/* Maximum encoded size of messages (where known) */
#define HwClosePush_size                         11

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define HWCLOSEPUSH_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
