/****************************************************************************************************************************
  Generic_WebSocketServerHttpHeaderValidation_WiFiNINA.ino
  For Generic boards using WiFiNINA Shield/Module

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  Originally Created on: 08.06.2016
  Original Author: Markus Sattler
 *****************************************************************************************************************************/

#if ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
  // Default pin 10 to SS/CS
  #define USE_THIS_SS_PIN       10
  #define BOARD_TYPE      "SAM DUE"
#elif ( defined(CORE_TEENSY) )
  #error You have to use examples written for Teensy
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME    BOARD_TYPE
#endif

#define _WEBSOCKETS_LOGLEVEL_     2
#define WEBSOCKETS_NETWORK_TYPE   NETWORK_WIFININA

#include <WebSocketsServer_Generic.h>

WebSocketsServer webSocket = WebSocketsServer(81);

int status = WL_IDLE_STATUS;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h

char ssid[] = "****";        // your network SSID (name)
char pass[] = "********";    // your network password (use for WPA, or use as key for WEP), length must be 8+

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

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("WebSockets Server IP address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup()
{
  // Serial.begin(921600);
  Serial.begin(115200);

  while (!Serial);

  Serial.print("\nStart Generic_WebSocketServerHttpHeaderValidation_WiFiNINA on ");
  Serial.println(BOARD_NAME);
  Serial.println(WEBSOCKETS_GENERIC_VERSION);

  Serial.println("Used/default SPI pinout:");
  Serial.print("MOSI:");
  Serial.println(MOSI);
  Serial.print("MISO:");
  Serial.println(MISO);
  Serial.print("SCK:");
  Serial.println(SCK);
  Serial.print("SS:");
  Serial.println(SS);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Communication with WiFi module failed!");

    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    //delay(10000);
  }

  printWifiStatus();

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
