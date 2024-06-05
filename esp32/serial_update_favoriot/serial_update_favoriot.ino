#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


#define APIKEY  "RwxZ3MgXBcW1hB7iIeORFkdyBO3PsZ2n"
#define DEVICE_DEV_ID "dht11_test_1_device@iiotsme"        //change id device

const char ssid[] = "1Oz6-2.4G";             //change SSID wifi
const char password[] = "1_Oz6@915";     //change password wifi
  

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

}

void loop()
{
  if(Serial.available()){
   int serialValue = Serial.parseInt();
   StaticJsonDocument<200> doc;
  
    JsonObject root = doc.to<JsonObject>(); // Json Object refer to { }
    root["device_developer_id"] = DEVICE_DEV_ID;
  
    JsonObject data = root.createNestedObject("data");
    data["Serial_Value"] = (float)serialValue;
    
  
    String body;
    serializeJson(root, body);
    Serial.println(body);
  
    HTTPClient http;
  
    http.begin("https://apiv2.favoriot.com/v2/streams");
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Apikey", APIKEY);
  
    int httpCode = http.POST(body);
    if (httpCode > 0) {
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    }
    else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }

}



