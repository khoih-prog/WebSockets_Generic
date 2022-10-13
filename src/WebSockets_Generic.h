/****************************************************************************************************************************
  WebSockets_Generic.h - WebSockets Library for boards

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  @original file WebSockets.h
  @date 20.05.2015
  @author Markus Sattler

  Copyright (c) 2015 Markus Sattler. All rights reserved.
  This file is part of the WebSockets for Arduino.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  Version: 2.16.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  2.1.3   K Hoang      15/05/2020 Initial porting to support SAMD21, SAMD51, nRF52 boards, such as AdaFruit Feather nRF52832,
                                  nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, etc.
  ...
  2.11.0  K Hoang      30/11/2021 Auto detect ESP32 core version. Fix bug in examples
  2.11.1  K Hoang      12/12/2021 Add option to use transport=websocket with sticky-session SIO server
  2.12.0  K Hoang      28/01/2022 Supporting SSL for ESP32-based WT32_ETH01 boards
  2.13.0  K Hoang      14/02/2022 Add support to ESP32_S3. Add PING and PONG SocketIO events
  2.14.0  K Hoang      17/02/2022 Suppress unnecessary warnings. Optimize code by passing by reference instead of value
  2.14.1  K Hoang      18/02/2022 Fix setInsecure() bug for WIO_Terminal. Update Packages_Patches for Seeeduino
  2.14.2  K Hoang      27/03/2022 Fix Async bug for ESP8266 when using NETWORK_ESP8266_ASYNC
  2.15.0  K Hoang      06/04/2022 Use Ethernet_Generic library as default. Sync with arduinoWebSockets v2.3.6
  2.16.0  K Hoang      13/10/2022 Add WS and WSS support to RP2040W using CYW43439 WiFi
 *****************************************************************************************************************************/

#pragma once

#ifndef WEBSOCKETS_GENERIC_H_
#define WEBSOCKETS_GENERIC_H_

////////////////////////////////////////

#define WEBSOCKETS_GENERIC_VERSION            "WebSockets_Generic v2.16.0"

#define WEBSOCKETS_GENERIC_VERSION_MAJOR      2
#define WEBSOCKETS_GENERIC_VERSION_MINOR      16
#define WEBSOCKETS_GENERIC_VERSION_PATCH      0

#define WEBSOCKETS_GENERIC_VERSION_INT        2016000

////////////////////////////////////////

#include "WebSocketsDebug_Generic.h"

////////////////////////////////////////

#ifdef STM32_DEVICE
  #include <application.h>
  #define bit(b) (1UL << (b))    // Taken directly from Arduino.h
#else
  #include <Arduino.h>
  #include <IPAddress.h>
#endif

////////////////////////////////////////

#ifdef ARDUINO_ARCH_AVR
  #error Version 2.x.x currently does not support Arduino with AVR since there is no support for std namespace of c++.
  #error Use Version 1.x.x. (ATmega branch)
#else
  #ifdef max
    // KH
    #if(_WEBSOCKETS_LOGLEVEL_>3)
      #warning Undef min/max in WebSockets_Generic
    #endif
    #undef max
  #endif
  #ifdef min
    #undef min
  #endif
  
  #include <functional>
#endif

////////////////////////////////////////

#if defined(TEENSYDUINO)
  namespace std
  {
    //To avoid Teensy linker issue wth STL library
    unsigned __exidx_start;
    unsigned __exidx_end;

    // This is defined so that calling a std::function<void()> can compile when
    // size optimization is enabled.
    __attribute__((weak))
    void __throw_bad_function_call()
    {
      Serial.println("Library Exception");
      while (true) yield();
    }
  }
#endif

////////////////////////////////////////

#ifndef NODEBUG_WEBSOCKETS
  #ifdef DEBUG_ESP_PORT
    #define DEBUG_WEBSOCKETS(...)           \
    {                                       \
        DEBUG_ESP_PORT.printf(__VA_ARGS__); \
        DEBUG_ESP_PORT.flush();             \
    }
  #else
    //#define DEBUG_WEBSOCKETS(...) os_printf( __VA_ARGS__ )
  #endif
#endif

