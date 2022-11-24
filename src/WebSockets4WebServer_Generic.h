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

#ifndef __WEBSOCKETS4WEBSERVER_GENERIC_H
#define __WEBSOCKETS4WEBSERVER_GENERIC_H

#include <WebSocketsServer_Generic.h>
#include <ESP8266WebServer.h>

#if WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266 && WEBSERVER_HAS_HOOK

////////////////////////////////////////
////////////////////////////////////////

class WebSockets4WebServer : public WebSocketsServerCore
{
  public:

    ////////////////////////////////////////

    WebSockets4WebServer(const String & origin = "", const String & protocol = "arduino")
      : WebSocketsServerCore(origin, protocol)
    {
      begin();
    }

    ////////////////////////////////////////

    ESP8266WebServer::HookFunction hookForWebserver(const String & wsRootDir, WebSocketServerEvent event)
    {
      onEvent(event);

      return [&, wsRootDir](const String & method, const String & url, WiFiClient * tcpClient,
                            ESP8266WebServer::ContentTypeFunction contentType)
      {
        UNUSED (contentType);

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

////////////////////////////////////////
////////////////////////////////////////

#else    // WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266 && WEBSERVER_HAS_HOOK

#ifndef WEBSERVER_HAS_HOOK
  #error Your current Framework / Arduino core version does not support Webserver Hook Functions
#else
  #error Your Hardware Platform does not support Webserver Hook Functions
#endif

#endif    // WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266 && WEBSERVER_HAS_HOOK

////////////////////////////////////////

#endif    // __WEBSOCKETS4WEBSERVER_GENERIC_H
