#include <ESP8266WiFi.h>

const char* ssid = "所连接WiFi账号";
const char* pwd  = "所连接WiFi密码";

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
  Serial.println("\nWiFi连接成功！控制IP：");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  String req = client.readStringUntil('\r');

  // 处理开关指令
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
    // 如果是开关指令，只返回简短文字，不返回整个网页
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: keep-alive");
    client.println("");
    client.println("OK");
  }else{
    // 浏览器第一次打开页面，返回完整网页
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html; charset=utf-8");
    client.println("Connection: keep-alive");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
