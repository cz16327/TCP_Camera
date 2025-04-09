#include "wifi_at.h"
#include "wifi_data.h"

extern UART_HandleTypeDef USARTx;

static AT_t g_AT_t;

/// @brief 获取AT指令全局单例
/// @param  void
/// @return AT指令全局单例
AT_t* get_AT_t(void)
{
	return &g_AT_t;
}


Ret_Status_e atcmd_send(unsigned char *cmd, unsigned int len, const char *expect, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	unsigned int timeout = HAL_GetTick();
	HAL_UART_Transmit_DMA(&USARTx, cmd, len);

	if (timeoutS) {
		while (analytic_ack(expect) != CZ_OK) {
			if ((HAL_GetTick() - timeout) > timeoutS * 1000) {
				CZ_ERR("timeout\r\n");
				ret = CZ_TIMEOUT;
			}
		}
	}
	return ret;
}


// static Ret_Status_e test_at(at_type_e type, unsigned int timeoutS)
// {
// 	Ret_Status_e ret = CZ_ERROR;
// 	if (type == Query) { // Query
// 		unsigned char Query_cmd[] = "AT+GMR?\r\n";
// 		CZ_RAW("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);

// 	}
// 	//CZ_RAW("%d\r\n", ret);
// 	return ret;
// }


/// @brief 模组信息
/// @param type 执行方式 Query
/// @param timeoutS 超时时间(s)
static Ret_Status_e mcu_gmr(at_type_e type, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	if (type == Query) { // Query
		unsigned char Query_cmd[] = "AT+GMR?\r\n";
		CZ_RAW("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);

	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}






static AT_t g_AT_t = {
	.Basic.at_gmr = mcu_gmr,
};
