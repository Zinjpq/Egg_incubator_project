/*
 * USART_M328P.c
 *
 * Created: 23/04/2017 21:15:03
 *  Author: Dumitru
 */
//#include "USART_M328P.h"
#include <avr/io.h>
#include <util/delay.h>

#define USART_BAUDRATE 9600//38400
#define F_CPU 16000000UL  // 16 MHz
#define UBRR_VALUE (((F_CPU / (9600 * 16UL))) - 1)

 
void USART0Init(void)
{
	// Set baud rate
	UBRR0H = (uint8_t)(UBRR_VALUE>>8);
	UBRR0L = (uint8_t)UBRR_VALUE;
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	//enable transmission and reception
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
	// Enable the USART Receive interrupt
	UCSR0B |= (1 << RXCIE0 );
}



void USART0SendByte(int u8Data)
{
	//wait while previous byte is completed
	while(!(UCSR0A&(1<<UDRE0))){};
	// Transmit data
	UDR0 = u8Data;
}
int USART0ReceiveByte()
{
	// Wait for byte to be received
	while(!(UCSR0A&(1<<RXC0))){};
	// Return received data
	return UDR0;
}

void USART_putstring(char* StringPtr){
	
	while(*StringPtr != 0x00){
		USART0SendByte(*StringPtr);
		
	StringPtr++;
	
	}
	
	
}
void USART_send_char( char  data)
{
	//while the transmit buffer is not empty loop
	while(!(UCSR0A & (1<<UDRE0)));
	
	//when the buffer is empty write data to the transmitted
	UDR0 = data;
}