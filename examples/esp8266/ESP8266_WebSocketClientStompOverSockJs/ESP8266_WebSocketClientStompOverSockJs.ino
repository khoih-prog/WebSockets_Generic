/****************************************************************************************************************************
  ESP8266_WebSocketClientStompOverSockJs.ino
  For ESP8266

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  Example for connecting and maintining a connection with a SockJS+STOMP websocket connection.
  In this example, we connect to a Spring application (see https://docs.spring.io/spring/docs/current/spring-framework-reference/html/websocket.html).

  Originally Created on: 18.07.2017
  Original Author: Martin Becker <mgbckr>, Contact: becker@informatik.uni-wuerzburg.de
*****************************************************************************************************************************/

#if !defined(ESP8266)
  #error This code is intended to run only on the ESP8266 boards ! Please check your Tools->Board setting.
#endif

#define _WEBSOCKETS_LOGLEVEL_     2

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

// base URL for SockJS (websocket) connection
// The complete URL will look something like this(cf. http://sockjs.github.io/sockjs-protocol/sockjs-protocol-0.3.3.html#section-36):
// ws://<ws_host>:<ws_port>/<ws_baseurl>/<3digits>/<randomstring>/websocket
// For the default config of Spring's SockJS/STOMP support, the default base URL is "/socketentry/".
const char* ws_baseurl            = "/socketentry/"; // don't forget leading and trailing "/" !!!

// VARIABLES

WebSocketsClient webSocket;

// FUNCTIONS

void webSocketEvent(const WStype_t& type, uint8_t * payload, const size_t& length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;

    case WStype_CONNECTED:
    {
      Serial.printf("[WSc] Connected to url: %s\n",  payload);
    }
    break;

    case WStype_TEXT:
    {
      // #####################
      // handle SockJs+STOMP protocol
      // #####################

      String text = (char*) payload;

      Serial.printf("[WSc] get text: %s\n", payload);

      if (payload[0] == 'h')
      {
        Serial.println("Heartbeat!");
      }
      else if (payload[0] == 'o')
      {
        // on open connection
        const char *msg = "[\"CONNECT\\naccept-version:1.1,1.0\\nheart-beat:10000,10000\\n\\n\\u0000\"]";
        webSocket.sendTXT(msg);

      }
      else if (text.startsWith("a[\"CONNECTED"))
      {
        // subscribe to some channels

        const char *msg = "[\"SUBSCRIBE\\nid:sub-0\\ndestination:/user/queue/messages\\n\\n\\u0000\"]";
        webSocket.sendTXT(msg);
        delay(1000);

        // and send a message
        msg = "[\"SEND\\ndestination:/app/message\\n\\n{\\\"user\\\":\\\"esp\\\",\\\"message\\\":\\\"Hello!\\\"}\\u0000\"]";
        webSocket.sendTXT(msg);
        delay(1000);
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

  Serial.print("\nStart ESP8266_WebSocketClientStompOverSockJs on ");
  Serial.println(ARDUINO_BOARD);
  Serial.println(WEBSOCKETS_GENERIC_VERSION);

  // connect to WiFi
  Serial.print("Logging into WLAN: ");
  Serial.print(wlan_ssid);
  Serial.print(" ...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(wlan_ssid, wlan_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println(" success.");
  Serial.print("WebSockets Client @ IP: ");
  Serial.println(WiFi.localIP());

  // #####################
  // create socket url according to SockJS protocol (cf. http://sockjs.github.io/sockjs-protocol/sockjs-protocol-0.3.3.html#section-36)
  // #####################
  String socketUrl = ws_baseurl;
  socketUrl += random(0, 999);
  socketUrl += "/";
  socketUrl += random(0, 999999); // should be a random string, but this works (see )
  socketUrl += "/websocket";

  // connect to websocket
  webSocket.begin(ws_host, ws_port, socketUrl);
  webSocket.setExtraHeaders(); // remove "Origin: file://" header because it breaks the connection with Spring's default websocket config
  //    webSocket.setExtraHeaders("foo: I am so funny\r\nbar: not"); // some headers, in case you feel funny
  webSocket.onEvent(webSocketEvent);
}

void loop()
{
  webSocket.loop();
}
