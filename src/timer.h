#ifndef __TIMER_H
#define __TIMER_H 			   
# include "stm32f0xx_conf.h"

void TIM3Init(uint32_t period, uint16_t prescaler, int8_t flickerDurationParam);
void TIM3_IRQHandler(void);

#endif





























