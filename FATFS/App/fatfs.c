/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "rtc.h"
#include "sdio.h"
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
	/* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
	unsigned char time[7];
	DWORD seconds = 0;
	rtc_time_get(time);
	seconds = rtc_time_to_seconds(time);
	return seconds;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

const char FR_Table[20][30] = {
	"FR_OK",					/* (0) Succeeded */
	"FR_DISK_ERR",				/* (1) A hard error occurred in the low level disk I/O layer */
	"FR_INT_ERR",				/* (2) Assertion failed */
	"FR_NOT_READY",				/* (3) The physical drive cannot work */
	"FR_NO_FILE",				/* (4) Could not find the file */
	"FR_NO_PATH",				/* (5) Could not find the path */
	"FR_INVALID_NAME",			/* (6) The path name format is invalid */
	"FR_DENIED",				/* (7) Access denied due to prohibited access or directory full */
	"FR_EXIST",					/* (8) Access denied due to prohibited access */
	"FR_INVALID_OBJECT",		/* (9) The file/directory object is invalid */
	"FR_WRITE_PROTECTED",		/* (10) The physical drive is write protected */
	"FR_INVALID_DRIVE",			/* (11) The logical drive number is invalid */
	"FR_NOT_ENABLED",			/* (12) The volume has no work area */
	"FR_NO_FILESYSTEM",			/* (13) There is no valid FAT volume */
	"FR_MKFS_ABORTED",			/* (14) The f_mkfs() aborted due to any problem */
	"FR_TIMEOUT",				/* (15) Could not get a grant to access the volume within defined period */
	"FR_LOCKED",				/* (16) The operation is rejected according to the file sharing policy */
	"FR_NOT_ENOUGH_CORE",		/* (17) LFN working buffer could not be allocated */
	"FR_TOO_MANY_OPEN_FILES",	/* (18) Number of open files > _FS_LOCK */
	"FR_INVALID_PARAMETER"		/* (19) Given parameter is invalid */
};

char filename[] = "STM32F407_SDIO_TEST.txt";
char wtext[] = "SUPER IDIL 的笑容都没你的甜，八月的阳光都没你耀眼爱上，爱上105度的你，还有纯净的蒸馏水";
char rtext[sizeof(wtext)] = {0};
char readfile[] = "SD card test.txt";
char readfile1[] = "SD_TEST_TEXT.txt";

