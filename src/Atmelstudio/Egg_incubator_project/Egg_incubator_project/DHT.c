/*
 * DHT.c
 *
 * Created: 19/04/2024 15:43:23
 *  Author: Zinj
 */ 

#define F_CPU 16000000UL

#define DHT11_PIN PINA0

#include "DHT.h"

uint8_t data[5];
float temp_f;

void DHT11_start(){
	DDRA |= (1<<DHT11_PIN);
	PORTA &= ~(1<<DHT11_PIN);	/* set to low pin */
	_delay_ms(20);			/* wait for 20ms */
	PORTA |= (1<<DHT11_PIN);	/* set to high pin */
}

void DHT11_response(){
	DDRA &= ~(1<<DHT11_PIN);
	while(PINA & (1<<DHT11_PIN));
	while((PINA & (1<<DHT11_PIN))==0);
	while(PINA & (1<<DHT11_PIN));
}

uint8_t DHT11_read_byte(){
	uint8_t data = 0;
	for(int i=0;i<8;i++){
		while((PINA & (1<<DHT11_PIN)) == 0);  /* check received bit 0 or 1 */
		_delay_us(30); //dht holds high 28us for 0
		if(PINA & (1<<DHT11_PIN)){  //dht high
			data = ((data<<1) | 1);
		}
		else{
			data = (data<<1);
		}
		while(PINA & (1<<DHT11_PIN));
	}
	return data;
}

uint8_t DHT11_read_data(char* hum, char* temp){
	uint8_t checksum = 0;
	DHT11_start();
	DHT11_response();
	data[0] = DHT11_read_byte();   //humedad int
	data[1] = DHT11_read_byte();   //humedad dec
	data[2] = DHT11_read_byte();   //temp int
	data[3] = DHT11_read_byte();   //temp dec
	data[4] = DHT11_read_byte();   //checksum
	checksum = data[0] + data[1] + data[2] + data[3];
	//end listening
	_delay_ms(10);
	DDRA |= 1<<DHT11_PIN;
	PORTA |= 1<<DHT11_PIN;
	
	if (checksum == data[4]){
		sprintf(hum, "%2d",data[0]);
		sprintf(temp, "%2d.%1d",data[2],data[3]);
		//sprintf(hum, "%2d.%1d",data[0],data[1]);
		//sprintf(temp, "%2d.%1d",data[2],data[3]);
		temp_f = (float)data[2];
		return 1;
	}
	else{
		return 0;
	}
}