/****************************************************************************************************************************
  WebSocketsServer_Generic.h - WebSockets Library for boards

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  @original file WebSocketsServer.h
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

#ifndef WEBSOCKETS_SERVER_GENERIC_H_
#define WEBSOCKETS_SERVER_GENERIC_H_

#include "WebSockets_Generic.h"

#ifndef WEBSOCKETS_SERVER_CLIENT_MAX
  #define WEBSOCKETS_SERVER_CLIENT_MAX (5)
#endif

class WebSocketsServerCore : protected WebSockets
{
  public:
    WebSocketsServerCore(const String & origin = "", const String & protocol = "arduino");
    virtual ~WebSocketsServerCore();

    void begin();
    void close();

#ifdef __AVR__
    typedef void (*WebSocketServerEvent)(const uint8_t& num, const WStype_t& type, uint8_t * payload, const size_t& length);
    typedef bool (*WebSocketServerHttpHeaderValFunc)(const String& headerName, const String& headerValue);
#else
    typedef std::function<void(const uint8_t& num, const WStype_t& type, uint8_t * payload, const size_t& length)>
    WebSocketServerEvent;
    typedef std::function<bool(const String& headerName, const String& headerValue)> WebSocketServerHttpHeaderValFunc;
#endif

    void onEvent(WebSocketServerEvent cbEvent);
    void onValidateHttpHeader(
      WebSocketServerHttpHeaderValFunc validationFunc,
      const char * mandatoryHttpHeaders[],
      size_t mandatoryHttpHeaderCount);

    bool sendTXT(const uint8_t& num, uint8_t * payload, size_t length = 0, bool headerToPayload = false);
    bool sendTXT(const uint8_t& num, const uint8_t * payload, size_t length = 0);
    bool sendTXT(const uint8_t& num, char * payload, size_t length = 0, bool headerToPayload = false);
    bool sendTXT(const uint8_t& num, const char * payload, size_t length = 0);
    bool sendTXT(const uint8_t& num, const String& payload);

    bool broadcastTXT(uint8_t * payload, size_t length = 0, bool headerToPayload = false);
    bool broadcastTXT(const uint8_t * payload, size_t length = 0);
    bool broadcastTXT(char * payload, size_t length = 0, bool headerToPayload = false);
    bool broadcastTXT(const char * payload, size_t length = 0);
    bool broadcastTXT(const String& payload);

    bool sendBIN(const uint8_t& num, uint8_t * payload, size_t length, bool headerToPayload = false);
    bool sendBIN(const uint8_t& num, const uint8_t * payload, size_t length);

    bool broadcastBIN(uint8_t * payload, size_t length, bool headerToPayload = false);
    bool broadcastBIN(const uint8_t * payload, size_t length);

    bool sendPing(const uint8_t& num, uint8_t * payload = NULL, size_t length = 0);
    bool sendPing(const uint8_t& num, const String& payload);

    bool broadcastPing(uint8_t * payload = NULL, size_t length = 0);
    bool broadcastPing(const String& payload);

    void disconnect();
    void disconnect(const uint8_t& num);

    void setAuthorization(const char * user, const char * password);
    void setAuthorization(const char * auth);

    int connectedClients(bool ping = false);

    bool clientIsConnected(const uint8_t& num);

    void enableHeartbeat(const uint32_t& pingInterval, const uint32_t& pongTimeout, const uint8_t& disconnectTimeoutCount);
    void disableHeartbeat();

#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)\
     || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)
    IPAddress remoteIP(const uint8_t& num);
#endif

#if(WEBSOCKETS_NETWORK_TYPE != NETWORK_ESP8266_ASYNC)
    void loop();    // handle client data only
#endif

    WSclient_t * newClient(WEBSOCKETS_NETWORK_CLASS * TCPclient);

  protected:

    // KH Debug
    uint8_t currentActiveClient = 0xFF;
    //////

    String _origin;
    String _protocol;
    String _base64Authorization;    ///< Base64 encoded Auth request
    String * _mandatoryHttpHeaders;
    size_t _mandatoryHttpHeaderCount;

    WSclient_t _clients[WEBSOCKETS_SERVER_CLIENT_MAX];

    WebSocketServerEvent _cbEvent;
    WebSocketServerHttpHeaderValFunc _httpHeaderValidationFunc;

    bool _runnning;

    uint32_t _pingInterval;
    uint32_t _pongTimeout;
    uint8_t _disconnectTimeoutCount;

    void messageReceived(WSclient_t * client, WSopcode_t opcode, uint8_t * payload, size_t length, bool fin);

    void clientDisconnect(WSclient_t * client);
    bool clientIsConnected(WSclient_t * client);

#if(WEBSOCKETS_NETWORK_TYPE != NETWORK_ESP8266_ASYNC)
    void handleClientData();
#endif

    void handleHeader(WSclient_t * client, String& headerLine);

    void handleHBPing(WSclient_t * client);    // send ping in specified intervals

    /**
           called if a non Websocket connection is coming in.
           Note: can be override
           @param client WSclient_t *  ptr to the client struct
    */
    virtual void handleNonWebsocketConnection(WSclient_t * client)
    {
      WSK_LOGDEBUG1("[WS-Server handleHeader] no Websocket connection close. Client =", client->num);

      client->tcp->write(
        "HTTP/1.1 400 Bad Request\r\n"
        "Server: arduino-WebSocket-Server\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 32\r\n"
        "Connection: close\r\n"
        "Sec-WebSocket-Version: 13\r\n"
        "\r\n"
        "This is a Websocket server only!");

      clientDisconnect(client);
    }

    /**
           called if a non Authorization connection is coming in.
           Note: can be override
           @param client WSclient_t *  ptr to the client struct
    */
    virtual void handleAuthorizationFailed(WSclient_t * client)
    {
      client->tcp->write(
        "HTTP/1.1 401 Unauthorized\r\n"
        "Server: arduino-WebSocket-Server\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 45\r\n"
        "Connection: close\r\n"
        "Sec-WebSocket-Version: 13\r\n"
        "WWW-Authenticate: Basic realm=\"WebSocket Server\""
        "\r\n"
        "This Websocket server requires Authorization!");

      clientDisconnect(client);
    }

    /**
           called for sending a Event to the app
           @param num uint8_t
           @param type WStype_t
           @param payload uint8_t
           @param length size_t
    */
    //virtual void runCbEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
    virtual void runCbEvent(const uint8_t& num, const WStype_t& type, uint8_t * payload, const size_t& length)
    {
      if (_cbEvent)
      {
        _cbEvent(num, type, payload, length);
      }
    }

    /*
           Called at client socket connect handshake negotiation time for each http header that is not
           a websocket specific http header (not Connection, Upgrade, Sec-WebSocket-*)
           If the custom httpHeaderValidationFunc returns false for any headerName / headerValue passed, the
           socket negotiation is considered invalid and the upgrade to websockets request is denied / rejected
           This mechanism can be used to enable custom authentication schemes e.g. test the value
           of a session cookie to determine if a user is logged on / authenticated
    */
    virtual bool execHttpHeaderValidation(const String& headerName, const String& headerValue)
    {
      if (_httpHeaderValidationFunc)
      {
        //return the value of the custom http header validation function
        return _httpHeaderValidationFunc(headerName, headerValue);
      }

      //no custom http header validation so just assume all is good
      return true;
    }

