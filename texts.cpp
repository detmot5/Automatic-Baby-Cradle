/*
 * texts.cpp
 *
 *  Created on: 2 mar 2020
 *      Author: norbe
 */



#include <avr/pgmspace.h>
#include "texts.h"

const char _endl[] PROGMEM = "\r\n";

const char _OK[] PROGMEM = "OK ";
const char _errorCmd[] PROGMEM = "ERROR ";
const char _unknownCmd[] PROGMEM = "ERROR: unknown cmd ";



const char _version[] PROGMEM = "Automatic Baby Cradle v1.0 ";

const char _atSpd[] PROGMEM = "Use AT+SPD=spd ";
const char _atDur[] PROGMEM = "Use AT+DUR=dur ";
const char _deviceStopped[] PROGMEM = "Device stopped ";


