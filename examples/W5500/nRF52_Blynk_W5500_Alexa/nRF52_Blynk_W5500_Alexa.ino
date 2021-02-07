/****************************************************************************************************************************
  nRF52_Blynk_W5500_Alexa.ino
  For nRF52 using W5x00 Ethernet Shield/Module
  
  BlynkEthernet_WM is a library for Teensy, ESP, SAM DUE and SAMD boards, with Ethernet W5X00 or ENC28J60 shields,
  to enable easy configuration/reconfiguration and autoconnect/autoreconnect of Ethernet/Blynk
  AVR Mega and W5100 is not supported.
  Library modified from Blynk library v0.6.1 https://github.com/blynkkk/blynk-library/releases
  Built by Khoi Hoang https://github.com/khoih-prog/BlynkEthernet_WM
  Licensed under MIT license
  
  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.
  
  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license        
 *****************************************************************************************************************************/
 
#include "defines.h"
#include "Credentials.h"
#include "dynamicParams.h"

#define DEBUG_SETUP         2

//Last device status, to minimize reporting / updating traffic
//Only report / update when current status != last satus
bool lastDevice_01_ON;

#define RELAY_PIN_1       5
//#include <Arduino.h>

#include <IPAddress.h>

//#include <WebSocketsClient.h> //  get it from https://github.com/Links2004/arduinoWebSockets/releases 
#include <WebSocketsClient_Generic.h> //  get it from https://github.com/Links2004/arduinoWebSockets/releases 

#include <ArduinoJson.h>      // get it from https://arduinojson.org/ or install via Arduino library manager

//#include <StreamString.h>

WebSocketsClient webSocket;

uint64_t heartbeatTimestamp     = 0;
bool isConnected                = false;

void setPowerStateOnServer(String deviceId, String value);

//To increase no of devices as needed
void turnOn(String deviceId)
{
  if (deviceId == SINRIC_Device_ID_1) // Device ID of 1st device
  {
#if (DEBUG_SETUP > 1)
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
#endif
    digitalWrite(RELAY_PIN_1, HIGH);
  }
  else
  {
#if (DEBUG_SETUP > 1)
    Serial.print("Turn on for unknown device id: ");
    Serial.println(deviceId);
#endif
  }
}

//To increase no of devices as needed
void turnOff(String deviceId)
{
  if (deviceId == SINRIC_Device_ID_1) // Device ID of 1st device
  {
#if (DEBUG_SETUP > 1)
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
#endif
    digitalWrite(RELAY_PIN_1, LOW);
  }
  else
  {
#if (DEBUG_SETUP > 1)
    Serial.print("Turn off for unknown device id: ");
    Serial.println(deviceId);
#endif
  }
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      isConnected = false;
#if (DEBUG_SETUP > 0)
      Serial.println("[WSc] Webservice disconnected from sinric.com!");
#endif
      break;
    case WStype_CONNECTED:
      {
        isConnected = true;
#if (DEBUG_SETUP > 0)
        Serial.print("[WSc] Service connected to sinric.com at url: ");
        Serial.println((char *) payload);
        Serial.println("Waiting for commands from sinric.com ...");
#endif
      }
      break;
    case WStype_TEXT:
      {
#if (DEBUG_SETUP > 1)
        Serial.print("[WSc] get text: ");
        Serial.println((char *) payload);
#endif
        // Example payloads

        // For Switch or Light device types
        // {"deviceId": xxxx, "action": "setPowerState", value: "ON"} // https://developer.amazon.com/docs/device-apis/alexa-powercontroller.html

        // For Light device type
        // Look at the light example in github

#if (ARDUINOJSON_VERSION_MAJOR >= 6)
        DynamicJsonDocument json(1024);
        auto deserializeError = deserializeJson(json, (char*)payload);
        if ( deserializeError )
        {
          Serial.println("JSON parseObject() failed");
          return;
        }
        //serializeJson(json, Serial);
#else
        DynamicJsonBuffer jsonBuffer;
        // Parse JSON string
        JsonObject& json = jsonBuffer.parseObject((char*)payload);
        // Test if parsing succeeds.
        if (!json.success())
        {
          Serial.println("JSON parseObject() failed");
          return;
        }
#endif

        String deviceId = json ["deviceId"];
        String action = json ["action"];

        if (action == "setPowerState")
        {
          // Switch or Light
          String value = json ["value"];
          if (value == "ON")
          {
            turnOn(deviceId);
          }
          else
          {
            turnOff(deviceId);
          }
        }
        else if (action == "SetTargetTemperature")
        {
          String deviceId = json ["deviceId"];
          String action = json ["action"];
          String value = json ["value"];
        }
        else if (action == "test")
        {
#if (DEBUG_SETUP > 1)
          Serial.println("[WSc] received test command from sinric.com");
#endif
        }
      }
      break;
    case WStype_BIN:
#if (DEBUG_SETUP > 1)
      Serial.print("[WSc] get binary length: ");
      Serial.println(length);
#endif
      break;
  }
}

