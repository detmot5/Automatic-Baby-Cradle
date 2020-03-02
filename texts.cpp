/*
 * texts.cpp
 *
 *  Created on: 2 mar 2020
 *      Author: norbe
 */



#include <avr/pgmspace.h>
#include "texts.h"

const char _endl[] PROGMEM = "\r\n";

const char _OK[] PROGMEM = "OK \r\n";
const char _errorCmd[] PROGMEM = "ERROR\r\n";
const char _unknownCmd[] PROGMEM = "ERROR - unknown command\r\n";



const char _version[] PROGMEM = "Automatic Baby Cradle v1.0\r\n";