#ifndef DEBUG_WEBSOCKETS
  #define DEBUG_WEBSOCKETS(...)
  #ifndef NODEBUG_WEBSOCKETS
    #define NODEBUG_WEBSOCKETS
  #endif
#endif

//////////////////////////////////////////////////////////////

#if defined(ESP8266) || defined(ESP32)

  #define WEBSOCKETS_MAX_DATA_SIZE (15 * 1024)
  #define WEBSOCKETS_USE_BIG_MEM
  #define GET_FREE_HEAP ESP.getFreeHeap()
  // moves all Header strings to Flash (~300 Byte)
  //#define WEBSOCKETS_SAVE_RAM

  #if defined(ESP8266)
    #if(_WEBSOCKETS_LOGLEVEL_>3)
      #warning Use ESP8266 in WebSockets_Generic
    #endif
    
    #define WEBSOCKETS_YIELD()      delay(0)
    #define WEBSOCKETS_YIELD_MORE() delay(1)
    
  #elif defined(ESP32)
  
    #if(_WEBSOCKETS_LOGLEVEL_>3)
      #warning Use ESP32 in WebSockets_Generic
    #endif 
     
    #define WEBSOCKETS_YIELD()      yield()
    #define WEBSOCKETS_YIELD_MORE() delay(1)
  #endif

#elif ( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
        defined(STM32L0) || defined(STM32L1) || defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
        defined(STM32WB) || defined(STM32MP1) )
  // KH
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning Use STM32F/L/H/G/WB/MP1 in WebSockets_Generic
  #endif

  #define WEBSOCKETS_MAX_DATA_SIZE (15 * 1024)
  
  // moves all Header strings to Flash (~300 Byte)
  //#define WEBSOCKETS_USE_BIG_MEM
  #define WEBSOCKETS_SAVE_RAM
  //#define GET_FREE_HEAP System.freeMemory()
  #define WEBSOCKETS_YIELD()
  #define WEBSOCKETS_YIELD_MORE()

#elif defined(STM32_DEVICE)

  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning Use STM32_DEVICE in WebSockets_Generic
  #endif

  #define WEBSOCKETS_MAX_DATA_SIZE (15 * 1024)
  #define WEBSOCKETS_USE_BIG_MEM
  #define GET_FREE_HEAP System.freeMemory()
  #define WEBSOCKETS_YIELD()
  #define WEBSOCKETS_YIELD_MORE()
  
#elif ( defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
        defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || defined(NRF52840_CLUE) || \
        defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
  // KH
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning Use nRF52 in WebSockets_Generic
  #endif

  #define WEBSOCKETS_MAX_DATA_SIZE (15 * 1024)

  // Try to use GET_FREE_HEAP and large mem
  // only for ESP since AVR has less HEAP
  // try to send data in one TCP package (only if some free Heap is there)
  //#define WEBSOCKETS_USE_BIG_MEM  

  // moves all Header strings to Flash (~300 Byte)
  #define WEBSOCKETS_SAVE_RAM

  #define WEBSOCKETS_YIELD()        yield()
  #define WEBSOCKETS_YIELD_MORE()   delay(1)

#elif  ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
      || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
      || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
      || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD21E18A__) || defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) \
      || defined(__SAMD51G19A__) || defined(__SAMD51P19A__) || defined(__SAMD21G18A__) )

  // KH
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning Use SAMD21/SAMD51 in WebSockets_Generic
  #endif

  #define WEBSOCKETS_MAX_DATA_SIZE (15 * 1024)

  // Try to use GET_FREE_HEAP and large mem
  // only for ESP since AVR has less HEAP
  // try to send data in one TCP package (only if some free Heap is there)
  #if defined(SEEED_WIO_TERMINAL)
    //#define WEBSOCKETS_USE_BIG_MEM
    //#define GET_FREE_HEAP 10000
  #endif
  // moves all Header strings to Flash (~300 Byte)
  #define WEBSOCKETS_SAVE_RAM

  #define WEBSOCKETS_YIELD()        yield()
  #define WEBSOCKETS_YIELD_MORE()   delay(1)

