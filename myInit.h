
/*
 * myInit.h
 *
 *  Created on: Dec 6, 2019
 *      Author: who_you_are
 */

#ifndef MYINIT_H_
#define MYINIT_H_

#define LED_PORT  gpioPortC
#define LED_PIN   9

#define TIMER_TOP                   100
#define TIMER_CHANNEL               0
#define  DUTY_CYCLE_ 10

void initTimer(void);

#endif /* MYINIT_H_ */
