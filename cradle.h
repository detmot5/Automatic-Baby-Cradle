/*
 * cradle.h
 *
 *  Created on: 27 lut 2020
 *      Author: norbe
 */

#ifndef CRADLE_H_
#define CRADLE_H_


typedef uint8_t ServoMvRes_t;

extern volatile uint16_t Timer1,Timer2,Timer3;
extern uint16_t servo_pos, servo_speed;
void softTimInit(void);
ServoMvRes_t servoEvent(void);
#endif /* CRADLE_H_ */
