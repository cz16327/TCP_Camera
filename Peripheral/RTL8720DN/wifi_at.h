#ifndef __WIFI_AT_H__
#define __WIFI_AT_H__

#include "main.h"

#define USARTx				huart2
#define WAIT_USART_DMA 		(500)	// 等待DMA发送完成的时间
#define WAIT_MS				(1000)
#define MAX_LINE_LEN	(1024)	// 单次透传最大数据长度

/***** 执行方式枚举体 *****/
typedef enum At_type_def
{
	Query = 1,	// 查询 "AT?"
	Set,		// 设置 "AT="
	Excution	// 执行 "AT"
} At_type_e;

/** 指定需要设置掩码的模式 **/
typedef enum SysMsg_mode_def
{
	SeriaNet = 1,	// 透传模式
} SysMsg_mode_e;

/******** 打印掩码 ********/
#define MASK_ALL 0xFFFFFFFF // 全部打印（枚举体默认int塞不下）
typedef enum Mask_def
{
	EVENT_WIFI_DISCONNECT = 0x00000001,
	EVENT_WIFI_CONNECT = 0x00000002,
	EVENT_WIFI_APCLIENTDISCONNECT_MAC = 0x00000004,
	EVENT_WIFI_APCLIENTCONNECTED_MAC = 0x00000008,
	EVENT_SOCKETDOWN_CONID_LENGTH_DATA = 0x00000010,
	EVENT_SOCKETSEED_SEEDCONID_SERVERCONID = 0x00000020,
	EVENT_SOCKETDISSCONNECT_CONID = 0x00000040,
	EVENT_SOCKETRECONNECT_CONID = 0x00000080,
	EVENT_SOCKETAUTODEL_CONID = 0x00000100,
	EVENT_MQTT_CONNECT = 0x00000200,
	EVENT_MQTT_DISCONNECT = 0x00000400,
	EVENT_MQTT_SUB_TOPIC_LEN_DATA = 0x00000800,
	EVENT_BLE_DISCONNECT = 0x00001000,
	EVENT_BLE_CONNECTED = 0x00002000,
	DATA_LEN_DATA = 0x00004000,
	EVENT_WIFI_GOT_IP = 0x00008000,
	EVENT_WIFI_SCAN_DON = 0x00010000,
} Mask_e;

/******** 是否保存 ********/
typedef enum Save_def
{
	UNSAVE,	// 不保存
	SAVE	// 保存
} Save_e;

/****** 设置睡眠模式 ******/
typedef enum Sleep_mode_def
{
	UNAUTO_LIGHT_SLEEP,	// 进入浅睡眠，上电不自动进入浅睡眠状态
	AUTO_LIGHT_SLEEP,	// 进入浅睡眠，上电自动进入浅睡眠
	DEEP_SLEEP,			// 进入深度睡眠状态
	NORMAL				// 普通模式
} Sleep_mode_e;

/* 设置唤醒源(仅 Sleep_mode_e = 0/1/2 时有效) */
typedef enum Wakeup_source_def
{
	TIMER_WAKEUP,	// 定时器唤醒
	GPIO_WAKEUP		// GPIO 唤醒
} Wakeup_source_e;

/*** GPIO 唤醒时的唤醒电平 ***/
typedef enum GPIO_wake_level_def
{
	LOW,		// 低电平唤醒
	HIGH,		// 高电平唤醒
	FALLING,	// 下降沿唤醒
	RISING,		// 上升沿唤醒
	DOUBLE		// 双边沿唤醒
} GPIO_wake_level_e;

/********* 数据位数 *********/
typedef enum Uart_databits_def
{
	DATABITS_5 = 5,	// 5 bit 数据位
	DATABITS_6,		// 6 bit 数据位
	DATABITS_7,		// 7 bit 数据位
	DATABITS_8		// 8 bit 数据位
} Uart_databits_e;

