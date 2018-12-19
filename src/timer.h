#ifndef __TIMER_H
#define __TIMER_H 			   
# include "stm32f0xx_conf.h"

int8_t isLeapYear(int);
void TIM3Init(uint32_t period, uint16_t prescaler, int8_t flickerDurationParam);
void TIM3_IRQHandler(void);
void TIM14Init(uint32_t period, uint16_t prescaler);
void TIM14_IRQHandler(void);
void TIM16Init(uint32_t period, uint16_t prescaler);
void TIM16_IRQHandler(void);

#endif





























