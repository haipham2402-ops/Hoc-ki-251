/*
 * global.c
 *
 *  Created on: Nov 12, 2025
 *      Author: kanek
 */
#include "global.h"

void Led_Init(void){
	HAL_GPIO_WritePin(Red_main_GPIO_Port, Red_main_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Yellow_main_GPIO_Port, Yellow_main_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Green_main_GPIO_Port, Green_main_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(Red_sub_GPIO_Port, Red_sub_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Yellow_sub_GPIO_Port, Yellow_sub_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Green_sub_GPIO_Port, Green_sub_Pin, GPIO_PIN_SET);

	// Tắt 7-segment khác
	HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET);

	// Reset các chân 7-segment
	HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
}

