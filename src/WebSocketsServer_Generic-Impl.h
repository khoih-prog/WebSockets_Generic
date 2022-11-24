/****************************************************************************************************************************
  WebSocketsServer_Generic-Impl.h - WebSockets Library for boards

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

#ifndef WEBSOCKETS_SERVER_GENERIC_IMPL_H_
#define WEBSOCKETS_SERVER_GENERIC_IMPL_H_

WebSocketsServerCore::WebSocketsServerCore(const String & origin, const String & protocol)
{
  _origin                 = origin;
  _protocol               = protocol;
  _runnning               = false;
  _pingInterval           = 0;
  _pongTimeout            = 0;
  _disconnectTimeoutCount = 0;

  _cbEvent = NULL;

  _httpHeaderValidationFunc = NULL;
  _mandatoryHttpHeaders     = NULL;
  _mandatoryHttpHeaderCount = 0;
}

WebSocketsServer::WebSocketsServer(const uint16_t& port, const String & origin, const String & protocol)
  : WebSocketsServerCore(origin, protocol)
{
  _port   = port;

  _server = new WEBSOCKETS_NETWORK_SERVER_CLASS(port);

#if(WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)
  _server->onClient([](void * s, AsyncClient * c)
  {
    ((WebSocketsServerCore *)s)->newClient(new AsyncTCPbuffer(c));
  },
  this);
#endif
}

WebSocketsServerCore::~WebSocketsServerCore()
{
  // disconnect all clients
  close();

  if (_mandatoryHttpHeaders)
    delete[] _mandatoryHttpHeaders;

  _mandatoryHttpHeaderCount = 0;
}

WebSocketsServer::~WebSocketsServer()
{
}

/**
   called to initialize the Websocket server
*/
void WebSocketsServerCore::begin()
{
  // adjust clients storage:
  // _clients[i]'s constructor are already called,
  // all its members are initialized to their default value,
  // except the ones explicitly detailed in WSclient_t() constructor.
  // Then we need to initialize some members to non-trivial values:
  for (int i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++)
  {
    _clients[i].init(i, _pingInterval, _pongTimeout, _disconnectTimeoutCount);
  }

#ifdef ESP8266
  randomSeed(RANDOM_REG32);
#elif defined(ESP32)
#define DR_REG_RNG_BASE 0x3ff75144
  randomSeed(READ_PERI_REG(DR_REG_RNG_BASE));
#else
  // TODO find better seed
  randomSeed(millis());
#endif

  _runnning = true;

  WSK_LOGDEBUG(WEBSOCKETS_GENERIC_VERSION);
}

void WebSocketsServerCore::close()
{
  _runnning = false;
  disconnect();

  // restore _clients[] to their initial state
  // before next call to ::begin()
  for (int i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++)
  {
    _clients[i] = WSclient_t();
  }
}

/**
   set callback function
   @param cbEvent WebSocketServerEvent
*/
void WebSocketsServerCore::onEvent(WebSocketServerEvent cbEvent)
{
  _cbEvent = cbEvent;
}

/*
   Sets the custom http header validator function
   @param httpHeaderValidationFunc WebSocketServerHttpHeaderValFunc ///< pointer to the custom http header validation function
   @param mandatoryHttpHeaders[] const char* ///< the array of named http headers considered to be mandatory / must be present in order for websocket upgrade to succeed
   @param mandatoryHttpHeaderCount size_t ///< the number of items in the mandatoryHttpHeaders array
*/
void WebSocketsServerCore::onValidateHttpHeader(WebSocketServerHttpHeaderValFunc validationFunc,
                                                const char * mandatoryHttpHeaders[], size_t mandatoryHttpHeaderCount)
{
  _httpHeaderValidationFunc = validationFunc;

  if (_mandatoryHttpHeaders)
    delete[] _mandatoryHttpHeaders;

  _mandatoryHttpHeaderCount = mandatoryHttpHeaderCount;
  _mandatoryHttpHeaders     = new String[_mandatoryHttpHeaderCount];

  for (size_t i = 0; i < _mandatoryHttpHeaderCount; i++)
  {
    _mandatoryHttpHeaders[i] = mandatoryHttpHeaders[i];
  }
}

