#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


#define APIKEY  ""
#define DEVICE_DEV_ID ""        //change id device

const char ssid[] = "";             //change SSID wifi
const char password[] = "";     //change password wifi

//testingCinn
const char* telegramBotToken = "";
//Flood Observatory System Berr 2024
const char* telegramChatId = ""; 


void setup() {
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

void sendTelegramMessage(String message) {

  WiFiClientSecure client;
  client.setInsecure(); // Bypass SSL verification

  HTTPClient http;
  String url = "https://api.telegram.org/bot" + String(telegramBotToken) + "/sendMessage";
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["chat_id"] = telegramChatId;
  jsonDoc["text"] = message;

  String jsonString;
  serializeJson(jsonDoc, jsonString);

  Serial.println("Sending to Telegram: " + jsonString); // Debugging output

  http.begin(client,url);
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(jsonString);
  if (httpCode > 0) {
    Serial.printf("[HTTP] POST to Telegram... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_ACCEPTED) {
      String payload = http.getString();
      Serial.println("Telegram response: " + payload);
    } else {
      Serial.printf("Telegram error code: %d\n", httpCode);
      String payload = http.getString();
      Serial.println("Error response: " + payload);
    }
  } else {
    Serial.printf("[HTTP] POST to Telegram... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}

void loop() {
  if (Serial.available()) {
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
      sendTelegramMessage("Alert: Data sent to Favoriot: " + body); // Send Telegram message
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
}





