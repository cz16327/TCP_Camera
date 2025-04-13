#ifndef __AT24C02_H__
#define __AT24C02_H__

#include "main.h"

#define I2Cx		hi2c1			// I2C句柄

#define I2C_TIMEOUT	HAL_MAX_DELAY	// I2C超时时间

#define I2C_WRITE_MEM(hi2c, DevAddress, MemAddress, MemAddSize, pData, Size, Timeout) \
	HAL_I2C_Mem_Write(hi2c, DevAddress, MemAddress, MemAddSize, pData, Size, Timeout)

#define I2C_READ_MEM(hi2c, DevAddress, MemAddress, MemAddSize, pData, Size, Timeout) \
	HAL_I2C_Mem_Read(hi2c, DevAddress, MemAddress, MemAddSize, pData, Size, Timeout)

#define I2C_MEMADD_8BIT	(I2C_MEMADD_SIZE_8BIT)	// 8位地址模式

#define DELAY_MS(ms) osDelay(ms)	// 延时函数

#define AT24C01		(127)
#define AT24C02		(255)
#define AT24C04		(511)
#define AT24C08		(1023)
#define AT24C16		(2047)
#define AT24C32		(4095)
#define AT24C64		(8191)
#define AT24C128	(16383)
#define AT24C256	(32767)

//使用的是24C02，所以定义EE_TYPE为AT24C02
#define EE_TYPE	AT24C02

#define TOTAL_PAGE	(32)	// AT24C02有32页
#define PAGE_SIZE	(8)		// 每页8字节
#define WRITE_ADDR	(0xA0)	// 写入地址
#define READ_ADDR	(0xA1)	// 读写地址
#define MEM_SIZE	(256)	// 24C02的存储器大小为256字节

Ret_Status_e at24c02_write(unsigned char addr, unsigned char *data, unsigned int len);
Ret_Status_e at24c02_read(unsigned char addr, unsigned char *data, unsigned int len);

#endif
