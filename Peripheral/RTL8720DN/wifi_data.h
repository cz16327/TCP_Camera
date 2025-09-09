#ifndef __WIFI_DATA_H__
#define __WIFI_DATA_H__

#include "main.h"

#pragma pack(1)
#pragma pack()

typedef struct Expect_def
{
	// 基础指令应答期望
	const char* const ack_OK;					// "OK"
	const char* const ack_ERROR;				// "ERROR"
	const char* const ack_sysMsg;				// "+SYSMSG:"
	const char* const ack_atVer;				// "at version:"
	const char* const ack_sdkVer;				// "sdk version:"
	const char* const ack_firmwareVer;			// "firmware version:"
	const char* const ack_flashManufacturerID;	// "Flash Manufacturer ID:"
	const char* const ack_flashDeviceID;		// "Flash Device ID:"
	const char* const ack_flashSize;			// "Flash Capacity ID:"
	const char* const ack_uartCfg;				// "+UARTCFG:"
	const char* const ack_uartFlowCfg;			// "+UARTFLOWCONTROL:"
	const char* const ack_ota;					// "+OTA:"
	const char* const ack_tickless;				// "+TICKLESS:"
	// IO控制指令应答期望
	const char* const ack_ioMap;				// "+SYSIOMAP:"
	const char* const ack_ioRead;				// "+SYSGPIOREAD:"
	// WIFI基础指令应答期望
	const char* const ack_wifiMode;				// "+WMODE:"
	const char* const ack_wifiScan;				// "+WSCAN:index SSID,CH,SECURITY,RSSI,BSSID"
	const char* const ack_wifiScanActive;		// "+WSCANACTIVE:"
	const char* const ack_wifiStaDhcp;			// "+WSDHCP:"
	const char* const ack_wifiInfoUnused;		// "+WJAP:"
	const char* const ack_wifiInfo;				// "+STAINFO:"
	const char* const ack_wifiJEAP;				// "+EVENT:WIFI_GOT_IP"
	const char* const ack_wifiAutoCon;			// "+WAUTOCONN:"
	const char* const ack_wifiApDhcp;			// "+WAPDHCP:"
	const char* const ack_wifiAp;				// "+WPA:"
	const char* const ack_wifiPing;				// "+PING:"
	const char* const ack_wifiCipStaMacDef;		// "+CIPSTAMAC_DEF:"
	const char* const ack_wifiCountry;			// "+WCOUNTRY:"
	const char* const ack_wifiConfig;			// "+WCONFIG:"
	const char* const ack_wifiScanOpt;			// "+WSCANOPT:"
	const char* const ack_wifiRssi;				// "+WRSSI :"
	const char* const ack_wifiSeriaNet;			// ">"
	const char* const ack_wifiSocketRead;		// "+SOCKETREAD:"
	const char* const ack_wifiSocketAutoTt;		// "+SOCKETAUTOTT:"
	const char* const ack_wifiSslCret;			// "+SSLCRET:"
	const char* const ack_wifiDoamin;			// "+WDOMAIN:"
	const char* const ack_wifiDns;				// "+WDNS:"
} Expect_t;

typedef struct URC_def
{
	const char* const ack_eventSocketDown;		// "+EVENT:SocketDown"
	const char* const ack_eventSocketSeed;		// "+EVENT:SocketSeed"
} URC_t;

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
	URC_t			urc;
	unsigned char* ack_data;		// AT应答数据
	unsigned int	ack_data_len;	// AT应答数据长度
} AT_Ack_t;

AT_Ack_t* get_Ack_t(void);
Ret_Status_e analytic_ack(const char* expect);

#endif
