/****************************************************************************************************************************
  ESP8266_WebSocketServerFragmentation.ino
  For ESP8266

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  Originally Created on: 22.05.2018
  Original Author: Markus Sattler
*****************************************************************************************************************************/

#if !defined(ESP8266)
  #error This code is intended to run only on the ESP8266 boards ! Please check your Tools->Board setting.
#endif

#define _WEBSOCKETS_LOGLEVEL_     2

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <WebSocketsServer_Generic.h>

#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;

WebSocketsServer webSocket = WebSocketsServer(81);

String fragmentBuffer = "";

void webSocketEvent(const uint8_t& num, const WStype_t& type, uint8_t * payload, const size_t& length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    case WStype_CONNECTED:
    {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

      // send message to client
      webSocket.sendTXT(num, "Connected");
    }
    break;

    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      break;

    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);
      break;

    // Fragmentation / continuation opcode handling
    // case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT_TEXT_START:
      fragmentBuffer = (char*)payload;
      Serial.printf("[%u] get start start of Textfragment: %s\n", num, payload);
      break;

    case WStype_FRAGMENT:
      fragmentBuffer += (char*)payload;
      Serial.printf("[%u] get Textfragment : %s\n", num, payload);
      break;

    case WStype_FRAGMENT_FIN:
      fragmentBuffer += (char*)payload;
      Serial.printf("[%u] get end of Textfragment: %s\n", num, payload);
      Serial.printf("[%u] full frame: %s\n", num, fragmentBuffer.c_str());
      break;

    default:
      break;
  }
}

void setup()
{
  // Serial.begin(921600);
  Serial.begin(115200);

  Serial.print("\nStart ESP8266_WebSocketServerFragmentation on ");
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

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // server address, port and URL
  Serial.print("WebSockets Server started @ IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  webSocket.loop();
}
