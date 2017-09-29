/*
 * Docoder.h
 *
 *  Created on: Apr 14, 2017
 *      Author: Source
 */
#ifndef _CONTROL_H_
#define _CONTROL_H_
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "protob/MsgUtil.h"
#include <protob/ConnectReq.pb.h>
#include <protob/ConnectResp.pb.h>
#include <protob/DisconnectReq.pb.h>
#include <protob/DisconnectResp.pb.h>
#include <protob/SignOffPush.pb.h>
#include <protob/HwAddCustomVoicePush.pb.h>
#include <protob/HwAddDiscussPush.pb.h>
#include <protob/HwCacheVoicePush.pb.h>
#include <protob/HwCapacityPush.pb.h>
#include <protob/HwCapacityToAppReq.pb.h>
#include <protob/HwChangeListOrderPush.pb.h>
#include <protob/HwChangeNamePush.pb.h>
#include <protob/HwChangeStatusReq.pb.h>
#include <protob/HwClosePush.pb.h>
#include <protob/HwDeleteCustomVoicePush.pb.h>
#include <protob/HwDeleteDownloadedVoicePush.pb.h>
#include <protob/HwDeleteGroupVoicePush.pb.h>
#include <protob/HwDeleteSDVoicePush.pb.h>
#include <protob/HwDeleteVoicePush.pb.h>
#include <protob/HwDLLocalPush.pb.h>
#include <protob/HwDLLocalStatusReq.pb.h>
#include <protob/HwDLLocalToAppReq.pb.h>
#include <protob/HwGetWifiPush.pb.h>
#include <protob/HwOperateStatusReq.pb.h>
#include <protob/HwPlayControlPush.pb.h>
#include <protob/HwPlayModePush.pb.h>
#include <protob/HwPlayPush.pb.h>
#include <protob/HwPowerStatusReq.pb.h>
#include <protob/HwPullDiscussReq.pb.h>
#include <protob/HwPullVoiceListReq.pb.h>
#include <protob/HwPullVoiceReq.pb.h>
#include <protob/HwRegisterPush.pb.h>
#include <protob/HwRegisterReq.pb.h>
#include <protob/HwRegisterResp.pb.h>
#include <protob/HwReleaseVoicePush.pb.h>
#include <protob/HwReleaseVoiceReq.pb.h>
#include <protob/HwSdVoiceAddReq.pb.h>
#include <protob/HwSetWifiPush.pb.h>
#include <protob/HwShareVoicePush.pb.h>
#include <protob/HwSleepModePush.pb.h>
#include <protob/HwStartPlayPush.pb.h>
#include <protob/HwStatusPush.pb.h>
#include <protob/HwStatusToAppReq.pb.h>
#include <protob/HwVersionUpdatePush.pb.h>
#include <protob/HwVolumePush.pb.h>
#include <protob/HwWifiToAppReq.pb.h>
#include <protob/AlterMasterPush.pb.h>
#include <protob/HwAddVoicePush.pb.h>
#include <protob/HwVoiceInfoReq.pb.h>
#include <protob/HwVoiceErrorInfoReq.pb.h>
#include <protob/SignOffPush.pb.h>
#include <protob/HwTalkbackClosePush.pb.h>
#include <protob/HwMonitorOpenPush.pb.h>
#include <protob/HwMonitorClosePush.pb.h>
#include <protob/HwTalkbackOpenPush.pb.h>
#include <protob/ImTokenReq.pb.h>
#include <protob/PushAddressReq.pb.h>
void DLu(char *msgBytes,int len);//设备登陆
void SPlay(char *msgBytes,int len);//开始播放
void Zce(char *msgBytes,int len);//设备注册
void Zxiao(char *msgBytes,int len);//设备注销
void PControl(char *msgBytes,int len);//播放控制
void DLocal(char *msgBytes,int len);//下载本地专辑
void RVoice(char *msgBytes,int len);//发布声音到群组
void SVoice(char *msgBytes,int len);//分享声音到群组
void DGVoice(char *msgBytes,int len);//删除群声音
void VUpdate(char *msgBytes,int len);//获取版本号
void GWifi(char *msgBytes,int len);//获取wifi信息
void SMode(char *msgBytes,int len);//设置睡眠模式
void HwClose(char *msgBytes,int len);//设备关机
void Swifi(char *msgBytes,int len);//设置网络
void Cap(char *msgBytes,int len);//获取当前存储情况
void ACVoice(char *msgBytes,int len);//添加自定义声音列表
void DCVoice(char *msgBytes,int len);//删除自定义声音列表
void CLOrder(char *msgBytes,int len);//修改自定义列表声音顺序
void CVoice(char *msgBytes,int len);//缓存声音到设备
void DDVoice(char *msgBytes,int len);//删除设备缓存声音
void DSDVoice(char *msgBytes,int len);//删除SD声音
void ADiscuss(char *msgBytes,int len);//添加硬件讨论群
void HStatus(char *msgBytes,int len);//设备状态查询
void CName(char *msgBytes,int len);//更改昵称
void Amaster(char *msgBytes,int len);//更改主控
void Pmode(char *msgBytes,int len);//改变播放模式
void Avoice(char *msgBytes,int len);//添加到播放列表
void Dvoice(char *msgBytes,int len);//从播放列表删除
void Vinfo(int pushUserId,int voiceId,int voiceType,int relType  );//硬件返回声音播放信息
void VEinfo(int pushUserId,int voiceId,int voiceType,int relType,char *error );//硬件返回声音错误播放信息
void PVlist(int page,int pageSize,int relType,char *relId  );//硬件拉取指定列表声音
void Ostatus(int userId,int operateType,int voiceId,char *ext  );//硬件推送操作结果
void Cstatus(int state,int reqTime);//硬件状态改变
void Pdiscuss(int reqTime );//硬件拉取群讨论
void Pvoice(int reqTime);//硬件拉取群声音
void DLstatus(void );//本地专辑下载状态
void SDvoiceadd(void);//硬件推送SD卡声音
void Powerstatus(int power,int powerStatus,int sleepMode,int timing );//推送电量与睡眠状态
void Topen(char *msgBytes,int len);//dakaishipinduijaing
void Mclose(char *msgBytes,int len);//guanbijiankong
void Mopen(char *msgBytes,int len);//dakaijiankong
void IToken(char *accid);//huoquyunxin
void PAddress(void);//huoqutuliu
void Tclose(char *msgBytes,int len);//guanbishipinduijaing
#endif
