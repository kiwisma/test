#include "fckfqcmd.h"
extern struct sockaddr_in addr;
extern int addrlen;
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
#if 0 //初始化函数-备用
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
#endif
int fcConfigSet(unsigned int cmdtype,const char *channelNo,const char *msgDetail)
{

	char fileName[CONFIG_FILE_LEN]={FILENAME_PREFIX};
	char linebuffer[CONFIG_LINE];
	char buffer1[CONFIG_LINE],buffer2[CONFIG_LINE];
	char configFile[CONFIG_ROW][CONFIG_LINE];
	int fd;
	int i=0,j;
	int rowNum=0;
	if((*channelNo>=CHANNEL_0)&&(*channelNo<=CHANNEL_3)) //判断是否为ifcfg-fc文件，如果是，则进行如下操作
	{
		strncat(fileName,channelNo,1);
		printf(":%s\n",fileName);
		FILE *fp = fopen(fileName,"r+");
		if(fp == NULL)
		{
			printf("open error\n");
			return FAIL_OPEN;
		}
		while(fgets(configFile[i],CONFIG_LINE,fp))
		{
			sscanf(configFile[i], "%[^=]=%[^=]", buffer1, buffer2 );
			printf("buffer1: %s , buffer2 :%s",buffer1, buffer2);
			switch(cmdtype)	
			{
				case SET_FC_IP_ADDR: 
				if(strncmp(buffer1,IPADDR_PREFIX,strlen(IPADDR_PREFIX))==0)
				{
				
					if(strcmp(msgDetail,buffer2))//判断是否ip地址一致，如果不一致则进行以下操作
					{
						strcpy(buffer2,msgDetail);
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
				break;
				case SET_NODENAME: 
					if(strncmp(buffer1,NODENAME_PREFIX,strlen(NODENAME_PREFIX)) == 0)//Nodename处理部分
					{
					if(strcmp(msgDetail,buffer2))//判断nodename是否变化
					{
						strcpy(buffer2,msgDetail);
						buffer1[strlen(buffer1)] = '=';
						buffer2[strlen(buffer2)] = '\n';
						strncat(buffer1,buffer2,strlen(buffer2));
						printf("buffer1: set to %s",buffer1);						
					}
					else{
						printf("Node name not changed\n");
					}
				}	
				break;
				case SET_PORTNAME: 	
				if(strncmp(buffer1,PORTNAME_PREFIX,strlen(PORTNAME_PREFIX)) == 0)//portname处理部分
				{
					if(strcmp(msgDetail,buffer2))//判断portname是否变化
					{
						strcpy(buffer2,msgDetail);
						buffer1[strlen(buffer1)] = '=';
						buffer2[strlen(buffer2)] = '\n';
						strncat(buffer1,buffer2,strlen(buffer2));
						printf("buffer1: set to %s",buffer1);						
					}
					else{
							printf("Port name not changed\n");
					}
				}		
				case SET_NETMASK:
					if(strncmp(buffer1,NETMASK_PREFIX,strlen(NETMASK_PREFIX)) == 0)//子网掩码处理部分
					{
						if(strcmp(msgDetail,buffer2))//判断子网掩码是否变化
						{
							strcpy(buffer2,msgDetail);
							buffer1[strlen(buffer1)] = '=';
							buffer2[strlen(buffer2)] = '\n';
							strncat(buffer1,buffer2,strlen(buffer2));
							printf("buffer1: set to %s",buffer1);						
						}
						else{
							printf("Netmask not changed\n");
						}
					}
				break;
				case SET_ROUTESWITCH:
					if(strncmp(buffer1,ROUTESWITCH_PREFIX,strlen(ROUTESWITCH_PREFIX)) == 0)//判断路由是否打开
					{
						if(strcmp(msgDetail,buffer2))
						{
							strcpy(buffer2,msgDetail);
							buffer1[strlen(buffer1)] = '=';
							buffer2[strlen(buffer2)] = '\n';
							strncat(buffer1,buffer2,strlen(buffer2));
							printf("buffer1: set to %s",buffer1);						
						}
						else{
							printf("routeSwitchSet not changed\n");
						}
					}
				break;
				case SET_ROUTE:
					if(strncmp(buffer1,ROUT_PREFIX,strlen(ROUT_PREFIX)) == 0)//路由关系设定部分
					{
						if(strcmp(msgDetail,buffer2))
						{
							strcpy(buffer2,msgDetail);
							buffer1[strlen(buffer1)] = '=';
							buffer2[strlen(buffer2)] = '\n';
							strncat(buffer1,buffer2,strlen(buffer2));
							printf("buffer1: set to %s",buffer1);						
						}
						else{
							printf("routeSwitchSet not changed\n");
						}
					}
				break;
				default:
					break;
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
	}
	else if(*channelNo == CHANNEL_DEFAULT)//此处通过判断通道号为0-3之外的值，对fckfq-config文件进行读取及修改
	{
		FILE *fp = fopen(CONFIG_PREFIX,"r+");
		if(fp == NULL)
		{
			printf("open error\n");
			return -1;
		}
		while(fgets(configFile[i],CONFIG_LINE,fp))
		{
			sscanf(configFile[i], "%[^=]=%[^=]", buffer1, buffer2 );
			printf("buffer1: %s , buffer2 :%s",buffer1, buffer2);
			switch(cmdtype)
			{
				case SET_GROUP01_MODE:
					if(strncmp(buffer1,GROUP01_MODE_PREFIX,strlen(GROUP01_MODE_PREFIX))==0)
					{
					
						if(strcmp(msgDetail,buffer2))//判断第01组模式发生变化，如果不一致则进行以下操作
						{
							strcpy(buffer2,msgDetail);
							buffer1[strlen(buffer1)] = '=';
							buffer2[strlen(buffer2)] = '\n';
							strncat(buffer1,buffer2,strlen(buffer2));
							printf("buffer1: set to %s",buffer1);				
						}
						else
						{
							printf("IP address not changed\n");
						}
					}					
				break;
				case SET_GROUP23_MODE:
					if(strncmp(buffer1,GROUP23_MODE_PREFIX,strlen(GROUP23_MODE_PREFIX))==0)
					{
					
						if(strcmp(msgDetail,buffer2))//判断是否ip地址一致，如果不一致则进行以下操作
						{
							strcpy(buffer2,msgDetail);
							buffer1[strlen(buffer1)] = '=';
							buffer2[strlen(buffer2)] = '\n';
							strncat(buffer1,buffer2,strlen(buffer2));
							printf("buffer1: set to %s",buffer1);				

						}
						else
						{
							printf("IP address not changed\n");
						}
					}						
				break;
				default:
					break;

			}
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
	}
	else
	{
		printf("error channel num");
		return ERROR_NO;
	}
    return 0;
}

#if 0 //设置函数-备用
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
#endif



#if 0
void setEthernetIpAddr(char *msg)
{

}
/*******************/
//函数：回送以太网IP地址设置结果 
//ret 0代表设置成功，其他为失败
//msg 通过udp包发送客户端的数据报文
/*******************/
void sendIpSetResult(int ret,char *msg)
{
	if(ret == 0)
		;
}
#endif
/*******************/
//函数：回送FC IP地址设置结果 
//channelNo:通道号['0'-'3']
//ret 0代表设置成功，其他为失败
//msg 通过udp包发送客户端的数据报文
/*******************/
void sendFcIpSetResult(char channelNo, int ret, char *msg)
{
	switch(channelNo)
	{
		case '0':
			if(!ret)
				strcpy(msg,CMD_FC0_IP_SET_OK);
			else
				strcpy(msg,CMD_FC0_IP_SET_FAIL);
		break;
		case '1':
			if(!ret)
				strcpy(msg,CMD_FC1_IP_SET_OK);
			else
				strcpy(msg,CMD_FC1_IP_SET_FAIL);
		break;		
		case '2':
			if(!ret)
				strcpy(msg,CMD_FC2_IP_SET_OK);
			else
				strcpy(msg,CMD_FC2_IP_SET_FAIL);
		break;
		case '3':
			if(!ret)
				strcpy(msg,CMD_FC3_IP_SET_OK);
			else
				strcpy(msg,CMD_FC3_IP_SET_FAIL);
		break;		
		default:
			strcpy(msg,CMD_ERROR_MSG);
			break;
	}
}
/*******************/
//函数：回送开发器模式地址设置结果 
//group:0代表port0和port1,1代表port2和port3
//ret 0代表设置成功，其他为失败
//msg 通过udp包发送客户端的数据报文
/*******************/
void sendSetModeResult(int group, int ret, char *msg)
{
	switch(group)
	{
		case 1:
			if(!ret)
				strcpy(msg,CMD_GROUP01_MODE_OK);
			else
				strcpy(msg,CMD_GROUP01_MODE_FAIL);
		break;
		case 2:
			if(!ret)
				strcpy(msg,CMD_GROUP23_MODE_OK);
			else
				strcpy(msg,CMD_GROUP23_MODE_FAIL);
		break;
		default:
			strcpy(msg,CMD_ERROR_MSG);
		break;		
	}
}
/*******************/
//函数：回送端口名称设置结果 
//channelNo:通道号['0'-'3']
//ret 0代表设置成功，其他为失败
//msg 通过udp包发送客户端的数据报文
/*******************/
void sendPortNameSetResult(char channelNo,int ret,char *msg)
{
	switch(channelNo)
	{
		case '0':
			if(!ret)
				strcpy(msg,CMD_PORTNAME0_OK);
			else
				strcpy(msg,CMD_PORTNAME0_FAIL);
		break;
		case '1':
			if(!ret)
				strcpy(msg,CMD_PORTNAME1_OK);
			else
				strcpy(msg,CMD_PORTNAME1_FAIL);
		break;		
		case '2':
			if(!ret)
				strcpy(msg,CMD_PORTNAME2_OK);
			else
				strcpy(msg,CMD_PORTNAME2_FAIL);
		break;
		case '3':
			if(!ret)
				strcpy(msg,CMD_PORTNAME3_OK);
			else
				strcpy(msg,CMD_PORTNAME3_FAIL);
		break;		
		default:
			strcpy(msg,CMD_ERROR_MSG);
			break;
	}	
	
}
/*******************/
//函数：解析udp命令 
//cmd： 客户端发送的命令
//msgDetail： 客户端发送的命令内容
//msg：通过udp包发送客户端的数据报文
//返回值，0代表成功
/*******************/
int parsCmd(const char *cmd, const char *msgDetail,char *msg)
{
	unsigned int cmdtype=0;
	char channelNo;
	int ret,group;
	if(!strcmp(cmd,CMD_SET_IP))
	{
		printf("ethernet ip set routine\n");
	}
	else if(!strcmp(cmd,CMD_FC0IPADDR))
	{
		printf("set FC port 0 IP Addr routine\n");
		cmdtype = SET_FC_IP_ADDR;
		channelNo = CHANNEL_0;
		fcConfigSet(cmdtype,&channelNo,msgDetail);
		sendFcIpSetResult(channelNo,ret,msg);
	}			
	else if(!strcmp(cmd,CMD_FC1IPADDR))
	{
		printf("set FC port 1 IP Addr routine");
		cmdtype = SET_FC_IP_ADDR;
		channelNo = CHANNEL_1;		
		fcConfigSet(cmdtype,&channelNo,msgDetail);	
		sendFcIpSetResult(channelNo,ret,msg);		
	}			
	else if(!strcmp(cmd,CMD_FC2IPADDR))
	{
		printf("set FC port 2 IP Addr routine");
		cmdtype = SET_FC_IP_ADDR;
		channelNo = CHANNEL_2;	
		fcConfigSet(cmdtype,&channelNo,msgDetail);		
		sendFcIpSetResult(channelNo,ret,msg);		
	}			
	else if(!strcmp(cmd,CMD_FC3IPADDR))
	{
		printf("set FC port 3 IP Addr routine");
		cmdtype = SET_FC_IP_ADDR;
		channelNo = CHANNEL_3;
		fcConfigSet(cmdtype,&channelNo,msgDetail);		
		sendFcIpSetResult(channelNo,ret,msg);		
	}					
	else if(!strcmp(cmd,CMD_SET_0_PORTNAME))
	{
		printf("set FC port0 portname\n");
		cmdtype = SET_PORTNAME;
		channelNo = CHANNEL_0;	
		fcConfigSet(cmdtype,&channelNo,msgDetail);	
		sendPortNameSetResult(channelNo,ret,msg);		
	}
	else if(!strcmp(cmd,CMD_SET_1_PORTNAME))
	{
		printf("set FC port1 portname\n");
		cmdtype = SET_PORTNAME;
		channelNo = CHANNEL_1;
		fcConfigSet(cmdtype,&channelNo,msgDetail);
		sendPortNameSetResult(channelNo,ret,msg);		
	}		
	else if(!strcmp(cmd,CMD_SET_2_PORTNAME))
	{
		printf("set FC port2 portname\n");
		cmdtype = SET_PORTNAME;
		channelNo = CHANNEL_2;
		fcConfigSet(cmdtype,&channelNo,msgDetail);		
		sendPortNameSetResult(channelNo,ret,msg);		
	}
	else if(!strcmp(cmd,CMD_SET_3_PORTNAME))
	{
		printf("set FC port3 portname\n");
		cmdtype = SET_PORTNAME;
		channelNo = CHANNEL_3;
		fcConfigSet(cmdtype,&channelNo,msgDetail);		
		sendPortNameSetResult(channelNo,ret,msg);		
	}
	else if(!strcmp(cmd,CMD_SET_PORT01_MODE))
	{
		printf("set FC gtoup01 mode\n");
		cmdtype = SET_GROUP01_MODE;
		channelNo = CHANNEL_DEFAULT;		
		group =1;
		fcConfigSet(cmdtype,&channelNo,msgDetail);		
		sendSetModeResult(group,ret,msg);
		
	}		
	else if(!strcmp(cmd,CMD_SET_PORT23_MODE))
	{
		printf("set FC gtoup23 mode\n");
		cmdtype = SET_GROUP23_MODE;		
		channelNo = CHANNEL_DEFAULT;
		group =2;
		fcConfigSet(cmdtype,&channelNo,msgDetail);		
		sendSetModeResult(group,ret,msg);		
	}
	else
	{
		printf("undefined cmd\n");
		return ERROR_CMD;
	}
}

int sendConfigFileDetail(char *configFile)
{
//	char configFile[CONFIG_FILE_DETAIL_LEN];
	char save[MAX_ROW_LEN];
	FILE *fp=fopen(FILENAME0,"r");
	if(fp == NULL)
	{
		printf("error open %s\n",FILENAME0);
		return FAIL_OPEN;
	}
	while(fgets(save,MAX_ROW_LEN,fp))
	{
		strncat(configFile,save,strlen(save));
	}
	fclose(fp);
	fp=fopen(FILENAME1,"r");
	if(fp == NULL)
	{
		printf("error open %s\n",FILENAME1);
		return FAIL_OPEN;
	}	
	while(fgets(save,MAX_ROW_LEN,fp))
	{
		strncat(configFile,save,strlen(save));
	}	
	fclose(fp);	
	fp=fopen(FILENAME2,"r");
	if(fp == NULL)
	{
		printf("error open %s\n",FILENAME2);
		return FAIL_OPEN;
	}	
	while(fgets(save,MAX_ROW_LEN,fp))
	{
		strncat(configFile,save,strlen(save));
	}	
	fclose(fp);
	fp=fopen(FILENAME3,"r");
	if(fp == NULL)
	{
		printf("error open %s\n",FILENAME3);
		return FAIL_OPEN;
	}	
	while(fgets(save,MAX_ROW_LEN,fp))
	{
		strncat(configFile,save,strlen(save));
	}		
	fclose(fp);
	fp=fopen(FILENAME5,"r");
	if(fp == NULL)
	{
		printf("error open %s\n",FILENAME5);
		return FAIL_OPEN;
	}	
	while(fgets(save,MAX_ROW_LEN,fp))
	{
		strncat(configFile,save,strlen(save));
	}		
	fclose(fp);	
	printf("%s essence is \n%s\n",FILENAME1,configFile);
	return 0;
}

void msg_send_thread(int *sockfd)
{
	int ret;
	char *configFile;
	while(1)
	{
		ret = sendConfigFileDetail(configFile);
		if(!ret)
		{
			
			sendto(*sockfd,configFile,strlen(configFile),0,(struct sockaddr *)(&addr),addrlen);
			sleep(2);
		}
		else
			pthread_exit(0);
	}
}