/*
   send text data to client
   @param num uint8_t client id
   @param payload uint8_t
   @param length size_t
   @param headerToPayload bool  (see sendFrame for more details)
   @return true if ok
*/
bool WebSocketsServerCore::sendTXT(const uint8_t& num, uint8_t * payload, size_t length, bool headerToPayload)
{
  if (num >= WEBSOCKETS_SERVER_CLIENT_MAX)
  {
    return false;
  }

  if (length == 0)
  {
    length = strlen((const char *)payload);
  }

  WSclient_t * client = &_clients[num];

  if (clientIsConnected(client))
  {
    return sendFrame(client, WSop_text, payload, length, true, headerToPayload);
  }

  return false;
}

bool WebSocketsServerCore::sendTXT(const uint8_t& num, const uint8_t * payload, size_t length)
{
  return sendTXT(num, (uint8_t *)payload, length);
}

bool WebSocketsServerCore::sendTXT(const uint8_t& num, char * payload, size_t length, bool headerToPayload)
{
  return sendTXT(num, (uint8_t *)payload, length, headerToPayload);
}

bool WebSocketsServerCore::sendTXT(const uint8_t& num, const char * payload, size_t length)
{
  return sendTXT(num, (uint8_t *)payload, length);
}

bool WebSocketsServerCore::sendTXT(const uint8_t& num, const String& payload)
{
  return sendTXT(num, (uint8_t *)payload.c_str(), payload.length());
}

/**
   send text data to client all
   @param payload uint8_t
   @param length size_t
   @param headerToPayload bool  (see sendFrame for more details)
   @return true if ok
*/
bool WebSocketsServerCore::broadcastTXT(uint8_t * payload, size_t length, bool headerToPayload)
{
  WSclient_t * client;
  bool ret = true;

  if (length == 0)
  {
    length = strlen((const char *)payload);
  }

  for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++)
  {
    client = &_clients[i];

    if (clientIsConnected(client))
    {
      if (!sendFrame(client, WSop_text, payload, length, true, headerToPayload))
      {
        ret = false;
      }
    }

    WEBSOCKETS_YIELD();
  }

  return ret;
}

bool WebSocketsServerCore::broadcastTXT(const uint8_t * payload, size_t length)
{
  return broadcastTXT((uint8_t *)payload, length);
}

bool WebSocketsServerCore::broadcastTXT(char * payload, size_t length, bool headerToPayload)
{
  return broadcastTXT((uint8_t *)payload, length, headerToPayload);
}

bool WebSocketsServerCore::broadcastTXT(const char * payload, size_t length)
{
  return broadcastTXT((uint8_t *)payload, length);
}

bool WebSocketsServerCore::broadcastTXT(const String& payload)
{
  return broadcastTXT((uint8_t *)payload.c_str(), payload.length());
}

/**
   send binary data to client
   @param num uint8_t client id
   @param payload uint8_t
   @param length size_t
   @param headerToPayload bool  (see sendFrame for more details)
   @return true if ok
*/
bool WebSocketsServerCore::sendBIN(const uint8_t& num, uint8_t * payload, size_t length, bool headerToPayload)
{
  if (num >= WEBSOCKETS_SERVER_CLIENT_MAX)
  {
    return false;
  }

  WSclient_t * client = &_clients[num];

  if (clientIsConnected(client))
  {
    return sendFrame(client, WSop_binary, payload, length, true, headerToPayload);
  }

  return false;
}

bool WebSocketsServerCore::sendBIN(const uint8_t& num, const uint8_t * payload, size_t length)
{
  return sendBIN(num, (uint8_t *)payload, length);
}

/**
   send binary data to client all
   @param payload uint8_t
   @param length size_t
   @param headerToPayload bool  (see sendFrame for more details)
   @return true if ok
*/
bool WebSocketsServerCore::broadcastBIN(uint8_t * payload, size_t length, bool headerToPayload)
{
  WSclient_t * client;
  bool ret = true;

  for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++)
  {
    client = &_clients[i];

    if (clientIsConnected(client))
    {
      if (!sendFrame(client, WSop_binary, payload, length, true, headerToPayload))
      {
        ret = false;
      }
    }

    WEBSOCKETS_YIELD();
  }

  return ret;
}

