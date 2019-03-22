#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>

//#define SERVER_PORT 8888
#define MAX_BUG_SIZE 1024
int main(int argc, char **argv)
{
	int sockfd,port,addrlen,n;
	char buffer[MAX_BUG_SIZE];
	struct sockaddr_in addr;
	if(argc!=3)
	{
	fprintf(stderr,"usage:%s server ip  server_port\n",argv[0]);
	exit(1);
	}
	if((port=atoi(argv[2]))<0)
	{
		fprintf(stderr,"usage:%s server ip  server_port\n",argv[0]); 
		exit(1);
	}
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
	{
		fprintf(stderr,"socket error :%s\n",strerror(errno));
		exit(1);
	}
	addrlen=sizeof(struct sockaddr_in);
	bzero(&addr,addrlen);
	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	if(inet_aton(argv[1],&addr.sin_addr)<0)
	{
		fprintf(stderr,"ip error:%s\n",strerror(errno));
		exit(1);
	}
	while(1)
	{
		bzero(buffer,MAX_BUG_SIZE);
		fgets(buffer,MAX_BUG_SIZE,stdin);
		sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr *)(&addr),addrlen);
		printf("client endpoint input message:%s",buffer);
		n=recvfrom(sockfd,buffer,strlen(buffer),0,(struct sockaddr *)(&addr),&addrlen);
		fprintf(stdout,"receive message  from server:%s",buffer);
	}
	close(sockfd);
	exit(0);
}