#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include "Encoder.h"
//#include "Decoder.h"
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
#include <protob/HwPullVoiceListReq.pb.h>
#include <protob/HwPullVoiceListResp.pb.h>
#include <protob/HwDLLocalStatusReq.pb.h>
#include <protob/HwDLLocalStatusResp.pb.h>

#include <protob/ConnectReq.pb.h>

#include "datacache.h"

#define DATABASE_PATH "datacache.db"

extern char msgid[16];
extern int sockee;

datacache_t datacache = DATACACHE_ZERO;

inline void datacache_zero(datacache_t *data)
{
	data->db = NULL;
	data->filepath = NULL;
	data->upd_flag = 0;
	memset(data->dev_info.serial_num, '\0', sizeof(data->dev_info.serial_num));
	memset(data->dev_info.nick_name, '\0', sizeof(data->dev_info.nick_name));
	data->dev_info.upd_flag = 0;
}

static const char *SQL_CREATE[] = {
	"create table if not exists devInfo(serialNum nvarchar(100), nickName nvarchar(100), updFlags integer)",
	"create table if not exists groupDiscuss(discussId integer, discussContent nvarchar(100), createTime integer)",
	"create table if not exists groupShare(voiceId integer, voicePath nvarchar(100), shareTime integer, voiceType integer)",
	"create table if not exists customList(relType integer, relId nvarchar(100), allCount integer)",
	"create table if not exists voiceList(voiceId integer, voiceName nvarchar(100), voicePath nvarchar(100), \
										voicePic nvarchar(100), voiceLength integer, voiceDesc nvarchar(100), \
										voiceSize integer, downloadTime integer, isCustom bool, appVoiceType integer)",
	"create table if not exists albumList(albumId integer, albumName nvarchar(100), albumCover nvarchar(100), \
										albumDesc nvarchar(100), albumVoiceCount integer, dlStatus integer)"
};

static int sql_table_rows_callback(void *para, int n_column,
	char **column_value, char **column_name)
{
	int *count = (int *)para;
	*count = atoi(column_value[0]);
	return 0;
}

static int sql_table_rows(const char *table, const sqlite3 *db)
{
	int count = -1;
	char sql[64] = {'\0'};
	char *errmsg;

	sprintf(sql, "select count(*) from %s", table);
	if (SQLITE_OK != sqlite3_exec(db, sql, sql_table_rows_callback, &count, &errmsg)) {
		printf("sqlite exec failed : %s\n", errmsg);
		return -1;
	}

	return count;
}

int devinfo_exec_sql(dc_sql_exec_type_t type, devinfo_fields_t field, dc_dev_info_t *devinfo)
{
	int ret;
	char sql[128] = {'\0'};
	char *errmsg;

	if (type == SQL_INSERT) {
		snprintf(sql, sizeof(sql), "insert into devInfo(serialNum,nickName,updFlags) values('%s','%s',%d)", \
			devinfo->serial_num, devinfo->nick_name, datacache.upd_flag);
	} else if (type == SQL_UPDATE) {
		switch (field) {
		case DEVINFO_DEFAULT:
			snprintf(sql, sizeof(sql), "update devInfo set nickName='%s',updFlags=%d where serialNum='%s'", \
				devinfo->nick_name, datacache.upd_flag, devinfo->serial_num);
			break;
		case DEVINFO_NICKNAME:
			snprintf(sql, sizeof(sql), "update devInfo set nickName='%s' where serialNum='%s'", \
				devinfo->nick_name, devinfo->serial_num);
			break;
		case DEVINFO_UPDFLAGS:
			snprintf(sql, sizeof(sql), "update devInfo set updFlags=%d where serialNum='%s'", \
				datacache.upd_flag, devinfo->serial_num);
			break;
		}
	} else if (type == SQL_SELECT) {
		switch (field) {
		case DEVINFO_DEFAULT:
			snprintf(sql, sizeof(sql), "select * from devInfo where serialNum='%s'", devinfo->serial_num);
			break;
		case DEVINFO_NICKNAME:
			snprintf(sql, sizeof(sql), "select nickName from devInfo where serialNum='%s'", \
				devinfo->serial_num);
			break;
		case DEVINFO_UPDFLAGS:
			snprintf(sql, sizeof(sql), "select updFlags from devInfo where serialNum='%s'", \
				devinfo->serial_num);
			break;
		}
	} else {
		printf("invalid sql exec type.\n");
		return -1;
	}

	if (type != SQL_SELECT) {
		ret = sqlite3_exec(datacache.db, sql, NULL, NULL, &errmsg);
		if (SQLITE_OK != ret) {
			printf("sqlite exec failed : %d: %s\n", ret, errmsg);
			return ret;
		}
	} else {
		char **result = NULL;
		int nrow, ncolumn;

		ret = sqlite3_get_table(datacache.db, sql, &result, &nrow, &ncolumn, &errmsg);
		if (SQLITE_OK != ret) {
			printf("sqlite3_get_table failed : %d: %s\n", ret, errmsg);
			sqlite3_free_table(result);
			return ret;
		}

		if (field == DEVINFO_DEFAULT) {
			strcpy(devinfo->nick_name, result[ncolumn+1]);
			devinfo->upd_flag = atoi(result[ncolumn+2]);
		} else if (field == DEVINFO_NICKNAME) {
			strcpy(devinfo->nick_name, result[ncolumn]);
		} else if (field == DEVINFO_UPDFLAGS) {
			devinfo->upd_flag = atoi(result[ncolumn]);
		}

		sqlite3_free_table(result);
	}

	return 0;
}

