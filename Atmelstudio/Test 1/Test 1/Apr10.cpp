/*
 * Apr10.cpp
 *
 * Created: 10/04/2024 00:11:10
 *  Author: Zinj
 */ 
#include <DHT11.h>

#define DHTPIN 2 // Ch�n k?t n?i d? li?u DHT11
#define LEDPIN 9 // Ch�n k?t n?i LED

DHT11 dht(DHTPIN);

void setup() {
	Serial.begin(9600); // Kh?i t?o giao ti?p n?i ti?p
	pinMode(LEDPIN, OUTPUT); // Ch?nh ch�n LED th�nh ng� ra
}

void loop() {
	// ??c d? li?u nhi?t ?? v� ?? ?m
	float temperature = dht.readTemperature();
	float humidity = dht.readHumidity();

	// Hi?n th? d? li?u tr�n Serial
	Serial.print("Nhi?t ??: ");
	Serial.print(temperature);
	Serial.print("�C ");
	Serial.print("?? ?m: ");
	Serial.print(humidity);
	Serial.println("%");

	// ?i?u ch?nh c??ng ?? ?�n d?a tr�n nhi?t ??
	if (temperature > 30) {
		int brightness = map(temperature, 30, 40, 255, 0);
		analogWrite(LEDPIN, brightness);
		} else {
		analogWrite(LEDPIN, 255); // B?t ?�n t?i ?a
	}

	delay(2000); // Ch? 2 gi�y tr??c khi ??c l?i d? li?u
}
