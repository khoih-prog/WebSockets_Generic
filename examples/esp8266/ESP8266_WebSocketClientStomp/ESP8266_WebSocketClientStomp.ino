/****************************************************************************************************************************
  ESP8266_WebSocketClientStomp.ino
  For ESP8266

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  Example for connecting and maintining a connection with a STOMP websocket connection.
  In this example, we connect to a Spring application (see https://docs.spring.io/spring/docs/current/spring-framework-reference/html/websocket.html).

  Originally Created on: 25.09.2017
  Original Author: Martin Becker <mgbckr>, Contact: becker@informatik.uni-wuerzburg.de

  Version: 2.3.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  2.1.3   K Hoang      15/05/2020 Initial porting to support SAMD21, SAMD51, nRF52 boards, such as AdaFruit Feather nRF52832,
                                  nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, etc.
  2.2.1   K Hoang      18/05/2020 Bump up to sync with v2.2.1 of original WebSockets library
  2.2.2   K Hoang      25/05/2020 Add support to Teensy, SAM DUE and STM32. Enable WebSocket Server for new supported boards.
  2.2.3   K Hoang      02/08/2020 Add support to W5x00's Ethernet2, Ethernet3, EthernetLarge Libraries. 
                                  Add support to STM32F/L/H/G/WB/MP1 and Seeeduino SAMD21/SAMD51 boards.
  2.3.1   K Hoang      07/10/2020 Sync with v2.3.1 of original WebSockets library. Add ENC28J60 EthernetENC library support
*****************************************************************************************************************************/

#if !defined(ESP8266)
  #error This code is intended to run only on the ESP8266 boards ! Please check your Tools->Board setting.
#endif

#define _WEBSOCKETS_LOGLEVEL_     3

// LIBRARIES

#include <Arduino.h>
#include <Hash.h>

#include <ESP8266WiFi.h>
#include <WebSocketsClient_Generic.h>

// SETTINGS

const char* wlan_ssid             = "yourssid";
const char* wlan_password         = "somepassword";

const char* ws_host               = "the.host.net";
const int   ws_port               = 80;

// URL for STOMP endpoint.
// For the default config of Spring's STOMP support, the default URL is "/socketentry/websocket".
const char* stompUrl            = "/socketentry/websocket"; // don't forget the leading "/" !!!

// VARIABLES

WebSocketsClient webSocket;

// FUNCTIONS

/**
   STOMP messages need to be NULL-terminated (i.e., \0 or \u0000).
   However, when we send a String or a char[] array without specifying
   a length, the size of the message payload is derived by strlen() internally,
   thus dropping any NULL values appended to the "msg"-String.

   To solve this, we first convert the String to a NULL terminated char[] array
   via "c_str" and set the length of the payload to include the NULL value.
*/
void sendMessage(String & msg) 
{
  webSocket.sendTXT(msg.c_str(), msg.length() + 1);
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) 
{
  switch (type) 
  {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
      
    case WStype_CONNECTED:
      {
        Serial.printf("[WSc] Connected to url: %s\n",  payload);

        String msg = "CONNECT\r\naccept-version:1.1,1.0\r\nheart-beat:10000,10000\r\n\r\n";
        sendMessage(msg);
      }
      break;
      
    case WStype_TEXT:
      {
        // #####################
        // handle STOMP protocol
        // #####################

        String text = (char*) payload;
        Serial.printf("[WSc] get text: %s\n", payload);

        if (text.startsWith("CONNECTED")) 
        {
          // subscribe to some channels
          String msg = "SUBSCRIBE\nid:sub-0\ndestination:/user/queue/messages\n\n";
          sendMessage(msg);
          delay(1000);

          // and send a message
          msg = "SEND\ndestination:/app/message\n\n{\"user\":\"esp\",\"message\":\"Hello!\"}";
          sendMessage(msg);
          delay(1000);
        } 
        else 
        {
          // do something with messages
        }
        break;
      }
      
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      hexdump(payload, length);

      // send data to server
      // webSocket.sendBIN(payload, length);
      break;

      default:
        break;
  }
}

void setup()
{
  // Serial.begin(921600);
  Serial.begin(115200);

  Serial.println("\nStart ESP8266_WebSocketClientStomp on " + String(ARDUINO_BOARD));
  
  // connect to WiFi
  Serial.print("Logging into WLAN: "); Serial.print(wlan_ssid); Serial.print(" ...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(wlan_ssid, wlan_password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println(" success.");
  Serial.print("WebSockets Client IP: "); 
  Serial.println(WiFi.localIP());

  // connect to websocket
  webSocket.begin(ws_host, ws_port, stompUrl);
  webSocket.setExtraHeaders(); // remove "Origin: file://" header because it breaks the connection with Spring's default websocket config
  //webSocket.setExtraHeaders("foo: I am so funny\r\nbar: not"); // some headers, in case you feel funny
  webSocket.onEvent(webSocketEvent);

  // server address, port and URL
  Serial.println("Connecting to WebSockets Server @ : " + String(ws_host) + ":" + ws_port + stompUrl );
}

void loop() 
{
  webSocket.loop();
}
