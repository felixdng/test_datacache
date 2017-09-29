/* pb_decode.c -- decode a protobuf using minimal resources
 *
 * 2011 Petteri Aimonen <jpa@kapsi.fi>
 */

/* Use the GCC warn_unused_result attribute to check that all return values
 * are propagated correctly. On other compilers and gcc before 3.4.0 just
 * ignore the annotation.
 */
#include <errno.h>
#include <stdio.h>
//#include "Decoder.h"
#include <protob/pb_encode.h>
#include <protob/pb_decode.h>
#include <protob/simple.pb.h>
#include <protob/pb_common.h>
#include "Control.h"

#include "Encoder.h"

extern int sockee;
extern int writing;
bool status;
extern char uid[37];
int downloadflag=0;
extern char msgid[16];
unsigned char buf[128];
int32_t voiceid;
int download;
int dlType;
char dl_url[100],local_url[100];
char DvoiceRe[100];
char relId[10];
/*设备登陆*/
void DLu(char *msgBytes,int len) {
    int i;
    ConnectResp *rmessage=malloc(sizeof(ConnectResp));
	//pb_istream_t stream = pb_istream_from_buffer(msgBytes,ctxBuffer.matchCount);
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream, ConnectResp_fields, rmessage);
        
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
        
        /* Print the data contained in the message. */
        printf("device state was %d!\n", rmessage->state);
		printf("master id was %d!\n", rmessage->masterId);
		printf("version code was %d!\n", rmessage->versionCode);
		printf("msg was %s!\n", rmessage->msg);
		printf("uuid was %s!\n", rmessage->uuid);
		printf("version url was %s!\n", rmessage->version);
		printf("nick name was %s!\n", rmessage->nickname);
		memset(uid,0,37);
		for(i=0;i<37;i++)
			{uid[i]=rmessage->uuid[i];
		}
		printf("uid was %s\n",uid);
   
	free(rmessage);

}
/*设备注册*/
void Zce(char *msgBytes,int len)
{   HwRegisterPush *rmessage=malloc(sizeof(HwRegisterPush));
    pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream, HwRegisterPush_fields, rmessage);
        
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
        
        /* Print the data contained in the message. */
        printf("userId was %d!\n", rmessage->userId);
		printf("needReset was %d!\n", rmessage->needReset);
		printf("nickname was %s!\n", rmessage->nickname);
		
   
	free(rmessage);
}
/*设备注销*/
void Zxiao(char *msgBytes,int len)
{  
		
   
	
}
/*开始播放*/
void SPlay(char *msgBytes,int len) {
    int i=0;
	int count=0;
	char s1[100]="sd://";
	char s2[100];
	size_t message_length;
        HwStartPlayPush *rmessage=malloc(sizeof(HwStartPlayPush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream, HwStartPlayPush_fields, rmessage);
        
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
        
        /* Print the data contained in the message. */
        printf("voiceId was %d!\n", rmessage->voiceId);
	printf("voicePath  was %s!\n", rmessage->voicePath);
	printf("voiceType was %d!\n", rmessage->voiceType);
	printf("relType was %d!\n", rmessage->relType);
	printf("relId was %s!\n", rmessage->relId);
	printf("allCount was %d!\n", rmessage->allCount);
	printf("userId was %d\n",rmessage->userId);
	printf("netPath was %s\n",rmessage->netPath);
		
		strcat(s1,rmessage->voicePath);
		while(s1[i]!='\0')
		{ if(s1[i]=='m')
			{s1[i]='M';}
		else if(s1[i]=='p')
		{s1[i]='P';}
		i++;}
		printf("playPath was %s\n",s1);
		//count=str_utf82gbk(s2, strlen(s1), s1);
		//s2[count]='\0';
		//for(i=0;i<10;i++)
		{relId[i]=rmessage->relId[i];}
		//printf("playPath2 was %s\n",s2);
		//nfl_player_play(s2, 1, 0);
		
	free(rmessage);

}
/*播放控制*/
void PControl(char *msgBytes,int len) {
    int i;
	
	size_t message_length;
        HwPlayModePush *rmessage=malloc(sizeof(HwPlayModePush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream, HwPlayModePush_fields, rmessage);
        
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
        
        /* Print the data contained in the message. */
        printf("playMode was %d!\n", rmessage->playMode);
	if(rmessage->playMode==2)
	 {//nfl_player_stop();
          }
	  else if(rmessage->playMode==3)
	 {  }
   
	free(rmessage);

}


static bool printfile_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{   size_t message_length; 
    int i;
    int count=0;
    char s1[100]="http://www.1shuo.com/";
    char s2[100]="0:";
	char s3[100];
    HwDLLocalPush_ObjVoice  *obj=malloc(sizeof(HwDLLocalPush_ObjVoice));
    //HwDLLocalPush_ObjVoice  obj={};
    if (!pb_decode(stream,  HwDLLocalPush_ObjVoice_fields,obj))
        return false;
    printf("voiceId was %d!\n",obj->voiceId);
    printf("netPath was %s!\n",obj->netPath);
    printf("localPath was %s!\n",obj->localPath);
    strcat(s1,obj->netPath);
    strcat(s2,obj->localPath);
    printf("playPath was %s\n",s1);
    printf("storePath was %s\n",s2);
    voiceid=obj->voiceId;
	/*count=str_utf82gbk(s3, strlen(s2), s2);
	s3[count]='\0';
	for(i=0;i<100;i++)
	{dl_url[i]=s1[i];}
    for(i=0;i<100;i++)
	{local_url[i]=s3[i];}*/
    
	//nfl_player_play(s1, NFL_MEDIA_TYPE_MP3, 0);
	//download=download_file_from_url_and_store(s1, s3);
	//NflThreadCreate(&downloadThread, "download", download_file_from_url_and_store, 
    //                 NULL, 4096, NFL_PRIO_HIGHEST);
	
	  
    return true;
}

static bool dload_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{     //HwDLLocalToAppReq_ObjVoice obj={};
      HwDLLocalToAppReq_ObjVoice *obj=malloc(sizeof(HwDLLocalToAppReq_ObjVoice));
      obj->voiceId=voiceid;
      obj->dlType=5;
	  status=pb_encode_tag_for_field(stream, field);
	   if (!status)
	   { return false;}

       status=pb_encode_submessage(stream, HwDLLocalToAppReq_ObjVoice_fields, obj);
        if (!status)
        {    return false;}
     free(obj);
     return true;
}

/*下载本地专辑*/
void DLocal(char *msgBytes,int len) {
     int i;
	size_t message_length;
    HwDLLocalPush *rmessage=malloc(sizeof(HwDLLocalPush));
	//HwDLLocalPush rmessage={};
    pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	rmessage->objVoice.funcs.decode = &printfile_callback;
	status = pb_decode(&stream,  HwDLLocalPush_fields,rmessage);
	
	
        
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
       //pb_decode(&stream,  HwDLLocalPush_ObjVoice_fields,obj);  
        /* Print the data contained in the message. */
        printf("dlType was %d!\n", rmessage->dlType);
	   if(rmessage->dlType==0)
		   downloadflag=1;
	   else
        downloadflag=0;			
   
       HwDLLocalToAppReq *response=malloc(sizeof(HwDLLocalToAppReq));
       memset(buf,0,sizeof(buf));
       pb_ostream_t stream1 = pb_ostream_from_buffer(buf, sizeof(buf));
       for(i=0;i<16;i++)
       {response->serialNum[i]=msgid[i];}
       response->objVoice.funcs.encode = &dload_callback;
       status =pb_encode(&stream1,HwDLLocalToAppReq_fields, response);
       message_length = stream1.bytes_written;
       if (!status)      
       {  printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));  
	   return 1;  }
          printf("DOWLOD RPLY \n");
	  char *destData = (char*) malloc((60 + message_length) * sizeof(char));
	  encode(0xf011f068, 1, buf,message_length, destData);
	  printf("DOWLOD RPLY. \n");
	 int length = send(sockee, destData, 60+message_length, 0);
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

	  printf("DOWLOD OK \n");
	  
       	   
}