int groupdiscuss_exec_sql(dc_sql_exec_type_t type, dc_group_discuss_t *obj)
{
	int ret;
	char *errmsg;
	char sql[128] = {'\0'};

	switch (type) {
	case SQL_INSERT:
		snprintf(sql, sizeof(sql), "insert into groupDiscuss(discussId,discussContent,createTime) values(%d,'%s',%d)", \
			obj->discussId, obj->discussContent, obj->createTime);
		break;
	case SQL_DELETE:
		if (obj == NULL) {
			strcpy(sql, "delete from groupDiscuss");
		} else {
			snprintf(sql, sizeof(sql), "delete from groupDiscuss where discussId=%d", obj->discussId);
		}
		break;
	case SQL_SELECT:
		snprintf(sql, sizeof(sql), "select * from groupDiscuss where discussId=%d", obj->discussId);
		break;
	default:
		break;
	}

	if (type != SQL_SELECT) {
		if (SQLITE_OK != sqlite3_exec(datacache.db, sql, NULL, NULL, &errmsg)) {
			printf("sqlite exec failed : %s\n", errmsg);
			return -1;
		}
	} else {
		char **result = NULL;
		int nrow, ncolumn;

		ret = sqlite3_get_table(datacache.db, sql, &result, &nrow, &ncolumn, &errmsg);
		if (SQLITE_OK != ret) {
			printf("sqlite3_get_table failed : %d: %s\n", ret, errmsg);
			sqlite3_free_table(result);
			return -1;
		}

		memcpy(obj->discussContent, result[ncolumn+1], sizeof(obj->discussContent));
		obj->createTime = atoi(result[ncolumn+2]);

		sqlite3_free_table(result);
	}

	return 0;
}

