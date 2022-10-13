/****************************************************************************************************************************
  SocketIOclient_Generic.h - WebSockets Library for boards
  
  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.
  
  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license
   
  @original file SocketIOclient.h
  @Created on: May 12, 2018
  @Author: links

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

#ifndef SOCKET_IO_CLIENT_GENERIC_H_
#define SOCKET_IO_CLIENT_GENERIC_H_

////////////////////////////////////////

#if !defined(USING_STICKY_SESSION_SIO)
  #define USING_STICKY_SESSION_SIO          false
#endif

////////////////////////////////////////

#if !defined(SIO_PING_INTERVAL)
  #define SIO_PING_INTERVAL                     60000L
#endif

#if !defined(SIO_PONG_TIMEOUT)
  #define SIO_PONG_TIMEOUT                      90000L
#endif

#if !defined(SIO_DISCONNECT_TIMEOUT_COUNT)
  #define SIO_DISCONNECT_TIMEOUT_COUNT          5
#endif

////////////////////////////////////////

#include "WebSockets_Generic.h"
#include "WebSocketsClient_Generic.h"

////////////////////////////////////////

#if !defined(EIO_HEARTBEAT_INTERVAL)
  #define EIO_HEARTBEAT_INTERVAL          20000
#endif

#define EIO_MAX_HEADER_SIZE (WEBSOCKETS_MAX_HEADER_SIZE + 1)
#define SIO_MAX_HEADER_SIZE (EIO_MAX_HEADER_SIZE + 1)

////////////////////////////////////////

typedef enum
{
  eIOtype_OPEN    = '0',    ///< Sent from the server when a new transport is opened (recheck)
  eIOtype_CLOSE   = '1',    ///< Request the close of this transport but does not shutdown the connection itself.
  eIOtype_PING    = '2',    ///< Sent by the client. Server should answer with a pong packet containing the same data
  eIOtype_PONG    = '3',    ///< Sent by the server to respond to ping packets.
  eIOtype_MESSAGE = '4',    ///< actual message, client and server should call their callbacks with the data
  eIOtype_UPGRADE = '5',    ///< Before engine.io switches a transport, it tests, if server and client can communicate over this transport. If this test succeed, the client sends an upgrade packets which requests the server to flush its cache on the old transport and switch to the new transport.
  eIOtype_NOOP    = '6',    ///< A noop packet. Used primarily to force a poll cycle when an incoming websocket connection is received.
} engineIOmessageType_t;

////////////////////////////////////////

typedef enum
{
  sIOtype_CONNECT      = '0',
  sIOtype_DISCONNECT   = '1',
  sIOtype_EVENT        = '2',
  sIOtype_ACK          = '3',
  sIOtype_ERROR        = '4',
  sIOtype_BINARY_EVENT = '5',
  sIOtype_BINARY_ACK   = '6',
  sIOtype_PING         = '7',
  sIOtype_PONG         = '8',
} socketIOmessageType_t;

////////////////////////////////////////
////////////////////////////////////////

class SocketIOclient : protected WebSocketsClient
{
  public:
#ifdef __AVR__
    typedef void (*SocketIOclientEvent)(socketIOmessageType_t type, uint8_t * payload, size_t length);
#else
    typedef std::function<void(socketIOmessageType_t type, uint8_t * payload, size_t length)> SocketIOclientEvent;
#endif

    SocketIOclient();
    virtual ~SocketIOclient();

    // KH, change to default EIO=4. v2.5.1
    void begin(const char * host, const uint16_t& port, const char * url = "/socket.io/?EIO=4", 
               const char * protocol = "arduino");
               
    void begin(const String& host, const uint16_t& port, const String& url = "/socket.io/?EIO=4", 
               const String& protocol = "arduino");
    // KH
    void begin(const IPAddress& host, const uint16_t& port, const String& url = "/socket.io/?EIO=4", 
               const String& protocol = "arduino");

#ifdef HAS_SSL
    void beginSSL(const char * host, const uint16_t& port, const char * url = "/socket.io/?EIO=4", 
                  const char * protocol = "arduino");
                  
    void beginSSL(const String& host, const uint16_t& port, const String& url = "/socket.io/?EIO=4", 
                  const String& protocol = "arduino");
#ifndef SSL_AXTLS
    void beginSSLWithCA(const char * host, const uint16_t& port, const char * url = "/socket.io/?EIO=4", 
                        const char * CA_cert = NULL, const char * protocol = "arduino");
                        
    void beginSSLWithCA(const char * host, const uint16_t& port, const char * url = "/socket.io/?EIO=4", 
                        BearSSL::X509List * CA_cert = NULL, const char * protocol = "arduino");
                        
    void setSSLClientCertKey(const char * clientCert = NULL, const char * clientPrivateKey = NULL);
    
    void setSSLClientCertKey(BearSSL::X509List * clientCert = NULL, BearSSL::PrivateKey * clientPrivateKey = NULL);
#endif
#endif

    bool isConnected();

    void onEvent(SocketIOclientEvent cbEvent);

    bool sendEVENT(uint8_t * payload, size_t length = 0, bool headerToPayload = false);
    bool sendEVENT(const uint8_t * payload, size_t length = 0);
    bool sendEVENT(char * payload, size_t length = 0, bool headerToPayload = false);
    bool sendEVENT(const char * payload, size_t length = 0);
    bool sendEVENT(const String& payload);

    bool send(socketIOmessageType_t type, uint8_t * payload, size_t length = 0, bool headerToPayload = false);
    bool send(socketIOmessageType_t type, const uint8_t * payload, size_t length = 0);
    bool send(socketIOmessageType_t type, char * payload, size_t length = 0, bool headerToPayload = false);
    bool send(socketIOmessageType_t type, const char * payload, size_t length = 0);
    bool send(socketIOmessageType_t type, const String& payload);
    
    void loop();
    
    void configureEIOping(bool disableHeartbeat = false);

    ////////////////////////////////////////
    
    // KH, add v2.5.1
    inline void setReconnectInterval(const unsigned long& time)
    {
      _reconnectInterval = time;
    }

    ////////////////////////////////////////

    inline void setExtraHeaders(const char * extraHeaders = nullptr)
    {
      WebSocketsClient::setExtraHeaders(extraHeaders);
    }

  protected:
    bool _disableHeartbeat  = false;
    uint64_t _lastHeartbeat = 0;
    SocketIOclientEvent _cbEvent;

    ////////////////////////////////////////
    
    virtual void runIOCbEvent(socketIOmessageType_t type, uint8_t * payload, size_t length)
    {
      if (_cbEvent)
      {
        _cbEvent(type, payload, length);
      }
    }

    ////////////////////////////////////////
    
    void initClient();

    ////////////////////////////////////////

    // Handling events from websocket layer
    virtual void runCbEvent(WStype_t type, uint8_t * payload, size_t length)
    {
      handleCbEvent(type, payload, length);
    }

    ////////////////////////////////////////

    void handleCbEvent(WStype_t type, uint8_t * payload, size_t length);
};

////////////////////////////////////////

#include "SocketIOclient_Generic-Impl.h"

#endif    // SOCKET_IO_CLIENT_GENERIC_H_

