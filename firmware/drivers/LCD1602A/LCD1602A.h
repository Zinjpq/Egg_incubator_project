// Giao tiếp LCD
#define LCD_RS PC2
#define LCD_E PC3
#define LCD_D4 PC4
#define LCD_D5 PC5
#define LCD_D6 PC6
#define LCD_D7 PC7

DDRC |= (1 << LCD_RS) | (1 << LCD_E) | (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);

// Các hàm giao tiếp với LCD

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

// Khởi tạo LCD
void lcd_init(){
	_delay_ms(20);  /* Thời gian khởi tạo LCD khi bật nguồn >15ms */
	lcd_command (0x02); /* Chế độ 4bit */
	lcd_command (0x28); /* Khởi tạo LCD 16X2 ở chế độ 4bit */
	lcd_command (0x0C); /* Bật hiển thị, tắt con trỏ */
	lcd_command (0x06); /* Tự động tăng con trỏ */
	lcd_command (0x01); /* Xóa màn hình */
	lcd_command (0x80); /* Đưa con trỏ về vị trí ban đầu */
}

// Gửi dữ liệu đến LCD
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

// Gửi chuỗi ký tự đến LCD
void lcd_string(const char* str) {
	int i = 0;
	while (str[i] != '\0') {
		lcd_data(str[i]);
		i++;
	}
}

// Gửi chuỗi ký tự đến LCD với delay
void lcd_string_with_delay(const char* str, double delay_ms) {
	// Xóa màn hình
	lcd_command(0x01);
	int i = 0;
	while (str[i] != '\0') {
		if (i == 16) {
			lcd_command(0xC0);  // Di chuyển con trỏ xuống dòng thứ hai
		}
		lcd_data(str[i]);
		i++;
		_delay_ms(10);
	}
}

// Gửi số nguyên đến LCD
void lcd_float(float number) {
	char str[10];
	char str_i[10];
	dtostrf(number, 2, 2, str_i);
	snprintf(str, sizeof(str), "%s", str_i);
	lcd_string(str);
}

// Xóa màn hình LCD
void lcd_clear() {
	lcd_command(0x01);  // Xóa màn hình LCD
}

// Hiển thị lời chào
void hello(){
	// Lời chào
	lcd_string_with_delay("    Group 4     EMA3028 20 2024", 10);
	_delay_ms(1000);
	lcd_string_with_delay(" The Incubator", 10);
	_delay_ms(1000);
	lcd_command(0x01);  // Xóa màn hình
}

// Hiển thị nhiệt độ và nhiệt độ set
void write_temp(int temp, int temp_set) {
	lcd_clear(); // Chỉ xóa nếu cần thiết

	lcd_string("Temp: ");
	lcd_float(temp);
	lcd_command(0xC0); // Di chuyển con trỏ xuống dòng thứ hai

	lcd_string("T_set: ");
	lcd_float(temp_set);
}
