/****************************************************************************************************************************
   Generic_WebSocketServerHttpHeaderValidation_WiFiNINA.ino
   For Generic boards using WiFiNINA Shield/Module

   Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
   to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

   Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
   Licensed under MIT license
   Version: 2.2.2

   Originally Created on: 08.06.2016
   Original Author: Markus Sattler

   Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
   2.1.3   K Hoang      15/05/2020 Initial porting to support SAMD21, SAMD51, nRF52 boards, such as AdaFruit Feather nRF52832,
                                  nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, etc.
   2.2.1   K Hoang      18/05/2020 Bump up to sync with v2.2.1 of original WebSockets library
   2.2.2   K Hoang      25/05/2020 Add support to Teensy, SAM DUE and STM32. Enable WebSocket Server for new supported boards.
 *****************************************************************************************************************************/

#define _WEBSOCKETS_LOGLEVEL_     3
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
bool isCookieValid(String rawCookieHeaderValue)
{
  if (rawCookieHeaderValue.indexOf("sessionId") != -1)
  {
    String sessionIdStr = rawCookieHeaderValue.substring(rawCookieHeaderValue.indexOf("sessionId=") + 10, rawCookieHeaderValue.indexOf("|"));
    unsigned long int sessionId = strtoul(sessionIdStr.c_str(), NULL, 10);
    
    return sessionId == validSessionId;
  }

  return false;
}

/*
   The WebSocketServerHttpHeaderValFunc delegate passed to webSocket.onValidateHttpHeader
*/
bool validateHttpHeader(String headerName, String headerValue)
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

  Serial.println("\nStart Generic_WebSocketServerHttpHeaderValidation_WiFiNINA");

  Serial.println("Used/default SPI pinout:");
  Serial.print("MOSI:");
  Serial.println(MOSI);
  Serial.print("MISO:");
  Serial.println(MISO);
  Serial.print("SCK:");
  Serial.println(SCK);
  Serial.print("SS:");
  Serial.println(SS);

  for (uint8_t t = 4; t > 0; t--)
  {
    Serial.println("[SETUP] BOOT WAIT " + String(t));
    Serial.flush();
    delay(1000);
  }

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
