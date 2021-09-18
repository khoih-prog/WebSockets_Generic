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
    typedef void (*WebSocketServerEvent)(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
    typedef bool (*WebSocketServerHttpHeaderValFunc)(String headerName, String headerValue);
#else
    typedef std::function<void(uint8_t num, WStype_t type, uint8_t * payload, size_t length)> WebSocketServerEvent;
    typedef std::function<bool(String headerName, String headerValue)> WebSocketServerHttpHeaderValFunc;
#endif

    void onEvent(WebSocketServerEvent cbEvent);
    void onValidateHttpHeader(
        WebSocketServerHttpHeaderValFunc validationFunc,
        const char * mandatoryHttpHeaders[],
        size_t mandatoryHttpHeaderCount);

    bool sendTXT(uint8_t num, uint8_t * payload, size_t length = 0, bool headerToPayload = false);
    bool sendTXT(uint8_t num, const uint8_t * payload, size_t length = 0);
    bool sendTXT(uint8_t num, char * payload, size_t length = 0, bool headerToPayload = false);
    bool sendTXT(uint8_t num, const char * payload, size_t length = 0);
    bool sendTXT(uint8_t num, String & payload);

    bool broadcastTXT(uint8_t * payload, size_t length = 0, bool headerToPayload = false);
    bool broadcastTXT(const uint8_t * payload, size_t length = 0);
    bool broadcastTXT(char * payload, size_t length = 0, bool headerToPayload = false);
    bool broadcastTXT(const char * payload, size_t length = 0);
    bool broadcastTXT(String & payload);

    bool sendBIN(uint8_t num, uint8_t * payload, size_t length, bool headerToPayload = false);
    bool sendBIN(uint8_t num, const uint8_t * payload, size_t length);

    bool broadcastBIN(uint8_t * payload, size_t length, bool headerToPayload = false);
    bool broadcastBIN(const uint8_t * payload, size_t length);

    bool sendPing(uint8_t num, uint8_t * payload = NULL, size_t length = 0);
    bool sendPing(uint8_t num, String & payload);

    bool broadcastPing(uint8_t * payload = NULL, size_t length = 0);
    bool broadcastPing(String & payload);

    void disconnect();
    void disconnect(uint8_t num);

    void setAuthorization(const char * user, const char * password);
    void setAuthorization(const char * auth);

    int connectedClients(bool ping = false);

    bool clientIsConnected(uint8_t num);

    void enableHeartbeat(uint32_t pingInterval, uint32_t pongTimeout, uint8_t disconnectTimeoutCount);
    void disableHeartbeat();

#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)\
     || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)
    IPAddress remoteIP(uint8_t num);
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

    void handleHeader(WSclient_t * client, String * headerLine);

    void handleHBPing(WSclient_t * client);    // send ping in specified intervals

    /**
         * called if a non Websocket connection is coming in.
         * Note: can be override
         * @param client WSclient_t *  ptr to the client struct
         */
    virtual void handleNonWebsocketConnection(WSclient_t * client) {
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
         * called if a non Authorization connection is coming in.
         * Note: can be override
         * @param client WSclient_t *  ptr to the client struct
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
         * called for sending a Event to the app
         * @param num uint8_t
         * @param type WStype_t
         * @param payload uint8_t *
         * @param length size_t
         */
    virtual void runCbEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) 
    {
        if(_cbEvent) 
        {
            _cbEvent(num, type, payload, length);
        }
    }

    /*
         * Called at client socket connect handshake negotiation time for each http header that is not
         * a websocket specific http header (not Connection, Upgrade, Sec-WebSocket-*)
         * If the custom httpHeaderValidationFunc returns false for any headerName / headerValue passed, the
         * socket negotiation is considered invalid and the upgrade to websockets request is denied / rejected
         * This mechanism can be used to enable custom authentication schemes e.g. test the value
         * of a session cookie to determine if a user is logged on / authenticated
         */
    virtual bool execHttpHeaderValidation(String headerName, String headerValue) 
    {
        if(_httpHeaderValidationFunc) 
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
     * drop native tcp connection (client->tcp)
    */
    void dropNativeClient(WSclient_t * client);

  private:
    /*
         * returns an indicator whether the given named header exists in the configured _mandatoryHttpHeaders collection
         * @param headerName String ///< the name of the header being checked
         */
    bool hasMandatoryHeader(String headerName);
};

