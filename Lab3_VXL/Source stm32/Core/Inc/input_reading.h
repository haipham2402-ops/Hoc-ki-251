/*
 * input_reading.h
 *
 *  Created on: Nov 5, 2025
 *      Author: kanek
 */

#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_

#include "main.h"
#include <stdint.h>   // để dùng uint8_t, uint16_t

// ======= Cấu hình số nút nhấn =======
#define N0_OF_BUTTONS 3  // Số lượng nút nhấn thực tế (BTN1, BTN2, BTN3)

// ======= Cấu hình thời gian giữ nút =======
// Gọi button_reading() mỗi 10ms → 100 lần * 10ms = 1s
#define DURATION_FOR_AUTO_INCREASING 100

// ======= Logic mức điện của nút =======
// Tùy vào mạch: nếu nhấn = 0V thì dùng GPIO_PIN_RESET
#define BUTTON_IS_PRESSED     GPIO_PIN_RESET
#define BUTTON_IS_RELEASED    GPIO_PIN_SET

// ======= Prototype các hàm =======

// Hàm đọc trạng thái nút, gọi mỗi 10ms trong timer interrupt
void button_reading(void);

// Kiểm tra nút có được nhấn (1 lần, sau chống dội)
unsigned char button_is_pressed(uint8_t index);

// Kiểm tra nút có được giữ hơn 1 giây không
unsigned char button_is_pressed_1s(uint8_t index);

#endif /* INC_INPUT_READING_H_ */