int groupshare_exec_sql(dc_sql_exec_type_t type, dc_group_voice_t *obj)
{
	int ret;
	char *errmsg;
	char sql[128] = {'\0'};

	switch (type) {
	case SQL_INSERT:
		snprintf(sql, sizeof(sql), "insert into groupShare(voiceId,voicePath,shareTime,voiceType) values(%d,'%s',%d,%d)", \
			obj->voiceId, obj->voicePath, obj->shareTime, obj->voiceType);
		break;
	case SQL_DELETE:
		if (obj == NULL) {
			strcpy(sql, "delete from groupShare");
		} else {
			snprintf(sql, sizeof(sql), "delete from groupShare where voiceId=%d", obj->voiceId);
		}
		break;
	case SQL_SELECT:
		snprintf(sql, sizeof(sql), "select * from groupShare where voiceId=%d", obj->voiceId);
		break;
	default:
		break;
	}

	if (type != SQL_SELECT) {
		if (SQLITE_OK != sqlite3_exec(datacache.db, sql, NULL, NULL, &errmsg)) {
			printf("sqlite exec failed : %s\n", errmsg);
			return -1;
		}
	} else {
		char **result = NULL;
		int nrow, ncolumn;

		ret = sqlite3_get_table(datacache.db, sql, &result, &nrow, &ncolumn, &errmsg);
		if (SQLITE_OK != ret) {
			printf("sqlite3_get_table failed : %d: %s\n", ret, errmsg);
			sqlite3_free_table(result);
			return -1;
		}

		memcpy(obj->voicePath, result[ncolumn+1], sizeof(obj->voicePath));
		obj->shareTime = atoi(result[ncolumn+2]);
		obj->voiceType = atoi(result[ncolumn+3]);

		sqlite3_free_table(result);
	}

	return 0;
}

int voicelist_exec_sql(dc_sql_exec_type_t type, dc_voicelist_t *obj)
{
	int ret;
	char *errmsg;
	char sql[128] = {'\0'};

	switch (type) {
	case SQL_INSERT:
		snprintf(sql, sizeof(sql), "insert into voiceList(voiceId,voiceName,voicePath,voicePic, \
					voiceLength,voiceDesc,voiceSize,downloadTime,isCustom,appVoiceType) \
					values(%d,'%s','%s','%s',%d,'%s',%d,%d,%d,%d)", \
			obj->voiceId, obj->voiceName, obj->voicePath, obj->voicePic, obj->voiceLength, \
			obj->voiceDesc, obj->voiceSize, obj->downloadTime, obj->isCustom, obj->appVoiceType);
		break;
	case SQL_DELETE:
		snprintf(sql, sizeof(sql), "delete from voiceList where voiceId=%d", obj->voiceId);
		break;
	case SQL_SELECT:
		snprintf(sql, sizeof(sql), "select * from voiceList where voiceId=%d", obj->voiceId);
		break;
	default:
		break;
	}

	if (type != SQL_SELECT) {
		if (SQLITE_OK != sqlite3_exec(datacache.db, sql, NULL, NULL, &errmsg)) {
			printf("sqlite exec failed : %s\n", errmsg);
			return -1;
		}
	} else {
		char **result = NULL;
		int nrow, ncolumn;

		ret = sqlite3_get_table(datacache.db, sql, &result, &nrow, &ncolumn, &errmsg);
		if (SQLITE_OK != ret) {
			printf("sqlite3_get_table failed : %d: %s\n", ret, errmsg);
			sqlite3_free_table(result);
			return -1;
		}

		memcpy(obj->voiceName, result[ncolumn+1], sizeof(obj->voiceName));
		memcpy(obj->voicePath, result[ncolumn+2], sizeof(obj->voicePath));
		memcpy(obj->voicePic, result[ncolumn+3], sizeof(obj->voicePic));
		obj->voiceLength= atoi(result[ncolumn+4]);
		memcpy(obj->voiceDesc, result[ncolumn+5], sizeof(obj->voiceDesc));
		obj->voiceSize = atoi(result[ncolumn+6]);
		obj->downloadTime = atoi(result[ncolumn+7]);
		obj->isCustom = atoi(result[ncolumn+8]);
		obj->appVoiceType = atoi(result[ncolumn+9]);

		sqlite3_free_table(result);
	}

	return 0;
}

