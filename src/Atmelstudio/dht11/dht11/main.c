/*
 * Egg_incubator_project.cpp
 *
 * Created: 18/04/2024 10:42:52
 * Author : Zinj.PQ
 */ 

#include "MYLIB.h"
#include "DHT.h"

//button configuration
ISR(INT0_vect) {
	if (PIND & (1 << PD2)) { // Check bit PD2
		temp_set++; // Increase temp_set variable
	}
}

ISR(INT1_vect) {
	if (PIND & (1 << PD3)) { // Check bit PD3
		temp_set--; // Reduce temp_set variable
	}
}

int main(void){
	// LCD setup
	DDRC |= (1 << LCD_RS) | (1 << LCD_E) | (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);
	lcd_init();
	
	//---------------------------------------------------------------------------------------------------//
	
	// Configure interrupt pins
	DDRD &= ~(1 << PD2); // PD2 is the INT0 interrupt pin
	DDRD &= ~(1 << PD3); // PD3 is the INT1 interrupt pin

	// Configure interrupt settings
	sei(); // Enable global interrupts

	// Configure peripheral interrupt settings
	GICR |= (1 << INT0) | (1 << INT1); // Use peripheral interrupts INT0 and INT1
	MCUCR = (1 << ISC00) | (1 << ISC01) | (1 << ISC10) | (1 << ISC11); // Configure interrupt edges for INT0 and INT1	
	
	//----------------------------------------------------------------------------------------------------//
	
	/*Fast PWM, On-Inverting, 1:1 PreScaler*/
	TCCR0 = (1 << WGM01) |(1 << WGM00) |(1 << COM01) |(1 << CS00);
	
	//----------------------------------------------------------------------------------------------------//
	
	//Variables
	double temperature[1];
	double humidity[1];

	//Setup
	DHT_Setup();
	
	
    while (1) {
	    //Read from sensor
	    DHT_Read( temperature, humidity);
	    
	    //Check status
	    switch (DHT_GetStatus())
	    {
		    case (DHT_Ok):
		    //Print temperature
		    //print(temperature[0]);
		    //updateDisplay((int)temperature, temp_set);
		    //Print humidity
		    //print(humidity[0]);
		    break;
		    case (DHT_Error_Checksum):
		    //Do something
		    break;
		    case (DHT_Error_Timeout):
		    //Do something else
		    break;
	    }
	    
	    //Sensor needs 1-2s to stabilize its readings
	    _delay_ms(1000);
	    //updateDisplay((int)I_Temp, temp_set);
    }
}

