#include <ESP8266WiFi.h>

const char* ssid = "REDMI K80";
const char* pwd  = "123546hmj";

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
    client.println("<head>");
    client.println("<title>ESP8266 无线控灯</title>");
    client.println("<script>");
    client.println("function sendCmd(cmd){");
    client.println("var xhr=new XMLHttpRequest();");
    client.println("xhr.open('GET',cmd);");
    client.println("xhr.send();");
    client.println("}");
    client.println("</script>");
    client.println("</head>");
    client.println("<body style='background:#f0f2f5;text-align:center;padding-top:50px;font-family:Arial'>");
    client.println("<h1 style='color:#333;'>ESP8266 无线控灯演示</h1>");
    client.println("<p style='color:#666;font-size:16px;'>通过WiFi无线控制LED灯</p >");
    client.println("<button onclick='sendCmd(\"/on\")' style='padding:15px 40px;margin:20px;background:#28a745;color:white;border:none;border-radius:8px;font-size:20px'>开灯</button>");
    client.println("<button onclick='sendCmd(\"/off\")' style='padding:15px 40px;margin:20px;background:#dc3545;color:white;border:none;border-radius:8px;font-size:20px'>关灯</button>");
    client.println("</body></html>");
  }
  client.stop();
}
