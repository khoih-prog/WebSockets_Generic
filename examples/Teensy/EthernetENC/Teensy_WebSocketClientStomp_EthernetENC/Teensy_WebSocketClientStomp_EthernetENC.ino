/****************************************************************************************************************************
  Teensy_WebSocketClientStomp_W5500.ino
  For Teensy boards using W5x00 Ethernet Shield/Module

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  Example for connecting and maintining a connection with a SockJS+STOMP websocket connection.
  In this example, we connect to a Spring application (see https://docs.spring.io/spring/docs/current/spring-framework-reference/html/websocket.html).

  First created on: 25.09.2017
  Original Author: Martin Becker <mgbckr>, Contact: becker@informatik.uni-wuerzburg.de
 *****************************************************************************************************************************/

#if ( defined(CORE_TEENSY) )
  // Default pin 10 to SS/CS
  #define USE_THIS_SS_PIN       10

  #if defined(__IMXRT1062__)
    // For Teensy 4.1/4.0
    #if defined(ARDUINO_TEENSY41)
      #define BOARD_TYPE      "TEENSY 4.1"
      // Use true for NativeEthernet Library, false if using other Ethernet libraries
      #define USE_NATIVE_ETHERNET     false   //true
    #elif defined(ARDUINO_TEENSY40)
      #define BOARD_TYPE      "TEENSY 4.0"
    #else
      #define BOARD_TYPE      "TEENSY 4.x"
    #endif
  #elif defined(__MK66FX1M0__)
    #define BOARD_TYPE "Teensy 3.6"
  #elif defined(__MK64FX512__)
    #define BOARD_TYPE "Teensy 3.5"
  #elif defined(__MKL26Z64__)
    #define BOARD_TYPE "Teensy LC"
  #elif defined(__MK20DX256__)
    #define BOARD_TYPE "Teensy 3.2" // and Teensy 3.1 (obsolete)
  #elif defined(__MK20DX128__)
    #define BOARD_TYPE "Teensy 3.0"
  #elif defined(__AVR_AT90USB1286__)
    #error Teensy 2.0++ not supported yet
  #elif defined(__AVR_ATmega32U4__)
    #error Teensy 2.0 not supported yet
  #else
    // For Other Boards
    #define BOARD_TYPE      "Unknown Teensy Board"
  #endif
#else
  #error This code is intended to run only on the Teensy boards ! Please check your Tools->Board setting.
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME    BOARD_TYPE
#endif

#define _WEBSOCKETS_LOGLEVEL_     2

#define USE_UIP_ETHERNET        false

// Only one if the following to be true
#define USE_ETHERNET_GENERIC    false
#define USE_ETHERNET_ESP8266    false
#define USE_ETHERNET_ENC        true

#if ( USE_ETHERNET_GENERIC )
  #define WEBSOCKETS_NETWORK_TYPE   NETWORK_W5100
#elif (USE_ETHERNET_ENC)
  #define WEBSOCKETS_NETWORK_TYPE   NETWORK_ETHERNET_ENC
#endif

#if USE_ETHERNET_GENERIC
  #include "Ethernet_Generic.h"
  #warning Using Ethernet_Generic lib

  #define ETHERNET_LARGE_BUFFERS

  #define _ETG_LOGLEVEL_        1

  #define SHIELD_TYPE           "W5x00 using Ethernet_Generic Library"
#elif USE_ETHERNET_ESP8266
  #include "Ethernet_ESP8266.h"
  #warning Using Ethernet_ESP8266 lib
  #define SHIELD_TYPE           "W5x00 using Ethernet_ESP8266 Library"
#elif USE_ETHERNET_ENC
  #include "EthernetENC.h"
  #warning Using EthernetENC lib
  #define SHIELD_TYPE           "ENC28J60 using EthernetENC Library"
#else
  #define USE_ETHERNET_GENERIC  true
  #include "Ethernet.h"
  #warning Using Ethernet_Generic lib
  #define SHIELD_TYPE           "W5x00 using default Ethernet_Generic Library"