bool WebSocketsServerCore::broadcastBIN(const uint8_t * payload, size_t length)
{
  return broadcastBIN((uint8_t *)payload, length);
}

/**
   sends a WS ping to Client
   @param num uint8_t client id
   @param payload uint8_t
   @param length size_t
   @return true if ping is send out
*/
bool WebSocketsServerCore::sendPing(const uint8_t& num, uint8_t * payload, size_t length)
{
  if (num >= WEBSOCKETS_SERVER_CLIENT_MAX)
  {
    return false;
  }

  WSclient_t * client = &_clients[num];

  if (clientIsConnected(client))
  {
    return sendFrame(client, WSop_ping, payload, length);
  }

  return false;
}

bool WebSocketsServerCore::sendPing(const uint8_t& num, const String& payload)
{
  return sendPing(num, (uint8_t *)payload.c_str(), payload.length());
}

/**
    sends a WS ping to all Client
   @param payload uint8_t
   @param length size_t
   @return true if ping is send out
*/
bool WebSocketsServerCore::broadcastPing(uint8_t * payload, size_t length)
{
  WSclient_t * client;
  bool ret = true;

  for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++)
  {
    client = &_clients[i];

    if (clientIsConnected(client))
    {
      if (!sendFrame(client, WSop_ping, payload, length))
      {
        ret = false;
      }
    }

    WEBSOCKETS_YIELD();
  }

  return ret;
}

bool WebSocketsServerCore::broadcastPing(const String& payload)
{
  return broadcastPing((uint8_t *)payload.c_str(), payload.length());
}

/**
   disconnect all clients
*/
void WebSocketsServerCore::disconnect()
{
  WSclient_t * client;

  for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++)
  {
    client = &_clients[i];

    if (clientIsConnected(client))
    {
      WebSockets::clientDisconnect(client, 1000);
    }
  }
}

/**
   disconnect one client
   @param num uint8_t client id
*/
void WebSocketsServerCore::disconnect(const uint8_t& num)
{
  if (num >= WEBSOCKETS_SERVER_CLIENT_MAX)
  {
    return;
  }

  WSclient_t * client = &_clients[num];

  if (clientIsConnected(client))
  {
    WebSockets::clientDisconnect(client, 1000);
  }
}

/*
   set the Authorization for the http request
   @param user const char
   @param password const char
*/
void WebSocketsServerCore::setAuthorization(const char * user, const char * password)
{
  if (user && password)
  {
    String auth = user;
    auth += ":";
    auth += password;
    _base64Authorization = base64_encode((uint8_t *)auth.c_str(), auth.length());
  }
}

/**
   set the Authorizatio for the http request
   @param auth const char * base64
*/
void WebSocketsServerCore::setAuthorization(const char * auth)
{
  if (auth)
  {
    _base64Authorization = auth;
  }
}

/**
   count the connected clients (optional ping them)
   @param ping bool ping the connected clients
*/
int WebSocketsServerCore::connectedClients(bool ping)
{
  WSclient_t * client;
  int count = 0;

  for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++)
  {
    client = &_clients[i];

    if (client->status == WSC_CONNECTED)
    {
      if (ping != true || sendPing(i))
      {
        count++;
      }
    }
  }

  return count;
}

/**
   see if one client is connected
   @param num uint8_t client id
*/
bool WebSocketsServerCore::clientIsConnected(const uint8_t& num)
{
  if (num >= WEBSOCKETS_SERVER_CLIENT_MAX)
  {
    return false;
  }

  WSclient_t * client = &_clients[num];

  return clientIsConnected(client);
}

#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC) || \
    (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)
/**
   get an IP for a client
   @param num uint8_t client id
   @return IPAddress
*/
IPAddress WebSocketsServerCore::remoteIP(const uint8_t& num)
{
  if (num < WEBSOCKETS_SERVER_CLIENT_MAX)
  {
    WSclient_t * client = &_clients[num];

    if (clientIsConnected(client))
    {
      return client->tcp->remoteIP();
    }
  }

  return IPAddress();
}
#endif

