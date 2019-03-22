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
void main(void)
{

	char fileName[CONFIG_FILE_LEN]={"/etc/ifcfg-fc"};
	int fd;
	char fcNum='0';
//	memset(fileName,0,CONFIG_FILE_LEN);
//	fileName = "ifcfg-fc";
//	fcNum=0;
//	(unsigned char *)(fileName+9) = &fcNum;
	strncat(fileName,&fcNum,1);
	printf(":%s\n",fileName);
	
	fd = open(fileName,O_WRONLY|O_CREAT|O_APPEND);
	if(fd == -1)
    {
        printf("Failed to open");
        return -1;
    }
	close(fd);
	/*
    write(fd,buf,MAX_MSG_SIZE);
	*/
    return 0;
}