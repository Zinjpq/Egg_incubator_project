/*
 * Apr10.cpp
 *
 * Created: 10/04/2024 00:11:10
 *  Author: Zinj
 */ 
#include <DHT11.h>

#define DHTPIN 2 // Chân k?t n?i d? li?u DHT11
#define LEDPIN 9 // Chân k?t n?i LED

DHT11 dht(DHTPIN);

void setup() {
	Serial.begin(9600); // Kh?i t?o giao ti?p n?i ti?p
	pinMode(LEDPIN, OUTPUT); // Ch?nh chân LED thành ngõ ra
}

void loop() {
	// ??c d? li?u nhi?t ?? và ?? ?m
	float temperature = dht.readTemperature();
	float humidity = dht.readHumidity();

	// Hi?n th? d? li?u trên Serial
	Serial.print("Nhi?t ??: ");
	Serial.print(temperature);
	Serial.print("°C ");
	Serial.print("?? ?m: ");
	Serial.print(humidity);
	Serial.println("%");

	// ?i?u ch?nh c??ng ?? ?èn d?a trên nhi?t ??
	if (temperature > 30) {
		int brightness = map(temperature, 30, 40, 255, 0);
		analogWrite(LEDPIN, brightness);
		} else {
		analogWrite(LEDPIN, 255); // B?t ?èn t?i ?a
	}

	delay(2000); // Ch? 2 giây tr??c khi ??c l?i d? li?u
}
