#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


#define APIKEY  ""
#define DEVICE_DEV_ID ""        //change id device

const char ssid[] = "";             //change SSID wifi
const char password[] = "";     //change password wifi

#define RAIN_SENSOR_PIN 34         

void setup()
{
  pinMode(RAIN_SENSOR_PIN,INPUT); 
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
  long x = analogRead(RAIN_SENSOR_PIN);
  String rain_output;
  if (x >= 3000) {
    rain_output =  "No rain detected";
    Serial.println("No rain detected");
  } else {
    rain_output =  "Rain detected";
    Serial.println("Rain detected");
  }

  Serial.println(x);
 
   StaticJsonDocument<200> doc;
  
    JsonObject root = doc.to<JsonObject>(); // Json Object refer to { }
    root["device_developer_id"] = DEVICE_DEV_ID;
  
    JsonObject data = root.createNestedObject("data");
    data["Rain"] = (float)x;
    
  
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

  delay(5000);
}
