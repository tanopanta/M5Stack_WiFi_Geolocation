#ifndef WIFI_TO_GEO_H
#define WIFI_TO_GEO_H

#include <HTTPClient.h>

typedef struct {
    double lat = 0.0;
    double lng = 0.0;
    double accuracy = 0.0;
} location_t;

class WifiGeo {
public:
    WifiGeo();
    location_t getGeoFromWifiAP();
protected:
    HTTPClient _client;
};

#endif //WIFI_TO_GEO_H