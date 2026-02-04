/*
 * Egg_incubator_project.c
 *
 * Created: 18/04/2024 10:42:52
 * Author : Zinj.PQ
 */ 

#include "MYLIB.h"
#include "DHT.h"

//button configuration
ISR(INT0_vect) {
	if (PIND & (1 << 2)) { // Check bit PD2
		temp_set++; // Increase temp_set variable
	}
}

ISR(INT1_vect) {
	if (PIND & (1 << 3)) { // Check bit PD3
		temp_set--; // Reduce temp_set variable
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
	
	/*Fast PWM, On-Inverting, 1:1 PreScaler*/
	TCCR0 = (1 << WGM01) |(1 << WGM00) |(1 << COM01) |(1 << CS00);
	OCR0 = 30;
	
	//----------------------------------------------------------------------------------------------------//
	
	//Setup
	DHT11_start();
	
	//hello();
    while (1) {
		//Read from sensor
		DHT11_read_data(hum,temp);
		//lcd_string(temp);
		updateDisplay(temp, hum, temp_set);
		//Sensor needs 1-2s to stabilize its readings
		_delay_ms(1000);
		fan_control(temp_f);
		led_control(temp_f);
    }
}

