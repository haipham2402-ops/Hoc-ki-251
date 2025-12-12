/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
void display7SEG(uint8_t num)
{
    // Tắt hết trước
    HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_SET);

    switch(num)
        {
            case 0:
                HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_SET);
                break;

            case 1:
                HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
                break;

            case 2:
                HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
                break;

            case 3:
                HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
                break;

            case 4:
                HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
                break;


            default:
                // Hiển thị gạch ngang "-"
                HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
                break;
        }
}
void display7SEG2(uint8_t num)
{
    // Tắt hết trước
    HAL_GPIO_WritePin(aa_GPIO_Port, aa_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(bb_GPIO_Port, bb_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(cc_GPIO_Port, cc_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(dd_GPIO_Port, dd_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ee_GPIO_Port, ee_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ff_GPIO_Port, ff_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(gg_GPIO_Port, gg_Pin, GPIO_PIN_SET);

    switch(num)
    {
        case 0:
            HAL_GPIO_WritePin(aa_GPIO_Port, aa_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(bb_GPIO_Port, bb_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(cc_GPIO_Port, cc_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(dd_GPIO_Port, dd_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(ee_GPIO_Port, ee_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(ff_GPIO_Port, ff_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(gg_GPIO_Port, gg_Pin, GPIO_PIN_SET);
            break;

        case 1:
            HAL_GPIO_WritePin(bb_GPIO_Port, bb_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(cc_GPIO_Port, cc_Pin, GPIO_PIN_RESET);
            break;

        case 2:
            HAL_GPIO_WritePin(aa_GPIO_Port, aa_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(bb_GPIO_Port, bb_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(dd_GPIO_Port, dd_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(ee_GPIO_Port, ee_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(gg_GPIO_Port, gg_Pin, GPIO_PIN_RESET);
            break;

        case 3:
            HAL_GPIO_WritePin(aa_GPIO_Port, aa_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(bb_GPIO_Port, bb_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(cc_GPIO_Port, cc_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(dd_GPIO_Port, dd_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(gg_GPIO_Port, gg_Pin, GPIO_PIN_RESET);
            break;

        case 4:
            HAL_GPIO_WritePin(bb_GPIO_Port, bb_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(cc_GPIO_Port, cc_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(ff_GPIO_Port, ff_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(gg_GPIO_Port, gg_Pin, GPIO_PIN_RESET);
            break;

        default:
            // Hiển thị gạch ngang "-"
            HAL_GPIO_WritePin(gg_GPIO_Port, gg_Pin, GPIO_PIN_RESET);
            break;
    }
}
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
    {
    /* USER CODE END WHILE */
	  while (1)
	    {
	      /* USER CODE END WHILE */

	  	  // RED ON (5s)
	  	     HAL_GPIO_WritePin(Led_red_GPIO_Port, Led_red_Pin, GPIO_PIN_RESET);
	  	     HAL_GPIO_WritePin(Led_yellow_GPIO_Port, Led_yellow_Pin, GPIO_PIN_SET);
	  	     HAL_GPIO_WritePin(Led_green_GPIO_Port, Led_green_Pin, GPIO_PIN_SET);

	  	     HAL_GPIO_WritePin(Led_red2_GPIO_Port, Led_red2_Pin, GPIO_PIN_SET);
	  	     HAL_GPIO_WritePin(Led_yellow2_GPIO_Port, Led_yellow2_Pin, GPIO_PIN_SET);
	  	     HAL_GPIO_WritePin(Led_green2_GPIO_Port, Led_green2_Pin, GPIO_PIN_RESET);
	  	   display7SEG(4);
	  	   display7SEG2(2);
	  	     HAL_Delay(1000);
	  	 display7SEG(3);
	  	display7SEG2(1);
	  	   HAL_Delay(1000);
	  	display7SEG(2);
	  	display7SEG2(0);
	  	 HAL_Delay(1000);


	  	     HAL_GPIO_WritePin(Led_red_GPIO_Port, Led_red_Pin, GPIO_PIN_RESET);
	  		 HAL_GPIO_WritePin(Led_yellow_GPIO_Port, Led_yellow_Pin, GPIO_PIN_SET);
	  		 HAL_GPIO_WritePin(Led_green_GPIO_Port, Led_green_Pin, GPIO_PIN_SET);

	  	     HAL_GPIO_WritePin(Led_red2_GPIO_Port, Led_red2_Pin, GPIO_PIN_SET);
	  	     HAL_GPIO_WritePin(Led_yellow2_GPIO_Port, Led_yellow2_Pin, GPIO_PIN_RESET);
	  	     HAL_GPIO_WritePin(Led_green2_GPIO_Port, Led_green2_Pin, GPIO_PIN_SET);
	  	   display7SEG(1);
		  	display7SEG2(1);
	  		 HAL_Delay(1000);
	  	 display7SEG(0);
		  	display7SEG2(0);
	  	 HAL_Delay(1000);

	  	     // GREEN ON (3s)
	  	     HAL_GPIO_WritePin(Led_red_GPIO_Port, Led_red_Pin, GPIO_PIN_SET);
	  	     HAL_GPIO_WritePin(Led_yellow_GPIO_Port, Led_yellow_Pin, GPIO_PIN_SET);
	  	     HAL_GPIO_WritePin(Led_green_GPIO_Port, Led_green_Pin, GPIO_PIN_RESET);

	  	     HAL_GPIO_WritePin(Led_red2_GPIO_Port, Led_red2_Pin, GPIO_PIN_RESET);
	  	     HAL_GPIO_WritePin(Led_yellow2_GPIO_Port, Led_yellow2_Pin, GPIO_PIN_SET);
	  	     HAL_GPIO_WritePin(Led_green2_GPIO_Port, Led_green2_Pin, GPIO_PIN_SET);
	  	   display7SEG(2);
		  	display7SEG2(4);
	  	     HAL_Delay(1000);
		  	   display7SEG(1);
			  	display7SEG2(3);
		  	     HAL_Delay(1000);
		  	   display7SEG(0);
			  	display7SEG2(2);
		  	     HAL_Delay(1000);

	  	  // YELLOW ON (2s)

	  	     HAL_GPIO_WritePin(Led_red_GPIO_Port, Led_red_Pin, GPIO_PIN_SET);
	  	     HAL_GPIO_WritePin(Led_yellow_GPIO_Port, Led_yellow_Pin, GPIO_PIN_RESET);
	  	     HAL_GPIO_WritePin(Led_green_GPIO_Port, Led_green_Pin, GPIO_PIN_SET);

	  	     HAL_GPIO_WritePin(Led_red2_GPIO_Port, Led_red2_Pin, GPIO_PIN_RESET);
	  	     HAL_GPIO_WritePin(Led_yellow2_GPIO_Port, Led_yellow2_Pin, GPIO_PIN_SET);
	  	     HAL_GPIO_WritePin(Led_green2_GPIO_Port, Led_green2_Pin, GPIO_PIN_SET);
			  	   display7SEG(1);
				  	display7SEG2(1);
			  	     HAL_Delay(1000);
			  	   display7SEG(0);
				  	display7SEG2(0);
	  	     HAL_Delay(1000);



	      /* USER CODE BEGIN 3 */
	    }

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Led_red_Pin|Led_yellow_Pin|Led_green_Pin|Led_red2_Pin
                          |Led_yellow2_Pin|Led_green2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|bb_Pin
                          |cc_Pin|dd_Pin|ee_Pin|ff_Pin
                          |gg_Pin|d_Pin|e_Pin|f_Pin
                          |g_Pin|aa_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Led_red_Pin Led_yellow_Pin Led_green_Pin Led_red2_Pin
                           Led_yellow2_Pin Led_green2_Pin */
  GPIO_InitStruct.Pin = Led_red_Pin|Led_yellow_Pin|Led_green_Pin|Led_red2_Pin
                          |Led_yellow2_Pin|Led_green2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : a_Pin b_Pin c_Pin bb_Pin
                           cc_Pin dd_Pin ee_Pin ff_Pin
                           gg_Pin d_Pin e_Pin f_Pin
                           g_Pin aa_Pin */
  GPIO_InitStruct.Pin = a_Pin|b_Pin|c_Pin|bb_Pin
                          |cc_Pin|dd_Pin|ee_Pin|ff_Pin
                          |gg_Pin|d_Pin|e_Pin|f_Pin
                          |g_Pin|aa_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
