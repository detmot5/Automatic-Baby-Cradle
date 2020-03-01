/*
 * cradle.h
 *
 *  Created on: 27 lut 2020
 *      Author: norbe
 */

#ifndef CRADLE_H_
#define CRADLE_H_


typedef int8_t ServoMvRes_t;

extern volatile uint16_t Timer1,Timer2,Timer3;
extern uint16_t servo_pos, servo_speed;
void softTimInit(void);

ServoMvRes_t servo_event(uint16_t angle, uint8_t speed);
void servoSet(uint16_t duration, uint8_t speed);


#endif /* CRADLE_H_ */
