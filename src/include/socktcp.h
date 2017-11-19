#ifndef __SOCK_TCP_H
#define __SOCK_TCP_H

#define MSG_MAX_LENGTH	10240
typedef struct _msginfo {
	unsigned int type;
	char msg[MSG_MAX_LENGTH];
	int length;
} msginfo_t, *pmsginfo_t;

typedef struct _socktcp {
	int (*connip)(const char *ip, unsigned int port);
	int (*connhost)(const char *host, unsigned int port);
	void (*disconn)(void);
	int (*send)(unsigned int type, const char *msg, int len);
} socktcp_t, *psocktcp_t;

extern void socktcp_init(psocktcp_t sc);

#endif
