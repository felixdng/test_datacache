#include <stdio.h>

#include "net.h"
#include "datacache.h"
#include "protob/HwPullDiscussResp.pb.h"
#include "socktcp.h"

#include "protob/ConnectReq.pb.h"
#include "Encoder.h"
#include "Decoder.h"
#include "protob/pb_encode.h"
#include "protob/pb_decode.h"
#include "protob/pb_common.h"
#include <protob/HwInfoReq.pb.h>
#include <protob/HwInfoResp.pb.h>

static int req_conn(socktcp_t *sock)
{
#if 1
	unsigned char buf[1280];
	int len, status;
	ConnectReq message={};
	pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));

	message.isReset=0;
	message.versionCode=1;
	strcpy(message.serialNum, "862855028033177");
	status = pb_encode(&stream, ConnectReq_fields , &message);
	len = stream.bytes_written;

	if (!status) {
		printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 1;
	}

	//send_msg(HARDWARE_CONNECT, buf, len);
	{
		char *dest = NULL;
		static int sn = 1;
	
		dest = (char *)malloc((60 + len) * sizeof(char));
		if (dest == NULL) {
			printf("malloc failed!\n");
			return -1;
		}
	
		encode(0xf011f001, sn++, buf, len, dest);
		sock->send(0xf011f001, dest, 60+len);
		//if (send(sock_fd, dest, 60 + len, 0) < 0) {
			//LOG_ERR("socket send failed!\n");
			//free(dest);
			//return -1;
		//}
	
		free(dest);
	}

#endif
}

static int req_dev_info(socktcp_t *sock)
{
	int ret;
	unsigned char buf[128];
	int msg_len;
	int i;
	char *destdata = NULL;

	HwInfoReq *message = (HwInfoReq *)malloc(sizeof(HwInfoReq));
	if (message == NULL) {
		printf("malloc failed.\n");
		return -1;
	}

	pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));

	//for (i = 0; i < 16; i++)
		//message->serialNum[i] = msgid[i];
	strcpy(message->serialNum, "862855028033177");

	ret = pb_encode(&stream, HwInfoReq_fields , message);
	msg_len = stream.bytes_written;
	if (!ret) {
		printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));  
		goto err1;
	}

	destdata = (char *)malloc((60 + msg_len) * sizeof(char));
	if (destdata == NULL) {
		printf("malloc failed.\n");
		goto err1;
	}

	encode(0xf011f082, 1, buf, msg_len, destdata);
	sock->send(0xf011f082, destdata, 60+msg_len);
	//ret = send(sockee, destdata, 60 + msg_len, 0);
	//if (ret < 0) {
		//printf("socket send failed!\n");
		//goto err2;
	//}

	printf("## %s() return OK.\n", __func__);

	free(destdata);
	free(message);

	return 0;

err2:
	free(destdata);
err1:
	free(message);
	return ret;
}


int main(void)
{
//	init_sqlite();
//	datacache_t dc;
//	strcpy(dc.dev_info.nick_name,"xiaom");

//    socket_connect();
//    create_sock_receive_pthread();
//	init_datacache(&dc);

	socktcp_t sock;
	socktcp_init(&sock);

	unsigned char buf[128];
	// size_t message_length;
	bool status;
	int i;
	int len,length;
	char *destData = NULL;
#if 0
		{
	ConnectReq *message=malloc(sizeof( ConnectReq));
	pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));

	message->isReset=0;
	message->versionCode=1;
	for(i=0;i<16;i++)
		message->serialNum[i]=msgid[i];
	status = pb_encode(&stream, ConnectReq_fields , message);
	len = stream.bytes_written;
	//printf("protobuf len:%d",len);
	//printf("buf:%s",buf);
	if (!status) {
		printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 1;
	}

	destData = (char*) malloc((60 + len) * sizeof(char));
	encode(0xf011f001, 1, buf,len, destData);
	//printf("destDATA:%s\n",destData);
		}
#endif
#if 1
	//sock.send(0xf011f001, destData, 60+len);
	//free(destData);
#else
	msginfo_t msg;
	printf("&msg: %p\n", &msg);
	sock.send_r(0xf011f001, destData, 60+len, &msg);
	printf("[%s] msglen: %d\n", __func__, msg.length);
	decode(msg.msg, msg.length);
	printf("dada--------\n");
#endif
	sleep(2);
	req_conn(&sock);
	sleep(1);
	req_dev_info(&sock);

    while (1) {
        sleep(5);
		printf("lalalala--------------\n");
    }

    close(sockee);
    return 0;
}
