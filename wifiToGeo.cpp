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
        
        int total = 0;

        for (int i = 0; i < n; ++i) {
            // 8個ぐらいあれば十分な気がする
            if(total > 8) {
                break;
            }
            String ssid = WiFi.SSID(i);
            if(this -> isAvoidSSID(ssid)) {
                Serial.print("not use: ");
                Serial.println(ssid);
                continue;
            }
            JsonObject &ap = wifiAccessPoints.createNestedObject();
            ap["macAddress"] = WiFi.BSSIDstr(i);
            ap["signalStrength"] = WiFi.RSSI(i);
            total++;
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
            result.lat = resRoot["location"]["lat"];
            result.lng = resRoot["location"]["lng"];
            result.accuracy = resRoot["accuracy"];
        } else {
            // Serial.print("Error code: ");
            // Serial.println(httpResponseCode);
        }
        _client.end();
    }
    return result;
}

const int blacklistNum = 6;
char blacklist[blacklistNum][8] = {
    // WiMAX
    "SPWN", 
    "W0",
    "wx0",
    // ワイモバイル
    "HWa",
    // b-mobile他
    "mobile",
    // ～のiPhone他
    "Phone"
};

// 使わないAPだったら true
bool WifiGeo::isAvoidSSID(const String &ssid) {
    // _nomapのオプトアウトに対応
    if(ssid.endsWith("_nomap")) {
        return true;
    }
    // モバイルWi-Fiかチェック
    for(int i = 0; i < blacklistNum; i++) {
        // indexOf:  一致したら場所を返す
        if(ssid.indexOf(blacklist[i]) != -1) {
            return true;
        }
    }

    return false;
}