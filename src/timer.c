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

	//ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	
	//��ʱ��TIM��ʼ��
	TIM_TimeBaseStructure.TIM_Period = period;                      //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler;                //����������ΪTIMʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                 //����ָ���Ĳ�����ʼ��TIM��ʱ�������λ
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);                      //ʹ��ָ����TIM�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;                  //���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                  //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);                                  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM					 
}

void TIM3_IRQHandler(void)   //TIM�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM�����жϷ������
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIM�����жϱ�־ 
		
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