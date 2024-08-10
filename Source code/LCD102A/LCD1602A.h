//Giao tiếp LCD
#define LCD_RS PC2
#define LCD_E PC3
#define LCD_D4 PC4
#define LCD_D5 PC5
#define LCD_D6 PC6
#define LCD_D7 PC7

DDRC |= (1 << LCD_RS) | (1 << LCD_E) | (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);

lcd_command (0x02);	/* 4bit mode */
lcd_command (0x28);	/* Initialization of 16X2 LCD in 4bit mode */
lcd_command (0x0C);	/* Display ON Cursor OFF */
lcd_command (0x06);	/* Auto Increment cursor */
lcd_command (0x01);	/* Clear display */
lcd_command (0x80);	/* Cursor at home position */


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

void lcd_string_with_delay(const char* str, double delay_ms) {
	//clear screen
	lcd_command(0x01);
	int i = 0;
	while (str[i] != '\0') {
		if (i == 16) {
			lcd_command(0xC0);  //Move the cursor down to the second line
		}
		lcd_data(str[i]);
		i++;
		_delay_ms(10);
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
	lcd_string_with_delay("    Group 4     EMA3028 20 2024", 10);
	_delay_ms(1000);
	lcd_string_with_delay(" The Incubator", 10);
	_delay_ms(1000);
	lcd_command(0x01);  // Clear display
}

void write_temp(int temp, int temp_set) {
	lcd_clear(); // Clear only if necessary

	lcd_string("Temp: ");
	lcd_float(temp);
	lcd_command(0xC0); // Move cursor to second line

	lcd_string("T_set: ");
	lcd_float(temp_set);
}