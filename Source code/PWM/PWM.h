// PWM 
/*Fast PWM, On-Inverting, 1:1 PreScaler*/
	TCCR0=(1<<WGM01)|(1<<WGM00)|(1<<COM01)|(1<<CS00);
	/*Duty Cycle*/
	OCR0=125; //default do sang=50%

// Hàm sử dụng
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