/********** 停止位 **********/
typedef enum Uart_stopbits_def
{
	STOPBITS_1 = 1,	// 1 bit 停止位
	STOPBITS_1_5,	// 1.5 bit 停止位
	STOPBITS_2		// 2 bit 停止位
} Uart_stopbits_e;

/********* 奇偶校验 *********/
typedef enum Uart_parity_def
{
	PARITY_NONE,	// 无校验
	PARITY_ODD,		// 奇校验
	PARITY_EVEN		// 偶校验
} Uart_parity_e;

/*********** 流控 ***********/
typedef enum Uart_flow_def
{
	FLOW_NONE,		// 无流控
	RTS_ENABLE,		// 使能 RTS
	CTS_ENABLE,		// 使能 CTS
	RTS_CTS_ENABLE	// 同时使能 RTS 和 CTS
} Uart_flow_e;

/********* 下载模式 *********/
typedef enum Download_mode_def
{
	UART_MODE = 1	// 串口下载模式
} Download_mode_e;

/******* OTA下载方式 *******/
typedef enum OTA_mode_def
{
	HTTP = 1,
	HTTPS = 2
} OTA_mode_e;

/********* IO 电平 *********/
typedef enum IO_level_def
{
	IO_RESET,	// 低电平
	IO_SET		// 高电平
} IO_level_e;

/****** WIFI 工作模式 ******/
typedef enum Wifi_mode_def
{
	UNINIT_MODE,	// 未初始化或者关闭 wifi
	STA_MODE,		// STA 模式
	AP_MODE,		// AP 模式
	AP_STA_MODE		// AP+STA 模式
} Wifi_mode_e;

/******* IP 获取模式 *******/
typedef enum IP_mode_def
{
	STATIC,	// 静态 IP
	DYNAMIC	// 动态 IP
} IP_mode_e;

/******* EAP 加密方式 *******/
typedef enum EAP_type_def
{
	PEAP = 1,	// PEAP 加密方式
	TLS,		// TLS 加密方式
	TTLS,		// TTLS 加密方式
	FAST		// FAST 加密方式
} EAP_type_e;

/********** 国家代码 **********/
typedef enum Country_Code_def
{
	DEF,	// 使用默认国家代码
	JP,		// 日本
	AS,		// 美属萨摩亚
	CA,		// 加拿大
	US,		// 美国
	CN,		// 中国
	HK,		// 中国香港
	TW,		// 中国台湾
	MO,		// 中国澳门
	IL,		// 以色列
	SG,		// 新加坡
	KR,		// 韩国
	TR,		// 土耳其
	AU,		// 澳大利亚
	ZA,		// 南非
	BR		// 巴西
} Country_Code_e;

/********** WIFI配网操作 **********/
typedef enum Wifi_Cfg_def
{
	OFF_CFG = 0,			// 关闭手机配网任务
	ONCE_WIFI_CFG = 1,		// 开启一次 wifi 配网(配网成功/配网超时会自动返回关闭状态)
	ONCE_BT_CFG = 2,		// 开启一次蓝牙配网(配网成功/配网超时会自动返回关闭状态)
	ONCE_AIRKISS_CFG = 3,	// 开启一次 AirKiss 配网
	ONCE_BLUFI_CFG = 9,		// 开启一次 blufi 配网
	WPS_CFG = 10			// WPS 配网
} Wifi_Cfg_e;

/********** WIFI配网操作 **********/
typedef enum Wifi_ScanOpt_def
{
	SSID_OPT = 0x01,		// 扫描信息显示SSID
	CHANNEL_OPT = 0x02,		// 扫描信息显示CHANNEL
	SECURITY_OPT = 0x04,	// 扫描信息显示SECURITY
	RSSI_OPT = 0x08,		// 扫描信息显示RSSI
	MAC_OPT = 0x10			// 扫描信息显示MAC
} Wifi_ScanOpt_e;