#if(WEBSOCKETS_NETWORK_TYPE != NETWORK_ESP8266_ASYNC)
    WSclient_t * handleNewClient(WEBSOCKETS_NETWORK_CLASS * tcpClient);
#endif

    /**
       drop native tcp connection (client->tcp)
    */
    void dropNativeClient(WSclient_t * client);

  private:
    /*
           returns an indicator whether the given named header exists in the configured _mandatoryHttpHeaders collection
           @param headerName String ///< the name of the header being checked
    */
    bool hasMandatoryHeader(const String& headerName);
};

class WebSocketsServer : public WebSocketsServerCore
{
  public:
    WebSocketsServer(const uint16_t& port, const String & origin = "", const String & protocol = "arduino");
    virtual ~WebSocketsServer();

    void begin();
    void close();

#if(WEBSOCKETS_NETWORK_TYPE != NETWORK_ESP8266_ASYNC)
    void loop();    // handle incoming client and client data
#else
    // Async interface not need a loop call
    void loop() __attribute__((deprecated)) {}
#endif

  protected:
#if(WEBSOCKETS_NETWORK_TYPE != NETWORK_ESP8266_ASYNC)
    void handleNewClients();
#endif

    // KH Debug
    uint8_t currentActiveClient = 0xFF;
    //////

    uint16_t _port;
    WEBSOCKETS_NETWORK_SERVER_CLASS * _server;
};


#include "WebSocketsServer_Generic-Impl.h"

#endif    // WEBSOCKETS_SERVER_GENERIC_H_