//#################################################################################
//#################################################################################
//#################################################################################

/**
   handle new client connection
   @param client
*/
WSclient_t * WebSocketsServerCore::newClient(WEBSOCKETS_NETWORK_CLASS * TCPclient)
{
  WSclient_t * client;

  // search free list entry for client
  for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++)
  {
    client = &_clients[i];

    // KH Debug
    //displayClientData(client, false);
    //displayClientData(client);

    // state is not connected or tcp connection is lost
    if (!clientIsConnected(client))
    {
      client->tcp = TCPclient;

#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32) || \
    (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)
      client->isSSL = false;
      client->tcp->setNoDelay(true);
#endif

#if (WEBSOCKETS_NETWORK_TYPE != NETWORK_ESP8266_ASYNC)
      // set Timeout for readBytesUntil and readStringUntil
      client->tcp->setTimeout(WEBSOCKETS_TCP_TIMEOUT);
#endif

      client->status = WSC_HEADER;

      // KH Debug
      //client->status = WSC_NOT_CONNECTED;

#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC) \
     || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)
#ifndef NODEBUG_WEBSOCKETS
      IPAddress ip = client->tcp->remoteIP();

      // KH New debug
      WSK_LOGDEBUG3("ESP New Client :", client->num, ", IP =", ip);
#endif
#else
      // KH New debug
      WSK_LOGDEBUG1("New Client :", client->num);
#endif

#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)

      client->tcp->onDisconnect(std::bind([](WebSocketsServerCore * server, AsyncTCPbuffer * obj,
                                             WSclient_t * client) -> bool
      {
        WSK_LOGDEBUG1("Disconnect Client :", client->num);

        AsyncTCPbuffer ** sl = &server->_clients[client->num].tcp;

        if (*sl == obj)
        {
          client->status = WSC_NOT_CONNECTED;
          *sl            = NULL;
        }

        return true;
      },
      this, std::placeholders::_1, client));

      client->tcp->readStringUntil('\n', &(client->cHttpLine), std::bind(&WebSocketsServerCore::handleHeader,
                                                                         this, client, &(client->cHttpLine)));

#endif

      client->pingInterval           = _pingInterval;
      client->pongTimeout            = _pongTimeout;
      client->disconnectTimeoutCount = _disconnectTimeoutCount;
      client->lastPing               = millis();
      client->pongReceived           = false;

      return client;
      break;
    }
  }

  return nullptr;
}

/**

   @param client WSclient_t *  ptr to the client struct
   @param opcode WSopcode_t
   @param payload  uint8_t
   @param length size_t
*/
void WebSocketsServerCore::messageReceived(WSclient_t * client, WSopcode_t opcode, uint8_t * payload, size_t length,
                                           bool fin)
{
  WStype_t type = WStype_ERROR;

  switch (opcode)
  {
    case WSop_text:
      type = fin ? WStype_TEXT : WStype_FRAGMENT_TEXT_START;
      break;

    case WSop_binary:
      type = fin ? WStype_BIN : WStype_FRAGMENT_BIN_START;
      break;

    case WSop_continuation:
      type = fin ? WStype_FRAGMENT_FIN : WStype_FRAGMENT;
      break;

    case WSop_ping:
      type = WStype_PING;
      break;

    case WSop_pong:
      type = WStype_PONG;
      break;

    case WSop_close:
    default:
      break;
  }

  runCbEvent(client->num, type, payload, length);
}

/**
   Discard a native client
   @param client WSclient_t *  ptr to the client struct contaning the native client "->tcp"
*/
void WebSocketsServerCore::dropNativeClient(WSclient_t * client)
{
  if (!client)
  {
    return;
  }

  if (client->tcp)
  {
    if (client->tcp->connected())
    {
#if (WEBSOCKETS_NETWORK_TYPE != NETWORK_ESP8266_ASYNC) && (WEBSOCKETS_NETWORK_TYPE != NETWORK_ESP32)
      client->tcp->flush();
#endif
      client->tcp->stop();
    }

#if(WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)
    client->status = WSC_NOT_CONNECTED;
#else
    delete client->tcp;
#endif
    client->tcp = NULL;
  }
}

