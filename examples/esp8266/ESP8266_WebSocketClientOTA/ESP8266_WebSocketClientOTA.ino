/****************************************************************************************************************************
  ESP8266_WebSocketClientOTA.ino
  For ESP8266

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  Originally Created on: 25.10.2021
  Original Author: Markus Sattler
*****************************************************************************************************************************/

#if !defined(ESP8266)
  #error This code is intended to run only on the ESP8266 boards ! Please check your Tools->Board setting.
#endif

#include <Arduino.h>
#include <ArduinoJson.h>

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <Updater.h>
#endif

#include <WiFiUdp.h>
#include <ESP8266WiFiMulti.h>

#include <WebSocketsClient_Generic.h>

#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

// Variables:
// Settable:
const char *version = "1.0.0";
const char *name    = "mydevice";

// Others:
const char *chip    = "esp8266";

uint32_t maxSketchSpace = 0;
int SketchSize          = 0;
bool ws_conn            = false;

#define WS_SERVER             "192.168.2.30"
#define WS_PORT               8081

bool alreadyConnected   = false;

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +
         String(ipAddress[1]) + String(".") +
         String(ipAddress[2]) + String(".") +
         String(ipAddress[3]);
}

void greetings_()
{
  StaticJsonDocument<200> doc;
  doc["type"] = "greetings";
  doc["mac"] = WiFi.macAddress();
  doc["ip"] = IpAddress2String(WiFi.localIP());
  doc["version"] = version;
  doc["name"] = name;
  doc["chip"] = chip;

  char data[200];
  serializeJson(doc, data);
  webSocket.sendTXT(data);
}

void register_()
{
  StaticJsonDocument<200> doc;
  doc["type"] = "register";
  doc["mac"] = WiFi.macAddress();

  char data[200];
  serializeJson(doc, data);
  webSocket.sendTXT(data);
  ws_conn = true;
}

typedef void (*CALLBACK_FUNCTION)(JsonDocument &msg);

typedef struct
{
  char  type[50];
  CALLBACK_FUNCTION  func;
} RESPONSES_STRUCT;

void OTA(JsonDocument &msg)
{
  Serial.print(F("[WSc] OTA mode: "));
  const char* go = "go";
  const char* ok = "ok";

  if (strncmp( msg["value"], go, strlen(go)) == 0 )
  {
    Serial.print(F("go\n"));
    SketchSize = int(msg["size"]);
    maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    Serial.printf("[WSc] Max sketch size: %u\n", maxSketchSpace);
    Serial.printf("[WSc] Sketch size: %d\n", SketchSize);
    Serial.setDebugOutput(true);

    if (!Update.begin(maxSketchSpace))
    {
      //start with max available size
      Update.printError(Serial);
      ESP.restart();
    }
  }
  else if (strncmp( msg["value"], ok, strlen(ok)) == 0)
  {
    Serial.print(F("OK\n"));
    register_();
  }
  else
  {
    Serial.print(F("unknown value : "));
    Serial.print(msg["value"].as<char>());
    Serial.print(F("\n"));
  }
}

void STATE(JsonDocument &msg)
{
  // Do something with message
  (void) msg;
}

RESPONSES_STRUCT responses[] =
{
  {"ota",           OTA},
  {"state",      STATE},
};

void text(uint8_t * payload, size_t length)
{
  // Convert mesage to something usable
  char msgch[length];

  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    msgch[i] = ((char)payload[i]);
  }

  msgch[length] = '\0';

  // Parse Json
  StaticJsonDocument<200> doc_in;
  DeserializationError error = deserializeJson(doc_in, msgch);

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  // Handle each TYPE of message
  int b = 0;

  for ( b = 0 ; strlen(responses[b].type) ; b++ )
  {
    if ( strncmp(doc_in["type"], responses[b].type, strlen(responses[b].type)) == 0 )
    {
      responses[b].func(doc_in);
    }
  }
}

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
      // webSocket.sendTXT("Connected");
      greetings_();
    }
    break;

    case WStype_TEXT:
      Serial.printf("[WSc] get text: %s\n", payload);

      // send message to server
      // webSocket.sendTXT("message here");
      text(payload, length);
      break;

    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);

      // hexdump(payload, length);

      if (Update.write(payload, length) != length)
      {
        Update.printError(Serial);
        ESP.restart();
      }

      yield();
      SketchSize -= length;
      Serial.printf("[WSc] Sketch size left: %u\n", SketchSize);

      if (SketchSize < 1)
      {
        if (Update.end(true))
        {
          //true to set the size to the current progress
          Serial.printf("Update Success: \nRebooting...\n");
          delay(5);
          yield();
          ESP.restart();
        }
        else
        {
          Update.printError(Serial);
          ESP.restart();
        }

        Serial.setDebugOutput(false);
      }

      // send data to server
      // webSocket.sendBIN(payload, length);
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

void setup()
{
  // Serial.begin(921600);
  Serial.begin(115200);

  while (!Serial);

  delay(200);

  Serial.print("\nStart ESP8266_WebSocketClientOTA on ");
  Serial.println(ARDUINO_BOARD);
  Serial.println(WEBSOCKETS_GENERIC_VERSION);

  //Serial.setDebugOutput(true);

  WiFiMulti.addAP("SSID", "passpasspass");

  //WiFi.disconnect();
  while (WiFiMulti.run() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println();

  // Client address
  Serial.print("WebSockets Client started @ IP address: ");
  Serial.println(WiFi.localIP());

  // server address, port and URL
  webSocket.begin(WS_SERVER, WS_PORT, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);

  // use HTTP Basic Authorization this is optional remove if not needed
  // webSocket.setAuthorization("USER", "PASS");

  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);

  // start heartbeat (optional)
  // ping server every 15000 ms
  // expect pong from server within 3000 ms
  // consider connection disconnected if pong is not received 2 times
  webSocket.enableHeartbeat(15000, 3000, 2);

  // server address, port and URL
  Serial.print("Connected to WebSockets OTA Server @ IP address: ");
  Serial.println(WS_SERVER);
}

void loop()
{
  webSocket.loop();
}
