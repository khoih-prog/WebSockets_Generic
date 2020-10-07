/****************************************************************************************************************************
  WebSocketClientSocketIO_W5500.ino
  For boards using W5x00 Ethernet Shield/Module
 
  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.
  
  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license
  Version: 2.3.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  2.1.3   K Hoang      15/05/2020 Initial porting to support SAMD21, SAMD51, nRF52 boards, such as AdaFruit Feather nRF52832,
                                  nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, etc.
  2.2.1   K Hoang      18/05/2020 Bump up to sync with v2.2.1 of original WebSockets library
  2.2.2   K Hoang      25/05/2020 Add support to Teensy, SAM DUE and STM32. Enable WebSocket Server for new supported boards.
  2.2.3   K Hoang      02/08/2020 Add support to W5x00's Ethernet2, Ethernet3, EthernetLarge Libraries. 
                                  Add support to STM32F/L/H/G/WB/MP1 and Seeeduino SAMD21/SAMD51 boards.
  2.3.1   K Hoang      07/10/2020 Sync with v2.3.1 of original WebSockets library. Add ENC28J60 EthernetENC library support
 *****************************************************************************************************************************/

#define _WEBSOCKETS_LOGLEVEL_     3

#define USE_UIP_ETHERNET        false

// Only one if the following to be true
#define USE_ETHERNET            false
#define USE_ETHERNET2           false
#define USE_ETHERNET3           false
#define USE_ETHERNET_LARGE      true
#define USE_ETHERNET_ESP8266    false
#define USE_ETHERNET_ENC        false

#if ( USE_ETHERNET2 || USE_ETHERNET3 || USE_ETHERNET_LARGE || USE_ETHERNET )
  #define WEBSOCKETS_NETWORK_TYPE   NETWORK_W5100
#elif (USE_ETHERNET_ENC)
  #define WEBSOCKETS_NETWORK_TYPE   NETWORK_ETHERNET_ENC
#endif

#if USE_ETHERNET3
  #include "Ethernet3.h"
  #warning Using Ethernet3 lib
  #define SHIELD_TYPE           "W5x00 using Ethernet3 Library"
#elif USE_ETHERNET2
  #include "Ethernet2.h"
  #warning Using Ethernet2 lib
  #define SHIELD_TYPE           "W5x00 using Ethernet2 Library"
#elif USE_ETHERNET_LARGE
  #include "EthernetLarge.h"
  #warning Using EthernetLarge lib
  #define SHIELD_TYPE           "W5x00 using EthernetLarge Library"
#elif USE_ETHERNET_ESP8266
  #include "Ethernet_ESP8266.h"
  #warning Using Ethernet_ESP8266 lib 
  #define SHIELD_TYPE           "W5x00 using Ethernet_ESP8266 Library" 
#elif USE_ETHERNET_ENC
  #include "EthernetENC.h"
  #warning Using EthernetENC lib
  #define SHIELD_TYPE           "ENC28J60 using EthernetENC Library"
#else
  #define USE_ETHERNET          true
  #include "Ethernet.h"
  #warning Using Ethernet lib
  #define SHIELD_TYPE           "W5x00 using Ethernet Library"
#endif

// Default pin 10 to SS/CS
#define USE_THIS_SS_PIN         10

#include <ArduinoJson.h>

#include <WebSocketsClient_Generic.h>
#include <SocketIOclient_Generic.h>

SocketIOclient socketIO;

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
IPAddress clientIP(192, 168, 2, 225);

// Select the IP address according to your local network
IPAddress serverIP(10, 11, 100, 100);

uint16_t  serverPort = 8880;

#define SDCARD_CS       4

void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) 
{
  switch (type) 
  {
    case sIOtype_DISCONNECT:
      Serial.println("[IOc] Disconnected");
      break;
    case sIOtype_CONNECT:
      Serial.print("[IOc] Connected to url: ");
      Serial.println((char*) payload);
      
      break;
    case sIOtype_EVENT:
      Serial.print("[IOc] Get event: ");
      Serial.println((char*) payload);
      
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
      
    default:
      break;  
  }
}

void setup() 
{
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card
  
  // Debug console
  Serial.begin(115200);
  while (!Serial);

  Serial.print("\nStart WebSocketClientSocketIO_W5500 on " + String(BOARD_NAME));
  Serial.println(" with " + String(SHIELD_TYPE));

  for (uint8_t t = 4; t > 0; t--)
  {
    Serial.println("[SETUP] BOOT WAIT " + String(t));
    Serial.flush();
    delay(1000);
  }

  WS_LOGWARN3(F("Board :"), BOARD_NAME, F(", setCsPin:"), USE_THIS_SS_PIN);

  WS_LOGWARN(F("Default SPI pinout:"));
  WS_LOGWARN1(F("MOSI:"), MOSI);
  WS_LOGWARN1(F("MISO:"), MISO);
  WS_LOGWARN1(F("SCK:"),  SCK);
  WS_LOGWARN1(F("SS:"),   SS);
  WS_LOGWARN(F("========================="));

  #if !(USE_BUILTIN_ETHERNET || USE_UIP_ETHERNET)
    // For other boards, to change if necessary
    #if ( USE_ETHERNET || USE_ETHERNET_LARGE || USE_ETHERNET2  || USE_ETHERNET_ENC )
      // Must use library patch for Ethernet, Ethernet2, EthernetLarge libraries
      Ethernet.init (USE_THIS_SS_PIN);
    
    #elif USE_ETHERNET3
      // Use  MAX_SOCK_NUM = 4 for 4K, 2 for 8K, 1 for 16K RX/TX buffer
      #ifndef ETHERNET3_MAX_SOCK_NUM
        #define ETHERNET3_MAX_SOCK_NUM      4
      #endif
    
      Ethernet.setCsPin (USE_THIS_SS_PIN);
      Ethernet.init (ETHERNET3_MAX_SOCK_NUM);
       
    #endif  //( ( USE_ETHERNET || USE_ETHERNET_LARGE || USE_ETHERNET2  || USE_ETHERNET_ENC )
  #endif

  // start the ethernet connection and the server:
  // Use DHCP dynamic IP and random mac
  uint16_t index = millis() % NUMBER_OF_MAC;
  // Use Static IP
  //Ethernet.begin(mac[index], clientIP);
  Ethernet.begin(mac[index]);
  
  Serial.print("WebSockets Client @ IP address: ");
  Serial.println(Ethernet.localIP());

  // server address, port and URL
  Serial.print("Connecting to WebSockets Server @ IP address: ");
  Serial.print(serverIP);
  Serial.print(", port: ");
  Serial.println(serverPort);

  // event handler
  socketIO.onEvent(socketIOEvent);
}

unsigned long messageTimestamp = 0;

void loop() 
{
  socketIO.loop();

  uint64_t now = millis();

  if (now - messageTimestamp > 2000) 
  {
    messageTimestamp = now;

    // creat JSON message for Socket.IO (event)
    DynamicJsonDocument doc(1024);
    JsonArray array = doc.to<JsonArray>();

    // add evnet name
    // Hint: socket.on('event_name', ....
    array.add("event_name");

    // add payload (parameters) for the event
    JsonObject param1 = array.createNestedObject();
    param1["now"]     = (uint32_t) now;

    // JSON to String (serializion)
    String output;
    serializeJson(doc, output);

    // Send event
    socketIO.sendEVENT(output);

    // Print JSON for debugging
    Serial.println(output);
  }
}
