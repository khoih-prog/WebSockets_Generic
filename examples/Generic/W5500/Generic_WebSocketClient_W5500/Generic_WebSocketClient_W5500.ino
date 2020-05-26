/****************************************************************************************************************************
   Generic_WebSocketClient_W5500.ino
   For Generic boards using W5x00 Ethernet Shield/Module

   Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
   to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

   Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
   Licensed under MIT license
   Version: 2.2.2

   Example for connecting and maintining a connection with a SockJS+STOMP websocket connection.
   In this example, we connect to a Spring application (see https://docs.spring.io/spring/docs/current/spring-framework-reference/html/websocket.html).

   Originally Created on: 24.05.2015
   Original Author: Markus Sattler

   Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
   2.1.3   K Hoang      15/05/2020 Initial porting to support SAMD21, SAMD51, nRF52 boards, such as AdaFruit Feather nRF52832,
                                  nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, etc.
   2.2.1   K Hoang      18/05/2020 Bump up to sync with v2.2.1 of original WebSockets library
   2.2.2   K Hoang      25/05/2020 Add support to Teensy, SAM DUE and STM32. Enable WebSocket Server for new supported boards.
 *****************************************************************************************************************************/
#define _WEBSOCKETS_LOGLEVEL_     3

#define WEBSOCKETS_NETWORK_TYPE   NETWORK_W5100

#include <WebSocketsClient_Generic.h>

WebSocketsClient webSocketClient;

uint8_t mac[6] =  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x09 };

// Select the IP address according to your local network
IPAddress clientIP(192, 168, 2, 225);
IPAddress serverIP(192, 168, 2, 222);

// Only for W5100
#define SDCARD_CS       4

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
        webSocketClient.sendTXT("Connected");
      }
      break;
    case WStype_TEXT:
      Serial.print("[WSc] get text: ");
      Serial.println((char *) payload);

      // send message to server
      // webSocketClient.sendTXT("message here");
      break;
    case WStype_BIN:
      Serial.print("[WSc] get binary length: ");
      Serial.println(length);
      
      // KH, To check
      // hexdump(payload, length);

      // send data to server
      webSocketClient.sendBIN(payload, length);
      break;

    default:
      break;
  }
}

void setup() 
{
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card
  
  // Debug console
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\nStart Generic_WebSocketClient_W5500");

  for (uint8_t t = 4; t > 0; t--)
  {
    Serial.println("[SETUP] BOOT WAIT " + String(t));
    Serial.flush();
    delay(1000);
  }

  // start the ethernet connection and the server:
  // Use Static IP
  Ethernet.begin(mac, clientIP);
  //Configure IP address via DHCP
  //Ethernet.begin(mac);
  Serial.print("WebSockets Client IP address: ");
  Serial.println(Ethernet.localIP());

  // server address, port and URL
  Serial.print("WebSockets Server IP address: ");
  Serial.println(serverIP);
  webSocketClient.begin(serverIP, 81, "/");


  // event handler
  webSocketClient.onEvent(webSocketEvent);

  // use HTTP Basic Authorization this is optional remove if not needed
  //webSocketClient.setAuthorization("user", "Password");

  // try ever 5000 again if connection has failed
  webSocketClient.setReconnectInterval(5000);

}

void loop() 
{
  webSocketClient.loop();
}
