/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.8 at Sun Jul 16 20:22:54 2017. */

#ifndef PB_HWSTATUSPUSH_PB_H_INCLUDED
#define PB_HWSTATUSPUSH_PB_H_INCLUDED
#include <protob/pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _HwStatusPush {
    int32_t userId;
/* @@protoc_insertion_point(struct:HwStatusPush) */
} HwStatusPush;

/* Default values for struct fields */

/* Initializer values for message structs */
#define HwStatusPush_init_default                {0}
#define HwStatusPush_init_zero                   {0}

/* Field tags (for use in manual encoding/decoding) */
#define HwStatusPush_userId_tag                  1

/* Struct field encoding specification for nanopb */
extern const pb_field_t HwStatusPush_fields[2];

/* Maximum encoded size of messages (where known) */
#define HwStatusPush_size                        11

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define HWSTATUSPUSH_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
