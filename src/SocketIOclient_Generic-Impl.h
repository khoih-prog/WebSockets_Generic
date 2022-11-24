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

#ifndef SOCKET_IO_CLIENT_GENERIC_IMPL_H_
#define SOCKET_IO_CLIENT_GENERIC_IMPL_H_

#include "WebSocketsClient_Generic.h"

////////////////////////////////////////

SocketIOclient::SocketIOclient()
{
}

////////////////////////////////////////

SocketIOclient::~SocketIOclient()
{
}

////////////////////////////////////////

void SocketIOclient::begin(const char * host, const uint16_t& port, const char * url, const char * protocol)
{
  WebSocketsClient::beginSocketIO(host, port, url, protocol);
  WebSocketsClient::enableHeartbeat(SIO_PING_INTERVAL, SIO_PONG_TIMEOUT, SIO_DISCONNECT_TIMEOUT_COUNT);
  initClient();
}

////////////////////////////////////////

void SocketIOclient::begin(const String& host, const uint16_t& port, const String& url, const String& protocol)
{
  WebSocketsClient::beginSocketIO(host, port, url, protocol);
  WebSocketsClient::enableHeartbeat(SIO_PING_INTERVAL, SIO_PONG_TIMEOUT, SIO_DISCONNECT_TIMEOUT_COUNT);
  initClient();
}

////////////////////////////////////////

void SocketIOclient::begin(const IPAddress& host, const uint16_t& port, const String& url, const String& protocol)
{
  WebSocketsClient::beginSocketIO(host, port, url, protocol);
  WebSocketsClient::enableHeartbeat(SIO_PING_INTERVAL, SIO_PONG_TIMEOUT, SIO_DISCONNECT_TIMEOUT_COUNT);
  initClient();
}

////////////////////////////////////////

#if defined(HAS_SSL)

////////////////////////////////////////

void SocketIOclient::beginSSL(const char * host, const uint16_t& port, const char * url, const char * protocol)
{
  WebSocketsClient::beginSocketIOSSL(host, port, url, protocol);
  WebSocketsClient::enableHeartbeat(SIO_PING_INTERVAL, SIO_PONG_TIMEOUT, SIO_DISCONNECT_TIMEOUT_COUNT);
  initClient();
}

////////////////////////////////////////

void SocketIOclient::beginSSL(const String& host, const uint16_t& port, const String& url, const String& protocol)
{
  WebSocketsClient::beginSocketIOSSL(host, port, url, protocol);
  WebSocketsClient::enableHeartbeat(SIO_PING_INTERVAL, SIO_PONG_TIMEOUT, SIO_DISCONNECT_TIMEOUT_COUNT);
  initClient();
}

////////////////////////////////////////

#if defined(SSL_BARESSL)

////////////////////////////////////////

void SocketIOclient::beginSSLWithCA(const char * host, const uint16_t& port, const char * url,
                                    const char * CA_cert, const char * protocol)
{
  WebSocketsClient::beginSocketIOSSLWithCA(host, port, url, CA_cert, protocol);
  WebSocketsClient::enableHeartbeat(SIO_PING_INTERVAL, SIO_PONG_TIMEOUT, SIO_DISCONNECT_TIMEOUT_COUNT);
  initClient();
}

////////////////////////////////////////

void SocketIOclient::beginSSLWithCA(const char * host, const uint16_t& port, const char * url,
                                    BearSSL::X509List * CA_cert, const char * protocol)
{
  WebSocketsClient::beginSocketIOSSLWithCA(host, port, url, CA_cert, protocol);
  WebSocketsClient::enableHeartbeat(SIO_PING_INTERVAL, SIO_PONG_TIMEOUT, SIO_DISCONNECT_TIMEOUT_COUNT);
  initClient();
}

////////////////////////////////////////

void SocketIOclient::setSSLClientCertKey(const char * clientCert, const char * clientPrivateKey)
{
  WebSocketsClient::setSSLClientCertKey(clientCert, clientPrivateKey);
}

////////////////////////////////////////

void SocketIOclient::setSSLClientCertKey(BearSSL::X509List * clientCert, BearSSL::PrivateKey * clientPrivateKey)
{
  WebSocketsClient::setSSLClientCertKey(clientCert, clientPrivateKey);
}

////////////////////////////////////////

#endif    // #if defined(SSL_BARESSL)
#endif    // #if defined(HAS_SSL)

////////////////////////////////////////

void SocketIOclient::configureEIOping(bool disableHeartbeat)
{
  _disableHeartbeat = disableHeartbeat;
}

////////////////////////////////////////

void SocketIOclient::initClient()
{
  if (_client.cUrl.indexOf("EIO=4") != -1)
  {
    WSK_LOGINFO("[wsIOc] found EIO=4 disable EIO ping on client");
    configureEIOping(true);
  }
}

////////////////////////////////////////

/**
   set callback function
   @param cbEvent SocketIOclientEvent
*/
void SocketIOclient::onEvent(SocketIOclientEvent cbEvent)
{
  _cbEvent = cbEvent;
}

