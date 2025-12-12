#include "main.h"
#include "led_display.h"

static uint8_t pos7Seg = 0;
static uint8_t pos7Seg_mode = 0;

int trafficTimeBackup[2][3] = {
    {3, 1, 4}, //xanh vàng đỏ
    {4, 3, 1} //đỏ xanh vàng
};

int trafficTime[2][3] = {
	{3, 1, 4},  // main: đỏ, vàng, xanh
	{4, 3, 1}
};

int mode_digit = 0;

static int curpos = 0;   // main
static int curpos2 = 0;  // sub

/* -------------------- Multiplex 7-segment -------------------- */
void TurnOffMultiplex(void) {
    HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
}

void TurnOffMultiplex_mode(void) {
    HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);
}

void DisplayMultiplex(void) {
    TurnOffMultiplex();
    // Đảo EN0 và EN1
    if (pos7Seg == 0) HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET);
    else HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_RESET);

    DisplayDigit(pos7Seg);
    pos7Seg = (pos7Seg + 1) % 2;
}

void DisplayMultiplex_mode(void) {
    TurnOffMultiplex_mode();
    if (pos7Seg_mode == 0) HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET);
    else HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET);

    DisplayDigit_mode(pos7Seg_mode);
    pos7Seg_mode = (pos7Seg_mode + 1) % 2;
}

/* -------------------- Hiển thị 7-segment -------------------- */
void DisplayDigit(int pos) {
    // Tắt tất cả LED
    HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_SET);

    int num = (pos == 0) ? trafficTime[0][curpos] / 10 : trafficTime[0][curpos] % 10;

    switch(num) {
        case 0: HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
                break;
        case 1: HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET); break;
        case 2: HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET); break;
        case 3: HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET); break;
        case 4: HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET); break;
        case 5: HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET); break;
        case 6: HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET); break;
        case 7: HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET); break;
        case 8: HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET); break;
        case 9: HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET); break;
        default: break;
    }
}

void DisplayDigit_mode(int pos) {
    // Tắt tất cả LED
    HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_SET);

    int num;
    if(pos == 0) num = mode_digit % 10;
    if(pos == 1) num = mode_digit / 10;

    switch(num) {
        case 0: HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_RESET); break;
        case 1: HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET); break;
        case 2: HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_RESET); break;
        case 3: HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_RESET); break;
        case 4: HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_RESET); break;
        case 5: HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_RESET); break;
        case 6: HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_RESET); break;
        case 7: HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET); break;
        case 8: HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_RESET); break;
        case 9: HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_RESET); break;
        default: break;
    }
}

/* -------------------- Traffic light logic -------------------- */
void resetTrafficTime() {
	for(int i = 0; i < 2 ; i++)
    for(int j=0;j<3;j++) trafficTime[i][j] = trafficTimeBackup[i][j];
    curpos = 0;
    curpos2 = 0;
}

void updateTrafficTime(void) {
    // main
    if(trafficTime[0][curpos]>0) trafficTime[0][curpos]--;
    if(trafficTime[0][curpos]<=0) {
        curpos++;
    }

    // sub
    if(trafficTime[1][curpos2]>0) trafficTime[1][curpos2]--;

    if(trafficTime[1][curpos2]<=0) {
        curpos2++;
    }
    if(curpos>2){
            	resetTrafficTime();
    }
    // Cập nhật LED
    updateTrafficLight_main(curpos);
    updateTrafficLight_sub(curpos2);
}

void updateTrafficLight_main(int pos){
    switch(pos)
    {
    case 2: // RED
        HAL_GPIO_WritePin(Red_main_GPIO_Port, Red_main_Pin, GPIO_PIN_RESET);      // bật đỏ
        HAL_GPIO_WritePin(Yellow_main_GPIO_Port, Yellow_main_Pin, GPIO_PIN_SET); // tắt vàng
        HAL_GPIO_WritePin(Green_main_GPIO_Port, Green_main_Pin, GPIO_PIN_SET);   // tắt xanh
        break;
    case 1: // YELLOW
        HAL_GPIO_WritePin(Red_main_GPIO_Port, Red_main_Pin, GPIO_PIN_SET);      // tắt đỏ
        HAL_GPIO_WritePin(Yellow_main_GPIO_Port, Yellow_main_Pin, GPIO_PIN_RESET);   // bật vàng
        HAL_GPIO_WritePin(Green_main_GPIO_Port, Green_main_Pin, GPIO_PIN_SET);   // tắt xanh
        break;
    case 0: // GREEN
        HAL_GPIO_WritePin(Red_main_GPIO_Port, Red_main_Pin, GPIO_PIN_SET);      // tắt đỏ
        HAL_GPIO_WritePin(Yellow_main_GPIO_Port, Yellow_main_Pin, GPIO_PIN_SET); // tắt vàng
        HAL_GPIO_WritePin(Green_main_GPIO_Port, Green_main_Pin, GPIO_PIN_RESET);     // bật xanh
        break;
    default:
        break;
    }
}

void updateTrafficLight_sub(int pos){
    switch(pos)
    {
    case 0: // RED
    	HAL_GPIO_WritePin(Red_sub_GPIO_Port, Red_sub_Pin, GPIO_PIN_RESET);      // tắt đỏ
		HAL_GPIO_WritePin(Yellow_sub_GPIO_Port, Yellow_sub_Pin, GPIO_PIN_SET); // tắt vàng
		HAL_GPIO_WritePin(Green_sub_GPIO_Port, Green_sub_Pin, GPIO_PIN_SET);
        break;
    case 2: // YELLOW
        HAL_GPIO_WritePin(Red_sub_GPIO_Port, Red_sub_Pin, GPIO_PIN_SET);      // tắt đỏ
        HAL_GPIO_WritePin(Yellow_sub_GPIO_Port, Yellow_sub_Pin, GPIO_PIN_RESET);   // bật vàng
        HAL_GPIO_WritePin(Green_sub_GPIO_Port, Green_sub_Pin, GPIO_PIN_SET);   // tắt xanh
        break;
    case 1: // GREEN
    	HAL_GPIO_WritePin(Red_sub_GPIO_Port, Red_sub_Pin, GPIO_PIN_SET);      // bật đỏ
		HAL_GPIO_WritePin(Yellow_sub_GPIO_Port, Yellow_sub_Pin, GPIO_PIN_SET); // tắt vàng
		HAL_GPIO_WritePin(Green_sub_GPIO_Port, Green_sub_Pin, GPIO_PIN_RESET);    // bật xanh
        break;
    default:
        break;
    }
}

void Led_Blinky(void){
	HAL_GPIO_TogglePin(Led_red_GPIO_Port, Led_red_Pin);
}

