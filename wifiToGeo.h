#ifndef WIFI_TO_GEO_H
#define WIFI_TO_GEO_H

#include <HTTPClient.h>

typedef struct {
    float lat = 0.0;
    float lng = 0.0;
    float accuracy = 0.0;
} location_t;

class WifiGeo {
public:
    WifiGeo();
    location_t getGeoFromWifiAP();
protected:
    HTTPClient _client;
};

#endif //WIFI_TO_GEO_H