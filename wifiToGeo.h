#ifndef WIFI_TO_GEO_H
#define WIFI_TO_GEO_H

#include <HTTPClient.h>

typedef struct {
    float lat = 0;
    float lon = 0;
    int accuracy = 0;
} location_t;

class WifiGeo {
public:
    WifiGeo();
    location_t getGeoFromWifiAP();
protected:
    HTTPClient _client;
};

#endif //WIFI_TO_GEO_H