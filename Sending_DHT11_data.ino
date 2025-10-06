#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"

#define DHTPIN 4        // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);


float humidity;
float temperature;

const char* ssid = "BEHDAD";
const char* password = "behdad1234";

// Your ThingsBoard access token
const char* accessToken = "PfaLAKE9bTmmjx5srEfq";

// ThingsBoard server
const char* server = "http://thingsboard.cloud";

void setup() {

  Serial.begin(115200);
  delay(1000);

  wifi_start();

  dht.begin();

}

void wifi_start(){

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  delay(1000);
  Serial.println("\nWiFi connected!");

}


void loop() {

  if (WiFi.status() == WL_CONNECTED) {
    
    WiFiClient client;
    HTTPClient http;

    // Construct the full URL like in your curl command
    String url = String(server) + "/api/v1/" + accessToken + "/telemetry";

    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");

    // JSON data to send
    // String payload = "{\"temperature\":25}";
    // String payload = "{\"temperature\":25, \"humidity\":60, \"pressure\":1013}";

    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature) )  // Check if any reads failed and exit early (to try again).
    {
      Serial.println(F("Failed to read from DHT sensor!"));
      temperature = 0;
      humidity = 0;
    }

    Serial.print(F("Humidity: "));
    Serial.print(humidity);
    Serial.print(F("%  Temperature: "));
    Serial.print(temperature);
    Serial.println(F("°C "));

    String payload = "{\"temperature\":" + String(temperature) + ", \"humidity\":" + String(humidity) + "}";


    // Send POST request
    int httpResponseCode = http.POST(payload);

    // Print the result
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    http.end();
  } 
  else 
  {
    Serial.println("WiFi disconnected!");

    Serial.println("Trying to reconnect to wifi!");

    wifi_start();
  }

  delay(5000); // send data every 5 seconds
}