/*发布声音到群组*/
void RVoice(char *msgBytes,int len) {
    char s1[100]="http://www.1shuo.com/";
	int i;
	
	size_t message_length;
    HwReleaseVoicePush *rmessage=malloc(sizeof(HwReleaseVoicePush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream,HwReleaseVoicePush_fields,rmessage);     
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }  
        /* Print the data contained in the message. */
        printf("voiceId was %d!\n", rmessage->voiceId);
	    printf("voicePath was %s!\n",rmessage->voicePath);
	    printf("shareTime was %d!\n",rmessage->shareTime);
		
		
	free(rmessage);

}

/*分享声音到群组*/
void SVoice(char *msgBytes,int len) {
    char s1[100]="http://www.1shuo.com/";
	int i;
	
	size_t message_length;
    HwShareVoicePush *rmessage=malloc(sizeof(HwShareVoicePush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream,HwShareVoicePush_fields,rmessage);     
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }  
        /* Print the data contained in the message. */
        printf("voiceId was %d!\n", rmessage->voiceId);
	printf("voicePath was %s!\n",rmessage->voicePath);
	printf("shareTime was %d!\n",rmessage->shareTime);
        printf("voiceType was %d!\n",rmessage->voiceType);
		
	free(rmessage);

}
static bool deletevoice_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{   size_t message_length; 
    int i;
    int count=0;
   
    HwDeleteGroupVoicePush_ObjVoice  *obj=malloc(sizeof(HwDeleteGroupVoicePush_ObjVoice));
    //HwDLLocalPush_ObjVoice  obj={};
      if(!pb_decode(stream,  HwDeleteGroupVoicePush_ObjVoice_fields,obj))  
	  return false;
        /* Print the data contained in the message. */
       // wmprintf("voiceId was %d!\n", rmessage->dlType);
       printf("voiceId was %d!\n",obj->voiceId);
       printf("voiceType was %d!\n",obj->voiceType);
	
		
       free(obj);  
    return true;
}
/*删除群声音*/
void DGVoice(char *msgBytes,int len) {
	int i;
	
	size_t message_length;
        HwDeleteGroupVoicePush *rmessage=malloc(sizeof(HwDeleteGroupVoicePush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	rmessage->objVoice.funcs.decode = &deletevoice_callback;
	status = pb_decode(&stream,  HwDLLocalPush_fields,rmessage);
      
        /* Check for errors... */
       if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
	printf("voiceIds was %d!\n",rmessage->voiceIds);
     
	free(rmessage);	


}
/*获取版本号*/
void VUpdate(char *msgBytes,int len) {
    
	int i;
	
	size_t message_length;
        HwVersionUpdatePush *rmessage=malloc(sizeof(HwVersionUpdatePush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream,HwVersionUpdatePush_fields,rmessage);     
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }  
        /* Print the data contained in the message. */
        printf("versionCode was %d!\n", rmessage->versionCode);
	printf("version was %s!\n",rmessage->version);
	 
	free(rmessage);
//		vo.setUuid(uuid);
}

/*获取wifi信息*/
void GWifi(char *msgBytes,int len) {
    
	int i;
	char account[20]="jinjidian005";char password[20]="82378068CK";
	size_t message_length;
        HwGetWifiPush *rmessage=malloc(sizeof(HwGetWifiPush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream,HwGetWifiPush_fields,rmessage);     
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }  
        /* Print the data contained in the message. */
        printf("userId was %d!\n", rmessage->userId);
     /*回推设备wifi信息*/		
	memset(buf,0,sizeof(buf));		
	HwWifiToAppReq *message=malloc(sizeof(HwWifiToAppReq));
	pb_ostream_t stream1 = pb_ostream_from_buffer(buf, sizeof(buf));
	   /* Fill in the lucky number */
	   
	 for(i=0;i<16;i++)
	{ message->serialNum[i]=msgid[i];}
	  message->userId=rmessage->userId;
	  for(i=0;i<20;i++)
	{ message->account[i]=account[i];}
	  for(i=0;i<20;i++)
	{ message->password[i]=password[i];}
	    
	   /* Now we are ready to encode the message! */   
	   status = pb_encode(&stream1, HwWifiToAppReq_fields , message);    
	   message_length = stream1.bytes_written;
	       
	   if (!status)      
	   	{            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream1));  
	   return 1;        }  
	   char *destData = (char*) malloc((60 + message_length) * sizeof(char));
	   //char *srcData= (char*) malloc(128 * sizeof(char));
	   encode(0xf011f031, 1, buf,message_length, destData);
	 
           int length = send(sockee, destData, 60+message_length, 0);
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

	  //net_write(sockee,destData, 60 + message_length);
	 
	  free(rmessage);
	  free(message);
//		vo.setUuid(uuid);
}
/*设置睡眠模式*/
void SMode(char *msgBytes,int len) {
    
	int i;
	
	size_t message_length;
        HwSleepModePush *rmessage=malloc(sizeof(HwSleepModePush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream,HwSleepModePush_fields,rmessage);     
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }  
        /* Print the data contained in the message. */
        printf("sleepMode was %d!\n", rmessage->sleepMode);	   
	
	free(rmessage);
//		vo.setUuid(uuid);
}
/*设备关机*/
void HwClose(char *msgBytes,int len) {
    
	int i;

	size_t message_length;
        HwClosePush *rmessage=malloc(sizeof(HwClosePush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream,HwClosePush_fields,rmessage);     
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }  
        /* Print the data contained in the message. */
        printf("timing was %d!\n", rmessage->timing);	
        memset(buf,0,sizeof(buf));		
	DisconnectReq *message=malloc(sizeof(DisconnectReq));
	pb_ostream_t stream1 = pb_ostream_from_buffer(buf, sizeof(buf));
	   /* Fill in the lucky number */
	   
	for(i=0;i<16;i++)
	{ message->serialNum[i]=msgid[i];}
	  
	    
	   /* Now we are ready to encode the message! */   
	  status = pb_encode(&stream1, DisconnectReq_fields , message);    
	  message_length = stream1.bytes_written;
	       
	  if (!status)      
	   	{            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream1));  
	   return 1;        }  
	   char *destData = (char*) malloc((60 + message_length) * sizeof(char));
	   //char *srcData= (char*) malloc(128 * sizeof(char));
	   encode(0xf011f031, 1, buf,message_length, destData);
	 
	 int length = send(sockee, destData, 60+message_length, 0);
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

	 
	  if(rmessage->timing==0)
	  {//nfl_power_poweroff();
}
       else
	   {//nfl_power_timed_poweroff(rmessage->timing);
}
	free(rmessage);

}
/*设置网络*/
void Swifi(char *msgBytes,int len) {
    
	int i;
	
	size_t message_length;
        HwSetWifiPush *rmessage=malloc(sizeof(HwSetWifiPush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream,HwSetWifiPush_fields,rmessage);     
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }  
        /* Print the data contained in the message. */
        printf("userId was %d!\n", rmessage->userId);
        printf("account was %s!\n", rmessage->account);
        printf("password was %s!\n", rmessage->password);
       // nfl_wifi_configure(rmessage->account, rmessage->password);
	
	free(rmessage);
//		vo.setUuid(uuid);
}

/*dakaijiankong*/
void Mopen(char *msgBytes,int len) {

        int i;

        size_t message_length;
        HwMonitorOpenPush *rmessage=malloc(sizeof(HwMonitorOpenPush));
        pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
        status = pb_decode(&stream,HwMonitorOpenPush_fields,rmessage);
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
        /* Print the data contained in the message. */
        printf("userId was %d!\n", rmessage->userId);
 

        free(rmessage);
//              vo.setUuid(uuid);
}

/*guanbijiankong*/
void Mclose(char *msgBytes,int len) {

        int i;

        size_t message_length;
        HwMonitorClosePush *rmessage=malloc(sizeof(HwMonitorClosePush));
        pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
        status = pb_decode(&stream,HwMonitorClosePush_fields,rmessage);
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
        /* Print the data contained in the message. */
        printf("userId was %d!\n", rmessage->userId);
       

        free(rmessage);
//              vo.setUuid(uuid);
}

/*dakaishipingduijaign*/
void Topen(char *msgBytes,int len) {

        int i;

        size_t message_length;
        HwTalkbackOpenPush *rmessage=malloc(sizeof(HwTalkbackOpenPush));
        pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
        status = pb_decode(&stream,HwTalkbackOpenPush_fields,rmessage);
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
        /* Print the data contained in the message. */
        printf("userId was %d!\n", rmessage->userId);
        printf("streamAddress was %d!\n", rmessage->streamAddress);

        free(rmessage);
//              vo.setUuid(uuid);
}

/*huoquyunxin*/
void IToken(char *accid)
{
        int i;

        size_t message_length;
        memset(buf,0,sizeof(buf));
        ImTokenReq *message=malloc(sizeof(ImTokenReq));
        pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
           /* Fill in the lucky number */

        for(i=0;i<20;i++)
        { message->accid[i]=accid[i];}


           /* Now we are ready to encode the message! */
          status = pb_encode(&stream, ImTokenReq_fields , message);
          message_length = stream.bytes_written;

          if (!status)
         {            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
           return 1;        }
           char *destData = (char*) malloc((60 + message_length) * sizeof(char));
           //char *srcData= (char*) malloc(128 * sizeof(char));
           encode(0xf011f032, 1, buf,message_length, destData);
         int  length = send(sockee, destData, 60+message_length, 0);
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

        free(message);
 
}
/*huoqutuiliudizhi*/
void PAddress(void)
{
       int i;

        size_t message_length;
        memset(buf,0,sizeof(buf));
        PushAddressReq *message=malloc(sizeof(PushAddressReq));
        pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
           /* Fill in the lucky number */

        for(i=0;i<16;i++)
        { message->serialNum[i]=msgid[i];}


           /* Now we are ready to encode the message! */
          status = pb_encode(&stream, PushAddressReq_fields , message);
          message_length = stream.bytes_written;

           if (!status)
           {            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
           return 1;        }
           char *destData = (char*) malloc((60 + message_length) * sizeof(char));
           //char *srcData= (char*) malloc(128 * sizeof(char));
           encode(0xf011f033, 1, buf,message_length, destData);
          int length = send(sockee, destData, 60+message_length, 0);
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

          free(message);

}
/*guanbishipingduijaign*/
void Tclose(char *msgBytes,int len) {

        int i;

        size_t message_length;
        HwTalkbackClosePush *rmessage=malloc(sizeof(HwTalkbackClosePush));
        pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
        status = pb_decode(&stream,HwTalkbackClosePush_fields,rmessage);
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
        /* Print the data contained in the message. */
        printf("userId was %d!\n", rmessage->userId);
        
        free(rmessage);
//              vo.setUuid(uuid);
}


static bool Capacity_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{     
      HwCapacityToAppReq_ObjCapacity *obj=malloc(sizeof(HwCapacityToAppReq_ObjCapacity));
      obj->type=1;
      obj->totalCapacity=8000;
      obj->remainCapacity=4000;
      status=pb_encode_tag_for_field(stream, field);
      if (!status)
      { return false;}

       status=pb_encode_submessage(stream,HwCapacityToAppReq_ObjCapacity_fields, obj);
        if (!status)
        {    return false;}
     free(obj);
     return true;
}

/*获取当前存储情况*/
void Cap(char *msgBytes,int len) {
    
	int i;
	
	size_t message_length;
        HwCapacityPush *rmessage=malloc(sizeof(HwCapacityPush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream,HwCapacityPush_fields,rmessage);     
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }  
        /* Print the data contained in the message. */
        printf("userId was %d!\n", rmessage->userId); 
       /*回复当前存储情况*/		
	memset(buf,0,sizeof(buf));		
	HwCapacityToAppReq *message=malloc(sizeof(HwCapacityToAppReq));
	pb_ostream_t stream1 = pb_ostream_from_buffer(buf, sizeof(buf));
	   /* Fill in the lucky number */
	   
	for(i=0;i<16;i++)
	{ message->serialNum[i]=msgid[i];}
	  message->userId=rmessage->userId;
		 
	  message->objCapacity.funcs.encode = &Capacity_callback; 
	   /* Now we are ready to encode the message! */   
	  status = pb_encode(&stream1, HwCapacityToAppReq_fields , message);    
	  message_length = stream1.bytes_written;
	       
	  if (!status)      
	  {            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream1));  
	   return 1;        }  
	  char *destData = (char*) malloc((60 + message_length) * sizeof(char));
	   //char *srcData= (char*) malloc(128 * sizeof(char));
	  encode(0xf011f031, 1, buf,message_length, destData);
	 
	 int length = send(sockee, destData, 60+message_length, 0);
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

	 	
	
	free(rmessage);
        free(message);
}
static bool addcvoice_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{   size_t message_length; 
    int i;
    int count=0;
   
    HwAddCustomVoicePush_ObjVoice  *obj=malloc(sizeof(HwAddCustomVoicePush_ObjVoice));
    if(!pb_decode(stream,  HwAddCustomVoicePush_ObjVoice_fields,obj)) 
	  return false;
        /* Print the data contained in the message. */
    printf("voiceId was %d!\n",obj->voiceId);
    printf("netPath was %s!\n",obj->voicePath);
    printf("voiceId was %d!\n",obj->voiceType);
    printf("netPath was %d!\n",obj->voiceTime);
    printf("voiceId was %d!\n",obj->orderBy);
    printf("netPath was %s!\n",obj->netPath);
	
		
    free(obj);  
    return true;
}
/*添加自定义声音列表*/
void ACVoice(char *msgBytes,int len) {
    
	int i;

	size_t message_length;
        HwAddCustomVoicePush *rmessage=malloc(sizeof(HwAddCustomVoicePush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	rmessage->objVoice.funcs.decode = &addcvoice_callback;
	status = pb_decode(&stream,HwAddCustomVoicePush_fields,rmessage);
	    
        /* Check for errors... */
       if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
      

	free(rmessage);

}
static bool dcvoice_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{   size_t message_length; 
    int i;
    int count=0;
   
    HwDeleteCustomVoicePush_ObjVoice  *obj=malloc(sizeof(HwDeleteCustomVoicePush_ObjVoice));
    if(!pb_decode(stream,HwDeleteCustomVoicePush_ObjVoice_fields,obj))  
	  return false;
        /* Print the data contained in the message. */
    printf("voiceId was %d!\n",obj->voiceId);
    printf("voiceType was %d!\n",obj->voiceType);
	
		
    free(obj);  
    return true;
}
/*删除自定义声音列表*/
void DCVoice(char *msgBytes,int len) {
    
	int i;
	
    size_t message_length;
    HwDeleteCustomVoicePush *rmessage=malloc(sizeof(HwDeleteCustomVoicePush));
	
    pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
    rmessage->objVoice.funcs.decode = &dcvoice_callback;
    status=pb_decode(&stream, HwDeleteCustomVoicePush_fields,rmessage);  	
        /* Check for errors... */
    if (!status)
    {
     printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
     }
     
      	
     free(rmessage);

}
/*修改自定义列表声音顺序*/
void CLOrder(char *msgBytes,int len) {
    
	int i;

	size_t message_length;
        HwChangeListOrderPush *rmessage=malloc(sizeof(HwChangeListOrderPush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream, HwChangeListOrderPush_fields,rmessage);

	
        
		
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
     
        /* Print the data contained in the message. */
       // wmprintf("voiceId was %d!\n", rmessage->dlType);
	    printf("relType was %d!\n",rmessage->relType);
	    printf("relId was %s!\n",rmessage->relId);
	    printf("allCount was %d!\n",rmessage->allCount);
			

	free(rmessage);

}

static bool cvoice_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{   size_t message_length; 
    int i;
    int count=0;
   
    HwCacheVoicePush_ObjVoice  *obj=malloc(sizeof(HwCacheVoicePush_ObjVoice));
    if(!pb_decode(stream, HwCacheVoicePush_ObjVoice_fields,obj)) 
	  return false;
        
        printf("voiceId was %d!\n",obj->voiceId);
	printf("voicePath was %s!\n",obj->voicePath);
	printf("voiceType was %d!\n",obj->voiceType);
	
		
	free(obj);  
        return true;
}
/*缓存声音到设备*/
void CVoice(char *msgBytes,int len) {
    
    int i;

    size_t message_length;
    HwCacheVoicePush *rmessage=malloc(sizeof(HwCacheVoicePush));
	
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	rmessage->objVoice.funcs.decode = &cvoice_callback;
	status=pb_decode(&stream, HwCacheVoicePush_fields,rmessage);		
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
      
      
			
		
	free(rmessage);

}

static bool ddvoice_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{   size_t message_length; 
    int i;
    int count=0;
   
    HwDeleteDownloadedVoicePush_ObjVoice  *obj=malloc(sizeof(HwDeleteDownloadedVoicePush_ObjVoice));
      if(!  pb_decode(stream, HwDeleteDownloadedVoicePush_ObjVoice_fields,obj))  
	  return false;
        
        printf("voiceId was %d!\n",obj->voiceId);
	printf("voiceType was %d!\n",obj->voiceType);
	
		
	free(obj);  
    return true;
}

/*删除设备缓存声音*/
void DDVoice(char *msgBytes,int len) {
    
	int i;
	
	size_t message_length;
    HwDeleteDownloadedVoicePush *rmessage=malloc(sizeof(HwDeleteDownloadedVoicePush));
	
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	rmessage->objVoice.funcs.decode = &cvoice_callback;	
	status=pb_decode(&stream,HwDeleteDownloadedVoicePush_fields,rmessage);
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
        
        /* Print the data contained in the message. */
        printf("voiceIds was %d!\n", rmessage->voiceIds);
	  		
	free(rmessage);

}
static bool dsdvoice_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{   size_t message_length; 
    int i;
    int count=0;
   
    HwDeleteSDVoicePush_ObjVoice  *obj=malloc(sizeof( HwDeleteSDVoicePush_ObjVoice));
    if(!pb_decode(stream, HwDeleteSDVoicePush_ObjVoice_fields,obj) )  
	return false;
        /* Print the data contained in the message. */
        printf("voiceId was %d!\n",obj->voiceId);
	printf("voiceName was %s!\n",obj->voiceName);
	
		
	free(obj);  
    return true;
}
/*删除SD声音*/
void DSDVoice(char *msgBytes,int len) {
    
	int i;
	
	size_t message_length;
    HwDeleteSDVoicePush *rmessage=malloc(sizeof(HwDeleteSDVoicePush));	 
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	rmessage->objVoice.funcs.decode = &dsdvoice_callback;	
	status=pb_decode(&stream, HwDeleteSDVoicePush_fields,rmessage);
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
        
      
	  
			
		
	free(rmessage);

}
/*添加硬件讨论群     */
void ADiscuss(char *msgBytes,int len) {
    char s1[100]="http://www.1shuo.com/";
	int i;
	
	size_t message_length;
    HwAddDiscussPush *rmessage=malloc(sizeof(HwAddDiscussPush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream, HwAddDiscussPush_fields,rmessage);

	
        
		
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
     
        /* Print the data contained in the message. */
       // wmprintf("voiceId was %d!\n", rmessage->dlType);
	    printf("discussId was %d!\n",rmessage->discussId);
	    printf("discussContent was %s!\n",rmessage->discussContent);
	    printf("createTime was %d!\n",rmessage->createTime);
	    strcat(s1,rmessage->discussContent);
		//nfl_player_play(s1, 1, 0);
	    for(i=0;i<100;i++)
	    {DvoiceRe[i]=s1[1];}
			
		
	free(rmessage);

}
/*设备状态查询*/
void HStatus(char *msgBytes,int len) {
    
	int i;
	
	size_t message_length;
    HwStatusPush *rmessage=malloc(sizeof(HwStatusPush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream, HwStatusPush_fields,rmessage);

	
        
		
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
     
        /* Print the data contained in the message. */
      
	    printf("userId was %d!\n",rmessage->userId);
	/*硬件返回当前设备状况*/
	    memset(buf,0,sizeof(buf));
            HwStatusToAppReq *message=malloc(sizeof(HwStatusToAppReq));
	    pb_ostream_t stream1 = pb_ostream_from_buffer(buf, sizeof(buf));
	   /* Fill in the lucky number */
	   
	  for(i=0;i<16;i++)
	  { message->serialNum[i]=msgid[i];}
	  message->userId=1;
	  message->voiceId=1;
	  message->voiceType=1;
	  message->playMode=1;
	  message->volume=1;
	  message->status=1;
	  message->relType=1;
	  message->power=1;
          message->powerStatus=1;
          message->sleepMode=1;
          message->timing=1; 
	  for(i=0;i<10;i++)
	  {message->relId[i]=relId[i];}
	   /* Now we are ready to encode the message! */   
	  status = pb_encode(&stream1, HwStatusToAppReq_fields , message);    
	  message_length = stream1.bytes_written;
	       
	   if (!status)      
	   {  printf("Encoding failed: %s\n", PB_GET_ERROR(&stream1));  
	   return 1;        }  
	   char *destData = (char*) malloc((60 + message_length) * sizeof(char));
	   encode(0xf011f031, 1, buf,message_length, destData);
	  
	 int length = send(sockee, destData, 60+message_length, 0);
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

	  
		
	free(rmessage);
	free(message);

}
/*更改昵称*/
void CName(char *msgBytes,int len) {
    
	int i;
	
	size_t message_length;
        HwChangeNamePush *rmessage=malloc(sizeof(HwChangeNamePush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream, HwChangeNamePush_fields,rmessage);

	
        
		
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
     
        /* Print the data contained in the message. */
       // wmprintf("voiceId was %d!\n", rmessage->dlType);
	    printf("userId was %d!\n",rmessage->userId);
	    printf("userId was %s!\n",rmessage->nickname);		
		
	free(rmessage);

}
/*更改主控*/
void Amaster(char *msgBytes,int len) {
    
	int i;
	
	size_t message_length;
        AlterMasterPush *rmessage=malloc(sizeof(AlterMasterPush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream, AlterMasterPush_fields,rmessage);
		
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
     
        /* Print the data contained in the message. */
       // wmprintf("voiceId was %d!\n", rmessage->dlType);
	    printf("newMasterId was %d!\n",rmessage->newMasterId);		
		
	free(rmessage);

}
/*添加到播放列表*/
void Avoice(char *msgBytes,int len) {
    
	int i;
	
	size_t message_length;
        HwAddVoicePush *rmessage=malloc(sizeof(HwAddVoicePush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream,HwAddVoicePush_fields,rmessage);
		
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
     
        /* Print the data contained in the message. */
       // wmprintf("voiceId was %d!\n", rmessage->dlType);
	    printf("voiceIds was %d!\n",rmessage->voiceIds);		
		
	free(rmessage);

}
/*从播放列表删除*/
void Dvoice(char *msgBytes,int len) {
    
	int i;
	
	size_t message_length;
        HwDeleteVoicePush *rmessage=malloc(sizeof(HwDeleteVoicePush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream, HwDeleteVoicePush_fields,rmessage);
		
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
     
        /* Print the data contained in the message. */
       // wmprintf("voiceId was %d!\n", rmessage->dlType);
	    printf("voiceIds was %d!\n",rmessage->voiceIds);		
		
	free(rmessage);

}
/*改变播放模式*/
void Pmode(char *msgBytes,int len) {
    
	int i;
	
	size_t message_length;
        HwPlayModePush *rmessage=malloc(sizeof(HwPlayModePush));
	pb_istream_t stream = pb_istream_from_buffer(msgBytes,len);
	status = pb_decode(&stream, HwPlayModePush_fields,rmessage);
		
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            //return 1;
        }
     
        /* Print the data contained in the message. */
       // wmprintf("voiceId was %d!\n", rmessage->dlType);
	    printf("playMode was %d!\n",rmessage->playMode);		
		
	free(rmessage);
//		vo.setUuid(uuid);
}

/*硬件返回声音播放信息*/
void Vinfo(int pushUserId,int voiceId,int voiceType,int relType  )
{   int i;
	
	size_t message_length;
	memset(buf,0,sizeof(buf));
	HwVoiceInfoReq *message=malloc(sizeof(HwVoiceInfoReq));
        pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
	   /* Fill in the lucky number */
	   
	   message->pushUserId=pushUserId;
	   message->voiceId=voiceId;
	   message->voiceType=voiceType;
	   message->relType=relType;
	  for(i=0;i<10;i++)
	   {message->relId[i]=relId[i];}
	    
	   /* Now we are ready to encode the message! */   
	   status = pb_encode(&stream, HwVoiceInfoReq_fields , message);    
	   message_length = stream.bytes_written;
	       
	   if (!status)      
	   { printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));  
	   return 1;        }  
	   char *destData = (char*) malloc((60 + message_length) * sizeof(char));
	   encode(0xf011f006, 1, buf,message_length, destData);
	  
	  int length = send(sockee, destData, 60+message_length, 0);
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

	 
	  free(message);
}
/*硬件返回声音错误播放信息*/
void VEinfo(int pushUserId,int voiceId,int voiceType,int relType,char *error )
{   int i;
	
	size_t message_length;
	memset(buf,0,sizeof(buf));
	HwVoiceErrorInfoReq *message=malloc(sizeof(HwVoiceErrorInfoReq));
        pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
	   /* Fill in the lucky number */
	   
	   message->pushUserId=pushUserId;
	   message->voiceId=voiceId;
	   message->voiceType=voiceType;
	   message->relType=relType;	 
	  for(i=0;i<10;i++)
	  {message->relId[i]=relId[i];}
	  for(i=0;i<10;i++)
	  {message->error[i]=error[i];} 
	   /* Now we are ready to encode the message! */   
	   status = pb_encode(&stream, HwVoiceInfoReq_fields , message);    
	   message_length = stream.bytes_written;
	       
	   if (!status)      
	   	{            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));  
	   return 1;        }  
	   char *destData = (char*) malloc((60 + message_length) * sizeof(char));
	   encode(0xf011f006, 1, buf,message_length, destData);
	 
	  int length = send(sockee, destData, 60+message_length, 0);
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

	  
	  free(message);
}
/*硬件拉取指定列表声音*/
void PVlist(int page,int pageSize,int relType,char *relId  )
{int i;
	
	size_t message_length;
	memset(buf,0,sizeof(buf));
	HwPullVoiceListReq *message=malloc(sizeof(HwPullVoiceListReq));
        pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
	   /* Fill in the lucky number */
	   
	   message->page=page;
	   message->pageSize=pageSize;
	   message->relType=relType;	 
	   for(i=0;i<10;i++)
		{message->relId[i]=relId[i];}
	   for(i=0;i<16;i++)
	   { message->serialNum[i]=msgid[i];}
	   /* Now we are ready to encode the message! */   
	   status = pb_encode(&stream, HwVoiceInfoReq_fields , message);    
	   message_length = stream.bytes_written;
	       
	   if (!status)      
	   	{            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));  
	   return 1;        }  
	   char *destData = (char*) malloc((60 + message_length) * sizeof(char));
	   encode(0xf011f006, 1, buf,message_length, destData);
	 
	  int length = send(sockee, destData, 60+message_length, 0);
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

	  
	  free(message);
	
	
}
/*硬件推送操作结果*/
void Ostatus(int userId,int operateType,int voiceId,char *ext  )
{   int i;
	
	size_t message_length;
	memset(buf,0,sizeof(buf));
	HwOperateStatusReq *message=malloc(sizeof(HwOperateStatusReq));
        pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
	   /* Fill in the lucky number */
	   
	   message->userId=userId;
	   message->operateType=operateType;
	   message->voiceId=voiceId;
           for(i=0;i<10;i++) 	   
	   {message->ext[i]=ext[i];}
	   for(i=0;i<16;i++)
	   { message->serialNum[i]=msgid[i];}
	   /* Now we are ready to encode the message! */   
	   status = pb_encode(&stream, HwOperateStatusReq_fields , message);    
	   message_length = stream.bytes_written;
	       
	   if (!status)      
	   	{            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));  
	   return 1;        }  
	   char *destData = (char*) malloc((60 + message_length) * sizeof(char));
	   encode(0xf011f019, 1, buf,message_length, destData);
	  
	  int length = send(sockee, destData, 60+message_length, 0);
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

	 
	  free(message);
	
	
}

