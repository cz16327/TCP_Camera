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
#include "spi.h"
#include "iwdg.h"
#include "wifi_at.h"
#include "AT24C02.h"
#include "fatfs.h"
#include "sdio.h"
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
uint8_t Buffer_Tx[512], Buffer_Rx[512] = { 0 };
uint32_t i;
/* USER CODE END Variables */
/* Definitions for initTask */
osThreadId_t initTaskHandle;
const osThreadAttr_t initTask_attributes = {
  .name = "initTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh7,
};
/* Definitions for sdTask */
osThreadId_t sdTaskHandle;
const osThreadAttr_t sdTask_attributes = {
  .name = "sdTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for liveTask */
osThreadId_t liveTaskHandle;
const osThreadAttr_t liveTask_attributes = {
  .name = "liveTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for adcTask */
osThreadId_t adcTaskHandle;
const osThreadAttr_t adcTask_attributes = {
  .name = "adcTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for atAck_semaphores */
osSemaphoreId_t atAck_semaphoresHandle;
const osSemaphoreAttr_t atAck_semaphores_attributes = {
  .name = "atAck_semaphores"
};
/* Definitions for adcSwitch_semaphores */
osSemaphoreId_t adcSwitch_semaphoresHandle;
const osSemaphoreAttr_t adcSwitch_semaphores_attributes = {
  .name = "adcSwitch_semaphores"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/// @brief 打印任务信息
/// @param  void
void dump_task_info(void)
{
	char InfoBuffer[512] = { 0 };
	vPortEnterCritical();
	vTaskList((char*)&InfoBuffer);
	CZ_RAW("task          state  priority  stack   NO.\r\n");
	CZ_RAW("\r\n%s\r\n", InfoBuffer);
	vPortExitCritical();
}


osSemaphoreId_t get_atAck_semap(void)
{
	return atAck_semaphoresHandle;
}

osSemaphoreId_t get_adcSwitch_semap(void)
{
	return adcSwitch_semaphoresHandle;
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

void InitTask(void *argument);
void SDcardTask(void *argument);
void LiveTask(void *argument);
void AdcTask(void *argument);

extern void MX_USB_DEVICE_Init(void);
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

  /* creation of adcSwitch_semaphores */
  adcSwitch_semaphoresHandle = osSemaphoreNew(1, 0, &adcSwitch_semaphores_attributes);

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
  /* creation of initTask */
  initTaskHandle = osThreadNew(InitTask, NULL, &initTask_attributes);

  /* creation of sdTask */
  sdTaskHandle = osThreadNew(SDcardTask, NULL, &sdTask_attributes);

  /* creation of liveTask */
  liveTaskHandle = osThreadNew(LiveTask, NULL, &liveTask_attributes);

  /* creation of adcTask */
  adcTaskHandle = osThreadNew(AdcTask, NULL, &adcTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
			  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
			  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_InitTask */
/**
  * @brief  Function implementing the initTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_InitTask */
void InitTask(void *argument)
{
  /* init code for USB_DEVICE */
//   MX_USB_DEVICE_Init();
  /* USER CODE BEGIN InitTask */
		/* Infinite loop */
	for (;;) {
		delay_init();
		memoryPool_init();
		cz_log_set_level(LOG_LEVEL_DBG);
		SD_PWRON;
		SD_Driver.disk_initialize(0);
		CZ_WARN("Init End. Delete InitTask Self\r\n");
		vTaskDelete(NULL);
		osDelay(100);
	}
  /* USER CODE END InitTask */
}

/* USER CODE BEGIN Header_SDcardTask */
/**
* @brief Function implementing the sdTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SDcardTask */
void SDcardTask(void *argument)
{
  /* USER CODE BEGIN SDcardTask */
	// MX_USB_DEVICE_Init();
	IWDG_Feed();
	/* Infinite loop */
	for (;;) {
		CZ_LOG("SD Card Process in\r\n");
		// Fatfs_RW_test();
		// SDCardInfo();
		CZ_LOG("SD Card Process end\r\n");
		IWDG_Feed();
		osDelay(10000);
	}
  /* USER CODE END SDcardTask */
}

/* USER CODE BEGIN Header_LiveTask */
/**
* @brief Function implementing the liveTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LiveTask */
void LiveTask(void *argument)
{
  /* USER CODE BEGIN LiveTask */
	dump_task_info();
	WIFI_EN;
	WIFI_PWRON;
	CAM_EN;
	CAM_PWRON;
	unsigned char* wifi_ssid = "potato";
	unsigned char* wifi_pwd = "erbailailaixiguatudou20160327.";
	AT_t* AT = get_AT_t();
	AT->Basic.at_rst(Excution, 0); // 重启模块
	delay_ms(1000);
	IWDG_Feed();
	AT->Wifi.Basic.at_wMode(Set, STA_MODE, SAVE, 0); // 设置模块的 Wi-Fi 工作模式并保存到 Flash
	delay_ms(2000);
	IWDG_Feed();
	AT->Wifi.Basic.at_wJap(Set, wifi_ssid, wifi_pwd, NULL, 0); // 连接目标路由器
	delay_ms(3000);
	CZ_RAW("CD BSP %d\r\n", HAL_GPIO_ReadPin(SD_BSP_GPIO_Port, SD_BSP_Pin));
	// AT->Wifi.Basic.at_wAutoConn(Set, BW_ENABLE, wifi_ssid, wifi_pwd, NULL, 0);
	/* Infinite loop */
	// unsigned int i = 0;
	// Ret_Status_e ii;
	// Ret_Status_e iii;
	// unsigned char data[30] = {0};
	// unsigned char get_data[30] = {0};
	IWDG_Feed();
	const unsigned char start_cmd = 0x11;
	const unsigned char stop_cmd = 0x22;
	unsigned char rx_start = 0;
	unsigned char rx_stop = 0;
	unsigned int cnt = 0;
	/* Infinite loop */
	for (;;) {
		// i++;
		// sprintf((char *)data, "hello AT24C02 %d !\r\n", i);
		// ii = at24c02_write(0, data, sizeof(data));
		// osDelay(10);
		// iii = at24c02_read(0, get_data, sizeof(get_data));
		// CZ_RAW("%s [%d %d]\r\n", get_data, ii, iii);
		IWDG_Feed();
		LED_TOGGLE;
		if (cnt == 10) {
			CZ_LOG("start %d\r\n", spi_TxRx(&hspi1, &start_cmd, 1, &rx_start, 0, 1000));
			if (rx_start == start_cmd) {
				CZ_LOG("start OK\r\n");
				rx_start = 0;
			}
		} else if (cnt >= 20) {
			CZ_LOG("stop %d\r\n", spi_TxRx(&hspi1, &stop_cmd, 1, &rx_stop, 0, 1000));
			if (rx_stop == stop_cmd) {
				CZ_LOG("stop OK\r\n");
				rx_stop = 0;
			}
			cnt = 0;
		}
		cnt++;
		osDelay(500);
	}
  /* USER CODE END LiveTask */
}

/* USER CODE BEGIN Header_AdcTask */
/**
* @brief Function implementing the adcTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AdcTask */
void AdcTask(void *argument)
{
  /* USER CODE BEGIN AdcTask */
	osSemaphoreId_t adcSwitch_Semap = get_adcSwitch_semap();
	/* Infinite loop */
	for (;;) {
		if (osOK == osSemaphoreAcquire(adcSwitch_Semap, 0))
			tim_vbat_adc_enable(CZ_ENABLE);
		osDelay(1);
	}
  /* USER CODE END AdcTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

