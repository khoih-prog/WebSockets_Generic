/****************************************************************************************************************************
  Teensy_WebSocketClient_QNEthernet.ino
  For Teensy 4.1 boards using QNEthernet Shield/Module

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  Example for connecting and maintining a connection with a SockJS+STOMP websocket connection.
  In this example, we connect to a Spring application (see https://docs.spring.io/spring/docs/current/spring-framework-reference/html/websocket.html).

  Originally Created on: 24.05.2015
  Original Author: Markus Sattler
 *****************************************************************************************************************************/

#if ( defined(CORE_TEENSY) && defined(__IMXRT1062__) && defined(ARDUINO_TEENSY41) )
  // For Teensy 4.1
  #define BOARD_TYPE      "TEENSY 4.1"
  // Use true for NativeEthernet Library, false if using other Ethernet libraries
  #define USE_NATIVE_ETHERNET     false
  #define USE_QN_ETHERNET         true
#else
  #error Only Teensy 4.1 supported
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME    BOARD_TYPE
#endif

#define _WEBSOCKETS_LOGLEVEL_     2

// Only one of the following to be true.
#define USE_ETHERNET_GENERIC      false
#define USE_ETHERNET_ESP8266      false
#define USE_ETHERNET_ENC          false
#define USE_ETHERNET_LAN8742A     false
#define USE_UIP_ETHERNET          false

#define USE_CUSTOM_ETHERNET       false

#if USE_NATIVE_ETHERNET
  #define WEBSOCKETS_NETWORK_TYPE   NETWORK_NATIVEETHERNET

  #include "NativeEthernet.h"
  #warning Using NativeEthernet lib for Teensy 4.1. Must also use Teensy Packages Patch or error
  #define SHIELD_TYPE           "using NativeEthernet"
#elif USE_QN_ETHERNET
  #define WEBSOCKETS_NETWORK_TYPE   NETWORK_QN_ETHERNET

  #include "QNEthernet.h"       // https://github.com/ssilverman/QNEthernet
  using namespace qindesign::network;
  #warning Using QNEthernet lib for Teensy 4.1. Must also use Teensy Packages Patch or error
  #define SHIELD_TYPE           "using QNEthernet"
#endif

#if USE_NATIVE_ETHERNET
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

#else

#define USING_DHCP    false   //true

#if !USING_DHCP
  // Set the static IP address to use if the DHCP fails to assign
  IPAddress myIP(192, 168, 2, 222);
  IPAddress myNetmask(255, 255, 255, 0);
  IPAddress myGW(192, 168, 2, 1);
  //IPAddress mydnsServer(192, 168, 2, 1);
  IPAddress mydnsServer(8, 8, 8, 8);
#endif

#endif

#include <WebSocketsClient_Generic.h>

WebSocketsClient webSocketClient;

// SSL not working here yet.
#define USE_SSL         false

#if USE_SSL
  // Deprecated echo.websocket.org to be replaced
  #define WS_SERVER           "wss://echo.websocket.org"
  #define WS_PORT             443
#else
  // To run a local WebSocket Server
  //#define WS_SERVER           "192.168.2.30"
  #define WS_SERVER           "192.168.2.86"
  #define WS_PORT             8080
#endif

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
      webSocketClient.sendTXT("Connected");
    }

    break;

    case WStype_TEXT:

      if (alreadyConnected)
      {
        Serial.print("[WSc] get text: ");
        Serial.println((char *) payload);

        // send message to server
        webSocketClient.sendTXT("Message from Client on Teensy4.1 and QNEthernet");
      }

      break;

    case WStype_BIN:

      if (alreadyConnected)
      {
        Serial.print("[WSc] get binary length: ");
        Serial.println(length);

        // KH, To check
        // hexdump(payload, length);

        // send data to server
        webSocketClient.sendBIN(payload, length);
      }

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

void setup()
{
  // Debug console
  Serial.begin(115200);

  while (!Serial);

  Serial.print("\nStarting Teensy_WebSocketClient_QNEthernet on ");
  Serial.print(BOARD_NAME);
  Serial.print(" ");
  Serial.println(SHIELD_TYPE);
  Serial.println(WEBSOCKETS_GENERIC_VERSION);

#if USE_NATIVE_ETHERNET
  Serial.println(F("======== USE_NATIVE_ETHERNET ========"));
#elif USE_QN_ETHERNET
  Serial.println(F("=========== USE_QN_ETHERNET ==========="));
#endif

#if USE_NATIVE_ETHERNET

  // start the ethernet connection and the server:
  // Use DHCP dynamic IP and random mac
  uint16_t index = millis() % NUMBER_OF_MAC;
  // Use Static IP
  //Ethernet.begin(mac[index], ip);
  Ethernet.begin(mac[index]);

  Serial.println(F("========================="));

  Serial.print("Using mac index =");
  Serial.println(index);
  Serial.print("Connected! IP address:");
  Serial.println(Ethernet.localIP());

#else

#if USING_DHCP
  // Start the Ethernet connection, using DHCP
  Serial.print("Initialize Ethernet using DHCP => ");
  Ethernet.begin();
#else
  // Start the Ethernet connection, using static IP
  Serial.print("Initialize Ethernet using static IP => ");
  Ethernet.begin(myIP, myNetmask, myGW);
  Ethernet.setDNSServerIP(mydnsServer);
#endif

  if (!Ethernet.waitForLocalIP(5000))
  {
    Serial.println(F("Failed to configure Ethernet"));

    if (!Ethernet.linkStatus())
    {
      Serial.println(F("Ethernet cable is not connected."));
    }

    // Stay here forever
    while (true)
    {
      delay(1);
    }
  }
  else
  {
    Serial.print(F("Connected! IP address:"));
    Serial.println(Ethernet.localIP());
  }

#endif

  // server address, port and URL
#if USE_SSL
  webSocketClient.beginSSL(WS_SERVER, WS_PORT);
#else
  webSocketClient.begin(WS_SERVER, WS_PORT, "/");
#endif

  // event handler
  webSocketClient.onEvent(webSocketEvent);

  // use HTTP Basic Authorization this is optional remove if not needed
  //webSocket.setAuthorization("user", "Password");

  // try ever 5000 again if connection has failed
  webSocketClient.setReconnectInterval(5000);

  // start heartbeat (optional)
  // ping server every 15000 ms
  // expect pong from server within 3000 ms
  // consider connection disconnected if pong is not received 2 times
  webSocketClient.enableHeartbeat(15000, 3000, 2);

  // server address, port and URL
  Serial.print("Connected to WebSockets Server @ ");
  Serial.println(WS_SERVER);
}

void loop()
{
  webSocketClient.loop();
}
