# include "stm32f0xx_conf.h"
# include "timer.h"
# include "ledDigit.h"

int8_t currentDigit = 0;
int8_t displayedDigits[4] = {1, 2, 3, 4}; // left -> right
int8_t displayedDecimalPoints[4] = {0, 0, 1, 1}; // left -> right

void TIM3Init(uint32_t period, uint16_t prescaler)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	
	//定时器TIM初始化
	TIM_TimeBaseStructure.TIM_Period = period;                      //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler;                //设置用来作为TIM时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                 //根据指定的参数初始化TIM的时间基数单位
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);                      //使能指定的TIM中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;                  //优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                  //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);                                  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIM					 
}

void TIM3_IRQHandler(void)   //TIM中断
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
		displayDigit(displayedDigits[currentDigit], displayedDecimalPoints[currentDigit]);
	}
}