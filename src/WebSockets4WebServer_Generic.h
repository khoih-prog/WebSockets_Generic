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
