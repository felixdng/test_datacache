/* pb_decode.c -- decode a protobuf using minimal resources
 *
 * 2011 Petteri Aimonen <jpa@kapsi.fi>
 */

/* Use the GCC warn_unused_result attribute to check that all return values
 * are propagated correctly. On other compilers and gcc before 3.4.0 just
 * ignore the annotation.
 */

#include <stdio.h>
#include "Decoder.h"
#include <protob/pb_encode.h>
#include <protob/pb_decode.h>
#include <protob/simple.pb.h>
#include <protob/pb_common.h>
#include <protob/ConnectReq.pb.h>
#include "protob/ConnectResp.pb.h"
#include "Encoder.h"
#include "Control.h"

const int RESP_CODE = 2;
static const int UUID_LENGTH = 36;
static const int TYPE = 4;
static const int LENGTH = 2;
static const int UPD_GROUP = 4;
static const int SN = 4;
static const int ADLER32 = 8;
static const int maxLineLength = 20000;
struct Buffer {
	int bufLeng;
	int matchCount; //需要匹配的msg长度，即协议里面的len
	int length; // 整个消息总长度
	char * bytes;
	int bytesSize;
};

struct Buffer ctxBuffer = { 0, 0, 0, NULL };
/**
 * 消息验证通过后，进行处理分发
 */
