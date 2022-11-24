/****************************************************************************************************************************
  WT32_ETH01_SSL_SIO.ino
  For WT32_ETH01

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license
*****************************************************************************************************************************/

#if !defined(ESP32)
  #error This code is intended to run only on the ESP32-based WT32_ETH01 boards ! Please check your Tools->Board setting.
#endif

#define WEBSOCKETS_NETWORK_TYPE   NETWORK_ESP32_ETH

#include <WebServer_WT32_ETH01.h>

#include "arduino_secrets.h"
#include <ArduinoJson.h>

#define _WEBSOCKETS_LOGLEVEL_     2

#define WEBSOCKETS_NETWORK_TYPE   NETWORK_ESP32_ETH

// For transport=websocket instead of transport=polling
#define USING_STICKY_SESSION_SIO        true

#include <ArduinoJson.h>

#include <WebSocketsClient_Generic.h>
#include <SocketIOclient_Generic.h>

SocketIOclient socketIO;

// Select the IP address according to your local network
IPAddress myIP(192, 168, 2, 232);
IPAddress myGW(192, 168, 2, 1);
IPAddress mySN(255, 255, 255, 0);

// Google DNS Server IP
IPAddress myDNS(8, 8, 8, 8);

//#define USE_SSL         false
#define USE_SSL         true

#if USE_SSL
  uint16_t serverPort = SOCKET_IO_PORT; // from arduino_secrets.h
  char server[] = SOCKET_IO_SERVER_URL; // from arduino_secrets.h
#else
  // To run a local WebSocket Server
  char server[]        = "192.168.2.30";
  uint16_t serverPort =    8080;
#endif

unsigned long messageTimestamp = 0;

void socketIOEvent(const socketIOmessageType_t& type, uint8_t * payload, const size_t& length)
{
  Serial.println("Receiving Payload:") ;
  Serial.println((char*)payload);

  switch (type)
  {
    case sIOtype_DISCONNECT:
      Serial.println("[IOc] Disconnected");

      break;

    case sIOtype_CONNECT:
      Serial.print("[IOc] Connected to url: ");
      Serial.println((char*)payload);

      // join default namespace (no auto join in Socket.IO V3)
      socketIO.send(sIOtype_CONNECT, "/");

      break;

    case sIOtype_EVENT:
      Serial.print("[IOc] Get event: ");
      Serial.println((char*)payload);

      break;

    case sIOtype_ACK:
      Serial.print("[IOc] Get ack: ");
      Serial.println(length);

      //hexdump(payload, length);

      break;

    case sIOtype_ERROR:
      Serial.print("[IOc] Get error: ");
      Serial.println(length);

      //hexdump(payload, length);

      break;

    case sIOtype_BINARY_EVENT:
      Serial.print("[IOc] Get binary: ");
      Serial.println(length);

      //hexdump(payload, length);

      break;

    case sIOtype_BINARY_ACK:
      Serial.print("[IOc] Get binary ack: ");
      Serial.println(length);

      //hexdump(payload, length);

      break;

    case sIOtype_PING:
      Serial.println("[IOc] Get PING");

      break;

    case sIOtype_PONG:
      Serial.println("[IOc] Get PONG");

      break;

    default:
      break;
  }
}

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  while (!Serial);

  delay(2000);

  // To be called before ETH.begin()
  WT32_ETH01_onEvent();

  //bool begin(uint8_t phy_addr=ETH_PHY_ADDR, int power=ETH_PHY_POWER, int mdc=ETH_PHY_MDC, int mdio=ETH_PHY_MDIO,
  //           eth_phy_type_t type=ETH_PHY_TYPE, eth_clock_mode_t clk_mode=ETH_CLK_MODE);
  //ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_TYPE, ETH_CLK_MODE);
  ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER);

  // Static IP, leave without this line to get IP via DHCP
  //bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = 0, IPAddress dns2 = 0);
  //ETH.config(myIP, myGW, mySN, myDNS);

  WT32_ETH01_waitForConnect();

  Serial.println();

  // Client address
  Serial.print("WebSockets Client started @ IP address: ");
  Serial.println(ETH.localIP());

  // give the Ethernet shield a second to initialize:
  delay(2000);

  // server address, port and URL
  Serial.print("Connecting to WebSockets Server @ IP address: ");
  Serial.print(server);
  Serial.print(", port: ");
  Serial.println(serverPort);

  // setReconnectInterval to 10s, new from v2.5.1 to avoid flooding server. Default is 0.5s
  socketIO.setReconnectInterval(10000);

  //socketIO.setExtraHeaders(SOCKET_IO_AUTHORIZATION);  // from arduino_secrets.h
  socketIO.setExtraHeaders("Authorization: 1234567890");

#if USE_SSL
  socketIO.beginSSL(server, serverPort);
#else
  socketIO.begin(server, serverPort, "/");
#endif

  socketIO.onEvent(socketIOEvent);
}

void loop()
{

  socketIO.loop();

  uint64_t now = millis();

  if (now - messageTimestamp > 30000)
  {
    messageTimestamp = now;

    // create JSON message for Socket.IO (event)
    DynamicJsonDocument doc(1024);
    JsonArray array = doc.to<JsonArray>();

    // add event name    // Hint: socket.on('event_name', ....
    array.add("subscribe");

    // add payload (parameters) for the event
    JsonObject param = array.createNestedObject();

    param["userName"] = "Default Test";
    param["sessionId"] = "th-VIZsyFibjFj1xGLPL4fk";

    // JSON to String (serializion)
    String output;
    output += "0";  // same as Web
    serializeJson(doc, output);

    // Send event
    socketIO.sendEVENT(output);

    // Print JSON for debugging
    Serial.println(output);
  }
}
