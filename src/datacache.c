#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include "Encoder.h"
#include "Decoder.h"
#include <protob/pb_encode.h>
#include <protob/pb_decode.h>
#include <protob/pb_common.h>
#include <protob/simple.pb.h>
#include <protob/HwInfoReq.pb.h>
#include <protob/HwInfoResp.pb.h>
#include <protob/HwPullDiscussReq.pb.h>
#include <protob/HwPullDiscussResp.pb.h>
#include <protob/HwPullVoiceReq.pb.h>
#include <protob/HwPullVoiceResp.pb.h>
#include <protob/HwDLLocalStatusReq.pb.h>
#include <protob/HwDLLocalStatusResp.pb.h>

#include <protob/ConnectReq.pb.h>


#include "sqlite3.h"

extern char msgid[16];
extern int sockee;

enum sql_exec_t {
	SQL_DEFAULT = 0,
	SQL_INSERT,
	SQL_SELECT,
	SQL_UPDATE,
	SQL_DELETE,
};

struct datacache {
	sqlite3 *db;
	char *filepath;
	unsigned char flag;
	enum sql_exec_t sql_exec;
	unsigned int updflag;
	int updtype;
};

#define DATABASE_PATH "datacache.db"
static struct datacache *pcache = NULL;

static const char *SQL_CREATE[] = {
	"create table if not exists devInfo(serialNum nvarchar(100), nickname nvarchar(100), updFlags integer)",
	"create table if not exists groupDiscuss(discussId integer, discussContent nvarchar(100), createTime integer)",
	"create table if not exists groupShare(voiceId integer, voicePath nvarchar(100), shareTime integer, voiceType integer)",
	"create table if not exists userDefinedList(relType integer, relId nvarchar(100), allCount integer)",
	"create table if not exists voiceList(relType integer, relId nvarchar(100), allCount integer)",
	"create table if not exists albumList(voiceId integer, netPath nvarchar(100), localPath nvarchar(100), \
				voiceName nvarchar(100))"
};

static int request_dev_info(void)
{
	int ret;
	unsigned char buf[128];
	int len;
	int i;
	char *destdata = NULL;
	char *errmsg;

	printf(">>>>>>>>>>>>>>> request_dev_info() <<<<<<<<<<<<<<<<<\n");

	HwInfoReq *message = (HwInfoReq *)malloc(sizeof(HwInfoReq));
	if (message == NULL) {
		printf("malloc failed.\n");
		return -1;
	}

	memset(buf, 0, sizeof(buf));
	pb_ostream_t output = pb_ostream_from_buffer(buf, sizeof(buf));

	for (i = 0; i < 16; i++)
		message->serialNum[i] = msgid[i];

	ret = pb_encode(&output, HwInfoReq_fields , message);
	len = output.bytes_written;
	if (!ret) {
		printf("Encoding failed: %s\n", PB_GET_ERROR(&output));  
		goto err1;
	}

	printf(">>>>>>>>>>>>>>> pb_encode: ", buf);
	for (i = 0; i < len; i++) printf("%u",buf[i]);
	printf("<<<<<<<<<<<<<<<<<\n");

	destdata = (char*) malloc((60 + len) * sizeof(char));
	if (destdata == NULL) {
		printf("malloc failed.\n");
		goto err1;
	}

	memset(destdata, 0, sizeof(destdata));
	encode(0xf011f082, 1, buf, len, destdata);
	ret = send(sockee, destdata, 60 + len, 0);
	if (ret < 0) {
		printf("socket send failed!\n");
		goto err2;
	}
	printf(">>>>>>>>>>>>>>> request_dev_info OK. <<<<<<<<<<<<<<<<<\n");

	uint8_t recvbuff[1024];
	memset(recvbuff,0,sizeof(recvbuff));
	ret = recv(sockee, recvbuff, sizeof(recvbuff), 0);
	if (ret < 0) {
		printf("recv error\n");
		goto err2;
	}
	
	char msgBytes[1024];
	int msglen = 0;
	bzero(msgBytes, sizeof(msgBytes));
	decode_msgbuf(recvbuff, ret, msgBytes, &msglen);

	HwInfoResp *rmessage = (HwInfoResp *)malloc(sizeof(HwInfoResp));
	if (rmessage == NULL) {
		printf("malloc failed.\n");
		goto err2;
	}

	pb_istream_t input = pb_istream_from_buffer(msgBytes, msglen);
	ret = pb_decode(&input, HwInfoResp_fields, rmessage);
	if (!ret) {
		printf("Decoding failed: %s\n", PB_GET_ERROR(&input));
		goto err3;
	}
	printf(">>>>>>>>>>>>>>> nickName: %s <<<<<<<<<<<<<<<<<\n", rmessage->nickName);

	char sql[128] = {'\0'};
	switch (pcache->sql_exec) {
	case SQL_INSERT:
		snprintf(sql, sizeof(sql), "insert into devInfo(serialNum,nickName,updFlags) values('%s','%s',0)", msgid, rmessage->nickName);
		break;
	case SQL_UPDATE:
		snprintf(sql, sizeof(sql), "update devInfo set nickname='%s' where serialNum='%s'", rmessage->nickName, msgid);
		break;
	default:
		break;
	}
	pcache->sql_exec = SQL_DEFAULT;

	ret = sqlite3_exec(pcache->db, sql, NULL, NULL, &errmsg);
	if (SQLITE_OK != ret) {
		printf("sqlite exec failed : %d: %s\n", ret, errmsg);
		goto err3;
	}

	free(rmessage);
	free(destdata);
	free(message);

	return 0;

err3:
	free(rmessage);
err2:
	free(destdata);
err1:
	free(message);
	return ret;
}

