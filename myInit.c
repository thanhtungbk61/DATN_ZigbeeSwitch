
/*
 * myInit.c
 *
 *  Created on: Dec 5, 2019
 *      Author: who_you_are
 */
#include "af.h"
#include "em_timer.h"
#include "myInit.h"

void initTimer(void)
{
      //CHIP_Init();

      CMU_ClockEnable(cmuClock_GPIO, true);
      CMU_ClockEnable(cmuClock_TIMER1, true);

      // Enable LED output
      GPIO_PinModeSet(LED_PORT, LED_PIN, gpioModePushPull, 0);

      // Create the timer count control object initializer
      TIMER_InitCC_TypeDef timerCCInit = TIMER_INITCC_DEFAULT;
      timerCCInit.mode = timerCCModePWM;
      timerCCInit.cmoa = timerOutputActionToggle;

      // Configure CC channel 2
      TIMER_InitCC(TIMER1, TIMER_CHANNEL, &timerCCInit);

      // Route CC2 to location 1 (PE3) and enable pin for cc2
        TIMER1->ROUTELOC0 |=  TIMER_ROUTELOC0_CC0LOC_LOC14;
        TIMER1->ROUTEPEN |= TIMER_ROUTEPEN_CC0PEN;

      // Set Top Value
      TIMER_TopSet(TIMER1, TIMER_TOP);

      // Set the PWM duty cycle here!
      TIMER_CompareBufSet(TIMER1, TIMER_CHANNEL, DUTY_CYCLE_);

      // Create a timerInit object, based on the API default
      TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
      timerInit.prescale = timerPrescale256;

      TIMER_Init(TIMER1, &timerInit);

        // Enable TIMER1 compare event interrupts to update the duty cycle
//        TIMER_IntEnable(TIMER1, TIMER_IEN_CC0);
//        NVIC_EnableIRQ(TIMER1_IRQn);
}


void TIMER1_IRQHandler(void)
{
	//USART_Tx(USART3,0);
	GPIO_PinOutClear(gpioPortD,9);

  uint32_t flags = TIMER_IntGet(TIMER1);
  TIMER_IntClear(TIMER1, flags);

  // Update CCVB to alter duty cycle starting next period
  TIMER_CompareBufSet(TIMER1, TIMER_CHANNEL, DUTY_CYCLE_);
  //TIMER_CompareBufSet(TIMER1, 0, (TIMER_TopGet(TIMER1) * DUTY_CYCLE_) / 100);
}
