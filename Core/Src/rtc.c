/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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
#include "rtc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 12;
  sTime.Minutes = 30;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_TUESDAY;
  sDate.Month = RTC_MONTH_APRIL;
  sDate.Date = 8;
  sDate.Year = 25;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the WakeUp
  */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
// year % 400 = 0 为闰年，year % 100 != 0 且 year % 4 = 0 为闰年
uint16_t YEAR_TABLE[] = {
	366 * 1 + 365 * 0,		//2020, 闰-366
	366 * 1 + 365 * 1,		//2021, 平-365
	366 * 1 + 365 * 2,		//2022, 平-365
	366 * 1 + 365 * 3,		//2023, 平-365
	366 * 2 + 365 * 3,		//2024, 闰-366
	366 * 2 + 365 * 4,		//2025, 平-365
	366 * 2 + 365 * 5,		//2026, 平-365
	366 * 2 + 365 * 6,		//2027, 平-365
	366 * 3 + 365 * 6,		//2028, 闰-366
	366 * 3 + 365 * 7,		//2029, 平-365
	366 * 3 + 365 * 8,		//2030, 平-365
	366 * 3 + 365 * 9,		//2031, 平-365
	366 * 4 + 365 * 9,		//2032, 闰-366
	366 * 4 + 365 * 10,		//2033, 平-365
	366 * 4 + 365 * 11,		//2034, 平-365
	366 * 4 + 365 * 12,		//2035, 平-365
	366 * 5 + 365 * 12,		//2036, 闰-366
	366 * 5 + 365 * 13,		//2037, 平-365
	366 * 5 + 365 * 14,		//2038, 平-365
	366 * 5 + 365 * 15,		//2039, 平-365
	366 * 6 + 365 * 15,		//2040, 闰-366
	366 * 6 + 365 * 16,		//2041, 平-365
	366 * 6 + 365 * 17,		//2042, 平-365
	366 * 6 + 365 * 18,		//2043, 平-365
	366 * 7 + 365 * 18,		//2044, 闰-366
	366 * 7 + 365 * 19,		//2045, 平-365
	366 * 7 + 365 * 20,		//2046, 平-365
	366 * 7 + 365 * 21,		//2047, 平-365
	366 * 8 + 365 * 21,		//2048, 闰-366
	366 * 8 + 365 * 22,		//2049, 平-365
	366 * 8 + 365 * 23 };	//2050, 平-365

//平年每个月多少天
uint16_t DAY_TABLE1[] = {
	31,
	31 + 28,
	31 + 28 + 31,
	31 + 28 + 31 + 30,
	31 + 28 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30,
	31 + 28 + 31 + 30 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31 };

//闰年每个月多少天
uint16_t DAY_TABLE2[] = {
	31,
	31 + 29,
	31 + 29 + 31,
	31 + 29 + 31 + 30,
	31 + 29 + 31 + 30 + 31,
	31 + 29 + 31 + 30 + 31 + 30,
	31 + 29 + 31 + 30 + 31 + 30 + 31,
	31 + 29 + 31 + 30 + 31 + 30 + 31 + 31,
	31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
	31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
	31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
	31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31 };


/// @brief 设置时间
/// @param time 时间
void rtc_time_set(unsigned char* time)
{
	RTC_TimeTypeDef time_s; //time
	RTC_DateTypeDef date_s; //date

	date_s.WeekDay = 0; //这里必须要设置星期，否则读取年份不对
	date_s.Year = time[1];
	date_s.Month = time[2];
	date_s.Date = time[3];
	time_s.Hours = time[4];
	time_s.Minutes = time[5];
	time_s.Seconds = time[6];
	time_s.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	time_s.StoreOperation = RTC_STOREOPERATION_RESET;

	HAL_RTC_SetDate(&hrtc, &date_s, RTC_FORMAT_BIN);
	HAL_RTC_SetTime(&hrtc, &time_s, RTC_FORMAT_BIN);
}


/// @brief 获取时间
/// @param time 存储
void rtc_time_get(unsigned char* time)
{
	RTC_TimeTypeDef time_s; //time
	RTC_DateTypeDef date_s; //date

	/* Get the RTC current Time */
	HAL_RTC_GetTime(&hrtc, &time_s, RTC_FORMAT_BIN);
	/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &date_s, RTC_FORMAT_BIN);

	time[0] = 20;
	time[1] = date_s.Year;
	time[2] = date_s.Month;
	time[3] = date_s.Date;
	time[4] = time_s.Hours;
	time[5] = time_s.Minutes;
	time[6] = time_s.Seconds;
}


/// @brief 时间转换为秒
/// @param time 当前时间
/// @return 秒
unsigned int rtc_time_to_seconds(unsigned char* time)
{
	char i;
	char YEAR = time[1];
	char MONTH = time[2];
	char DAY = time[3];
	char HOUR = time[4];
	char MINUTE = time[5];
	char SECOND = time[6];
	uint32_t SwitchSec = 0;

	SwitchSec = SECOND;
	SwitchSec += (uint32_t)MINUTE * 60;
	SwitchSec += (uint32_t)HOUR * 3600;

	for (i = 0; i < ArrayLength(YEAR_TABLE); i++) {
		if ((YEAR - START_YEAR) <= i) {
			break;
		}
	}

	if (i > 0) {
		SwitchSec += (uint32_t)YEAR_TABLE[i - 1] * 86400L;
	}

	if (YEAR / 4 * 4 == YEAR) {
		for (i = 1; i < 13; i++) {
			if (MONTH <= i) {
				break;
			}
		}

		if (i != 1) {
			SwitchSec += (uint32_t)DAY_TABLE2[i - 2] * 86400L;
		}

		SwitchSec += (uint32_t)(DAY - 1) * 86400L;
	} else {
		for (i = 1;i < 13;i++) {
			if (MONTH <= i) {
				break;
			}
		}

		if (i != 1) {
			SwitchSec += (uint32_t)DAY_TABLE1[i - 2] * 86400L;
		}
		SwitchSec += (uint32_t)(DAY - 1) * 86400L;
	}

	return  SwitchSec;
}


/// @brief 向备份寄存器写入数据
/// @param BackupRegister 寄存器
/// @param Data 数据
void RTCEx_BKUPWrite(unsigned int BackupRegister, unsigned int Data)
{
	HAL_RTCEx_BKUPWrite(&hrtc, BackupRegister, Data);
}


/// @brief 从备份寄存器读数据
/// @param BackupRegister 寄存器
/// @return 
unsigned int RTCEx_BKUPRead(unsigned int BackupRegister)
{
	return HAL_RTCEx_BKUPRead(&hrtc, BackupRegister);
}

/* USER CODE END 1 */
