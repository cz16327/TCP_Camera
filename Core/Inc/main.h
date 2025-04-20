/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "public.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TEST_LED_Pin GPIO_PIN_13
#define TEST_LED_GPIO_Port GPIOC
#define ADC_VBT_Pin GPIO_PIN_0
#define ADC_VBT_GPIO_Port GPIOA
#define USB_PHY_RESET_Pin GPIO_PIN_1
#define USB_PHY_RESET_GPIO_Port GPIOA
#define CHRGOFF_Pin GPIO_PIN_2
#define CHRGOFF_GPIO_Port GPIOA
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define BUTTON1_Pin GPIO_PIN_10
#define BUTTON1_GPIO_Port GPIOD
#define BUTTON1_EXTI_IRQn EXTI15_10_IRQn
#define BUTTON2_Pin GPIO_PIN_11
#define BUTTON2_GPIO_Port GPIOD
#define BUTTON2_EXTI_IRQn EXTI15_10_IRQn
#define CAM_IO_Pin GPIO_PIN_12
#define CAM_IO_GPIO_Port GPIOD
#define CAM_PWR_EN_Pin GPIO_PIN_13
#define CAM_PWR_EN_GPIO_Port GPIOD
#define SD_PWR_EN_Pin GPIO_PIN_14
#define SD_PWR_EN_GPIO_Port GPIOD
#define WIFI_PWR_EN_Pin GPIO_PIN_15
#define WIFI_PWR_EN_GPIO_Port GPIOD
#define USB_MSO_Pin GPIO_PIN_8
#define USB_MSO_GPIO_Port GPIOA
#define USB_ID_Pin GPIO_PIN_15
#define USB_ID_GPIO_Port GPIOA
#define SD_BSP_Pin GPIO_PIN_0
#define SD_BSP_GPIO_Port GPIOD
#define WIFI_EN_Pin GPIO_PIN_1
#define WIFI_EN_GPIO_Port GPIOD
#define SD_CD_Pin GPIO_PIN_3
#define SD_CD_GPIO_Port GPIOD
#define ADC_VBT_EN_Pin GPIO_PIN_4
#define ADC_VBT_EN_GPIO_Port GPIOD
#define CAM_RST_Pin GPIO_PIN_7
#define CAM_RST_GPIO_Port GPIOD
#define PWM0_Pin GPIO_PIN_4
#define PWM0_GPIO_Port GPIOB
#define SPI1_CS_Pin GPIO_PIN_8
#define SPI1_CS_GPIO_Port GPIOB
#define PWM1_Pin GPIO_PIN_9
#define PWM1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#ifdef CM_BACKTRACE
#define APPNAME                        "TCP_CAM"
#define HARDWARE_VERSION               "V1.0.0"
#define SOFTWARE_VERSION               "V1.0.0"
#endif
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
