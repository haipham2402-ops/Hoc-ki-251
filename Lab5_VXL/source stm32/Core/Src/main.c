#include "main.h"
#include <stdio.h>
#include <string.h>
ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htim2;
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);

#define MAX_BUFFER_SIZE 30
#define NO_CMD   0
#define RST_CMD  1
#define OK_CMD   2
uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t ADC_signal = NO_CMD;
int signal_flag = 0;
int timer0_counter = 0;
int timer0_flag = 0;
int TIMER_CYCLE = 10; // đơn vị: ms (chu kỳ của ngắt timer)
void setTimer0(int duration){
    timer0_counter = duration / TIMER_CYCLE;
    timer0_flag = 0;
}

void timer_run(){
	if(timer0_counter > 0) timer0_counter--;
	else  timer0_flag = 1;
}
char str[30];

// Biến trạng thái cho Command Parser
int status_parser = 0;
void HAL_UART_RxCpltCallback (UART_HandleTypeDef *huart){
	if(huart->Instance == USART2) {
		buffer[index_buffer++] = temp;
		if(index_buffer >= 30) index_buffer = 0;
		signal_flag = 1;
		HAL_UART_Receive_IT (&huart2, &temp , 1);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        timer_run();
    }
}

void command_parser_fsm() {
    int current_index = (index_buffer == 0) ? (MAX_BUFFER_SIZE - 1) : (index_buffer - 1);
    uint8_t data = buffer[current_index];

    switch (status_parser) {
        case 0: // Trạng thái chờ ký tự bắt đầu '!'
            if (data == '!') {
                status_parser = 1;
            }
            break;

        case 1: // Đã có '!', chờ 'R'
            if (data == 'R') status_parser = 2;
            else if (data == 'O') status_parser = 11;
            else status_parser = 0;
            break;

        case 2: // Đã có '!R', chờ 'S'
            if (data == 'S') status_parser = 3;
            else status_parser = 0;
            break;

        case 3: // Đã có '!RS', chờ 'T'
            if (data == 'T') status_parser = 4;
            else status_parser = 0;
            break;

        case 4: // Đã có '!RST', chờ '#'
            if (data == '#') {
                // ---> PHÁT HIỆN LỆNH !RST# THÀNH CÔNG <---
                ADC_signal = RST_CMD;
                signal_flag = 1;
                status_parser = 0; // Reset về đầu để chờ lệnh mới
            }
            else if (data == '!') status_parser = 1;
            else status_parser = 0;
            break;

        //Kiem tra tiep tuc cho !OK#
        case 11:
        	if(data == 'K') status_parser = 12;
        	else status_parser = 0;
        	break;
        case 12:
        	if(data == '#'){
        		ADC_signal = OK_CMD;
                signal_flag = 1;
        	}
        	status_parser = 0;
        	break;
        default:
            status_parser = 0;
            break;
    }
}

enum { IDLE_STATE, WAIT_OK_STATE };

uint32_t uart = IDLE_STATE;
uint32_t ADC_value = 0;
void uart_communication_fsm(){
	switch(uart){
		case IDLE_STATE:
			if(ADC_signal == RST_CMD){
				ADC_signal = NO_CMD;

				HAL_ADC_PollForConversion(&hadc1, 100);
				ADC_value = HAL_ADC_GetValue(&hadc1);

				sprintf(str, "\r\n!ADC=%lu#\r\n", ADC_value);
				HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), 1000);
				HAL_UART_Receive_IT (&huart2, &temp , 1);
				setTimer0(3000);

				uart = WAIT_OK_STATE;
			}
			break;
		case WAIT_OK_STATE:
			if(ADC_signal == OK_CMD){
				ADC_signal = NO_CMD;

				uart = IDLE_STATE;
			}
			if (timer0_flag == 1){
				sprintf(str, "!ADC=%lu#\r\n", ADC_value);
				HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), 1000);
				HAL_UART_Receive_IT (&huart2, &temp , 1);
				setTimer0(3000);
			}
			break;
		default:
			uart = IDLE_STATE;
			break;
	}
}


int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  HAL_UART_Receive_IT(&huart2, &temp, 1);
  HAL_TIM_Base_Start_IT(&htim2);

  HAL_ADC_Start(&hadc1);

  while (1)
  {
      if (signal_flag == 1) {
    	  HAL_GPIO_TogglePin(Led_red_GPIO_Port,Led_red_Pin);
          command_parser_fsm();
          signal_flag = 0;
      }

      uart_communication_fsm();
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_ADC1_Init(void)
{
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  ADC_ChannelConfTypeDef sConfig = {0};
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

static void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Led_red_GPIO_Port, Led_red_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Led_red_Pin */
  GPIO_InitStruct.Pin = Led_red_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Led_red_GPIO_Port, &GPIO_InitStruct);

}

void Error_Handler(void)
{
  __disable_irq();
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line);
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