class WebSocketsServer : public WebSocketsServerCore 
{
  public:
    WebSocketsServer(uint16_t port, const String & origin = "", const String & protocol = "arduino");
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


#if 0
/////////////////////////////////////////////////////////////////////////////////

class WebSocketsServer : protected WebSockets
{
  public:
#ifdef __AVR__
    typedef void (*WebSocketServerEvent)(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
    typedef bool (*WebSocketServerHttpHeaderValFunc)(String headerName, String headerValue);
#else
    typedef std::function<void(uint8_t num, WStype_t type, uint8_t * payload, size_t length)> WebSocketServerEvent;
    typedef std::function<bool(String headerName, String headerValue)> WebSocketServerHttpHeaderValFunc;
#endif

    WebSocketsServer(uint16_t port, String origin = "", String protocol = "arduino");
    virtual ~WebSocketsServer();

    //KH New Debug
    void displayClientData(WSclient_t *client, bool onlyTCPValid = true);
    
    void begin();
    void close();

#if(WEBSOCKETS_NETWORK_TYPE != NETWORK_ESP8266_ASYNC)
    void loop();
#else
    // Async interface not need a loop call
    void loop() __attribute__((deprecated)) {}
#endif

    void onEvent(WebSocketServerEvent cbEvent);
    void onValidateHttpHeader(
      WebSocketServerHttpHeaderValFunc validationFunc,
      const char * mandatoryHttpHeaders[],
      size_t mandatoryHttpHeaderCount);

    bool sendTXT(uint8_t num, uint8_t * payload, size_t length = 0, bool headerToPayload = false);
    bool sendTXT(uint8_t num, const uint8_t * payload, size_t length = 0);
    bool sendTXT(uint8_t num, char * payload, size_t length = 0, bool headerToPayload = false);
    bool sendTXT(uint8_t num, const char * payload, size_t length = 0);
    bool sendTXT(uint8_t num, String & payload);

    bool broadcastTXT(uint8_t * payload, size_t length = 0, bool headerToPayload = false);
    bool broadcastTXT(const uint8_t * payload, size_t length = 0);
    bool broadcastTXT(char * payload, size_t length = 0, bool headerToPayload = false);
    bool broadcastTXT(const char * payload, size_t length = 0);
    bool broadcastTXT(String & payload);

    bool sendBIN(uint8_t num, uint8_t * payload, size_t length, bool headerToPayload = false);
    bool sendBIN(uint8_t num, const uint8_t * payload, size_t length);

    bool broadcastBIN(uint8_t * payload, size_t length, bool headerToPayload = false);
    bool broadcastBIN(const uint8_t * payload, size_t length);

    bool sendPing(uint8_t num, uint8_t * payload = NULL, size_t length = 0);
    bool sendPing(uint8_t num, String & payload);

    bool broadcastPing(uint8_t * payload = NULL, size_t length = 0);
    bool broadcastPing(String & payload);

    void disconnect();
    void disconnect(uint8_t num);

    void setAuthorization(const char * user, const char * password);
    void setAuthorization(const char * auth);

    int connectedClients(bool ping = false);
    
    bool clientIsConnected(uint8_t num);

    void enableHeartbeat(uint32_t pingInterval, uint32_t pongTimeout, uint8_t disconnectTimeoutCount);
    void disableHeartbeat();

#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)\
     || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)
    IPAddress remoteIP(uint8_t num);
#endif

    WSclient_t * newClient(WEBSOCKETS_NETWORK_CLASS * TCPclient);

  protected:
  
    // KH Debug
    uint8_t currentActiveClient = 0xFF;
    //////
    
    uint16_t _port;
    String     _origin;
    String    _protocol;
    String    _base64Authorization;    ///< Base64 encoded Auth request
    String *  _mandatoryHttpHeaders;
    size_t    _mandatoryHttpHeaderCount;

    WEBSOCKETS_NETWORK_SERVER_CLASS * _server;

    WSclient_t _clients[WEBSOCKETS_SERVER_CLIENT_MAX];

    WebSocketServerEvent _cbEvent;
    WebSocketServerHttpHeaderValFunc _httpHeaderValidationFunc;

    bool _runnning;

    uint32_t  _pingInterval;
    uint32_t  _pongTimeout;
    uint8_t   _disconnectTimeoutCount;



    void messageReceived(WSclient_t * client, WSopcode_t opcode, uint8_t * payload, size_t length, bool fin);

    void clientDisconnect(WSclient_t * client);
    bool clientIsConnected(WSclient_t * client);

#if(WEBSOCKETS_NETWORK_TYPE != NETWORK_ESP8266_ASYNC)
    void handleNewClients();
    void handleClientData();
#endif

    void handleHeader(WSclient_t * client, String * headerLine);

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
    virtual void runCbEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
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
    virtual bool execHttpHeaderValidation(String headerName, String headerValue)
    {
      if (_httpHeaderValidationFunc)
      {
        //return the value of the custom http header validation function
        return _httpHeaderValidationFunc(headerName, headerValue);
      }
      
      //no custom http header validation so just assume all is good
      return true;
    }

  private:
    /*
           returns an indicator whether the given named header exists in the configured _mandatoryHttpHeaders collection
           @param headerName String ///< the name of the header being checked
    */
    bool hasMandatoryHeader(String headerName);
};
#endif

#include "WebSocketsServer_Generic-Impl.h"

#endif    // WEBSOCKETS_SERVER_GENERIC_H_
