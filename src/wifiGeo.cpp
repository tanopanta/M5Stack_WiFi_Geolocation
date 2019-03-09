#include <arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "wifiGeo.h"

const char* googleURL = "https://www.googleapis.com/geolocation/v1/geolocate?key=";
const char* mozillaURL = "https://location.services.mozilla.com/v1/geolocate?key=";


WifiGeo::WifiGeo() {
    ;
}

bool WifiGeo::beginAPI(HTTPClient *client, const char* key, int type) {
    
    if(NULL == client) {
        Serial.println("HTTP Client is not initialized.");
        return false;
    }

    this->_client = client;
    switch(type) {
        case MOZILLA_API:
            postUrl = String(mozillaURL) + String(key);
            break;
        case GOOGLE_API:
            postUrl = String(googleURL) + String(key);
            break;
        default:
            Serial.println("type is MOZILLA_API or GOOGLE_API");
            return false;
    }
}

int WifiGeo::getWifiJson(String &jsonStr){
    // int16_t WiFiScanClass::scanNetworks(bool async = false, bool show_hidden = false, bool passive = false, uint32_t max_ms_per_chan = 300)
    int n = WiFi.scanNetworks(false, false, true, 100);


    DynamicJsonBuffer jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonArray &wifiAccessPoints = root.createNestedArray("wifiAccessPoints");

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
    root.printTo(jsonStr);
    
    return total;
}

location_t WifiGeo::getGeoFromWifiAP() {
    location_t result;
    String json;
    int num = this->getWifiJson(json);
    if(num == 0) {
        return result;
    }

    this->_client->begin(postUrl);
    this->_client->addHeader("Content-Type", "application/json");

    //char json[] = "{\"wifiAccessPoints\":[{\"macAddress\":\"E0:9D:B8:DF:5B:0E\",\"signalStrength\":-80}]}";

    int httpResponseCode = this->_client->POST(json);
    if(httpResponseCode == 200) {
        // Allocate JsonBuffer
        // Use arduinojson.org/assistant to compute the capacity.
        const size_t bufferSize = 2*JSON_OBJECT_SIZE(2) + 60;
        DynamicJsonBuffer resJsonBuffer(bufferSize);

        String response = this->_client->getString();  //Get the response to the request
        
        JsonObject& resRoot = resJsonBuffer.parseObject(response);
        result.lat = resRoot["location"]["lat"];
        result.lng = resRoot["location"]["lng"];
        result.accuracy = resRoot["accuracy"];
    } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    this->_client->end();
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