static bool group_discuss_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	char *errmsg;

	printf(">>>>>>>>>>>>>>> group_discuss_callback() <<<<<<<<<<<<<<<<<\n");

    HwPullDiscussResp_ObjDiscuss *obj = (HwPullDiscussResp_ObjDiscuss *)malloc(sizeof(HwPullDiscussResp_ObjDiscuss));
	if (obj == NULL) {
		printf("malloc failed\n");
		return false;
	}
    
    if (!pb_decode(stream, HwPullDiscussResp_ObjDiscuss_fields, obj)) {
		fprintf(stderr, "Decode failed: %s\n", PB_GET_ERROR(stream));
		free(obj);
        return false;
    }

	printf(">>>>>>>>>>>>>>> decode group discuss: %d %s %d\n", obj->discussId, obj->discussContent, obj->createTime);

    char sql[128] = {'\0'};
	snprintf(sql, sizeof(sql), "insert into groupDiscuss(discussId,discussContent,createTime) values(%d,'%s',%d)", \
			obj->discussId, obj->discussContent, obj->createTime);
	if (SQLITE_OK != sqlite3_exec(pcache->db, sql, NULL, NULL, &errmsg)) {
		printf("sqlite exec failed : %s\n", errmsg);
		free(obj);
		return false;
	}

	free(obj);
    return true;
}

static int request_group_discuss(void)
{
	int ret;
	unsigned char buf[128];
	int len;
	int i;
	char *destdata = NULL;

	printf(">>>>>>>>>>>>>>> request_group_discuss() <<<<<<<<<<<<<<<<<\n");

	HwPullDiscussReq *message = (HwPullDiscussReq *)malloc(sizeof(HwPullDiscussReq));
	if (message == NULL) {
		printf("malloc failed.\n");
		return -1;
	}

	memset(buf, 0, sizeof(buf));
	pb_ostream_t output = pb_ostream_from_buffer(buf, sizeof(buf));

	for (i = 0; i < 16; i++)
		message->serialNum[i] = msgid[i];
	message->has_reqTime = 0;
	message->reqTime = 0;
	
	ret = pb_encode(&output, HwPullDiscussReq_fields , message);
	len = output.bytes_written;
	if (!ret) {
		printf("Encoding failed: %s\n", PB_GET_ERROR(&output));  
		goto err1;
	}

	printf(">>>>>>>>>>>>>>> pb_encode: ", buf);
	for (i = 0; i < len; i++) printf("%u",buf[i]);
	printf("<<<<<<<<<<<<<<<<<\n");

	destdata = (char*) malloc((60 + len) * sizeof(char));
	if (destdata == NULL) {
		printf("malloc failed.\n");
		goto err1;
	}

	memset(destdata, 0, sizeof(destdata));
	encode(0xf011f037, 1, buf, len, destdata);
	ret = send(sockee, destdata, 60 + len, 0);
	if (ret < 0) {
		printf("socket send failed!\n");
		goto err2;
	}
	printf(">>>>>>>>>>>>>>> request_group_discuss OK. <<<<<<<<<<<<<<<<<\n");

	uint8_t recvbuff[1024];
	memset(recvbuff,0,sizeof(recvbuff));
	ret = recv(sockee, recvbuff, sizeof(recvbuff), 0);
	if (ret < 0) {
		printf("recv error\n");
		goto err2;
	}
		
	char msgBytes[1024];
	int msglen = 0;
	bzero(msgBytes, sizeof(msgBytes));
	decode_msgbuf(recvbuff, ret, msgBytes, &msglen);

	printf(">>>>>>>>>>>>>> msgbyte:");
	for (i = 0; i < msglen; i++) printf("%x",msgBytes[i]);
	printf(" <<<<<<<<<<<<<<<<<\n");

	HwPullDiscussResp *response = (HwPullDiscussResp *)malloc(sizeof(HwPullDiscussResp));
	if (response == NULL) {
		printf("malloc failed.\n");
		goto err2;
	}

	pb_istream_t input = pb_istream_from_buffer(msgBytes, msglen);
	response->objDiscuss.funcs.decode = &group_discuss_callback;
	if (!pb_decode(&input, HwPullDiscussResp_fields, response)) {
		fprintf(stderr, "Decode failed: %s\n", PB_GET_ERROR(&input));
		goto err3;
	}

	printf(">>>>>>>>>>>>>>> thisReqTime: %ld\n", response->thisReqTime);

	free(response);
	free(destdata);
	free(message);

	return 0;

err3:
	free(response);
err2:
	free(destdata);
err1:
	free(message);
	return ret;
}

