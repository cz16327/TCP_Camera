/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.h
  * @brief   This file contains all the function prototypes for
  *          the rtc.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN Private defines */
#define START_YEAR	(20) //时间->sec, 计算起始年份，闰年

#define ArrayLength(Array) (sizeof(Array) / sizeof(Array[0]))

#define RTC_YEAR1_DEF	(20)
#define RTC_YEAR2_DEF	(24)
#define RTC_MONTH_DEF	(1)
#define RTC_DAY_DEF		(1)
#define RTC_HOUR_DEF	(12)
#define RTC_MIN_DEF		(0)
#define RTC_SEC_DEF		(0)
/* USER CODE END Private defines */

void MX_RTC_Init(void);

/* USER CODE BEGIN Prototypes */
void rtc_time_set(unsigned char* time);
void rtc_time_get(unsigned char* time);
unsigned int rtc_time_to_seconds(unsigned char* time);
void RTCEx_BKUPWrite(unsigned int BackupRegister, unsigned int Data);
unsigned int RTCEx_BKUPRead(unsigned int BackupRegister);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __RTC_H__ */

