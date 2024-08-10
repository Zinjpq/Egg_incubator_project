/*
 * CFile1.c
 *
 * Created: 23/6/2022 23:41:09
 *  Author: Fran
 */ 
#include "UART.h"
#include "dht.h"
#include "menu.h"

static volatile uint8_t BUFFER_RX[RX_BUFFER_LENGHT]; //Buffer para la recepcion
static volatile uint8_t BUFFER_TX[TX_BUFFER_LENGHT]; //Buffer para la transmision
static volatile uint8_t indiceTX_escritura= 0; //Indice de escritura para el buffer de transmision
static volatile uint8_t indiceTX_lectura= 0; //Indice de lectura para el buffer de transmision
static volatile uint8_t se_apreto_enter= 0; //Flag de si el usuario apreto ENTER
volatile unsigned char flag_hora = 0;
static unsigned char cont_hora = 0;
char hum [5];
char temp [5];


void UART_init(uint8_t config)
{
	//Configura la UART 9600bps, 8 bit data
	UCSR0B = 0;
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	UBRR0H = (unsigned char)(config>>8);
	UBRR0L = (unsigned char)config;
	UCSR0B |= (1<<TXEN0); //Activa la UART para transmision
	UCSR0B |= (1<<RXEN0); //Activa la UART para recepcion
	UCSR0B |= (1<<RXCIE0); //Activa las interrupciones de recepcion
	TCCR1B |= (1 << WGM12);				// Modo CTC con OCR1A
	TCCR1B |= (1 << CS12);				// Prescaler = F_CPU/256
	OCR1A = 3125;						// seteo el contador en 3125
	TIMSK1 |= (1 << OCIE1A);			// habilito interrupci n del contador OCR1A
	sei();
}

void UART_TX_Interrupt_Enable(void) //Funcion que activa las interrupciones de transmision de la UART
{
	UCSR0B |= (1<<UDRIE0);
}

void UART_TX_Interrupt_Disable(void) //Funcion que desactiva las interrupciones de transmision de la UART
{
	UCSR0B &=~(1<<UDRIE0);
}

void UART_write_char_buffer(uint8_t caracter) //Funcion que recibe un caracter para escribir en el buffer de transmision
{
	if ((indiceTX_escritura+1)%TX_BUFFER_LENGHT!=indiceTX_lectura) //Si el indice de escritura no se paso del tamaño del buffer
	{
		BUFFER_TX[indiceTX_escritura]= caracter; //Guarda el caracter en el buffer
		indiceTX_escritura= (indiceTX_escritura+1)%(TX_BUFFER_LENGHT); //Incrementa el indice o resetea en el caso de llegar a la ultima posicion
	}
	else
	{
		//ERROR el buffer esta lleno
	}
}

void UART_write_string_buffer(uint8_t * cadena)
{
	uint8_t i;
	for (i=0;cadena[i]!='\0';i++)//Itera la cadena caracter a caracter
	{
		UART_write_char_buffer(cadena[i]); //Llama a la funcion que escribe un caracter mandando el caracter
	}
}

void UART_get_string_from_buffer(uint8_t * cadena) //Funcion que retorna la cadena leida en la variable cadena
{
	uint8_t i;
	for (i=0;BUFFER_RX[i]!='\0';i++) //Itera el buffer de recepcion hasta que encuentre un valor vacio ('\0')
	{
		cadena[i]= BUFFER_RX[i]; //Guarda caracter a caracter en la variable cadena
	}
}

void SEOS_Schedule_Tasks(void)
{
	if (++cont_hora == 20) { // Cada 1 segundo
		flag_hora = 1;
		cont_hora = 0;
	}
}


void SEOS_Dispatch_Tasks (void) {
	if ((flag_hora)&&(imprimo_flag())) {
		DHT11_read_data(hum,temp);
		UART_write_string_buffer((uint8_t*)hum);
		UART_write_string_buffer("\n\r");
		UART_write_string_buffer((uint8_t*)temp);
		UART_write_string_buffer("\n\r");
		UART_TX_Interrupt_Enable();
		flag_hora = 0;
	}
}

ISR(USART_RX_vect){
	static uint8_t i = 0;
	BUFFER_RX[i] = UDR0;
	if (BUFFER_RX[i]=='\r') //Si el valor guardado es un ENTER
	{
		BUFFER_RX[i]= '\0'; //Remplaza el valor por un vacio '\0'
		se_apreto_enter= 1; //Activa el flag de que se apreto ENTER
		i=0; //Resetea la variable de iteracion
	}else
	{		if (i==(RX_BUFFER_LENGHT-1)) //Si i llego a la posicion maxima del buffer
		{
			i=0; //Resetea el valor
			BUFFER_RX[i]= 'a';
		}
		i++;
	}
}

ISR(USART_UDRE_vect) //Interrupcion de que se puede transmitir en la UART
{
	if (indiceTX_lectura!=indiceTX_escritura) //Si tengo datos para transmitir
	{
		UDR0= BUFFER_TX[indiceTX_lectura]; //Transmito un caracter del buffer
		indiceTX_lectura= (indiceTX_lectura+1)%(TX_BUFFER_LENGHT-1);
		//Incremento en uno el valor del indice de lectura o lo reseteo si llego al limite
	}else //Si no hay mas nada para transmitir
	{
		UART_TX_Interrupt_Disable(); //Desactivo las interrupciones de transmision
	}
}

ISR (TIMER1_COMPA_vect)
{
	SEOS_Schedule_Tasks();
}

uint8_t get_se_apreto_enter(void) //Esta funcion devuelve el valor del flag de si se apreto ENTER
{
	return se_apreto_enter;
}
void set_se_apreto_enter(uint8_t valor) //Esta funcion setea el valor del flag de si se apreto ENTER por el valor pasado en la funcion
{
	se_apreto_enter= valor;
}