/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.8 at Tue Jul 18 19:08:07 2017. */

#ifndef PB_HWCAPACITYTOAPPREQ_PB_H_INCLUDED
#define PB_HWCAPACITYTOAPPREQ_PB_H_INCLUDED
#include <protob/pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _HwCapacityToAppReq {
    char serialNum[64];
    pb_callback_t objCapacity;
    int32_t userId;
/* @@protoc_insertion_point(struct:HwCapacityToAppReq) */
} HwCapacityToAppReq;

typedef struct _HwCapacityToAppReq_ObjCapacity {
    int32_t type;
    int32_t totalCapacity;
    int32_t remainCapacity;
/* @@protoc_insertion_point(struct:HwCapacityToAppReq_ObjCapacity) */
} HwCapacityToAppReq_ObjCapacity;

/* Default values for struct fields */

/* Initializer values for message structs */
#define HwCapacityToAppReq_init_default          {{{NULL}, NULL}, {{NULL}, NULL}, 0}
#define HwCapacityToAppReq_ObjCapacity_init_default {0, 0, 0}
#define HwCapacityToAppReq_init_zero             {{{NULL}, NULL}, {{NULL}, NULL}, 0}
#define HwCapacityToAppReq_ObjCapacity_init_zero {0, 0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define HwCapacityToAppReq_serialNum_tag         1
#define HwCapacityToAppReq_objCapacity_tag       2
#define HwCapacityToAppReq_userId_tag            3
#define HwCapacityToAppReq_ObjCapacity_type_tag  1
#define HwCapacityToAppReq_ObjCapacity_totalCapacity_tag 2
#define HwCapacityToAppReq_ObjCapacity_remainCapacity_tag 3

/* Struct field encoding specification for nanopb */
extern const pb_field_t HwCapacityToAppReq_fields[4];
extern const pb_field_t HwCapacityToAppReq_ObjCapacity_fields[4];

/* Maximum encoded size of messages (where known) */
/* HwCapacityToAppReq_size depends on runtime parameters */
#define HwCapacityToAppReq_ObjCapacity_size      33

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define HWCAPACITYTOAPPREQ_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif