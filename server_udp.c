
#include "fckfqcmd.h"

struct sockaddr_in addr;
int addrlen;
int main(void)
{
//	int fcip_1,fcip_2,fcip_3,fcip_4;
	int sockfd,n;

	char msg[MAX_MSG_SIZE];
	char cmd[CMD_LENGTH];
	char msgDetail[CMD_DETAIL_LENGTH];
	char msgSend[MSG_SEND_LEN];
//	struct config_struct *currSettings;
//	struct config_struct *udpSettings;
	pthread_t tid1;
	int thread_arg,err;
//	configStructInit(currSettings);
//	configStructInit(udpSettings);
	
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
	{
		fprintf(stderr,"socket Error:%s\n",strerror(errno));
		exit(1);
	}
	addrlen=sizeof(struct sockaddr_in);
	bzero(&addr,addrlen);
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port=htons(SERVER_PORT);
	if(bind(sockfd,(struct sockaddr *)(&addr),addrlen)<0)
	{
		fprintf(stderr,"Bind error %s\n",strerror(errno));
		close(sockfd);
		exit(1);
	}
	//创建发送线程
	thread_arg = sockfd;
	err=pthread_create(&tid1,NULL,(void*)msg_send_thread,(void *)&thread_arg);
    if(err!=0)
    {
        perror("pthread not create\n");
		close(sockfd);
        exit(-1);
    }
	err = pthread_join(tid1,NULL);
    if(err!=0)
    {
        perror("pthread join failed\n");
		close(sockfd);
        exit(-1);
    }	
	while(1)
	{
		bzero(msg,MAX_MSG_SIZE);
		n=recvfrom(sockfd,msg,sizeof(msg),0,(struct sockaddr*)(&addr),&addrlen);
		fprintf(stdout,"receive message from client:%s",msg);
		sscanf(msg,"%s %s",cmd,msgDetail);
		if(!strcmp(cmd,CMD_SHUTDOWN))
		{
			close(sockfd);
			poweroff();
		}
		else
		{
			parsCmd(cmd,msgDetail,msgSend);
		}
		printf("server endpoint input message :%s",msgSend);
		sendto(sockfd,msgSend,strlen(msgSend),0,(struct sockaddr *)(&addr),addrlen);
	}
	
	close(sockfd);
	exit(0);
}