static bool group_share_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	char *errmsg;

	HwPullVoiceResp_ObjVoice *obj = (HwPullVoiceResp_ObjVoice *)malloc(sizeof(HwPullVoiceResp_ObjVoice));
	if (obj == NULL) {
		printf("malloc failed\n");
		return false;
	}
    
    if (!pb_decode(stream, HwPullVoiceResp_ObjVoice_fields, obj)) {
		fprintf(stderr, "Decode failed: %s\n", PB_GET_ERROR(stream));
		free(obj);
        return false;
    }

	printf("decode group share: %d %s %d %d\n", obj->voiceId, obj->voicePath, obj->shareTime, obj->voiceType);

    char sql[128] = {'\0'};
	snprintf(sql, sizeof(sql), "insert into groupShare(voiceId,voicePath,shareTime,voiceType) values(%d,'%s',%d,%d)", \
			obj->voiceId, obj->voicePath, obj->shareTime, obj->voiceType);
	if (SQLITE_OK != sqlite3_exec(pcache->db, sql, NULL, NULL, &errmsg)) {
		printf("sqlite exec failed : %s\n", errmsg);
		free(obj);
		return false;
	}

	free(obj);	
	return true;
}

static int request_group_share(void)
{
	int ret;
	unsigned char buf[BUFSIZ];
	int len;
	char *destdata = NULL;

	HwPullVoiceReq *message = (HwPullVoiceReq *)malloc(sizeof(HwPullVoiceReq));
	if (message == NULL) {
		printf("malloc failed.\n");
		return -1;
	}

	memset(message->serialNum, '\0', sizeof(message->serialNum));
	memcpy(message->serialNum, msgid, sizeof(msgid));
	message->has_reqTime = 0;
	message->reqTime = 0;

	bzero(buf, BUFSIZ);
	pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
	len = stream.bytes_written;
	ret = pb_encode(&stream, HwPullVoiceReq_fields , message);
	if (!ret) {
		printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));  
		goto err1;
	}

	destdata = (char*) malloc((60 + len) * sizeof(char));
	if (destdata == NULL) {
		printf("malloc failed.\n");
		goto err1;
	}

	memset(destdata, 0, sizeof(destdata));
	encode(0xf011f039, 1, buf, len, destdata);
	ret = send(sockee, destdata, sizeof(destdata), 0);
	if (ret < 0) {
		printf("socket send failed!\n");
		goto err2;
	}
	printf("%---------------------- request_group_share OK. ----------------------%\n");

	uint8_t recvbuff[1024];
	memset(recvbuff,0,sizeof(recvbuff));
	ret = recv(sockee, recvbuff, sizeof(recvbuff), 0);
	if (ret < 0) {
		printf("recv error\n");
		goto err2;
	}

	char msgBytes[1024];
	int msglen = 0;
	bzero(msgBytes, sizeof(msgBytes));
	decode_msgbuf(recvbuff, ret, msgBytes, &msglen);

	HwPullVoiceResp *response = (HwPullVoiceResp *)malloc(sizeof(HwPullVoiceResp));
	if (response == NULL) {
		printf("malloc failed\n");
		goto err2;
	}

	pb_istream_t input = pb_istream_from_buffer(msgBytes, msglen);
	response->objVoice.funcs.decode = &group_share_callback;
	if (!pb_decode(&input, HwPullVoiceResp_fields, response)) {
		fprintf(stderr, "Decode failed: %s\n", PB_GET_ERROR(&input));
		goto err3;
	}

	printf("%---------------------- thisReqTime: %d\n", response->thisReqTime);

	free(response);
	free(destdata);
	free(message);

	return 0;

