#ifndef __WIFI_AT_H__
#define __WIFI_AT_H__

#include "main.h"

#define USARTx	huart2
#define WAIT_MS	(1000)

/***** 执行方式枚举体 *****/
typedef enum at_type_def
{
	Query = 1,	// 查询
	Set,		// 设置
	Excution	// 执行（部分指令为，显示可输入参数）
} at_type_e;

#pragma pack(1)
typedef struct Basic_AT_def
{
	Ret_Status_e(*at)(void);
	Ret_Status_e(*at_help)(void);
	Ret_Status_e(*at_rst)(void);
	Ret_Status_e(*at_restore)(void);
	Ret_Status_e(*ate1)(void);
	Ret_Status_e(*ate0)(void);
	Ret_Status_e(*at_sysMsg)(void);
	Ret_Status_e(*at_gmr)(at_type_e type, unsigned int timeoutS);
	Ret_Status_e(*at_flashId)(void);
	Ret_Status_e(*at_sleep)(void);
	Ret_Status_e(*at_uartCfg)(void);
	Ret_Status_e(*at_uartFlowCon)(void);
	Ret_Status_e(*at_setDownload)(void);
	Ret_Status_e(*at_ota)(void);
	Ret_Status_e(*at_tickless)(void);
} Basic_AT_t;

typedef struct IOctrl_AT_def
{
	// AT+SYSIOMAP=16,21,34,NC,23,NC,26,29,NC,NC,30,NC,22,27,20,NC,NC
	Ret_Status_e(*at_sysIoMap)(void);
	Ret_Status_e(*at_sysGpioWrite)(void);
	Ret_Status_e(*at_sysGpioRead)(void);
	Ret_Status_e(*at_pwmCfg)(void);
	Ret_Status_e(*at_pwmCfgs)(void);
	Ret_Status_e(*at_pwmStop)(void);
	Ret_Status_e(*at_pwmDutySet)(void);
	Ret_Status_e(*at_pwmDutySets)(void);
} IOctrl_AT_t;

typedef struct Wifi_Basic_def
{
	Ret_Status_e(*at_wMode)(void);
	Ret_Status_e(*at_wDisconnect)(void);
	Ret_Status_e(*at_wScan)(void);
	Ret_Status_e(*at_wScanActive)(void);
	Ret_Status_e(*at_wSDhcp)(void);
	Ret_Status_e(*at_wJap)(void);
	Ret_Status_e(*at_staInfo)(void);
	Ret_Status_e(*at_wJeap)(void);
	Ret_Status_e(*at_wAutoConn)(void);
	Ret_Status_e(*at_wApDhcp)(void);
	Ret_Status_e(*at_wAp)(void);
	Ret_Status_e(*at_wApInfo)(void);
	Ret_Status_e(*at_ping)(void);
	Ret_Status_e(*at_cipStaMac_Def)(void);
	Ret_Status_e(*at_wCountry)(void);
	Ret_Status_e(*at_wConfig)(void);
	Ret_Status_e(*at_wScanOpt)(void);
	Ret_Status_e(*at_wRssi)(void);
	Ret_Status_e(*at_blufiSend)(void);
} Wifi_Basic_t;

typedef struct Wifi_TcpIp_def
{
	/* 此处有接收数据 */
	/* 此处有接收数据 */
	Ret_Status_e(*at_socket)(void);
	Ret_Status_e(*at_socket2)(void);
	Ret_Status_e(*at_socketSend)(void);
	Ret_Status_e(*at_socketSendLine)(void);
	Ret_Status_e(*at_socketSendHex)(void);
	Ret_Status_e(*at_socketRead)(void);
	Ret_Status_e(*at_socketDel)(void);
	Ret_Status_e(*at_socketRecvCfg)(void);
	Ret_Status_e(*at_socketTt)(void);
	Ret_Status_e(*at_socketAutoTt)(void);
	Ret_Status_e(*at_sslCret)(void);
	Ret_Status_e(*at_wDomain)(void);
	Ret_Status_e(*at_wDns)(void);
} Wifi_TcpIp_t;

