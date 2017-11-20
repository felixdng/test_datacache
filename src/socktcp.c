/**
 * socktcp.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <semaphore.h>

#include "socktcp.h"
//#include "yishuo.h"
#include "Encoder.h"
//#include "decoder.h"
#include "protob/pb_encode.h"
#include "protob/pb_decode.h"
#include "protob/pb_common.h"
#include "protob/ConnectReq.pb.h"

#undef _YS_DEBUG
#define _YS_DEBUG
#ifdef _YS_DEBUG
#define LOG_DEBUG(format, args...) \
	fprintf(stdout, "[DEBUG] [func %s()] : "format, __func__, ##args)
#else
#define LOG_DEBUG(format, args...)
#endif

#define LOG_INFO(format, args...) \
	fprintf(stdout, "[INFO] [func %s()] : "format, __func__, ##args)

#define LOG_ERR(format, args...) \
	fprintf(stderr, "[ERROR] [func %s()] : "format, __func__, ##args)


#define msglist_lock(lock)						\
	do {                                                            \
		int i = 0;                                              \
									\
		while (pthread_mutex_trylock(lock)) {                   \
			if (i++ >= 100) {                               \
				printf("#### {%s, %s, %d} dead lock ####\n", \
				       __FILE__, __func__, __LINE__); \
				i = 0;                                  \
			}                                               \
			usleep(100 * 1000);                             \
		}                                                       \
	} while (0)

#define msglist_unlock(lock)					\
	do {								\
		pthread_mutex_unlock(lock);				\
	} while (0)

typedef enum _QUEUE_STATUS {
	QUEUE_OK = 0,
	QUEUE_FAILED = -1, 
	QUEUE_FULL = -2, 
	QUEUE_EMPTY = -3, 
} QUEUE_STATUS;

#define QUEUE_SIZE  1024
typedef struct _msg_queue {
	msginfo_t data[QUEUE_SIZE];
	int front, rear;
} msg_queue_t, *msg_queue_p;

static msg_queue_t msgqueue_s;
static msg_queue_t msgqueue_r;

static int sock_fd;
static char hostname[] = "1shuo.com";
static unsigned int port = 7101;

static pthread_mutex_t msglist_mutex_s = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t msglist_mutex_r = PTHREAD_MUTEX_INITIALIZER;

static sem_t sem_msgrecv;
static msginfo_t msgrecv;
static int is_recv = 0;

static char msgid[16]="862855028033177";

/*****************************************************************************/
static bool is_empty_queue(msg_queue_p queue)
{
	if (queue->front == queue->rear)
		return true;
	else
		return false;
}

static bool is_full_queue(msg_queue_p queue)
{
	if (queue->front == ((queue->rear + 1) % QUEUE_SIZE))
		return true;
	else
		return false;
}

static void msg_queue_init(msg_queue_p queue)
{
	queue->front = queue->rear = QUEUE_SIZE - 1;
}

static QUEUE_STATUS msg_queue_push(msg_queue_p queue, msginfo_t msg)
{
	if (is_full_queue(queue)) {
		LOG_INFO("msg queue is full.");
		return QUEUE_FULL;
	}

	queue->rear = (queue->rear + 1) % QUEUE_SIZE;

	queue->data[queue->rear].type = msg.type;
	memcpy(queue->data[queue->rear].msg, msg.msg, MSG_MAX_LENGTH);
	queue->data[queue->rear].length = msg.length;

	return QUEUE_OK;
}

static QUEUE_STATUS msg_queue_get(msg_queue_p queue, msginfo_t *msg)
{
	if (is_empty_queue(queue)) {
		LOG_INFO("msg queue is empty.");
		return QUEUE_EMPTY;
	}

	queue->front = (queue->front + 1 ) % QUEUE_SIZE;

	msg->type = queue->data[queue->front].type;
	memcpy(msg->msg, queue->data[queue->front].msg, MSG_MAX_LENGTH);
	msg->length = queue->data[queue->front].length;

	queue->data[queue->front].type = 0;
	memset(queue->data[queue->front].msg, 0, MSG_MAX_LENGTH);
	queue->data[queue->front].length = 0;

	return QUEUE_OK;
}
/*****************************************************************************/

static int push_msg_s(msginfo_t msg)
{
	int ret = 0;
	msglist_lock(&msglist_mutex_s);
	ret = msg_queue_push(&msgqueue_s, msg);
	msglist_unlock(&msglist_mutex_s);
	return ret;
}

static int push_msg_r(msginfo_t msg)
{
	int ret = 0;
	msglist_lock(&msglist_mutex_r);
	ret = msg_queue_push(&msgqueue_r, msg);
	msglist_unlock(&msglist_mutex_r);
	return ret;
}

static int get_msg_s(msginfo_t *msg)
{
	int ret = 0;
	msglist_lock(&msglist_mutex_s);
	ret = msg_queue_get(&msgqueue_s, msg);
	msglist_unlock(&msglist_mutex_s);
	return ret;
}

static int get_msg_r(msginfo_t *msg)
{
	int ret = 0;
	msglist_lock(&msglist_mutex_r);
	ret = msg_queue_get(&msgqueue_r, msg);
	msglist_unlock(&msglist_mutex_r);
	return ret;
}

