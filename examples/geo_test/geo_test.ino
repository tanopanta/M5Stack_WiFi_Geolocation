#include <M5Stack.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <wifiGeo.h>

const char* ssid     = "";
const char* password = "";
HTTPClient httpClient;

WifiGeo geo;
void setup() {
    M5.begin();

    M5.Lcd.print("wifi to geo");

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
    
    geo.beginAPI(&httpClient); // == geo.beginAPI(&httpClient, "test", MOZILLA_API);

    //geo.beginAPI(&httpClient, "abcdefg12345678", GOOGLE_API); // google Geolocation api

    location_t result = geo.getGeoFromWifiAP();
    Serial.printf("lat:%f, lng:%f, accuracy:%f\n", result.lat, result.lng, result.accuracy);

    
    String json;
    int num = geo.getWifiJson(json);

    Serial.printf("%d AP\n", num);
    Serial.println(json);
}

void loop() {
    delay(30000);
}