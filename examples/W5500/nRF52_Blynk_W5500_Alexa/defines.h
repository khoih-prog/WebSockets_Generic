/****************************************************************************************************************************
  defines.h for nRF52_Blynk_W5500_Alexa.ino
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

#ifndef defines_h
#define defines_h

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define DEBUG_WIFI_WEBSERVER_PORT Serial

#define DRD_GENERIC_DEBUG         true
#define BLYNK_WM_DEBUG            3

#define WEBSOCKETS_NETWORK_TYPE   NETWORK_W5100

#if ( defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
        defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || defined(NRF52840_CLUE) || \
        defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
#if defined(ETHERNET_USE_NRF52)
#undef ETHERNET_USE_NRF528XX
#endif
#define ETHERNET_USE_NRF528XX         true
#define USE_DYNAMIC_PARAMETERS        true
#else
#error This code is intended to run only on the nRF52 boards ! Please check your Tools->Board setting.
#endif

#if defined(ETHERNET_USE_NRF528XX)
  #if defined(NRF52840_FEATHER)
    #define BOARD_TYPE      "NRF52840_FEATHER"
  #elif defined(NRF52832_FEATHER)
    #define BOARD_TYPE      "NRF52832_FEATHER"
  #elif defined(NRF52840_FEATHER_SENSE)
    #define BOARD_TYPE      "NRF52840_FEATHER_SENSE"
  #elif defined(NRF52840_ITSYBITSY)
    #define BOARD_TYPE      "NRF52840_ITSYBITSY"
  #elif defined(NRF52840_CIRCUITPLAY)
    #define BOARD_TYPE      "NRF52840_CIRCUITPLAY"
  #elif defined(NRF52840_CLUE)
    #define BOARD_TYPE      "NRF52840_CLUE"
  #elif defined(NRF52840_METRO)
    #define BOARD_TYPE      "NRF52840_METRO"
  #elif defined(NRF52840_PCA10056)
    #define BOARD_TYPE      "NRF52840_PCA10056"
  #elif defined(NINA_B302_ublox)
    #define BOARD_TYPE      "NINA_B302_ublox"
  #elif defined(NINA_B112_ublox)
    #define BOARD_TYPE      "NINA_B112_ublox"
  #elif defined(PARTICLE_XENON)
    #define BOARD_TYPE      "PARTICLE_XENON"
  #elif defined(ARDUINO_NRF52_ADAFRUIT)
    #define BOARD_TYPE      "ARDUINO_NRF52_ADAFRUIT"
  #else
    #define BOARD_TYPE      "nRF52 Unknown"
  #endif

#endif

#ifndef BOARD_NAME
  #define BOARD_NAME    BOARD_TYPE
#endif

// Start location in EEPROM to store config data. Default 0
// Config data Size currently is 128 bytes)
#define EEPROM_START     0

#define EEPROM_SIZE     (2048)

#define USE_BLYNK_WM      true
//#define USE_BLYNK_WM      false

#define USE_SSL             false

#if USE_BLYNK_WM

// To use faster 25MHz clock instead of defaulf 14MHz. Only for W5200 and W5500. W5100 also tested OK.
//#define USE_W5100     false

//#define USE_ETHERNET_WRAPPER    true
//#define USE_ETHERNET_WRAPPER    false

// Use true  for ENC28J60 and UIPEthernet library (https://github.com/UIPEthernet/UIPEthernet)
// Use false for W5x00 and Ethernetx library      (https://www.arduino.cc/en/Reference/Ethernet)

//#define USE_UIP_ETHERNET   true
//#define USE_UIP_ETHERNET   false

//#define USE_CUSTOM_ETHERNET     true

// Note: To rename ESP628266 Ethernet lib files to Ethernet_ESP8266.h and Ethernet_ESP8266.cpp
// In order to USE_ETHERNET_ESP8266
#if ( !defined(USE_UIP_ETHERNET) || !USE_UIP_ETHERNET )

// To override the default CS/SS pin. Don't use unless you know exactly which pin to use
//#define USE_THIS_SS_PIN   27//22  //21  //5 //4 //2 //15

// Only one if the following to be true
#define USE_ETHERNET2         false //true
#define USE_ETHERNET3         false //true
#define USE_ETHERNET_LARGE    false //true
#define USE_ETHERNET_ESP8266  false //true

#if !USE_ETHERNET_WRAPPER

#if ( USE_ETHERNET2 || USE_ETHERNET3 || USE_ETHERNET_LARGE || USE_ETHERNET_ESP8266 )
#ifdef USE_CUSTOM_ETHERNET
#undef USE_CUSTOM_ETHERNET
#endif
#define USE_CUSTOM_ETHERNET   true
#endif

#if USE_ETHERNET3
#include "Ethernet3.h"
#warning Use Ethernet3 lib
#elif USE_ETHERNET2
#include "Ethernet2.h"
#warning Use Ethernet2 lib
#elif USE_ETHERNET_LARGE
#include "EthernetLarge.h"
#warning Use EthernetLarge lib
#elif USE_ETHERNET_ESP8266
#include "Ethernet_ESP8266.h"
#warning Use Ethernet_ESP8266 lib
#elif USE_CUSTOM_ETHERNET
#include "Ethernet_XYZ.h"
#warning Use Custom Ethernet library from EthernetWrapper. You must include a library here or error.
#else
#define USE_ETHERNET          true
#include "Ethernet.h"
#warning Use Ethernet lib
#endif

// Ethernet_Shield_W5200, EtherCard, EtherSia not supported
// Select just 1 of the following #include if uncomment #define USE_CUSTOM_ETHERNET
// Otherwise, standard Ethernet library will be used for W5x00

#endif    //#if !USE_UIP_ETHERNET
#endif    //USE_ETHERNET_WRAPPER

#if USE_SSL
// Need ArduinoECCX08 and ArduinoBearSSL libraries
// Currently, error not enough memory for UNO, Mega2560. Don't use
#include <BlynkSimpleEthernetSSL_WM.h>
#else
#include <BlynkSimpleEthernet_WM.h>
#endif

#else   ////USE_BLYNK_WM

#if USE_SSL
// Need ArduinoECCX08 and ArduinoBearSSL libraries
// Currently, error not enough memory for UNO, Mega2560. Don't use
#include <BlynkSimpleEthernetSSL.h>
#else
#include <BlynkSimpleEthernet.h>
#endif

#endif    //USE_BLYNK_WM

#ifdef HOST_NAME
  #undef HOST_NAME
#endif

#define HOST_NAME   "nRF52-W5500"

#define W5100_CS        10
#define SDCARD_CS       4

#define SINRIC_WEBSERVER                "iot.sinric.com"
#define SINRIC_WEBSERVER_PORT           80
#define SINRIC_WEBSOCKET_RETRY_TIME     5000

#define HEARTBEAT_INTERVAL              300000 // 5 Minutes 

#endif      //defines_h
