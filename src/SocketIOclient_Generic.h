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

#ifndef SOCKET_IO_CLIENT_GENERIC_H_
#define SOCKET_IO_CLIENT_GENERIC_H_

#if !defined(USING_STICKY_SESSION_SIO)
  #define USING_STICKY_SESSION_SIO          false
#endif

#include "WebSockets_Generic.h"

#define EIO_HEARTBEAT_INTERVAL 20000

#define EIO_MAX_HEADER_SIZE (WEBSOCKETS_MAX_HEADER_SIZE + 1)
#define SIO_MAX_HEADER_SIZE (EIO_MAX_HEADER_SIZE + 1)

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

typedef enum
{
  sIOtype_CONNECT      = '0',
  sIOtype_DISCONNECT   = '1',
  sIOtype_EVENT        = '2',
  sIOtype_ACK          = '3',
  sIOtype_ERROR        = '4',
  sIOtype_BINARY_EVENT = '5',
  sIOtype_BINARY_ACK   = '6',
} socketIOmessageType_t;

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
    void begin(const char * host, uint16_t port, const char * url = "/socket.io/?EIO=4", const char * protocol = "arduino");
    void begin(String host, uint16_t port, String url = "/socket.io/?EIO=4", String protocol = "arduino");
    // KH
    void begin(IPAddress host, uint16_t port, String url = "/socket.io/?EIO=4", String protocol = "arduino");

#ifdef HAS_SSL
    void beginSSL(const char * host, uint16_t port, const char * url = "/socket.io/?EIO=4", const char * protocol = "arduino");
    void beginSSL(String host, uint16_t port, String url = "/socket.io/?EIO=4", String protocol = "arduino");
#ifndef SSL_AXTLS
    void beginSSLWithCA(const char * host, uint16_t port, const char * url = "/socket.io/?EIO=4", const char * CA_cert = NULL, const char * protocol = "arduino");
    void beginSSLWithCA(const char * host, uint16_t port, const char * url = "/socket.io/?EIO=4", BearSSL::X509List * CA_cert = NULL, const char * protocol = "arduino");
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
    bool sendEVENT(String & payload);

    bool send(socketIOmessageType_t type, uint8_t * payload, size_t length = 0, bool headerToPayload = false);
    bool send(socketIOmessageType_t type, const uint8_t * payload, size_t length = 0);
    bool send(socketIOmessageType_t type, char * payload, size_t length = 0, bool headerToPayload = false);
    bool send(socketIOmessageType_t type, const char * payload, size_t length = 0);
    bool send(socketIOmessageType_t type, String & payload);

    void loop();
    
    void configureEIOping(bool disableHeartbeat = false);
    
    // KH, add v2.5.1
    void setReconnectInterval(unsigned long time)
    {
      _reconnectInterval = time;
    }
    
    void setExtraHeaders(const char * extraHeaders = nullptr)
    {
      WebSocketsClient::setExtraHeaders(extraHeaders);
    }

  protected:
    bool _disableHeartbeat  = false;
    uint64_t _lastHeartbeat = 0;
    SocketIOclientEvent _cbEvent;
    virtual void runIOCbEvent(socketIOmessageType_t type, uint8_t * payload, size_t length)
    {
      if (_cbEvent)
      {
        _cbEvent(type, payload, length);
      }
    }
    
    void initClient();

    // Handling events from websocket layer
    virtual void runCbEvent(WStype_t type, uint8_t * payload, size_t length)
    {
      handleCbEvent(type, payload, length);
    }

    void handleCbEvent(WStype_t type, uint8_t * payload, size_t length);
};

#include "SocketIOclient_Generic-Impl.h"

#endif    // SOCKET_IO_CLIENT_GENERIC_H_

