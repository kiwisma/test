#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#define SERVER_PORT 8888
#define MAX_MSG_SIZE 1024
int main(void)
{
//	int fcip_1,fcip_2,fcip_3,fcip_4;
	int sockfd,addrlen,n;
	struct sockaddr_in addr;
	char msg[MAX_MSG_SIZE];
	char fileName[CONFIG_FILE_LEN]={"ifcfg-fc"};
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
		exit(1);
	}
	while(1)
	{
		bzero(msg,MAX_MSG_SIZE);
		n=recvfrom(sockfd,msg,sizeof(msg),0,(struct sockaddr*)(&addr),&addrlen);
		fprintf(stdout,"receive message from client:%s",msg);
		fgets(msg,MAX_MSG_SIZE,stdin);
		printf("server endpoint input message :%s",msg);
		sendto(sockfd,msg,n,0,(struct sockaddr *)(&addr),addrlen);
	}
		close(sockfd);
		exit(0);
}

