/****************************************************************************************************************************
  Teensy_WebSocketServerHttpHeaderValidation_QNEthernet.ino
  For Teensy 4.1 boards using QNEthernet Shield/Module

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  First created on: 08.06.2016
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


#include <EthernetWebServer.h>

#include <WebSocketsServer_Generic.h>

// Change pins according to your boards
#define RED_LED     7
#define GREEN_LED   6
#define BLUE_LED    5

EthernetWebServer server(80);

WebSocketsServer webSocket = WebSocketsServer(81);

const unsigned long int validSessionId = 12345; //some arbitrary value to act as a valid sessionId


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

  Serial.print("\nStarting Teensy_WebSocketServerHttpHeaderValidation_QNEthernet on ");
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
