#ifndef DHT_H
#define DHT_H
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t DHT11_read_data(char*, char*);
uint8_t DHT11_read_byte();
void DHT11_response();
void DHT11_start();

#endif