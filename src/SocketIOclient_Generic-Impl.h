/****************************************************************************************************************************
  SocketIOclient_Generic-Impl.h - WebSockets Library for boards
  
  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.
  
  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license
   
  @original file SocketIOclient.cpp
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

#include "WebSocketsClient_Generic.h"

SocketIOclient::SocketIOclient()
{
}

SocketIOclient::~SocketIOclient()
{
}

void SocketIOclient::begin(const char * host, uint16_t port, const char * url, const char * protocol)
{
  WebSocketsClient::beginSocketIO(host, port, url, protocol);
  WebSocketsClient::enableHeartbeat(60 * 1000, 90 * 1000, 5);
}

void SocketIOclient::begin(String host, uint16_t port, String url, String protocol)
{
  WebSocketsClient::beginSocketIO(host, port, url, protocol);
  WebSocketsClient::enableHeartbeat(60 * 1000, 90 * 1000, 5);
}

void SocketIOclient::begin(IPAddress host, uint16_t port, String url, String protocol)
{
  WebSocketsClient::beginSocketIO(host, port, url, protocol);
  WebSocketsClient::enableHeartbeat(60 * 1000, 90 * 1000, 5);
}

/**
   set callback function
   @param cbEvent SocketIOclientEvent
*/
void SocketIOclient::onEvent(SocketIOclientEvent cbEvent)
{
  _cbEvent = cbEvent;
}

bool SocketIOclient::isConnected(void)
{
  return WebSocketsClient::isConnected();
}

/**
   send text data to client
   @param num uint8_t client id
   @param type socketIOmessageType_t
   @param payload uint8_t
   @param length size_t
   @param headerToPayload bool (see sendFrame for more details)
   @return true if ok
*/
bool SocketIOclient::send(socketIOmessageType_t type, uint8_t * payload, size_t length, bool headerToPayload)
{
  bool ret = false;

  if (length == 0)
  {
    length = strlen((const char *)payload);
  }

  if (clientIsConnected(&_client))
  {
    if (!headerToPayload)
    {
      // webSocket Header
      ret = WebSocketsClient::sendFrameHeader(&_client, WSop_text, length + 2, true);

      // Engine.IO / Socket.IO Header
      if (ret)
      {
        uint8_t buf[3] = { eIOtype_MESSAGE, type, 0x00 };
        ret            = WebSocketsClient::write(&_client, buf, 2);
      }

      if (ret && payload && length > 0)
      {
        ret = WebSocketsClient::write(&_client, payload, length);
      }

      return ret;
    }
    else
    {
      // TODO implement
    }
  }

  return false;
}

bool SocketIOclient::send(socketIOmessageType_t type, const uint8_t * payload, size_t length)
{
  return send(type, (uint8_t *)payload, length);
}

bool SocketIOclient::send(socketIOmessageType_t type, char * payload, size_t length, bool headerToPayload)
{
  return send(type, (uint8_t *)payload, length, headerToPayload);
}

bool SocketIOclient::send(socketIOmessageType_t type, const char * payload, size_t length)
{
  return send(type, (uint8_t *)payload, length);
}

bool SocketIOclient::send(socketIOmessageType_t type, String & payload)
{
  return send(type, (uint8_t *)payload.c_str(), payload.length());
}

/**
   send text data to client
   @param num uint8_t client id
   @param payload uint8_t
   @param length size_t
   @param headerToPayload bool  (see sendFrame for more details)
   @return true if ok
*/
bool SocketIOclient::sendEVENT(uint8_t * payload, size_t length, bool headerToPayload)
{
  return send(sIOtype_EVENT, payload, length, headerToPayload);
}

bool SocketIOclient::sendEVENT(const uint8_t * payload, size_t length)
{
  return sendEVENT((uint8_t *)payload, length);
}

bool SocketIOclient::sendEVENT(char * payload, size_t length, bool headerToPayload)
{
  return sendEVENT((uint8_t *)payload, length, headerToPayload);
}

bool SocketIOclient::sendEVENT(const char * payload, size_t length)
{
  return sendEVENT((uint8_t *)payload, length);
}

bool SocketIOclient::sendEVENT(String & payload)
{
  return sendEVENT((uint8_t *)payload.c_str(), payload.length());
}

void SocketIOclient::loop(void)
{
  WebSocketsClient::loop();
  unsigned long t = millis();

  if ((t - _lastConnectionFail) > EIO_HEARTBEAT_INTERVAL)
  {
    _lastConnectionFail = t;

    WS_LOGDEBUG("[wsIOc] send ping");

    WebSocketsClient::sendTXT(eIOtype_PING);
  }
}

void SocketIOclient::handleCbEvent(WStype_t type, uint8_t * payload, size_t length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      runIOCbEvent(sIOtype_DISCONNECT, NULL, 0);

      WS_LOGDEBUG("[wsIOc] Disconnected!");

      break;
    case WStype_CONNECTED:
      {
        WS_LOGDEBUG1("[wsIOc] Connected to url:", (char *) payload);

        // send message to server when Connected
        // Engine.io upgrade confirmation message (required)
        WebSocketsClient::sendTXT(eIOtype_UPGRADE);
        runIOCbEvent(sIOtype_CONNECT, payload, length);
      } break;
    case WStype_TEXT:
      {
        if (length < 1)
        {
          break;
        }

        engineIOmessageType_t eType = (engineIOmessageType_t)payload[0];
        switch (eType)
        {
          case eIOtype_PING:
            payload[0] = eIOtype_PONG;

            WS_LOGDEBUG1("[wsIOc] get ping send pong:", (char *) payload);

            WebSocketsClient::sendTXT(payload, length, false);
            break;
          case eIOtype_PONG:
            WS_LOGDEBUG("[wsIOc] get pong");

            break;
          case eIOtype_MESSAGE:
            {
              if (length < 2)
              {
                break;
              }

              socketIOmessageType_t ioType = (socketIOmessageType_t)payload[1];
              uint8_t * data               = &payload[2];
              size_t lData                 = length - 2;

              switch (ioType)
              {
                case sIOtype_EVENT:
                  WS_LOGDEBUG1("[wsIOc] get event: ", lData);
                  WS_LOGDEBUG1("[wsIOc] get data: ", (char *) data);

                  break;
                case sIOtype_CONNECT:
                case sIOtype_DISCONNECT:
                case sIOtype_ACK:
                case sIOtype_ERROR:
                case sIOtype_BINARY_EVENT:
                case sIOtype_BINARY_ACK:
                default:
                  WS_LOGDEBUG1("[wsIOc] Socket.IO Message Type is not implemented:", ioType);
                  WS_LOGDEBUG1("[wsIOc] get text:", (char *) payload);

                  break;
              }

              runIOCbEvent(ioType, data, lData);
            } break;
          case eIOtype_OPEN:
          case eIOtype_CLOSE:
          case eIOtype_UPGRADE:
          case eIOtype_NOOP:
          default:
            WS_LOGDEBUG1("[wsIOc] Socket.IO Message Type is not implemented:", eType);
            WS_LOGDEBUG1("[wsIOc] get text:", (char *) payload);
            break;
        }
      }
      break;
    case WStype_ERROR:
    case WStype_BIN:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    case WStype_PING:
    case WStype_PONG:
      break;
  }
}

