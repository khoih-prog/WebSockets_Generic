/****************************************************************************************************************************
  WT32_ETH01_WebSocketClient.ino
  For WT32_ETH01

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  Originally Created on: 24.05.2015
  Original Author: Markus Sattler
*****************************************************************************************************************************/

#if !defined(ESP32)
  #error This code is intended to run only on the ESP32 boards ! Please check your Tools->Board setting.
#endif

#define _WEBSOCKETS_LOGLEVEL_     4

#include <WebServer_WT32_ETH01.h>     // https://github.com/khoih-prog/WebServer_WT32_ETH01

#include <WebSocketsClient_Generic.h>

WebSocketsClient  webSocket;

// Select the IP address according to your local network
IPAddress myIP(192, 168, 2, 232);
IPAddress myGW(192, 168, 2, 1);
IPAddress mySN(255, 255, 255, 0);

// Google DNS Server IP
IPAddress myDNS(8, 8, 8, 8);

bool eth_connected = false;

#define USE_SSL         false

#if USE_SSL
  #define WS_SERVER           "wss://echo.websocket.org"
  #define WS_PORT             443
#else  
  #define WS_SERVER           "ws://echo.websocket.org"
  #define WS_PORT             80
#endif

void hexdump(const void *mem, uint32_t len, uint8_t cols = 16)
{
  const uint8_t* src = (const uint8_t*) mem;

  Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);

  for (uint32_t i = 0; i < len; i++)
  {
    if (i % cols == 0)
    {
      Serial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
    }

    Serial.printf("%02X ", *src);
    src++;
  }
  Serial.printf("\n");
}

bool alreadyConnected = false;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length)
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
        webSocket.sendTXT("Connected");
      }
      break;
    case WStype_TEXT:
      Serial.printf("[WSc] get text: %s\n", payload);

      // send message to server
      webSocket.sendTXT("message here");
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      hexdump(payload, length);

      // send data to server
      webSocket.sendBIN(payload, length);
      break;

    case WStype_PING:
      // pong will be send automatically
      Serial.printf("[WSc] get ping\n");
      break;
    case WStype_PONG:
      // answer to a ping we send
      Serial.printf("[WSc] get pong\n");
      break;      
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;

    default:
      break;
  }
}

void WiFiEvent(WiFiEvent_t event)
{
  switch (event)
  {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("\nETH Started");
      //set eth hostname here
      ETH.setHostname("WT32-ETH01");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;

    case SYSTEM_EVENT_ETH_GOT_IP:
      if (!eth_connected)
      {
        Serial.print("ETH MAC: ");
        Serial.print(ETH.macAddress());
        Serial.print(", IPv4: ");
        Serial.print(ETH.localIP());

        if (ETH.fullDuplex())
        {
          Serial.print(", FULL_DUPLEX");
        }

        Serial.print(", ");
        Serial.print(ETH.linkSpeed());
        Serial.println("Mbps");
        eth_connected = true;
      }

      break;

    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;

    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("\nETH Stopped");
      eth_connected = false;
      break;

    default:
      break;
  }
}

void setup()
{
  // Serial.begin(921600);
  Serial.begin(115200);
  while (!Serial);

  Serial.print("\nStarting WT32_ETH01_WebSocketClient on " + String(ARDUINO_BOARD));
  Serial.println(" with " + String(SHIELD_TYPE));
  Serial.println(WEBSERVER_WT32_ETH01_VERSION);
  Serial.println(WEBSOCKETS_GENERIC_VERSION);

  Serial.setDebugOutput(true);

  //bool begin(uint8_t phy_addr=ETH_PHY_ADDR, int power=ETH_PHY_POWER, int mdc=ETH_PHY_MDC, int mdio=ETH_PHY_MDIO, 
  //           eth_phy_type_t type=ETH_PHY_TYPE, eth_clock_mode_t clk_mode=ETH_CLK_MODE);
  //ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_TYPE, ETH_CLK_MODE);
  ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER);

  // Static IP, leave without this line to get IP via DHCP
  //bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = 0, IPAddress dns2 = 0);
  ETH.config(myIP, myGW, mySN, myDNS);

  WiFi.onEvent(WiFiEvent);

  while (!eth_connected)
    delay(100);

  // Client address
  Serial.print("WebSockets Client started @ IP address: ");
  Serial.println(ETH.localIP());

  // server address, port and URL
  Serial.print("Connecting to WebSockets Server @ ");
  Serial.println(WS_SERVER);
  
  // server address, port and URL
#if USE_SSL
  webSocket.beginSSL(WS_SERVER, WS_PORT);
#else  
  webSocket.begin(WS_SERVER, WS_PORT, "/");
#endif

  // event handler
  webSocket.onEvent(webSocketEvent);

  // use HTTP Basic Authorization this is optional remove if not needed
  //webSocket.setAuthorization("user", "Password");

  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);

  // start heartbeat (optional)
  // ping server every 15000 ms
  // expect pong from server within 3000 ms
  // consider connection disconnected if pong is not received 2 times
  webSocket.enableHeartbeat(15000, 3000, 2);

  // server address, port and URL
  Serial.print("Connected to WebSockets Server @ IP address: ");
  Serial.println(WS_SERVER);
}

void loop()
{
  webSocket.loop();
}