int datacmp(char str1[4],char str2[4])
{int i=0;
while(str1[i]==str2[i])
{i++;}
printf("i: %d",i);
if(i==4 || i>4)
	return 0;
}
void processStream() {
    
	char typeStr[4];
	int i;
	memcpy(typeStr, ctxBuffer.bytes, TYPE);	
	char snStr[4];
	memcpy(snStr, ctxBuffer.bytes + TYPE, SN);
	char respCodeStr[2];
	memcpy(respCodeStr, ctxBuffer.bytes + TYPE + SN, RESP_CODE);
	char updGroup[4];
	memcpy(updGroup,ctxBuffer.bytes + TYPE + SN + RESP_CODE,UPD_GROUP);
	unsigned char msgBytes[ctxBuffer.matchCount];
	memcpy(msgBytes, ctxBuffer.bytes + TYPE + SN + RESP_CODE + UPD_GROUP + LENGTH,
			ctxBuffer.matchCount);

	printf("ctx bytes: ");
	for (i = 0; i < ctxBuffer.length; ++i) {
	printf("%d, ", ctxBuffer.bytes[i]);
	}
	printf("\n");

	printf("msgBytes: ");
	for (i = 0; i < ctxBuffer.matchCount; ++i) {
		printf("%d, ", msgBytes[i]);
	}
	printf("\n");

	char adler32[8];
	memcpy(adler32, ctxBuffer.bytes + TYPE + SN + RESP_CODE + UPD_GROUP + LENGTH + ctxBuffer.matchCount,
			ADLER32);
	uint64_t adl = recoverNetworkBytesOrderLong(adler32, 0);

	uint64_t verifyAdl = getAdler32(msgBytes, ctxBuffer.matchCount);
	if (adl == verifyAdl) {
		printf("adler32 succeed\n");
	} else {
		printf("adler32 failed\n");
		printf("msg adler32 = %ld, verify adler32 = %ld\n", adl, verifyAdl);
	}
   if(datacmp(typeStr, DengLu)==0)
		{printf("equard");
        DLu(msgBytes,ctxBuffer.matchCount);}//建立连接
   else if(datacmp(typeStr, StartPlay)==0)
   	{ SPlay(msgBytes,ctxBuffer.matchCount);    }//开始播放
   else if(datacmp(typeStr, PlayControl)==0)
   	{ PControl(msgBytes,ctxBuffer.matchCount);    } //播放控制
   else if(datacmp(typeStr, DLLocal)==0)
   	{ DLocal(msgBytes,ctxBuffer.matchCount);    }  //本地下载
   else if(datacmp(typeStr,ReleaseVoice)==0)
   {  RVoice(msgBytes,ctxBuffer.matchCount);}   //发布声音到群组
   else if(datacmp(typeStr,ShareVoice)==0)
   {  SVoice(msgBytes,ctxBuffer.matchCount);}   //分享声音到群组
   else if(datacmp(typeStr,DeleteGroupVoice)==0)
   {  DGVoice(msgBytes,ctxBuffer.matchCount);}   //删除群声音
    else if(datacmp(typeStr,VersionUpdate)==0)
   {  VUpdate(msgBytes,ctxBuffer.matchCount);}  //获取版本号
    else if(datacmp(typeStr,GetWifi)==0)
   {  GWifi(msgBytes,ctxBuffer.matchCount);}   //获取wifi信息
    else if(datacmp(typeStr,SleepMode)==0)
   {  SMode(msgBytes,ctxBuffer.matchCount);}  //设置睡眠模式
    else if(datacmp(typeStr,Close)==0)
   {  HwClose(msgBytes,ctxBuffer.matchCount);}  //关闭设备
    else if(datacmp(typeStr,SetWifi)==0) 
   {  Swifi(msgBytes,ctxBuffer.matchCount);}    //设置网络
    else if(datacmp(typeStr,Capacity)==0)
   {  Cap(msgBytes,ctxBuffer.matchCount);}     //获取当前存储情况
    else if(datacmp(typeStr,AddCustomVoice)==0)
   {  ACVoice(msgBytes,ctxBuffer.matchCount);} //添加自定义声音列表
     else if(datacmp(typeStr,DeleteCustomVoice)==0)
   {  DCVoice(msgBytes,ctxBuffer.matchCount);}  //删除自定义声音列表
    else if(datacmp(typeStr,ChangeListOrder)==0)
   {  CLOrder(msgBytes,ctxBuffer.matchCount);}  //修改自定义列表声音顺序
    else if(datacmp(typeStr,CacheVoice)==0)
   {   CVoice(msgBytes,ctxBuffer.matchCount);}  //缓存声音到设备
    else if(datacmp(typeStr,DeleteDownloadedVoice)==0)
   {   DDVoice(msgBytes,ctxBuffer.matchCount);}  //删除设备缓存声音
    else if(datacmp(typeStr,DeleteSDVoice)==0)
   {   DSDVoice(msgBytes,ctxBuffer.matchCount);}  //删除SD声音
    else if(datacmp(typeStr,AddDiscuss)==0)
   {   ADiscuss(msgBytes,ctxBuffer.matchCount);}  //添加硬件讨论群
     else if(datacmp(typeStr,HwStatus)==0)
   {    HStatus(msgBytes,ctxBuffer.matchCount);}  //设备状态查询
    else if(datacmp(typeStr,ChangeName)==0)
   {    CName(msgBytes,ctxBuffer.matchCount);}    //更改昵称
    else if(datacmp(typeStr,AlterMaster)==0)
   {    Amaster(msgBytes,ctxBuffer.matchCount);}  //更改主控
    else if(datacmp(typeStr,PlayMode)==0)
   {    Pmode(msgBytes,ctxBuffer.matchCount);}   //改变播放模式
    else if(datacmp(typeStr,AddVoice)==0)
   {    Avoice(msgBytes,ctxBuffer.matchCount);}   //添加到播放列表
    else if(datacmp(typeStr,DeleteVoice)==0)
   {    Dvoice(msgBytes,ctxBuffer.matchCount);}   //从播放列表删除
    else if(datacmp(typeStr,ZhuCe)==0)
   {   Zce(msgBytes,ctxBuffer.matchCount);    }//设备注册
    else if(datacmp(typeStr,ZhuCe)==0)
   {   Zxiao(msgBytes,ctxBuffer.matchCount);    }//设备注销

    else if(datacmp(typeStr,MonitorOpen)==0)
   {    Mopen(msgBytes,ctxBuffer.matchCount);}  //dakaijiankong
    else if(datacmp(typeStr,MonitorClose)==0)
   {    Mclose(msgBytes,ctxBuffer.matchCount);}   //guanbijiankong
    else if(datacmp(typeStr,TalkbackOpen)==0)
   {    Topen(msgBytes,ctxBuffer.matchCount);}   //dakaiduijaing
    else if(datacmp(typeStr,TalkbackClose)==0)
   {    Tclose(msgBytes,ctxBuffer.matchCount);}   //guanbiduijaing
    

	free(ctxBuffer.bytes);
	ctxBuffer.bytes = NULL;
	ctxBuffer.bufLeng = 0;
	ctxBuffer.matchCount = 0;
	ctxBuffer.length = 0;
	ctxBuffer.bytesSize = 0;
	//free(rmessage);

}
void decode(  char* data, int size) {
	int oldPos = 0;
	int sn = 0;
	while (1) {
		int bufLeng = ctxBuffer.bufLeng;
		int countLeng = bufLeng + size - oldPos;
		// 可以计算消息长 度，并且没有计算过，或已经计算过但是消息长度为0
		int len = ctxBuffer.matchCount;
		if (countLeng >= TYPE + SN + RESP_CODE + UPD_GROUP) {
			if (bufLeng <= TYPE + SN + RESP_CODE + UPD_GROUP) {
				// 取出标识msg长度的字段
				len = recoverNetworkBytesOrderShort(data,
						oldPos + TYPE + SN + RESP_CODE + UPD_GROUP - bufLeng);
				
			} else if (bufLeng > TYPE + SN + RESP_CODE + UPD_GROUP
					&& bufLeng <= TYPE + SN + RESP_CODE + UPD_GROUP + LENGTH) {
				char by[4];
				int index = 0;
				int i;
				for (i = 0; i < bufLeng - (TYPE + SN + RESP_CODE + UPD_GROUP); i++) {
					by[i] = ctxBuffer.bytes[i + TYPE + SN + RESP_CODE + UPD_GROUP];
					index = i;
				}
				
				for (i = 0; i < TYPE + SN + RESP_CODE + UPD_GROUP + LENGTH - bufLeng; i++) {
					by[++index] = data[i + oldPos];
				}
				// 取出标识msg长度的字段
				len = recoverNetworkBytesOrderInt(by, 0);
			} else {
		
				len = recoverNetworkBytesOrderInt(ctxBuffer.bytes,
						TYPE + SN + RESP_CODE + UPD_GROUP);
				
			}
			
			ctxBuffer.matchCount = len;
			ctxBuffer.length = len + 28;

			if (len > maxLineLength || len < 0) {
				// Out of the limit
			}
            
			if (size + bufLeng - oldPos >= ctxBuffer.length
					&& ctxBuffer.length > 0) { // 判断字节是否可够解码
				if (!ctxBuffer.bytes) {
					
					ctxBuffer.bytes = (char*) malloc(
							ctxBuffer.length * sizeof(char));
					ctxBuffer.bytesSize = ctxBuffer.length;
					
//						printf("data: \n");
//						for (int i = 0; i < ctxBuffer.length; ++i) {
//							printf("%d, ", data[i]);
//						}
//						printf("\n");
					memcpy(ctxBuffer.bytes, data + oldPos, ctxBuffer.length);

				} else if (ctxBuffer.bytesSize != ctxBuffer.length) {
				  
					ctxBuffer.bytes = (char*) realloc(ctxBuffer.bytes,
							ctxBuffer.length * sizeof(char));
					ctxBuffer.bytesSize = ctxBuffer.length;
					memcpy(ctxBuffer.bytes + bufLeng, data + oldPos,
							ctxBuffer.length - bufLeng);
				} else {
					if (ctxBuffer.length - bufLeng > size) {
						memcpy(ctxBuffer.bytes + bufLeng, data + oldPos, size);
					} else {
						memcpy(ctxBuffer.bytes + bufLeng, data + oldPos,
								ctxBuffer.length - bufLeng);
					}
				}
				oldPos += (ctxBuffer.length - bufLeng);
				processStream();	
			
				if (size - oldPos < 44) { // 不够解码器解码
					if (size - oldPos > 0 && !ctxBuffer.bytes) {
						ctxBuffer.bytes = (char*) malloc(
								(size - oldPos) * sizeof(char));
						ctxBuffer.bytesSize = size - oldPos;
						memcpy(ctxBuffer.bytes, data + oldPos, size - oldPos);
						ctxBuffer.bufLeng += (size - oldPos);
					} else {
//							memcpy(ctxBuffer.bytes, data + oldPos,
//									size - oldPos);
//							ctxBuffer.bufLeng += (size - oldPos);
					}
					break;
				}
			} else if (ctxBuffer.length > 0) {
				if (!ctxBuffer.bytes) {
					ctxBuffer.bytes = (char*) malloc(
							ctxBuffer.length * sizeof(char));
					ctxBuffer.bytesSize = ctxBuffer.length;
				} else if (ctxBuffer.bytesSize != ctxBuffer.length) {
					ctxBuffer.bytes = (char*) realloc(ctxBuffer.bytes,
							ctxBuffer.length * sizeof(char));
					ctxBuffer.bytesSize = ctxBuffer.length;
				}

				memcpy(ctxBuffer.bytes + bufLeng, data + oldPos, size - oldPos);

				ctxBuffer.bufLeng += (size - oldPos);
				break;
			}

		}

	}
}


