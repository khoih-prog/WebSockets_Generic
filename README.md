## WebSockets_Generic

[![arduino-library-badge](https://www.ardu-badge.com/badge/WebSockets_Generic.svg?)](https://www.ardu-badge.com/WebSockets_Generic)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/WebSockets_Generic.svg)](https://github.com/khoih-prog/WebSockets_Generic/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/WebSockets_Generic/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/WebSockets_Generic.svg)](http://github.com/khoih-prog/WebSockets_Generic/issues)


WebSocket Server and Client for Arduino based on RFC6455.

#### New in v2.2.1

1. Bump up to sync with v2.2.1 of original WebSockets library

#### New in v2.1.3

1. Add support to ***nRF52*** boards, such as ***AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B30_ublox, etc.***.
2. Add support to ***SAM51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.)***.
3. Add support to ***SAMD21 (ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit CIRCUITPLAYGROUND_EXPRESS, etc.)***.


##### Supported features of RFC6455 #####
 - text frame
 - binary frame
 - connection close
 - ping
 - pong
 - continuation frame
 
## Prerequisite
 1. [`Arduino IDE 1.8.12 or later` for Arduino](https://www.arduino.cc/en/Main/Software)
 2. [`Blynk library 0.6.1 or later`](https://github.com/blynkkk/blynk-library/releases) if use Blynk
 3. [`ESP32 core 1.0.4 or later`](https://github.com/espressif/arduino-esp32/releases) for ESP32 boards
 4. [`ESP8266 core 2.6.3 or later` for Arduino](https://github.com/esp8266/Arduino#installing-with-boards-manager) for ESP8266 boards. To use ESP8266 core 2.7.1+ for LittleFS.
 5. `Arduino AVR core 1.8.2 or later` for Arduino (Use Arduino Board Manager)
 6. [`Teensy core 1.51 or later`](https://www.pjrc.com/teensy/td_download.html) for Teensy (4.0, 3.6, 3.5, 3,2, 3.1, 3.0) boards.
 7. [`Arduino SAM DUE core 1.6.12 or later`](https://www.arduino.cc/en/Guide/ArduinoDue) for SAM DUE ARM Cortex-M3 boards
 8. [`Arduino SAMD core 1.8.5 or later`](https://www.arduino.cc/en/Guide/ArduinoM0) for SAMD ARM Cortex-M0+ boards
 9. [`Adafruit SAMD core 1.5.11 or later`](https://www.adafruit.com/) for SAMD ARM Cortex-M0+ and M4 boards (Nano 33 IoT, etc.)
10. [`Adafruit nRF52 v0.20.1 or later`](https://www.adafruit.com/) for nRF52 boards such as AdaFruit Feather nRF52840 Express, NINA_B302_ublox, etc.
11. [`Arduino Core for STM32 v1.8.0 or later`](https://github.com/khoih-prog/Arduino_Core_STM32) for STM32 boards. To install go to Arduino IDE, select Boards Manager, search for ***`STM32`***
12. [`EthernetWebServer library v1.0.8+`](https://github.com/khoih-prog/EthernetWebServer) if necessary to use certain Ethernet features.
13. Depending on which Ethernet card you're using:
   - [`Ethernet library`](https://www.arduino.cc/en/Reference/Ethernet) for W5200 and W5500
   - [`Ethernet2 library`](https://github.com/khoih-prog/Ethernet2) for W5500 (Deprecated, use Arduino Ethernet library)
   - [`Ethernet3 library`](https://github.com/sstaub/Ethernet3) for W5500/WIZ550io/WIZ850io/USR-ES1 with Wiznet W5500 chip.
   - [`EthernetLarge library`](https://github.com/OPEnSLab-OSU/EthernetLarge) for W5100, W5200 and W5500.
   - [`UIPEthernet library`](https://github.com/khoih-prog/UIPEthernet) for ENC28J60
14. [`WiFiNINA_Generic library v1.5.1-final or later`](https://github.com/khoih-prog/WiFiNINA_Generic) if necessary to use WiFiNINA. To install. check [![arduino-library-badge](https://www.ardu-badge.com/badge/WiFiNINA_Generic.svg?)](https://www.ardu-badge.com/WiFiNINA_Generic)
15. [`WiFiWebServer library v1.0.1 or later`](https://github.com/khoih-prog/WiFiWebServer) if necessary to use certain WiFi/WiFiNINA features. To install, check [![arduino-library-badge](https://www.ardu-badge.com/badge/WiFiWebServer.svg?)](https://www.ardu-badge.com/WiFiWebServer)
16. [`FlashStorage_SAMD library v1.0.0`](https://github.com/khoih-prog/FlashStorage_SAMD) for SAMD21 and SAMD51 boards (ZERO, MKR, ***NANO_33_IOT***, M0, M0 Pro, ***AdaFruit Itsy-Bitsy M4***, etc.) if necessary to use certain features.
17. [`DueFlashStorage library`](https://github.com/sebnil/DueFlashStorage) for SAM DUE if necessary to use certain features.
18. [`Adafruit's LittleFS/InternalFS`](https://www.adafruit.com) for nRF52. Already included if you already installed Adafruit ***nRF52 board package*** from Boards Manager.
19. [`DoubleResetDetector_Generic v1.0.2 or later`](https://github.com/khoih-prog/DoubleResetDetector_Generic). To install. check [![arduino-library-badge](https://www.ardu-badge.com/badge/DoubleResetDetector_Generic.svg?)](https://www.ardu-badge.com/DoubleResetDetector_Generic) if necessary to use certain features.

## Installation

### Use Arduino Library Manager
The best and easiest way is to use `Arduino Library Manager`. Search for `WebSockets_Generic`, then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/WebSockets_Generic.svg?)](https://www.ardu-badge.com/WebSockets_Generic) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [WebSockets_Generic](https://github.com/khoih-prog/WebSockets_Generic) page.
2. Download the latest release `WebSockets_Generic-master.zip`.
3. Extract the zip file to `WebSockets_Generic-master` directory 
4. Copy whole `WebSockets_Generic-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.


##### Limitations #####
 - max input length is limited to the ram size and the ```WEBSOCKETS_MAX_DATA_SIZE``` define
 - max output length has no limit (the hardware is the limit)
 - Client send big frames with mask 0x00000000 (on AVR all frames)
 - continuation frame reassembly need to be handled in the application code

##### Limitations for Async #####
 - Functions called from within the context of the websocket event might not honor `yield()` and/or `delay()`.  See [this issue](https://github.com/Links2004/arduinoWebSockets/issues/58#issuecomment-192376395) for more info and a potential workaround.
 - wss / SSL is not possible.

##### Supported Hardware #####

 - ESP8266 [Arduino for ESP8266](https://github.com/esp8266/Arduino/)
 - ESP32 [Arduino for ESP32](https://github.com/espressif/arduino-esp32)
 - ESP31B
 - Particle with STM32 ARM Cortex M3
 - ATmega328 with Ethernet Shield (ATmega branch)
 - ATmega328 with enc28j60 (ATmega branch)
 - ATmega2560 with Ethernet Shield (ATmega branch)
 - ATmega2560 with enc28j60 (ATmega branch)
 
##### New support from v2.1.3
 
 - ***nRF52*** boards, such as ***AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B30_ublox, etc.***
 - ***SAM51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.) and SAM DUE***.
 - ***SAMD21 (ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit CIRCUITPLAYGROUND_EXPRESS, etc.)***

##### Note: #####

  version 2.0 and up is not compatible with AVR/ATmega, check ATmega branch.

  Arduino for AVR not supports std namespace of c++.

### wss / SSL ###

 supported for:
 - wss client on the ESP8266
 - wss / SSL is not natively supported in WebSocketsServer however it is possible to achieve secure websockets
   by running the device behind an SSL proxy. See [Nginx](examples/Nginx/esp8266.ssl.reverse.proxy.conf) for a
   sample Nginx server configuration file to enable this.

### ESP Async TCP ###

This libary can run in Async TCP mode on the ESP.

The mode can be activated in the ```WebSockets.h``` (see WEBSOCKETS_NETWORK_TYPE define).

[ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP) libary is required.

### How to use

In your code, select one of the currently supported network connection from the following list:

1. ***NETWORK_ESP8266_ASYNC*** for ESP8266 Async
2. ***NETWORK_W5100*** for W5x00 Ethernet
3. ***NETWORK_ENC28J60*** for ENC28J60 Ethernet
4. ***NETWORK_ESP32*** for ESP32 WiFi
5. ***NETWORK_ESP32_ETH*** for ESP32 Ethernet
6. ***NETWORK_WIFININA*** for WiFiNINA

then add `#define WEBSOCKETS_NETWORK_TYPE`  before `#include <WebSocketsClient_Generic.h>`

```
#define WEBSOCKETS_NETWORK_TYPE   NETWORK_WIFININA

#include <WebSocketsClient_Generic.h>
```

### High Level Client API ###

 - `begin` : Initiate connection sequence to the websocket host.
 
```
void begin(const char *host, uint16_t port, const char * url = "/", const char * protocol = "arduino");
void begin(String host, uint16_t port, String url = "/", String protocol = "arduino");
```

 - `onEvent`: Callback to handle for websocket events

```
void onEvent(WebSocketClientEvent cbEvent);
```

 - `WebSocketClientEvent`: Handler for websocket events
 
```
void (*WebSocketClientEvent)(WStype_t type, uint8_t * payload, size_t length)
```

Where `WStype_t type` is defined as:

```
typedef enum 
{
  WStype_ERROR,
  WStype_DISCONNECTED,
  WStype_CONNECTED,
  WStype_TEXT,
  WStype_BIN,
  WStype_FRAGMENT_TEXT_START,
  WStype_FRAGMENT_BIN_START,
  WStype_FRAGMENT,
  WStype_FRAGMENT_FIN,
} WStype_t;
```

### Examples

##### For WiFiNINA

1. [nRF52_Blynk_NINA_Alexa](examples/WiFiNINA/nRF52_Blynk_NINA_Alexa)
2. [SAMD_Blynk_NINA_Alexa](examples/WiFiNINA/SAMD_Blynk_NINA_Alexa)
3. [WebSocketClient_NINA](examples/WiFiNINA/WebSocketClient_NINA)
4. [WebSocketClientSocketIO_NINA](examples/WiFiNINA/WebSocketClientSocketIO_NINA)
5. [WebSocketClientStomp_NINA](examples/WiFiNINA/WebSocketClientStomp_NINA)
6. [WebSocketClientStompOverSockJs_NINA](examples/WiFiNINA/WebSocketClientStompOverSockJs_NINA)

##### For W5x00 Ethernet shield

1. [nRF52_Blynk_W5500_Alexa](examples/W5500/nRF52_Blynk_W5500_Alexa)
2. [SAMD_Blynk_W5500_Alexa](examples/W5500/SAMD_Blynk_W5500_Alexa)
3. [WebSocketClientSocketIO_W5500](examples/W5500/WebSocketClientSocketIO_W5500)
4. [WebSocketClientSocketIO_W5500](examples/W5500/WebSocketClientSocketIO_W5500)
5. [WebSocketClientStomp_W5500](examples/W5500/WebSocketClientStomp_W5500)
6. [WebSocketClientStompOverSockJs_W5500](examples/W5500/WebSocketClientStompOverSockJs_W5500)

##### For ENC28J60 Ethernet shield

1. [nRF52_Blynk_ENC_Alexa](examples/ENC28J60/nRF52_Blynk_ENC_Alexa)
2. [SAMD_Blynk_ENC_Alexa](examples/ENC28J60/SAMD_Blynk_ENC_Alexa)
3. [WebSocketClientSocketIO_ENC](examples/ENC28J60/WebSocketClientSocketIO_ENC)
4. [WebSocketClientSocketIO_ENC](examples/ENC28J60/WebSocketClientSocketIO_ENC)
5. [WebSocketClientStomp_ENC](examples/ENC28J60/WebSocketClientStomp_ENC)
6. [WebSocketClientStompOverSockJs_ENC](examples/ENC28J60/WebSocketClientStompOverSockJs_ENC)

##### For AVR board

1. [WebSocketClientAVR](examples/avr/WebSocketClientAVR)

##### For ESP32 board

1. [WebSocketClient](examples/esp32/WebSocketClient)
2. [WebSocketClientSSL](examples/esp32/WebSocketClientSSL)
3. [WebSocketServer](examples/esp32/WebSocketServer)

##### For ESP8266 board

 1. [WebSocketClient](examples/esp8266/WebSocketClient)
 2. [WebSocketClientSocketIO](examples/esp8266/WebSocketClientSocketIO)
 3. [WebSocketClientSSL](examples/esp8266/WebSocketClientSSL)
 4. [WebSocketClientStomp](examples/esp8266/WebSocketClientStomp)
 5. [WebSocketClientStompOverSockJs](examples/esp8266/WebSocketClientStompOverSockJs)
 6. [WebSocketServer](examples/esp8266/WebSocketServer)
 7. [WebSocketServerAllFunctionsDemo](examples/esp8266/WebSocketServerAllFunctionsDemo)
 8. [WebSocketServerFragmentation](examples/esp8266/WebSocketServerFragmentation)
 9. [WebSocketServerHttpHeaderValidation](examples/esp8266/WebSocketServerHttpHeaderValidation)
10. [WebSocketServer_LEDcontrol](examples/esp8266/WebSocketServer_LEDcontrol)

##### For Particle board

1. [ParticleWebSocketClient](examples/particle/ParticleWebSocketClient)

### Example [WebSocketClient_NINA](examples/WiFiNINA/WebSocketClient_NINA)

```cpp
#define WEBSOCKETS_NETWORK_TYPE   NETWORK_WIFININA

#include <WiFiNINA_Generic.h>

#include <WebSocketsClient_Generic.h>

//#include <Hash.h>

WebSocketsClient webSocket;

int status = WL_IDLE_STATUS;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "****";        // your network SSID (name)
char pass[] = "********";    // your network password (use for WPA, or use as key for WEP), length must be 8+


void webSocketEvent(WStype_t type, uint8_t * payload, size_t length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      {
        Serial.printf("[WSc] Connected to url: %s\n", payload);

        // send message to server when Connected
        webSocket.sendTXT("Connected");
      }
      break;
    case WStype_TEXT:
      Serial.printf("[WSc] get text: %s\n", payload);

      // send message to server
      // webSocket.sendTXT("message here");
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      // KH, To check
      // hexdump(payload, length);

      // send data to server
      webSocket.sendBIN(payload, length);
      break;
  }

}

void setup()
{
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Used/default SPI pinout:");
  Serial.print("MOSI:");
  Serial.println(MOSI);
  Serial.print("MISO:");
  Serial.println(MISO);
  Serial.print("SCK:");
  Serial.println(SCK);
  Serial.print("SS:");
  Serial.println(SS);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    //delay(10000);
  }

  // server address, port and URL
  webSocket.begin("192.168.2.123", 81, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);

  // use HTTP Basic Authorization this is optional remove if not needed
  webSocket.setAuthorization("user", "Password");

  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);

}

void loop()
{
  webSocket.loop();
}
```

### Debug Terminal Output when running [nRF52_Blynk_W5500_Alexa](examples/W5500/nRF52_Blynk_W5500_Alexa)

You can see the ***Adafruit NRF52840_FEATHER Express*** board, with W5500 Ethernet shield, connects to Blynk using [BlynkEthernet_WM library](https://github.com/khoih-prog/BlynkEthernet_WM). 

It also uses [WebSockets_Generic library](https://github.com/khoih-prog/WebSockets_Generic) to communicate with `SINRIC` skills, and `Alexa`, to control a relay using Alexa voice control of a `Amazon Alexa` phone APP

```
Start nRF52_Blynk_W5500_Alexa using W5x00_Shield on NRF52840_FEATHER
LittleFS Flag read = 0xd0d04321
Flag read = 0xd0d04321
No doubleResetDetected
Saving DOUBLERESETDETECTOR_FLAG to DRD file : 0xd0d01234
Saving DRD file OK
SetFlag write = 0xd0d01234
[881] ======= Start Default Config Data =======
[881] Hdr=NonSSL,BName=Air-Control
[881] Svr=account.duckdns.org,Tok=token1
[881] Svr1=blynk-cloud.com,Tok1=<<my real Blynk auth>>
[882] Prt=8080,SIP=
[882] LoadCfgFile 
[883] OK
[883] ======= Start Stored Config Data =======
[883] Hdr=W5X00,BName=Air-Control
[883] Svr=account.duckdns.org,Tok=****
[884] Svr1=blynk-cloud.com,Tok1=****
[884] Prt=8080,SIP=
[884] CCSum=0x262e,RCSum=0x262e
[886] LoadCredFile 
[886] ChkCrR: Buffer allocated, sz=37
[886] ChkCrR:pdata=****,len=36
[886] ChkCrR:pdata=****,len=24
[886] OK
[887] CrCCsum=ea7,CrRCsum=ea7
[887] Buffer freed
[887] Valid Stored Dynamic Data
[889] LoadCredFile 
[889] CrR:pdata=****,len=36
[889] CrR:pdata=****,len=24
[889] OK
[889] CrCCsum=ea7,CrRCsum=ea7
[890] Hdr=W5X00,BName=Air-Control
[890] Svr=account.duckdns.org,Tok=****
[890] Svr1=blynk-cloud.com,Tok1=****
[890] Prt=8080,SIP=
[890] MAC:FE-F8-E0-CB-D0-BD
_pinCS = 0
W5100 init, using SS_PIN_DEFAULT = 10, new ss_pin = 10, W5100Class::ss_pin = 10
W5100::init: W5500, SSIZE =4096
[2577] IP:192.168.2.89
[2577] bg: noConfigPortal = true
[2577] bg: noConfigPortal = true
[2577] bg:ECon.TryB
[2577] 
    ___  __          __
   / _ )/ /_ _____  / /__
  / _  / / // / _ \/  '_/
 /____/_/\_, /_//_/_/\_\
        /___/ v0.6.1 on Arduino

[2578] BlynkArduinoClient.connect: Connecting to account.duckdns.org:8080
[2687] Ready (ping: 3ms).
[2754] Connected to Blynk Server = account.duckdns.org, Token = ****
[2754] bg:EBCon

Your stored Credentials :
SINRIC API Key = ****
Device_ID1 = ****
[WSc] Service connected to sinric.com at url: /
Waiting for commands from sinric.com ...
Stop doubleResetDetecting
Saving to DRD file : 0xd0d04321
Saving DRD file OK
LittleFS Flag read = 0xd0d04321
ClearFlag write = 0xd0d04321
[WSc] get text: {"deviceId":"****","action":"setPowerState","value":"ON"}
Turn on for unknown device id: ****
[WSc] get text: {"deviceId":"****","action":"setPowerState","value":"OFF"}
Turn off for unknown device id: ****
[WSc] get text: {"action":"test","sender":"web"}
[WSc] received test command from sinric.com
[WSc] get text: {"action":"test","sender":"web"}
[WSc] received test command from sinric.com
[WSc] get text: {"deviceId":"****","action":"setPowerState","value":"ON"}
Turn on device id: ****
[WSc] get text: {"deviceId":"****","action":"setPowerState","value":"OFF"}
Turn off Device ID: ****
```
### Debug Terminal Output when running [SAMD_Blynk_NINA_Alexa](examples/WiFiNINA/SAMD_Blynk_NINA_Alexa)

You can see the ***Arduino SAMD NANO_33_IoT*** board, with built-in WiFiNINA, connects to Blynk using [Blynk_WiFiNINA_WM library](https://github.com/khoih-prog/Blynk_WiFiNINA_WM). 

It also uses [WebSockets_Generic library](https://github.com/khoih-prog/WebSockets_Generic) to communicate with `SINRIC` skills, and `Alexa`, to control a relay using Alexa voice control of a `Amazon Alexa` phone APP

```
Start Blynk_WiFiNINA_WM using WiFiNINA_Shield on SAMD NANO_33_IOT
Start Blynk_WM
Flag read = 0xd0d04321
No doubleResetDetected
SetFlag write = 0xd0d01234
[21425] ======= Start Default Config Data =======
[21425] Hdr=NonSSL,BrdName=Air-Control
[21426] SSID=SSID1,PW=password1
[21426] SSID1=SSID2,PW1=password2
[21426] Server=account.ddns.net,Token=token
[21426] Server1=account.duckdns.org,Token1=token1
[21426] Port=8080
[21427] ======= End Config Data =======
[21427] Hostname=SAMD-WiFiNINA
[21427] bg: noConfigPortal = true
[21427] ChkCrR:CrCCsum=ef8,CrRCsum=ef8
[21428] CrCCSum=3832,CrRCSum=3832
[21428] ======= Start Stored Config Data =======
[21428] Hdr=SHD_WiFiNINA,BrdName=Air-Control
[21428] SSID=HueNet1,PW=****
[21429] SSID1=HueNet2,PW1=****
[21429] Server=account.ddns.net,Token=****
[21429] Server1=account.duckdns.org,Token1=****
[21429] Port=8080
[21429] ======= End Config Data =======
[21430] CCSum=0x3678,RCSum=0x3678
[21430] Hdr=SHD_WiFiNINA,BrdName=Air-Control
[21430] SSID=HueNet1,PW=****
[21430] SSID1=HueNet2,PW1=****
[21430] Server=account.ddns.net,Token=****
[21431] Server1=account.duckdns.org,Token1=****
[21431] Port=8080
[21431] ======= End Config Data =======
[21431] Connecting MultiWifi...
[22192] con2WF:spentMsec=761
WiFi-begin: return1 = 3
WiFi-begin: return2 = 3
[25451] con2WF:OK
[25452] IP = 192.168.2.46, GW = 192.168.2.1, SN = 255.255.0.0
[25453] b:WOK.TryB
[25453] 
    ___  __          __
   / _ )/ /_ _____  / /__
  / _  / / // / _ \/  '_/
 /____/_/\_, /_//_/_/\_\
        /___/ v0.6.1 on Arduino

[25454] BlynkArduinoClient.connect: Connecting to account.ddns.net:8080
[25488] Ready (ping: 5ms).
[25559] Con2BlynkServer=account.ddns.net,Token=****
[25559] b:WBOK
Stop doubleResetDetecting
ClearFlag write = 0xd0d04321

Your stored Credentials :
SINRIC API Key = ****
Device_ID1 = ****
[WSc] Service connected to sinric.com at url: /
Waiting for commands from sinric.com ...
[WSc] get text: {"deviceId":"****","action":"setPowerState","value":"ON"}
Turn on device id: ****
[WSc] get text: {"deviceId":"****","action":"setPowerState","value":"OFF"}
Turn off Device ID: ****
```

#### New in v2.2.1

1. Bump up to sync with v2.2.1 of original WebSockets library

#### New in v2.1.3

1. Add support to ***nRF52*** boards, such as ***AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B30_ublox, etc.***.
2. Add support to ***SAM51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.)***.
3. Add support to ***SAMD21 (ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit CIRCUITPLAYGROUND_EXPRESS, etc.)***.
### Issues ###

Submit issues to: [WebSockets_Generic issues](https://github.com/khoih-prog/WebSockets_Generic/issues)

### Contributions and thanks

1. Based on and modified from [Markus Sattler's WebSockets](https://github.com/Links2004/arduinoWebSockets)

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library


### License and credits ###

- All credits go to original author [Markus Sattler](https://github.com/Links2004).

- The library is licensed under [LGPLv2.1](https://github.com/Links2004/arduinoWebSockets/blob/master/LICENSE) and [MIT](https://github.com/khoih-prog/WebSockets_Generic/blob/master/LICENSE)

- [libb64](http://libb64.sourceforge.net/) written by Chris Venter. It is distributed under Public Domain see [LICENSE](https://github.com/Links2004/arduinoWebSockets/blob/master/src/libb64/LICENSE).
