# Automatic-Baby-Cradle

This is my second project. I do it with knowledge about lot's of mistakes that I made in the previous one, like RS232 transmission.
> Hardware ATmega88

> Compiler avr-g++


### This is a device which is a dream come true for all parents ;) 

It is an attachment to baby cradle which can rock it automatically using servo motor. User will be able to control it by Android app via Bluetooth or using rotary enconder and LED display.

There is few parameters which can be controlled:
 1. Speed
 2. Move range of servo
 3. Rocking Time
 
 > Device can also be stopped using single button any time.



In this project I implemented AT_comannds handling using events and callbacks, the explanation of it is given below.

* AT - Standard command. When sent, device answers "OK"
* ATI - When sent device prints us information about it.
* AT+RST - Reset device
* AT+SPD - Changing speed of rocking
* AT+DUR - Changing range of rocking 
* AT+STOP - Stops device immediatelly 
* AT+TIM - Turns on the cradle for a specified time
* AT+FAC - Restore factory settings




