# include "stm32f0xx_conf.h"
# include "delay.h"
# include "key.h"
# include "ledDigit.h"
# include "timer.h"
# include "string.h"
	
// global variables
int8_t displayedDigits[4] = {1, 2, 3, 4};         // left -> right
int8_t displayedDecimalPoints[4] = {0, 0, 1, 1};  // left -> right
int8_t flicker[4] = {0, 0, 0, 0};                 // whether or not flicker

int yyyy = 2018;
int mm = 12;
int dd = 21;
int HH = 19;
int MM = 45;
int SS = 00;

int8_t daysInMonth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int8_t daysInMonthAtLeapYear[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int timerMSec = 0;

// declaration of some functions
void displayyyyy(void);
void displaymmdd(void);
void displayHHMM(void);
void displaySS(void);
void displayTimer(void);

void switchMenuItem(int8_t *);
void updateCurrentDisplay(int8_t);
void enterSettingMode(int8_t);

void setFlickeringItem(int8_t);
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
	TIM3Init(49, 799, 40);   // TIM3 interrupt every 5ms, and on / off each last 200 ms
	TIM14Init(9999, 799);    // TIM14 interrupt every 1s
	TIM16Init(9, 3);         // TIM16 interrupt every 1ms, note that TIM6 = 40kHz, not 8MHz
	
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
			updateCurrentDisplay(menuItem); // refresh here instead of in TIM14_IRQHandler()
		}	
    }
}

void updateCurrentDisplay(int8_t menuItem){
	switch(menuItem){
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
			displayTimer();
			break;
		}
	}
}

void switchMenuItem(int8_t* menuItemPtr){
	*menuItemPtr = ((*menuItemPtr) + 1) % 5;
	updateCurrentDisplay(*menuItemPtr);
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
void displayTimer (void) {
	displayedDigits[0] = timerMSec / 1000;
	displayedDigits[1] = (timerMSec / 100) % 10;
	displayedDigits[2] = (timerMSec / 10) % 10;
	displayedDigits[3] = timerMSec % 10;
	displayedDecimalPoints[0] = 1;
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
			if (yyyy >= 2070) yyyy = 1970; // the range of year is 1970 - 2069
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

// applys to mmdd and HHMM
void setFlickeringItem(int8_t item) {
	if (item == 0) {
		flicker[0] = 1;
		flicker[1] = 1;
		flicker[2] = 0;
		flicker[3] = 0;
	} else {
		flicker[0] = 0;
		flicker[1] = 0;
		flicker[2] = 1;
		flicker[3] = 1;
	}
}

void setmmdd(){
	int8_t settingItem = 0;          // the item being set, 0 = mm, 1 = dd
	int8_t keyPressed = 0;
	setFlickeringItem(settingItem);
	
	while (1) {
		keyPressed = keyScan();
		if (keyPressed == 1) {       // SW1
			if (settingItem == 0) {  // setting mm
				mm ++;
				if (mm > 12) mm = 1; // the range of month is 1 - 12
			} else {                 // setting dd
				dd ++;
				if (dd > ((isLeapYear(yyyy))? daysInMonthAtLeapYear[mm]: daysInMonth[mm])) {
					dd = 1;
				}
			}
			displaymmdd();
		} else if (keyPressed == 2) { // SW2
			settingItem = ~settingItem;
			setFlickeringItem(settingItem);
		} else if (keyPressed == 3) { // SW1 + SW2, exit the setting mode
			break;
		} else {
			delayMsec(50);
		}
	}
		
	memset(flicker, 0, 4);
	return;
}
void setHHMM(){
	int8_t settingItem = 0;          // the item being set, 0 = HH, 1 = MM
	int8_t keyPressed = 0;
	setFlickeringItem(settingItem);
	
	while (1) {
		keyPressed = keyScan();
		if (keyPressed == 1) {       // SW1
			if (settingItem == 0) {  // setting HH
				HH = (HH + 1) % 24;  // the range of hour is 0 - 23
			} else {                 // setting MM
				MM = (MM + 1) % 60;  // the range of hour is 0 - 59
			}
			displayHHMM();
		} else if (keyPressed == 2) { // SW2
			settingItem = ~settingItem;
			setFlickeringItem(settingItem);
		} else if (keyPressed == 3) { // SW1 + SW2, exit the setting mode
			break;
		} else {
			delayMsec(50);
		}
	}
		
	memset(flicker, 0, 4);
	return;
}
void setSS(){
	int8_t keyPressed = 0;
	
	TIM_Cmd(TIM14, DISABLE);  // Disable TIM14, so time will not run
	flicker[0] = 0;
	flicker[1] = 0;
	flicker[2] = 1;
	flicker[3] = 1;
	
	while (1) {
		keyPressed = keyScan();
		if (keyPressed == 1) { // SW1
			SS ++;
			if (SS >= 60) SS = 0; // the range of second is 0 - 59
			displaySS();
		} else if (keyPressed == 3) { // SW1 + SW2, exit the setting mode
			break;
		} else {
			delayMsec(50);
		}
	}
	
	memset(flicker, 0, 4);
	TIM_Cmd(TIM14, ENABLE);  // Enable TIM
	return;
}

void runAsTimer(){
	int8_t keyPressed = 0;
	memset(flicker, 1, 4);   // start flickering
	
	while (1) {
		keyPressed = keyScan();
		if (keyPressed == 1) { // SW1, start timer
			memset(flicker, 0, 4);
			TIM_Cmd(TIM16, ENABLE);
			
			while (1) {
				displayTimer();
				keyPressed = keyScan();
				if (keyPressed == 1){ // SW1, stop timer
					break;
				}
			}
			
			TIM_Cmd(TIM16, DISABLE);
			memset(flicker, 1, 4);
		} else if (keyPressed == 2) { // SW2, when timer is stopped, clear the timer
			timerMSec = 0;		
			displayTimer();
		} else if (keyPressed == 3) { // SW1 + SW2, exit the timer mode
			break;
		} 
	}
	
	memset(flicker, 0, 4);
	return;
}



