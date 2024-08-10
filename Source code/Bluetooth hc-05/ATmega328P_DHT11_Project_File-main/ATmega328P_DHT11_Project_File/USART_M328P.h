/*
 * USART_M328P.h
 *
 * Created: 23/04/2017 21:26:16
 *  Author: Dumitru
 */ 



#ifndef USART_M328P_H_
#define USART_M328P_H_
void USART0Init(void);
void USART0SendByte(int u8Data);
int USART0ReceiveByte();
void USART_putstring(char* StringPtr);
void USART_send_char( char  data);


#endif /* USART_M328P_H_ */


//if (StrRx_flag==1){
//	StrRx_flag=0;
//	PORTB |= (1 << PINB4); // PD4 goes high
//	_delay_ms(500);
//	PORTB &= ~(1 << PINB4); // PD0 goes low
//}

