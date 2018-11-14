#include <arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "wifiToGeo.h"

WifiGeo::WifiGeo() {
    ;
}
location_t WifiGeo::getGeoFromWifiAP() {
    Serial.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        StaticJsonBuffer<200> jsonBuffer;
        JsonObject &root = jsonBuffer.createObject();
        JsonArray &wifiAccessPoints = root.createNestedArray("wifiAccessPoints");

        
        Serial.print(n);
        Serial.println(" networks found");
        if(n > 8) {
          n = 8;
        }
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.BSSIDstr(i));
            Serial.print(" ");
            Serial.println(WiFi.RSSI(i));
            
            JsonObject &ap = wifiAccessPoints.createNestedObject();
            ap["macAddress"] = WiFi.BSSIDstr(i);
            ap["signalStrength"] = WiFi.RSSI(i);
            delay(10);
        }
        Serial.println(root.measureLength());
        char output[200];
        root.printTo(output);
        Serial.println(output);
        
        _client.begin("https://location.services.mozilla.com/v1/geolocate?key=test");
        _client.addHeader("Content-Type", "application/json");
    
        //char json[] = "{\"wifiAccessPoints\":[{\"macAddress\":\"E0:9D:B8:DF:5B:0E\",\"signalStrength\":-80}]}";
        int httpResponseCode = _client.POST(output);
        if(httpResponseCode>0){
            String response = _client.getString();  //Get the response to the request
            Serial.println(httpResponseCode);   //Print return code
            Serial.println(response);           //Print request answer
        }else{
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
        }
        _client.end();
    }
    Serial.println("");
    location_t result;
    return result;
}