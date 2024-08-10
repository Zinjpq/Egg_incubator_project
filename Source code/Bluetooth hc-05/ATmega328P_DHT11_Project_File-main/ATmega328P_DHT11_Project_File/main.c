/*
 * ATmega328P_DHT11_Project_File.c
 *
 * Created: 25/06/2020 21:30:53
 * Author : bee
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART_M328P.h"
#define  DHT11_PIN PC0
uint8_t c=0,I_RH,D_RH,I_Temp,D_Temp,CheckSum,StrRx_flag;
char buffer[10];


ISR(USART_RX_vect)
{
	buffer[10]=UDR0;         //Read USART data register
	StrRx_flag=1;        //Set String received flag
	
}//end ISR USART

void Request()						/* Microcontroller send start pulse or request */
{
	DDRC |= (1<<DHT11_PIN);
	PORTC &= ~(1<<DHT11_PIN);		/* set to low pin */
	_delay_ms(20);					/* wait for 20ms */
	PORTC |= (1<<DHT11_PIN);		/* set to high pin */
	
}

void Response()						/* receive response from DHT11 */
{
	DDRC &= ~(1<<DHT11_PIN);
	while(PINC & (1<<DHT11_PIN));
	while((PINC & (1<<DHT11_PIN))==0);
	while(PINC & (1<<DHT11_PIN));

}

uint8_t Receive_data()							/* receive data */
{
	for (int q=0; q<8; q++)
	{
		while((PINC & (1<<DHT11_PIN)) == 0);	/* check received bit 0 or 1 */
		_delay_ms(30);
		if(PINC & (1<<DHT11_PIN))				/* if high pulse is greater than 30 ms */
		{c = (c<<1)|(0x01);}					/* then its logic HIGH */
		else		
		{c = (c<<1);}							/* otherwise its logic LOW */
		while(PINC & (1<<DHT11_PIN));
	}
	USART_putstring(c);
	return c;
	
}
int main(void)
{
	char data[5];
	USART0Init();					/* initialize USART in microcontroller */
	sei();							/* enable global interrupt */
	
    while (1) 
    {
		USART_putstring("Humidity =");	/* send string to bluetooth */
		
		Request();					/* send start pulse */
		Response();					/* receive response */
		I_RH=Receive_data();		/* store first eight bit in I_RH */
		D_RH=Receive_data();		/* store next eight bit in D_RH */
		I_Temp=Receive_data();		/* store next eight bit in I_Temp */
		D_Temp=Receive_data();		/* store next eight bit in D_Temp */
		CheckSum=Receive_data();	/* store next eight bit in CheckSum */

		if ((I_RH + D_RH + I_Temp) != CheckSum)
		{
			USART_putstring("Error");
		}

		else
		{
			itoa(I_RH,data,10);
			USART_putstring(data);
			USART_putstring(".");
			
			itoa(D_RH,data,10);
			USART_putstring(data);
			USART_putstring("%");
			
			itoa(I_Temp,data,10);
			USART_putstring(data);
			USART_putstring(".");
			
			itoa(D_Temp,data,10);
			USART_putstring(data);
			USART_putstring("C ");
			
			itoa(CheckSum,data,10);
			USART_putstring(data);
			USART_putstring(" ");
		}

	_delay_ms(500);
    
	}// end while
	
} // end main

