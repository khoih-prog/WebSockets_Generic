/****************************************************************************************************************************
  Generic_WebSocketClientSocketIO_WiFiNINA.ino
  For Generic boards using WiFiNINA Shield/Module
  
  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.
  
  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license
  
  Created on: 06.06.2016
  Original Author: Markus Sattler
  
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
#define WEBSOCKETS_NETWORK_TYPE   NETWORK_WIFININA

#include <ArduinoJson.h>

#include <WebSocketsClient_Generic.h>
#include <SocketIOclient_Generic.h>

SocketIOclient socketIO;

// Select the IP address according to your local network
IPAddress clientIP(192, 168, 2, 225);

// Select the IP address according to your local network
IPAddress serverIP(10, 11, 100, 100);

uint16_t  serverPort = 8880;

int status = WL_IDLE_STATUS;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h

char ssid[] = "your_ssid";        // your network SSID (name)
char pass[] = "12345678";    // your network password (use for WPA, or use as key for WEP), length must be 8+

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


void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("WebSockets Client IP Address: ");
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

  Serial.println("\nStart Generic_WebSocketClientSocketIO_WiFiNINA on " + String(BOARD_NAME));

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
