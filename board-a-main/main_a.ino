/*
温馨注意
1.D4引脚自带板子上的灯光模块，可用于测试，需接继电器时需将D4换为其他‘D’开头引脚
2.WiFi账密与其余副板的IP需自己填写
3.供电引脚需用3.3v的，用5v的容易电流过低导致灯不亮（亲身实测）
4.下面web服务内代码里的中文都可根据个人需求更改，不会对代码功能产生影响
5.高低电平逻辑变换可通过交换50-51行代码里的HIGH与LOW来更改（注意副板也要更改，更改代码的具体行数请见副板代码）
*/
#include <ESP8266WiFi.h>

const char* ssid = "所连接的wifi账号";
const char* pwd  = "所连接的WiFi密码";
const char* espB_IP[] = {"副板1的IP","副板2的IP"};//可先烧录单板代码或副板代码到副板内获得副板IP

WiFiServer server(80);
const int relayMain = D4;

void remoteCmd(const char* ip, String cmd){
  WiFiClient client;
  if(client.connect(ip,80)){
    String httpReq = "GET /"+cmd+" HTTP/1.1\r\nHost:"+String(ip)+"\r\nConnection:close\r\n\r\n";
    client.print(httpReq);
    client.stop();
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(relayMain, OUTPUT);
  digitalWrite(relayMain, HIGH);
  Serial.print("连接WiFi中");
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println(F("\nWiFi连接成功！主ESP IP："));
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient webClient = server.available();
  if (!webClient) return;
  String req = webClient.readStringUntil('\r');
  while(webClient.available()) webClient.read();

  bool isCmd = false;
  if (req.indexOf("/on1") != -1)  { digitalWrite(relayMain, HIGH);  isCmd = true; }
  if (req.indexOf("/off1") != -1) { digitalWrite(relayMain, LOW); isCmd = true; }

  if (req.indexOf("/on2") != -1)  { remoteCmd(espB_IP[0], "on");  isCmd = true; }
  if (req.indexOf("/off2") != -1) { remoteCmd(espB_IP[0], "off"); isCmd = true; }
  if (req.indexOf("/on3") != -1)  { remoteCmd(espB_IP[1], "on");  isCmd = true; }
  if (req.indexOf("/off3") != -1) { remoteCmd(espB_IP[1], "off"); isCmd = true; }

  if(isCmd){
    webClient.println(F("HTTP/1.1 200 OK"));
    webClient.println(F("Content-Type: text/plain"));
    webClient.println(F("Connection: close"));
    webClient.println(F(""));
    webClient.println(F("OK"));
  }else{
    webClient.println(F("HTTP/1.1 200 OK"));
    webClient.println(F("Content-Type: text/html; charset=utf-8"));
    webClient.println(F("Connection: close"));
    webClient.println(F(""));
    webClient.print(F("<!DOCTYPE HTML><html><head><meta name='viewport' content='width=device-width,initial-scale=1'><title>智能家居控制系统</title>"));
    webClient.print(F("<style>"));
    webClient.print(F("*{margin:0;padding:0;box-sizing:border-box;font-family:'PingFang SC','Microsoft YaHei',sans-serif}"));
    webClient.print(F("body{min-height:100vh;background:#142236;background-image:radial-gradient(circle at 50% 20%,rgba(90,120,200,.5),rgba(20,34,54,0) 60%),linear-gradient(160deg,#0d1b2a,#1b263b,#415a77);display:flex;flex-direction:column;align-items:center;padding:44px 16px}"));
    webClient.print(F("#qr{position:fixed;top:20px;left:20px;width:130px;height:130px;z-index:99;background:#fff;padding:8px;border-radius:12px;box-shadow:0 6px 18px rgba(0,0,0,.35)}"));
    webClient.print(F(".wrap{text-align:center;margin-bottom:30px}"));
    webClient.print(F(".title{font-size:42px;font-weight:900;letter-spacing:5px;background:linear-gradient(90deg,#fff,#ffd700,#fff);-webkit-background-clip:text;background-clip:text;-webkit-text-fill-color:transparent;margin-bottom:8px}"));
    webClient.print(F(".subtitle{color:#ddd;font-size:13px;letter-spacing:8px;border-top:1px solid rgba(255,255,255,.35);padding-top:10px;display:inline-block}"));
    webClient.print(F(".card{border-radius:22px;padding:26px 24px;width:100%;max-width:340px;margin:0 auto 22px auto;color:#fff;box-shadow:0 14px 34px rgba(0,0,0,.4);border:1px solid rgba(255,255,255,.22)}"));
    webClient.print(F(".fan{background:linear-gradient(135deg,rgba(0,180,255,.95),rgba(0,110,240,.95))}"));
    webClient.print(F(".humid{background:linear-gradient(135deg,rgba(50,200,210,.95),rgba(80,120,225,.95))}"));
    webClient.print(F(".air{background:linear-gradient(135deg,rgba(16,150,138,.95),rgba(60,230,120,.95))}"));
    webClient.print(F(".icon{font-size:44px;margin-bottom:10px;display:block}"));
    webClient.print(F(".card-title{font-size:20px;font-weight:800;letter-spacing:2px;margin-bottom:4px}"));
    webClient.print(F(".card-sub{font-size:12px;opacity:.9;letter-spacing:1px;margin-bottom:18px}"));
    webClient.print(F(".btn-row{display:flex;gap:14px}"));
    webClient.print(F("button{flex:1;padding:14px 0;border:none;border-radius:14px;font-size:18px;font-weight:800;letter-spacing:6px;color:#fff;cursor:pointer;transition:.18s}"));
    webClient.print(F(".on{background:rgba(255,255,255,.32)}.on:hover{background:rgba(255,255,255,.48)}"));
    webClient.print(F(".off{background:rgba(0,0,0,.42)}.off:hover{background:rgba(0,0,0,.58)}"));
    webClient.print(F("button:active{transform:scale(.93)}"));
    webClient.print(F("</style></head><body>"));
    webClient.print(F("<div id='qr'></div>"));
    webClient.print(F("<div class='wrap'><div class='title'>智能家居控制系统</div><div class='subtitle'>WIFI · 智能控制</div></div>"));
    webClient.print(F("<div class='card fan'><span class='icon'>🌀</span><div class='card-title'>风扇</div><div class='card-sub'>通风设备</div><div class='btn-row'>"));
    webClient.print(F("<button class='on' onclick='sendCmd(\"/on1\")'>开</button>"));
    webClient.print(F("<button class='off' onclick='sendCmd(\"/off1\")'>关</button></div></div>"));
    webClient.print(F("<div class='card humid'><span class='icon'>💧</span><div class='card-title'>加湿器</div><div class='card-sub'>空气加湿</div><div class='btn-row'>"));
    webClient.print(F("<button class='on' onclick='sendCmd(\"/on2\")'>开</button>"));
    webClient.print(F("<button class='off' onclick='sendCmd(\"/off2\")'>关</button></div></div>"));
    webClient.print(F("<div class='card air'><span class='icon'>💡</span><div class='card-title'>电灯</div><div class='card-sub'>照明设备</div><div class='btn-row'>"));
    webClient.print(F("<button class='on' onclick='sendCmd(\"/on3\")'>开</button>"));
    webClient.print(F("<button class='off' onclick='sendCmd(\"/off3\")'>关</button></div></div>"));
    webClient.print(F("<script>function sendCmd(cmd){var xhr=new XMLHttpRequest();xhr.open('GET',cmd);xhr.send();}</script>"));
    webClient.print(F("<script src='https://cdn.jsdelivr.net/gh/davidshimjs/qrcodejs@master/qrcode.min.js' async onerror='this.remove()'></script>"));
    webClient.print(F("<script>window.onload=function(){if(window.QRCode)new QRCode(document.getElementById('qr'),{text:'http://192.168.3.19',width:96,height:96});}</script>"));
    webClient.print(F("</body></html>"));
  }
  webClient.stop();
}
