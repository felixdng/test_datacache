# Copyright 2008-2017, Marvell International Ltd.

libs-y += libprotob

libprotob-objs-y := pb_common.c pb_decode.c pb_encode.c simple.pb.c MsgUtil.c  ConnectReq.pb.c ConnectResp.pb.c \
 DisconnectReq.pb.c DisconnectResp.pb.c HwAddCustomVoicePush.pb.c HwAddDiscussPush.pb.c HwCacheVoicePush.pb.c \
 HwCapacityPush.pb.c HwCapacityToAppReq.pb.c HwChangeListOrderPush.pb.c HwChangeNamePush.pb.c HwChangeStatusReq.pb.c \
 HwClosePush.pb.c HwDeleteCustomVoicePush.pb.c HwDeleteDownloadedVoicePush.pb.c HwDeleteGroupVoicePush.pb.c \
 HwDeleteSDVoicePush.pb.c HwDeleteVoicePush.pb.c HwDLLocalPush.pb.c HwDLLocalStatusReq.pb.c HwDLLocalToAppReq.pb.c \
 HwGetWifiPush.pb.c HwOperateStatusReq.pb.c HwPlayControlPush.pb.c HwPlayModePush.pb.c HwPlayPush.pb.c  HwPowerStatusReq.pb.c \
 HwPullDiscussReq.pb.c HwPullVoiceListReq.pb.c HwPullVoiceReq.pb.c HwRegisterReq.pb.c  HwRegisterResp.pb.c HwReleaseVoicePush.pb.c \
 HwReleaseVoiceReq.pb.c HwSdVoiceAddReq.pb.c HwSetWifiPush.pb.c HwShareVoicePush.pb.c  HwSleepModePush.pb.c  \
 HwStartPlayPush.pb.c  HwStatusPush.pb.c HwStatusToAppReq.pb.c HwVersionUpdatePush.pb.c HwVolumePush.pb.c  \
 HwWifiToAppReq.pb.c AlterMasterPush.pb.c HwInfoReq.pb.c HwInfoResp.pb.c

libprotob-supported-toolchain-y := arm_gcc iar
