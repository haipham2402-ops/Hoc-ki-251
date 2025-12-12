#ifndef INC_TIMER_H_
#define INC_TIMER_H_
#include "main.h"
extern int timer0_flag;
extern int timer1_flag;

void setTimer0(int duration);
void setTimer1(int duration);
void timer_run(void);
#endif /* INC_TIMER_H_ */
