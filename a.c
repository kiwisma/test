#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
int timeIsOver=0;
void test_func(void)
{
	timeIsOver =1;
	printf("
}

void init_sigaction()
{
    struct sigaction act;
          
    act.sa_handler = test_func; //设置处理信号的函数
    act.sa_flags  = 0;

    sigemptyset(&act.sa_mask);
    sigaction(SIGPROF, &act, NULL);//时间到发送SIGROF信号
}

void init_time()
{
    struct itimerval val;
         
    val.it_value.tv_sec = 1; //1秒后启用定时器
    val.it_value.tv_usec = 0;

    val.it_interval = val.it_value; //定时器间隔为1s

    setitimer(ITIMER_PROF, &val, NULL);
}

int main(int argc, char **argv)
{
	//	int fcip_1,fcip_2,fcip_3,fcip_4;
	int sockfd,addrlen,n;
	struct sockaddr_in addr;
	char msg[MAX_MSG_SIZE];
	char cmd[CMD_LENGTH];
	char msgDetail[CMD_DETAIL_LENGTH];
	char msgSend[MSG_SEND_LEN];
	struct config_struct *currSettings;
	struct config_struct *udpSettings;
	
	configStructInit(currSettings);
	configStructInit(udpSettings);
	
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
	

    init_sigaction();
    init_time();

    while(1);

    return 0;
}