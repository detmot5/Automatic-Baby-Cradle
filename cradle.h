/*
 * cradle.h
 *
 *  Created on: 27 lut 2020
 *      Author: norbe
 */

#ifndef CRADLE_H_
#define CRADLE_H_

//--------------------------------------------------------------------------
//						    LIBRARY SETTINGS
//--------------------------------------------------------------------------

#define _SERVO_MIN						800						// Min value of PWM duration
#define _SERVO_MAX						2200					// Max value of PWM duration

#define SERVO_OUT DDRB |= (1<<PB1);						// OCR1 pin out (atmega88)

#define servoWrite(_dur)	OCR1A = usToTicks(_dur)

//--------------------------------------------------------------------------
//						 SERVO CONTROL DEFINES
//--------------------------------------------------------------------------

#define _SERVO_NEUTRAL 					(_SERVO_MIN + _SERVO_MAX)/2

#define _TICKS_PER_US					F_CPU/1000000UL
#define _SERVO_REFRESH_INTERVAL 		19999

#define usToTicks(_us)  ((_TICKS_PER_US * _us)/8)





/*

class Servo{
public:

	static void SetDuration(uint16_t duration){
		_duration = duration;
	}
	static void SetSpeed(uint8_t speed){
		_speed = speed;
	}

	static uint16_t GetDuration(void){
		return _duration;
	}
	static uint8_t GetSpeed(void){
		return _speed;
	}

private:
	static uint16_t _duration;
	static uint8_t _speed;


};

*/


//--------------------------------------------------------------------------
//					DECLARATIONS OF FUNCTIONS AND VARIABLES
//--------------------------------------------------------------------------

extern volatile uint16_t Timer1,Timer2,Timer3;

void timersInit(void);

int8_t servo_event(uint16_t angle, uint8_t speed);


#endif /* CRADLE_H_ */
