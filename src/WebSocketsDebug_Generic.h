/****************************************************************************************************************************
   WebSocketsDebug_Generic.h - WebSockets Library for boards
   
   Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWS
   to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52, Teensy, STM32F, SAM DUE boards, etc.
   
   Built by Khoi Hoang https://github.com/khoih-prog/WS_Generic
   Licensed under MIT license
   Version: 2.2.2
   
   @original file WSClient.h
   @date 20.05.2015
   @author Markus Sattler

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
  
   Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
   2.1.3   K Hoang      15/05/2020 Initial porting to support SAMD21, SAMD51, nRF52 boards, such as AdaFruit Feather nRF52832, 
                                   nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, etc.
   2.2.1   K Hoang      18/05/2020 Bump up to sync with v2.2.1 of original WS library
   2.2.2   K Hoang      25/05/2020 Add support to Teensy, SAM DUE and STM32. Enable WebSockets Server for new supported boards.
  *****************************************************************************************************************************/

#ifndef WebSocketsDebug_Generic_H
#define WebSocketsDebug_Generic_H

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


#define LOGERROR(x)     if(_WEBSOCKETS_LOGLEVEL_>0) { DBG_PORT.print("[WS] "); DBG_PORT.println(x); }
#define LOGERROR1(x,y)  if(_WEBSOCKETS_LOGLEVEL_>0) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.println(y); }
#define LOGWARN(x)      if(_WEBSOCKETS_LOGLEVEL_>1) { DBG_PORT.print("[WS] "); DBG_PORT.println(x); }
#define LOGWARN1(x,y)   if(_WEBSOCKETS_LOGLEVEL_>1) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.println(y); }
#define LOGINFO(x)      if(_WEBSOCKETS_LOGLEVEL_>2) { DBG_PORT.print("[WS] "); DBG_PORT.println(x); }
#define LOGINFO1(x,y)   if(_WEBSOCKETS_LOGLEVEL_>2) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.println(y); }
#define LOGINFO2(x,y,z) if(_WEBSOCKETS_LOGLEVEL_>3) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.print(y); DBG_PORT.print(" "); DBG_PORT.println(z); }

#define LOGDEBUG(x)      if(_WEBSOCKETS_LOGLEVEL_>3) { DBG_PORT.print("[WS] "); DBG_PORT.println(x); }
#define LOGDEBUG0(x)     if(_WEBSOCKETS_LOGLEVEL_>3) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); }
#define LOGDEBUG1(x,y)   if(_WEBSOCKETS_LOGLEVEL_>3) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.println(y); }
#define LOGDEBUG2(x,y,z) if(_WEBSOCKETS_LOGLEVEL_>3) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.print(y); DBG_PORT.print(" "); DBG_PORT.println(z); }


#endif    //WebSocketsDebug_Generic_H
