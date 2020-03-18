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

#include "cradle.h"
#include "eeprom.h"
#include "common.h"
//--------------------------------------------------------------------------
//						    GLOBAL VARIABLES
//--------------------------------------------------------------------------

svParams_t servoParams_eep EEMEM;
svParams_t servoParams;

bool stopFlag;
bool isCradleTimActive;

static volatile uint16_t Timer;
static uint16_t servo_pos = _SERVO_MIN;


//--------------------------------------------------------------------------
//						  STATIC FUNCTIONS
//--------------------------------------------------------------------------


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

//--------------------------------------------------------------------------

void cradleInit(void){
		//**INITIALIZE SERVO PARAMETERS**
	eeprom_read_duration();
	eeprom_read_speed();
	eeprom_read_actual_pos();

		// **INIT SERVO TIMER**
	TCCR1A |= (1<<WGM11);						// Fast PWM mode - TOP value - ICR1
	TCCR1B |= (1<<WGM12) | (1<<WGM13);
	TCCR1B |= (1<<CS11);						// Timer prescaler /8
	TCCR1A |= (1<<COM1A1);						// OC1A active (non inverting mode)
	ICR1 = usToTicks(_SERVO_REFRESH_INTERVAL);	// Set top value to 20000us
	servo_pos = servoParams.duration+1;
	SERVO_OUT;


		// **INIT TIME BASE TIMER**
	TCCR0A |= (1<<WGM01);						// CTC mode
	TCCR0B |= (1<<CS02) | (1<<CS00);			// Timer prescaler /1024
	TIMSK0 |= (1<<OCIE0A);						// Output Compare Interrupt enable
	OCR0A = 2;									// Time base

	sei();
}



int8_t CRADLE_EVENT(void){
	int8_t result;
	if(!Timers[cradleDownCnt] && isCradleTimActive){
		stopFlag = true;
	}

	if((result = servoDrive()) == 1 && !stopFlag){
		servo_pos = _SERVO_MIN; 	// move done, go back
	}
	else if(result == 2 && !stopFlag){
		servo_pos = servoParams.duration;		//repeat
	}
	return result;
}




//ISR - time base for servo speed control
ISR(TIMER0_COMPA_vect){
	uint16_t n;
	n = Timer;
	if (n) Timer = --n;
}
