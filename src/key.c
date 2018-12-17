# include "stm32f0xx_conf.h"
# include "key.h"
# include "delay.h"

void keyInit (void)
{ 
	// SW1 --> PA9, SW2 --> PA10  之前的板子用的上拉输入？
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; // ??
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

int8_t keyScan (void) {
	int8_t pressed = 0;
	int8_t i;
	for (i = 5; i > 0; i --) {
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == 0) pressed |= 1;         // sw1
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == 0) pressed |= (1 << 1); // sw2
		delayMsec(5);
	}	
	return pressed;
}
