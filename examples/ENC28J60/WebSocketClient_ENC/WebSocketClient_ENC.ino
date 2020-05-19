/****************************************************************************************************************************
   WebSocketClient_ENC.ino
   For SAMD21/SAMD51, nRF52 using ENC28J60 Shield/Module

   Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
   to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

   Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
   Licensed under MIT license
   Version: 2.1.3

   Created on: 24.05.2015
   Author: Markus Sattler

   Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
   2.1.3   K Hoang      15/05/2020 Initial porting to support SAMD21, SAMD51, nRF52 boards, such as AdaFruit Feather nRF52832,
                                  nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, etc.
 *****************************************************************************************************************************/

#define WEBSOCKETS_NETWORK_TYPE   NETWORK_ENC28J60

#include <WebSocketsClient_Generic.h>

//#include <Hash.h>

WebSocketsClient webSocket;

uint8_t mac[6] =  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x08 };

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      Serial.println("[WSc] Disconnected!");
      break;
    case WStype_CONNECTED:
      {
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
      // webSocket.sendTXT("message here");
      break;
    case WStype_BIN:
      Serial.print("[WSc] get binary length: ");
      Serial.println(length);

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

  //Configure IP address via DHCP
  Ethernet.begin(mac);

  Serial.print("Connected! IP address: ");
  Serial.println(Ethernet.localIP());

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
