#include <ESP8266WiFi.h>

unsigned long previousMillis = 0; // stores the last time the count was printed
const unsigned long interval = 10000; // interval at which to print (10 seconds)

const char* apikey =  "";
const String device_developer_id = "";   //change id device

const char ssid[] = "";             //change SSID wifi
const char pass[] = "";     //change password wifi  
const char* server = "apiv2.favoriot.com";     

int status = WL_IDLE_STATUS;
WiFiClient client;

void setup() {
  // Initialize serial communication at a baud rate of 115200
  Serial.begin(9600);
  WiFi.disconnect();
  Serial.println("Connecting...");
  WiFi.begin(ssid,pass);
  
  while((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print(".");
  }
  
  Serial.println(WiFi.status());
  Serial.println("Connected!");
  Serial.println("");
  Serial.println(WiFi.localIP());

}

void loop() {
  // put your main code here, to run repeatedly:
  // Generate a random value between 0 and 1023
  int randomValue = random(0, 1024);

  // Print the random value to the Serial Monitor
  Serial.print("Before transmitting to Favoriot Random Value: ");
  Serial.println(randomValue);

    //=========================json favoriot data send ==================================================
  String json = "{\"device_developer_id\":\""+device_developer_id+"\",\"data\":{\"Random Value:\":\""+randomValue+"\"}}";
  Serial.println(json);
  if (client.connect(server, 80)) {
    client.println("POST /v2/streams HTTP/1.1");
    client.println("Host: apiv2.favoriot.com");
    client.println("apikey: " + String(apikey));
    client.println("Content-Type: application/json");
    client.println("cache-control: no-cache");
    client.print("Content-Length: ");
    int thisLength = json.length();
    client.println(thisLength);
    client.println("Connection: close");
    client.println();
    client.println(json);
  }
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  if (!client.connected()) {
    client.stop();
  }
  delay(5000);

}
