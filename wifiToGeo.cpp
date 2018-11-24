#include <arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "wifiToGeo.h"

WifiGeo::WifiGeo() {
    ;
}
location_t WifiGeo::getGeoFromWifiAP() {
    // Serial.println("scan start");
    location_t result;

    // WiFi.scanNetworks will return the number of networks found
    //　第4引数で1チャンネル当たりの探索時間を指定 デフォルト300ms
    int n = WiFi.scanNetworks(false, false, false, 101);
    
    // Serial.println("scan done");
    if (n == 0) {
        //Serial.println("no networks found");
    } else {
        DynamicJsonBuffer jsonBuffer;
        JsonObject &root = jsonBuffer.createObject();
        JsonArray &wifiAccessPoints = root.createNestedArray("wifiAccessPoints");

        
        // Serial.print(n);
        // Serial.println(" networks found");
        // 8個ぐらいあれば十分な気がする
        if(n > 8) {
          n = 8;
        }
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            // Serial.print(i + 1);
            // Serial.print(": ");
            // Serial.print(WiFi.BSSIDstr(i));
            // Serial.print(" ");
            // Serial.println(WiFi.RSSI(i));
            
            JsonObject &ap = wifiAccessPoints.createNestedObject();
            ap["macAddress"] = WiFi.BSSIDstr(i);
            ap["signalStrength"] = WiFi.RSSI(i);
            // delay(10);
        }
        // Serial.println(root.measureLength());
        //char output[512];
        String output;
        root.printTo(output);
        // Serial.println(output);
        
        _client.begin("https://location.services.mozilla.com/v1/geolocate?key=test");
        _client.addHeader("Content-Type", "application/json");
    
        //char json[] = "{\"wifiAccessPoints\":[{\"macAddress\":\"E0:9D:B8:DF:5B:0E\",\"signalStrength\":-80}]}";

        int httpResponseCode = _client.POST(output);
        if(httpResponseCode == 200) {
            // Allocate JsonBuffer
            // Use arduinojson.org/assistant to compute the capacity.
            const size_t bufferSize = 2*JSON_OBJECT_SIZE(2) + 60;
            DynamicJsonBuffer resJsonBuffer(bufferSize);

            String response = _client.getString();  //Get the response to the request
            
            JsonObject& resRoot = resJsonBuffer.parseObject(response);
            result.lat = resRoot["location"]["lat"]; // -22.7539192
            result.lng = resRoot["location"]["lng"]; // -43.4371081
            result.accuracy = resRoot["accuracy"];
            
            // Serial.println(httpResponseCode);   //Print return code
            // Serial.println(response);           //Print request answer
        } else {
            // Serial.print("Error code: ");
            // Serial.println(httpResponseCode);
        }
        _client.end();
    }
    return result;
}