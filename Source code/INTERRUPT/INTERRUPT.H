// Interrupt
	//setup interrupt button//
	//enable global interrupt
	sei();
	//set using external interrupt
	GICR|=(1<<INT0)|(1<<INT1);
	//set mode of interrupt
	MCUCR=(1<<ISC00)|(1<<ISC01)|(1<<ISC10)|(1<<ISC11);
	//set prevtime//
	init_millis(16000000UL); //frequency the atmega328p is running at
	sei();

//Cách dùng
	//button configuration
	ISR(INT0_vect){
		temp_set++;
		_delay_ms(20);
		//Anti-vibration keys
		while ((PIND & 0x04)==0)	
		{}
		_delay_ms(20);
		//Delete with interrupt
		GIFR |= (1 << INTF0);
	}

	ISR(INT1_vect){
		temp_set--;
		//Anti-vibration keys
		_delay_ms(20);
		while ((PIND & 0x08)==0)
		{}
		_delay_ms(20);
		//Delete with interrupt
		GIFR |= (1 << INTF1);
	}

