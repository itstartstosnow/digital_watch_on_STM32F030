# include "stm32f0xx_conf.h"
# include "delay.h"
# include "key.h"
# include "ledDigit.h"
# include "timer.h"
# include "string.h"
	
// the displayed variables, externed to timer.c for display
int8_t displayedDigits[4] = {1, 2, 3, 4};         // left -> right
int8_t displayedDecimalPoints[4] = {0, 0, 1, 1};  // left -> right
int8_t flicker[4] = {0, 0, 0, 0};                 // whether or not flicker

// current date and time
int yyyy = 2018;
int mm = 12;
int dd = 21;
int HH = 13;
int MM = 15;
int SS = 00;

// declaration of some functions
void displayyyyy(void);
void displaymmdd(void);
void displayHHMM(void);
void displaySS(void);
// void displayTimer(void); // Todo

void switchMenuItem(int8_t *);
void enterSettingMode(int8_t);

void setyyyy(void);
void setmmdd(void);
void setHHMM(void);
void setSS(void);
void runAsTimer(void);

int main()
{
	/*
	Use left key to switch among the following 5 items(functions):
	0: y y y y.        year
	1: m m.d d         month and day
	2: H H.M M.        hour and minute
	3: - -.S S		   only second
	4: 0.0 0 0         the timer mode
	*/
	int8_t menuItem = 4;     // current item in the main menu

	int8_t keyPressed = 0;

	// some initializations
	SystemInit();            //Ensure CPU is running at correctly set clock speed
	SystemCoreClockUpdate(); //Update SystemCoreClock variable to current clock speed
    ledDigitInit();
	keyInit();
	delayInit();
	TIM3Init(49, 799, 50);   // interrupt every 5ms, and on / off each last 250 ms
	
    while(1)
    {	
		keyPressed = keyScan();
		if(keyPressed) {
			switch(keyPressed){
				case 1:{     // SW1, switch among menu items
					switchMenuItem(&menuItem);
					break;
				}
				case 3:{     // SW1 + SW2, enter the setting mode
					enterSettingMode(menuItem); // This function will not return until user exits setting mode
					break;
				}
			}
		}else{
			delayMsec(50);
		}	
		
    }
}

void switchMenuItem(int8_t* menuItemPtr){
	*menuItemPtr = ((*menuItemPtr) + 1) % 5;
	switch(*menuItemPtr){
		case 0: {
			displayyyyy();
			break;
		}
		case 1: {
			displaymmdd();
			break;
		}
		case 2: {
			displayHHMM();
			break;
		}
		case 3:{
			displaySS();
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

void enterSettingMode(int8_t menuItem){
	switch (menuItem) {
		case 0: {
			setyyyy(); // these functions will not return until user exits setting mode
			break;
		}
		case 1: {
			setmmdd();
			break;
		}
		case 2: {
			setHHMM();
			break;
		}
		case 3: {
			setSS();
			break;
		}
		case 4: {
			runAsTimer();
			break;
		}
	}
}

void displayyyyy(void) {
	displayedDigits[0] = yyyy / 1000;
	displayedDigits[1] = (yyyy / 100) % 10;
	displayedDigits[2] = (yyyy / 10) % 10;
	displayedDigits[3] = yyyy % 10;
	displayedDecimalPoints[0] = 0;
	displayedDecimalPoints[1] = 0;
	displayedDecimalPoints[2] = 0;
	displayedDecimalPoints[3] = 1;
}

void displaymmdd(void) {
	displayedDigits[0] = mm / 10;
	displayedDigits[1] = mm % 10;
	displayedDigits[2] = dd / 10;
	displayedDigits[3] = dd % 10;
	displayedDecimalPoints[0] = 0;
	displayedDecimalPoints[1] = 1;
	displayedDecimalPoints[2] = 0;
	displayedDecimalPoints[3] = 0;
}

void displayHHMM(void) {
	displayedDigits[0] = HH / 10;
	displayedDigits[1] = HH % 10;
	displayedDigits[2] = MM / 10;
	displayedDigits[3] = MM % 10;
	displayedDecimalPoints[0] = 0;
	displayedDecimalPoints[1] = 1;
	displayedDecimalPoints[2] = 0;
	displayedDecimalPoints[3] = 1;	
}

void displaySS(void) {
	displayedDigits[0] = 10;
	displayedDigits[1] = 10;
	displayedDigits[2] = SS / 10;
	displayedDigits[3] = SS % 10;
	displayedDecimalPoints[0] = 0;
	displayedDecimalPoints[1] = 0;
	displayedDecimalPoints[2] = 0;
	displayedDecimalPoints[3] = 0;
}

void setyyyy(){
	int8_t keyPressed = 0;
	memset(flicker, 1, 4);
	
	while (1) {
		keyPressed = keyScan();
		if (keyPressed == 1) { // SW1
			yyyy ++;
			if (yyyy >= 2070) yyyy = 1970; // the range of year is 1970 - 2070
			displayyyyy();
		} else if (keyPressed == 3) { // SW1 + SW2, exit the setting mode
			break;
		} else {
			delayMsec(50);
		}
	}
	
	memset(flicker, 0, 4);
	return;
}
void setmmdd(){
	return;
}
void setHHMM(){
	return;
}
void setSS(){
	return;
}
void runAsTimer(){
	return;
}



