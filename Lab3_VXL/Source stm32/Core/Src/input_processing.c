/*
 * input_processing.c
 *
 *  Created on: Nov 5, 2025
 *      Author: kanek
 */
#include "main.h"
#include "input_reading.h"
#include "led_display.h"
#include <stdlib.h>
// =============================
// Biến toàn cục
// =============================
int mode = 0;
int edit_phase = 0;      // 0=xanh, 1=vàng, 2=đỏ
int trafficTimeTemp[2][3];

// =============================
// FSM chính xử lý nút nhấn
// =============================
void fsm_for_input_processing(void) {
    switch (mode) {
    // -------------------- MODE 0 --------------------
    case 0: // Chế độ hoạt động bình thường
        if (button_is_pressed(0)) {  // BTN1: chuyển sang chế độ chỉnh
            mode = 1;
            edit_phase = 0;
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 3; j++)
                    trafficTimeTemp[i][j] = trafficTimeBackup[i][j];
            mode_digit = trafficTimeBackup[0][0];
        }
        break;

    // -------------------- MODE 1 2 3 --------------------
    case 1: // Chế độ chỉnh
        // BTN2: Tăng thời gian
        if (button_is_pressed(1)) {  // Nhấn ngắn → tăng 1
            trafficTimeTemp[0][edit_phase]++;
            mode_digit++;
            if(mode_digit > 99) mode_digit = 5;
        }

        if (button_is_pressed_1s(1)) {  // Giữ ≥ 1s → tăng 5 mỗi 1s
            trafficTimeTemp[0][edit_phase] += 5;
            mode_digit += 5;
            if(mode_digit > 99) mode_digit = 5;
        }

        // Giới hạn tối đa để không bị tràn
        if (trafficTimeTemp[0][edit_phase] > 99) {
            trafficTimeTemp[0][edit_phase] = 5;
        }

        // BTN1: Chuyển pha chỉnh
        if (button_is_pressed(0)) {
            edit_phase++;
            if (edit_phase > 2) {
                edit_phase = -1; // quay lại pha đỏ
                mode = 0;
            }
            if(edit_phase == -1) mode_digit = 0;
            else
            mode_digit = trafficTimeBackup[0][edit_phase];

        }

        if (button_is_pressed(2)) {
        	if(edit_phase == 0)
        		trafficTimeTemp[1][edit_phase] += trafficTimeTemp[0][edit_phase] - trafficTimeBackup[0][edit_phase];
        	if(edit_phase == 1)
        		trafficTimeTemp[1][2] += trafficTimeTemp[0][edit_phase] - trafficTimeBackup[0][edit_phase];
        	if(edit_phase == 2)
        		trafficTimeTemp[1][1] = trafficTimeTemp[0][edit_phase] - trafficTimeBackup[0][edit_phase];
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 3; j++)
                    trafficTimeBackup[i][j] = trafficTimeTemp[i][j];
        }
        break;
    }
}
