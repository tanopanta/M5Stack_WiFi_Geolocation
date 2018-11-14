#include <M5Stack.h>
#include <WiFi.h>

#include "wifiToGeo.h"

const char* ssid     = "";
const char* password = "";

WifiGeo geo;
void setup() {
    // put your setup code here, to run once:
    M5.begin();

    // LCD display
    M5.Lcd.print("Hello World");
    Serial.begin(115200);


    delay(4000);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { //Check for the connection
      delay(1000);
      Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
    geo.getGeoFromWifiAP();
}

void loop() {
    
    // Wait a bit before scanning again
    delay(30000);

}