////////////////////////////////////////

bool SocketIOclient::isConnected()
{
  return WebSocketsClient::isConnected();
}

////////////////////////////////////////

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

  if (clientIsConnected(&_client) && _client.status == WSC_CONNECTED)
  {
    if (!headerToPayload)
    {
      // webSocket Header
      ret = WebSocketsClient::sendFrameHeader(&_client, WSop_text, length + 2, true);

      // Engine.IO / Socket.IO Header
      if (ret)
      {
        uint8_t buf[3] = { eIOtype_MESSAGE, type, 0x00 };

        ret = WebSocketsClient::write(&_client, buf, 2);
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

////////////////////////////////////////

bool SocketIOclient::send(socketIOmessageType_t type, const uint8_t * payload, size_t length)
{
  return send(type, (uint8_t *)payload, length);
}

////////////////////////////////////////

bool SocketIOclient::send(socketIOmessageType_t type, char * payload, size_t length, bool headerToPayload)
{
  return send(type, (uint8_t *)payload, length, headerToPayload);
}

////////////////////////////////////////

bool SocketIOclient::send(socketIOmessageType_t type, const char * payload, size_t length)
{
  return send(type, (uint8_t *)payload, length);
}

////////////////////////////////////////

bool SocketIOclient::send(socketIOmessageType_t type, const String& payload)
{
  return send(type, (uint8_t *)payload.c_str(), payload.length());
}

////////////////////////////////////////

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

////////////////////////////////////////

bool SocketIOclient::sendEVENT(const uint8_t * payload, size_t length)
{
  return sendEVENT((uint8_t *)payload, length);
}

////////////////////////////////////////

bool SocketIOclient::sendEVENT(char * payload, size_t length, bool headerToPayload)
{
  return sendEVENT((uint8_t *)payload, length, headerToPayload);
}

////////////////////////////////////////

bool SocketIOclient::sendEVENT(const char * payload, size_t length)
{
  return sendEVENT((uint8_t *)payload, length);
}

////////////////////////////////////////

bool SocketIOclient::sendEVENT(const String& payload)
{
  return sendEVENT((uint8_t *)payload.c_str(), payload.length());
}

////////////////////////////////////////

void SocketIOclient::loop()
{
  WebSocketsClient::loop();
  unsigned long t = millis();

  if (!_disableHeartbeat && (t - _lastHeartbeat) > EIO_HEARTBEAT_INTERVAL)
  {
    _lastHeartbeat = t;
    WSK_LOGINFO("[wsIOc] send ping\n");
    WebSocketsClient::sendTXT(eIOtype_PING);
  }
}

////////////////////////////////////////

void SocketIOclient::handleCbEvent(WStype_t type, uint8_t * payload, size_t length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      runIOCbEvent(sIOtype_DISCONNECT, NULL, 0);

      WSK_LOGINFO("[wsIOc] Disconnected!");

      break;

    case WStype_CONNECTED:
    {
      WSK_LOGWARN1("[wsIOc] Connected to url:", (char *) payload);

      // send message to server when Connected
      // Engine.io upgrade confirmation message (required)
      WebSocketsClient::sendTXT("2probe");
      WebSocketsClient::sendTXT(eIOtype_UPGRADE);
      runIOCbEvent(sIOtype_CONNECT, payload, length);
    }

    break;

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

          WSK_LOGWARN1("[wsIOc] get ping send pong:", (char *) payload);

          WebSocketsClient::sendTXT(payload, length, false);
          runIOCbEvent(sIOtype_PING, payload, length);

          break;

        case eIOtype_PONG:
          WSK_LOGWARN("[wsIOc] get pong");
          runIOCbEvent(sIOtype_PONG, payload, length);

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
              WSK_LOGWARN1("[wsIOc] get event: len = ", lData);
              WSK_LOGWARN1("[wsIOc] get data: ", (char *) data);

              break;

            case sIOtype_CONNECT:
              WSK_LOGWARN1("[wsIOc] connected: len = ", lData);
              WSK_LOGWARN1("[wsIOc] data: ", (char *) data);

              return;

            case sIOtype_DISCONNECT:
            case sIOtype_ACK:
            case sIOtype_ERROR:
            case sIOtype_BINARY_EVENT:
            case sIOtype_BINARY_ACK:
            default:
              WSK_LOGINFO1("[wsIOc] Socket.IO Message Type is not implemented:", ioType);
              WSK_LOGWARN1("[wsIOc] get text:", (char *) payload);

              break;
          }

          runIOCbEvent(ioType, data, lData);
        }

        break;

        case eIOtype_OPEN:
        case eIOtype_CLOSE:
        case eIOtype_UPGRADE:
        case eIOtype_NOOP:
        default:
          WSK_LOGINFO1("[wsIOc] Socket.IO Message Type is not implemented:", eType);
          WSK_LOGWARN1("[wsIOc] get text:", (char *) payload);

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

////////////////////////////////////////

#endif    // SOCKET_IO_CLIENT_GENERIC_IMPL_H_