/**
   Disconnect an client
   @param client WSclient_t *  ptr to the client struct
*/
void WebSocketsServerCore::clientDisconnect(WSclient_t * client)
{
#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32) || \
    (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)

  if (client->isSSL && client->ssl)
  {
    if (client->ssl->connected())
    {
      client->ssl->flush();
      client->ssl->stop();
    }

    delete client->ssl;
    client->ssl = NULL;
    client->tcp = NULL;
  }

#endif

  dropNativeClient(client);

  client->cUrl         = "";
  client->cKey         = "";
  client->cProtocol    = "";
  client->cVersion     = 0;
  client->cIsUpgrade   = false;
  client->cIsWebsocket = false;

  client->cWsRXsize = 0;

#if(WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)
  client->cHttpLine = "";
#endif

  client->status = WSC_NOT_CONNECTED;

  // KH New debug
  WSK_LOGDEBUG1("Disconnected Client :", client->num);
  //WSK_LOGINFO1("Disconnected Client :", client->num);

  runCbEvent(client->num, WStype_DISCONNECTED, NULL, 0);
}

/**
   get client state
   @param client WSclient_t *  ptr to the client struct
   @return true = connected
*/
bool WebSocketsServerCore::clientIsConnected(WSclient_t * client)
{
  if (!client->tcp)
  {
    return false;
  }

  if (client->tcp->connected())
  {
    if (client->status != WSC_NOT_CONNECTED)
    {
      return true;
    }
  }
  else
  {
    // client lost
    if (client->status != WSC_NOT_CONNECTED)
    {
      WSK_LOGDEBUG1("Clean up : Connection lost. Client :", client->num );
      // do cleanup
      clientDisconnect(client);
    }
  }

  if (client->tcp)
  {
    // do cleanup
    WSK_LOGDEBUG1("[WS-Server] client->tcp, client list cleanup 2. Client :", client->num);
    clientDisconnect(client);
  }

  return false;
}

#if (WEBSOCKETS_NETWORK_TYPE != NETWORK_ESP8266_ASYNC)
/**
   Handle incoming Connection Request
*/
WSclient_t * WebSocketsServerCore::handleNewClient(WEBSOCKETS_NETWORK_CLASS * tcpClient)
{
  WSclient_t * client = newClient(tcpClient);

  if (!client)
  {
    // no free space to handle client
#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32) || \
    (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)
#ifndef NODEBUG_WEBSOCKETS
    IPAddress ip = tcpClient->remoteIP();

    WSK_LOGERROR1("[WS-Server][handleNewClient] No free space for new client from", ip);
#endif
#else
    WSK_LOGERROR("[WS-Server][handleNewClient] No free space new client");
#endif

    // no client! => create dummy!
    WSclient_t dummy = WSclient_t();
    client = & dummy;
    client->tcp = tcpClient;

    dropNativeClient(client);
  }

  WEBSOCKETS_YIELD();

  return client;
}

/**
   Handle incoming Connection Request
*/
void WebSocketsServer::handleNewClients()
{
#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32) || \
    (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)

  while (_server->hasClient())
  {
#endif

    // store new connection
    WEBSOCKETS_NETWORK_CLASS * tcpClient = new WEBSOCKETS_NETWORK_CLASS(_server->available());

    if (!tcpClient)
    {
      WSK_LOGERROR("[WS-Server][handleNewClients] Creating Network class failed!");

      return;
    }

    handleNewClient(tcpClient);

#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32) || \
    (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)
  }

#endif
}

