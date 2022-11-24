/****************************************************************************************************************************
  STM32_WebSocketServerHttpHeaderValidation_LAN8742A.ino
  For STM32 boards using LAN8742A Ethernet Shield/Module

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  First created on: 08.06.2016
  Original Author: Markus Sattler
 *****************************************************************************************************************************/

#if !( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
       defined(STM32L0) || defined(STM32L1) || defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
       defined(STM32WB) || defined(STM32MP1) )
#error This code is designed to run on STM32F/L/H/G/WB/MP1 platform! Please check your Tools->Board setting.
#endif

#define _WEBSOCKETS_LOGLEVEL_       2

#define WEBSOCKETS_NETWORK_TYPE     NETWORK_LAN8742A
#define USE_BUILTIN_ETHERNET        true

//#warning Using LAN8742A Ethernet & STM32Ethernet lib
#define SHIELD_TYPE           "LAN8742A Ethernet & STM32Ethernet Library"

#include <EthernetWebServer_STM32.h>

#include <WebSocketsServer_Generic.h>

// Change pins according to your boards
#define RED_LED     7
#define GREEN_LED   6
#define BLUE_LED    5

EthernetWebServer server(80);

WebSocketsServer webSocket = WebSocketsServer(81);

const unsigned long int validSessionId = 12345; //some arbitrary value to act as a valid sessionId

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
IPAddress ip(192, 168, 2, 222);

// Only for W5100
#define SDCARD_CS       4

/*
   Returns a bool value as an indicator to describe whether a user is allowed to initiate a websocket upgrade
   based on the value of a cookie. This function expects the rawCookieHeaderValue to look like this "sessionId=<someSessionIdNumberValue>|"
*/
bool isCookieValid(const String& rawCookieHeaderValue)
{
  if (rawCookieHeaderValue.indexOf("sessionId") != -1)
  {
    String sessionIdStr = rawCookieHeaderValue.substring(rawCookieHeaderValue.indexOf("sessionId=") + 10,
                                                         rawCookieHeaderValue.indexOf("|"));
    unsigned long int sessionId = strtoul(sessionIdStr.c_str(), NULL, 10);

    return sessionId == validSessionId;
  }

  return false;
}

/*
   The WebSocketServerHttpHeaderValFunc delegate passed to webSocket.onValidateHttpHeader
*/
bool validateHttpHeader(const String& headerName, const String& headerValue)
{
  //assume a true response for any headers not handled by this validator
  bool valid = true;

  if (headerName.equalsIgnoreCase("Cookie"))
  {
    //if the header passed is the Cookie header, validate it according to the rules in 'isCookieValid' function
    valid = isCookieValid(headerValue);
  }

  return valid;
}

void setup()
{
  // Serial.begin(921600);
  Serial.begin(115200);

  while (!Serial);

  Serial.print("\nStart STM32_WebSocketServerHttpHeaderValidation_LAN8742A on ");
  Serial.print(BOARD_NAME);
  Serial.print(" with ");
  Serial.println(SHIELD_TYPE);
  Serial.println(WEBSOCKETS_GENERIC_VERSION);

  // start the ethernet connection and the server:
  // Use DHCP dynamic IP and random mac
  uint16_t index = millis() % NUMBER_OF_MAC;
  // Use Static IP
  //Ethernet.begin(mac[index], clientIP);
  Ethernet.begin(mac[index]);

  // server address, port and URL
  Serial.print("WebSockets Server @ IP address: ");
  Serial.println(Ethernet.localIP());

  //connecting clients must supply a valid session cookie at websocket upgrade handshake negotiation time
  const char * headerkeys[] = { "Cookie" };
  size_t headerKeyCount = sizeof(headerkeys) / sizeof(char*);
  webSocket.onValidateHttpHeader(validateHttpHeader, headerkeys, headerKeyCount);
  webSocket.begin();
}

void loop()
{
  webSocket.loop();
}
