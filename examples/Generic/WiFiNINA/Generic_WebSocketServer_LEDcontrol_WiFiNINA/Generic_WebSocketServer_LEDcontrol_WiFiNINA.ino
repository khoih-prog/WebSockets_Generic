/****************************************************************************************************************************
  Generic_WebSocketServer_LEDcontrol_WiFiNINA.ino
  For Generic boards using WiFiNINA Shield/Module

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  Originally Created on: 26.11.2015
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

#include <WiFiWebServer.h>
#include <WebSocketsServer_Generic.h>

// Change pins according to your boards
#define RED_LED     15
#define GREEN_LED   12
#define BLUE_LED    13

WiFiWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

int status = WL_IDLE_STATUS;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h

char ssid[] = "your_ssid";        // your network SSID (name)
char pass[] = "12345678";    // your network password (use for WPA, or use as key for WEP), length must be 8+

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

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("Server IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup()
{
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  digitalWrite(RED_LED, 1);
  digitalWrite(GREEN_LED, 1);
  digitalWrite(BLUE_LED, 1);

  // Serial.begin(921600);
  Serial.begin(115200);

  while (!Serial);

  Serial.print("\nStart Generic_WebSocketServer_LEDcontrol_WiFiNINA on ");
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
