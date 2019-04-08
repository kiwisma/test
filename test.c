#include <fckfqcmd.h>

#ifdef READ_CONFIG
int fcEthernetConfigRead(struct config_struct *destConfigSettings)
{

}
#endif

void poweroff(void) //关机函数
{
	system("sync"); 
	printf("poweroff now\n");
	system("halt"); 
}
void configStructInit(struct config_struct *configSettings)
{
	configSettings->channelNo = '0';
	strcpy(configSettings->ipAddrSet,"0");
	strcpy(configSettings->nodeNameSet,"0");
	strcpy(configSettings->portNameSet,"0");
	strcpy(configSettings->netMaskSet,"0");
	strcpy(configSettings->routeSwitchSet,"0");
	strcpy(configSettings->routeSet,"0");
}

int fcEthernetConfigSet(struct config_struct *destConfigSettings)
{

	char fileName[CONFIG_FILE_LEN]={FILENAME_PREFIX};
	char linebuffer[CONFIG_LINE];
	char buffer1[CONFIG_LINE],buffer2[CONFIG_LINE];
	int fd;
	
	/*
	char configFile[CONFIG_ROW][CONFIG_LINE];
	char ipAddrSet[IPADDR_LEN];
	char nodeNameSet[NODENAME_LEN];
	char portNameSet[PORTNAME_LEN];
	char netMaskSet[NETMASK_LEN];
	char routeSwitchSet[ROUTESWITCH_LEN];
	char routeSet[ROUTE_SET_LEN];
	*/
	int i=0,j;
	int rowNum=0;
	strncat(fileName,destConfigSettings,1);
	printf(":%s\n",fileName);
	FILE *fp = fopen("/home/bica/work/configapp/ifcfg-fc2","r+");
	if(fp == NULL)
	{
		printf("open error");
		return -1;
	}
	while(fgets(configFile[i],CONFIG_LINE,fp))
	{
		sscanf(configFile[i], "%[^=]=%[^=]", buffer1, buffer2 );
		printf("buffer1: %s , buffer2 :%s",buffer1, buffer2);
	
		if(strncmp(buffer1,IPADDR_PREFIX,strlen(IPADDR_PREFIX))==0)
		{
			if(strcmp(destConfigSettings->ipAddrSet,buffer2))//判断是否ip地址一致，如果不一致则进行以下操作
			{
				strcpy(buffer2,destConfigSettings->ipAddrSet);
				buffer1[strlen(buffer1)] = '=';
				buffer2[strlen(buffer2)] = '\n';
				strncat(buffer1,buffer2,strlen(buffer2));
				printf("buffer1: set to %s",buffer1);				

			}
			else
			{
				printf("IP address not changed\n");
			}
//			printf("changed string %s\n",configFile[i]);
		}
		if(strncmp(buffer1,NODENAME_PREFIX,strlen(NODENAME_PREFIX)) == 0)//Nodename处理部分
		{
			if(strncmp(destConfigSettings->nodeNameSet,buffer2))//判断nodename是否变化
			{
				strcpy(buffer2,destConfigSettings->nodeNameSet);
				buffer1[strlen(buffer1)] = '=';
				buffer2[strlen(buffer2)] = '\n';
				strncat(buffer1,buffer2,strlen(buffer2));
				printf("buffer1: set to %s",buffer1);						
			}
			else{
					printf("Node name not changed\n");
			}
		}	
		if(strncmp(buffer1,PORTNAME_PREFIX,strlen(PORTNAME_PREFIX)) == 0)//portname处理部分
		{
			if(strncmp(destConfigSettings->portNameSet,buffer2))//判断portname是否变化
			{
				strcpy(buffer2,destConfigSettings->portNameSet);
				buffer1[strlen(buffer1)] = '=';
				buffer2[strlen(buffer2)] = '\n';
				strncat(buffer1,buffer2,strlen(buffer2));
				printf("buffer1: set to %s",buffer1);						
			}
			else{
					printf("Port name not changed\n");
			}
		}		
		if(strncmp(buffer1,NETMASK_PREFIX,strlen(NETMASK_PREFIX)) == 0)//子网掩码处理部分
		{
			if(strncmp(destConfigSettings->netMaskSet,buffer2))//判断子网掩码是否变化
			{
				strcpy(buffer2,destConfigSettings->netMaskSet);
				buffer1[strlen(buffer1)] = '=';
				buffer2[strlen(buffer2)] = '\n';
				strncat(buffer1,buffer2,strlen(buffer2));
				printf("buffer1: set to %s",buffer1);						
			}
			else{
					printf("Netmask not changed\n");
			}
		}
		if(strncmp(buffer1,ROUTESWITCH_PREFIX,strlen(ROUTESWITCH_PREFIX)) == 0)//判断路由是否打开
		{
			if(strncmp(destConfigSettings->routeSwitchSet,buffer2))
			{
				strcpy(buffer2,destConfigSettings->routeSwitchSet);
				buffer1[strlen(buffer1)] = '=';
				buffer2[strlen(buffer2)] = '\n';
				strncat(buffer1,buffer2,strlen(buffer2));
				printf("buffer1: set to %s",buffer1);						
			}
			else{
					printf("routeSwitchSet not changed\n");
			}
		}
		if(strncmp(buffer1,ROUT_PREFIX,strlen(ROUT_PREFIX)) == 0)//路由关系设定部分
		{
			if(strncmp(destConfigSettings->routeSet,buffer2))
			{
				strcpy(buffer2,destConfigSettings->routeSet);
				buffer1[strlen(buffer1)] = '=';
				buffer2[strlen(buffer2)] = '\n';
				strncat(buffer1,buffer2,strlen(buffer2));
				printf("buffer1: set to %s",buffer1);						
			}
			else{
					printf("routeSwitchSet not changed\n");
			}
		}							
		

				strcpy(configFile[i],buffer1);
				printf("found: buffer1 length %d set to %s",(int)(strlen(buffer1)),configFile[i]);
		i++;
	}
	rowNum=i;//记录配置文件总行数
	fseek ( fp,0,SEEK_SET );//重新定位到文件开始处
    for(j=0;j<rowNum;j++)
   	{
        fwrite(configFile[j],strlen(configFile[j]),1,fp);
    }
	printf("close file pointer\n");
	fclose(fp);
    return 0;
}