static void get_msg_bytes(char *msg_buf, int *msg_len)
{    
	char typeStr[4];
	memcpy(typeStr, ctxBuffer.bytes, TYPE);

	char snStr[4];
	memcpy(snStr, ctxBuffer.bytes + TYPE, SN);

	char respCodeStr[2];
	memcpy(respCodeStr, ctxBuffer.bytes + TYPE + SN, RESP_CODE);

	char updGroup[4];
	memcpy(updGroup,ctxBuffer.bytes + TYPE + SN + RESP_CODE,UPD_GROUP);

	unsigned char msgBytes[ctxBuffer.matchCount];
	memcpy(msgBytes, ctxBuffer.bytes + TYPE + SN + RESP_CODE + UPD_GROUP + LENGTH,
			ctxBuffer.matchCount);

	char adler32[8];
	memcpy(adler32, ctxBuffer.bytes + TYPE + SN + RESP_CODE + UPD_GROUP + LENGTH + ctxBuffer.matchCount,
			ADLER32);
	uint64_t adl = recoverNetworkBytesOrderLong(adler32, 0);

	uint64_t verifyAdl = getAdler32(msgBytes, ctxBuffer.matchCount);
	if (adl == verifyAdl) {
		printf("adler32 succeed\n");
	} else {
		printf("adler32 failed\n");
		printf("msg adler32 = %ld, verify adler32 = %ld\n", adl, verifyAdl);
	}

	memcpy(msg_buf, msgBytes, ctxBuffer.matchCount);
	*msg_len = ctxBuffer.matchCount;

	free(ctxBuffer.bytes);
	ctxBuffer.bytes = NULL;
	ctxBuffer.bufLeng = 0;
	ctxBuffer.matchCount = 0;
	ctxBuffer.length = 0;
	ctxBuffer.bytesSize = 0;
	//free(rmessage);

}