err3:
	free(response);
err2:
	free(destdata);
err1:
	free(message);
	return ret;
}

static int request_user_defined_list(void)
{
	printf("%---------------------- request_user_defined_list OK. ----------------------%\n");
	return 0;
}

static int request_voice_list(void)
{
	/* get sound list */

	/* update sound list */

	/* start download */

	/* update cache */

	/* push sound list status */

	printf("%---------------------- request_voice_list OK. ----------------------%\n");
	return 0;
}

static bool dlingvoice_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	HwDLLocalStatusResp_ObjVoice *obj = (HwDLLocalStatusResp_ObjVoice *)malloc(sizeof(HwDLLocalStatusResp_ObjVoice));
	if (obj == NULL) {
		printf("malloc failed\n");
		return false;
	}
    
    if (!pb_decode(stream, HwDLLocalStatusResp_ObjVoice_fields, obj)) {
		fprintf(stderr, "Decode failed: %s\n", PB_GET_ERROR(stream));
		free(obj);
        return false;
    }

    printf("%d %s %s %s\n", obj->voiceId, obj->netPath, obj->localPath, obj->voiceName);

	/* save sqlite */

	free(obj);

	return true;
}

static bool pausevoice_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	HwDLLocalStatusResp_ObjVoice *obj = (HwDLLocalStatusResp_ObjVoice *)malloc(sizeof(HwDLLocalStatusResp_ObjVoice));
	if (obj == NULL) {
		printf("malloc failed\n");
		return false;
	}
    
    if (!pb_decode(stream, HwDLLocalStatusResp_ObjVoice_fields, obj)) {
		fprintf(stderr, "Decode failed: %s\n", PB_GET_ERROR(stream));
		free(obj);
        return false;
    }

    printf("%d %s %s %s\n", obj->voiceId, obj->netPath, obj->localPath, obj->voiceName);

	/* save sqlite */

	free(obj);

	return true;
}

static int request_album_list(void)
{
	int ret;
	unsigned char buf[BUFSIZ];
	int len;
	char *destdata = NULL;

	HwDLLocalStatusReq *message = (HwDLLocalStatusReq *)malloc(sizeof(HwDLLocalStatusReq));
	if (message == NULL) {
		printf("malloc failed.\n");
		return -1;
	}

	memset(message->serialNum, '\0', sizeof(message->serialNum));
	memcpy(message->serialNum, msgid, sizeof(msgid));

	bzero(buf, sizeof(buf));
	pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
	len = stream.bytes_written;
	ret = pb_encode(&stream, HwDLLocalStatusReq_fields , message);
	if (!ret) {
		printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));  
		goto err1;
	}

	destdata = (char*) malloc((60 + len) * sizeof(char));
	if (destdata == NULL) {
		printf("malloc failed.\n");
		goto err1;
	}

	memset(destdata, 0, sizeof(destdata));
	encode(0xf011f069, 1, buf, len, destdata);
	ret = send(sockee, destdata, sizeof(destdata), 0);
	if (ret < 0) {
		printf("socket send failed!\n");
		goto err2;
	}
	printf("%---------------------- request_album_list OK. ----------------------%\n");

	uint8_t recvbuff[1024];
	memset(recvbuff,0,sizeof(recvbuff));
	ret = recv(sockee, recvbuff, sizeof(recvbuff), 0);
	if (ret < 0) {
		printf("recv error\n");
		goto err2;
	}

	char msgBytes[1024];
	int msglen = 0;
	bzero(msgBytes, sizeof(msgBytes));
	decode_msgbuf(recvbuff, ret, msgBytes, &msglen);

	HwDLLocalStatusResp *response = (HwDLLocalStatusResp *)malloc(sizeof(HwDLLocalStatusResp));
	if (response == NULL) {
		printf("malloc failed\n");
		goto err2;
	}

	pb_istream_t input = pb_istream_from_buffer(msgBytes, msglen);
	response->dlingVoice.funcs.decode = &dlingvoice_callback;
	response->pauseVoice.funcs.decode = &pausevoice_callback;
	if (!pb_decode(&input, HwDLLocalStatusResp_fields, response)) {
		fprintf(stderr, "Decode failed: %s\n", PB_GET_ERROR(&input));		
		goto err3;
	}

	printf("%---------------------- dledVoiceId: %d\n", response->dledVoiceId);

	free(response);
	free(destdata);
	free(message);

	return 0;

