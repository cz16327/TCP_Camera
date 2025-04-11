#ifndef __WIFI_DATA_H__
#define __WIFI_DATA_H__

#include "main.h"

#pragma pack(1)
typedef struct Expect_def
{
	// 基础指令应答期望
	const char* ack_OK;						// "OK"
	const char* ack_ERROR;					// "ERROR"
	const char* ack_sysMsg;					// "+SYSMSG:"
	const char* ack_atVer;					// "at version:"
	const char* ack_sdkVer;					// "sdk version:"
	const char* ack_firmwareVer;			// "firmware version:"
	const char* ack_flashManufacturerID;	// "Flash Manufacturer ID:"
	const char* ack_flashDeviceID;			// "Flash Device ID:"
	const char* ack_flashSize;				// "Flash Capacity ID:"
	const char* ack_uartCfg;				// "+UARTCFG:"
	const char* ack_uartFlowCfg;			// "+UARTFLOWCONTROL:"
	const char* ack_ota;					// "+OTA:"
	const char* ack_tickless;				// "+TICKLESS:"

	// IO控制指令应答期望
	const char* ack_ioMap;					// "+SYSIOMAP:"
	const char* ack_ioRead;					// "+SYSGPIOREAD:"

	// WIFI基础指令应答期望
	const char* ack_wifiMode;				// "+WMODE:"
	const char* ack_wifiScan;				// "+WSCAN:index SSID,CH,SECURITY,RSSI,BSSID"
	const char* ack_wifiScanActive;			// "+WSCANACTIVE:"
	const char* ack_wifiStaDhcp;			// "+WSDHCP:"
	const char* ack_wifiInfoUnused;			// "+WJAP:"
	const char* ack_wifiInfo;				// "+STAINFO:"
	const char* ack_wifiEAP;				// "+EVENT:WIFI_GOT_IP"
	const char* ack_autoCon;				// "+WAUTOCONN:"
	const char* ack_wifiApDhcp;				// "+WAPDHCP:"
} Expect_t;

typedef struct Basic_AT_ACK_def
{
	char a;
} Basic_AT_ACK_t;

typedef struct IOctrl_AT_ACK_def
{
	char a;
} IOctrl_AT_ACK_t;

typedef struct Wifi_Basic_ACK_def
{
	char a;
} Wifi_Basic_ACK_t;

typedef struct Wifi_TcpIp_ACK_def
{
	char a;
} Wifi_TcpIp_ACK_t;

typedef struct Wifi_Mqtt_ACK_def
{
	char a;
} Wifi_Mqtt_ACK_t;

typedef struct Wifi_Http_ACK_def
{
	char a;
} Wifi_Http_ACK_t;

typedef struct Wifi_Sntp_ACK_def
{
	char a;
} Wifi_Sntp_ACK_t;

typedef struct Wifi_AT_ACK_def
{
	Wifi_Basic_ACK_t	Basic;
	Wifi_TcpIp_ACK_t	TcpIp;
	Wifi_Mqtt_ACK_t		Mqtt;
	Wifi_Http_ACK_t		Http;
	Wifi_Sntp_ACK_t		Sntp;
} Wifi_AT_ACK_t;

typedef struct Ble_Basic_ACK_def
{
	char a;
} Ble_Basic_ACK_t;

typedef struct Ble_Slave_ACK_def
{
	char a;
} Ble_Slave_ACK_t;

typedef struct Ble_Master_ACK_def
{
	char a;
} Ble_Master_ACK_t;

typedef struct Ble_iBeacon_ACK_def
{
	char a;
} Ble_iBeacon_ACK_t;

typedef struct Ble_Mesh_ACK_def
{
	char a;
} Ble_Mesh_ACK_t;

typedef struct Ble_AT_ACK_def
{
	Ble_Basic_ACK_t		Basic;
	Ble_Slave_ACK_t		Slave;
	Ble_Master_ACK_t	Master;
	Ble_iBeacon_ACK_t	iBeacon;
	Ble_Mesh_ACK_t		Mesh;
} Ble_AT_ACK_t;

typedef struct AT_Ack_def
{
	Basic_AT_ACK_t	Basic;
	IOctrl_AT_ACK_t	IOctrl;
	Wifi_AT_ACK_t	Wifi;
	Ble_AT_ACK_t	Ble;
	Expect_t		expect;
	unsigned char*	ack_data;		// AT应答数据
	unsigned int	ack_data_len;	// AT应答数据长度
} AT_Ack_t;
#pragma pack()

AT_Ack_t* get_Ack_t(void);
Ret_Status_e analytic_ack(const char *expect);

#endif
