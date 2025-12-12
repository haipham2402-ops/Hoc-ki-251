#include "input_reading.h"

// ======= Biến lưu trữ trạng thái nút =======
static GPIO_PinState buttonBuffer[N0_OF_BUTTONS];        // Kết quả sau chống dội
static GPIO_PinState debounceButtonBuffer1[N0_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[N0_OF_BUTTONS];
static GPIO_PinState buttonBufferBefore[N0_OF_BUTTONS];  // Lưu trạng thái trước đó

// ======= Biến đếm và cờ =======
static uint16_t counterForButtonPress1s[N0_OF_BUTTONS];
static uint8_t flagForButtonPress1s[N0_OF_BUTTONS];      // =1 nếu giữ >1s
static uint8_t flagForButtonPressDetected[N0_OF_BUTTONS]; // =1 nếu nhấn 1 lần

// ======= Đọc nút nhấn (gọi mỗi 10ms trong timer interrupt) =======
void button_reading(void) {
	for (uint8_t i = 0; i < N0_OF_BUTTONS; i++) {
		// --- Đọc chân thực tế ---
		switch (i) {
			case 0:
				debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin);
				break;
			case 1:
				debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin);
				break;
			case 2:
				debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin);
				break;
			default:
				break;
		}

		// --- Chống dội ---
		if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
			buttonBufferBefore[i] = buttonBuffer[i];
			buttonBuffer[i] = debounceButtonBuffer1[i];
		}
		debounceButtonBuffer2[i] = debounceButtonBuffer1[i];

		// --- Kiểm tra nhấn 1 lần ---
		if (buttonBufferBefore[i] == BUTTON_IS_RELEASED && buttonBuffer[i] == BUTTON_IS_PRESSED) {
			flagForButtonPressDetected[i] = 1;
		}

		// --- Kiểm tra giữ lâu hơn 1s ---
		if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
			if (counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING) {
				counterForButtonPress1s[i]++;
			} else {
				flagForButtonPress1s[i] = 1;
				HAL_GPIO_TogglePin(Led_red_GPIO_Port, Led_red_Pin);
			}
		} else {
			counterForButtonPress1s[i] = 0;
			flagForButtonPress1s[i] = 0;
		}
	}
}

// ======= Hàm kiểm tra nhấn 1 lần (chỉ true đúng 1 chu kỳ đọc) =======
unsigned char button_is_pressed(uint8_t index) {
	if (index >= N0_OF_BUTTONS) return 0;
	if (flagForButtonPressDetected[index]) {
		flagForButtonPressDetected[index] = 0; // reset sau khi đọc
		return 1;
	}
	return 0;
}

// ======= Hàm kiểm tra giữ lâu > 1s =======
unsigned char button_is_pressed_1s(uint8_t index) {
	if (index >= N0_OF_BUTTONS) return 0xFF;
	return (flagForButtonPress1s[index] == 1);
}
