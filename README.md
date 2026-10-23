## 项目介绍
零基础基于ESP8266（NodeMCU）WiFi模块开发的分布式智能家居控制系统。采用一主板、多副板主从架构，主板内置Web网页服务器，前端使用AJAX异步请求实现页面无刷新交互；主板通过局域网HTTP协议发送控制指令，远程控制各个副板上的继电器，以此驱动风扇、加湿器、空气净化器等市电电器。

## 项目功能
- 网页控制面板，AJAX异步通信，操作时页面不用刷新
- 主板和多块ESP8266副板局域网WiFi TCP/HTTP通信
- 单路继电器开关控制，支持常开NO模式
- 可以分别独立控制多个外设：风扇、台灯、加湿器、空气净化器
- 上电继电器默认关闭，电平逻辑可灵活调换
- 局域网内任意设备打开网页即可访问控制界面

## 硬件模块
- ESP8266 NodeMCU（ESP-12E / ESP-12F）：WiFi通信、代码运行
- 5V继电器模块：高低电平控制强电通断
- 直流电源：给ESP8266和继电器供电

## 用到技术
C语言、Arduino框架、WiFiServer、WiFiClient、TCP、HTTP GET请求、AJAX、HTML、CSS、局域网Socket通信

## ⚠️ 烧录与使用注意事项
1. 提前安装Arduino IDE和CH340驱动。Arduino在官网直接下载，驱动安装包B站有大量教程资源。
2. 打开Arduino，左上角文件 → 首选项（Preferences，快捷键 Ctrl+,），找到【附加开发板管理器网址(Additional Boards Manager URLs)】，把(http://arduino.esp8266.com/stable/package_esp8266com_index.json)粘贴进去，点击OK保。
3. 打开上方工具（Tools），打开串口监视器（serial monitor），波特率设置为115200。
4. 在上方help(不要点开)下面有个方框，点开下面有个 select other board and port, 点开，搜索 "NodeMCU 1.0"，选中对应开发板；端口选择你插上USB后新出现的COM口。
> 如果看不到COM口：去电脑设备管理器查看串口，缺少就重装CH340驱动。
5. 将对应代码复制到Arduino编辑器，先点击验证（Verify）编译，编译无报错后再点击上传（Upload）。