/********** Socket模式 **********/
typedef enum Socket_Mode_def
{
	UDP_SERVER = 1,	// UDP服务端
	UDP_CLIENT,		// UDP客户端
	TCP_SERVER,		// TCP服务端
	TCP_CLIENT,		// TCP客户端
	// TCP_SEED,	// TCP占位符，不可用，客户端连接TCP Server模组时自动产生
	SSL_SERVER = 6,	// SSL服务端
	SSL_CLIENT,		// SSL客户端
	// SSL_SEED		// SSL占位符，不可用，客户端连接SSL Server模组时自动产生
} Socket_Mode_e;

/********** 接收模式 **********/
typedef enum Recv_Mode_def
{
	RECV_PASSIVE,	// 被动接收
	RECV_ACTIVE		// 主动接收
} Recv_Mode_e;

/********** 透传模式 **********/
typedef enum Tt_Mode_def
{
	TT_UDP_FIRST,
	TT_NORMAL,
	TT_UDP_LAST
} Tt_Mode_e;

/********** 自动透传类型 **********/
typedef enum AutoTt_Type_def
{
	AUTO_TT_DISABLE,		// 0:禁用自动进入透传模式
	AUTO_TT_UDPSERVER,		// 1:自动进入 UDPServer 透传模式
	AUTO_TT_UDPCLIENT,		// 2:自动进入 UDPClient 透传模式
	AUTO_TT_TCPCLIENT = 4	// 4:自动进入 TCPClient 透传模式
} AutoTt_Type_e;

/********** SSL证书类型 **********/
typedef enum SSL_Type_def
{
	SSL_CA = 1,	// CA 根证书
	SSL_PUBLIC,	// 客户端公钥
	SSL_PRIVATE	// 客户端私钥
} SSL_Type_e;

/********** MQTT 配置项 **********/
typedef enum Mqtt_Cfg_def
{
	MQTT_CFG_IP = 1,	// 设置连接的域名或 IP
	MQTT_CFG_PORT,		// 设置服务器端口号
	MQTT_CFG_TYPE,		// 设置连接方式(1：使用 TCP 连接；2：使用 SSL 连接)
	MQTT_CFG_ID,		// 设置客户端 ID
	MQTT_CFG_USER,		// 设置用户名(最大长度 63 字节)
	MQTT_CFG_PSWD,		// 设置密码(最大长度 63 字节)
	MQTT_CFG_LWT		// 设置遗嘱消息格式为AT+MQTT=7,<LWT_topic>,<LWT_qos>,<LWT_Retained>,<LWTpayload>
	// LWT_topic：遗嘱主题(不需要遗嘱这里设置为"")
	// LWT_qos：遗嘱 QOS(0/1/2)
	// LWT_Retained：遗嘱 retained(0/1)
	// LWTpayload：遗嘱消息内容(不需要遗嘱这里设置为"")
} Mqtt_Cfg_e;

/********** 使失能 **********/
typedef enum BW_enable_def
{
	BW_DISABLE,
	BW_ENABLE
} BW_enable_e;

#pragma pack(1)

#pragma pack()