/*硬件状态改变*/
void Cstatus(int state,int reqTime)
{   int i;
	
	size_t message_length;
	memset(buf,0,sizeof(buf));
	HwChangeStatusReq *message=malloc(sizeof(HwChangeStatusReq));
    pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
	   /* Fill in the lucky number */
	   
	   message->state=state;
	   message->reqTime=reqTime;
	   for(i=0;i<16;i++)
	   { message->serialNum[i]=msgid[i];}
	  	    
	   /* Now we are ready to encode the message! */   
	   status = pb_encode(&stream, HwChangeStatusReq_fields , message);    
	   message_length = stream.bytes_written;
	       
	   if (!status)      
	   	{            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));  
	   return 1;        }  
	   char *destData = (char*) malloc((60 + message_length) * sizeof(char));
	   encode(0xf011f009, 1, buf,message_length, destData);
	  
	  int length = send(sockee, destData, 60+message_length, 0);
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

	  
	  free(message);
}
/*硬件拉取群讨论*/
void Pdiscuss(int reqTime)
{   int i;
	
	size_t message_length;
	memset(buf,0,sizeof(buf));
	HwPullDiscussReq *message=malloc(sizeof(HwPullDiscussReq));
        pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
	   /* Fill in the lucky number */
	   
	   message->reqTime=reqTime;
	   for(i=0;i<16;i++)
	  { message->serialNum[i]=msgid[i];}
	  
	    
	   /* Now we are ready to encode the message! */   
	   status = pb_encode(&stream, HwPullDiscussReq_fields , message);    
	   message_length = stream.bytes_written;
	       
	   if (!status)      
	   	{            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));  
	   return 1;        }  
	   char *destData = (char*) malloc((60 + message_length) * sizeof(char));
	   encode(0xf011f037, 1, buf,message_length, destData);
	  
	  int length = send(sockee, destData, 60+message_length, 0);
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

	 
	  free(message);
}

