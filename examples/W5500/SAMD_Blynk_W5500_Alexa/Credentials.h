/****************************************************************************************************************************
  Credentials.h for SAMD_Blynk_W5500_Alexa.ino
  For SAMD21/SAMD51 using W5x00 Ethernet Shield/Module
  
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

#ifndef Credentials_h
#define Credentials_h

#include "defines.h"

#if USE_BLYNK_WM

/// Start Default Config Data //////////////////

/*
  #define BLYNK_SERVER_MAX_LEN      32
#define BLYNK_TOKEN_MAX_LEN       36

typedef struct
{
  char blynk_server[BLYNK_SERVER_MAX_LEN];
  char blynk_token [BLYNK_TOKEN_MAX_LEN];
}  Blynk_Credentials;

#define NUM_BLYNK_CREDENTIALS     2

// Configurable items besides fixed Header
#define NUM_CONFIGURABLE_ITEMS    ( 3 + (2 * NUM_BLYNK_CREDENTIALS) )

typedef struct Configuration
{
  char header         [16];
  Blynk_Credentials Blynk_Creds [NUM_BLYNK_CREDENTIALS];
  int  blynk_port;
  char static_IP      [16];
  char board_name     [24];
  int  checkSum;
} Blynk_Configuration;
*/

#define TO_LOAD_DEFAULT_CONFIG_DATA      true

#if TO_LOAD_DEFAULT_CONFIG_DATA

bool LOAD_DEFAULT_CONFIG_DATA = false;

Blynk_Configuration defaultConfig =
{
  //char header[16], dummy, not used
#if USE_SSL
  "SSL",
#else
  "NonSSL",
#endif
  // Blynk_Credentials Blynk_Creds [NUM_BLYNK_CREDENTIALS];
  // Blynk_Creds.blynk_server and Blynk_Creds.blynk_token
  "account.duckdns.org",  "token1",
  "blynk-cloud.com",     "<<my real Blynk auth>>",
  //int  blynk_port;
#if USE_SSL
  9443,
#else
  8080,
#endif
  // char static_IP      [16];
  //"192.168.2.230",
  // Use dynamic DHCP IP
  "",
  //char board_name     [24];
  "Air-Control",
  // terminate the list
  //int  checkSum, dummy, not used
  0
  /////////// End Default Config Data /////////////
};

#else

bool LOAD_DEFAULT_CONFIG_DATA = false;

Blynk_Configuration defaultConfig;

#endif    // TO_LOAD_DEFAULT_CONFIG_DATA

/////////// End Default Config Data /////////////

#else     //#if USE_BLYNK_WM

#define USE_LOCAL_SERVER      true

#if USE_LOCAL_SERVER
char auth[] = "******";
char server[] = "account.duckdns.org";
//char server[] = "192.168.2.112";
#else
char auth[] = "******";
char server[] = "blynk-cloud.com";
#endif

#define BLYNK_HARDWARE_PORT       8080

#endif      //#if USE_BLYNK_WM

#endif    //Credentials_h