// If you are going to use a push button to on/off the switch manually, use this function to update the status on the server
// so it will reflect on Alexa app.
// eg: setPowerStateOnServer("deviceid", "ON")
void setPowerStateOnServer(String deviceId, String value)
{

#if (ARDUINOJSON_VERSION_MAJOR >= 6)
  DynamicJsonDocument root(1024);
#else
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
#endif

  root["deviceId"] = deviceId;
  root["action"] = "setPowerState";
  root["value"] = value;

  //StreamString databuf;
  String databuf;

#if (ARDUINOJSON_VERSION_MAJOR >= 6)
  serializeJson(root, databuf);
#else
  root.printTo(databuf);
#endif

  webSocket.sendTXT(databuf);
}

// Create VirtualLEDs bound to Virtual Pins to reflect the status of PINs
WidgetLED LED_D1(V1);

#define BLYNK_STATUS_UPDATE_INTERVAL    2000      // in millisecs => 2 secs

BlynkTimer timer;

void updateBlynkStatus( void )
{
  // Give Blynk the current status to display on user's Blynk screen
  if (digitalRead(RELAY_PIN_1))
  {
    if (!lastDevice_01_ON)
    {
      LED_D1.on();
      //Update for Alexa
      setPowerStateOnServer(SINRIC_Device_ID_1, "ON");
    }
    lastDevice_01_ON = true;
  }
  else
  {
    if (lastDevice_01_ON)
    {
      LED_D1.off();
      //Update for Alexa
      setPowerStateOnServer(SINRIC_Device_ID_1, "OFF");
    }
    lastDevice_01_ON = false;
  }
}

void myTimerEvent(void)
{
  updateBlynkStatus();
}

void heartBeatPrint(void)
{
  static int num = 1;

  if (Blynk.connected())
    Serial.print(F("B"));
  else
    Serial.print(F("F"));

  if (num == 80)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    Serial.print(F(" "));
  }
}

void check_status()
{
  static unsigned long checkstatus_timeout = 0;

#define STATUS_CHECK_INTERVAL     10000L

  // Send status report every STATUS_REPORT_INTERVAL (60) seconds: we don't need to send updates frequently if there is no status change.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    heartBeatPrint();
    checkstatus_timeout = millis() + STATUS_CHECK_INTERVAL;
  }
}

void setup()
{
  //Assuming all devices on, to be safe, we have to turn off all of them later by calling updateBlynkStatus()
  //Call only after Blynk and Alexa / SINRIC already initialized
  lastDevice_01_ON = true;
  pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, LOW);

  // Debug console
  Serial.begin(115200);
  while (!Serial);

#if ( USE_LITTLEFS || USE_SPIFFS)
  Serial.println("\nStart nRF52_Blynk_W5500_Alexa using " + String(CurrentFileFS) + " on " + String(BOARD_NAME));
#else
  Serial.println("\nStart nRF52_Blynk_W5500_Alexa on " + String(BOARD_NAME));
