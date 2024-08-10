#define F_CPU 16000000UL  // Define CPU frequency for delay functions
#include <avr/io.h>       // Include AVR I/O library
#include <util/delay.h>   // Include delay library

void PWM_set() {
	DDRD |= (1 << PD7);  // Set PD7 as output for PWM signal
	TCCR2 |= (1 << WGM20) | (1 << WGM21);  // Fast PWM mode, non-inverting
	TCCR2 |= (1 << COM21) | (1 << CS20);  // Clear OC2 on compare match, no prescaling
}

int main() {
	unsigned char duty;
	PWM_set();  // Configure PWM settings

	while (1) {
		for (duty = 0; duty < 255; duty++) {
			OCR2 = duty;  // Increase duty cycle to gradually brighten LED
			_delay_ms(10);
		}

		for (duty = 254; duty >= 0; duty--) {  // Start from 254 to avoid overflow
			OCR2 = duty;  // Decrease duty cycle to gradually dim LED
			_delay_ms(10);
		}
	}
}
