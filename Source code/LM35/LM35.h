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
// Cách dùng"

int main(void){
	adc_init();
	uint16_t adc_value;
	float temperature;
	while(1){
		adc_value = adc_read(0);
		temperature = adc_value * 0.48875;
		lcd_float(temperature);
	}
}

//Lưu ý: nối AREF với tụ và đất khi chọn kiểu setup REFS0 = 1 và REFS1 = 1;
/*
Bit 5-ADLAR (ADC Left Adjust Result): là bit cho phép hiệu chỉnh trái kết quả chuyển đổi. 
Sở dĩ có bit này là vì ADC trên AVR có độ phân giải 10 bit, nghĩa là kết quả thu được sau 
chuyển đổi là 1 số có độ dài 10 bit (tối đa 1023), AVR bố trí 2 thanh ghi data 8 bit để 
chứa giá trị sau chuyển đổi. Như thế giá trị chuyển đổi sẽ không lắp đầy 2 thanh ghi  data, 
trong một số trường hợp người dùng muốn 10 bit kết quả nằm lệch về phía trái trong khi 
cũng có trường hợp người dùng muốn kết quả nằm về phía phải. Bit ADLAR sẽ quyết định vị 
trí của 10 bit kết quả trong 16 bit của 2 thanh ghi data. Nếu ADLAR=0 kết quả sẽ được hiệu 
chỉnh về phía phải (thanh ghi ADCL chứa trọn 8 bit thấp và thanh ghi ADCH chứa 2 bit cao 
trong 10 bit kết quả), và nếu ADLAR=1 thì kết quả được hiệu chỉnh trái (thanh ghi ADCH chứa 
trọn 8 bit cao nhất, các bit từ 9 đến 2, và thanh ADCL chứa 2 bit thấp nhất trong 10 bit 
kết quả (bạn xem hình cách bố trí 2 thanh ghi ADCL và ADCH bên dưới để hiểu rõ hơn).

Bit 7:6- REFS1:0 (Reference Selection Bits): là các bit chọn điện áp tham chiếu cho ADC, 
1 trong 3 nguồn điện áp tham chiếu có thể được chọn là: điện áp ngoài từ chân VREF, điện áp 
tham chiếu nội 2.56V hoặc điện áp AVCC. Bảng 2 tóm tắt giá trị các bit và điện áp tham 
chiếu tương ứng.