#endif

  Serial.println(WEBSOCKETS_GENERIC_VERSION);
  
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card

  #if USE_ETHERNET_WRAPPER

    EthernetInit();

  #else

    #if USE_ETHERNET
      LOGWARN(F("=========== USE_ETHERNET ==========="));
    #elif USE_ETHERNET2
      LOGWARN(F("=========== USE_ETHERNET2 ==========="));
    #elif USE_ETHERNET3
      LOGWARN(F("=========== USE_ETHERNET3 ==========="));
    #elif USE_ETHERNET_LARGE
      LOGWARN(F("=========== USE_ETHERNET_LARGE ==========="));
    #elif USE_ETHERNET_ESP8266
      LOGWARN(F("=========== USE_ETHERNET_ESP8266 ==========="));
    #else
      LOGWARN(F("========================="));
    #endif
   
      LOGWARN(F("Default SPI pinout:"));
      LOGWARN1(F("MOSI:"), MOSI);
      LOGWARN1(F("MISO:"), MISO);
      LOGWARN1(F("SCK:"),  SCK);
      LOGWARN1(F("SS:"),   SS);
      LOGWARN(F("========================="));
       
    #if defined(ESP8266)
      // For ESP8266, change for other boards if necessary
      #ifndef USE_THIS_SS_PIN
        #define USE_THIS_SS_PIN   D2    // For ESP8266
      #endif
      
      LOGWARN1(F("ESP8266 setCsPin:"), USE_THIS_SS_PIN);
      
      #if ( USE_ETHERNET || USE_ETHERNET_LARGE || USE_ETHERNET2 )
        // For ESP8266
        // Pin                D0(GPIO16)    D1(GPIO5)    D2(GPIO4)    D3(GPIO0)    D4(GPIO2)    D8
        // Ethernet           0                 X            X            X            X        0
        // Ethernet2          X                 X            X            X            X        0
        // Ethernet3          X                 X            X            X            X        0
        // EthernetLarge      X                 X            X            X            X        0
        // Ethernet_ESP8266   0                 0            0            0            0        0
        // D2 is safe to used for Ethernet, Ethernet2, Ethernet3, EthernetLarge libs
        // Must use library patch for Ethernet, EthernetLarge libraries
        //Ethernet.setCsPin (USE_THIS_SS_PIN);
        Ethernet.init (USE_THIS_SS_PIN);
  
      #elif USE_ETHERNET3
        // Use  MAX_SOCK_NUM = 4 for 4K, 2 for 8K, 1 for 16K RX/TX buffer
        #ifndef ETHERNET3_MAX_SOCK_NUM
          #define ETHERNET3_MAX_SOCK_NUM      4
        #endif
        
        Ethernet.setCsPin (USE_THIS_SS_PIN);
        Ethernet.init (ETHERNET3_MAX_SOCK_NUM);
 
      #endif  //( USE_ETHERNET || USE_ETHERNET2 || USE_ETHERNET3 || USE_ETHERNET_LARGE )
        
    #elif defined(ESP32)
  
      // You can use Ethernet.init(pin) to configure the CS pin
      //Ethernet.init(10);  // Most Arduino shields
      //Ethernet.init(5);   // MKR ETH shield
      //Ethernet.init(0);   // Teensy 2.0
      //Ethernet.init(20);  // Teensy++ 2.0
      //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
      //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet
      
      #ifndef USE_THIS_SS_PIN
        #define USE_THIS_SS_PIN   22    // For ESP32
      #endif
      
      LOGWARN1(F("ESP32 setCsPin:"), USE_THIS_SS_PIN);
      
      // For other boards, to change if necessary
      #if ( USE_ETHERNET || USE_ETHERNET_LARGE || USE_ETHERNET2 )
        // Must use library patch for Ethernet, EthernetLarge libraries
        // ESP32 => GPIO2,4,5,13,15,21,22 OK with Ethernet, Ethernet2, EthernetLarge
        // ESP32 => GPIO2,4,5,15,21,22 OK with Ethernet3
           
        //Ethernet.setCsPin (USE_THIS_SS_PIN);
        Ethernet.init (USE_THIS_SS_PIN);
  
      #elif USE_ETHERNET3
        // Use  MAX_SOCK_NUM = 4 for 4K, 2 for 8K, 1 for 16K RX/TX buffer
        #ifndef ETHERNET3_MAX_SOCK_NUM
          #define ETHERNET3_MAX_SOCK_NUM      4
        #endif
        
        Ethernet.setCsPin (USE_THIS_SS_PIN);
        Ethernet.init (ETHERNET3_MAX_SOCK_NUM);
              
      #endif  //( USE_ETHERNET || USE_ETHERNET2 || USE_ETHERNET3 || USE_ETHERNET_LARGE )
  
    #else   //defined(ESP8266)
      // unknown board, do nothing, use default SS = 10
      #ifndef USE_THIS_SS_PIN
        #define USE_THIS_SS_PIN   10    // For other boards
      #endif
           
      LOGWARN1(F("Unknown board setCsPin:"), USE_THIS_SS_PIN);
  
      // For other boards, to change if necessary
      #if ( USE_ETHERNET || USE_ETHERNET_LARGE || USE_ETHERNET2 )
        // Must use library patch for Ethernet, Ethernet2, EthernetLarge libraries
  
        Ethernet.init (USE_THIS_SS_PIN);
  
      #elif USE_ETHERNET3
        // Use  MAX_SOCK_NUM = 4 for 4K, 2 for 8K, 1 for 16K RX/TX buffer
        #ifndef ETHERNET3_MAX_SOCK_NUM
          #define ETHERNET3_MAX_SOCK_NUM      4
        #endif
        
        Ethernet.setCsPin (USE_THIS_SS_PIN);
        Ethernet.init (ETHERNET3_MAX_SOCK_NUM);
                        
      #endif  //( USE_ETHERNET || USE_ETHERNET2 || USE_ETHERNET3 || USE_ETHERNET_LARGE )
      
    #endif    //defined(ESP8266)
  
  
  #endif  //USE_ETHERNET_WRAPPER

  Serial.println("\nStart nRF52_Blynk_W5500_Alexa using W5x00_Shield on " + String(BOARD_NAME));

