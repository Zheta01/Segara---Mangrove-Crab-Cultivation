#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "class.h"
#include <String.h>

AsyncWebServer server(80);

const char* ssid = "Dua Musim";
const char* password = "HALLO2musim";

const char* PARAM_MESSAGE = "message";

unsigned long int interval=2000;
unsigned long int lastTime=0;

IPAddress subnet(255, 255, 0, 0);			            
IPAddress gateway(192, 168, 1, 1);			            
IPAddress local_IP(192, 168, 1, 184);

String data="kosong";

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {
    
    Serial.begin(115200);

    if(WiFi.config(local_IP,gateway,subnet)){
        Serial.println("Static IP configured");
    }
    else {
    Serial.println("Static IP Configuration Failed");
        }

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED){
        Serial.print(".");
    }
  
    Serial.print("");
    Serial.println("WiFi Connected! IP address:");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", data);
    });

    server.onNotFound(notFound);
    server.begin();

}

void loop() {

while(Serial.available()>0){
      data = Serial.readString();
    }
  if(millis()-lastTime > interval){
    Serial.print(1);
    lastTime = millis();
  }
}