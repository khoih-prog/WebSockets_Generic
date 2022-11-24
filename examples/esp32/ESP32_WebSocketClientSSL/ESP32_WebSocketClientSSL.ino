/****************************************************************************************************************************
  ESP32_WebSocketClientSSL.ino
  For ESP32

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  Originally Created on: 24.05.2015
  Original Author: Markus Sattler
*****************************************************************************************************************************/

#if !defined(ESP32)
  #error This code is intended to run only on the ESP32 boards ! Please check your Tools->Board setting.
#endif

#define _WEBSOCKETS_LOGLEVEL_     2

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include <WebSocketsClient_Generic.h>

WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

// Deprecated echo.websocket.org to be replaced
#define WS_SERVER           "wss://echo.websocket.org"
#define SSL_PORT            443

void hexdump(const void *mem, const uint32_t& len, const uint8_t& cols = 16)
{
  const uint8_t* src = (const uint8_t*) mem;

  Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);

  for (uint32_t i = 0; i < len; i++)
  {
    if (i % cols == 0)
    {
      Serial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
    }

    Serial.printf("%02X ", *src);
    src++;
  }

  Serial.printf("\n");
}

void webSocketEvent(const WStype_t& type, uint8_t * payload, const size_t& length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");

      break;

    case WStype_CONNECTED:
      Serial.printf("[WSc] Connected to url: %s\n", payload);

      // send message to server when Connected
      webSocket.sendTXT("Connected");

      break;

    case WStype_TEXT:
      Serial.printf("[WSc] get text: %s\n", payload);

      // send message to server
      webSocket.sendTXT("message here");

      break;

    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      hexdump(payload, length);

      // send data to server
      webSocket.sendBIN(payload, length);

      break;

    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:

      break;

    default:
      break;
  }
}

void setup()
{
  // Serial.begin(921600);
  Serial.begin(115200);

  while (!Serial);

  delay(200);

  Serial.print("\nStarting ESP32_WebSocketClientSSL on ");
  Serial.println(ARDUINO_BOARD);
  Serial.println(WEBSOCKETS_GENERIC_VERSION);

  //Serial.setDebugOutput(true);

  WiFiMulti.addAP("SSID", "passpasspass");

  //WiFi.disconnect();
  while (WiFiMulti.run() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println();

  // Client address
  Serial.print("WebSockets Client started @ IP address: ");
  Serial.println(WiFi.localIP());

  // server address, port and URL
  Serial.print("Connecting to WebSockets Server @ ");
  Serial.println(WS_SERVER);

  webSocket.beginSSL(WS_SERVER, SSL_PORT);
  webSocket.onEvent(webSocketEvent);

  // server address, port and URL
  Serial.print("Connected to WebSockets Server @ ");
  Serial.println(WS_SERVER);
}

void loop()
{
  webSocket.loop();
}
