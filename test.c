#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define MAX_MSG_SIZE 1024
#define CONFIG_FILE_LEN 15
#define CONFIG_ROW	6
#define CONFIG_LINE	50
#define NODENAME_LEN 40
#define PORTNAME_LEN 40
#define NODENAME_PREFIX 	"Node_Name="
#define PORTNAME_PREFIX 	"Port_Name="
#define IPADDR_PREFIX		"fcIpAddr="
#define NETMASK_PREFIX		"netMask="
#define ROUTESWITCH_PREFIX	"routeSwitch="
#define	ROUT_PREFIX			"route="

int main(void)
{

	char fileName[CONFIG_FILE_LEN]={"/etc/ifcfg-fc"};
	int fd;
	char fcNum='1';
	char configFile[CONFIG_ROW][CONFIG_LINE];
	char nodeName[NODENAME_LEN]={"BXKFQ_00"};
	char portName[PORTNAME_LEN]={"KF_Port1"};
//	char *configFile="0";
	int i=0;
//	memset(fileName,0,CONFIG_FILE_LEN);
//	fileName = "ifcfg-fc";
//	fcNum=0;
//	(unsigned char *)(fileName+9) = &fcNum;
	strncat(fileName,&fcNum,1);
	printf(":%s\n",fileName);
/*	
	fd = open(fileName,O_WRONLY|O_CREAT|O_APPEND);
	if(fd == -1)
    {
        printf("Failed to open");
        return -1;
    }
*/

	//FILE *fp = fopen(fileName,"r+");
	FILE *fp = fopen("/etc/ifcfg-fc1","r+");
	printf("open /home/bica/hello.c\n");
	if(fp == NULL)
	{
		printf("open error");
		return -1;
	}
	while(i<6)
	{

		fgets(configFile[i],CONFIG_LINE,fp);

		printf("ifconfig: %s\n",configFile[i]);
		i++;
	}
/*	
	while(fgets(configFile, CONFIG_LINE, fp))
	{
		printf("ifconfig %i:%s\n",i,configFile);
    }
*/	
	printf("close file pointer\n");
	fclose(fp);

	/*
    write(fd,buf,MAX_MSG_SIZE);
	*/
    return 0;
}