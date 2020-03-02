/*
 * AT_commands.cpp
 *
 *  Created on: 1 mar 2020
 *      Author: norbe
 */



#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include <string.h>

#include "AT_commands.h"
#include "UARTlib/uart.h"
#include "cradle.h"
#include "texts.h"

#define AT_CNT 4






const atcmd_t atCommands[AT_CNT] PROGMEM{
	{"AT", 	at_service},
	{"ATI", ati_service},
	{"AT+SPD", at_spd_service},
	{"AT+DUR", at_dur_service}
};



void parse_uart_data(char *pbuf){
	atresult_t (*at_srv) (uint8_t inout, char *data);
	uint8_t len,i = 0;
	char *cmd_str;
	char *rest;


	if(strpbrk(pbuf,"=?")){
		if(strpbrk(pbuf,"?")){
			//device ask handle
			cmd_str = strtok_r(pbuf,"?",&rest);
			len = strlen(cmd_str);
			for(i = 0; i < AT_CNT; i++){
				if(len && 0 == strncasecmp_P(cmd_str,atCommands[i].AT_command,len)){
					if(pgm_read_word(atCommands[i].at_service)){
						at_srv = reinterpret_cast<atresult_t (*)(uint8_t, char*)>(pgm_read_word( &atCommands[i].at_service ));
						if(at_srv) at_srv(0,rest);
					}
					USART_PutStr_P(_endl);
					break;
				}
			}

		}
		else{
			// AT+CMD = parameters
			cmd_str = strtok_r(pbuf,"=", &rest);
			len = strlen(cmd_str);
			for(i = 0; i<AT_CNT; i++){
				if(len && 0 == strncasecmp_P(cmd_str,atCommands[i].AT_command,len)){
					if(pgm_read_word(atCommands[i].AT_command)){
						at_srv = reinterpret_cast<atresult_t (*)(uint8_t, char*)>(pgm_read_word( &atCommands[i].at_service ));
						if(at_srv && !at_srv(1,rest)) USART_PutStr_P(_OK);
						else USART_PutStr_P(_errorCmd);
					}
					break;
				}
			}
		}
	}
	else{
		//no parameters
		if(0 == pbuf[0]) USART_PutStr_P(_endl);
		else{
			for(i = 0; i < AT_CNT; i++){
				if(0 == strncasecmp_P(pbuf,atCommands[i].AT_command,strlen(pbuf))){
					if(pgm_read_word(atCommands[i].AT_command) ){
						at_srv = reinterpret_cast<atresult_t (*)(uint8_t, char*)>(pgm_read_word( &atCommands[i].at_service));
						if(at_srv) at_srv(2,0);
					}
					break;
				}
			}
		}
	}
	if(AT_CNT == i) USART_PutStr_P(_unknownCmd);
}

atresult_t at_service(uint8_t inout, char *params){
	USART_PutStr_P(_OK);
	return 0;
}
atresult_t ati_service(uint8_t inout, char *params){
	USART_PutStr_P(_version);
	return 0;
}

atresult_t at_spd_service(uint8_t inout, char *params){
	return 0;
}
atresult_t at_dur_service(uint8_t inout, char *params){
	return 0;
}



