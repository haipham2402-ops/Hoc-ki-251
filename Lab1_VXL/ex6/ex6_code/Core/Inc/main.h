/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define a1_Pin GPIO_PIN_4
#define a1_GPIO_Port GPIOA
#define a2_Pin GPIO_PIN_5
#define a2_GPIO_Port GPIOA
#define a3_Pin GPIO_PIN_6
#define a3_GPIO_Port GPIOA
#define a4_Pin GPIO_PIN_7
#define a4_GPIO_Port GPIOA
#define a5_Pin GPIO_PIN_8
#define a5_GPIO_Port GPIOA
#define a6_Pin GPIO_PIN_9
#define a6_GPIO_Port GPIOA
#define a7_Pin GPIO_PIN_10
#define a7_GPIO_Port GPIOA
#define a8_Pin GPIO_PIN_11
#define a8_GPIO_Port GPIOA
#define a9_Pin GPIO_PIN_12
#define a9_GPIO_Port GPIOA
#define a10_Pin GPIO_PIN_13
#define a10_GPIO_Port GPIOA
#define a11_Pin GPIO_PIN_14
#define a11_GPIO_Port GPIOA
#define a12_Pin GPIO_PIN_15
#define a12_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
