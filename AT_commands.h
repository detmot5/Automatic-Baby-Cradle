/*
 * AT_commands.h
 *
 *  Created on: 1 mar 2020
 *      Author: norbe
 */

#ifndef AT_COMMANDS_H_
#define AT_COMMANDS_H_


typedef int8_t atresult_t;

typedef struct{
	char AT_command[10];
	atresult_t (* at_service)(uint8_t inout, char *params);
}atcmd_t;


extern const atcmd_t atCommands[] PROGMEM;

void parse_uart_data(char *pbuf);

atresult_t at_service(uint8_t inout, char *params);
atresult_t ati_service(uint8_t inout, char *params);
atresult_t at_spd_service(uint8_t inout, char *params);
atresult_t at_dur_service(uint8_t inout, char *params);


#endif /* AT_COMMANDS_H_ */