int albumlist_exec_sql(dc_sql_exec_type_t type, dc_albumlist_t *obj)
{
	int ret;
	char *errmsg;
	char sql[128] = {'\0'};

	switch (type) {
	case SQL_INSERT:
		snprintf(sql, sizeof(sql), "insert into albumList(albumId,albumName,albumCover,albumDesc,albumVoiceCount,dlStatus) \
					values(%d,'%s','%s','%s',%d,%d)", \
			obj->albumId, obj->albumName, obj->albumCover, obj->albumDesc, obj->albumVoiceCount, obj->dlStatus);
		break;
	case SQL_DELETE:
		snprintf(sql, sizeof(sql), "delete from albumList where albumId=%d", obj->albumId);
		break;
	case SQL_SELECT:
		snprintf(sql, sizeof(sql), "select * from albumList where albumId=%d", obj->albumId);
		break;
	default:
		break;
	}

	if (type != SQL_SELECT) {
		if (SQLITE_OK != sqlite3_exec(datacache.db, sql, NULL, NULL, &errmsg)) {
			printf("sqlite exec failed : %s\n", errmsg);
			return -1;
		}
	} else {
		char **result = NULL;
		int nrow, ncolumn;

		ret = sqlite3_get_table(datacache.db, sql, &result, &nrow, &ncolumn, &errmsg);
		if (SQLITE_OK != ret) {
			printf("sqlite3_get_table failed : %d: %s\n", ret, errmsg);
			sqlite3_free_table(result);
			return -1;
		}

		memcpy(obj->albumName, result[ncolumn+1], sizeof(obj->albumName));
		memcpy(obj->albumCover, result[ncolumn+2], sizeof(obj->albumCover));
		memcpy(obj->albumDesc, result[ncolumn+3], sizeof(obj->albumDesc));
		obj->albumVoiceCount = atoi(result[ncolumn+4]);
		obj->dlStatus= atoi(result[ncolumn+5]);

		sqlite3_free_table(result);
	}

	return 0;
}

int response_dev_info(char *msgBytes, int len)
{
	HwInfoResp *rmessage = (HwInfoResp *)malloc(sizeof(HwInfoResp));
	if (rmessage == NULL) {
		printf("malloc failed.\n");
		return -1;
	}

	pb_istream_t input = pb_istream_from_buffer(msgBytes, len);
	if (!pb_decode(&input, HwInfoResp_fields, rmessage)) {
		printf("Decoding failed: %s\n", PB_GET_ERROR(&input));
		free(rmessage);
		return -1;
	}

	printf("-------- %s() ---------\n", __func__);
	printf("## state: %d\n", rmessage->state);
	printf("## msg: %s\n", rmessage->msg);
	printf("## status: %d\n", rmessage->result.status);
	printf("## nickname: %s\n", rmessage->result.nickname);

	strcpy(datacache.dev_info.nick_name, rmessage->result.nickname);
	devinfo_exec_sql(SQL_UPDATE, DEVINFO_NICKNAME, &datacache.dev_info);

	free(rmessage);

	return 0;
}

static int request_dev_info(void)
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

	for (i = 0; i < 16; i++)
		message->serialNum[i] = msgid[i];

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
	ret = send(sockee, destdata, 60 + msg_len, 0);
	if (ret < 0) {
		printf("socket send failed!\n");
		goto err2;
	}

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

static bool group_discuss_objdiscuss(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	//pb_wire_type_t wire_type;
    //uint32_t tag;
    //bool eof;

    HwPullDiscussResp_ObjDiscuss *obj = (HwPullDiscussResp_ObjDiscuss *)malloc(sizeof(HwPullDiscussResp_ObjDiscuss));
	if (obj == NULL) {
		printf("malloc failed\n");
		return false;
	}
    //pb_decode_tag(stream, &wire_type, &tag, &eof);
    if (!pb_decode(stream, HwPullDiscussResp_ObjDiscuss_fields, obj)) {
		fprintf(stderr, "%s(): Decode failed: %s\n", __func__, PB_GET_ERROR(stream));
		free(obj);
        return false;
    }

	printf("## discussId: %d\n", obj->discussId);
	printf("## discussContent: %s\n", obj->discussContent);
	printf("## createTime: %d\n", obj->createTime);

	dc_group_discuss_t dis;
	dis.discussId = obj->discussId;
	memcpy(dis.discussContent, obj->discussContent, sizeof(dis.discussContent));
	dis.createTime = obj->createTime;
	groupdiscuss_exec_sql(SQL_INSERT, &dis);

	free(obj);
    return true;
}