#elif ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )

  // KH
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning Use SAM DUE in WebSockets_Generic
  #endif
  
  #define WEBSOCKETS_MAX_DATA_SIZE (15 * 1024)

  // Try to use GET_FREE_HEAP and large mem
  // only for ESP since AVR has less HEAP
  // try to send data in one TCP package (only if some free Heap is there)
  //#define WEBSOCKETS_USE_BIG_MEM

  // moves all Header strings to Flash (~300 Byte)
  #define WEBSOCKETS_SAVE_RAM

  #define WEBSOCKETS_YIELD()        yield()
  #define WEBSOCKETS_YIELD_MORE()   delay(1)

#elif defined(TEENSYDUINO)

  // KH
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning Use Teensy in WebSockets_Generic
  #endif

  #define WEBSOCKETS_MAX_DATA_SIZE (15 * 1024)

  // Try to use GET_FREE_HEAP and large mem
  // only for ESP since AVR has less HEAP
  // try to send data in one TCP package (only if some free Heap is there)
  //#define WEBSOCKETS_USE_BIG_MEM

  // moves all Header strings to Flash (~300 Byte)
  #define WEBSOCKETS_SAVE_RAM

  #define WEBSOCKETS_YIELD()        yield()
  #define WEBSOCKETS_YIELD_MORE()   delay(1)
  
#elif ( defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) || defined(ARDUINO_GENERIC_RP2040) )

  // KH
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning Use RP2040 in WebSockets_Generic
  #endif

  #define WEBSOCKETS_MAX_DATA_SIZE (15 * 1024)

  // Try to use GET_FREE_HEAP and large mem
  // try to send data in one TCP package (only if some free Heap is there)
  // moves all Header strings to Flash (~300 Byte)
  #define WEBSOCKETS_SAVE_RAM

  #define WEBSOCKETS_YIELD()        yield()
  #define WEBSOCKETS_YIELD_MORE()   delay(1)
  
#elif ( ( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) ) && defined(ARDUINO_ARCH_MBED) )

  // KH
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning Use Portenta_H7 in WebSockets_Generic
  #endif

  #define WEBSOCKETS_MAX_DATA_SIZE (15 * 1024)

  // Try to use GET_FREE_HEAP and large mem
  // try to send data in one TCP package (only if some free Heap is there)
  // moves all Header strings to Flash (~300 Byte)
  #define WEBSOCKETS_SAVE_RAM

  #define WEBSOCKETS_YIELD()        yield()
  #define WEBSOCKETS_YIELD_MORE()   delay(1)
    
#else

  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning Use atmega328p in WebSockets_Generic
  #endif

  //atmega328p has only 2KB ram!
  #define WEBSOCKETS_MAX_DATA_SIZE (1024)
  // moves all Header strings to Flash
  #define WEBSOCKETS_SAVE_RAM
  
  #define WEBSOCKETS_YIELD()
  #define WEBSOCKETS_YIELD_MORE()

#endif

//////////////////////////////////////////////////////////////

#ifndef WEBSOCKETS_TCP_TIMEOUT
   #define WEBSOCKETS_TCP_TIMEOUT (5000)
#endif

//////////////////////////////////////////////////////////////

#define NETWORK_ESP8266_ASYNC             (0)
#define NETWORK_ESP8266                   (1)
#define NETWORK_W5100                     (2)
#define NETWORK_ENC28J60                  (3)
#define NETWORK_ESP32                     (4)
#define NETWORK_ESP32_ETH                 (5)
//KH
#define NETWORK_WIFININA                  (6)
#define NETWORK_ETHERNET_ENC              (7)
#define NETWORK_RTL8720DN                 (8)
#define NETWORK_NATIVEETHERNET            (9)
#define NETWORK_LAN8742A                  (10)
#define NETWORK_WIFI101                   (11)
#define NETWORK_QN_ETHERNET               (12)
#define NETWORK_PORTENTA_H7_WIFI          (13)
#define NETWORK_PORTENTA_H7_ETHERNET      (14)

// KH, For new Async
#define NETWORK_ESP32_ASYNC								(15)

#define NETWORK_RP2040W_WIFI              (16)

