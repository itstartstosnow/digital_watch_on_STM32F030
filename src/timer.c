# include "stm32f0xx_conf.h"
# include "timer.h"
# include "ledDigit.h"

int8_t currentDigit = 0;                                           // currently displayed digit

// only effective when flicker is on
int8_t flickerState = 0;                                           // current state, 0 = off
int8_t flickerCount = 0;                                           // count to control the flicker time
int8_t flickerDuration;                                            // the duration of a on/off in a flicker

extern int8_t displayedDigits[4];                                  // from main.c
extern int8_t displayedDecimalPoints[4];                           // from main.c
extern int8_t flicker[4];                                          // from main.c

// time between two interruption: t = (prescaler + 1) / 8MHz * (period + 1)
// on and off time when flicker: t * flickerDurationParam
void TIM3Init(uint32_t period, uint16_t prescaler, int8_t flickerDurationParam)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// Enable clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	
	TIM_TimeBaseStructure.TIM_Period = period;                      //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler;                //设置用来作为TIM时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                 //根据指定的参数初始化TIM的时间基数单位
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);                      //使能指定的TIM中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;                  //优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                  //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);                                  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  // Enable TIM

	flickerDuration = flickerDurationParam;
}

// TIM interrupt
// display a digit
void TIM3_IRQHandler(void)   
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIM更新中断标志 
		
		currentDigit = (currentDigit + 1) % 4; // 0, 1, 2, 3, left --> right
		
		switch(currentDigit){
			case 3: { // LD1, the right digit
				GPIO_SetBits(GPIOA, GPIO_Pin_5);
				GPIO_ResetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_7);
				break;
			}
			case 2: { // LD2
				GPIO_SetBits(GPIOA, GPIO_Pin_4);
				GPIO_ResetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
				break;
			}
			case 1: { // LD3
				GPIO_SetBits(GPIOA, GPIO_Pin_7);
				GPIO_ResetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
				break;
			}
			case 0: { // LD4, the left digit
				GPIO_SetBits(GPIOA, GPIO_Pin_6);
				GPIO_ResetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7);
				break;
			}
		}
		// if it is flickering, and now it is in the "off" phase, shut it down
		if (flicker[currentDigit] && flickerState == 0) { 
			GPIO_ResetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
		}
		
		displayDigit(displayedDigits[currentDigit], displayedDecimalPoints[currentDigit]);
	
		flickerCount ++;
		if (flickerCount >= flickerDuration) {
			flickerCount = 0;
			flickerState = ~flickerState;
		}
	}
}

