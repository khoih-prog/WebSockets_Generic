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
  
  Version: 2.3.2

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
  *****************************************************************************************************************************/

#pragma once

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
  #define LOGERROR        WS_LOGERROR
  #define LOGERROR0       WS_LOGERROR0
  #define LOGERROR1       WS_LOGERROR1
  #define LOGERROR2       WS_LOGERROR2
  #define LOGERROR3       WS_LOGERROR3
#endif

#ifndef LOGWARN  
  #define LOGWARN         WS_LOGWARN
  #define LOGWARN0        WS_LOGWARN0
  #define LOGWARN1        WS_LOGWARN1
  #define LOGWARN2        WS_LOGWARN2
  #define LOGWARN3        WS_LOGWARN3
#endif

#ifndef LOGINFO   
  #define LOGINFO         WS_LOGINFO
  #define LOGINFO0        WS_LOGINFO0
  #define LOGINFO1        WS_LOGINFO1
  #define LOGINFO2        WS_LOGINFO2
  #define LOGINFO3        WS_LOGINFO3
#endif

#ifndef LOGDEBUG   
  #define LOGDEBUG        WS_LOGDEBUG
  #define LOGDEBUG0       WS_LOGDEBUG0
  #define LOGDEBUG1       WS_LOGDEBUG1
  #define LOGDEBUG2       WS_LOGDEBUG2
  #define LOGDEBUG3       WS_LOGDEBUG3
#endif

/////////////////////

#define WS_LOGERROR(x)         if(_WEBSOCKETS_LOGLEVEL_>0) { DBG_PORT.print("[WS] "); DBG_PORT.println(x); }
#define WS_LOGERROR0(x)        if(_WEBSOCKETS_LOGLEVEL_>0) { DBG_PORT.print(x); }
#define WS_LOGERROR1(x,y)      if(_WEBSOCKETS_LOGLEVEL_>0) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.println(y); }
#define WS_LOGERROR2(x,y,z)    if(_WEBSOCKETS_LOGLEVEL_>0) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.print(y); DBG_PORT.print(" "); DBG_PORT.println(z); }
#define WS_LOGERROR3(x,y,z,w)  if(_WEBSOCKETS_LOGLEVEL_>0) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.print(y); DBG_PORT.print(" "); DBG_PORT.print(z); DBG_PORT.print(" "); DBG_PORT.println(w); }

/////////////////////

#define WS_LOGWARN(x)          if(_WEBSOCKETS_LOGLEVEL_>1) { DBG_PORT.print("[WS] "); DBG_PORT.println(x); }
#define WS_LOGWARN0(x)         if(_WEBSOCKETS_LOGLEVEL_>1) { DBG_PORT.print(x); }
#define WS_LOGWARN1(x,y)       if(_WEBSOCKETS_LOGLEVEL_>1) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.println(y); }
#define WS_LOGWARN2(x,y,z)     if(_WEBSOCKETS_LOGLEVEL_>1) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.print(y); DBG_PORT.print(" "); DBG_PORT.println(z); }
#define WS_LOGWARN3(x,y,z,w)   if(_WEBSOCKETS_LOGLEVEL_>1) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.print(y); DBG_PORT.print(" "); DBG_PORT.print(z); DBG_PORT.print(" "); DBG_PORT.println(w); }

/////////////////////

#define WS_LOGINFO(x)          if(_WEBSOCKETS_LOGLEVEL_>2) { DBG_PORT.print("[WS] "); DBG_PORT.println(x); }
#define WS_LOGINFO0(x)         if(_WEBSOCKETS_LOGLEVEL_>2) { DBG_PORT.print(x); }
#define WS_LOGINFO1(x,y)       if(_WEBSOCKETS_LOGLEVEL_>2) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.println(y); }
#define WS_LOGINFO2(x,y,z)     if(_WEBSOCKETS_LOGLEVEL_>2) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.print(y); DBG_PORT.print(" "); DBG_PORT.println(z); }
#define WS_LOGINFO3(x,y,z,w)   if(_WEBSOCKETS_LOGLEVEL_>2) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.print(y); DBG_PORT.print(" "); DBG_PORT.print(z); DBG_PORT.print(" "); DBG_PORT.println(w); }

/////////////////////

#define WS_LOGDEBUG(x)         if(_WEBSOCKETS_LOGLEVEL_>3) { DBG_PORT.print("[WS] "); DBG_PORT.println(x); }
#define WS_LOGDEBUG0(x)        if(_WEBSOCKETS_LOGLEVEL_>3) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); }
#define WS_LOGDEBUG1(x,y)      if(_WEBSOCKETS_LOGLEVEL_>3) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.println(y); }
#define WS_LOGDEBUG2(x,y,z)    if(_WEBSOCKETS_LOGLEVEL_>3) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.print(y); DBG_PORT.print(" "); DBG_PORT.println(z); }
#define WS_LOGDEBUG3(x,y,z,w)  if(_WEBSOCKETS_LOGLEVEL_>3) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.print(y); DBG_PORT.print(" "); DBG_PORT.print(z); DBG_PORT.print(" "); DBG_PORT.println(w); }

/////////////////////

