#include "AT24C02.h"

extern I2C_HandleTypeDef I2Cx;

/// @brief AT24C02写入函数
/// @param addr 起始地址
/// @param data 数据
/// @param len 长度
/// @return 结果
Ret_Status_e at24c02_write(unsigned char addr, unsigned char *data, unsigned int len)
{
	Ret_Status_e ret = CZ_ERROR;
	unsigned char total_package = len / PAGE_SIZE; // 包数
	unsigned char last_package_size = len % PAGE_SIZE; // 剩余字节数
	if (len + addr > MEM_SIZE) {
		CZ_ERR("AT24C02 write error: len OVERFLOW [%d]\r\n", len + addr - MEM_SIZE);
		ret = CZ_ERROR;
		goto EXIT;
	}
	if (last_package_size)
		total_package++; // 如果有剩余字节数，则包数加1
	for (unsigned char index = 0; index < total_package; index++) {
		if (index == total_package - 1)
			ret = (Ret_Status_e)I2C_WRITE_MEM(&I2Cx, WRITE_ADDR, addr + index * PAGE_SIZE, I2C_MEMADD_8BIT, &data[index * PAGE_SIZE], last_package_size, I2C_TIMEOUT);
		else
			ret = (Ret_Status_e)I2C_WRITE_MEM(&I2Cx, WRITE_ADDR, addr + index * PAGE_SIZE, I2C_MEMADD_8BIT, &data[index * PAGE_SIZE], PAGE_SIZE, I2C_TIMEOUT);
		if (ret)
			CZ_ERR("AT24C02 write error ret[%d] index[%d]\r\n", ret, index);
		DELAY_MS(5); // 延时5ms
	}
EXIT:
	return ret;
}


/// @brief AT24C02读取函数
/// @param addr 起始地址
/// @param data 缓存
/// @param len 长度
/// @return 结果
Ret_Status_e at24c02_read(unsigned char addr, unsigned char *data, unsigned int len)
{
	Ret_Status_e ret = CZ_ERROR;
	unsigned char total_package = len / PAGE_SIZE; // 包数
	unsigned char last_package_size = len % PAGE_SIZE; // 剩余字节数
	if (len + addr > MEM_SIZE) {
		CZ_ERR("AT24C02 read error: len OVERFLOW [%d]\r\n", len + addr - MEM_SIZE);
		ret = CZ_ERROR;
		goto EXIT;
	}
	if (last_package_size)
		total_package++; // 如果有剩余字节数，则包数加1
	for (unsigned char index = 0; index < total_package; index++) {
		if (index == total_package - 1)
			ret = (Ret_Status_e)I2C_READ_MEM(&I2Cx, READ_ADDR, addr + index * PAGE_SIZE, I2C_MEMADD_8BIT, &data[index * PAGE_SIZE], last_package_size, I2C_TIMEOUT);
		else
			ret = (Ret_Status_e)I2C_READ_MEM(&I2Cx, READ_ADDR, addr + index * PAGE_SIZE, I2C_MEMADD_8BIT, &data[index * PAGE_SIZE], PAGE_SIZE, I2C_TIMEOUT);
		if (ret)
			CZ_ERR("AT24C02 write error ret[%d] index[%d]\r\n", ret, index);
		DELAY_MS(5); // 延时5ms
	}
EXIT:
	return ret;
}
