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
#define Led_red_Pin GPIO_PIN_5
#define Led_red_GPIO_Port GPIOA
#define Led_yellow_Pin GPIO_PIN_6
#define Led_yellow_GPIO_Port GPIOA
#define Led_green_Pin GPIO_PIN_7
#define Led_green_GPIO_Port GPIOA
#define a_Pin GPIO_PIN_0
#define a_GPIO_Port GPIOB
#define b_Pin GPIO_PIN_1
#define b_GPIO_Port GPIOB
#define c_Pin GPIO_PIN_2
#define c_GPIO_Port GPIOB
#define bb_Pin GPIO_PIN_10
#define bb_GPIO_Port GPIOB
#define cc_Pin GPIO_PIN_11
#define cc_GPIO_Port GPIOB
#define dd_Pin GPIO_PIN_12
#define dd_GPIO_Port GPIOB
#define ee_Pin GPIO_PIN_13
#define ee_GPIO_Port GPIOB
#define ff_Pin GPIO_PIN_14
#define ff_GPIO_Port GPIOB
#define gg_Pin GPIO_PIN_15
#define gg_GPIO_Port GPIOB
#define Led_red2_Pin GPIO_PIN_8
#define Led_red2_GPIO_Port GPIOA
#define Led_yellow2_Pin GPIO_PIN_9
#define Led_yellow2_GPIO_Port GPIOA
#define Led_green2_Pin GPIO_PIN_10
#define Led_green2_GPIO_Port GPIOA
#define d_Pin GPIO_PIN_3
#define d_GPIO_Port GPIOB
#define e_Pin GPIO_PIN_4
#define e_GPIO_Port GPIOB
#define f_Pin GPIO_PIN_5
#define f_GPIO_Port GPIOB
#define g_Pin GPIO_PIN_6
#define g_GPIO_Port GPIOB
#define aa_Pin GPIO_PIN_9
#define aa_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