#endif

// Default pin 10 to SS/CS
#define USE_THIS_SS_PIN         10


#include <WebSocketsClient_Generic.h>

// SETTINGS

const char* ws_host               = "the.host.net";
const int   ws_port               = 80;

// URL for STOMP endpoint.
// For the default config of Spring's STOMP support, the default URL is "/socketentry/websocket".
const char* stompUrl            = "/socketentry/websocket"; // don't forget the leading "/" !!!


// VARIABLES

WebSocketsClient webSocket;

// Enter a MAC address and IP address for your controller below.
#define NUMBER_OF_MAC      20

byte mac[][NUMBER_OF_MAC] =
{
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x02 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x03 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x04 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x05 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x06 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x07 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x08 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x09 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0A },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0B },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0C },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0D },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0E },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0F },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x10 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x11 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x12 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x13 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x14 },
};


// Select the IP address according to your local network
IPAddress clientIP(192, 168, 2, 225);
IPAddress serverIP(192, 168, 2, 222);

// FUNCTIONS

/**
   STOMP messages need to be NULL-terminated (i.e., \0 or \u0000).
   However, when we send a String or a char[] array without specifying
   a length, the size of the message payload is derived by strlen() internally,
   thus dropping any NULL values appended to the "msg"-String.

   To solve this, we first convert the String to a NULL terminated char[] array
   via "c_str" and set the length of the payload to include the NULL value.
*/
void sendMessage(const String& msg)
{
  webSocket.sendTXT(msg.c_str(), msg.length() + 1);
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
      Serial.print("[WSc] get text: ");
      Serial.println((char *) payload);

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
      Serial.print("[WSc] get binary length: ");
      Serial.println(length);

      //hexdump(payload, length);

      // send data to server
      webSocket.sendBIN(payload, length);

      break;

    default:
      break;
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  while (!Serial);

  Serial.print("\nStart Teensy_WebSocketClientStomp_EthernetENC on ");
  Serial.print(BOARD_NAME);
  Serial.print(" with ");
  Serial.println(SHIELD_TYPE);
  Serial.println(WEBSOCKETS_GENERIC_VERSION);

  WSK_LOGWARN3(F("Board :"), BOARD_NAME, F(", setCsPin:"), USE_THIS_SS_PIN);

  WSK_LOGWARN(F("Default SPI pinout:"));
  WSK_LOGWARN1(F("MOSI:"), MOSI);
  WSK_LOGWARN1(F("MISO:"), MISO);
  WSK_LOGWARN1(F("SCK:"),  SCK);
  WSK_LOGWARN1(F("SS:"),   SS);
  WSK_LOGWARN(F("========================="));

#if !(USE_BUILTIN_ETHERNET || USE_UIP_ETHERNET)
  // For other boards, to change if necessary
#if ( USE_ETHERNET_GENERIC  || USE_ETHERNET_ENC )
  // Must use library patch for Ethernet, Ethernet2, EthernetLarge libraries
  Ethernet.init (USE_THIS_SS_PIN);

#endif  //( ( USE_ETHERNET_GENERIC || USE_ETHERNET_ENC )
#endif

  // start the ethernet connection and the server:
  // Use DHCP dynamic IP and random mac
  uint16_t index = millis() % NUMBER_OF_MAC;
  // Use Static IP
  //Ethernet.begin(mac[index], clientIP);
  Ethernet.begin(mac[index]);

  Serial.print("WebSockets Client IP address: ");
  Serial.println(Ethernet.localIP());

  // connect to websocket
  webSocket.begin(ws_host, ws_port, stompUrl);
  webSocket.setExtraHeaders(); // remove "Origin: file://" header because it breaks the connection with Spring's default websocket config
  //webSocket.setExtraHeaders("foo: I am so funny\r\nbar: not"); // some headers, in case you feel funny
  webSocket.onEvent(webSocketEvent);
}

void loop()
{
  webSocket.loop();
}