/*硬件拉取群声音*/
void Pvoice(int reqTime )
{   int i;
	
	size_t message_length;
	memset(buf,0,sizeof(buf));
	HwPullVoiceReq *message=malloc(sizeof(HwPullVoiceReq));
        pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
	   /* Fill in the lucky number */
	   
	   message->reqTime=reqTime;
	   for(i=0;i<16;i++)
	   { message->serialNum[i]=msgid[i];}
	  
	    
	   /* Now we are ready to encode the message! */   
	   status = pb_encode(&stream, HwPullVoiceReq_fields , message);    
	   message_length = stream.bytes_written;
	       
	   if (!status)      
	   	{            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));  
	   return 1;        }  
	   char *destData = (char*) malloc((60 + message_length) * sizeof(char));
	   encode(0xf011f039, 1, buf,message_length, destData);
	  
	  int length = send(sockee, destData, 60+message_length, 0);
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

	
	  free(message);
}
/*本地专辑下载状态*/
void DLstatus(void )
{   int i;
	
	size_t message_length;
	memset(buf,0,sizeof(buf));
	HwDLLocalStatusReq *message=malloc(sizeof(HwDLLocalStatusReq));
        pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
	   /* Fill in the lucky number */
	   
	    for(i=0;i<16;i++)
	   { message->serialNum[i]=msgid[i];}
	  
	    
	   /* Now we are ready to encode the message! */   
	   status = pb_encode(&stream, HwDLLocalStatusReq_fields , message);    
	   message_length = stream.bytes_written;
	       
	   if (!status)      
	   	{            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));  
	   return 1;        }  
	   char *destData = (char*) malloc((60 + message_length) * sizeof(char));
	   encode(0xf011f069, 1, buf,message_length, destData);
	 
	  int length = send(sockee, destData, 60+message_length, 0);
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

	 
	  free(message);
}

