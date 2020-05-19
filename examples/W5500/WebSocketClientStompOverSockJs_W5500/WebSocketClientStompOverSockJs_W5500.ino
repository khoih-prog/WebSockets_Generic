/****************************************************************************************************************************
   WebSocketClientStompOverSockJs_W5500.ino
   For SAMD21/SAMD51 using W5x00 Ethernet Shield/Module

   BlynkEthernet_WM is a library for Teensy, ESP, SAM DUE and SAMD boards, with Ethernet W5X00 or ENC28J60 shields,
   to enable easy configuration/reconfiguration and autoconnect/autoreconnect of Ethernet/Blynk
   AVR Mega and W5100 is not supported.
   Library modified from Blynk library v0.6.1 https://github.com/blynkkk/blynk-library/releases
   Built by Khoi Hoang https://github.com/khoih-prog/BlynkEthernet_WM
   Licensed under MIT license
   Version: 1.0.16

   Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
   to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

   Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
   Licensed under MIT license
   Version: 2.1.3

   Example for connecting and maintining a connection with a SockJS+STOMP websocket connection.
   In this example, we connect to a Spring application (see https://docs.spring.io/spring/docs/current/spring-framework-reference/html/websocket.html).

   Created on: 18.07.2017
   Author: Martin Becker <mgbckr>, Contact: becker@informatik.uni-wuerzburg.de

   Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
   2.1.3   K Hoang      15/05/2020 Initial porting to support SAMD21, SAMD51, nRF52 boards, such as AdaFruit Feather nRF52832,
                                  nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, etc.
 *****************************************************************************************************************************/

#define WEBSOCKETS_NETWORK_TYPE   NETWORK_W5100

#include <WebSocketsClient_Generic.h>

//#include <Hash.h>

WebSocketsClient webSocket;

uint8_t mac[6] =  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x09 };

#define SDCARD_CS       4

const char* ws_host               = "the.host.net";
const int   ws_port               = 80;

// base URL for SockJS (websocket) connection
// The complete URL will look something like this(cf. http://sockjs.github.io/sockjs-protocol/sockjs-protocol-0.3.3.html#section-36):
// ws://<ws_host>:<ws_port>/<ws_baseurl>/<3digits>/<randomstring>/websocket
// For the default config of Spring's SockJS/STOMP support, the default base URL is "/socketentry/".
const char* ws_baseurl            = "/socketentry/"; // don't forget leading and trailing "/" !!!

// FUNCTIONS

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      Serial.println("[WSc] Disconnected!");
      break;
    case WStype_CONNECTED:
      {
        Serial.print("[WSc] Connected to url: ");
        Serial.println((char *) payload);
      }
      break;
    case WStype_TEXT:
      {
        // #####################
        // handle SockJs+STOMP protocol
        // #####################

        String text = (char*) payload;

        Serial.print("[WSc] get text: ");
        Serial.println((char *) payload);

        if (payload[0] == 'h')
        {
          Serial.println("Heartbeat!");
        }
        else if (payload[0] == 'o')
        {
          // on open connection
          String msg = "[\"CONNECT\\naccept-version:1.1,1.0\\nheart-beat:10000,10000\\n\\n\\u0000\"]";

          webSocket.sendTXT(msg);
        }
        else if (text.startsWith("a[\"CONNECTED"))
        {
          // subscribe to some channels
          String msg = "[\"SUBSCRIBE\\nid:sub-0\\ndestination:/user/queue/messages\\n\\n\\u0000\"]";

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
      Serial.print("[WSc] get binary length: ");
      Serial.println(length);
      //hexdump(payload, length);

      // send data to server
      webSocket.sendBIN(payload, length);
      break;
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\nStart WebSocketClientStompOverSockJs_W5500");

  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card

  //Configure IP address via DHCP
  Ethernet.begin(mac);

  Serial.print("Connected! IP address: ");
  Serial.println(Ethernet.localIP());

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
