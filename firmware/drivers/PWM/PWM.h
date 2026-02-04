// PWM 
/*PWM nhanh, không đảo chiều, bộ chia tần số 1:1*/
	TCCR0=(1<<WGM01)|(1<<WGM00)|(1<<COM01)|(1<<CS00);
	/*Chu kỳ nhiệm vụ*/
	OCR0=125; //độ sáng mặc định = 50%

// Hàm sử dụng
void dieu_khien_led(float nhiet_do){
	current_time = millis();
	if (current_time - prev_time > 100) {
		int pwm=OCR0;
		// Điều chỉnh xung theo nhiệt độ
		if (nhiet_do > nhiet_do_dat) {
			pwm -= 5;
			} else if (nhiet_do < nhiet_do_dat) {
			pwm += 5;
		}
		prev_time=current_time;
		// đặt lại
		if (pwm < 0) {
			pwm = 0;
			} else if (pwm > 255) {
			pwm = 255;
		}
		//Lặp lại cho OCR0 để điều chỉnh xung
		OCR0=pwm;
		_delay_ms(50);
	}
}