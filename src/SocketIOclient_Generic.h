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

    SocketIOclient(void);
    virtual ~SocketIOclient(void);

    void begin(const char * host, uint16_t port, const char * url = "/socket.io/?EIO=3", const char * protocol = "arduino");
    void begin(String host, uint16_t port, String url = "/socket.io/?EIO=3", String protocol = "arduino");
    // KH
    void begin(IPAddress host, uint16_t port, String url = "/socket.io/?EIO=3", String protocol = "arduino");

    bool isConnected(void);

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

    void loop(void);

  protected:
    uint64_t _lastHeartbeat = 0;
    SocketIOclientEvent _cbEvent;
    virtual void runIOCbEvent(socketIOmessageType_t type, uint8_t * payload, size_t length)
    {
      if (_cbEvent)
      {
        _cbEvent(type, payload, length);
      }
    }

    // Handeling events from websocket layer
    virtual void runCbEvent(WStype_t type, uint8_t * payload, size_t length)
    {
      handleCbEvent(type, payload, length);
    }

    void handleCbEvent(WStype_t type, uint8_t * payload, size_t length);
};

#include "SocketIOclient_Generic-Impl.h"