/**
   Handel incomming data from Client
*/
void WebSocketsServerCore::handleClientData()
{
  WSclient_t * client;

  //static uint8_t currentActiveClient = 0xFF;

  for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++)
  {
    client = &_clients[i];

    // KH New debug
    //displayClientData(client);

    if (clientIsConnected(client))
      // KH Debug
      //if ( clientIsConnected(client) && client->cHttpHeadersValid )
    {
      int len = client->tcp->available();

      if (len > 0)
      {
        // KH New
        WSK_LOGINFO3("[handleClientData] Client:", client->num, ", tcp->available len:", len);

        switch (client->status)
        {
          case WSC_HEADER:
          {
            // KH New
            WSK_LOGINFO1(client->num, "[handleClientData] =================== Start =======================");

            String headerLine = client->tcp->readStringUntil('\n');

            // KH New
            WSK_LOGINFO3("[handleClientData] Status WSC_HEADER. Client:", client->num, ", headerLine:", headerLine);
            // KH Debug
            //if ( client->cHttpHeadersValid )
            {
              handleHeader(client, headerLine);
            }

            // KH New
            currentActiveClient = client->num;

            // KH New
            WSK_LOGINFO1(client->num, "[handleClientData] =================== End =======================");
          }

          break;

          case WSC_CONNECTED:
            // KH New
            WSK_LOGINFO1("[handleClientData] Status WSC_CONNECTED. handleWebsocket. Client:", client->num);

            WebSockets::handleWebsocket(client);

            break;

          default:
            // KH New
            WSK_LOGINFO3("[handleClientData] default: clientDisconnect. Client:", client->num,
                         "unknown client status", client->status);
            WebSockets::clientDisconnect(client, 1002);

            // KH New
            currentActiveClient = 0xFF;

            break;
        }
      }

      handleHBPing(client);
      handleHBTimeout(client);
    }

    WEBSOCKETS_YIELD();
  }
}
#endif    // #if (WEBSOCKETS_NETWORK_TYPE != NETWORK_ESP8266_ASYNC)

/*
   returns an indicator whether the given named header exists in the configured _mandatoryHttpHeaders collection
   @param headerName String ///< the name of the header being checked
*/
bool WebSocketsServerCore::hasMandatoryHeader(const String& headerName)
{
  for (size_t i = 0; i < _mandatoryHttpHeaderCount; i++)
  {
    if (_mandatoryHttpHeaders[i].equalsIgnoreCase(headerName))
      return true;
  }

  return false;
}

