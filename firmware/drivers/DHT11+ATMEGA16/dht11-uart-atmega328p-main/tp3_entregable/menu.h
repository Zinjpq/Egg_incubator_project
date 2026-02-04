/*
 * IncFile1.h
 *
 * Created: 23/6/2022 23:48:35
 *  Author: Fran
 */ 


#ifndef MENU_H_
#define MENU_H_

	#include "stdint.h"
	#include "string.h"
	#include <avr/io.h>
	#include "dht.h"
	#include "UART.h"
	
	void menu_show(void);
	void menu_update(void);
	void set_hay_para_transmitir(uint8_t valor);
	uint8_t get_hay_para_transmitir(void);
	uint8_t imprimo_flag(void);

#endif