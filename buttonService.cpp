/*
 * buttonService.cpp
 *
 *  Created on: 27 Mar 2020
 *      Author: Norbert Bielak
 */


#include <avr/io.h>

#include "MicroSwitch/MicroSwitch.h"
#include "LCD/lcd44780.h"
#include "common.h"
#include "cradle.h"




#define SPEED 0
#define DURATION 1



static uint8_t changingValue;




static void buttonSwitchHandler(void){

	changingValue ^= 1;
	DBG_LED_TOG();
}

static void buttonResetHandler(void){
	reset();
}

static void buttonUpHandler(void){
	int8_t mappedValue;

	switch(changingValue){
	case speed:
		mappedValue = cradleGetParams(speed);
		if(mappedValue > 0 && mappedValue <= 9){
			mappedValue += 1;
			cradleSetParams(speed, mappedValue);
		}
		break;
	case range:
		mappedValue = cradleGetParams(range);
		if(mappedValue >= 1 && mappedValue <= 9){
			mappedValue += 1;
			cradleSetParams(range, mappedValue);
		}
		break;
	}
}

static void buttonDownHandler(void){
	int8_t mappedValue;

	switch(changingValue){
	case speed:
		mappedValue = cradleGetParams(speed);
		if(mappedValue >= 1 && mappedValue <= 9){
			mappedValue -= 1;
			cradleSetParams(speed, mappedValue);
		}
		break;
	case range:
		mappedValue = cradleGetParams(range);
		if(mappedValue >= 1 && mappedValue <= 9){
			mappedValue -= 1;
			cradleSetParams(range, mappedValue);
		}
		break;
	}
}



void registerButtonsCallbacks(void){
	ButtonSwitch.registerPressEventCallback(buttonSwitchHandler);
	ButtonReset.registerPressEventCallback(buttonResetHandler);

	ButtonUp.registerPressEventCallback(buttonUpHandler);
	ButtonUp.registerHoldEventCallback(buttonUpHandler);

	ButtonDown.registerPressEventCallback(buttonDownHandler);
	ButtonDown.registerHoldEventCallback(buttonDownHandler);

}