int response_group_discuss(char *msgBytes, int len)
{
	HwPullDiscussResp *response = (HwPullDiscussResp *)malloc(sizeof(HwPullDiscussResp));
	if (response == NULL) {
		printf("malloc failed.\n");
		return -1;
	}

	pb_istream_t input = pb_istream_from_buffer(msgBytes, len);
	response->result.objDiscuss.funcs.decode = &group_discuss_objdiscuss;
	if (!pb_decode(&input, HwPullDiscussResp_fields, response)) {
		fprintf(stderr, "%s(): Decode failed: %s\n", __func__, PB_GET_ERROR(&input));
		free(response);
		return -1;
	}

	printf("-------- function: %s() ---------\n", __func__);
	printf("## state: %d\n", response->state);
	printf("## msg: %s\n", response->msg);
	printf("## thisReqTime: %d\n", response->result.thisReqTime);

	free(response);
	return 0;
}
static int request_group_discuss(void)
{
	int ret;
	unsigned char buf[128];
	int len;
	int i;
	char *destdata = NULL;

	HwPullDiscussReq *message = (HwPullDiscussReq *)malloc(sizeof(HwPullDiscussReq));
	if (message == NULL) {
		printf("malloc failed.\n");
		return -1;
	}

	pb_ostream_t output = pb_ostream_from_buffer(buf, sizeof(buf));

	for (i = 0; i < 16; i++)
		message->serialNum[i] = msgid[i];
	message->has_reqTime = false;
	
	ret = pb_encode(&output, HwPullDiscussReq_fields , message);
	len = output.bytes_written;
	if (!ret) {
		printf("Encoding failed: %s\n", PB_GET_ERROR(&output));  
		goto err1;
	}

	destdata = (char*)malloc((60 + len) * sizeof(char));
	if (destdata == NULL) {
		printf("malloc failed.\n");
		goto err1;
	}

	encode(0xf011f037, 1, buf, len, destdata);
	ret = send(sockee, destdata, 60 + len, 0);
	if (ret < 0) {
		printf("socket send failed!\n");
		goto err2;
	}

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

static bool group_share_objvoice(pb_istream_t *stream, const pb_field_t *field, void **arg)
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

	printf("## voiceId: %d\n", obj->voiceId);
	printf("## voicePath: %s\n", obj->voicePath);
	printf("## createTime: %d\n", obj->createTime);
	printf("## voiceType: %d\n", obj->voiceType);

	free(obj);

	return true;
}

int response_group_share(char *msgBytes, int len)
{
	HwPullVoiceResp *response = (HwPullVoiceResp *)malloc(sizeof(HwPullVoiceResp));
	if (response == NULL) {
		printf("malloc failed\n");
		return -1;
	}

	pb_istream_t input = pb_istream_from_buffer(msgBytes, len);
	response->result.objVoice.funcs.decode = &group_share_objvoice;
	if (!pb_decode(&input, HwPullVoiceResp_fields, response)) {
		fprintf(stderr, "Decode failed: %s\n", PB_GET_ERROR(&input));
		free(response);
		return -1;
	}

	printf("-------- function: %s() ---------\n", __func__);
	printf("## state: %d\n", response->state);
	printf("## msg: %s\n", response->msg);
	printf("## thisReqTime : %d\n", response->result.thisReqTime);

	free(response);

	return 0;
}

static int request_group_share(void)
{
	int ret;
	int i;
	unsigned char buf[128];
	int len;
	char *destdata = NULL;

	HwPullVoiceReq *message = (HwPullVoiceReq *)malloc(sizeof(HwPullVoiceReq));
	if (message == NULL) {
		printf("malloc failed.\n");
		return -1;
	}

	pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));

	for (i = 0; i < 16; i++)
		message->serialNum[i] = msgid[i];
	message->has_reqTime = false;

	ret = pb_encode(&stream, HwPullVoiceReq_fields , message);
	len = stream.bytes_written;
	if (!ret) {
		printf("%s(): Encoding failed: %s\n", __func__, PB_GET_ERROR(&stream));  
		goto err1;
	}

	destdata = (char*) malloc((60 + len) * sizeof(char));
	if (destdata == NULL) {
		printf("malloc failed.\n");
		goto err1;
	}

	encode(0xf011f039, 1, buf, len, destdata);
	ret = send(sockee, destdata, 60+len, 0);
	if (ret < 0) {
		printf("socket send failed!\n");
		goto err2;
	}

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