void Fatfs_RW_test(void)
{
	unsigned int fre_clust, fre_sect = 0, tot_sect = 0;
	unsigned int write_count;
	// unsigned int read_count;
	unsigned int br;
	FATFS* fs1;
	retSD = f_mount(&SDFatFS, (TCHAR const*)SDPath, 1);

	if (retSD) {
		CZ_RAW("mount error: %d,%s\r\n", retSD, FR_Table[retSD]);
	} else {
		CZ_RAW("mount success!!!\r\n");
	}

	retSD = f_getfree((const TCHAR*)SDPath, (DWORD*)&fre_clust, &fs1);
	if (retSD == 0) {
		tot_sect = (SDFatFS.n_fatent - 2) * fs1->csize;
		fre_sect = fre_clust * fs1->csize;
		CZ_RAW("total:%dMB, free:%dMB\r\n", tot_sect >> 11, fre_sect >> 11);
	}

	FIL fil;
	// FILINFO finfo;
	// DIR fdir;

	retSD = f_open(&fil, filename, FA_CREATE_ALWAYS | FA_WRITE);
	if (retSD) {
		CZ_RAW("open error: %d,%s\r\n", retSD, FR_Table[retSD]);
	} else {
		CZ_RAW("open file success!!!\r\n");
	}
	memset(rtext, 0X00, sizeof(rtext));
	retSD = f_write(&fil, wtext, sizeof(wtext), (void*)&write_count);
	if (retSD) {
		CZ_RAW("write error: %d,%s\r\n", retSD, FR_Table[retSD]);
	} else {
		CZ_RAW("write file success!!!\r\n");
	}
	f_close(&fil);
	if (retSD) {
		CZ_RAW("mount error: %d,%s\r\n", retSD, FR_Table[retSD]);
	} else {
		CZ_RAW("close success!!!\r\n");
	}

	retSD = f_open(&fil, readfile, FA_READ);
	if (retSD) {
		CZ_RAW("open error: %d,%s\r\n", retSD, FR_Table[retSD]);
	} else {
		CZ_RAW("open file success!!!\r\n");
	}
	memset(rtext, 0X00, sizeof(rtext));
	retSD = f_read(&fil, rtext, 50, &br);
	if (retSD) {
		CZ_RAW("read error: %d,%s\r\n", retSD, FR_Table[retSD]);
	} else {
		CZ_RAW("read file %s success!!!\r\n", "SD card test.txt");
		CZ_RAW("%s\r\n", rtext);
	}
	f_close(&fil);
	if (retSD) {
		CZ_RAW("mount error: %d,%s\r\n", retSD, FR_Table[retSD]);
	} else {
		CZ_RAW("close success!!!\r\n");
	}

	retSD = f_open(&fil, readfile1, FA_READ);
	if (retSD) {
		CZ_RAW("open error: %d,%s\r\n", retSD, FR_Table[retSD]);
	} else {
		CZ_RAW("open file success!!!\r\n");
	}
	memset(rtext, 0X00, sizeof(rtext));
	retSD = f_read(&fil, rtext, 50, &br);
	if (retSD) {
		CZ_RAW("read error: %d,%s\r\n", retSD, FR_Table[retSD]);
	} else {
		CZ_RAW("read file %s success!!!\r\n", "SD_TEST_TEXT.txt");
		CZ_RAW("%s\r\n", rtext);
	}
	f_close(&fil);
	if (retSD) {
		CZ_RAW("mount error: %d,%s\r\n", retSD, FR_Table[retSD]);
	} else {
		CZ_RAW("close success!!!\r\n");
	}

	retSD = f_open(&fil, filename, FA_READ);
	if (retSD) {
		CZ_RAW("open error: %d,%s\r\n", retSD, FR_Table[retSD]);
	} else {
		CZ_RAW("open file success!!!\r\n");
	}
	memset(rtext, 0X00, sizeof(rtext));
	retSD = f_read(&fil, rtext, sizeof(rtext), &br);
	if (retSD) {
		CZ_RAW("read error: %d,%s\r\n", retSD, FR_Table[retSD]);
	} else {
		CZ_RAW("read file %s success!!!\r\n", filename);
		CZ_RAW("%s\r\n", rtext);
	}
	f_close(&fil);
	if (retSD) {
		CZ_RAW("mount error: %d,%s\r\n", retSD, FR_Table[retSD]);
	} else {
		CZ_RAW("close success!!!\r\n");
	}

	retSD = f_mount(NULL, (TCHAR const*)SDPath, 1);
	if (retSD) {
		CZ_RAW("UNmount error: %d,%s\r\n", retSD, FR_Table[retSD]);
	} else {
		CZ_RAW("UNmount success!!! \r\n");
	}
}


// 注意: 本函数需要在f_mount()执行后再调用，因为CubeMX生成的FatFs代码, 会在f_mount()函数内对SD卡进行初始化
/// @brief 获取SD卡信息
/// @param  void
void SDCardInfo(void)
{
	HAL_SD_CardInfoTypeDef pCardInfo = {0}; // SD卡信息结构体
	uint8_t status = HAL_SD_GetCardState(&hsd); // SD卡状态标志值
	if (status == HAL_SD_CARD_TRANSFER)
	{
		HAL_SD_GetCardInfo(&hsd, &pCardInfo); // 获取 SD 卡的信息
		CZ_RAW("\r\n");
		CZ_RAW("*** Get SD Card Info *** \r\n");
		CZ_RAW("SD Card Type: %d \r\n", pCardInfo.CardType);	// 类型返回：0-SDSC、1-SDHC/SDXC、3-SECURED
		CZ_RAW("SD Card Ver:  %d \r\n", pCardInfo.CardVersion);	// 版本返回：0-CARD_V1、1-CARD_V2
		CZ_RAW("Block Number: %d \r\n", pCardInfo.BlockNbr);	// 可用的块数量
		CZ_RAW("Block Size:   %d \r\n", pCardInfo.BlockSize);	// 每个块的大小; 单位：字节
		CZ_RAW("SD Card Size: %lluGB \r\n", ((uint64_t)pCardInfo.BlockSize * pCardInfo.BlockNbr) / 1024 / 1024 / 1024); // 计算卡的容量; 单位：GB
	} else {
		CZ_RAW("SD Card Error: %d\r\n", status);
	}
}
/* USER CODE END Application */
