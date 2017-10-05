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

void processStream(void)
{
	unsigned char typeStr[4];
	unsigned char snStr[4];
	unsigned char updGroup[4];
	unsigned char respCodeStr[2];
	unsigned char msgBytes[ctxBuffer.matchCount];
	unsigned char adler32[8];

	memcpy(typeStr, ctxBuffer.bytes, TYPE);
	memcpy(snStr, ctxBuffer.bytes + TYPE, SN);
	memcpy(updGroup,ctxBuffer.bytes + TYPE + SN ,UPD_GROUP);
	memcpy(respCodeStr, ctxBuffer.bytes + TYPE + SN + UPD_GROUP, RESP_CODE);
	memcpy(msgBytes, ctxBuffer.bytes + TYPE + SN + UPD_GROUP + RESP_CODE + LENGTH,
			ctxBuffer.matchCount);
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

	unsigned int type = recoverNetworkBytesOrderInt(typeStr, 0);
	switch (type) {
	case HARDWARE_CONNECT:
		DLu(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_START_PLAY:
		SPlay(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_PLAY_CONTROL:
		PControl(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_DL_LOCAL:
		DLocal(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_RELEASE_VOICE:
		RVoice(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_SHARE_VOICE:
		SVoice(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_DELETE_GROUP_VOICE:
		DGVoice(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_VERSION_UPDATE:
		VUpdate(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_WIFI_GET:
		GWifi(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_SLEEP_MODE:
		SMode(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_CLOSE:
		HwClose(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_WIFI_SET:
		Swifi(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_CAPACITY:
		Cap(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_ADD_CUSTOM:
		ACVoice(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_DELETE_CUSTOM:
		DCVoice(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_CHANGE_LIST_ORDER:
		CLOrder(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_CACHE_VOICE:
		CVoice(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_DELETE_DOWNLOADED:
		DDVoice(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_DELETE_SD_VOICE:
		DSDVoice(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_ADD_DISCUSS:
		ADiscuss(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_STATUS:
		HStatus(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_CHANGE_NAME:
		CName(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_ALTER_MASTER:
		Amaster(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_PLAY_MODE:
		Pmode(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_ADD_VOICE:
		Avoice(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_DELETE_VOICE:
		Dvoice(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_REGISTER:
		Zce(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_SIGNOFF:
		Zxiao(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_MONITOR_OPEN:
		Mopen(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_MONITOR_CLOSE:
		Mclose(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_TALKBACK_OPEN:
		Topen(msgBytes,ctxBuffer.matchCount);
		break;
	case HARDWARE_TALKBACK_CLOSE:
		Tclose(msgBytes,ctxBuffer.matchCount);
		break;
	default:
		break;
	}

	free(ctxBuffer.bytes);
	ctxBuffer.bytes = NULL;
	ctxBuffer.bufLeng = 0;
	ctxBuffer.matchCount = 0;
	ctxBuffer.length = 0;
	ctxBuffer.bytesSize = 0;
}

void decode(  char* data, int size)
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


