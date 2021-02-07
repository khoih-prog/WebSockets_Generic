/****************************************************************************************************************************
  SAMD_Blynk_WM_Alexa.ino
  For SAMD21/SAMD51 using WiFiNINA Shield/Module
  
  Blynk_WiFiNINA_WM is a library for the Mega, Teensy, SAM DUE, nRF52, STM32 and SAMD boards
  (https://github.com/khoih-prog/Blynk_WiFiNINA_WM) to enable easy configuration/reconfiguration and
  autoconnect/autoreconnect of WiFiNINA/Blynk
  
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

#define RELAY_PIN_1       5         // Pin D1 mapped to pin GPIO5 of ESP8266

#include <IPAddress.h>

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
WidgetLED LED_D2(V2);
WidgetLED LED_D3(V3);
WidgetLED LED_D4(V7);
WidgetLED LED_D5(V5);
WidgetLED LED_D6(V6);

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
  //delay(1000);

  Serial.println("\nStart Blynk_WiFiNINA_WM using WiFiNINA_Shield on " + String(BOARD_NAME));
  Serial.println(WEBSOCKETS_GENERIC_VERSION);

#if USE_BLYNK_WM
  Serial.println(F("Start Blynk_WM"));
  Blynk.setConfigPortalIP(IPAddress(192, 168, 120, 1));
  //Blynk.setConfigPortal("SAMD", "MySAMD");
  //Blynk.begin(SAMD-WiFiNINA");
  Blynk.begin(HOST_NAME);
#else
  Serial.println(F("Start Blynk"));
  Blynk.begin(auth, ssid, pass, BlynkServer.c_str(), BLYNK_SERVER_HARDWARE_PORT);
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

#if (USE_BLYNK_WM && USE_DYNAMIC_PARAMETERS)
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
