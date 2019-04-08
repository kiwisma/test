#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define CONFIG_FILE_DETAIL_LEN 1024
#define MAX_ROW_LEN			100
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
#define MAX_MSG_SIZE 		50
#define CMD_LENGTH			20
#define CMD_DETAIL_LENGTH	50
#define MSG_SEND_LEN		20
#define SET_ETH_IP			1
#define SET_GROUP01_MODE	2
#define SET_GROUP23_MODE	3
#define SET_PORTNAME		4
#define SET_NODENAME		5
//#define SET_2_PORTNAME		6
//#define SET_3_PORTNAME		7
#define	SET_FC_IP_ADDR		8
//#define SET_1_IP_ADDR		9
//#define	SET_2_IP_ADDR		10
//#define SET_3_IP_ADDR		11
#define SET_NETMASK			12
#define SET_ROUTESWITCH		13
#define	SET_ROUTE			14
#define CHANNEL_0			'0'
#define CHANNEL_1			'1'
#define CHANNEL_2			'2'
#define CHANNEL_3			'3'
#define CHANNEL_DEFAULT		'4'
/*****************错误代码*************************/
#define ERROR_NO			-2
#define	ERROR_CMD			-3
#define FAIL_OPEN			-1
/****************配置文件名***********************/
#define FILENAME0 "/etc/ifcfg-fc0"
#define FILENAME1 "/etc/ifcfg-fc1"
#define FILENAME2 "/etc/ifcfg-fc2"
#define FILENAME3 "/etc/ifcfg-fc3"
#define FILENAME5 "/etc/fckfq-config"
/*****************命令定义*************************/
#define NODENAME_PREFIX 	"Node_Name"
#define PORTNAME_PREFIX 	"Port_Name"
#define IPADDR_PREFIX		"fcIpAddr"
#define NETMASK_PREFIX		"netMask"
#define ROUTESWITCH_PREFIX	"routeSwitch"
#define	ROUT_PREFIX			"route"
#define FILENAME_PREFIX		"/etc/ifcfg-fc"
#define CONFIG_PREFIX		"/etc/fckfq-config"
/*****************开发器盒子配置文件********************/
#define GROUP01_MODE_PREFIX	"Group01"
#define GROUP23_MODE_PREFIX	"Group23"
#define	BOX_NAME_PREFIX		"Box_Name"
#define	BOX_IP_ETH_ADDR_PREFIX		"Ip_Addr"
#define	BOX_SOCKET_PORT_PREFIX		"Socket"
#define	BOX_NETMASK_PREFIX			"netMask"
/*****************默认设置*******************/
#define BOX_NAME			"FCBXKFQ_0000"
#define BOX_IP_ADDR			"192.168.1.1"
#define BOX_NETMASK			"255.255.255.0"
#define BOX_SOCKET			"26896"
#define	BOX_GROUP01_MODE	"KF_MODE"
#define	BOX_GROUP23_MODE	"KF_MODE"
/*****************合法设置******************/
#define	KF_MODE				"KF_MODE"
#define	JC_MODE				"JC_MODE"
#define ROUTE_ON			"ON"
#define ROUTE_OFF			"OFF"
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
#define CMD_FC0IPADDR		"setFc0IpAddr"
#define CMD_FC1IPADDR		"setFc1IpAddr"
#define CMD_FC2IPADDR		"setFc2IpAddr"
#define CMD_FC3IPADDR		"setFc3IpAddr"
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

#define CMD_FC0_IP_SET_OK			"setFc0IpAddrOk"
#define CMD_FC0_IP_SET_FAIL			"setFc0IpAddrFail"
#define CMD_FC1_IP_SET_OK			"setFc1IpAddrOk"
#define CMD_FC1_IP_SET_FAIL			"setFc1IpAddrFail"
#define CMD_FC2_IP_SET_OK			"setFc2IpAddrOk"
#define CMD_FC2_IP_SET_FAIL			"setFc2IpAddrFail"
#define CMD_FC3_IP_SET_OK			"setFc3IpAddrOk"
#define CMD_FC3_IP_SET_FAIL			"setFc3IpAddrFail"
#define	CMD_ERROR_MSG				"Error Code"
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
void poweroff(void); //关机函数
int fcConfigSet(unsigned int cmdtype,const char *channelNo,const char *msgDetail);
void sendFcIpSetResult(char channelNo, int ret, char *msg);
void sendSetModeResult(int group, int ret, char *msg);
void sendPortNameSetResult(char channelNo,int ret,char *msg);
int parsCmd(const char *cmd, const char *msgDetail,char *msg);
int sendConfigFileDetail(char *configFile);
void msg_send_thread(int *sockfd);