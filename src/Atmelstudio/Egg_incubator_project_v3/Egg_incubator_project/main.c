/*
 * Egg_incubator_project.c
 *
 * Created: 18/04/2024 10:42:52
 * Author : Zinj.PQ
 */ 

#define F_CPU 8000000UL   //set clock 8MHZ
#include "MYLIB.h"
#include "DHT.h"
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)  //define baud

//UART functions
char dat;

void UART_init(unsigned long USART_BAUDRATE)
{
	sei();
	// Enable transmitter and receiver
	UCSRB |= (1 << RXEN) | (1 << TXEN) | (1<<RXCIE);

	// Set 8-bit data transfer mode
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	UCSRB &= ~(1 << UCSZ2);

	// Set baud rate
	UBRRL = BAUD_PRESCALE;
	UBRRH = (BAUD_PRESCALE >> 8);
}
unsigned char UART_RXchar(void)
{
	while ((UCSRA & (1<<RXC))== 0 ); //wait till flag RX complete is 1
	return UDR;
}

void UART_TxChar(char ch)
{
	while (! (UCSRA & (1<<UDRE)));    /* Wait for empty transmit buffer*/
	UDR = ch ;
}

void UART_SendString(char *str)
{
	unsigned char j=0;
	
	while (str[j]!=0)   	 /* Send string till null */
	{
		UART_TxChar(str[j]);
		j++;
	}
}

ISR(USART_RXC_vect)
{
	dat = UDR;
}


//button configuration
ISR(INT0_vect) {
	if (PIND & (1 << 2)) { // Check bit PD2
		temp_set++; // Increase temp_set variable
		_delay_ms(2);
		//chong rung phim
		while ((PIND & 0x04)==0)
		{}
		_delay_ms(2);
		//xoa co ngat
		//GIFR |= (1 << INTF0);
	}
}

ISR(INT1_vect) {
	if (PIND & (1 << 3)) { // Check bit PD3
		temp_set--; // Reduce temp_set variable
		//chong rung phim
		_delay_ms(2);
		while ((PIND & 0x08)==0)
		{}
		_delay_ms(2);
		//xoa co ngat
		//GIFR |= (1 << INTF1);
	}
}

int main(void){
	// LCD setup
	DDRC |= (1 << LCD_RS) | (1 << LCD_E) | (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);
	lcd_init();
	
	//---------------------------------------------------------------------------------------------------//
	
	// Configure interrupt pins
	DDRD &= ~(1 << 2); // PD2 is the INT0 interrupt pin
	DDRD &= ~(1 << 3); // PD3 is the INT1 interrupt pin

	// Configure interrupt settings
	sei(); // Enable global interrupts

	// Configure peripheral interrupt settings
	GICR |= (1 << INT0) | (1 << INT1); // Use peripheral interrupts INT0 and INT1
	MCUCR = (1 << ISC00) | (1 << ISC01) | (1 << ISC10) | (1 << ISC11); // Configure interrupt edges for INT0 and INT1	
	
	//----------------------------------------------------------------------------------------------------//
	
	/*Fast PWM, On-Inverting, PreScaler=8*/
	TCCR0 = (1 << WGM01) |(1 << WGM00) |(1 << COM01) |(1 << CS01) |(1 << CS00); //set prescaler cao 1 chut de cho tan so tran khong nhanh qua gay ra tac nghen chuong trinh
	/*OC0 PWM Pin Output*/
	DDRB|=(1<<3);
	/*FAN*/
	DDRB|=(1<<2);
	
	//test
	
	
	//----------------------------------------------------------------------------------------------------//
	
	//Variables
	char hum [5];
	char temp [5];
	//Setup
	DHT11_start();
	//UART
	UART_init(9600);
	char str1[80];
	char str2[80];
	//lcd_string("hello");
	hello();
	OCR0=120; //default duty cycle
	int pwm=OCR0;
	//Sensor needs 1-2s to stabilize its readings
	_delay_ms(1000);
    while (1) {
		//Read from sensor
		DHT11_read_data(hum,temp);
		updateDisplay(temp, hum, temp_set);
		//Sensor needs 1-2s to stabilize its readings
		real_temp=atoi(temp); //chuyen tu char[] sang int
		//epkieu + uart
		sprintf(str1,"%dC",real_temp);
		sprintf(str2,"%dC    ",temp_set);
		if (dat == '1')
		{
		  temp_set++;
		  //UART_TxChar(data);
		  dat='a';
		}
		if (dat == '0')
		{
			temp_set--;
			//UART_TxChar(data);
			dat='b';
		}
		//UART_SendString("\n\t Echo Test ");
		UART_SendString(str1);       	//in temp ra dien thoai
		UART_SendString("|");
		UART_SendString(str2);         //in desire temp dien thoai
		_delay_ms(50);      	
		//fan control
		fan_control(real_temp);
		// dieu chinh xung theo nhiet do
		if (real_temp > temp_set+1) {
			pwm -= 15;
			} else if (real_temp < temp_set) {
			pwm += 15;
		}
		// reset
		if (pwm < 0) {
			pwm = 0;
			} else if (pwm > 255) {
			pwm = 255;
		}
		//gan lai cho OCR0 de dieu chinh xung
		OCR0=pwm;
    }
}

