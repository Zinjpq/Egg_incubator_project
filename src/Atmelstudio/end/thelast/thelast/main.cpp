#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

void adc_init() {
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read(uint8_t adc_port) {
	ADMUX = (1 << REFS0) | adc_port;
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC)) {
		_delay_ms(1);
	}
	return ADCW;
}

void lcd_command(uint8_t cmd) {
	PORTA &= ~(1 << 0);
	PORTD = cmd;
	PORTA |= (1 << 2);
	_delay_ms(1);
	PORTA &= ~(1 << 2);
}

void lcd_data(uint8_t data) {
	PORTA |= (1 << 0);
	PORTD = data;
	PORTA |= (1 << 2);
	_delay_ms(1);
	PORTA &= ~(1 << 2);
}

void lcd_string(const char* str) {
	int i = 0;
	while (str[i] != '\0') {
		lcd_data(str[i]);
		i++;
	}
}

void lcd_float(float number) {
	char str[6];
	snprintf(str, sizeof(str), "%.2f", number);
	lcd_string(str);
}

int main(void) {
	DDRA |= (1 << DDA0) | (1 << DDA1) | (1 << DDA2);
	DDRD = 0xFF;
	DDRB = 0xFF;
	DDRC = 0x00;

	int num = 0;
	int y = 35;
	adc_init();

	while (1) {
		lcd_command(0x38);
		lcd_command(0x0C);
		lcd_command(0x80);

		num = adc_read(1) / 3.41;
		lcd_string("Temp: ");
		lcd_float(num);

		lcd_command(0xC0);
		lcd_string("Temp want: ");
		lcd_float(y);

		if (PINC & (1 << 0)) {
			_delay_ms(20);
			if (PINC & (1 << 0)) {
				y++;
			}
		}
		if (PINC & (1 << 1)) {
			_delay_ms(20);
			if (PINC & (1 << 1)) {
				y--;
			}
		}

		if (num >= (y - 0.5) && num <= (y + 0.5)) {
			PORTB &= ~(1 << 0);
			PORTB &= ~(1 << 1);
		}
		else if (num > (y + 0.5)) {
			PORTB &= ~(1 << 0);
			PORTB |= (1 << 1);
		}
		else {
			PORTB |= (1 << 0);
			PORTB &= ~(1 << 1);
		}
	}
}