#include <M5Stack.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


const char* ssid     = "***";
const char* password = "***";

HTTPClient http;

void setup() {
    // put your setup code here, to run once:
    M5.begin();

    // LCD display
    M5.Lcd.print("Hello World");
    Serial.begin(115200);


    delay(4000);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { //Check for the connection
      delay(1000);
      Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
}

void loop() {
    // put your main code here, to run repeatedly:
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
        
        http.begin("https://location.services.mozilla.com/v1/geolocate?key=test");
        http.addHeader("Content-Type", "application/json");
    
        //char json[] = "{\"wifiAccessPoints\":[{\"macAddress\":\"E0:9D:B8:DF:5B:0E\",\"signalStrength\":-80}]}";
        int httpResponseCode = http.POST(output);
        if(httpResponseCode>0){
            String response = http.getString();  //Get the response to the request
            Serial.println(httpResponseCode);   //Print return code
            Serial.println(response);           //Print request answer
        }else{
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
        }
        http.end();
    }
    Serial.println("");
    
    

    // Wait a bit before scanning again
    delay(30000);

}