////////////////////////////////////////

#if ( (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32_ASYNC) )
	// Combined for all Async
	#define NETWORK_ASYNC				true
#else
	#define NETWORK_ASYNC				false
#endif

////////////////////////////////////////

// max size of the WS Message Header
#define WEBSOCKETS_MAX_HEADER_SIZE (14)

////////////////////////////////////////

#ifndef WEBSOCKETS_NETWORK_TYPE
  // select Network type based
  #if defined(ESP8266) || defined(ESP31B)
    //KH
    #if(_WEBSOCKETS_LOGLEVEL_>2)
      #warning WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266
    #endif
    
    #define WEBSOCKETS_NETWORK_TYPE NETWORK_ESP8266
    //#define WEBSOCKETS_NETWORK_TYPE NETWORK_ESP8266_ASYNC
    //#define WEBSOCKETS_NETWORK_TYPE NETWORK_W5100

  #elif defined(ESP32)
    //KH
    #if(_WEBSOCKETS_LOGLEVEL_>2)
      #warning WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32
    #endif
    
    #define WEBSOCKETS_NETWORK_TYPE NETWORK_ESP32
    //#define WEBSOCKETS_NETWORK_TYPE NETWORK_ESP32_ETH
    //#define WEBSOCKETS_NETWORK_TYPE NETWORK_ESP32_ASYNC

  #elif ( defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
          defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || defined(NRF52840_CLUE) || \
          defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
    //KH
    #if(_WEBSOCKETS_LOGLEVEL_>2)
      #warning WEBSOCKETS_NETWORK_TYPE == NETWORK_WIFININA
    #endif
    
    #define WEBSOCKETS_NETWORK_TYPE     NETWORK_WIFININA

  #elif ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
       || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
       || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
       || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD21E18A__) || defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) \
       || defined(__SAMD51G19A__) || defined(__SAMD51P19A__) || defined(__SAMD21G18A__) )
       
       
    #if defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) 
      //KH
      #if(_WEBSOCKETS_LOGLEVEL_>2)
        #warning WEBSOCKETS_NETWORK_TYPE == NETWORK_WIFI101  
      #endif
         
      #define WEBSOCKETS_NETWORK_TYPE     NETWORK_WIFI101
    #elif defined(SEEED_WIO_TERMINAL)
      //KH, from v2.3.2
      #if(_WEBSOCKETS_LOGLEVEL_>2)
        #warning WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN
      #endif
      
      #define WEBSOCKETS_NETWORK_TYPE     NETWORK_RTL8720DN
    #else  
      //KH
      #if(_WEBSOCKETS_LOGLEVEL_>2)
        #warning WEBSOCKETS_NETWORK_TYPE == NETWORK_WIFININA     
      #endif
      
      #define WEBSOCKETS_NETWORK_TYPE     NETWORK_WIFININA
    #endif
    
  #else
    //KH
    #if(_WEBSOCKETS_LOGLEVEL_>2)
      #warning WEBSOCKETS_NETWORK_TYPE == NETWORK_W5100
    #endif
    
    #define WEBSOCKETS_NETWORK_TYPE       NETWORK_W5100

  #endif  //#if defined(ESP8266) || defined(ESP31B)
  
#endif    //#ifndef WEBSOCKETS_NETWORK_TYPE

////////////////////////////////////////

// Includes and defined based on Network Type
#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)

  // Note:
  //   No SSL/WSS support for client in Async mode
  //   TLS lib need a sync interface!

  #if defined(ESP8266)
    #include <ESP8266WiFi.h>
  #elif defined(ESP32)
    #include <WiFi.h>
    #include <WiFiClientSecure.h>
    
    // From v2.3.1
    #define SSL_AXTLS
    //////
    
  #elif defined(ESP31B)
    #include <ESP31BWiFi.h>
  #else
    #error "network type ESP8266 ASYNC only possible on the ESP mcu!"
  #endif

  #include <ESPAsyncTCP.h>
  #include <ESPAsyncTCPbuffer.h>
  #define WEBSOCKETS_NETWORK_CLASS          AsyncTCPbuffer
  #define WEBSOCKETS_NETWORK_SERVER_CLASS   AsyncServer

