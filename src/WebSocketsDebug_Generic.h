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

  Version: 2.16.1

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
  2.16.1  K Hoang      24/11/2022 Using new WiFi101_Generic library
  *****************************************************************************************************************************/

#pragma once

#ifndef WEBSOCKETS_DEBUG_GENERIC_H_
#define WEBSOCKETS_DEBUG_GENERIC_H_

#include <stdio.h>

////////////////////////////////////////

#ifdef DEBUG_WEBSOCKETS_PORT
  #define DBG_PORT      DEBUG_WEBSOCKETS_PORT
#else
  #define DBG_PORT      Serial
#endif

////////////////////////////////////////

// Change _WEBSOCKETS_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _WEBSOCKETS_LOGLEVEL_
  #define _WEBSOCKETS_LOGLEVEL_       0
#endif

////////////////////////////////////////

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

////////////////////////////////////////

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

////////////////////////////////////////

const char WS_MARK[] = "[WS] ";

#define WS_PRINT_MARK   WS_PRINT(WS_MARK)

#define WS_PRINT        DBG_PORT.print
#define WS_PRINTLN      DBG_PORT.println

////////////////////////////////////////

#define WSK_LOGERROR(x)         if(_WEBSOCKETS_LOGLEVEL_>0) { WS_PRINT_MARK; WS_PRINTLN(x); }
#define WSK_LOGERROR0(x)        if(_WEBSOCKETS_LOGLEVEL_>0) { WS_PRINT(x); }
#define WSK_LOGERROR1(x,y)      if(_WEBSOCKETS_LOGLEVEL_>0) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINTLN(y); }
#define WSK_LOGERROR2(x,y,z)    if(_WEBSOCKETS_LOGLEVEL_>0) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINT(y); WS_PRINTLN(z); }
#define WSK_LOGERROR3(x,y,z,w)  if(_WEBSOCKETS_LOGLEVEL_>0) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINT(y); WS_PRINT(z); WS_PRINTLN(w); }

////////////////////////////////////////

#define WSK_LOGWARN(x)          if(_WEBSOCKETS_LOGLEVEL_>1) { WS_PRINT_MARK; WS_PRINTLN(x); }
#define WSK_LOGWARN0(x)         if(_WEBSOCKETS_LOGLEVEL_>1) { WS_PRINT(x); }
#define WSK_LOGWARN1(x,y)       if(_WEBSOCKETS_LOGLEVEL_>1) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINTLN(y); }
#define WSK_LOGWARN2(x,y,z)     if(_WEBSOCKETS_LOGLEVEL_>1) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINT(y); WS_PRINTLN(z); }
#define WSK_LOGWARN3(x,y,z,w)   if(_WEBSOCKETS_LOGLEVEL_>1) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINT(y); WS_PRINT(z);  WS_PRINTLN(w); }

////////////////////////////////////////


#define WSK_LOGINFO(x)          if(_WEBSOCKETS_LOGLEVEL_>2) { WS_PRINT_MARK; WS_PRINTLN(x); }
#define WSK_LOGINFO0(x)         if(_WEBSOCKETS_LOGLEVEL_>2) { WS_PRINT(x); }
#define WSK_LOGINFO1(x,y)       if(_WEBSOCKETS_LOGLEVEL_>2) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINTLN(y); }
#define WSK_LOGINFO2(x,y,z)     if(_WEBSOCKETS_LOGLEVEL_>2) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINT(y); WS_PRINTLN(z); }
#define WSK_LOGINFO3(x,y,z,w)   if(_WEBSOCKETS_LOGLEVEL_>2) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINT(y); WS_PRINT(z); WS_PRINTLN(w); }

////////////////////////////////////////

#define WSK_LOGDEBUG(x)         if(_WEBSOCKETS_LOGLEVEL_>3) { WS_PRINT_MARK; WS_PRINTLN(x); }
#define WSK_LOGDEBUG0(x)        if(_WEBSOCKETS_LOGLEVEL_>3) { WS_PRINT_MARK; WS_PRINT(x); }
#define WSK_LOGDEBUG1(x,y)      if(_WEBSOCKETS_LOGLEVEL_>3) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINTLN(y); }
#define WSK_LOGDEBUG2(x,y,z)    if(_WEBSOCKETS_LOGLEVEL_>3) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINT(y); WS_PRINTLN(z); }
#define WSK_LOGDEBUG3(x,y,z,w)  if(_WEBSOCKETS_LOGLEVEL_>3) { WS_PRINT_MARK; WS_PRINT(x); WS_PRINT(y); WS_PRINT(z); WS_PRINTLN(w); }

////////////////////////////////////////

#endif    // WEBSOCKETS_DEBUG_GENERIC_H_

