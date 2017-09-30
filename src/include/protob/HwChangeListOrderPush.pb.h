/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.8 at Mon Jul 17 01:36:44 2017. */

#ifndef PB_HWCHANGELISTORDERPUSH_PB_H_INCLUDED
#define PB_HWCHANGELISTORDERPUSH_PB_H_INCLUDED
#include <protob/pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _HwChangeListOrderPush {
    int32_t relType;
    char relId[64];
    int32_t allCount;
/* @@protoc_insertion_point(struct:HwChangeListOrderPush) */
} HwChangeListOrderPush;

/* Default values for struct fields */

/* Initializer values for message structs */
#define HwChangeListOrderPush_init_default       {0, {{NULL}, NULL}, 0}
#define HwChangeListOrderPush_init_zero          {0, {{NULL}, NULL}, 0}

/* Field tags (for use in manual encoding/decoding) */
#define HwChangeListOrderPush_relType_tag        1
#define HwChangeListOrderPush_relId_tag          2
#define HwChangeListOrderPush_allCount_tag       3

/* Struct field encoding specification for nanopb */
extern const pb_field_t HwChangeListOrderPush_fields[4];

/* Maximum encoded size of messages (where known) */
/* HwChangeListOrderPush_size depends on runtime parameters */

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define HWCHANGELISTORDERPUSH_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif