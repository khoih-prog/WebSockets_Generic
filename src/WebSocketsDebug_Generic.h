/****************************************************************************************************************************
  WebSocketsDebug_Generic.h - WebSockets Library for boards

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWS
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52, Teensy, STM32F, SAM DUE boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WS_Generic
  Licensed under MIT license

  Copyright (c) 2015 Markus Sattler. All rights reserved.
  This file is part of the WS for Arduino.

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
  
  Version: 2.12.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  2.1.3   K Hoang      15/05/2020 Initial porting to support SAMD21, SAMD51, nRF52 boards, such as AdaFruit Feather nRF52832,
                                  nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, etc.
  2.2.1   K Hoang      18/05/2020 Bump up to sync with v2.2.1 of original WebSockets library
  2.2.2   K Hoang      25/05/2020 Add support to Teensy, SAM DUE and STM32. Enable WebSocket Server for new supported boards.
  2.2.3   K Hoang      02/08/2020 Add support to W5x00's Ethernet2, Ethernet3, EthernetLarge Libraries. 
                                  Add support to STM32F/L/H/G/WB/MP1 and Seeeduino SAMD21/SAMD51 boards.
  2.3.1   K Hoang      07/10/2020 Sync with v2.3.1 of original WebSockets library. Add ENC28J60 EthernetENC library support
  2.3.2   K Hoang      12/11/2020 Add RTL8720DN Seeed_Arduino_rpcWiFi library support
  2.3.3   K Hoang      28/11/2020 Fix compile error for WIO_TERMINAL and boards using libraries with lib64.
  2.3.4   K Hoang      12/12/2020 Add SSL support to SAMD21 Nano-33-IoT using WiFiNINA. Upgrade WS and WSS examples.
  2.4.0   K Hoang      06/02/2021 Add support to Teensy 4.1 NativeEthernet and STM32 built-in LAN8742A. 
                                  Sync with v2.3.4 of original WebSockets library
  2.4.1   K Hoang      19/03/2021 Sync with v2.3.5 of original WebSockets library to adapt to ESP32 SSL changes 
  2.5.0   K Hoang      22/05/2021 Add support to WiFi101
  2.5.1   K Hoang      22/05/2021 Default to EIO4 for Socket.IO. Permit increase reconnectInterval in Socket.IO
  2.6.0   K Hoang      23/05/2021 Fix breaking problem with SocketIO. Add setExtraHeaders to SocketIO
  2.7.0   K Hoang      24/05/2021 Add support to RP2040-based boards using Arduino-pico and Arduino mbed_rp2040 core
  2.8.0   K Hoang      08/07/2021 Add support to WT32_ETH01 (ESP32 + LAN8720) boards
  2.9.0   K Hoang      05/09/2021 Add support to QNEthernet Library for Teensy 4.1
  2.10.0  K Hoang      18/09/2021 Add support to Portenta_H7, using either WiFi or Vision-shield Ethernet
  2.10.1  K Hoang      12/10/2021 Update `platform.ini` and `library.json`
  2.11.0  K Hoang      30/11/2021 Auto detect ESP32 core version. Fix bug in examples
  2.11.1  K Hoang      12/12/2021 Add option to use transport=websocket with sticky-session SIO server
  2.12.0  K Hoang      28/01/2022 Supporting SSL for ESP32-based WT32_ETH01 boards
  *****************************************************************************************************************************/

#pragma once

#ifndef WEBSOCKETS_DEBUG_GENERIC_H_
#define WEBSOCKETS_DEBUG_GENERIC_H_

#include <stdio.h>

#ifdef DEBUG_WEBSOCKETS_PORT
  #define DBG_PORT      DEBUG_WEBSOCKETS_PORT
#else
  #define DBG_PORT      Serial
#endif

// Change _WEBSOCKETS_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _WEBSOCKETS_LOGLEVEL_
  #define _WEBSOCKETS_LOGLEVEL_       0
#endif

/////////////////////

//For compatibility to old version libraries relying on this lib
#ifndef LOGERROR
  #define LOGERROR        WSK_LOGERROR
  #define LOGERROR0       WSK_LOGERROR0
  #define LOGERROR1       WSK_LOGERROR1
  #define LOGERROR2       WSK_LOGERROR2
  #define LOGERROR3       WSK_LOGERROR3
#endif

#ifndef LOGWARN  
  #define LOGWARN         WSK_LOGWARN
  #define LOGWARN0        WSK_LOGWARN0
  #define LOGWARN1        WSK_LOGWARN1
  #define LOGWARN2        WSK_LOGWARN2
  #define LOGWARN3        WSK_LOGWARN3
#endif

#ifndef LOGINFO   
  #define LOGINFO         WSK_LOGINFO
  #define LOGINFO0        WSK_LOGINFO0
  #define LOGINFO1        WSK_LOGINFO1
  #define LOGINFO2        WSK_LOGINFO2
  #define LOGINFO3        WSK_LOGINFO3
#endif

#ifndef LOGDEBUG   
  #define LOGDEBUG        WSK_LOGDEBUG
  #define LOGDEBUG0       WSK_LOGDEBUG0
  #define LOGDEBUG1       WSK_LOGDEBUG1
  #define LOGDEBUG2       WSK_LOGDEBUG2
  #define LOGDEBUG3       WSK_LOGDEBUG3
