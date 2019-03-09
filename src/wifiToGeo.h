#ifndef WIFI_TO_GEO_H
#define WIFI_TO_GEO_H

#include <HTTPClient.h>

#define MOZILLA_API 1
#define GOOGLE_API 2


typedef struct {
    double lat = 0.0;
    double lng = 0.0;
    double accuracy = 0.0;
} location_t;

class WifiGeo {
public:
    WifiGeo();
    
    //APIを使う場合は必ず呼び出す
    bool beginAPI(HTTPClient *client, const char* key = "test", int type = MOZILLA_API);

    location_t getGeoFromWifiAP();
protected:
    HTTPClient *_client;
    bool isAvoidSSID(const String &ssid);
    String postUrl;
};

#endif //WIFI_TO_GEO_H