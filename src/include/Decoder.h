/*
 * Docoder.h
 *
 *  Created on: Apr 14, 2017
 *      Author: Source
 */
#ifndef _DECODER_H_
#define _DECODER_H_
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "protob/MsgUtil.h"

void decode_msgbuf(  char* data, int size, char *msg_buf, int *msg_len);

void decode(char* data, int size);
static char DengLu[4]={240,17,240,1};//登陆
static char DuanKai[4]={240,17,240,49};//断开连接
static char ZhuCe[4]={240,17,240,99};//注册设备号
static char ZhuXiao[4]={240,17,240,8};//注销设备
static char ChangeName[4]={240,17,240,88};//更改设备名称
static char AlterMaster[4]={240,17,240,7};//更改主控
static char StartPlay[4]={240,17,240,3};//开始播放
static char PlayControl[4]={240,17,240,4};//播放控制
static char PlayMode[4]={240,17,240,21};//播放模式
static char AddVoice[4]={240,17,240,23};//添加到播放列表
static char DeleteVoice[4]={240,17,240,24};//从播放列表删除
static char Volum[4]={240,17,240,5};//调整音量
static char HwStatus[4]={240,17,240,37};//硬件状态
static char AddDiscuss[4]={240,17,240,54};//添加硬件讨论群
static char ReleaseVoice[4]={240,17,240,72};//发布声音到群组
static char ShareVoice[4]={240,17,240,22};//分享声音到群组
static char DeleteGroupVoice[4]={240,17,240,34};//删除群声音
static char DeleteSDVoice[4]={240,17,240,131};//删除SD声音
static char DLLocal[4]={240,17,240,103};//下载本地专辑
static char AddCustomVoice[4]={240,17,240,69};//添加自定义声音列表
static char DeleteCustomVoice[4]={240,17,240,70};//删除自定义声音列表
static char ChangeListOrder[4]={240,17,240,80};//修改自定义列表声音顺序
static char CacheVoice[4]={240,17,240,35};//缓存声音到设备
static char DeleteDownloadedVoice[4]={240,17,240,67};//删除设备缓存声音
static char SetWifi[4]={240,17,240,83};//设置wifi
static char Capacity[4]={240,17,240,89};//获取当前存储情况
static char VersionUpdate[4]={240,17,240,113};//获取版本号
static char Close[4]={240,17,240,114};//关闭硬件
static char SleepMode[4]={240,17,240,115};//设置睡眠模式
static char GetWifi[4]={240,17,240,81};//获取wifi信息
static char MonitorOpen[4]={240,17,240,39};//kaiqijiankong
static char MonitorClose[4]={240,17,240,40};//guanbijiankong
static char TalkbackOpen[4]={240,17,240,41};//kaiqiduijaing
static char TalkbackClose[4]={240,17,240,48};//guanbiduijaing








#endif