void decode_msgbuf(  char* data, int size, char *msg_buf, int *msg_len)
{
	int oldPos = 0;
	int sn = 0;
	while (1) {
		int bufLeng = ctxBuffer.bufLeng;
		int countLeng = bufLeng + size - oldPos;
		// 可以计算消息长 度，并且没有计算过，或已经计算过但是消息长度为0
		int len = ctxBuffer.matchCount;
		if (countLeng >= TYPE + SN + RESP_CODE + UPD_GROUP) {
			if (bufLeng <= TYPE + SN + RESP_CODE + UPD_GROUP) {
				// 取出标识msg长度的字段
				len = recoverNetworkBytesOrderShort(data,
						oldPos + TYPE + SN + RESP_CODE + UPD_GROUP - bufLeng);
				
			} else if (bufLeng > TYPE + SN + RESP_CODE + UPD_GROUP
					&& bufLeng <= TYPE + SN + RESP_CODE + UPD_GROUP + LENGTH) {
				char by[4];
				int index = 0;
				int i;
				for (i = 0; i < bufLeng - (TYPE + SN + RESP_CODE + UPD_GROUP); i++) {
					by[i] = ctxBuffer.bytes[i + TYPE + SN + RESP_CODE + UPD_GROUP];
					index = i;
				}
				
				for (i = 0; i < TYPE + SN + RESP_CODE + UPD_GROUP + LENGTH - bufLeng; i++) {
					by[++index] = data[i + oldPos];
				}
				// 取出标识msg长度的字段
				len = recoverNetworkBytesOrderInt(by, 0);
			} else {
		
				len = recoverNetworkBytesOrderInt(ctxBuffer.bytes,
						TYPE + SN + RESP_CODE + UPD_GROUP);
				
			}
			
			ctxBuffer.matchCount = len;
			ctxBuffer.length = len + 28;

			if (len > maxLineLength || len < 0) {
				// Out of the limit
			}
            
			if (size + bufLeng - oldPos >= ctxBuffer.length
					&& ctxBuffer.length > 0) { // 判断字节是否可够解码
				if (!ctxBuffer.bytes) {
					
					ctxBuffer.bytes = (char*) malloc(
							ctxBuffer.length * sizeof(char));
					ctxBuffer.bytesSize = ctxBuffer.length;
					
//						printf("data: \n");
//						for (int i = 0; i < ctxBuffer.length; ++i) {
//							printf("%d, ", data[i]);
//						}
//						printf("\n");
					memcpy(ctxBuffer.bytes, data + oldPos, ctxBuffer.length);

				} else if (ctxBuffer.bytesSize != ctxBuffer.length) {
				  
					ctxBuffer.bytes = (char*) realloc(ctxBuffer.bytes,
							ctxBuffer.length * sizeof(char));
					ctxBuffer.bytesSize = ctxBuffer.length;
					memcpy(ctxBuffer.bytes + bufLeng, data + oldPos,
							ctxBuffer.length - bufLeng);
				} else {
					if (ctxBuffer.length - bufLeng > size) {
						memcpy(ctxBuffer.bytes + bufLeng, data + oldPos, size);
					} else {
						memcpy(ctxBuffer.bytes + bufLeng, data + oldPos,
								ctxBuffer.length - bufLeng);
					}
				}
				oldPos += (ctxBuffer.length - bufLeng);
				get_msg_bytes(msg_buf, msg_len);	
			
				if (size - oldPos < 44) { // 不够解码器解码
					if (size - oldPos > 0 && !ctxBuffer.bytes) {
						ctxBuffer.bytes = (char*) malloc(
								(size - oldPos) * sizeof(char));
						ctxBuffer.bytesSize = size - oldPos;
						memcpy(ctxBuffer.bytes, data + oldPos, size - oldPos);
						ctxBuffer.bufLeng += (size - oldPos);
					} else {
//							memcpy(ctxBuffer.bytes, data + oldPos,
//									size - oldPos);
//							ctxBuffer.bufLeng += (size - oldPos);
					}
					break;
				}
			} else if (ctxBuffer.length > 0) {
				if (!ctxBuffer.bytes) {
					ctxBuffer.bytes = (char*) malloc(
							ctxBuffer.length * sizeof(char));
					ctxBuffer.bytesSize = ctxBuffer.length;
				} else if (ctxBuffer.bytesSize != ctxBuffer.length) {
					ctxBuffer.bytes = (char*) realloc(ctxBuffer.bytes,
							ctxBuffer.length * sizeof(char));
					ctxBuffer.bytesSize = ctxBuffer.length;
				}

				memcpy(ctxBuffer.bytes + bufLeng, data + oldPos, size - oldPos);

				ctxBuffer.bufLeng += (size - oldPos);
				break;
			}

		}

	}
}