#if USE_BLYNK_WM
  Blynk.begin();
#else
#if USE_LOCAL_SERVER
  Blynk.begin(auth, server, BLYNK_HARDWARE_PORT);
#else
  //Blynk.begin(auth);
  // You can also specify server:
  Blynk.begin(auth, server, BLYNK_HARDWARE_PORT);
#endif
#endif

  // Setup a function to be called every BLYNK_STATUS_UPDATE_INTERVAL/1000 seconds
  timer.setInterval(BLYNK_STATUS_UPDATE_INTERVAL, myTimerEvent);

  // server address, port and URL
  //webSocket.begin("iot.sinric.com", 80, "/");
  webSocket.begin(SINRIC_WEBSERVER, SINRIC_WEBSERVER_PORT, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);
  webSocket.setAuthorization("apikey", SINRIC_API_KEY);

  // try again every SINRIC_WEBSOCKET_RETRY_TIME = 5000ms if connection has failed
  // If you see 'class WebSocketsClient' has no member named 'setReconnectInterval' error update arduinoWebSockets
  webSocket.setReconnectInterval(SINRIC_WEBSOCKET_RETRY_TIME /*5000*/);

  //Call only after Blynk and Alexa / SINRIC already initialized
  updateBlynkStatus();
}

#if (USE_BLYNK_WM && USE_DYNAMIC_PARAMETERS)
void displayCredentials(void)
{
  Serial.println("\nYour stored Credentials :");

  for (int i = 0; i < NUM_MENU_ITEMS; i++)
  {
    Serial.println(String(myMenuItems[i].displayName) + " = " + myMenuItems[i].pdata);
  }
}
#endif

void loop()
{
  Blynk.run();
  timer.run();
  check_status();

  webSocket.loop();

  if (isConnected)
  {
    uint64_t now = millis();

    // Send heartbeat in order to avoid disconnections during ISP resetting IPs over night. Thanks @MacSass
    if ((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL)
    {
      heartbeatTimestamp = now;
      webSocket.sendTXT("H");
    }
  }

#if 0 //(USE_BLYNK_WM && USE_DYNAMIC_PARAMETERS)
  static bool displayedCredentials = false;

  if (!displayedCredentials)
  {
    for (int i = 0; i < NUM_MENU_ITEMS; i++)
    {
      if (!strlen(myMenuItems[i].pdata))
      {
        break;
      }

      if ( i == (NUM_MENU_ITEMS - 1) )
      {
        displayedCredentials = true;
        displayCredentials();
      }
    }
  }
#endif
}
