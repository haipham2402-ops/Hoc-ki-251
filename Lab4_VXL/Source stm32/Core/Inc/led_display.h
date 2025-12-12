/*
 * led_display.h
 *
 *  Created on: Nov 5, 2025
 *      Author: kanek
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_
#include "main.h"
extern int trafficTime[2][3];          // thời gian hiện tại
extern int trafficTimeBackup[2][3];
extern int mode_digit;
void DisplayDigit(int pos);
void DisplayDigit_mode(int pos);
void DisplayMultiplex(void);
void DisplayMultiplex_mode(void);
void TurnOffMultiplex(void);
void TurnOffMultiplex_mode(void);
void resetTrafficTime();
void updateTrafficTime(void);
void updateTrafficLight_main(int pos);
void updateTrafficLight_sub(int pos);
void Led_Blinky(void);
#endif /* INC_LED_DISPLAY_H_ */
