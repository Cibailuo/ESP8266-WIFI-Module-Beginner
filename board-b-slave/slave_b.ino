//注意详情请看主板
//高低电平逻辑变换在34-44行代码
#include <ESP8266WiFi.h>

const char* ssid = "所连接wifi账号";
const char* pwd  = "所连接wifi密码";

WiFiServer server(80);
const int led = D4;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);

  Serial.print("连接WiFi中");
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println(F("\nWiFi连接成功！从ESP IP："));
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  String req = client.readStringUntil('\r');
  while(client.available()){
    client.read();
  }

  bool isCmd = false;
  if (req.indexOf("/on") != -1) {
    digitalWrite(led, LOW);
    isCmd = true;
  }
  if (req.indexOf("/off") != -1) {
    digitalWrite(led, HIGH);
    isCmd = true;
  }

  if(isCmd){
    client.println(F("HTTP/1.1 200 OK"));
    client.println(F("Content-Type: text/plain"));
    client.println(F("Connection: close"));
    client.println(F(""));
    client.println(F("OK"));
  }else{
    client.println(F("HTTP/1.1 200 OK"));
    client.println(F("Content-Type: text/html; charset=utf-8"));
    client.println(F("Connection: close"));
    client.println(F(""));
    client.print(F("<!DOCTYPE HTML><html><head><title>ESP2灯</title></head><body><h2>ESP2 页面</h2></body></html>"));
  }
  client.stop();
}
