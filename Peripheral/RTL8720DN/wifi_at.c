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


/// @brief AT 指令发送函数
/// @param cmd 指令字符串
/// @param len 指令长度
/// @param expect 期望字符串
/// @param timeoutS 超时时间（s）
/// @return 结果
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


/// @brief AT 测试指令
/// @param type 类型 Excution
/// @param timeoutS 超时时间（s）
/// @return 结果
static Ret_Status_e test_at(At_type_e type, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Excution) { // Excution
		unsigned char Excution_cmd[] = "AT\r\n";
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
		ret = atcmd_send(Excution_cmd, strlen((const char*)Excution_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 查看 AT 指令集
/// @param type 类型 Excution
/// @param timeoutS 超时时间（s）
/// @return 结果
static Ret_Status_e help_list(At_type_e type, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Excution) { // Excution
		unsigned char Excution_cmd[] = "AT+HELP\r\n";
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
		ret = atcmd_send(Excution_cmd, strlen((const char*)Excution_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 模块重启指令
/// @param type 类型 Excution
/// @param timeoutS 超时时间（s）
/// @return 结果
static Ret_Status_e mcu_rst(At_type_e type, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Excution) { // Excution
		unsigned char Excution_cmd[] = "AT+RST\r\n";
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
		ret = atcmd_send(Excution_cmd, strlen((const char*)Excution_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 恢复出厂设置
/// @param type 类型 Excution
/// @param timeoutS 超时时间（s）
/// @return 结果
static Ret_Status_e mcu_restore(At_type_e type, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Excution) { // Excution
		unsigned char Excution_cmd[] = "AT+RESTORE\r\n";
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
		ret = atcmd_send(Excution_cmd, strlen((const char*)Excution_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 打开回显
/// @param type 类型 Excution
/// @param timeoutS 超时时间（s）
/// @return 结果
static Ret_Status_e mcu_echo_enable(At_type_e type, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Excution) { // Excution
		unsigned char Excution_cmd[] = "ATE1\r\n";
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
		ret = atcmd_send(Excution_cmd, strlen((const char*)Excution_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 关闭回显
/// @param type 类型 Excution
/// @param timeoutS 超时时间（s）
/// @return 结果
static Ret_Status_e mcu_echo_disable(At_type_e type, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Excution) { // Excution
		unsigned char Excution_cmd[] = "ATE0\r\n";
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
		ret = atcmd_send(Excution_cmd, strlen((const char*)Excution_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 查询或设置系统提示信息
/// @param type 类型 Query / Set
/// @param mode 指定需要设置掩码的模式
/// @param mask 打印掩码
/// @param saveFlash 是否保存到Flash
/// @param timeoutS 超时时间（s）
/// @return 结果
static Ret_Status_e mcu_sysMsgCfg(At_type_e type, SysMsg_mode_e mode, Mask_e mask, Save_e saveFlash, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Query) { // Query
		unsigned char Query_cmd[] = "AT+SYSMSG?\r\n";
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_sysMsg, timeoutS);
	} else if (type == Set) { // Set
		unsigned char Set_cmd[25];
		sprintf((char*)Set_cmd, "AT+SYSMSG=%d,%08x,%d\r\n", mode, mask, saveFlash);
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 查询版本信息
/// @param type 类型 Excution
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e mcu_version(At_type_e type, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Excution) { // Excution
		unsigned char Excution_cmd[] = "AT+GMR\r\n";
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
		ret = atcmd_send(Excution_cmd, strlen((const char*)Excution_cmd), ack->expect.ack_atVer, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 查询 FLASH ID
/// @param type 类型 Excution
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e mcu_flashID(At_type_e type, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Excution) { // Excution
		unsigned char Excution_cmd[] = "AT+FLASHID\r\n";
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
		ret = atcmd_send(Excution_cmd, strlen((const char*)Excution_cmd), ack->expect.ack_flashManufacturerID, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 睡眠模式
/// @param type 类型 Set
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e mcu_sleep(At_type_e type, Sleep_mode_e mode, Wakeup_source_e wake, unsigned int ms_pin, GPIO_wake_level_e level, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Set) { // Set
		unsigned char Set_cmd[30];
		sprintf((char*)Set_cmd, "AT+SLEEP=%d,%d,%d,%d\r\n", mode, wake, ms_pin, level);
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 串口设置指令
/// @param type 类型 Query / Set
/// @param baudrate 波特率
/// @param databits 数据位
/// @param stopbits 停止位
/// @param parity 奇偶校验
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e mcu_uartCfg(At_type_e type, unsigned int baudrate, Uart_databits_e databits, Uart_stopbits_e stopbits, Uart_parity_e parity, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Query) { // Query
		unsigned char Query_cmd[] = "AT+UARTCFG?\r\n";
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_uartCfg, timeoutS);
	} else if (type == Set) { // Set
		unsigned char Set_cmd[30];
		sprintf((char*)Set_cmd, "AT+UARTCFG=%d,%d,%d,%d\r\n", baudrate, databits, stopbits, parity);
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 串口设置流控
/// @param type 类型 Query / Set
/// @param flowcontrol 流控类型
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e mcu_uartFlowCfg(At_type_e type, Uart_flow_e flowcontrol, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Query) { // Query
		unsigned char Query_cmd[] = "AT+UARTFLOWCONTROL?\r\n";
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_uartFlowCfg, timeoutS);
	} else if (type == Set) { // Set
		unsigned char Set_cmd[30];
		sprintf((char*)Set_cmd, "AT+UARTFLOWCONTROL=%d\r\n", flowcontrol);
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 进入下载模式
/// @param type 类型 Set
/// @param mode 下载模式
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e mcu_download(At_type_e type, Download_mode_e mode, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Set) { // Set
		unsigned char Set_cmd[30];
		sprintf((char*)Set_cmd, "AT+SETDOWNLOADMODE=%d\r\n", mode);
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 在线升级指令
/// @param type 类型 Query / Set / Excution
/// @param mode 下载方式
/// @param Host_name 服务器域名
/// @param Port 服务器端口号
/// @param Route 要下载的资源地址
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e mcu_ota(At_type_e type, OTA_mode_e mode, unsigned char *Host_name, unsigned short Port, unsigned char *Route, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Query) { // Query
		unsigned char Query_cmd[] = "AT+OTA?\r\n";
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_ota, timeoutS);
	}else if (type == Set) { // Set
		unsigned char Set_cmd[200];
		sprintf((char*)Set_cmd, "AT+OTA=%d,%s,%d,%s\r\n", mode, Host_name, Port, Route);
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	} else if (type == Excution) { // Excution
		unsigned char Excution_cmd[] = "AT+OTA\r\n";
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
		ret = atcmd_send(Excution_cmd, strlen((const char*)Excution_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 查询或设置进入浅睡眠模式监听 AP beacon 的间隔（需配合 AT+SLEEP 指令使用）
/// @param type 类型 Query / Set / Excution
/// @param tickless 进入浅睡眠模监听 AP beacon 的间隔，单位为 AP beacon 间隔，默认值：10，范围：[1,100]
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e mcu_tickless(At_type_e type, unsigned char tickless, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Query) { // Query
		unsigned char Query_cmd[] = "AT+TICKLESS?\r\n";
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_tickless, timeoutS);
	}else if (type == Set) { // Set
		unsigned char Set_cmd[20];
		sprintf((char*)Set_cmd, "AT+OTA=%d\r\n", tickless);
		CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


static AT_t g_AT_t = {
	.Basic.at = test_at,
	.Basic.at_help = help_list,
	.Basic.at_rst = mcu_rst,
	.Basic.at_restore = mcu_restore,
	.Basic.ate1 = mcu_echo_enable,
	.Basic.ate0 = mcu_echo_disable,
	.Basic.at_sysMsg = mcu_sysMsgCfg,
	.Basic.at_gmr = mcu_version,
	.Basic.at_flashId = mcu_flashID,
	.Basic.at_sleep = mcu_sleep,
	.Basic.at_uartCfg = mcu_uartCfg,
	.Basic.at_uartFlowControl = mcu_uartFlowCfg,
	.Basic.at_setDownloadMode = mcu_download,
	.Basic.at_ota = mcu_ota,
	.Basic.at_tickless = mcu_tickless,
};
