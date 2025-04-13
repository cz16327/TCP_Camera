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


/// @brief 删除 AT 指令最后的逗号
/// @param cmd 指令内容
/// @param len 指令长度
static void delete_cmd_last_comma(unsigned char *cmd, unsigned int *len)
{
	if (*len < 3) {
		CZ_ERR("AT cmd error len < 3 \r\n");
		return;
	}
	if (cmd[*len - 2] != '\r' || cmd[*len - 1] != '\n') {
		CZ_ERR("AT cmd error need \\r\\n [%02X %02X]\r\n", cmd[*len - 2], cmd[*len - 1]);
		return;
	}
	unsigned char* ptail = &cmd[*len - 3];
	while (ptail >= cmd && *ptail == ',') {
		*ptail = *(ptail + 1);
		*(ptail + 1) = *(ptail + 2);
		*(ptail + 2) = '\0';
		ptail++;
		*len--;
	}
}


/// @brief AT 指令发送函数
/// @param cmd 指令字符串
/// @param len 指令长度
/// @param expect 期望字符串
/// @param timeoutS 超时时间（s）
/// @return 结果
Ret_Status_e atcmd_send(unsigned char* cmd, unsigned int len, const char* expect, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	unsigned int timeout = HAL_GetTick();
	delete_cmd_last_comma(cmd, &len);
	CZ_LOG("%d[%d] %s\r\n", strlen((const char*)cmd), len, cmd);
	HAL_UART_Transmit_DMA(&USARTx, cmd, len);
	while (USARTx.gState != HAL_UART_STATE_READY) {
        if ((HAL_GetTick() - timeout) > WAIT_USART_DMA) {
            CZ_ERR("USART DMA send timeout\r\n");
            ret = CZ_BUSY;
            goto EXIT;
        }
    }
	if (timeoutS) {
		while (ret != CZ_OK) {
			ret = analytic_ack(expect);
			if ((HAL_GetTick() - timeout) > timeoutS * 1000) {
				CZ_ERR("timeout\r\n");
				ret = CZ_TIMEOUT;
				goto EXIT;
			}
		}
	}
EXIT:
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
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
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
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
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
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
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
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
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
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
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
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
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
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_sysMsg, timeoutS);
	} else if (type == Set) { // Set
		unsigned char Set_cmd[25];
		sprintf((char*)Set_cmd, "AT+SYSMSG=%d,%08x,%d\r\n", mode, mask, saveFlash);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
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
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
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
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
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
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
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
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_uartCfg, timeoutS);
	} else if (type == Set) { // Set
		unsigned char Set_cmd[30];
		sprintf((char*)Set_cmd, "AT+UARTCFG=%d,%d,%d,%d\r\n", baudrate, databits, stopbits, parity);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
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
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_uartFlowCfg, timeoutS);
	} else if (type == Set) { // Set
		unsigned char Set_cmd[30];
		sprintf((char*)Set_cmd, "AT+UARTFLOWCONTROL=%d\r\n", flowcontrol);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
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
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
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
static Ret_Status_e mcu_ota(At_type_e type, OTA_mode_e mode, unsigned char* Host_name, unsigned short Port, unsigned char* Route, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Query) { // Query
		unsigned char Query_cmd[] = "AT+OTA?\r\n";
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_ota, timeoutS);
	} else if (type == Set) { // Set
		if (!Host_name)
			Host_name = "";
		if (!Route)
			Route = "";
		unsigned char Set_cmd[strlen((const char*)Host_name) + strlen((const char*)Route) + 20];
		sprintf((char*)Set_cmd, "AT+OTA=%d,%s,%d,%s\r\n", mode, Host_name, Port, Route);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	} else if (type == Excution) { // Excution
		unsigned char Excution_cmd[] = "AT+OTA\r\n";
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
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
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_tickless, timeoutS);
	} else if (type == Set) { // Set
		unsigned char Set_cmd[20];
		sprintf((char*)Set_cmd, "AT+OTA=%d\r\n", tickless);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 查询或设置 IO 映射表
/// @param type 类型 Query / Set
/// @param PinNumber 引脚总数
/// @param pinx_list 引脚映射列表
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e io_map(At_type_e type, unsigned char PinNumber, unsigned char* pinx_list, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Query) { // Query
		unsigned char Query_cmd[] = "AT+SYSIOMAP?\r\n";
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_ioMap, timeoutS);
	} else if (type == Set) { // Set
		if (!pinx_list)
			pinx_list = "";
		// BW16 默认映射 AT+SYSIOMAP=16,21,34,NC,23,NC,26,29,NC,NC,30,NC,22,27,20,NC,NC
		unsigned char Set_cmd[strlen((const char*)pinx_list) + 20];
		sprintf((char*)Set_cmd, "AT+SYSIOMAP=%d,%s\r\n", PinNumber, pinx_list);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 设置 GPIO 输出电平
/// @param type 类型 Query / Set
/// @param pin 引脚号
/// @param level 电平类型
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e io_write(At_type_e type, unsigned char pin, IO_level_e level, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Set) { // Set
		unsigned char Set_cmd[25];
		sprintf((char*)Set_cmd, "AT+SYSGPIOWRITE=%d,%d\r\n", pin, level);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 读取 GPIO 电平
/// @param type 类型 Set
/// @param pin 引脚号
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e io_read(At_type_e type, unsigned char pin, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Set) { // Set
		unsigned char Set_cmd[25];
		sprintf((char*)Set_cmd, "AT+SYSGPIOREAD=%d\r\n", pin);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_ioRead, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 配置 PWM 功能（PWM同时开启时必须注意芯片引脚的 IO 序号对 5 取余不能重复，否则只会有一个生效，例如设置了 IO1/2/6 实际只有 IO2/6 生效，IO1 被 IO6 覆盖了）
/// @param type 类型 Set
/// @param pin 引脚号
/// @param cycle PWM 周期(us)
/// @param duty 占空比时间(us)
/// @param timeoutS 超时时间(s)
/// @return 结果
// __attribute__((deprecated("io_pwmCfgUs is legacy, use io_pwmCfgPer instead of io_pwmCfgUs")))
static Ret_Status_e io_pwmCfgUs(At_type_e type, unsigned char pin, unsigned int cycle, unsigned int duty, unsigned int timeoutS)
{
	fprintf(stderr, "io_pwmCfgUs is legacy, use io_pwmCfgPer instead of io_pwm\r\n");
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Set) { // Set
		unsigned char Set_cmd[25];
		sprintf((char*)Set_cmd, "AT+PWMCFG=%d,%d,%d\r\n", pin, cycle, duty);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 配置 PWM 功能（参考 AT+PWMCFG 配置 PWM 功能中注意事项）
/// @param type 类型 Set
/// @param pin 引脚号
/// @param cycle PWM 周期(us)
/// @param duty 整数 0~100 表示占空比的百分比
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e io_pwmCfgPer(At_type_e type, unsigned char pin, unsigned int cycle, unsigned int duty, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Set) { // Set
		unsigned char Set_cmd[25];
		sprintf((char*)Set_cmd, "AT+PWMCFGS=%d,%d,%d\r\n", pin, cycle, duty);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 关闭 PWM 功能
/// @param type 类型 Set
/// @param pin 引脚号
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e io_pwmStop(At_type_e type, unsigned char pin, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Set) { // Set
		unsigned char Set_cmd[25];
		sprintf((char*)Set_cmd, "AT+PWMSTOP=%d\r\n", pin);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 更新 PWM 占空比
/// @param type 类型 Set
/// @param pin 引脚号
/// @param duty 占空比时间(us)
/// @param timeoutS 超时时间(s)
/// @return 结果
// __attribute__((deprecated("io_pwmSetDutyUs is legacy, use io_pwmSetDutyPer instead of io_pwmSetDutyUs")))
static Ret_Status_e io_pwmSetDutyUs(At_type_e type, unsigned char pin, unsigned int duty, unsigned int timeoutS)
{
	fprintf(stderr, "io_pwmSetDutyUs is legacy, use io_pwmSetDutyPer instead of io_pwm\r\n");
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Set) { // Set
		unsigned char Set_cmd[25];
		sprintf((char*)Set_cmd, "AT+PWMDUTYSET=%d,%d\r\n", pin, duty);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}

/// @brief 更新 PWM 占空比
/// @param type 类型 Set
/// @param pin 引脚号
/// @param duty 整数 0~100 表示占空比的百分比
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e io_pwmSetDutyPer(At_type_e type, unsigned char pin, unsigned int duty, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Set) { // Set
		unsigned char Set_cmd[25];
		sprintf((char*)Set_cmd, "AT+PWMDUTYSETS=%d,%d\r\n", pin, duty);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 查询或设置 WIFI 工作模式（包含AP的多模式混用时必须先开启AP模式，例如AP+STA+BT，必须先AP，其他随意 ）
/// @param type 类型 Query / Set
/// @param mode 工作模式
/// @param saveFlash 是否保存到Flash
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e wifi_mode(At_type_e type, Wifi_mode_e mode, Save_e saveFlash, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Query) { // Query
		unsigned char Query_cmd[] = "AT+WMODE?\r\n";
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_wifiMode, timeoutS);
	} else if (type == Set) { // Set
		unsigned char Set_cmd[20];
		if (mode == UNINIT_MODE)
			saveFlash = UNSAVE;
		sprintf((char*)Set_cmd, "AT+WMODE=%d,%d\r\n", mode, saveFlash);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 断开 wifi 连接
/// @param type 类型 Excution
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e wifi_disconnect(At_type_e type, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Excution) { // Excution
		unsigned char Excution_cmd[] = "AT+WDISCONNECT\r\n";
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Excution_cmd), Excution_cmd);
		ret = atcmd_send(Excution_cmd, strlen((const char*)Excution_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 扫描 WIFI 列表
/// @param type 类型 Query / Set
/// @param ssid 扫描指定的 SSID
/// @param mac 扫描指定的 mac 地址
/// @param channel 扫描指定的通道号
/// @param rssi 过滤掉信号强度低于 rssi 参数值的 AP，单位：dBm，默认值：-100，范围：[-100,40]
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e wifi_scan(At_type_e type, unsigned char* ssid, unsigned char* mac, unsigned char channel, char rssi, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Query) { // Query
		unsigned char Query_cmd[] = "AT+WSCAN?\r\n";
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_wifiScan, timeoutS);
	} else if (type == Set) { // Set
		if (!ssid)
			ssid = "";
		if (!mac)
			mac = "";
		unsigned char Set_cmd[strlen((const char*)ssid) + strlen((const char*)mac) + 30];
		sprintf((char*)Set_cmd, "AT+WSCAN=%s,%s,%d,%d\r\n", ssid, mac, channel, rssi);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_wifiScan, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 主动模式扫描指定 SSID
/// @param type 类型 Set
/// @param ssid 需要扫描的 SSID
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e wifi_scanActive(At_type_e type, unsigned char* ssid, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Set) { // Set
		if (!ssid)
			ssid = "";
		unsigned char Set_cmd[strlen((const char*)ssid) + 20];
		sprintf((char*)Set_cmd, "AT+WSCANACTIVE=%s\r\n", ssid);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_wifiScanActive, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 查询或设置 STA 模式下 DHCP 参数
/// @param type 类型 Query / Set
/// @param mode DHCP 模式
/// @param ip IP 地址
/// @param mask 子网掩码
/// @param gateway 网关地址
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e wifi_staDhcp(At_type_e type, IP_mode_e mode, unsigned char* ip, unsigned char* mask, unsigned char* gateway, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Query) { // Query
		unsigned char Query_cmd[] = "AT+WSDHCP?\r\n";
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_wifiStaDhcp, timeoutS);
	} else if (type == Set) { // Set
		if (!ip)
			ip = "";
		if (!mask)
			mask = "";
		if (!gateway)
			gateway = "";
		unsigned char Set_cmd[strlen((const char*)ip) + strlen((const char*)mask) + strlen((const char*)gateway) + 30];
		sprintf((char*)Set_cmd, "AT+WSDHCP=%d,%s,%s,%s\r\n", mode, ip, mask, gateway);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 连接 AP
/// @param type 类型 Query / Set
/// @param ssid 连接的 AP 的 SSID
/// @param pwd 密码
/// @param bssid 连接的 AP 的 mac 地址,小写 16 进制，用冒号分隔(当有个多个同名 SSID 可以使用 bssid 区分 eg:94:c9:60:12:34:56)
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e wifi_connect(At_type_e type, unsigned char* ssid, unsigned char* pwd, unsigned char* bssid, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Query) { // Query
		fprintf(stderr, "AT+WJAP? is legacy, use AT+STAINFO? instead of AT+WJAP?\r\n");
		unsigned char Query_cmd[] = "AT+WJAP?\r\n";
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_wifiInfoUnused, timeoutS);
	} else if (type == Set) { // Set
		if (!ssid)
			ssid = "";
		if (!pwd)
			pwd = "";
		if (!bssid)
			bssid = "";
		unsigned char Set_cmd[strlen((const char*)ssid) + strlen((const char*)pwd) + strlen((const char*)bssid) + 20];
		sprintf((char*)Set_cmd, "AT+WJAP=%s,%s,%s\r\n", ssid, pwd, bssid);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 查询 wifi 连接信息
/// @param type 类型 Query
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e wifi_info(At_type_e type, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Query) { // Query
		unsigned char Query_cmd[] = "AT+STAINFO?\r\n";
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_wifiInfo, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 连接企业认证热点
/// @param type 类型 Set
/// @param EAPtype EAP 加密方式
/// @param ssid 连接的 AP 的 SSID
/// @param identity 登录用户名
/// @param pwd 用户密码
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e wifi_companyAP(At_type_e type, EAP_type_e EAPtype, unsigned char* ssid, unsigned char* identity, unsigned char* pwd, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Set) { // Set
		if (!ssid)
			ssid = "";
		if (!identity)
			identity = "";
		if (!pwd)
			pwd = "";
		unsigned char Set_cmd[strlen((const char*)ssid) + strlen((const char*)identity) + strlen((const char*)pwd) + 20];
		sprintf((char*)Set_cmd, "AT+WJEAP=%d,%s,%s,%s\r\n", EAPtype, ssid, identity, pwd);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_wifiEAP, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 上电自动重连 wifi
/// @param type 类型 Query
/// @param enable 使失能
/// @param ssid 连接的 AP 的 SSID
/// @param pwd 连接密码
/// @param bssid 连接的 AP 的 mac 地址,小写 16 进制，用冒号分隔(当有个多个同名 SSID 可以使用 bssid 区分 eg:94:c9:60:12:34:56)
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e wifi_autoConn(At_type_e type, BW_enable_e enable, unsigned char* ssid, unsigned char* pwd, unsigned char* bssid, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Query) { // Query
		unsigned char Query_cmd[] = "AT+WAUTOCONN?\r\n";
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_autoCon, timeoutS);
	} else if (type == Set) { // Set
		if (!ssid)
			ssid = "";
		if (!pwd)
			pwd = "";
		if (!bssid)
			bssid = "";
		unsigned char Set_cmd[strlen((const char*)ssid) + strlen((const char*)pwd) + strlen((const char*)bssid) + 25];
		sprintf((char*)Set_cmd, "AT+WAUTOCONN=%d,%s,%s,%s\r\n", enable, ssid, pwd, bssid);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


/// @brief 查询或设置 AP 模式下 DHCP 参数
/// @param type 类型 Query
/// @param enable 使失能
/// @param start_ip 起始 IP 地址
/// @param end_ip 结束 IP 地址
/// @param gateway 网关 IP(使用 DHCP 时模组 IP 就是网关 IP)，使能 DHCP 时需设置。eg：192.168.43.1
/// @param timeoutS 超时时间(s)
/// @return 结果
static Ret_Status_e wifi_apDhcp(At_type_e type, BW_enable_e enable, unsigned char* start_ip, unsigned char* end_ip, unsigned char* gateway, unsigned int timeoutS)
{
	Ret_Status_e ret = CZ_ERROR;
	AT_Ack_t* ack = get_Ack_t();
	if (type == Query) { // Query
		unsigned char Query_cmd[] = "AT+WAUTOCONN?\r\n";
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Query_cmd), Query_cmd);
		ret = atcmd_send(Query_cmd, strlen((const char*)Query_cmd), ack->expect.ack_wifiApDhcp, timeoutS);
	} else if (type == Set) { // Set
		if (!start_ip)
			start_ip = "";
		if (!end_ip)
			end_ip = "";
		if (!gateway)
			gateway = "";
		unsigned char Set_cmd[strlen((const char*)start_ip) + strlen((const char*)end_ip) + strlen((const char*)gateway) + 25];
		sprintf((char*)Set_cmd, "AT+WAUTOCONN=%d,%s,%s,%s\r\n", enable, start_ip, end_ip, gateway);
		// CZ_LOG("[%d] %s\r\n", strlen((const char*)Set_cmd), Set_cmd);
		ret = atcmd_send(Set_cmd, strlen((const char*)Set_cmd), ack->expect.ack_OK, timeoutS);
	}
	//CZ_RAW("%d\r\n", ret);
	return ret;
}


static AT_t g_AT_t = {
	// 基础指令
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

	// IO 控制指令
	.IOctrl.at_sysIoMap = io_map,
	.IOctrl.at_sysGpioWrite = io_write,
	.IOctrl.at_sysGpioRead = io_read,
	.IOctrl.at_pwmCfg = io_pwmCfgUs,
	.IOctrl.at_pwmCfgs = io_pwmCfgPer,
	.IOctrl.at_pwmStop = io_pwmStop,
	.IOctrl.at_pwmDutySet = io_pwmSetDutyUs,
	.IOctrl.at_pwmDutySets = io_pwmSetDutyPer,

	// WIFI 控制指令
	.Wifi.Basic.at_wMode = wifi_mode,
	.Wifi.Basic.at_wDisconnect = wifi_disconnect,
	.Wifi.Basic.at_wScan = wifi_scan,
	.Wifi.Basic.at_wScanActive = wifi_scanActive,
	.Wifi.Basic.at_wSDhcp = wifi_staDhcp,
	.Wifi.Basic.at_wJap = wifi_connect,
	.Wifi.Basic.at_staInfo = wifi_info,
	.Wifi.Basic.at_wJeap = wifi_companyAP,
	.Wifi.Basic.at_wAutoConn = wifi_autoConn,
	.Wifi.Basic.at_wApDhcp = wifi_apDhcp,
};
