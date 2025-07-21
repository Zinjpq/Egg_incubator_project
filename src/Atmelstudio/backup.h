//Điều khiển quạt và đèn
void fan_control(float temp){
	if (temp >= temp_set) {
		PORTB |= (1 << FAN); // Turn fan on
		} else {
		PORTB &= ~(1 << FAN); // Turn fan off
	}
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


// Đọc dữ liệu từ Lm35
uint16_t adc_read(uint8_t adc_port) {
	ADMUX = (1<<REFS0) | adc_port; // Set ADC port
	ADCSRA |= (1 << ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)) {
		// Wait for conversion to finish
	}
	return ADCW; // Return the converted value
}

void adc_init(){
	ADMUX = (1 << REFS0); // Select AVCC as the reference source
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	// Turn on ADC and select prescaler 1/128 (ADPS2:0 = 111)

	//ADCSRB = 0; // Set all bits in ADCSRB to 0 to not use any functions

	// Wait for the ADC to stabilize
	_delay_ms(2);
}

