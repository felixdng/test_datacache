#ifndef __NET_H
#define __NET_H

extern int sockee;
extern char msgid[16];
extern int socket_connect(void);
extern int create_sock_receive_pthread(void);

#endif
