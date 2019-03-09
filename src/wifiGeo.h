#ifndef WIFI_GEO_H
#define WIFI_GEO_H

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
    
    // APIを使う場合は必ず呼び出す
    // @return 成功: true, 失敗: false
    // client: HTTPClientのポインタ
    // key: APIキー
    // type: MOZILLA_API or GOOGLE_API
    bool beginAPI(HTTPClient *client, const char* key = "test", int type = MOZILLA_API);

    // 周囲のAP情報をJSON形式でjsonStrに上書き
    // @return 見つかったAPの数
    // jsonStr: 上書きするStringの参照
    int getWifiJson(String &jsonStr);

    location_t getGeoFromWifiAP();
protected:
    HTTPClient *_client;
    bool isAvoidSSID(const String &ssid);
    String postUrl;
};

#endif //WIFI_GEO_H