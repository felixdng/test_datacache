#ifndef __DATACACHE_H__
#define __DADACACHE_H__

#include <stdio.h>
#include <string.h>
#include "sqlite3.h"


typedef enum _devinfo_fields {
	DEVINFO_DEFAULT = 0,
	DEVINFO_NICKNAME,
	DEVINFO_UPDFLAGS,
} devinfo_fields_t;

typedef enum _dc_sql_exec_type {
	SQL_DEFAULT = 0,
	SQL_INSERT,
	SQL_SELECT,
	SQL_UPDATE,
	SQL_DELETE,
} dc_sql_exec_type_t;

typedef struct _dc_dev_info {
	char serial_num[16];
	char nick_name[64];
	unsigned int upd_flag;
} dc_dev_info_t;

typedef struct _dc_group_discuss {
	unsigned int discussId;
	char discussContent[128];
	unsigned int createTime;
} dc_group_discuss_t;

typedef struct _dc_group_voice {
    unsigned int voiceId;
    char voicePath[128];
    unsigned int shareTime;
    unsigned int voiceType;
} dc_group_voice_t;

typedef struct _dc_voicelist {
	unsigned int voiceId;
	char voiceName[64];
	char voicePath[128];
	char voicePic[64];
	unsigned int voiceLength;
	char voiceDesc[128];
	unsigned int voiceSize;
	unsigned int downloadTime;
	unsigned int isCustom;
	unsigned int appVoiceType;
} dc_voicelist_t;

typedef struct _dc_albumlist {
	unsigned int albumId;
	char albumName[64];
	char albumCover[64];
	char albumDesc[128];
	unsigned int albumVoiceCount;
	unsigned int dlStatus;
} dc_albumlist_t;

typedef struct _datacache {
	sqlite3 *db;
	char *filepath;
	unsigned int upd_flag;
	dc_dev_info_t dev_info;
} datacache_t;

#define DATACACHE_ZERO {NULL, NULL, 0, {"","",0}}
extern void datacache_zero(datacache_t *data);

extern int devinfo_exec_sql(dc_sql_exec_type_t type, devinfo_fields_t field, dc_dev_info_t *devinfo);
extern int groupdiscuss_exec_sql(dc_sql_exec_type_t type, dc_group_discuss_t *obj);
extern int groupshare_exec_sql(dc_sql_exec_type_t type, dc_group_voice_t *obj);
extern int voicelist_exec_sql(dc_sql_exec_type_t type, dc_voicelist_t *obj);
extern int albumlist_exec_sql(dc_sql_exec_type_t type, dc_albumlist_t *obj);

extern int response_dev_info(char *msgBytes, int len);
extern int response_group_discuss(char *msgBytes, int len);
extern int response_group_share(char *msgBytes, int len);
extern int response_custom_list(char *msgBytes, int len);
extern int response_voice_list(char *msgBytes, int len);
extern int response_album_list(char *msgBytes, int len);
extern int response_pull_voice_list(char *msgBytes, int len);

extern int update_cache(unsigned int updflag);
extern int init_sqlite(void);
extern int init_datacache(const datacache_t *cache);
extern void clear_datacache(void);


#endif /* __DATACACHE_H__ */
