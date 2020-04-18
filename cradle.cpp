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


#include "UARTlib/uart.h"
#include "cradle.h"
#include "eeprom.h"
#include "common.h"
#include "d_Led.h"
#include "texts.h"

namespace Cradle{

//--------------------------------------------------------------------------
//						    GLOBAL VARIABLES
//--------------------------------------------------------------------------

	svParams_t Params_eep EEMEM;
	svParams_t Params;

	bool stopFlag;
	bool isTimActive;

	static bool goSleep;
	static bool goStop;
	static bool goStart;


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

		if(servo_pos != Params.actualPos){
			isDone = 0;
			if(!Timer && !isDone){
					// speed - delay between each step
				Timer = Params.speed;
				// going forward
				if(Params.actualPos < servo_pos && !stopFlag){
						//increment on every step
					Params.actualPos++;
						//set this value to servo
					servoWrite(Params.actualPos);
				}
				//going back
				else if(!stopFlag){
						//decrement on every step
					Params.actualPos--;
					servoWrite(Params.actualPos);
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


	static void startStopProcedure(void){
		static uint8_t loadedSpeed = 0;
		static uint8_t actualSpeed = 0;
		static uint8_t cnt = 0;
		static bool isSpeedLoaded = false;
		static bool stopFlagChanged = false;


		if(goStop && !stopFlag){
			goStart = false;
			actualSpeed = GetParams(speed);
			if(!isSpeedLoaded){
				loadedSpeed = actualSpeed;
				isSpeedLoaded = true;
			}
			if(actualSpeed <= 1){
				stopFlag = true;
				goStop = false;
				isSpeedLoaded = false;
				stopFlagChanged = true;
			}
			else if(!Timers[start_stopCntTim]){
				SetParams(speed,--actualSpeed);
				Timers[start_stopCntTim] = 100;
			}
		}
		else if(goStart){
			if(loadedSpeed){
				stopFlag = false;
				if(cnt == loadedSpeed){
					cnt = 0;
					loadedSpeed = 0;
					goStart = false;
					stopFlagChanged = true;
				}
				else if(!Timers[start_stopCntTim]){
					SetParams(speed, ++cnt);
					Timers[start_stopCntTim] = 100;
				}
			}
		}
		if(stopFlagChanged){
			USART_deviceAnswer(PSTR("AT_STOP="),stopFlag);
			stopFlagChanged = false;
		}
	}




	static void stopEvent(void){

		dLED::dP_blink(stopFlag);

		if(!Timers[cradleDownCnt] && isTimActive){
			Stop(true);
			if(!goSleep){
				goSleep = true;
				Timers[timeToSleep] = Params.secondsToEnterSleep*100; //convert to 10ms ticks
			}
			else if(!Timers[timeToSleep]){ // if time to enter sleep elapsed
				stopFlag = false;
				isTimActive = false;
				goSleep = false;
				sleep();
			}

		}
	}




	//--------------------------------------------------------------------------

	bool ManipulateAllowed(){
		if(!stopFlag && !goStop) return true;
		return false;
	}


	void Stop(bool state){
		goStop = state;
		goStart = !state;
	}

	void USART_deviceAnswer(const char *PGMcmd, uint16_t value){
		USART_PutStr_P(PGMcmd);
		USART_PutInt(value,dec);
		USART_PutStr_P(_endl);
	}


	void Init(void){
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
		SERVO_OUT();
		servoWrite(Params.actualPos);


			// **INIT TIME BASE TIMER**
		TCCR0A |= (1<<WGM01);						// CTC mode
		TCCR0B |= (1<<CS02) | (1<<CS00);			// Timer prescaler /1024
		TIMSK0 |= (1<<OCIE0A);						// Output Compare Interrupt enable
		OCR0A = 5;									// Time base

		sei();
	}



	int8_t EVENT(void){
		int8_t result;

		stopEvent();
		startStopProcedure();

		if((result = servoDrive()) == 1 && !stopFlag){
			servo_pos = _SERVO_MIN; 	// move done, go back
		}
		else if(result == 2 && !stopFlag){
			servo_pos = Params.duration;		//repeat
		}
		return result;
	}


		// Function to set parameters of cradle
		// input - target(range or speed), value from 0-9
		// output: 0 for correct value and -1 for wrong
	int8_t SetParams(svParamsEnum_t cradleParam, uint8_t value){
		int8_t result;
			result = 0;
		if(value >= 1 && value <= 9){
			switch(cradleParam){
			case range:
				Params.duration = map(value, 1 ,9, _SERVO_MIN+100,_SERVO_MAX);
				eeprom_update_duration();
				dLED::print(value); //TODO
				USART_deviceAnswer(PSTR("AT_RANGE="),value);
				break;

			case speed:
				Params.speed = map(value, 1, 9, _SERVO_MAX_DELAY, _SERVO_MIN_DELAY); //invert values
				eeprom_update_speed();
				dLED::print(value);
				USART_deviceAnswer(PSTR("AT_SPD="),value);
				break;
			}
		}
		else result = -1;


		return result;
	}

	uint8_t GetParams(svParamsEnum_t cradleParam){
		uint8_t value = 0;
		switch(cradleParam){
		case range:
			value = map(Params.duration, _SERVO_MIN, _SERVO_MAX, 1, 9);
			break;

		case speed:
			value = map(Params.speed, _SERVO_MAX_DELAY, _SERVO_MIN_DELAY, 1, 9);
			break;
		}

		return value;
	}






}

//ISR - time base for servo speed control
ISR(TIMER0_COMPA_vect){
	uint16_t n;
	n = Cradle::Timer;
	if (n) Cradle::Timer = --n;
}
