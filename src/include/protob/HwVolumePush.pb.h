/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.8 at Sat Sep  9 01:00:37 2017. */

#ifndef PB_HWVOLUMEPUSH_PB_H_INCLUDED
#define PB_HWVOLUMEPUSH_PB_H_INCLUDED
#include <protob/pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _HwVolumePush {
    int32_t volume;
/* @@protoc_insertion_point(struct:HwVolumePush) */
} HwVolumePush;

/* Default values for struct fields */

/* Initializer values for message structs */
#define HwVolumePush_init_default                {0}
#define HwVolumePush_init_zero                   {0}

/* Field tags (for use in manual encoding/decoding) */
#define HwVolumePush_volume_tag                  1

/* Struct field encoding specification for nanopb */
extern const pb_field_t HwVolumePush_fields[2];

/* Maximum encoded size of messages (where known) */
#define HwVolumePush_size                        11

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define HWVOLUMEPUSH_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif