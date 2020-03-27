/* C++ Library for comfortable using microswitches
 *
 * The Event method is not created by me,
 * so out of respect for author I moved it to another file
 * which is in .gitignore
 *
 * You can put there your own one.
 * It have to do some debounce and call callback functions.
 *
 * In first object constructor an reference to variable which decrements every 10ms must be passed
 *
 *
 *
 * mSwitch.h
 *
 *  Created on: 27 Mar 2020
 *      Author: Norbert Bielak
 */

#ifndef MICROSWITCH_MICROSWITCH_H_
#define MICROSWITCH_MICROSWITCH_H_

#define DEFAULT_HOLD_TIME 100		// 150 of 10ms ticks - 1500ms
#define DEFAULT_REP_TIME 20			// 20 of 10ms ticks - 200ms


class MicroSwitch{
public:
	MicroSwitch(volatile uint8_t* _PINx, uint8_t _pin, volatile uint32_t *timeBase = 0,
			uint8_t _holdTime = DEFAULT_HOLD_TIME, uint8_t _holdRepeatTime = DEFAULT_REP_TIME);

	void registerPressEventCallback(void (*callback)());
	void registerHoldEventCallback(void (*callback)());
	void Event(void);


private:
	static volatile uint32_t *timeBase10Ms;
	volatile uint8_t *PINx;
	uint8_t pin;
	uint8_t pinMask;
	uint8_t holdTime;
	uint8_t holdRepeatTime;
	void (*PressEventCallback)();
	void (*HoldEventCallback)();

};


#endif /* MICROSWITCH_MICROSWITCH_H_ */
