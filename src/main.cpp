#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <esp32-hal.h> // temperatureRead function

const char *ssid = "Redmi";
const char *password = "robert000";
const char *insertionAddress = "http://lukboz.cba.pl/esp/api/insert/";
int counter = 0;

void setup()
{
	Serial.begin(9600);

	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.println("Connecting to WiFi..");
	}
	Serial.println("Connected to the WiFi network");
}

void loop()
{
	if (WiFi.status() == WL_CONNECTED)
	{
		HTTPClient http;
		http.begin(insertionAddress);
		http.addHeader("Content-Type", "application/x-www-form-urlencoded");

		// read temperature from buildin sensor and convert to Celcius
		float currentTemperature = temperatureRead();
		String message ="content={param1:" + String(currentTemperature) + ",param2:" + String(counter) + "}";
		
		int responseCode = http.POST(message);
	
		String responseMessage = http.getString();
		if (responseCode > 0)
		{	
			Serial.println("Code: " + String(responseCode) + "; Message: " + responseMessage);
			Serial.println("Temp: " + String(currentTemperature));
			++counter;
		}
		else
			Serial.println("Error on sending data");

		http.end();
	}
	// send request every 3 seconds
	delay(3000);
}