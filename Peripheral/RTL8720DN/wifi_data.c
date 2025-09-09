#include "wifi_data.h"

static AT_Ack_t g_Ack_t;


/// @brief 获取AT应答全局单例
/// @param  void
/// @return AT应答全局单例
AT_Ack_t* get_Ack_t(void)
{
	return &g_Ack_t;
}


Ret_Status_e analytic_ack(const char* expect)
{
	Ret_Status_e ret = CZ_ERROR;
	osSemaphoreId_t atAck_Semap = get_atAck_semap();
	osMemoryPoolId_t memPool = get_memoryPoolId_t();
	char* expectIndex = NULL;
	if (g_Ack_t.ack_data == NULL) // || g_Ack_t.ack_data_len == 0)
		// CZ_ERR("ack_data is NULL  size: %d\r\n", g_Ack_t.ack_data_len);
		goto EXIT;
	osSemaphoreAcquire(atAck_Semap, 0);
	expectIndex = strstr((const char*)g_Ack_t.ack_data, expect);
	if (expectIndex == NULL) // 解析失败
		goto EXIT;
	osMemoryPoolFree(memPool, g_Ack_t.ack_data);
	g_Ack_t.ack_data = NULL;
	osSemaphoreRelease(atAck_Semap);
EXIT:
	return ret;
}

AT_Ack_t g_Ack_t = {
	// 基础指令应答期望
	.expect.ack_OK = "OK",
	.expect.ack_ERROR = "ERROR",
	.expect.ack_sysMsg = "+SYSMSG:",
	.expect.ack_atVer = "at version:",
	.expect.ack_sdkVer = "sdk version:",
	.expect.ack_firmwareVer = "firmware version:",
	.expect.ack_flashManufacturerID = "Flash Manufacturer ID:",
	.expect.ack_flashDeviceID = "Flash Device ID:",
	.expect.ack_flashSize = "Flash Capacity ID:",
	.expect.ack_uartCfg = "+UARTCFG:",
	.expect.ack_uartFlowCfg = "+UARTFLOWCONTROL:",
	.expect.ack_ota = "+OTA:",
	.expect.ack_tickless = "+TICKLESS:",

	// IO控制指令应答期望
	.expect.ack_ioMap = "+SYSIOMAP:",
	.expect.ack_ioRead = "+SYSGPIOREAD:",

	// WIFI基础指令应答期望
	.expect.ack_wifiMode = "+WMODE:",
	.expect.ack_wifiScan = "+WSCAN:index SSID,CH,SECURITY,RSSI,BSSID",
	.expect.ack_wifiScanActive = "+WSCANACTIVE:",
	.expect.ack_wifiStaDhcp = "+WSDHCP:",
	.expect.ack_wifiInfoUnused = "+WJAP:",
	.expect.ack_wifiInfo = "+STAINFO:",
	.expect.ack_wifiJEAP = "+EVENT:WIFI_GOT_IP",
	.expect.ack_wifiAutoCon = "+WAUTOCONN:",
	.expect.ack_wifiApDhcp = "+WAPDHCP:",
	.expect.ack_wifiAp = "+WPA:",
	.expect.ack_wifiPing = "+PING:",
	.expect.ack_wifiCipStaMacDef = "+CIPSTAMAC_DEF:",
	.expect.ack_wifiCountry = "+WCOUNTRY:",
	.expect.ack_wifiConfig = "+WCONFIG:",
	.expect.ack_wifiScanOpt = "+WSCANOPT:",
	.expect.ack_wifiRssi = "+WRSSI :",
	.expect.ack_wifiSeriaNet = ">",
	.expect.ack_wifiSocketRead = "+SOCKETREAD:",
	.expect.ack_wifiSocketAutoTt = "+SOCKETAUTOTT:",
	.expect.ack_wifiSslCret = "+SSLCRET:",
	.expect.ack_wifiDoamin = "+WDOMAIN:",
	.expect.ack_wifiDns = "+WDNS:",
	// WIFI URC
	.urc.ack_eventSocketDown = "+EVENT:SocketDown",
	.urc.ack_eventSocketSeed = "+EVENT:SocketSeed"
};
