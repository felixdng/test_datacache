/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.8 at Sun Jul 16 19:48:46 2017. */

#ifndef PB_HWPLAYCONTROLPUSH_PB_H_INCLUDED
#define PB_HWPLAYCONTROLPUSH_PB_H_INCLUDED
#include <protob/pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _HwPlayControlPush {
    int32_t playType;
/* @@protoc_insertion_point(struct:HwPlayControlPush) */
} HwPlayControlPush;

/* Default values for struct fields */

/* Initializer values for message structs */
#define HwPlayControlPush_init_default           {0}
#define HwPlayControlPush_init_zero              {0}

/* Field tags (for use in manual encoding/decoding) */
#define HwPlayControlPush_playType_tag           1

/* Struct field encoding specification for nanopb */
extern const pb_field_t HwPlayControlPush_fields[2];

/* Maximum encoded size of messages (where known) */
#define HwPlayControlPush_size                   11

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define HWPLAYCONTROLPUSH_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif