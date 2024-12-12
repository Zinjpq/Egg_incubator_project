// Ngắt
	// Cấu hình nút ngắt //
	// Kích hoạt ngắt toàn cục
	sei();
	// Sử dụng ngắt ngoài
	GICR |= (1 << INT0) | (1 << INT1);
	// Cài đặt chế độ ngắt
	MCUCR = (1 << ISC00) | (1 << ISC01) | (1 << ISC10) | (1 << ISC11);
	// Cài đặt thời gian trước đó //
	init_millis(16000000UL); // tần số mà atmega328p đang chạy
	sei();

// Cách dùng
	// Cấu hình nút
	ISR(INT0_vect) {
		
		# <code>

		_delay_ms(20);
		// Chống rung phím
		while ((PIND & 0x04) == 0)	
		{}
		_delay_ms(20);
		// Xóa ngắt
		GIFR |= (1 << INTF0);
	}

	ISR(INT1_vect) {

		# <code>

		// Chống rung phím
		_delay_ms(20);
		while ((PIND & 0x08) == 0)
		{}
		_delay_ms(20);
		// Xóa ngắt
		GIFR |= (1 << INTF1);
	}