/**
   handles http header reading for WebSocket upgrade
   @param client WSclient_t * ///< pointer to the client struct
   @param headerLine String ///< the header being read / processed
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////

void WebSocketsServerCore::handleHeader(WSclient_t * client, String& headerLine)
{
  static const char * NEW_LINE = "\r\n";

  //WSK_LOGINFO3("[handleHeader] Client:", client->num, ", RX before trim:", headerLine.c_str());

  headerLine.trim();    // remove \r

  //WSK_LOGINFO3("[handleHeader] Client:", client->num, ", RX after trim:", headerLine.c_str());

  if (headerLine.length() > 0)
  {
    WSK_LOGINFO3("[handleHeader] Client:", client->num, ", RX:", headerLine.c_str());

    //KH New
    WSK_LOGINFO1("[handleHeader] RX: HeaderLine Len =", headerLine.length());

    // websocket requests always start with GET see rfc6455
    if (headerLine.startsWith("GET "))
    {
      // cut URL out
      client->cUrl = headerLine.substring(4, headerLine.indexOf(' ', 4));

      //KH New
      WSK_LOGINFO1("[handleHeader] RX: client->cUrl =", client->cUrl);

      //reset non-websocket http header validation state for this client
      client->cHttpHeadersValid      = true;
      client->cMandatoryHeadersCount = 0;

    }
    else if (headerLine.indexOf(':') >= 0)
    {
      String headerName  = headerLine.substring(0, headerLine.indexOf(':'));
      String headerValue = headerLine.substring(headerLine.indexOf(':') + 1);

      // remove space in the beginning (RFC2616)
      if (headerValue[0] == ' ')
      {
        headerValue.remove(0, 1);
      }

      if (headerName.equalsIgnoreCase(WEBSOCKETS_STRING("Connection")))
      {
        headerValue.toLowerCase();

        if (headerValue.indexOf(WEBSOCKETS_STRING("upgrade")) >= 0)
        {
          client->cIsUpgrade = true;
        }
      }
      else if (headerName.equalsIgnoreCase(WEBSOCKETS_STRING("Upgrade")))
      {
        if (headerValue.equalsIgnoreCase(WEBSOCKETS_STRING("websocket")))
        {
          client->cIsWebsocket = true;
        }
      }
      else if (headerName.equalsIgnoreCase(WEBSOCKETS_STRING("Sec-WebSocket-Version")))
      {
        client->cVersion = headerValue.toInt();
      }
      else if (headerName.equalsIgnoreCase(WEBSOCKETS_STRING("Sec-WebSocket-Key")))
      {
        client->cKey = headerValue;
        client->cKey.trim();    // see rfc6455
      }
      else if (headerName.equalsIgnoreCase(WEBSOCKETS_STRING("Sec-WebSocket-Protocol")))
      {
        client->cProtocol = headerValue;
      }
      else if (headerName.equalsIgnoreCase(WEBSOCKETS_STRING("Sec-WebSocket-Extensions")))
      {
        client->cExtensions = headerValue;
      }
      else if (headerName.equalsIgnoreCase(WEBSOCKETS_STRING("Authorization")))
      {
        client->base64Authorization = headerValue;
      }
      else
      {
        client->cHttpHeadersValid &= execHttpHeaderValidation(headerName, headerValue);

        if (_mandatoryHttpHeaderCount > 0 && hasMandatoryHeader(headerName))
        {
          client->cMandatoryHeadersCount++;
        }
      }
    }
    else
    {
      WSK_LOGINFO1("[handleHeader] Header error. RX:", headerLine.c_str());
    }

    headerLine = "";

#if(WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)
    client->tcp->readStringUntil('\n', &(client->cHttpLine), std::bind(&WebSocketsServerCore::handleHeader,
                                                                       this, client, &(client->cHttpLine)));
#endif
  }
  else
  {
    WSK_LOGINFO1(client->num, "Header read fin.");
    WSK_LOGINFO2(client->num, "   - cURL:",                    client->cUrl.c_str());
    WSK_LOGINFO2(client->num, "   - cIsUpgrade:",              client->cIsUpgrade);
    WSK_LOGINFO2(client->num, "   - cIsWebsocket:",            client->cIsWebsocket);
    WSK_LOGINFO2(client->num, "   - cKey:",                    client->cKey.c_str());
    WSK_LOGINFO2(client->num, "   - cProtocol:",               client->cProtocol.c_str());
    WSK_LOGINFO2(client->num, "   - cExtensions:",             client->cExtensions.c_str());
    WSK_LOGINFO2(client->num, "   - cVersion:",                client->cVersion);
    WSK_LOGINFO2(client->num, "   - base64Authorization:",     client->base64Authorization.c_str());
    WSK_LOGINFO2(client->num, "   - cHttpHeadersValid:",       client->cHttpHeadersValid);
    WSK_LOGINFO2(client->num, "   - cMandatoryHeadersCount:",  client->cMandatoryHeadersCount);

    bool ok = (client->cIsUpgrade && client->cIsWebsocket);

    if (ok)
    {
      if (client->cUrl.length() == 0)
      {
        ok = false;
      }

      if (client->cKey.length() == 0)
      {
        ok = false;
      }

      if (client->cVersion != 13)
      {
        ok = false;
      }

      if (!client->cHttpHeadersValid)
      {
        ok = false;
      }

      if (client->cMandatoryHeadersCount != _mandatoryHttpHeaderCount)
      {
        ok = false;
      }
    }

    if (_base64Authorization.length() > 0)
    {
      String auth = WEBSOCKETS_STRING("Basic ");
      auth += _base64Authorization;

      if (auth != client->base64Authorization)
      {
        WSK_LOGDEBUG1("[handleHeader] HTTP Authorization failed! Client:", client->num);

        handleAuthorizationFailed(client);
        return;
      }
    }

    if (ok)
    {
      WSK_LOGDEBUG1("[handleHeader] Websocket connection incoming. Client:", client->num);

      // generate Sec-WebSocket-Accept key
      String sKey = acceptKey(client->cKey);

      WSK_LOGDEBUG2(client->num, "[handleHeader]  - sKey:", sKey.c_str());

      client->status = WSC_CONNECTED;

      String handshake = WEBSOCKETS_STRING(
                           "HTTP/1.1 101 Switching Protocols\r\n"
                           "Server: arduino-WebSocketsServer\r\n"
                           "Upgrade: websocket\r\n"
                           "Connection: Upgrade\r\n"
                           "Sec-WebSocket-Version: 13\r\n"
                           "Sec-WebSocket-Accept: ");

      handshake += sKey + NEW_LINE;

      if (_origin.length() > 0)
      {
        handshake += WEBSOCKETS_STRING("Access-Control-Allow-Origin: ");
        handshake += _origin + NEW_LINE;
      }

      if (client->cProtocol.length() > 0)
      {
        handshake += WEBSOCKETS_STRING("Sec-WebSocket-Protocol: ");
        handshake += _protocol + NEW_LINE;
      }

      // header end
      handshake += NEW_LINE;

      WSK_LOGDEBUG3("[handleHeader] Client:", client->num, ", handshake:", (char *)handshake.c_str());

      write(client, (uint8_t *)handshake.c_str(), handshake.length());

      headerDone(client);

      // send ping
      WebSockets::sendFrame(client, WSop_ping);

      runCbEvent(client->num, WStype_CONNECTED, (uint8_t *)client->cUrl.c_str(), client->cUrl.length());

    }
    else
    {
      handleNonWebsocketConnection(client);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

/**
   send heartbeat ping to server in set intervals
*/
void WebSocketsServerCore::handleHBPing(WSclient_t * client)
{
  if (client->pingInterval == 0)
    return;

  uint32_t pi = millis() - client->lastPing;

  if (pi > client->pingInterval)
  {
    WSK_LOGDEBUG1("[handleHeader] Sending HB ping to Client:", client->num);

    if (sendPing(client->num))
    {
      client->lastPing     = millis();
      client->pongReceived = false;
    }
  }
}

