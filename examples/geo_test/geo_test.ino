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
    M5.Lcd.print("wifi to geo");

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { //Check for the connection
      delay(500);
      Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
    
    unsigned long tstart = millis();
    
    location_t result = geo.getGeoFromWifiAP();
    Serial.printf("lat:%f, lng:%f, accuracy:%f\n", result.lat, result.lng, result.accuracy);

    unsigned long tend = millis();
    Serial.println(tend -tstart);
}

void loop() {
    
    // Wait a bit before scanning again
    delay(30000);

}