////////////////////////////////////////

#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266)

  #if !defined(ESP8266) && !defined(ESP31B)
    #error "network type ESP8266 only possible on the ESP mcu!"
  #endif

  #ifdef ESP8266
    #include <ESP8266WiFi.h>
    
    // From v2.3.1
    #if defined(wificlientbearssl_h) && !defined(USING_AXTLS) && !defined(wificlientsecure_h)
      #define SSL_BARESSL
      #define SSL_BEARSSL
    #else
      #define SSL_AXTLS
    #endif
    //////
    
  #else
    #include <ESP31BWiFi.h>
  #endif

  #define WEBSOCKETS_NETWORK_CLASS          WiFiClient
  #define WEBSOCKETS_NETWORK_SSL_CLASS      WiFiClientSecure
  #define WEBSOCKETS_NETWORK_SERVER_CLASS   WiFiServer

////////////////////////////////////////

#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_W5100)

  #ifdef STM32_DEVICE
    #define WEBSOCKETS_NETWORK_CLASS TCPClient
    #define WEBSOCKETS_NETWORK_SERVER_CLASS TCPServer
  #else
    #include <SPI.h>
    // KH, New v2.2.3 to support Ethernet2, Ethernet3, EthernetLarge Lib
    #if USE_ETHERNET_GENERIC    //(WEBSOCKETS_NETWORK_LIB == _ETHERNET_)
      #include <Ethernet_Generic.h>
      
      #if(_WEBSOCKETS_LOGLEVEL_>3)
        #warning Using Ethernet_Generic W5x00 Library
      #endif
    #else  
      #include <Ethernet_Generic.h>
      
      #if(_WEBSOCKETS_LOGLEVEL_>3)
        #warning Using default Ethernet_Generic W5x00 Library  
      #endif  
    #endif
    //////
    
    #define WEBSOCKETS_NETWORK_CLASS          EthernetClient
    #define WEBSOCKETS_NETWORK_SERVER_CLASS   EthernetServer
    
    // KH, test SSL
    //#define WEBSOCKETS_NETWORK_SSL_CLASS      EthernetSSLClient
    
  #endif

////////////////////////////////////////

#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_ENC28J60)

  #include <UIPEthernet.h>
  #define WEBSOCKETS_NETWORK_CLASS UIPClient
  #define WEBSOCKETS_NETWORK_SERVER_CLASS UIPServer

////////////////////////////////////////

#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32)

  #include <WiFi.h>
  #include <WiFiClientSecure.h>
  
  // From v2.3.1
  #define SSL_AXTLS
  //////
  
  #define WEBSOCKETS_NETWORK_CLASS            WiFiClient
  #define WEBSOCKETS_NETWORK_SSL_CLASS        WiFiClientSecure
  #define WEBSOCKETS_NETWORK_SERVER_CLASS     WiFiServer

////////////////////////////////////////
 
#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32_ASYNC)

	#error "Network type NETWORK_ESP32_ASYNC not ready yet"
  // Note:
  //   No SSL/WSS support for client in Async mode
  //   TLS lib need a sync interface!

  #if defined(ESP32)
    #include <WiFi.h>
    #include <WiFiClientSecure.h>
    
    // From v2.3.1
    #define SSL_AXTLS
    //////
    
  #else
    #error "Network type NETWORK_ESP32_ASYNC only for ESP32"
  #endif

  #include <AsyncTCP.h>

  #define WEBSOCKETS_NETWORK_CLASS            AsyncClient
  #define WEBSOCKETS_NETWORK_SERVER_CLASS     AsyncServer    

////////////////////////////////////////

#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32_ETH)

  #include <ETH.h>  
 
  #include <WiFi.h>
  #include <WiFiClientSecure.h>
  
  // From v2.3.1
  #define SSL_AXTLS
  //////
  
  #define WEBSOCKETS_NETWORK_CLASS            WiFiClient
  #define WEBSOCKETS_NETWORK_SSL_CLASS        WiFiClientSecure
  #define WEBSOCKETS_NETWORK_SERVER_CLASS     WiFiServer

