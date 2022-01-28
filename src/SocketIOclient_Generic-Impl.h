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

#ifndef SOCKET_IO_CLIENT_GENERIC_IMPL_H_
#define SOCKET_IO_CLIENT_GENERIC_IMPL_H_

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
  initClient();
}

void SocketIOclient::begin(String host, uint16_t port, String url, String protocol)
{
  WebSocketsClient::beginSocketIO(host, port, url, protocol);
  WebSocketsClient::enableHeartbeat(60 * 1000, 90 * 1000, 5);
  initClient();
}

void SocketIOclient::begin(IPAddress host, uint16_t port, String url, String protocol)
{
  WebSocketsClient::beginSocketIO(host, port, url, protocol);
  WebSocketsClient::enableHeartbeat(60 * 1000, 90 * 1000, 5);
  initClient();
}

#if defined(HAS_SSL)
void SocketIOclient::beginSSL(const char * host, uint16_t port, const char * url, const char * protocol) 
{
  WebSocketsClient::beginSocketIOSSL(host, port, url, protocol);
  WebSocketsClient::enableHeartbeat(60 * 1000, 90 * 1000, 5);
  initClient();
}

void SocketIOclient::beginSSL(String host, uint16_t port, String url, String protocol) 
{
  WebSocketsClient::beginSocketIOSSL(host, port, url, protocol);
  WebSocketsClient::enableHeartbeat(60 * 1000, 90 * 1000, 5);
  initClient();
}

#if defined(SSL_BARESSL)
void SocketIOclient::beginSSLWithCA(const char * host, uint16_t port, const char * url, const char * CA_cert, const char * protocol) 
{
  WebSocketsClient::beginSocketIOSSLWithCA(host, port, url, CA_cert, protocol);
  WebSocketsClient::enableHeartbeat(60 * 1000, 90 * 1000, 5);
  initClient();
}

void SocketIOclient::beginSSLWithCA(const char * host, uint16_t port, const char * url, BearSSL::X509List * CA_cert, const char * protocol) 
{
  WebSocketsClient::beginSocketIOSSLWithCA(host, port, url, CA_cert, protocol);
  WebSocketsClient::enableHeartbeat(60 * 1000, 90 * 1000, 5);
  initClient();
}

void SocketIOclient::setSSLClientCertKey(const char * clientCert, const char * clientPrivateKey) 
{
  WebSocketsClient::setSSLClientCertKey(clientCert, clientPrivateKey);
}

void SocketIOclient::setSSLClientCertKey(BearSSL::X509List * clientCert, BearSSL::PrivateKey * clientPrivateKey) 
{
  WebSocketsClient::setSSLClientCertKey(clientCert, clientPrivateKey);
}

#endif
#endif

void SocketIOclient::configureEIOping(bool disableHeartbeat) 
{
  _disableHeartbeat = disableHeartbeat;
}

void SocketIOclient::initClient() 
{
  if(_client.cUrl.indexOf("EIO=4") != -1) 
  {
    WSK_LOGINFO("[wsIOc] found EIO=4 disable EIO ping on client");
    configureEIOping(true);
  }
}

/**
   set callback function
   @param cbEvent SocketIOclientEvent
*/
void SocketIOclient::onEvent(SocketIOclientEvent cbEvent)
{
  _cbEvent = cbEvent;
}

bool SocketIOclient::isConnected()
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

void SocketIOclient::loop()
{
  WebSocketsClient::loop();
  unsigned long t = millis();
  
  if(!_disableHeartbeat && (t - _lastHeartbeat) > EIO_HEARTBEAT_INTERVAL) 
  {
    _lastHeartbeat = t;
    WSK_LOGINFO("[wsIOc] send ping\n");
    WebSocketsClient::sendTXT(eIOtype_PING);
  }
}

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
            break;
          case eIOtype_PONG:
            WSK_LOGWARN("[wsIOc] get pong");

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

#endif    // SOCKET_IO_CLIENT_GENERIC_IMPL_H_