/**
   enable ping/pong heartbeat process
   @param pingInterval uint32_t how often ping will be sent
   @param pongTimeout uint32_t millis after which pong should timout if not received
   @param disconnectTimeoutCount uint8_t how many timeouts before disconnect, 0=> do not disconnect
*/
void WebSocketsServerCore::enableHeartbeat(const uint32_t& pingInterval, const uint32_t& pongTimeout,
                                           const uint8_t& disconnectTimeoutCount)
{
  _pingInterval           = pingInterval;
  _pongTimeout            = pongTimeout;
  _disconnectTimeoutCount = disconnectTimeoutCount;

  WSclient_t * client;

  for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++)
  {
    client = &_clients[i];
    WebSockets::enableHeartbeat(client, pingInterval, pongTimeout, disconnectTimeoutCount);
  }
}

/**
   disable ping/pong heartbeat process
*/
void WebSocketsServerCore::disableHeartbeat()
{
  _pingInterval = 0;

  WSclient_t * client;

  for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++)
  {
    client               = &_clients[i];
    client->pingInterval = 0;
  }
}


////////////////////
// WebSocketServer

/**
   called to initialize the Websocket server
*/
void WebSocketsServer::begin()
{
  WebSocketsServerCore::begin();
  _server->begin();

  WSK_LOGDEBUG("[WS-Server] Server Started.");
}

void WebSocketsServer::close()
{
  WebSocketsServerCore::close();

#if(WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266)
  _server->close();
#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC) || \
      (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)
  _server->end();
#else
  // TODO how to close server?
#endif
}

#if (WEBSOCKETS_NETWORK_TYPE != NETWORK_ESP8266_ASYNC)
/**
   called in arduino loop
*/
void WebSocketsServerCore::loop()
{
  if (_runnning)
  {
    WEBSOCKETS_YIELD();
    handleClientData();
  }
}

/**
   called in arduino loop
*/
void WebSocketsServer::loop()
{
  if (_runnning)
  {
    WEBSOCKETS_YIELD();
    handleNewClients();
    WebSocketsServerCore::loop();
  }
}
#endif

#endif    // WEBSOCKETS_SERVER_GENERIC_IMPL_H_