////////////////////////////////////////

#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_WIFININA)

  //KH
  #include <WiFiNINA_Generic.h>
  #include <WiFiSSLClient_Generic.h>
  
  #define SSL_AXTLS
  
  #define WEBSOCKETS_NETWORK_CLASS            WiFiClient
  #define WEBSOCKETS_NETWORK_SSL_CLASS        WiFiSSLClient
  #define WEBSOCKETS_NETWORK_SERVER_CLASS     WiFiServer

////////////////////////////////////////
 
#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_WIFI101)

  //KH
  #include <WiFi101.h>
  #include <WiFiSSLClient.h>
  
  #define SSL_AXTLS
  
  #define WEBSOCKETS_NETWORK_CLASS            WiFiClient
  #define WEBSOCKETS_NETWORK_SSL_CLASS        WiFiSSLClient
  #define WEBSOCKETS_NETWORK_SERVER_CLASS     WiFiServer  

////////////////////////////////////////

#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_ETHERNET_ENC)

  //KH
  #include <EthernetENC.h>
  
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning Using ENC28J60 EthernetENC Library
  #endif
  
  #define WEBSOCKETS_NETWORK_CLASS            EthernetClient
  #define WEBSOCKETS_NETWORK_SERVER_CLASS     EthernetServer

////////////////////////////////////////
 
#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)

  //KH, from v2.3.2
  #include <rpcWiFi.h>
  #include <WiFiClientSecure.h>
  
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning Using RTL8720DN Seeed_Arduino_rpcWiFi Library
  #endif
  
  #define SSL_AXTLS
  
  #define WEBSOCKETS_NETWORK_CLASS            WiFiClient
  #define WEBSOCKETS_NETWORK_SSL_CLASS        WiFiClientSecure
  #define WEBSOCKETS_NETWORK_SERVER_CLASS     WiFiServer

////////////////////////////////////////

#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_NATIVEETHERNET)

  //KH, from v2.4.0
  #include <NativeEthernet.h>
  
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning Using Teensy 4.1 NativeEthernet Library
  #endif
  
  #define WEBSOCKETS_NETWORK_CLASS          EthernetClient
  #define WEBSOCKETS_NETWORK_SERVER_CLASS   EthernetServer

////////////////////////////////////////

#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_QN_ETHERNET)

  //KH, from v2.9.0
  #include <QNEthernet.h>
  using namespace qindesign::network;
  
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning Using Teensy 4.1 QNEthernet Library
  #endif
  
  #define WEBSOCKETS_NETWORK_CLASS          EthernetClient
  #define WEBSOCKETS_NETWORK_SERVER_CLASS   EthernetServer

////////////////////////////////////////

#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_PORTENTA_H7_WIFI)

  //KH, from v2.10.0
  #include <WiFi.h>
  //#include <WiFiSSLClient.h>
  
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning Using Portenta_H7 WiFi Library
  #endif
   
  #define WEBSOCKETS_NETWORK_CLASS          WiFiClient
  //#define WEBSOCKETS_NETWORK_SSL_CLASS      WiFiSSLClient
  #define WEBSOCKETS_NETWORK_SERVER_CLASS   WiFiServer

////////////////////////////////////////

#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_PORTENTA_H7_ETHERNET)

  //KH, from v2.10.0
  #include <PortentaEthernet.h>
  
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning Using Portenta_H7 Ethernet Library
  #endif
  
  #define WEBSOCKETS_NETWORK_CLASS          EthernetClient
  //#define WEBSOCKETS_NETWORK_SSL_CLASS      EthernetSSLClient
  #define WEBSOCKETS_NETWORK_SERVER_CLASS   EthernetServer

////////////////////////////////////////
 
#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_LAN8742A)

  //KH, from v2.4.0
  #include <LwIP.h>
  #include <STM32Ethernet.h>
  
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning Using LAN8742A Ethernet & STM32Ethernet lib
  #endif
  
  #define WEBSOCKETS_NETWORK_CLASS          EthernetClient
  #define WEBSOCKETS_NETWORK_SERVER_CLASS   EthernetServer

