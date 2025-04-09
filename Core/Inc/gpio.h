/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
// LED 控制
#define LED_ON			HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, GPIO_PIN_SET)
#define LED_OFF			HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, GPIO_PIN_RESET)
#define LED_TOGGLE		HAL_GPIO_TogglePin(TEST_LED_GPIO_Port, TEST_LED_Pin)
// USB PHY 使失能
#define USB_EN			HAL_GPIO_WritePin(USB_PHY_RESET_GPIO_Port, USB_PHY_RESET_Pin, GPIO_PIN_SET)
#define USB_DIS			HAL_GPIO_WritePin(USB_PHY_RESET_GPIO_Port, USB_PHY_RESET_Pin, GPIO_PIN_RESET)
// USB OTG 角色
#define USB_HOST		HAL_GPIO_WritePin(USB_PHY_RESET_GPIO_Port, USB_PHY_RESET_Pin, GPIO_PIN_RESET)
#define USB_DEVICE		HAL_GPIO_WritePin(USB_PHY_RESET_GPIO_Port, USB_PHY_RESET_Pin, GPIO_PIN_SET)
// CAM 开关电
#define CAM_PWRON		HAL_GPIO_WritePin(CAM_PWR_EN_GPIO_Port, CAM_PWR_EN_Pin, GPIO_PIN_SET)
#define CAM_PWROFF		HAL_GPIO_WritePin(CAM_PWR_EN_GPIO_Port, CAM_PWR_EN_Pin, GPIO_PIN_RESET)
// CAM 使失能
#define CAM_EN			HAL_GPIO_WritePin(CAM_RST_GPIO_Port, CAM_RST_Pin, GPIO_PIN_SET)
#define CAM_DIS			HAL_GPIO_WritePin(CAM_RST_GPIO_Port, CAM_RST_Pin, GPIO_PIN_RESET)
// SD 开关电
#define SD_PWRON		HAL_GPIO_WritePin(SD_PWR_EN_GPIO_Port, SD_PWR_EN_Pin, GPIO_PIN_SET)
#define SD_PWROFF		HAL_GPIO_WritePin(SD_PWR_EN_GPIO_Port, SD_PWR_EN_Pin, GPIO_PIN_RESET)
// SD 检查
#define SD_CHECK		HAL_GPIO_ReadPin(SIDO_CD_GPIO_Port, SIDO_CD_Pin)
// WIFI 开关电
#define WIFI_PWRON		HAL_GPIO_WritePin(WIFI_PWR_EN_GPIO_Port, WIFI_PWR_EN_Pin, GPIO_PIN_SET)
#define WIFI_PWROFF		HAL_GPIO_WritePin(WIFI_PWR_EN_GPIO_Port, WIFI_PWR_EN_Pin, GPIO_PIN_RESET)
// WIFI 使失能
#define WIFI_EN			HAL_GPIO_WritePin(WIFI_EN_GPIO_Port, WIFI_EN_Pin, GPIO_PIN_SET)
#define WIFI_DIS		HAL_GPIO_WritePin(WIFI_EN_GPIO_Port, WIFI_EN_Pin, GPIO_PIN_RESET)
// VBAT 使失能采集
#define VBAT_ADC_EN		HAL_GPIO_WritePin(ADC_VBT_EN_GPIO_Port, ADC_VBT_EN_Pin, GPIO_PIN_SET)
#define VBAT_ADC_DIS	HAL_GPIO_WritePin(ADC_VBT_EN_GPIO_Port, ADC_VBT_EN_Pin, GPIO_PIN_RESET)
// SPI CS 控制
#define SPI_CSL_EN		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET)
#define SPI_CSH_DIS		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET)
// 按钮判断
#define BOTTON1_GET		HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin)
#define BOTTON2_GET		HAL_GPIO_ReadPin(BUTTON2_GPIO_Port, BUTTON2_Pin)
/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

