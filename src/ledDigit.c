# include "stm32f0xx_conf.h"
# include "ledDigit.h"
# include "delay.h"

int8_t displayDigits[10]={ // written in the order of "PGFEDCBA", 1 = on, 0 = off
	0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b01111111,
    0b01101111
}; // 0-9

void displayDigit(int8_t digit, int8_t decimalPoint){
	int8_t display, i;
	display = displayDigits[digit];
	if (decimalPoint) display |= 0x80; // display the decimal points
	
	for(i = 0; i < 8; i ++) {
		// A1 = DDA = 14 = SI = data, serial input signal
		if (display & 0x80) {
			GPIO_SetBits(GPIOA, GPIO_Pin_1); 
		} else {
			GPIO_ResetBits(GPIOA, GPIO_Pin_1); 
		}
		display = display << 1; 
		
		// A3 = DCK = 11 = SCK = CLK, 0 --> 1, save one digit to S.R.
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
		GPIO_SetBits(GPIOA, GPIO_Pin_3);
	}
	// A2 = DLC = 12 = RCK, 0 --> 1, stored into register
	GPIO_ResetBits(GPIOA, GPIO_Pin_2); 
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
}


void ledDigitInit ()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	GPIO_InitStructure.GPIO_Pin |= (GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