////////////////////////////////////////
  
#elif ( defined(ARDUINO_RASPBERRY_PI_PICO_W) && (WEBSOCKETS_NETWORK_TYPE == NETWORK_RP2040W_WIFI) )

  //KH, from v2.16.0
  #include <WiFi.h>
  
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning Using RP2040W CYC43439 WiFi
  #endif
  
  #define SSL_BARESSL
  #define SSL_BEARSSL
  
  #define WEBSOCKETS_NETWORK_CLASS            WiFiClient
  #define WEBSOCKETS_NETWORK_SSL_CLASS        WiFiClientSecure
  #define WEBSOCKETS_NETWORK_SERVER_CLASS     WiFiServer

////////////////////////////////////////
  
#else
  #error "no network type selected!"
#endif

////////////////////////////////////////
////////////////////////////////////////

#ifdef WEBSOCKETS_NETWORK_SSL_CLASS

  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning This network type Supporting SSL for WebSockets
  #endif
  
  #define HAS_SSL
#endif

////////////////////////////////////////

// moves all Header strings to Flash (~300 Byte)
#ifdef WEBSOCKETS_SAVE_RAM
  #define WEBSOCKETS_STRING(var) F(var)
#else
  #define WEBSOCKETS_STRING(var) var
#endif

////////////////////////////////////////

typedef enum
{
  WSC_NOT_CONNECTED,
  WSC_HEADER,
  WSC_BODY,
  WSC_CONNECTED
} WSclientsStatus_t;

////////////////////////////////////////

typedef enum
{
  WStype_ERROR,
  WStype_DISCONNECTED,
  WStype_CONNECTED,
  WStype_TEXT,
  WStype_BIN,
  WStype_FRAGMENT_TEXT_START,
  WStype_FRAGMENT_BIN_START,
  WStype_FRAGMENT,
  WStype_FRAGMENT_FIN,
  WStype_PING,
  WStype_PONG,
} WStype_t;

////////////////////////////////////////

typedef enum
{
  WSop_continuation = 0x00,    ///< %x0 denotes a continuation frame
  WSop_text         = 0x01,    ///< %x1 denotes a text frame
  WSop_binary       = 0x02,    ///< %x2 denotes a binary frame
  ///< %x3-7 are reserved for further non-control frames
  WSop_close = 0x08,           ///< %x8 denotes a connection close
  WSop_ping  = 0x09,           ///< %x9 denotes a ping
  WSop_pong  = 0x0A            ///< %xA denotes a pong
               ///< %xB-F are reserved for further control frames
} WSopcode_t;

////////////////////////////////////////

typedef struct
{
  bool fin;
  bool rsv1;
  bool rsv2;
  bool rsv3;

  WSopcode_t opCode;
  bool mask;

  size_t payloadLen;

  uint8_t * maskKey;
} WSMessageHeader_t;

////////////////////////////////////////

