#ifndef __DELAY_H
#define __DELAY_H 			   
# include "stm32f0xx_conf.h"

/*
// Removed on 11:48, 2018.12.14 时间总是慢三倍，不知道为什么
void delay_init(void);
void delay_ms(int16_t nms);
void delay_us(int16_t nus);
*/

void SysTick_Handler(void);
void delayInit (void);
void delayMsec(uint32_t);
void delaySec(uint32_t);

#endif





























