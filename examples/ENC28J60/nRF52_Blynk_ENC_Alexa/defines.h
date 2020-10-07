/****************************************************************************************************************************
  defines.h for nRF52_Blynk_ENC_Alexa.ino
  For nRF52 using ENC28J60 Shield/Module
  
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

#ifndef defines_h
#define defines_h

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define DEBUG_WIFI_WEBSERVER_PORT Serial

#define DRD_GENERIC_DEBUG         true
#define BLYNK_WM_DEBUG            3

#define WEBSOCKETS_NETWORK_TYPE   NETWORK_ENC28J60

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

#endif    //ETHERNET_USE_NRF528XX

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

  #if USE_SSL
    // Need ArduinoECCX08 and ArduinoBearSSL libraries
    // Currently, error not enough memory for UNO, Mega2560. Don't use
    #include <BlynkSimpleUIPEthernetSSL_WM.h>
  #else
    #include <BlynkSimpleUIPEthernet_WM.h>
  #endif

#else   ////USE_BLYNK_WM

  #if USE_SSL
    // Need ArduinoECCX08 and ArduinoBearSSL libraries
    // Currently, error not enough memory for UNO, Mega2560. Don't use
    #include <BlynkSimpleUIPEthernetSSL.h>
  #else
    #include <BlynkSimpleUIPEthernet.h>
  #endif

#endif    //USE_BLYNK_WM

#define BLYNK_HOST_NAME   "nRF52-ENC28J60"

#define SINRIC_WEBSERVER                "iot.sinric.com"
#define SINRIC_WEBSERVER_PORT           80
#define SINRIC_WEBSOCKET_RETRY_TIME     5000

#define HEARTBEAT_INTERVAL              300000 // 5 Minutes 

#endif      //defines_h
