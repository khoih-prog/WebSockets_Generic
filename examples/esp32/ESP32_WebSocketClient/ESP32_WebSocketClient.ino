/****************************************************************************************************************************
  ESP32_WebSocketClient.ino
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

WiFiMulti         WiFiMulti;
WebSocketsClient  webSocket;

#define USE_SSL         false

#if USE_SSL
  // Deprecated echo.websocket.org to be replaced
  #define WS_SERVER           "wss://echo.websocket.org"
  #define WS_PORT             443
#else
  // To run a local WebSocket Server
  //#define WS_SERVER           "192.168.2.222"
  #define WS_SERVER           "192.168.2.86"
  //#define WS_SERVER           "192.168.2.30"
  #define WS_PORT             8080
#endif

String messageToServer = String("Message from Client on ") + String(ARDUINO_BOARD);

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
      Serial.printf("[WSc] get text: %s\n", payload);

      // send message to server
      //webSocket.sendTXT("Message from ESP32");
      webSocket.sendTXT(messageToServer);

      break;

    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      hexdump(payload, length);

      // send data to server
      webSocket.sendBIN(payload, length);

      break;

    case WStype_PING:
      // pong will be send automatically
      Serial.printf("[WSc] get ping\n");

      break;

    case WStype_PONG:
      // answer to a ping we send
      Serial.printf("[WSc] get pong\n");

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

  Serial.print("\nStarting ESP32_WebSocketClient on ");
  Serial.println(ARDUINO_BOARD);
  Serial.println(WEBSOCKETS_GENERIC_VERSION);

  Serial.setDebugOutput(true);

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

  // server address, port and URL
#if USE_SSL
  webSocket.beginSSL(WS_SERVER, WS_PORT);
#else
  webSocket.begin(WS_SERVER, WS_PORT, "/");
#endif

  // event handler
  webSocket.onEvent(webSocketEvent);

  // use HTTP Basic Authorization this is optional remove if not needed
  //webSocket.setAuthorization("user", "Password");

  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);

  // start heartbeat (optional)
  // ping server every 15000 ms
  // expect pong from server within 3000 ms
  // consider connection disconnected if pong is not received 2 times
  webSocket.enableHeartbeat(15000, 3000, 2);

  // server address, port and URL
  Serial.print("Connected to WebSockets Server @ IP address: ");
  Serial.println(WS_SERVER);
}

void loop()
{
  webSocket.loop();
}
