/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "iwdg.h"
#include "gpio.h"
#include "tim.h"
#include "iwdg.h"
#include "wifi_at.h"
#include "AT24C02.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// 定义内存池对象
osMemoryPoolId_t memoryPoolId;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
#define BLOCK_START_ADDR         0     /* Block start address      */
#define NUM_OF_BLOCKS            1   /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */
uint8_t Buffer_Tx[512],Buffer_Rx[512] = {0};
uint32_t i;
/* USER CODE END Variables */
/* Definitions for startTask */
osThreadId_t startTaskHandle;
const osThreadAttr_t startTask_attributes = {
  .name = "startTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for atAck_semaphores */
osSemaphoreId_t atAck_semaphoresHandle;
const osSemaphoreAttr_t atAck_semaphores_attributes = {
  .name = "atAck_semaphores"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/// @brief 打印任务信息
/// @param  void
void dump_task_info(void)
{
	static char InfoBuffer[512] = {0};
	vPortEnterCritical();
	vTaskList((char *) &InfoBuffer);
	CZ_RAW("task          state  priority  stack   NO.\r\n");
	CZ_RAW("\r\n%s\r\n", InfoBuffer);
	vPortExitCritical();
}


osSemaphoreId_t get_atAck_semap(void)
{
	return atAck_semaphoresHandle;
}


osMemoryPoolId_t get_memoryPoolId_t(void)
{
	return memoryPoolId;
}


/// @brief 初始化内存池
/// @brief void
/// @param  void
void memoryPool_init(void)
{
	// 创建内存池
	memoryPoolId = osMemoryPoolNew(NUM_MEMORY_BLOCKS, MEMORY_BLOCK_SIZE, NULL);
	if (memoryPoolId == NULL) {
		CZ_ERR("Failed to create memory pool\n");
		NVIC_SystemReset();
	}
}
/* USER CODE END FunctionPrototypes */

void StartTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char* pcTaskName)
{
	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
	called if a stack overflow is detected. */
	CZ_RAW("task : \"%s\" overflow\r\n", pcTaskName);
}
/* USER CODE END 4 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
		/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of atAck_semaphores */
  atAck_semaphoresHandle = osSemaphoreNew(1, 1, &atAck_semaphores_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
		/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
		/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
		/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of startTask */
  startTaskHandle = osThreadNew(StartTask, NULL, &startTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
		/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
		/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartTask */
/**
  * @brief  Function implementing the startTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTask */
void StartTask(void *argument)
{
  /* USER CODE BEGIN StartTask */
	memoryPool_init();
	cz_log_set_level(LOG_LEVEL_DBG);
	dump_task_info();
	tim_vbat_adc_enable(CZ_ENABLE);
	WIFI_EN;
	WIFI_PWRON;
	CAM_EN;
	CAM_PWRON;
	SD_PWRON;
	VBAT_ADC_EN;
	unsigned char* wifi_ssid = "potato";
	unsigned char* wifi_pwd = "erbailailaixiguatudou20160327.";
	AT_t* AT = get_AT_t();
	AT->Basic.at_rst(Excution, 0); // 重启模块
	AT->Wifi.Basic.at_wMode(Set, STA_MODE, SAVE, 0); // 设置模块的 Wi-Fi 工作模式并保存到 Flash
	AT->Wifi.Basic.at_wJap(Set, wifi_ssid, wifi_pwd, NULL, 0); // 连接目标路由器
	// AT->Wifi.Basic.at_wAutoConn(Set, BW_ENABLE, wifi_ssid, wifi_pwd, NULL, 0);
	/* Infinite loop */
	// unsigned int i = 0;
	// Ret_Status_e ii;
	// Ret_Status_e iii;
	// unsigned char data[30] = {0};
	// unsigned char get_data[30] = {0};
	extern SD_HandleTypeDef hsd;
	for (;;) {
		// i++;
		// sprintf((char *)data, "hello AT24C02 %d !\r\n", i);
		// ii = at24c02_write(0, data, sizeof(data));
		// osDelay(10);
		// iii = at24c02_read(0, get_data, sizeof(get_data));
		// CZ_RAW("%s [%d %d]\r\n", get_data, ii, iii);

		CZ_RAW("Micro SD Card Test...\r\n");
		/* 检测SD卡是否正常（处于数据传输模式的传输状态） */
		if (HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER) {
			CZ_RAW("Initialize SD card successfully!\r\n");
			// 打印SD卡基本信息
			CZ_RAW("SD card information! \r\n");
			CZ_RAW("CardCapacity  : %llu \r\n", (unsigned long long)hsd.SdCard.BlockSize * hsd.SdCard.BlockNbr);// 显示容量
			CZ_RAW("CardBlockSize : %d \r\n", hsd.SdCard.BlockSize);   // 块大小
			CZ_RAW("LogBlockNbr   : %d \r\n", hsd.SdCard.LogBlockNbr);	// 逻辑块数量
			CZ_RAW("LogBlockSize  : %d \r\n", hsd.SdCard.LogBlockSize);// 逻辑块大小
			CZ_RAW("RCA           : %d \r\n", hsd.SdCard.RelCardAdd);  // 卡相对地址
			CZ_RAW("CardType      : %d \r\n", hsd.SdCard.CardType);    // 卡类型
			// 读取并打印SD卡的CID信息
			HAL_SD_CardCIDTypeDef sdcard_cid;
			HAL_SD_GetCardCID(&hsd,&sdcard_cid);
			CZ_RAW(" ManufacturerID: %d \r\n",sdcard_cid.ManufacturerID);
		} else {
			CZ_RAW("SD card init fail!\r\n" );
		}

		/* 擦除SD卡块 */
		CZ_RAW("------------------- Block Erase -------------------------------\r\n");
		if(HAL_SD_Erase(&hsd, BLOCK_START_ADDR, NUM_OF_BLOCKS) == HAL_OK) {
			/* Wait until SD cards are ready to use for new operation */
			while(HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER);
			CZ_RAW("\r\nErase Block Success!\r\n");
		} else {
			CZ_RAW("\r\nErase Block Failed!\r\n");
		}

		IWDG_Feed();
		LED_TOGGLE;
		osDelay(1000);
	}
  /* USER CODE END StartTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