static bool pull_voice_list_objvoice(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	char *errmsg;

	HwPullVoiceListResp_ObjVoice *obj = (HwPullVoiceListResp_ObjVoice *)malloc(sizeof(HwPullVoiceListResp_ObjVoice));
	if (obj == NULL) {
		printf("malloc failed\n");
		return false;
	}

    if (!pb_decode(stream, HwPullVoiceListResp_ObjVoice_fields, obj)) {
		fprintf(stderr, "Decode failed: %s\n", PB_GET_ERROR(stream));
		free(obj);
        return false;
    }

	printf("## voiceId: %d\n", obj->voiceId);
	printf("## voiceType: %d\n", obj->voiceType);
	printf("## voicePath: %s\n", obj->voicePath);
	printf("## createTime: %d\n", obj->createTime);
	printf("## orderBy: %d\n", obj->orderBy);
	printf("## voiceName: %s\n", obj->voiceName);

	free(obj);

	return true;
}

int response_pull_voice_list(char *msgBytes, int len)
{
	HwPullVoiceListResp *response = (HwPullVoiceListResp *)malloc(sizeof(HwPullVoiceListResp));
	if (response == NULL) {
		printf("malloc failed\n");
		return -1;
	}

	pb_istream_t input = pb_istream_from_buffer(msgBytes, len);
	response->objVoice.funcs.decode = &pull_voice_list_objvoice;
	if (!pb_decode(&input, HwPullVoiceListResp_fields, response)) {
		fprintf(stderr, "Decode failed: %s\n", PB_GET_ERROR(&input));
		free(response);
		return -1;
	}

	printf("-------- function: %s() ---------\n", __func__);
	printf("## state: %d\n", response->state);
	printf("## msg: %s\n", response->msg);
	printf("## allCount : %d\n", response->allCount);

	free(response);
	return 0;
}

static int request_pull_voice_list(int page, int pageSize, int relType, char *relId)
{
	int ret;
	int i;
	unsigned char buf[128];
	int len;
	char *destdata = NULL;

	HwPullVoiceListReq *message = (HwPullVoiceListReq *)malloc(sizeof(HwPullVoiceListReq));
	if (message == NULL) {
		printf("malloc failed.\n");
		return -1;
	}

	pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));

	message->page = page;
	message->pageSize = pageSize;
	message->relType = relType;
	if (relId != NULL)
		strncpy(message->relId, relId, sizeof(message->relId));
	for (i = 0; i < 16; i++)
		message->serialNum[i] = msgid[i];

	ret = pb_encode(&stream, HwPullVoiceListReq_fields , message);
	len = stream.bytes_written;
	if (!ret) {
		printf("%s(): Encoding failed: %s\n", __func__, PB_GET_ERROR(&stream));
		goto err1;
	}

	destdata = (char*) malloc((60 + len) * sizeof(char));
	if (destdata == NULL) {
		printf("malloc failed.\n");
		goto err1;
	}

	encode(0xf011f047, 1, buf, len, destdata);
	ret = send(sockee, destdata, 60+len, 0);
	if (ret < 0) {
		printf("socket send failed!\n");
		goto err2;
	}

	free(destdata);
	free(message);

	return 0;

err2:
	free(destdata);
err1:
	free(message);
	return ret;
}