static int connip(const char *ip, unsigned int port)
{
	struct sockaddr_in sin;

	//创建套接字
	if( (sock_fd = socket(AF_INET,SOCK_STREAM,0)) < 0){
		LOG_ERR("create socket failed!\n");
		return -1;
	}

	//填充地址
	bzero(&sin,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(ip);

	//申请连接服务器
	if( connect(sock_fd,(struct sockaddr*)&sin,sizeof(sin)) < 0){
		LOG_ERR("conenct failed!\n");
		return -1;
	}

	return 0;
}

static int connhost(const char *hostname, unsigned int port)
{
	struct sockaddr_in dest;

	/* Get host address from the input name */
	struct hostent *hostinfo = gethostbyname(hostname);
	if (!hostinfo) {
		LOG_ERR("gethostbyname Failed!\n");
		return -1;
	}

	/* Create a socket */
	/*---Open socket for streaming---*/
	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		LOG_ERR("create socket failed!\n");
		return -1;
	}

	/*---Initialize server address/port struct---*/
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(port);
	dest.sin_addr = *((struct in_addr *) hostinfo->h_addr);

	/*---Connect to server---*/
	if (connect(sock_fd, (struct sockaddr*)&dest, sizeof(dest)) != 0) {
		LOG_ERR("conenct failed!\n");
		return -1;
	}

#if 0
	unsigned char buf[1280];
	int len, status;
	ConnectReq message={};
	pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));

	message.isReset=0;
	message.versionCode=1;
	strcpy(message.serialNum, msgid);
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
			LOG_ERR("malloc failed!\n");
			return -1;
		}
	
		encode(HARDWARE_CONNECT, sn++, buf, len, dest);
		if (send(sock_fd, dest, 60 + len, 0) < 0) {
			LOG_ERR("socket send failed!\n");
			free(dest);
			return -1;
		}
	
		free(dest);
	}

#endif
	return 0;
}

static void disconn(void)
{
	if (sock_fd > 0) {
		close(sock_fd);
		sock_fd = -1;
	}
}

static void sock_write_handler(void)
{
	msginfo_t msg;

	while (!is_empty_queue(&msgqueue_s)) {
		get_msg_s(&msg);
		LOG_DEBUG("type: %x, msg: %s, len: %d\n", msg.type, msg.msg, msg.length);
		send(sock_fd, msg.msg, msg.length, 0);
	}
}

static void sock_read_handler(void)
{
	int rbuffsize = 10240;
	unsigned char rbuff[rbuffsize];
	int count;

	char destData[60];
	char buf[1] = {};
	encode(0xB000B000, 2, buf, 0, destData);

	memset(rbuff, 0, sizeof(rbuff));
	count = recv(sock_fd, rbuff, sizeof(rbuff), 0);
	if (count > 0) {
		//if (count < sizeof(recvBuff)) {
			//recvBuff[count] = 0;
		//} else {
			//recvBuff[count -1] = 0;
		//}

		if (rbuff[0] == 240) {
			LOG_DEBUG("recv: %s\n", rbuff);
			if (is_recv) {
				msginfo_t msg;
				//msg.type = type;
				memcpy(msg.msg, rbuff, count);
				msg.length = count;
				push_msg_r(msg);
				//memcpy(msgrecv.msg, rbuff, count);
				//msgrecv.length = count;
				is_recv = 0;
				LOG_DEBUG("befor sem_post\n");
				sem_post(&sem_msgrecv);
			} else {
				decode(rbuff, count);
			}
		} else if (rbuff[0] == 176) {
			LOG_DEBUG("recv 176\n");
			send(sock_fd, destData, 60, 0);
		} else {
			LOG_DEBUG("Receive nothing\n");
		}

	}else {
		disconn();
		connhost(hostname, port);
	}
}

static void *sock_recv_thread(void *arg)
{
	struct timeval timeout;
	fd_set readset;
	fd_set writeset;
	int status;

	while (1) {	
		timeout.tv_sec = 10;
		timeout.tv_usec = 0;

		FD_ZERO(&readset);
		FD_ZERO(&writeset);

		FD_SET(sock_fd, &readset);
		FD_SET(sock_fd, &writeset);

		status = select(sock_fd + 1, &readset, &writeset, 0, &timeout);
		if (status < 0) {
				//close(sockee);
				//socket_connect();
		} else if (status == 0) {
			//timeout
			continue;
		} else {
			if ( FD_ISSET(sock_fd, &writeset) && !(is_empty_queue(&msgqueue_s)) ) {
				LOG_DEBUG("write...\n");
				sock_write_handler();
			} else if (FD_ISSET(sock_fd, &readset)) {
				LOG_DEBUG("read\n");
				sock_read_handler();
			}		
		}

	}/* end while (1) */

}

static void create_sock_recv_thread(void)
{
	int ret = 0;
	pthread_t recv_thread;

	ret = pthread_create(&recv_thread, NULL, sock_recv_thread, NULL);
	if(ret != 0) {
		LOG_ERR("Create receive thread failed: %s\n", strerror(errno));
	} else {
		pthread_detach(recv_thread);
	}
}

static int msgsend(unsigned int type, const char *msg, int len)
{
	msginfo_t msg_s;
	msg_s.type = type;
	memcpy(msg_s.msg, msg, len);
	msg_s.length = len;
	return push_msg_s(msg_s);
}

void socktcp_init(psocktcp_t sc)
{
	sock_fd = -1;
	msg_queue_init(&msgqueue_s);
	msg_queue_init(&msgqueue_r);

	sc->connip = connip;
	sc->connhost = connhost;
	sc->disconn = disconn;
	sc->send = msgsend;

	sem_init(&sem_msgrecv, 0, 0);
	connhost(hostname, port);
	//connip("192.168.1.105", 5678);
	create_sock_recv_thread();
	usleep(5000);
}
#if 0
int main(int argc, char **argv)
{
	socket_con_t sc;
	socket_con_init(&sc);
	//sc.send(0xf011f001, "hello_1", 8);
	//sc.send(0xf011f001, "hello_2", 8);
	//sc.send(0xf011f001, "hello_3", 8);

	sleep(1);
	if (is_empty_queue(&msgqueue))
		printf("empty\n");
	else
		printf("not empty\n");
	sleep(3);
	return 0;
}
#endif