static bool SDvoice_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{     int i;
      char voicename[64]="lalala";
      HwSdVoiceAddReq_ObjVoice *obj=malloc(sizeof(HwSdVoiceAddReq_ObjVoice));
      obj->voiceId=voiceid;
	  obj->voiceLength=5;
	  for(i=0;i<64;i++)
	  {obj->voiceName[i]=voicename[i];}
	  status=pb_encode_tag_for_field(stream, field);
	   if (!status)
	   { return false;}

       status=pb_encode_submessage(stream, HwSdVoiceAddReq_ObjVoice_fields, obj);
        if (!status)
        {    return false;}
     free(obj);
     return true;
}
/*硬件推送SD卡声音*/
void SDvoiceadd(void )
{   int i;
	
	size_t message_length;
	memset(buf,0,sizeof(buf));
	HwSdVoiceAddReq *message=malloc(sizeof( HwSdVoiceAddReq));
        pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
	   /* Fill in the lucky number */
	   
	    for(i=0;i<16;i++)
	   { message->serialNum[i]=msgid[i];}
       message->isReset=0;
	   message->objVoice.funcs.encode = &SDvoice_callback;
	   /* Now we are ready to encode the message! */   
	   status = pb_encode(&stream, HwSdVoiceAddReq_fields , message);    
	   message_length = stream.bytes_written;
	       
	   if (!status)      
	   {            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));  
	   return 1;        }  
	   char *destData = (char*) malloc((60 + message_length) * sizeof(char));
	   encode(0xf011f076, 1, buf,message_length, destData);
	  
	  int length = send(sockee, destData, 60+message_length, 0);
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

	  
	  free(message);
}
/*推送电量与睡眠状态*/
void Powerstatus(int power,int powerStatus,int sleepMode,int timing)
{   int i;
	
	size_t message_length;
	memset(buf,0,sizeof(buf));
	HwPowerStatusReq *message=malloc(sizeof( HwPowerStatusReq));
    pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
	   /* Fill in the lucky number */
	   
       message->power=power;
       message->powerStatus=powerStatus;
       message->sleepMode=sleepMode;
       message->timing=timing;	 
       for(i=0;i<16;i++)
	   { message->serialNum[i]=msgid[i];}
	   /* Now we are ready to encode the message! */   
	   status = pb_encode(&stream, HwPowerStatusReq_fields , message);    
	   message_length = stream.bytes_written;
	       
	   if (!status)      
	   	{            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));  
	   return 1;        }  
	   char *destData = (char*) malloc((60 + message_length) * sizeof(char));
	   encode(0xf011f076, 1, buf,message_length, destData);
	  
	  int length = send(sockee, destData, 60+message_length, 0);
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

	  
	  free(message);
}
