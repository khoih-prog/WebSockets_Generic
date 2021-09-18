/****************************************************************************************************************************
  WebSockets4WebServer_Generic.h - WebSockets Library for boards
  
  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.
  
  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license
   
  @original file WebSocketsServer.cpp
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
  
  Version: 2.10.0

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
 *****************************************************************************************************************************/

#pragma once

#ifndef __WEBSOCKETS4WEBSERVER_GENERIC_H
#define __WEBSOCKETS4WEBSERVER_GENERIC_H

#include <WebSocketsServer_Generic.h>
#include <ESP8266WebServer.h>

#if WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266 && WEBSERVER_HAS_HOOK

class WebSockets4WebServer : public WebSocketsServerCore
{
  public:
    WebSockets4WebServer(const String & origin = "", const String & protocol = "arduino")
      : WebSocketsServerCore(origin, protocol)
    {
      begin();
    }

    ESP8266WebServer::HookFunction hookForWebserver(const String & wsRootDir, WebSocketServerEvent event)
    {
      onEvent(event);

      return [&, wsRootDir](const String & method, const String & url, WiFiClient * tcpClient, ESP8266WebServer::ContentTypeFunction contentType) 
      {
        (void)contentType;

        if (!(method == "GET" && url.indexOf(wsRootDir) == 0)) 
        {
          return ESP8266WebServer::CLIENT_REQUEST_CAN_CONTINUE;
        }

        // allocate a WiFiClient copy (like in WebSocketsServer::handleNewClients())
        WEBSOCKETS_NETWORK_CLASS * newTcpClient = new WEBSOCKETS_NETWORK_CLASS(*tcpClient);

        // Then initialize a new WSclient_t (like in WebSocketsServer::handleNewClient())
        WSclient_t * client = handleNewClient(newTcpClient);

        if (client) 
        {
          // give "GET <url>"
          String headerLine;
          headerLine.reserve(url.length() + 5);
          headerLine = "GET ";
          headerLine += url;
          handleHeader(client, &headerLine);
        }

        // tell webserver to not close but forget about this client
        return ESP8266WebServer::CLIENT_IS_GIVEN;
      };
    }
};
#else    // WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266 && WEBSERVER_HAS_HOOK

#ifndef WEBSERVER_HAS_HOOK
  #error Your current Framework / Arduino core version does not support Webserver Hook Functions
#else
  #error Your Hardware Platform does not support Webserver Hook Functions
#endif

#endif    // WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266 && WEBSERVER_HAS_HOOK

#endif    // __WEBSOCKETS4WEBSERVER_GENERIC_H
