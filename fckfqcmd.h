#include <stdio.h>
#include <stdlib.h>
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
#define DEFAULT_SETTING		1		
#define IPADDR_LEN			15
#define NETMASK_LEN			15
#define ROUTESWITCH_LEN		2
#define ROUTE_SET_LEN		50
#define SERVER_PORT 		26896 //udp通信端口
#define MAX_MSG_SIZE 		1024
/*****************命令定义*************************/
#define NODENAME_PREFIX 	"Node_Name"
#define PORTNAME_PREFIX 	"Port_Name"
#define IPADDR_PREFIX		"fcIpAddr"
#define NETMASK_PREFIX		"netMask"
#define ROUTESWITCH_PREFIX	"routeSwitch"
#define	ROUT_PREFIX			"route"
#define FILENAME_PREFIX		"/etc/ifcfg-fc"
#define CONFIG_PREFIX		"/etc/fckfq-config"
/******************上位机发送命令******************/
#define CMD_SET_IP			"setIpAddr"
#define CMD_SET_0_PORTNAME	"setFc0PortName"
#define CMD_SET_1_PORTNAME	"setFc1PortName"
#define CMD_SET_2_PORTNAME	"setFc2PortName"
#define CMD_SET_3_PORTNAME	"setFc3PortName"

#define CMD_SET_0_NODENAME	"setFc0NodeName"
#define CMD_SET_1_NODENAME	"setFc1NodeName"
#define CMD_SET_2_NODENAME	"setFc2NodeName"
#define CMD_SET_3_NODENAME	"setFc3NodeName"
#define CMD_SHUTDOWN		"shutDown"
#define CMD_SET_PORT01_MODE	"setGroup01mode"
#define CMD_SET_PORT23_MODE	"setGroup23mode"
/******************下位机回复设置结果*************/
#define CMD_IP_SET_OK			"setIpAddrOk"
#define CMD_IP_SET_FAIL			"setIpAddrFail"
#define CMD_GROUP01_MODE_OK		"setGroup01modeOk"
#define CMD_GROUP23_MODE_OK 	"setGroup23modeOk"
#define CMD_GROUP01_MODE_FAIL	"setGroup01modeFail"
#define CMD_GROUP23_MODE_FAIL	"setGroup23modeFail"
#define CMD_SHUTDOWN_OK			"shutDownOk"
#define CMD_SHUTDOWN_FAIL		"shutDownFail"
#define CMD_PORTNAME0_OK		"setFc0PortNameOk"
#define CMD_PORTNAME0_FAIL		"setFc0PortNameFail"
#define CMD_PORTNAME1_OK		"setFc1PortNameOk"
#define CMD_PORTNAME1_FAIL		"setFc1PortNameFail"
#define CMD_PORTNAME2_OK		"setFc2PortNameOk"
#define CMD_PORTNAME2_FAIL		"setFc2PortNameFail"
#define CMD_PORTNAME3_OK		"setFc3PortNameOk"
#define CMD_PORTNAME3_FAIL		"setFc3PortNameFail"

#define CMD_NODENAME0_OK		"setFc0NodeNameOk"
#define CMD_NODENAME0_FAIL		"setFc0NodeNameFail"
#define CMD_NODENAME1_OK		"setFc1NodeNameOk"
#define CMD_NODENAME1_FAIL		"setFc1NodeNameFail"
#define CMD_NODENAME2_OK		"setFc2NodeNameOk"
#define CMD_NODENAME2_FAIL		"setFc2NodeNameFail"
#define CMD_NODENAME3_OK		"setFc3NodeNameOk"
#define CMD_NODENAME3_FAIL		"setFc3NodeNameFail"
/**********************定义结束******************/

struct config_struct{
	char channelNo;
	char ipAddrSet[IPADDR_LEN];
	char nodeNameSet[NODENAME_LEN];
	char portNameSet[PORTNAME_LEN];
	char netMaskSet[NETMASK_LEN];
	char routeSwitchSet[ROUTESWITCH_LEN];
	char routeSet[ROUTE_SET_LEN];
};