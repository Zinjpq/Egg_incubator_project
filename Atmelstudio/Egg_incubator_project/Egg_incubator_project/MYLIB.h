/*
 * mylib.h
 *
 * Created: 18/04/2024 10:46:47
 *  Author: Zinj
 */ 


#ifndef MYLIB_H_
#define MYLIB_H_

#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h> //Interrupt library
#include "MILLIS.h"

// LCD pin definition
#define LCD_RS		PC2
#define LCD_E		PC3
#define LCD_D4		PC4
#define LCD_D5		PC5
#define LCD_D6		PC6
#define LCD_D7		PC7

#define FAN PB2

int temp_set = 0; // Variable temp_set is used in ISR
int current_time;
int prev_time;

//Variables
char hum [5];
char temp [5];

float temp_f;

void lcd_command(uint8_t cmd) {
	PORTC = (cmd & 0xF0) | (1 << LCD_E);
	PORTC &= ~(1 << LCD_RS);
	_delay_ms(1);
	PORTC &= ~(1 << LCD_E);
	_delay_ms(1);

	PORTC = ((cmd & 0x0F) << 4) | (1 << LCD_E);
	PORTC &= ~(1 << LCD_RS);
	_delay_ms(1);
	PORTC &= ~(1 << LCD_E);
	_delay_ms(1);
}

void lcd_init(){
	_delay_ms(20);  /* LCD Power ON Initialization time >15ms */
	lcd_command (0x02); /* 4bit mode */
	lcd_command (0x28); /* Initialization of 16X2 LCD in 4bit mode */
	lcd_command (0x0C); /* Display ON Cursor OFF */
	lcd_command (0x06); /* Auto Increment cursor */
	lcd_command (0x01); /* Clear display */
	lcd_command (0x80); /* Cursor at home position */
}

void lcd_data(uint8_t data) {
	PORTC = (data & 0xF0) | (1 << LCD_E) | (1 << LCD_RS);
	_delay_ms(1);
	PORTC &= ~(1 << LCD_E);
	_delay_ms(1);

	PORTC = ((data & 0x0F) << 4) | (1 << LCD_E) | (1 << LCD_RS);
	_delay_ms(1);
	PORTC &= ~(1 << LCD_E);
	_delay_ms(1);
}

void lcd_string(const char* str) {
	int i = 0;
	while (str[i] != '\0') {
		lcd_data(str[i]);
		i++;
	}
}

void lcd_string_with_delay(const char* str ) {
	//clear screen
	lcd_command(0x01);
	int i = 0;
	while (str[i] != '\0') {
		if (i == 16) {
			lcd_command(0xC0);  // Move the cursor down to the second line
		}
		lcd_data(str[i]);
		i++;
		_delay_ms(300);
	}
}

void lcd_float(float number) {
	char str[10];
	char str_i[10];
	dtostrf(number, 2, 2, str_i);
	snprintf(str, sizeof(str), "%s", str_i);
	lcd_string(str);
}

void lcd_clear() {
	lcd_command(0x01);  // Clear LCD screen
}

void hello(){
	//Greeting
	lcd_string_with_delay("    Group 4     EMA3028 20 2024");
	_delay_ms(1000);
	lcd_string_with_delay(" The Incubator");
	_delay_ms(1000);
	lcd_command(0x01);  // Clear display
}

void updateDisplay(int *temp,int *humi, int temp_set) {
	//lcd_clear(); // Clear only if necessary
	lcd_command(0x80);
	lcd_string("T:");
	lcd_string (temp);
	lcd_string("*C");
	lcd_string(" H:");
	lcd_string (humi);
	lcd_string("%");
	lcd_command(0xC0); // Move cursor to second line

	lcd_string("Set up: ");
	lcd_float(temp_set);
	lcd_string("*C");
}

void led_control(float temp){
	current_time = millis();
	if (current_time - prev_time > 100) {
		int pwm=OCR0;
		// Adjust pulse according to temperature
		if (temp > temp_set) {
			pwm -= 5;
			} else if (temp < temp_set) {
			pwm += 5;
		}
		prev_time=current_time;
		// reset
		if (pwm < 0) {
			pwm = 0;
			} else if (pwm > 255) {
			pwm = 255;
		}
		//Repeat for OCR0 to adjust the pulse
		OCR0=pwm;
		_delay_ms(50);
	}
}

void fan_control(float temp) {
	if (temp >= temp_set) {
		PORTB |= (1 << FAN); // Turn fan on
		} else {
		PORTB &= ~(1 << FAN); // Turn fan off
	}
}

#endif /* MYLIB_H_ */