#endif

/////////////////////

//For compatibility to old version examples if not fixed yet

#ifndef WS_LOGERROR
  #define WS_LOGERROR        WSK_LOGERROR
  #define WS_LOGERROR0       WSK_LOGERROR0
  #define WS_LOGERROR1       WSK_LOGERROR1
  #define WS_LOGERROR2       WSK_LOGERROR2
  #define WS_LOGERROR3       WSK_LOGERROR3

  #define WS_LOGWARN         WSK_LOGWARN
  #define WS_LOGWARN0        WSK_LOGWARN0
  #define WS_LOGWARN1        WSK_LOGWARN1
  #define WS_LOGWARN2        WSK_LOGWARN2
  #define WS_LOGWARN3        WSK_LOGWARN3
 
  #define WS_LOGINFO         WSK_LOGINFO
  #define WS_LOGINFO0        WSK_LOGINFO0
  #define WS_LOGINFO1        WSK_LOGINFO1
  #define WS_LOGINFO2        WSK_LOGINFO2
  #define WS_LOGINFO3        WSK_LOGINFO3

  #define WS_LOGDEBUG        WSK_LOGDEBUG
  #define WS_LOGDEBUG0       WSK_LOGDEBUG0
  #define WS_LOGDEBUG1       WSK_LOGDEBUG1
  #define WS_LOGDEBUG2       WSK_LOGDEBUG2
  #define WS_LOGDEBUG3       WSK_LOGDEBUG3
#endif

/////////////////////

const char WS_MARK[] = "[WS] ";

#define WS_PRINT_MARK   WS_PRINT(WS_MARK)

#define WS_PRINT        DBG_PORT.print
#define WS_PRINTLN      DBG_PORT.println

/////////////////////

#define WSK_LOGERROR(x)         if(_WEBSOCKETS_LOGLEVEL_>0) { WS_PRINT_MARK; WS_PRINTLN(x); }
#define WSK_LOGERROR0(x)        if(_WEBSOCKETS_LOGLEVEL_>0) { WS_PRINT(x); }
#define WSK_LOGERROR1(x,y)      if(_WEBSOCKETS_LOGLEVEL_>0) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINTLN(y); }
#define WSK_LOGERROR2(x,y,z)    if(_WEBSOCKETS_LOGLEVEL_>0) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINT(y); WS_PRINTLN(z); }
#define WSK_LOGERROR3(x,y,z,w)  if(_WEBSOCKETS_LOGLEVEL_>0) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINT(y); WS_PRINT(z); WS_PRINTLN(w); }

/////////////////////

#define WSK_LOGWARN(x)          if(_WEBSOCKETS_LOGLEVEL_>1) { WS_PRINT_MARK; WS_PRINTLN(x); }
#define WSK_LOGWARN0(x)         if(_WEBSOCKETS_LOGLEVEL_>1) { WS_PRINT(x); }
#define WSK_LOGWARN1(x,y)       if(_WEBSOCKETS_LOGLEVEL_>1) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINTLN(y); }
#define WSK_LOGWARN2(x,y,z)     if(_WEBSOCKETS_LOGLEVEL_>1) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINT(y); WS_PRINTLN(z); }
#define WSK_LOGWARN3(x,y,z,w)   if(_WEBSOCKETS_LOGLEVEL_>1) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINT(y); WS_PRINT(z);  WS_PRINTLN(w); }

/////////////////////

#define WSK_LOGINFO(x)          if(_WEBSOCKETS_LOGLEVEL_>2) { WS_PRINT_MARK; WS_PRINTLN(x); }
#define WSK_LOGINFO0(x)         if(_WEBSOCKETS_LOGLEVEL_>2) { WS_PRINT(x); }
#define WSK_LOGINFO1(x,y)       if(_WEBSOCKETS_LOGLEVEL_>2) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINTLN(y); }
#define WSK_LOGINFO2(x,y,z)     if(_WEBSOCKETS_LOGLEVEL_>2) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINT(y); WS_PRINTLN(z); }
#define WSK_LOGINFO3(x,y,z,w)   if(_WEBSOCKETS_LOGLEVEL_>2) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINT(y); WS_PRINT(z); WS_PRINTLN(w); }

/////////////////////

#define WSK_LOGDEBUG(x)         if(_WEBSOCKETS_LOGLEVEL_>3) { WS_PRINT_MARK; WS_PRINTLN(x); }
#define WSK_LOGDEBUG0(x)        if(_WEBSOCKETS_LOGLEVEL_>3) { WS_PRINT_MARK; WS_PRINT(x); }
#define WSK_LOGDEBUG1(x,y)      if(_WEBSOCKETS_LOGLEVEL_>3) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINTLN(y); }
#define WSK_LOGDEBUG2(x,y,z)    if(_WEBSOCKETS_LOGLEVEL_>3) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINT(y); WS_PRINTLN(z); }
#define WSK_LOGDEBUG3(x,y,z,w)  if(_WEBSOCKETS_LOGLEVEL_>3) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINT(y); WS_PRINT(z); WS_PRINTLN(w); }

/////////////////////

#endif    // WEBSOCKETS_DEBUG_GENERIC_H_

