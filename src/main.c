# include "stm32f0xx_conf.h"
# include "delay.h"
# include "key.h"
# include "ledDigit.h"
# include "timer.h"
	
// the displayed variables, externed to timer.c for display
int8_t displayedDigits[4] = {1, 2, 3, 4};         // left -> right
int8_t displayedDecimalPoints[4] = {0, 0, 1, 1};  // left -> right

// current date and time
int yyyy = 2018;
int mm = 12;
int dd = 21;
int HH = 13;
int MM = 30;
int SS = 00;

/*
Use left key to switch among the following 5 items(functions):
0 - y y y y.        year
1 - m m.d d         month and day
2 - H H.M M.        hour and minute
3 - M M.S S			minute and second
4 - 0.0 0 0         the timer mode
*/
int8_t menuItem = 4; // current item in the main menu. 

// declaration of some functions
void switchMenuItem(void);

int main()
{
	SystemInit(); //Ensure CPU is running at correctly set clock speed
	SystemCoreClockUpdate(); //Update SystemCoreClock variable to current clock speed
	
	int8_t keyPressed = 0;
	int8_t toggleVar = 0;
	
    ledDigitInit();
	keyInit();
	delayInit();
	TIM3Init(49, 799); // (799 + 1) / 8MHz = 0.1 ms, then plus (period + 1)
	
    while(1)
    {	
		keyPressed = keyScan();
		if(keyPressed) {
			switch(keyPressed){
				case 1:{ // SW1
					switchMenuItem();
					break;
				}
				case 3:{ // SW1 + SW2
					// Todo, enter the setting mode
					break;
				}
			}
		}else{
			delayMsec(100);
		}	
		
    }
}

void switchMenuItem(void){
	menuItem = (menuItem + 1) % 5;
	switch(menuItem){
		case 0: {
			displayedDigits[0] = yyyy / 1000;
			displayedDigits[1] = (yyyy / 100) % 10;
			displayedDigits[2] = (yyyy / 10) % 10;
			displayedDigits[3] = yyyy % 10;
			displayedDecimalPoints[0] = 0;
			displayedDecimalPoints[1] = 0;
			displayedDecimalPoints[2] = 0;
			displayedDecimalPoints[3] = 1;
			break;
		}
		case 1: {
			displayedDigits[0] = mm / 10;
			displayedDigits[1] = mm % 10;
			displayedDigits[2] = dd / 10;
			displayedDigits[3] = dd % 10;
			displayedDecimalPoints[0] = 0;
			displayedDecimalPoints[1] = 1;
			displayedDecimalPoints[2] = 0;
			displayedDecimalPoints[3] = 0;
			break;
		}
		case 2: {
			displayedDigits[0] = HH / 10;
			displayedDigits[1] = HH % 10;
			displayedDigits[2] = MM / 10;
			displayedDigits[3] = MM % 10;
			displayedDecimalPoints[0] = 0;
			displayedDecimalPoints[1] = 1;
			displayedDecimalPoints[2] = 0;
			displayedDecimalPoints[3] = 1;
			break;
		}
		case 3:{
			displayedDigits[0] = MM / 10;
			displayedDigits[1] = MM % 10;
			displayedDigits[2] = SS / 10;
			displayedDigits[3] = SS % 10;
			displayedDecimalPoints[0] = 0;
			displayedDecimalPoints[1] = 1;
			displayedDecimalPoints[2] = 0;
			displayedDecimalPoints[3] = 0;
			break;
		}
		case 4:{
			displayedDigits[0] = 0;
			displayedDigits[1] = 0;
			displayedDigits[2] = 0;
			displayedDigits[3] = 0;
			displayedDecimalPoints[0] = 1;
			displayedDecimalPoints[1] = 0;
			displayedDecimalPoints[2] = 0;
			displayedDecimalPoints[3] = 0;
			break;
		}
	}
}