err3:
	free(response);
err2:
	free(destdata);
err1:
	free(message);
	return ret;
}

static int init_sqlite(struct datacache *dc)
{
	int status = 0;
	int i;
	char *errmsg = NULL;

	/* open database */
	status = sqlite3_open(dc->filepath, &dc->db);
	if (SQLITE_OK != status) {
		printf("sqlite> open failed : %d\n", status);
		return status;
	}

	if (dc->flag != 0)
		return 0;
	/* create table */
	for (i = 0; i < sizeof(SQL_CREATE)/sizeof(SQL_CREATE[0]); ++i) {
		status = sqlite3_exec(dc->db, SQL_CREATE[i], NULL, NULL, &errmsg);
		if (SQLITE_OK != status) {
			printf("sqlite> create table failed : %d %s\n", status, errmsg);
			goto err;
		}
	}
	dc->flag = 1;
	
	return 0;

err:
	sqlite3_close(dc->db);
	return status;
}

static int parse_flag(unsigned int flag)
{
	int ret = 0;
	unsigned char i;

	for (i = 31; i >= 26; --i) {
		if (flag & (1<<i))
			break;
		++ret;
	}

	return ret;
}

int update_cache(unsigned int updflag)
{
	char *errmsg;
	int ret;
	int updtype;

	if (pcache->updtype != (-1)) {
		printf("do not need to update.\n");
		return 0;
	}
	
	pcache->updflag = updflag;

	char sql[128] = {'\0'};
	snprintf(sql, sizeof(sql), "update devInfo set updFlags=%d where serialNum='%s'", updflag, msgid);
	ret = sqlite3_exec(pcache->db, sql, NULL, NULL, &errmsg);
	if (SQLITE_OK != ret) {
		printf("sqlite exec failed : %d: %s\n", ret, errmsg);
		return ret;
	}

	updtype = parse_flag(pcache->updflag);
	pcache->sql_exec = SQL_UPDATE;
	switch (updtype) {
	case 0:
		ret = request_dev_info();
		break;
	case 1:
		ret = request_group_discuss();
		break;
	case 2:
		ret = request_group_share();
		break;
	case 3:
		ret = request_user_defined_list();
		break;
	case 4:
		ret = request_voice_list();
		break;
	case 5:
		ret = request_album_list();
		break;
	}

	if (ret == 0) {
		printf("request update [%d] OK.\n", updtype);
		pcache->updtype = updtype;
	} else {
		printf("request update [%d] failed!\n", updtype);
		pcache->updtype = -1;
	}

	return 0;
}

static int request_init_cache(void)
{
	int ret;

	pcache->sql_exec = SQL_INSERT;
	ret = request_dev_info();
	if (ret != 0)
		return ret;
/*
	pcache->sql_exec = SQL_INSERT;
	ret = request_group_discuss();
	if (ret != 0)
		return ret;

	pcache->sql_exec = SQL_INSERT;
	ret = request_group_share();
	if (ret != 0)
		return ret;

	pcache->sql_exec = SQL_INSERT;
	ret = request_user_defined_list();
	if (ret != 0)
		return ret;

	pcache->sql_exec = SQL_INSERT;
	ret = request_voice_list();
	if (ret != 0)
		return ret;

	pcache->sql_exec = SQL_INSERT;
	ret = request_album_list();
	if (ret != 0)
		return ret;
*/
	return 0;
}

int init_datacache(void)
{
	int ret = -1;

	pcache = (struct datacache *)malloc(sizeof(struct datacache));
	if (NULL == pcache) {
		printf("malloc failed\n");
		goto err1;
	}

	pcache->filepath = DATABASE_PATH;
	pcache->db = NULL;
	pcache->flag = 0;
	pcache->sql_exec = SQL_DEFAULT;
	pcache->updtype = -1;

	/* open database and create tables */	
	ret = init_sqlite(pcache);
	if (ret != 0) {
		printf("init sqlite failed!\n");
		goto err2;
	}

	ret = request_init_cache();
	if (ret != 0) {
		printf("request init cache failed!\n");
		goto err2;
	}

	printf("\n##---------------- % init datacache OK. % --------------------##\n");
	
	return 0;

err2:
	free(pcache);
err1:
	return ret;
}

void clear_datacache(void)
{
	/* delete group discuss and sound */

	/* save sound list and file */

	/* save album list and file */
}

void exit_datacache(void)
{
	clear_datacache();
	sqlite3_close(pcache->db);
	free(pcache);
}