typedef struct Wifi_Mqtt_def
{
	Ret_Status_e(*at_mqtt)(void);
	Ret_Status_e(*at_mqttVer)(void);
	Ret_Status_e(*at_mqttBuf)(void);
	Ret_Status_e(*at_mqttKeepAlive)(void);
	Ret_Status_e(*at_mqttCret)(void);
	Ret_Status_e(*at_mqttDisconn)(void);
	Ret_Status_e(*at_mqttPub)(void);
	Ret_Status_e(*at_mqttPubRaw)(void);
	Ret_Status_e(*at_mqttSub)(void);
	Ret_Status_e(*at_mqttUnsub)(void);
} Wifi_Mqtt_t;

typedef struct Wifi_Http_def
{
	Ret_Status_e(*at_httpClientLine)(void);
	Ret_Status_e(*at_httpRaw)(void);
} Wifi_Http_t;

typedef struct Wifi_Sntp_def
{
	Ret_Status_e(*at_sntpTime)(void);
	Ret_Status_e(*at_sntpTimeCfg)(void);
	Ret_Status_e(*at_sntpIntv)(void);
} Wifi_Sntp_t;

typedef struct Wifi_AT_def
{
	Wifi_Basic_t	Basic;
	Wifi_TcpIp_t	TcpIp;
	Wifi_Mqtt_t		Mqtt;
	Wifi_Http_t		Http;
	Wifi_Sntp_t		Sntp;
} Wifi_AT_t;

typedef struct Ble_Basic_def
{
	Ret_Status_e(*at_bleMac)(void);
	Ret_Status_e(*at_bleMode)(void);
	Ret_Status_e(*at_bleRfPwr)(void);
	Ret_Status_e(*at_bleState)(void);
	Ret_Status_e(*at_bleDiscon)(void);
	Ret_Status_e(*at_bleMtu)(void);
	Ret_Status_e(*at_bleSend)(void);
	Ret_Status_e(*at_bleSendRaw)(void);
	Ret_Status_e(*at_bleSerUuid)(void);
	Ret_Status_e(*at_bleTxUuid)(void);
	Ret_Status_e(*at_bleRxUuid)(void);
	Ret_Status_e(*at_transEnter)(void);
} Ble_Basic_t;

typedef struct Ble_Slave_def
{
	/* 此处有接收数据 */
	Ret_Status_e(*at_bleName)(void);
	Ret_Status_e(*at_bleConIntv)(void);
	Ret_Status_e(*at_bleAuth)(void);
	Ret_Status_e(*at_bleAdvIntv)(void);
	Ret_Status_e(*at_bleAdvData)(void);
	Ret_Status_e(*at_bleAdvEn)(void);
} Ble_Slave_t;

typedef struct Ble_Master_def
{
	Ret_Status_e(*at_bleScan)(void);
	Ret_Status_e(*at_bleConnect)(void);
	Ret_Status_e(*at_bleAutoCon)(void);
	Ret_Status_e(*at_bleDisAutoCon)(void);
} Ble_Master_t;

typedef struct Ble_iBeacon_def
{
	Ret_Status_e(*at_bleIbecnUuid)(void);
	Ret_Status_e(*at_bleIbecnData)(void);
} Ble_iBeacon_t;

typedef struct Ble_Mesh_def
{
	Ret_Status_e(*at_proVision)(void);
	Ret_Status_e(*at_meshSend)(void);
	Ret_Status_e(*at_meshAddr)(void);
	Ret_Status_e(*at_meshState)(void);
	// 针对天猫精灵的接口
	/* 此处有接收数据 */
	Ret_Status_e(*at_AliGenie)(void);
	Ret_Status_e(*at_send2Ali)(void);
} Ble_Mesh_t;

typedef struct Ble_AT_def
{
	Ble_Basic_t		Basic;
	Ble_Slave_t		Slave;
	Ble_Master_t	Master;
	Ble_iBeacon_t	iBeacon;
	Ble_Mesh_t		Mesh;
} Ble_AT_t;

typedef struct AT_def
{
	Basic_AT_t	Basic;
	IOctrl_AT_t	IOctrl;
	Wifi_AT_t	Wifi;
	Ble_AT_t	Ble;
} AT_t;
#pragma pack()

AT_t* get_AT_t(void);

#endif