typedef struct Basic_AT_def
{
	Ret_Status_e(*at)(At_type_e type, unsigned int timeoutS);
	Ret_Status_e(*at_help)(At_type_e type, unsigned int timeoutS);
	Ret_Status_e(*at_rst)(At_type_e type, unsigned int timeoutS);
	Ret_Status_e(*at_restore)(At_type_e type, unsigned int timeoutS);
	Ret_Status_e(*ate1)(At_type_e type, unsigned int timeoutS);
	Ret_Status_e(*ate0)(At_type_e type, unsigned int timeoutS);
	Ret_Status_e(*at_sysMsg)(At_type_e type, SysMsg_mode_e mode, Mask_e mask, Save_e saveFlash, unsigned int timeoutS);
	Ret_Status_e(*at_gmr)(At_type_e type, unsigned int timeoutS);
	Ret_Status_e(*at_flashId)(At_type_e type, unsigned int timeoutS);
	Ret_Status_e(*at_sleep)(At_type_e type, Sleep_mode_e mode, Wakeup_source_e wake, unsigned int ms_pin, GPIO_wake_level_e level, unsigned int timeoutS);
	Ret_Status_e(*at_uartCfg)(At_type_e type, unsigned int baudrate, Uart_databits_e databits, Uart_stopbits_e stopbits, Uart_parity_e parity, unsigned int timeoutS);
	Ret_Status_e(*at_uartFlowControl)(At_type_e type, Uart_flow_e flowcontrol, unsigned int timeoutS);
	Ret_Status_e(*at_setDownloadMode)(At_type_e type, Download_mode_e mode, unsigned int timeoutS);
	Ret_Status_e(*at_ota)(At_type_e type, OTA_mode_e mode, unsigned char* Host_name, unsigned short Port, unsigned char* Route, unsigned int timeoutS);
	Ret_Status_e(*at_tickless)(At_type_e type, unsigned char tickless, unsigned int timeoutS);
} Basic_AT_t;

typedef struct IOctrl_AT_def
{
	Ret_Status_e(*at_sysIoMap)(At_type_e type, unsigned char PinNumber, unsigned char* pinx_list, unsigned int timeoutS);
	Ret_Status_e(*at_sysGpioWrite)(At_type_e type, unsigned char pin, IO_level_e level, unsigned int timeoutS);
	Ret_Status_e(*at_sysGpioRead)(At_type_e type, unsigned char pin, unsigned int timeoutS);
	Ret_Status_e(*at_pwmCfg)(At_type_e type, unsigned char pin, unsigned int cycle, unsigned int duty, unsigned int timeoutS);
	Ret_Status_e(*at_pwmCfgs)(At_type_e type, unsigned char pin, unsigned int cycle, unsigned int duty, unsigned int timeoutS);
	Ret_Status_e(*at_pwmStop)(At_type_e type, unsigned char pin, unsigned int timeoutS);
	Ret_Status_e(*at_pwmDutySet)(At_type_e type, unsigned char pin, unsigned int duty, unsigned int timeoutS);
	Ret_Status_e(*at_pwmDutySets)(At_type_e type, unsigned char pin, unsigned int duty, unsigned int timeoutS);
} IOctrl_AT_t;

typedef struct Wifi_Basic_def
{
	Ret_Status_e(*at_wMode)(At_type_e type, Wifi_mode_e mode, Save_e saveFlash, unsigned int timeoutS);
	Ret_Status_e(*at_wDisconnect)(At_type_e type, unsigned int timeoutS);
	Ret_Status_e(*at_wScan)(At_type_e type, unsigned char* ssid, unsigned char* mac, unsigned char channel, char rssi, unsigned int timeoutS);
	Ret_Status_e(*at_wScanActive)(At_type_e type, unsigned char* ssid, unsigned int timeoutS);
	Ret_Status_e(*at_wSDhcp)(At_type_e type, IP_mode_e mode, unsigned char* ip, unsigned char* mask, unsigned char* gateway, unsigned int timeoutS);
	Ret_Status_e(*at_wJap)(At_type_e type, unsigned char* ssid, unsigned char* pwd, unsigned char* bssid, unsigned int timeoutS);
	Ret_Status_e(*at_staInfo)(At_type_e type, unsigned int timeoutS);
	Ret_Status_e(*at_wJeap)(At_type_e type, EAP_type_e EAPtype, unsigned char* ssid, unsigned char* identity, unsigned char* pwd, unsigned int timeoutS);
	Ret_Status_e(*at_wAutoConn)(At_type_e type, BW_enable_e enable, unsigned char* ssid, unsigned char* pwd, unsigned char* bssid, unsigned int timeoutS);
	Ret_Status_e(*at_wApDhcp)(At_type_e type, BW_enable_e enable, unsigned char* start_ip, unsigned char* end_ip, unsigned char* gateway, unsigned int timeoutS);
	// Ret_Status_e(*at_wAp)(At_type_e type, unsigned char* ssid, unsigned char* pwd, unsigned short channel, unsigned char maxConn, BW_enable_e ssidHidden, unsigned int timeoutS);
	Ret_Status_e(*at_wApInfo)(At_type_e type, unsigned char* ssid, unsigned char* pwd, unsigned short channel, unsigned char maxConn, BW_enable_e ssidHidden, unsigned int timeoutS);
	Ret_Status_e(*at_ping)(At_type_e type, unsigned char* addr, unsigned char count, unsigned int timeoutS);
	Ret_Status_e(*at_cipStaMac_Def)(At_type_e type, unsigned char* mac, unsigned int timeoutS);
	Ret_Status_e(*at_wCountry)(At_type_e type, Country_Code_e country_code, unsigned int timeoutS);
	Ret_Status_e(*at_wConfig)(At_type_e type, Wifi_Cfg_e status, unsigned char* name, unsigned int timeoutS);
	Ret_Status_e(*at_wScanOpt)(At_type_e type, unsigned char option, unsigned int timeoutS);
	Ret_Status_e(*at_wRssi)(At_type_e type, unsigned int timeoutS);
	Ret_Status_e(*at_blufiSend)(At_type_e type, unsigned int len, unsigned int timeoutS);
} Wifi_Basic_t;

