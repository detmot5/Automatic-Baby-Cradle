/*
 * cradle.cpp
 *
 *  Created on: 27 lut 2020
 *      Author: norbe
 */

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#include "cradle.h"
#include "eeprom.h"
#include "common.h"
#include "d_Led.h"
//--------------------------------------------------------------------------
//						    GLOBAL VARIABLES
//--------------------------------------------------------------------------

svParams_t servoParams_eep EEMEM;
svParams_t servoParams;

bool stopFlag;
bool isCradleTimActive;
bool goSleep;
bool goStop;

static volatile uint16_t Timer;
static uint16_t servo_pos = _SERVO_MIN;


//--------------------------------------------------------------------------
//						  STATIC FUNCTIONS
//--------------------------------------------------------------------------

	// from Arduino
static inline int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max) {
return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



static int8_t servoDrive(void){
	static uint8_t isDone;

	if(servo_pos != servoParams.actualPos){
		isDone = 0;
		if(!Timer && !isDone){
				// speed - delay between each step
			Timer = servoParams.speed;
			// going forward
			if(servoParams.actualPos < servo_pos && !stopFlag){
					//increment on every step
				servoParams.actualPos++;
					//set this value to servo
				servoWrite(servoParams.actualPos);
			}
			//going back
			else if(!stopFlag){
					//decrement on every step
				servoParams.actualPos--;
				servoWrite(servoParams.actualPos);
			}
		}
	}
	else{
			//if first move done, 1 signalize ready to go back
			//if back move done, 2 signalize ready to next cycle
		isDone++;
	}
	return isDone;
}

static void stopProcedure(void){

	if(goStop){
		uint8_t actualSpeed = cradleGetParams(speed);
		if(actualSpeed <= 1){
			stopFlag = true;
			goStop = false;
		}
		else if(!Timers[start_stopCntTim]){
			cradleSetParams(speed,--actualSpeed);
			Timers[start_stopCntTim] = 100;
		}
	}
}



static void stopEvent(void){

	dLED::dP_blink(stopFlag);

	if(!Timers[cradleDownCnt] && isCradleTimActive){
		stopFlag = true;
		if(!goSleep){
			goSleep = true;
			Timers[timeToSleep] = servoParams.secondsToEnterSleep*100; //convert to 10ms ticks
		}
		else if(!Timers[timeToSleep]){ // if time to enter sleep elapsed
			stopFlag = false;
			isCradleTimActive = false;
			goSleep = false;
			sleep();
		}

	}
}




//--------------------------------------------------------------------------

void cradleInit(void){
		//**INITIALIZE SERVO PARAMETERS**
	eeprom_read_duration();
	eeprom_read_speed();
	eeprom_read_actual_pos();
	eeprom_read_time_to_sleep();

		// **INIT SERVO TIMER**
	TCCR1A |= (1<<WGM11);						// Fast PWM mode - TOP value - ICR1
	TCCR1B |= (1<<WGM12) | (1<<WGM13);
	TCCR1B |= (1<<CS11);						// Timer prescaler /8
	TCCR1A |= (1<<COM1A1);						// OC1A active (non inverting mode)
	ICR1 = usToTicks(_SERVO_REFRESH_INTERVAL);	// Set top value to 20000us
	servoWrite(servoParams.actualPos);
	stopFlag = true;
	SERVO_OUT();

		// **INIT TIME BASE TIMER**
	TCCR0A |= (1<<WGM01);						// CTC mode
	TCCR0B |= (1<<CS02) | (1<<CS00);			// Timer prescaler /1024
	TIMSK0 |= (1<<OCIE0A);						// Output Compare Interrupt enable
	OCR0A = 2;									// Time base

	sei();
}



int8_t CRADLE_EVENT(void){
	int8_t result;

	stopEvent();
	stopProcedure();

	if((result = servoDrive()) == 1 && !stopFlag){
		servo_pos = _SERVO_MIN; 	// move done, go back
	}
	else if(result == 2 && !stopFlag){
		servo_pos = servoParams.duration;		//repeat
	}
	return result;
}


	// Function to set parameters of cradle
	// input - target(range or speed), value from 0-9
	// output: 0 for correct value and -1 for wrong
int8_t cradleSetParams(svParamsEnum_t cradleParam, uint8_t value){
	int8_t result;
	if(value >= 1 && value <= 9){
		result = 0;
		switch(cradleParam){
		case range:
			servoParams.duration = map(value, 1 ,9, _SERVO_MIN,_SERVO_MAX);
			eeprom_update_duration();
			dLED::print(value);
			break;

		case speed:
			servoParams.speed = map(value, 1, 9, _SERVO_MAX_DELAY, _SERVO_MIN_DELAY); //invert values
			eeprom_update_speed();
			dLED::print(value);
			break;
		}
		// there will be also code to print it on screen
	}
	else result = -1;
	return result;
}

uint8_t cradleGetParams(svParamsEnum_t cradleParam){
	uint8_t value = 0;
	switch(cradleParam){
	case range:
		value = map(servoParams.duration, _SERVO_MIN, _SERVO_MAX, 1, 9);
		break;

	case speed:
		value = map(servoParams.speed, _SERVO_MAX_DELAY, _SERVO_MIN_DELAY, 1, 9);
		break;
	}

	return value;
}


void stop(bool isEnabled){
	goStop = isEnabled;
}






//ISR - time base for servo speed control
ISR(TIMER0_COMPA_vect){
	uint16_t n;
	n = Timer;
	if (n) Timer = --n;
}
