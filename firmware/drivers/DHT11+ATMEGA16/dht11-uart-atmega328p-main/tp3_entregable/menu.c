#include "menu.h"

static uint8_t msj_menu1[]= "Registrador de temperatura y humedad\n\r";
static uint8_t msj_menu2[]= "Ingrese: ON para encender, OFF para apagar y RST para reiniciar\n\r";
static uint8_t msj_error[]= "Comando no valido\n\r";
static uint8_t hay_para_transmitir = 0;
char hum [5];
char temp [5];
uint8_t deboImprimir = 0;

void menu_show(void){
	UART_write_string_buffer(msj_menu1);
	UART_write_string_buffer(msj_menu2);
	hay_para_transmitir = 1;
}

uint8_t get_hay_para_transmitir(void) //Esta funcion devuelve el valor del flag de si hay cosas para transmitir
{
	return hay_para_transmitir;
}

void set_hay_para_transmitir(uint8_t valor) //Esta funcion setea el valor del flag de si hay cosas para imprimir por el valor pasado en la funcion
{
	hay_para_transmitir = valor;
}

void menu_update(){
	uint8_t cadena[6]="";
	UART_get_string_from_buffer(cadena);
	if (strcmp((const char*)cadena,(const char*)"ON")==0){
		UART_write_string_buffer((uint8_t*)"Encendido\n\r");
		hay_para_transmitir = 1;
		//FUNCION DEL DHT
		deboImprimir=1;
	}
	else {
		if (strcmp((const char*)cadena,(const char*)"OFF")==0){
			deboImprimir=0;
			UART_write_string_buffer((uint8_t*)"Apagado\n\r");
			hay_para_transmitir = 1;
			//FUNCION DEL DHT
		}
		else {
			if (strcmp((const char*)cadena,(const char*)"RST")==0){
				deboImprimir = 0;
				menu_show();
				//FUNCION DEL DHT
			}
			else {
				UART_write_string_buffer(msj_error);
				hay_para_transmitir=1;
			}
		}
	}
}

uint8_t imprimo_flag(){
	return deboImprimir;
}