static bool custom_list_objvoice(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	char *errmsg;

	HwPullVoiceListResp_ObjVoice *obj = (HwPullVoiceListResp_ObjVoice *)malloc(sizeof(HwPullVoiceListResp_ObjVoice));
	if (obj == NULL) {
		printf("malloc failed\n");
		return false;
	}

    if (!pb_decode(stream, HwPullVoiceListResp_ObjVoice_fields, obj)) {
		fprintf(stderr, "Decode failed: %s\n", PB_GET_ERROR(stream));
		free(obj);
        return false;
    }

	printf("## voiceId: %d\n", obj->voiceId);
	printf("## voiceType: %d\n", obj->voiceType);
	printf("## voicePath: %s\n", obj->voicePath);
	printf("## createTime: %d\n", obj->createTime);
	printf("## orderBy: %d\n", obj->orderBy);
	printf("## voiceName: %s\n", obj->voiceName);

	free(obj);

	return true;
}

int response_custom_list(char *msgBytes, int len)
{
	HwPullVoiceListResp *response = (HwPullVoiceListResp *)malloc(sizeof(HwPullVoiceListResp));
	if (response == NULL) {
		printf("malloc failed\n");
		return -1;
	}

	pb_istream_t input = pb_istream_from_buffer(msgBytes, len);
	response->objVoice.funcs.decode = &custom_list_objvoice;
	if (!pb_decode(&input, HwPullVoiceListResp_fields, response)) {
		fprintf(stderr, "Decode failed: %s\n", PB_GET_ERROR(&input));
		free(response);
		return -1;
	}

	printf("-------- function: %s() ---------\n", __func__);
	printf("## state: %d\n", response->state);
	printf("## msg: %s\n", response->msg);
	printf("## allCount : %d\n", response->allCount);

	free(response);
	return 0;
}

static int request_custom_list(void)
{
	int ret;

	ret = request_pull_voice_list(1, 10, 1, NULL);
	if (ret != 0) {
		printf("## %s() : request failed!\n", __func__);
		return ret;
	}

	printf("## %s() return OK.\n", __func__);

	return 0;
}

static bool voice_list_objvoice(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	char *errmsg;

	HwPullVoiceListResp_ObjVoice *obj = (HwPullVoiceListResp_ObjVoice *)malloc(sizeof(HwPullVoiceListResp_ObjVoice));
	if (obj == NULL) {
		printf("malloc failed\n");
		return false;
	}

    if (!pb_decode(stream, HwPullVoiceListResp_ObjVoice_fields, obj)) {
		fprintf(stderr, "Decode failed: %s\n", PB_GET_ERROR(stream));
		free(obj);
        return false;
    }

	printf("## voiceId: %d\n", obj->voiceId);
	printf("## voiceType: %d\n", obj->voiceType);
	printf("## voicePath: %s\n", obj->voicePath);
	printf("## netPath: %s\n", obj->netPath);
	printf("## voiceName: %s\n", obj->voiceName);

	free(obj);

	return true;
}

int response_voice_list(char *msgBytes, int len)
{
	HwPullVoiceListResp *response = (HwPullVoiceListResp *)malloc(sizeof(HwPullVoiceListResp));
	if (response == NULL) {
		printf("malloc failed\n");
		return -1;
	}

	pb_istream_t input = pb_istream_from_buffer(msgBytes, len);
	response->objVoice.funcs.decode = &voice_list_objvoice;
	if (!pb_decode(&input, HwPullVoiceListResp_fields, response)) {
		fprintf(stderr, "Decode failed: %s\n", PB_GET_ERROR(&input));
		free(response);
		return -1;
	}

	printf("-------- function: %s() ---------\n", __func__);
	printf("## state: %d\n", response->state);
	printf("## msg: %s\n", response->msg);
	printf("## allCount : %d\n", response->allCount);

	free(response);
	return 0;
}

static int request_voice_list(void)
{
	int ret;

	ret = request_pull_voice_list(1, 10, 2, NULL);
	if (ret != 0) {
		printf("## %s() : request failed!\n", __func__);
		return ret;
	}

	printf("## %s() return OK.\n", __func__);

	return 0;
}

int response_album_list(char *msgBytes, int len)
{

	return 0;
}

static int request_album_list(void)
{
	int ret;
	char relId[] = "123562";

	ret = request_pull_voice_list(1, 10, 3, relId);
	if (ret != 0) {
		printf("## %s() : request failed!\n", __func__);
		return ret;
	}

	printf("## %s() return OK.\n", __func__);

	return 0;
}

