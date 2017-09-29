#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <netdb.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <malloc.h>
#include <unistd.h>
#include "protob/ConnectReq.pb.h"
#include "Encoder.h"
#include "Decoder.h"
#include "protob/pb_encode.h"
#include "protob/pb_decode.h"
#include "protob/pb_common.h"
#include "protob/simple.pb.h"

#include "net.h"


char *hostname= "1shuo.com";
#define PORT 7101
#define MAXBUF 1024
int sockee=-1;
static int writing = 0;

#define RECV_BUFF_SIZE      1024
static uint8_t recvBuff[RECV_BUFF_SIZE];
char msgid[16]="862855028033177";


int socket_connect(void)
{
        unsigned char buf[128];  
       // size_t message_length; 
	bool status;
	int i;
        int len,length;

        /* Get host address from the input name */
        struct hostent *hostinfo = gethostbyname(hostname);
        if (!hostinfo) {
                printf("gethostbyname Failed\r\n");
                exit(0);
        }

        struct sockaddr_in dest;

        /* Create a socket */
        /*---Open socket for streaming---*/
        if ((sockee = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

                perror("Socket");
                exit(errno);
        }

        /*---Initialize server address/port struct---*/
        memset(&dest, 0, sizeof(dest));
        dest.sin_family = AF_INET;
        dest.sin_port = htons(PORT);
 
        dest.sin_addr = *((struct in_addr *) hostinfo->h_addr);

        /*---Connect to server---*/
         if (connect(sockee, (struct sockaddr*)&dest, sizeof(dest)) != 0)
  {
    printf("conenct failed!");
    perror("Connect");
    exit(errno);
  }

           ConnectReq *message=malloc(sizeof( ConnectReq));
	   pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
	   
	   
	   message->isReset=0;
	   message->versionCode=1;
	  for(i=0;i<16;i++)
		{ message->serialNum[i]=msgid[i];}   
	   status = pb_encode(&stream, ConnectReq_fields , message);
	   len = stream.bytes_written;
           printf("protobuf len:%d",len);   
           printf("buf:%s",buf);  
	   if (!status)      
	   	{            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));  
	   return 1;        }
           char *destData = (char*) malloc((60 + len) * sizeof(char));
           encode(0xf011f001, 1, buf,len, destData);
           printf("destDATA:%s",destData);
           length = send(sockee, destData, 60+len, 0);
           if (length < 0)
        {
          printf("Message '%s' failed to send ! \
                The error code is %d, error message '%s'\n",
                destData, errno, strerror(errno));

        }
        else
        {
          printf("News: %s \t send, sent a total of %d bytes!\n",
                destData, length);
        }
      

        return 0;
}

void *sock_receive_func(void *arg)
{   int length;
    int err = 0;
    unsigned int timeout = 2000;
    int i=0;
    char buf[]={};
    char *destData = (char*) malloc(60  * sizeof(char));
    encode(0xB000B000, 2, buf,0, destData);
    while (1) {

          int exit = 0;
                  struct timeval timeout;
                  timeout.tv_sec = 20;
                  timeout.tv_usec = 0;
                  while (!exit)
                        {
            fd_set readset;
            fd_set writeset;
            int32_t r = 0;

            FD_ZERO(&readset);
            FD_ZERO(&writeset);

            FD_SET(sockee, &readset);
            if (writing) {
                FD_SET(sockee, &writeset);}

            r = select(sockee + 1, &readset, &writeset, 0, &timeout);

   //            printf("select returns: %d", r);

                                         if (r < 0) {                exit = 1;    close(sockee);

                            socket_connect();        }
                                         else
                                                {
                                                //if (FD_ISSET(socket, &readset))
                                                FD_ISSET(sockee, &readset);
                                                //nlog_i("readset");    
                    int count = recv(sockee, recvBuff, sizeof(recvBuff), 0);
     //               printf("recv returns %d", count);
                   if (count > 0) {
                                        if (count < sizeof(recvBuff)) {
                                                recvBuff[count] = 0;                        }
                                                else {   recvBuff[count -1] = 0;   }

                                                if(recvBuff[0]==240)
                                                        { decode(recvBuff, count);
                         memset(recvBuff, 0, RECV_BUFF_SIZE);
                                                // nfl_wifi_release(APP_WIFI_CONNECT);
                                                }
                                       else if(recvBuff[0]==176)
                                                        {//sendSocketMsg(destData);
                                                 length = send(sockee, destData, 60, 0);
                       if (length < 0)
                     {
                       printf("Message '%s' failed to send ! \
                       The error code is %d, error message '%s'\n",
                       destData, errno, strerror(errno));
                       break;
                     }
                      else
                    {
       //                printf("News: %s \t send, sent a total of %d bytes!\n",
         //              destData, length);
                    }

                                                 memset(recvBuff, 0, RECV_BUFF_SIZE);
                                                 timeout.tv_sec = 1000;
                                                 timeout.tv_usec = 1000;
                                                }
                                                 else
                                                        {printf("Receive nothing\n");
                                 i++;
                                                 timeout.tv_sec = 1000;
                                                 timeout.tv_usec = 1000;
                            if(i==10)
                               {close(sockee);
                                                exit=1;
                            socket_connect();
                             i=0;
                               }
                                                        }}else
                                                {close(sockee);
                                                exit=1;
                            socket_connect();}
                                                }}}


}

int create_sock_receive_pthread(void)
{
       int ret = 0;
       pthread_t receive_pthread;
       ret=pthread_create(&receive_pthread,NULL,sock_receive_func,NULL);
       if(ret !=0) {
       printf("Create receive pthread failed: %s!\n", strerror(errno));
                   return ret;
                }
        pthread_detach(receive_pthread);
        return ret;
}
