# include "stm32f0xx_conf.h"
# include "delay.h"
# include "key.h"
# include "ledDigit.h"
	

int main()
{
	SystemInit(); //Ensure CPU is running at correctly set clock speed
	SystemCoreClockUpdate(); //Update SystemCoreClock variable to current clock speed
	
	int8_t keyPressed = 0;
	int8_t toggleVar = 0;
	
    ledDigitInit();
	keyInit();
	delayInit();

	GPIO_SetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	
    while(1)
    {	
		int8_t d;
		for (d = 0; d < 10; d ++) {
			displayDigit(d, 0);
			delayMsec(500);
		}
		
		
//		
//		keyPressed = keyScan();
////		if(keyPressed) {
//			switch(keyPressed){
//				case 1:{ // SW1
//					GPIO_SetBits(GPIOA, GPIO_Pin_5); 
//					break;
//				}
//				case 2:{ // SW2
//					GPIO_ResetBits(GPIOA, GPIO_Pin_5); 
//					break;
//				}
//			}
//		}else{
//			delayMsec(50);
//		}	
		
		
//		toggleVar = ~toggleVar;
//		if (toggleVar) {
//			GPIO_SetBits(GPIOA, GPIO_Pin_5);
//		} else {
//			GPIO_ResetBits(GPIOA, GPIO_Pin_5);
//		}
//		delayMsec(500);
    }
}