typedef struct
{
  void init(const uint8_t& num, const uint32_t& pingInterval, const uint32_t& pongTimeout,
            const uint8_t& disconnectTimeoutCount) 
  {
    this->num                    = num;
    this->pingInterval           = pingInterval;
    this->pongTimeout            = pongTimeout;
    this->disconnectTimeoutCount = disconnectTimeoutCount;
  }

  uint8_t num = 0;    ///< connection number

  WSclientsStatus_t status = WSC_NOT_CONNECTED;

  WEBSOCKETS_NETWORK_CLASS * tcp = nullptr;

  bool isSocketIO = false;    ///< client for socket.io server

#if defined(HAS_SSL)
  bool isSSL = false;    ///< run in ssl mode
  WEBSOCKETS_NETWORK_SSL_CLASS * ssl = nullptr;
#endif

  String cUrl;           ///< http url
  uint16_t cCode = 0;    ///< http code

  bool cIsClient    = false;    ///< will be used for masking
  bool cIsUpgrade   = false;    ///< Connection == Upgrade
  bool cIsWebsocket = false;    ///< Upgrade == websocket

  String cSessionId;        ///< client Set-Cookie (session id)
  String cKey;              ///< client Sec-WebSocket-Key
  String cAccept;           ///< client Sec-WebSocket-Accept
  String cProtocol;         ///< client Sec-WebSocket-Protocol
  String cExtensions;       ///< client Sec-WebSocket-Extensions
  uint16_t cVersion = 0;    ///< client Sec-WebSocket-Version

  uint8_t cWsRXsize = 0;                            ///< State of the RX
  uint8_t cWsHeader[WEBSOCKETS_MAX_HEADER_SIZE];    ///< RX WS Message buffer
  WSMessageHeader_t cWsHeaderDecode;

  String base64Authorization;    ///< Base64 encoded Auth request
  String plainAuthorization;     ///< Base64 encoded Auth request

  String extraHeaders;

  bool cHttpHeadersValid = false;    ///< non-websocket http header validity indicator
  size_t cMandatoryHeadersCount;     ///< non-websocket mandatory http headers present count

  bool pongReceived              = false;
  uint32_t pingInterval          = 0;    // how often ping will be sent, 0 means "heartbeat is not active"
  uint32_t lastPing              = 0;    // millis when last pong has been received
  uint32_t pongTimeout           = 0;    // interval in millis after which pong is considered to timeout
  uint8_t disconnectTimeoutCount = 0;    // after how many subsequent pong timeouts discconnect will happen, 0 means "do not disconnect"
  uint8_t pongTimeoutCount       = 0;    // current pong timeout count

#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)
  String cHttpLine;    ///< HTTP header lines
#endif

} WSclient_t;

////////////////////////////////////////
////////////////////////////////////////

class WebSockets
{
  protected:
#ifdef __AVR__
    typedef void (*WSreadWaitCb)(WSclient_t * client, bool ok);
#else
    typedef std::function<void(WSclient_t * client, bool ok)> WSreadWaitCb;
#endif

    virtual void clientDisconnect(WSclient_t * client)  = 0;
    virtual bool clientIsConnected(WSclient_t * client) = 0;

    void clientDisconnect(WSclient_t * client, uint16_t code, char * reason = NULL, size_t reasonLen = 0);

    virtual void messageReceived(WSclient_t * client, WSopcode_t opcode, uint8_t * payload, 
                                 size_t length, bool fin) = 0;

    uint8_t createHeader(uint8_t * buf, WSopcode_t opcode, size_t length, bool mask, 
                         uint8_t maskKey[4], bool fin);
                         
    bool sendFrameHeader(WSclient_t * client, WSopcode_t opcode, size_t length = 0, bool fin = true);
    
    bool sendFrame(WSclient_t * client, WSopcode_t opcode, uint8_t * payload = NULL, size_t length = 0, 
                   bool fin = true, bool headerToPayload = false);

    void headerDone(WSclient_t * client);

    void handleWebsocket(WSclient_t * client);

    bool handleWebsocketWaitFor(WSclient_t * client, size_t size);
    void handleWebsocketCb(WSclient_t * client);
    void handleWebsocketPayloadCb(WSclient_t * client, bool ok, uint8_t * payload);

    String acceptKey(String & clientKey);
    String base64_encode(uint8_t * data, size_t length);

    bool readCb(WSclient_t * client, uint8_t * out, size_t n, WSreadWaitCb cb);
    virtual size_t write(WSclient_t * client, uint8_t * out, size_t n);
    size_t write(WSclient_t * client, const char * out);

    void enableHeartbeat(WSclient_t * client, const uint32_t& pingInterval, const uint32_t& pongTimeout, 
                         const uint8_t& disconnectTimeoutCount);
                         
    void handleHBTimeout(WSclient_t * client);
};

////////////////////////////////////////
////////////////////////////////////////

// KH
String WS_IPAddressToString(const IPAddress& _address)
{
  String str = String(_address[0]);
  str += ".";
  str += String(_address[1]);
  str += ".";
  str += String(_address[2]);
  str += ".";
  str += String(_address[3]);
  
  return str;
}

////////////////////////////////////////

#ifndef UNUSED
  #define UNUSED(var) (void)(var)
#endif

#include "WebSockets_Generic-Impl.h"

#endif    // WEBSOCKETS_GENERIC_H_

