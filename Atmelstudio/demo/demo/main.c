/*
 * demo.c
 *
 * Created: 24/04/2024 15:11:28
 * Author : Zinj
 */ 
//Headers
#include "MYLIB.h"
#include <util/delay.h>
#include "DHT.h"

#define F_CPU 16000000UL

int main(void)
{
	DDRC |= (1 << LCD_RS) | (1 << LCD_E) | (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);
	lcd_init();
	

	//Variables
	double temperature[1];
	double humidity[1];

	//Setup
	DHT_Setup();
	
	//Loop
	while (1 == 1)
	{
		//Read from sensor
		DHT_Read(temperature, humidity);
		
		//Check status
		switch (DHT_GetStatus())
		{
			case (DHT_Ok):
			//Print temperature
			//print(temperature[0]);
			updateDisplay((int)temperature, temp_set);
			//Print humidity
			//print(humidity[0]);
			break;
			case (DHT_Error_Checksum):
			//Do something
			lcd_string("DHT_Error_Checksum");
			break;
			case (DHT_Error_Timeout):
			//Do something else
			lcd_string("DHT_Error_Timeout");
			break;
		}
		
		//Sensor needs 1-2s to stabilize its readings
		_delay_ms(1000);
	}
	
	return 0;
}

