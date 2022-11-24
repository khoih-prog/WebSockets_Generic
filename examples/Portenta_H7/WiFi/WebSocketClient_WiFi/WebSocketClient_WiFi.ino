/****************************************************************************************************************************
  WebSocketClient_WiFi.ino
  For Portenta_H7 boards using Murata WiFi Module

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  Created on: 24.05.2015
  Author: Markus Sattler
 *****************************************************************************************************************************/

#if ( ( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) ) && defined(ARDUINO_ARCH_MBED) )

  #if defined(BOARD_NAME)
    #undef BOARD_NAME
  #endif

  #if defined(CORE_CM7)
    #warning Using Portenta H7 M7 core
    #define BOARD_NAME            "PORTENTA_H7_M7"
  #else
    #warning Using Portenta H7 M4 core
    #define BOARD_NAME            "PORTENTA_H7_M4"
  #endif

  #define USE_WIFI_PORTENTA_H7  true

  #define USE_WIFI_NINA         false

  // To use the default WiFi library here
  #define USE_WIFI_CUSTOM       false

#else

  #error For Portenta_H7 only

#endif

#define _WEBSOCKETS_LOGLEVEL_     2

#define WEBSOCKETS_NETWORK_TYPE   NETWORK_PORTENTA_H7_WIFI

#include <WebSocketsClient_Generic.h>

WebSocketsClient webSocket;

int status = WL_IDLE_STATUS;

#define USE_SSL         false

#if USE_SSL
  // Deprecated echo.websocket.org to be replaced
  #define WS_SERVER           "wss://echo.websocket.org"
  #define WS_PORT             443
#else
  // To run a local WebSocket Server
  #define WS_SERVER           "192.168.2.30"
  #define WS_PORT             8080
#endif

///////please enter your sensitive data in the Secret tab/arduino_secrets.h

char ssid[] = "your_ssid";        // your network SSID (name)
char pass[] = "12345678";         // your network password (use for WPA, or use as key for WEP), length must be 8+

bool alreadyConnected = false;

void webSocketEvent(const WStype_t& type, uint8_t * payload, const size_t& length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      if (alreadyConnected)
      {
        Serial.println("[WSc] Disconnected!");
        alreadyConnected = false;
      }

      break;

    case WStype_CONNECTED:
    {
      alreadyConnected = true;

      Serial.print("[WSc] Connected to url: ");
      Serial.println((char *) payload);

      // send message to server when Connected
      webSocket.sendTXT("Connected");
    }
    break;

    case WStype_TEXT:
      Serial.print("[WSc] get text: ");
      Serial.println((char *) payload);

      // send message to server
      webSocket.sendTXT("message here");
      break;

    case WStype_BIN:
      Serial.print("[WSc] get binary length: ");
      Serial.println(length);

      // KH, To check
      // hexdump(payload, length);

      // send data to server
      webSocket.sendBIN(payload, length);
      break;

    case WStype_PING:
      // pong will be send automatically
      Serial.println("[WSc] get ping");
      break;

    case WStype_PONG:
      // answer to a ping we send
      Serial.println("[WSc] get pong");
      break;

    default:
      break;
  }
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("WebSockets Client IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup()
{
  //Initialize serial and wait for port to open:
  Serial.begin(115200);

  while (!Serial);

  Serial.print("\nStart WebSocketClient_WiFi on ");
  Serial.println(BOARD_NAME);
  Serial.println(WEBSOCKETS_GENERIC_VERSION);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Communication with WiFi module failed!");

    // don't continue
    while (true);
  }

  Serial.print(F("Connecting to SSID: "));
  Serial.println(ssid);

  status = WiFi.begin(ssid, pass);

  delay(1000);

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED)
  {
    delay(500);

    // Connect to WPA/WPA2 network
    status = WiFi.status();
  }

  printWifiStatus();

  // server address, port and URL
  Serial.print("WebSockets Server : ");
  Serial.println(WS_SERVER);

  // server address, port and URL
#if USE_SSL
  webSocket.beginSSL(WS_SERVER, WS_PORT);
#else
  webSocket.begin(WS_SERVER, WS_PORT, "/");
#endif

  // event handler
  webSocket.onEvent(webSocketEvent);

  // server address, port and URL
  Serial.print("Connected to WebSockets Server @ ");
  Serial.println(WS_SERVER);
}

void loop()
{
  webSocket.loop();
}
