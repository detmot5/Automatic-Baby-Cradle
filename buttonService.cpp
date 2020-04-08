/*
 * buttonService.cpp
 *
 *  Created on: 27 Mar 2020
 *      Author: Norbert Bielak
 */


#include <avr/io.h>
#include <avr/pgmspace.h>

#include "MicroSwitch/MicroSwitch.h"
#include "common.h"
#include "cradle.h"
#include "d_Led.h"




static uint8_t changingValue;




static void buttonSwitchHandler(void){
	changingValue ^= 1;

	// For speed manipulating option the S
	// For range manipulating option the "A" symbol is displayed
	switch(changingValue){
	case speed:
		dLED::print(cradleGetParams(speed));
		dLED::displayOnTop(SPEED_SYMBOL,1000);
		break;

	case range:
		dLED::print(cradleGetParams(range));
		dLED::displayOnTop(RANGE_SYMBOL,1000);
		break;
	}
}

static void buttonSwitchHoldHandler(void){
	stop(true);
}


static void buttonResetHandler(void){
	reset();
}


static void buttonResetHoldHandler(void){
	// turn on sleep mode
	sleep();
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
	ButtonSwitch.registerHoldEventCallback(buttonSwitchHoldHandler);

	ButtonReset.registerPressEventCallback(buttonResetHandler);
	ButtonReset.registerHoldEventCallback(buttonResetHoldHandler);

	ButtonUp.registerPressEventCallback(buttonUpHandler);
	ButtonUp.registerHoldEventCallback(buttonUpHandler);

	ButtonDown.registerPressEventCallback(buttonDownHandler);
	ButtonDown.registerHoldEventCallback(buttonDownHandler);

}