static int parse_updflag(unsigned int value)
{
	int count = 0;
	unsigned int i;
	const unsigned int index_start = 31;
	const unsigned int index_end = 26;

	for (i = index_start; i >= index_end; --i) {
		if (value & (1<<i))
			break;
		++count;
	}

	if (count > (index_start - index_end))
		return -1;

	return count;
}

int update_cache(unsigned int updflag)
{
	int status = -1;
	int updtype;

	if (updflag == 0)
		return -1;

	datacache.upd_flag = updflag;
	devinfo_exec_sql(SQL_UPDATE, DEVINFO_UPDFLAGS, &datacache.dev_info);

	updtype = parse_updflag(updflag);
	if (updtype < 0) {
		printf("error updtype\n");
		datacache.upd_flag = 0;
		devinfo_exec_sql(SQL_UPDATE, DEVINFO_UPDFLAGS, &datacache.dev_info);
		return 1;
	}

	switch (updtype) {
	case 0:
		status = request_dev_info();
		break;
	case 1:
		status = request_group_discuss();
		break;
	case 2:
		status = request_group_share();
		break;
	case 3:
		status = request_custom_list();
		break;
	case 4:
		status = request_voice_list();
		break;
	case 5:
		status = request_album_list();
		break;
	default:
		break;
	}

	if (status < 0) {
		printf("## update cache failed!\n");
		return status;
	}

	/* clear flag */
	datacache.upd_flag = 0;
	devinfo_exec_sql(SQL_UPDATE, DEVINFO_UPDFLAGS, &datacache.dev_info);

	return 0;
}

static int request_init_cache(void)
{
	int ret;

	if (sql_table_rows("devInfo", datacache.db) > 0)
		ret = devinfo_exec_sql(SQL_UPDATE, DEVINFO_DEFAULT, &datacache.dev_info);
	else
		ret = devinfo_exec_sql(SQL_INSERT, DEVINFO_DEFAULT, &datacache.dev_info);
	if (ret != 0)
		return ret;

	ret = request_group_discuss();
	if (ret != 0)
		return ret;

	ret = request_group_share();
	if (ret != 0)
		return ret;

	ret = request_custom_list();
	if (ret != 0)
		return ret;

	ret = request_voice_list();
	if (ret != 0)
		return ret;

	ret = request_album_list();
	if (ret != 0)
		return ret;

	return 0;
}

int init_sqlite(void)
{
	int ret;
	int i;
	char *errmsg = NULL;
	datacache_t *dc = &datacache;

	dc->filepath = DATABASE_PATH;
	dc->db = NULL;

	/* open database */
	ret = sqlite3_open(dc->filepath, &dc->db);
	if (SQLITE_OK != ret) {
		printf("## sqlite open failed : %d\n", ret);
		return ret;
	}

	/* create tables */
	for (i = 0; i < sizeof(SQL_CREATE)/sizeof(SQL_CREATE[0]); ++i) {
		ret = sqlite3_exec(dc->db, SQL_CREATE[i], NULL, NULL, &errmsg);
		if (SQLITE_OK != ret) {
			printf("## sqlite create table failed : %d %s\n", ret, errmsg);
			sqlite3_close(dc->db);
			return ret;
		}
	}

	return 0;
}

int init_datacache(const datacache_t *cache)
{
	int ret;

	strcpy(datacache.dev_info.serial_num, msgid);
	strcpy(datacache.dev_info.nick_name, cache->dev_info.nick_name);
	datacache.upd_flag = 0;
	
	ret = request_init_cache();
	if (ret != 0) {
		printf("## %s(): request init cache failed!\n", __func__);
		return ret;
	}

	printf("## %s() return OK.\n", __func__);
	
	return 0;
}

void clear_datacache(void)
{
	groupdiscuss_exec_sql(SQL_DELETE, NULL);
	groupshare_exec_sql(SQL_DELETE, NULL);
	sqlite3_close(datacache.db);
	datacache_zero(&datacache);
}

