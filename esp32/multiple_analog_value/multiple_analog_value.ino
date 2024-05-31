#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


#define APIKEY  ""
#define DEVICE_DEV_ID "dht11_test_1_device@iiotsme"        //change id device

const char ssid[] = "1Oz6-2.4G";             //change SSID wifi
const char password[] = "1_Oz6@915";



#define PIN_1 34 
#define PIN_2 35
#define PIN_3 32
#define PIN_4 33        

const long interval = 5000; // Interval at which to blink (milliseconds)
unsigned long previousMillis = 0; // Stores the last time the LED was updated

void setup()
{
  pinMode(PIN_1,INPUT); 
  pinMode(PIN_2,INPUT); 
  pinMode(PIN_3,INPUT); 
  pinMode(PIN_4,INPUT); 
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

void displayValue(int a, int b,int c,int d){
  Serial.print("Output 1: ");
  Serial.println(a);
  Serial.print("Output 2: ");
  Serial.println(b);
  Serial.print("Output 3: ");
  Serial.println(c);
  Serial.print("Output 4: ");
  Serial.println(d);
}

void loop()
{
  unsigned long currentMillis = millis(); // Get the current time
  if(currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
    int out1 = analogRead(PIN_1);
    int out2 = analogRead(PIN_2);
    int out3 = analogRead(PIN_3);
    int out4 = analogRead(PIN_4);
    displayValue(out1,out2,out3,out4);

    //This creates a static JSON document with a capacity of 200 bytes.
    StaticJsonDocument<200> doc;

    //This initializes the root JSON object within the document.
    JsonObject root = doc.to<JsonObject>(); // Json Object refer to { }
    //This adds a key-value pair to the root object, 
    //where "device_developer_id" is the key and DEVICE_DEV_ID is its value.
    root["device_developer_id"] = DEVICE_DEV_ID;

    JsonObject data = root.createNestedObject("data");

    data["data1"] = (float)out1;
    data["data2"] = (float)out2;
    data["data3"] = (float)out3;
    data["data4"] = (float)out4;
    
  
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
