/****************************************************************************************************************************
  Generic_WebSocketServer_LEDcontrol_W5500.ino
  For Generic boards using W5x00 Ethernet Shield/Module

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  First created on: 26.11.2015
  Original Author: Markus Sattler
 *****************************************************************************************************************************/

#if ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
  // Default pin 10 to SS/CS
  #define USE_THIS_SS_PIN       10
  #define BOARD_TYPE      "SAM DUE"
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME    BOARD_TYPE
#endif

#define _WEBSOCKETS_LOGLEVEL_     2

#define USE_UIP_ETHERNET        false

// Only one if the following to be true
#define USE_ETHERNET_GENERIC    true
#define USE_ETHERNET_ESP8266    false
#define USE_ETHERNET_ENC        false

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

#if ( defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) || defined(ARDUINO_GENERIC_RP2040) )
  #if defined(ETHERNET_USE_RPIPICO)
    #undef ETHERNET_USE_RPIPICO
  #endif
  #define ETHERNET_USE_RPIPICO      true
#endif

#if ETHERNET_USE_RPIPICO
  // Default pin 10 to SS/CS
  #define USE_THIS_SS_PIN         SS
#else
  // Default pin 10 to SS/CS
  #define USE_THIS_SS_PIN         10
#endif


#include <EthernetWebServer.h>

#include <WebSocketsServer_Generic.h>

// Change pins according to your boards
#define RED_LED     7
#define GREEN_LED   6
#define BLUE_LED    5

EthernetWebServer server(80);

WebSocketsServer webSocket = WebSocketsServer(81);

// Select the IP address according to your local network
IPAddress ip(192, 168, 2, 222);

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

// Only for W5100
#define SDCARD_CS       4

void webSocketEvent(const uint8_t& num, const WStype_t& type, uint8_t * payload, const size_t& length)
{
  (void) length;

  switch (type)
  {
    case WStype_DISCONNECTED:
      //Serial.println( "[" + String(num) + "] Disconnected!");
      break;

    case WStype_CONNECTED:
    {
      //IPAddress ip = webSocket.remoteIP(num);
      //Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

      // send message to client
      webSocket.sendTXT(num, "Connected");
    }
    break;

    case WStype_TEXT:
      Serial.println( "[" + String(num) + "] get Text: " + String((char *) payload));

      if (payload[0] == '#')
      {
        // we get RGB data

        // decode rgb data
        uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);

        analogWrite(RED_LED, ((rgb >> 16) & 0xFF));
        analogWrite(GREEN_LED, ((rgb >> 8) & 0xFF));
        analogWrite(BLUE_LED, ((rgb >> 0) & 0xFF));
      }

      break;

    default:
      break;
  }
}

void setup()
{
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card

  // Serial.begin(921600);
  Serial.begin(115200);

  while (!Serial);

  Serial.print("\nStart Generic_WebSocketServer_LEDcontrol_W5500 on ");
  Serial.print(BOARD_NAME);
  Serial.print(" with ");
  Serial.println(SHIELD_TYPE);
  Serial.println(WEBSOCKETS_GENERIC_VERSION);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  digitalWrite(RED_LED, 1);
  digitalWrite(GREEN_LED, 1);
  digitalWrite(BLUE_LED, 1);

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

  // server address, port and URL
  Serial.print("WebSockets Server @ IP address: ");
  Serial.println(Ethernet.localIP());

  // start webSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // handle index
  server.on("/", []()
  {
    // send index.html
    server.send(200, "text/html",
                "<html><head><script>var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']); connection.onopen = function () {  connection.send('Connect ' + new Date()); }; connection.onerror = function (error) {    console.log('WebSocket Error ', error);};connection.onmessage = function (e) {  console.log('Server: ', e.data);};function sendRGB() {  var r = parseInt(document.getElementById('r').value).toString(16);  var g = parseInt(document.getElementById('g').value).toString(16);  var b = parseInt(document.getElementById('b').value).toString(16);  if(r.length < 2) { r = '0' + r; }   if(g.length < 2) { g = '0' + g; }   if(b.length < 2) { b = '0' + b; }   var rgb = '#'+r+g+b;    console.log('RGB: ' + rgb); connection.send(rgb); }</script></head><body>LED Control:<br/><br/>R: <input id=\"r\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" oninput=\"sendRGB();\" /><br/>G: <input id=\"g\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" oninput=\"sendRGB();\" /><br/>B: <input id=\"b\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" oninput=\"sendRGB();\" /><br/></body></html>");
  });

  server.begin();

  digitalWrite(RED_LED, 0);
  digitalWrite(GREEN_LED, 0);
  digitalWrite(BLUE_LED, 0);
}

void loop()
{
  webSocket.loop();
  server.handleClient();
}
