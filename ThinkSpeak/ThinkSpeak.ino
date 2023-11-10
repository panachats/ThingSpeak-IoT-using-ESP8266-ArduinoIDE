#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <DHT.h>

float temp;
float hum;
DHT dht11(D4, DHT11);

const char* ssid = "vivo V23 5G";
const char* password = "212224236";

unsigned long channelID = 2338316;
const char* writeKey = "7FWSXWPPH9HM5GJX";

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

WiFiClient  client;

void ReadDHT11() {
  temp = dht11.readTemperature();
  hum = dht11.readHumidity();
}

void setup() {
  Serial.begin(9600);   
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);   
  dht11.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
    }

    ReadDHT11();
    ThingSpeak.setField(1, hum);
    ThingSpeak.setField(2, temp);
    float x = ThingSpeak.writeFields(channelID, writeKey);

    if (x == 200) {
        Serial.println("Channel update successful.");
    } else {
        Serial.println("Problem updating channel. HTTP error code " + String(x));
    }

    lastTime = millis();
  }
}










