/*
 * bluetooth.h
 *biblioteka do oblugi interfejsu bluetooth
 *  Author: Artur Hadasz
 */ 

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

void bluetooth_init();
void send_char(char c);
void send_uint16(uint16_t i);
char receiveByte();



#endif /* BLUETOOTH_H_ */