typedef struct Wifi_TcpIp_def
{
	/* 此处有接收数据 */
	/* 此处有接收数据 */
	Ret_Status_e(*at_socket)(At_type_e type, Socket_Mode_e mode, unsigned char* remote_host, unsigned char* port, unsigned int keep_alive, unsigned int conID, unsigned int timeoutS);
	// Ret_Status_e(*at_socket2)(At_type_e type, Socket_Mode_e mode, unsigned char* remote_host, unsigned char* port, unsigned int keep_alive, unsigned int conID, unsigned int timeoutS);
	Ret_Status_e(*at_socketSend)(At_type_e type, unsigned int conID, unsigned int len, unsigned int timeoutS);
	Ret_Status_e(*at_socketSendLine)(At_type_e type, unsigned int conID, unsigned int len, unsigned char* data, unsigned int timeoutS);
	Ret_Status_e(*at_socketSendHex)(At_type_e type, unsigned int conID, unsigned int len, unsigned char* data, unsigned int timeoutS);
	Ret_Status_e(*at_socketRead)(At_type_e type, unsigned int conID, unsigned int timeoutS);
	Ret_Status_e(*at_socketDel)(At_type_e type, unsigned int conID, unsigned int timeoutS);
	Ret_Status_e(*at_socketRecvCfg)(At_type_e type, Recv_Mode_e mode, unsigned int timeoutS);
	Ret_Status_e(*at_socketTt)(At_type_e type, Tt_Mode_e mode, unsigned int timeoutS);
	Ret_Status_e(*at_socketAutoTt)(At_type_e type, AutoTt_Type_e mode, unsigned char* remote_host, unsigned char* port, unsigned int timeoutS);
	Ret_Status_e(*at_sslCret)(At_type_e type, SSL_Type_e ssl_type, unsigned int len, unsigned int timeoutS);
	Ret_Status_e(*at_wDomain)(At_type_e type, unsigned char* server_name, unsigned int timeoutS);
	Ret_Status_e(*at_wDns)(At_type_e type, unsigned char* dns_ip1, unsigned char* dns_ip2, unsigned int timeoutS);
} Wifi_TcpIp_t;

typedef struct Wifi_Mqtt_def
{
	Ret_Status_e(*at_mqtt)(At_type_e type, Mqtt_Cfg_e key, unsigned char* data, unsigned int timeoutS);
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

AT_t* get_AT_t(void);

#endif
