#include "wifi_data.h"

static AT_Ack_t g_Ack_t;

/// @brief 获取AT应答全局单例
/// @param  void
/// @return AT应答全局单例
AT_Ack_t* get_Ack_t(void)
{
	return &g_Ack_t;
}

Ret_Status_e analytic_ack(const char *expect)
{
	Ret_Status_e ret = CZ_ERROR;
	osSemaphoreId_t atAck_Semap = get_atAck_semap();
	osMemoryPoolId_t memPool = get_memoryPoolId_t();
	osSemaphoreAcquire(atAck_Semap, 0);
	if (g_Ack_t.ack_data == NULL || g_Ack_t.ack_data_len == 0) {
		CZ_ERR("ack_data is NULL  size: %d\r\n", g_Ack_t.ack_data_len);
		goto EXIT;
	}

	if (strstr((const char*)g_Ack_t.ack_data, expect) != NULL) {
		// 解析成功
		ret = CZ_BUSY;
	}
EXIT:
	osMemoryPoolFree(memPool, g_Ack_t.ack_data);
	g_Ack_t.ack_data = NULL;
	osSemaphoreRelease(atAck_Semap);
	return ret;
}

static AT_Ack_t g_Ack_t = {
	.expect.ack_OK = "OK",
	.expect.ack_ERROR = "ERROR",
	.expect.ack_sysMsg = "",
};
