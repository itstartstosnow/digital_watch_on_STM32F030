# include "stm32f0xx_conf.h"
# include "delay.h"

// Define time keeping variables, millisecond, second
volatile uint32_t msec = 0, sec = 0;

// Systick interrupt handler, interrupts at "interrupt rate" per second
// Currently set to interrupt at millisecond intervals
// Note that this function overrides line 97 - 99 in stm32f0xx.c
void SysTick_Handler(void){
	msec ++; //Increment millisecond variable
	if(msec % 1000 == 0){ //If 1000 msec have passed, increment sec
		sec++;
	}
}

void delayInit (void) {
	//Set time variables to zero at the start of the infinite loop
	SysTick_Config(SystemCoreClock / 1e3); //Set up a systick interrupt every millisecond
	msec = sec = 0;
}

// Delay function for millisecond delay
void delayMsec(uint32_t ms){
	volatile uint32_t start = msec;
	while((msec - start) < ms) asm volatile("nop");
}

// Delay function for second delay
void delaySec(uint32_t s){
	volatile uint32_t start = sec;
	while((sec - start) < s) asm volatile("nop");
}


/* 
// Removed on 11:48, 2018.12.14 ʱ����������������֪��Ϊʲô

# include "stm32f0xx_conf.h"
# include "delay.h"

static int8_t  fac_us = 0;//us��ʱ������
static int16_t fac_ms = 0;//ms��ʱ������

//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
void delay_init()	 
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	fac_us = SystemCoreClock / 8;	// sysTick 48MHz
	fac_ms = (int16_t)fac_us * 1000;//��ucos��,����ÿ��ms��Ҫ��systickʱ����   
}								    

void delay_us(int16_t nus)
{		
	unsigned int temp = nus * fac_us;	    	 
	SysTick->LOAD = temp - 1; //ʱ�����	  		 
	SysTick->VAL = 0x00;        //��ռ�����
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;          //��ʼ����	 
	do
	{
		temp = SysTick->CTRL;
	}
	while((temp & SysTick_CTRL_COUNTFLAG_Msk) != SysTick_CTRL_COUNTFLAG_Msk);
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL = 0X00;       //��ռ�����	 
}

void delay_ms(int16_t nms)
{	 		  	  
	int32_t temp = nms * fac_ms;		   
	SysTick->LOAD = temp - 1;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL = 0x00;           //��ռ�����
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;          //��ʼ����  
	do
	{
		temp = SysTick->CTRL;
	}
	while((temp & SysTick_CTRL_COUNTFLAG_Msk) != SysTick_CTRL_COUNTFLAG_Msk); 
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL = 0X00;       //��ռ�����	  	    
} 
*/

