/****************************************************************************************************************************
  WebSocketsClient_Generic-Impl.h - WebSockets Library for boards
  
  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.
  
  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license
   
  @original file WebSocketsClient.cpp
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

#ifndef WEBSOCKETS_CLIENT_GENERIC_IMPL_H_
#define WEBSOCKETS_CLIENT_GENERIC_IMPL_H_

#include "WebSockets_Generic.h"

////////////////////////////////////////

WebSocketsClient::WebSocketsClient()
{
  _cbEvent             = NULL;
  _client.num          = 0;
  _client.cIsClient    = true;
  _client.extraHeaders = WEBSOCKETS_STRING("Origin: file://");
  _reconnectInterval   = 500;

  _port                = 0;
  _host                = "";
}

////////////////////////////////////////

WebSocketsClient::~WebSocketsClient()
{
  disconnect();
}

////////////////////////////////////////

/**
   calles to init the Websockets server
*/
void WebSocketsClient::begin(const char * host, const uint16_t& port, const char * url, const char * protocol)
{
  _host = host;
  _port = port;

#if defined(HAS_SSL)

  _fingerprint = SSL_FINGERPRINT_NULL;

  _CA_cert     = NULL;

#endif

  _client.num    = 0;
  _client.status = WSC_NOT_CONNECTED;
  _client.tcp    = NULL;

#if defined(HAS_SSL)
  _client.isSSL = false;
  _client.ssl   = NULL;
#endif

  _client.cUrl                = url;
  _client.cCode               = 0;
  _client.cIsUpgrade          = false;
  _client.cIsWebsocket        = true;
  _client.cKey                = "";
  _client.cAccept             = "";
  _client.cProtocol           = protocol;
  _client.cExtensions         = "";
  _client.cVersion            = 0;
  _client.base64Authorization = "";
  _client.plainAuthorization  = "";
  _client.isSocketIO          = false;

  _client.lastPing         = 0;
  _client.pongReceived     = false;
  _client.pongTimeoutCount = 0;

#ifdef ESP8266
  randomSeed(RANDOM_REG32);
#else
  // todo find better seed
  randomSeed(millis());
#endif

#if ( (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32_ASYNC) )
  asyncConnect();
#endif

  _lastConnectionFail = 0;
  _lastHeaderSent     = 0;
  
  WSK_LOGINFO(WEBSOCKETS_GENERIC_VERSION);
}

////////////////////////////////////////

void WebSocketsClient::begin(const String& host, const uint16_t& port, const String& url, const String& protocol)
{
  begin(host.c_str(), port, url.c_str(), protocol.c_str());
}

////////////////////////////////////////

void WebSocketsClient::begin(const IPAddress& host, const uint16_t& port, const char * url, const char * protocol)
{
#if defined(ESP8266) || defined(ESP32) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN) || \
                                          (WEBSOCKETS_NETWORK_TYPE == NETWORK_RP2040W_WIFI)
  return begin(host.toString().c_str(), port, url, protocol);
#else  
  return begin(WS_IPAddressToString(host).c_str(), port, url, protocol);
#endif  
}

////////////////////////////////////////

//KH
void WebSocketsClient::begin(const IPAddress& host, const uint16_t& port, const String& url, const String& protocol)
{
  return begin(WS_IPAddressToString(host).c_str(), port, url.c_str(), protocol.c_str());
}

////////////////////////////////////////

#if defined(HAS_SSL)

#if defined(SSL_AXTLS)

void WebSocketsClient::beginSSL(const char * host, const uint16_t& port, const char * url, const char * fingerprint, 
                                const char * protocol)
{
  begin(host, port, url, protocol);
  _client.isSSL = true;
  _fingerprint  = fingerprint;
  _CA_cert      = NULL;
}

////////////////////////////////////////

void WebSocketsClient::beginSSL(const String& host, const uint16_t& port, const String& url, const String& fingerprint, 
                                const String& protocol)
{
  beginSSL(host.c_str(), port, url.c_str(), fingerprint.c_str(), protocol.c_str());
}

////////////////////////////////////////

// KH
void WebSocketsClient::beginSSL(const IPAddress& host, const uint16_t& port, const String& url, const String& fingerprint, 
                                const String& protocol)
{
  beginSSL(WS_IPAddressToString(host).c_str(), port, url.c_str(), fingerprint.c_str(), protocol.c_str());
}

////////////////////////////////////////

void WebSocketsClient::beginSslWithCA(const char * host, const uint16_t& port, const char * url, const char * CA_cert, 
                                      const char * protocol)
{
  begin(host, port, url, protocol);
  _client.isSSL = true;

  _fingerprint  = SSL_FINGERPRINT_NULL;


  _CA_cert      = CA_cert;
}

////////////////////////////////////////

#else   // SSL_AXTLS

////////////////////////////////////////

void WebSocketsClient::beginSSL(const char * host, const uint16_t& port, const char * url, const uint8_t * fingerprint, 
                                const char * protocol)
{
  begin(host, port, url, protocol);
  _client.isSSL = true;
  _fingerprint  = fingerprint;
  _CA_cert      = NULL;
}

////////////////////////////////////////

// KH
void WebSocketsClient::beginSSL(const IPAddress& host, const uint16_t& port, const String& url, const String& fingerprint, 
                                const String& protocol)
{
  beginSSL(WS_IPAddressToString(host).c_str(), port, url.c_str(), (uint8_t *) fingerprint.c_str(), protocol.c_str());
}

////////////////////////////////////////

void WebSocketsClient::beginSslWithCA(const char * host, const uint16_t& port, const char * url, BearSSL::X509List * CA_cert, 
                                      const char * protocol)
{
  begin(host, port, url, protocol);
  _client.isSSL = true;
  _fingerprint  = SSL_FINGERPRINT_NULL;
  _CA_cert      = CA_cert;
}

////////////////////////////////////////

void WebSocketsClient::beginSslWithCA(const char * host, const uint16_t& port, const char * url, const char * CA_cert, 
                                      const char * protocol)
{
  begin(host, port, url, protocol);
  _client.isSSL = true;
  _fingerprint  = SSL_FINGERPRINT_NULL;
  _CA_cert      = new BearSSL::X509List(CA_cert);
}

////////////////////////////////////////

void WebSocketsClient::setSSLClientCertKey(BearSSL::X509List * clientCert, BearSSL::PrivateKey * clientPrivateKey) 
{
    _client_cert = clientCert;
    _client_key  = clientPrivateKey;
}

void WebSocketsClient::setSSLClientCertKey(const char * clientCert, const char * clientPrivateKey) 
{
    setSSLClientCertKey(new BearSSL::X509List(clientCert), new BearSSL::PrivateKey(clientPrivateKey));
}

////////////////////////////////////////

#endif    // SSL_AXTLS

#endif    // HAS_SSL

////////////////////////////////////////
////////////////////////////////////////

void WebSocketsClient::beginSocketIO(const char * host, const uint16_t& port, const char * url, const char * protocol)
{
  WSK_LOGDEBUG("[WS-Client] beginSocketIO with const char");
  
  begin(host, port, url, protocol);
  _client.isSocketIO = true;
}

////////////////////////////////////////

void WebSocketsClient::beginSocketIO(const String& host, const uint16_t& port, const String& url, const String& protocol)
{
  WSK_LOGDEBUG("[WS-Client] beginSocketIO with String");
  
  beginSocketIO(host.c_str(), port, url.c_str(), protocol.c_str());
}

////////////////////////////////////////

// KH
void WebSocketsClient::beginSocketIO(const IPAddress& host, const uint16_t& port, const String& url, const String& protocol)
{
  WSK_LOGDEBUG("[WS-Client] beginSocketIO with IPAddress");

  beginSocketIO(WS_IPAddressToString(host).c_str(), port, url.c_str(), protocol.c_str());
}

////////////////////////////////////////

#if defined(HAS_SSL)
void WebSocketsClient::beginSocketIOSSL(const char * host, const uint16_t& port, const char * url, const char * protocol)
{
  begin(host, port, url, protocol);
  _client.isSocketIO = true;
  _client.isSSL      = true;

  _fingerprint       = SSL_FINGERPRINT_NULL;
}

////////////////////////////////////////

void WebSocketsClient::beginSocketIOSSL(const String& host, const uint16_t& port, const String& url, const String& protocol)
{
  beginSocketIOSSL(host.c_str(), port, url.c_str(), protocol.c_str());
}

////////////////////////////////////////

// KH
void WebSocketsClient::beginSocketIOSSL(const IPAddress& host, const uint16_t& port, const String& url, const String& protocol)
{
  beginSocketIOSSL(WS_IPAddressToString(host).c_str(), port, url.c_str(), protocol.c_str());
}

////////////////////////////////////////

#if defined(SSL_BARESSL)
void WebSocketsClient::beginSocketIOSSLWithCA(const char * host, const uint16_t& port, const char * url, 
                                              BearSSL::X509List * CA_cert, const char * protocol) 
{
    begin(host, port, url, protocol);
    _client.isSocketIO = true;
    _client.isSSL      = true;
    _fingerprint       = SSL_FINGERPRINT_NULL;
    _CA_cert           = CA_cert;
}
#endif

////////////////////////////////////////

void WebSocketsClient::beginSocketIOSSLWithCA(const char * host, const uint16_t& port, const char * url, const char * CA_cert, 
                                              const char * protocol) 
{
    begin(host, port, url, protocol);
    _client.isSocketIO = true;
    _client.isSSL      = true;
    _fingerprint       = SSL_FINGERPRINT_NULL;
#if defined(SSL_BARESSL)
    _CA_cert = new BearSSL::X509List(CA_cert);
#else
    _CA_cert = CA_cert;
#endif
}

#endif    // HAS_SSL

////////////////////////////////////////

#if !( (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32_ASYNC) )
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning WEBSOCKETS_NETWORK_TYPE != NETWORK ASYNC
  #endif
/**
   called in arduino loop
*/
void WebSocketsClient::loop()
{
  if (_port == 0)
  {
    return;
  }

  WEBSOCKETS_YIELD();

  if (!clientIsConnected(&_client))
  {
    // do not flood the server
    if ((millis() - _lastConnectionFail) < _reconnectInterval)
    {
      return;
    }

#if defined(HAS_SSL)
  #if(_WEBSOCKETS_LOGLEVEL_>2)
    #warning HAS_SSL
  #endif
  
    if (_client.isSSL)
    {
      WSK_LOGWARN("[WS-Client] Connect wss...");

      if (_client.ssl)
      {
        delete _client.ssl;
        _client.ssl = NULL;
        _client.tcp = NULL;
      }
     
      _client.ssl = new WEBSOCKETS_NETWORK_SSL_CLASS();
      _client.tcp = _client.ssl;
     
      if (_CA_cert)
      {
        WSK_LOGWARN("[WS-Client] Setting CA certificate");

#if defined(ESP32) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning ESP32 or NETWORK_RTL8720DN
  #endif
  
        _client.ssl->setCACert(_CA_cert);
        
#elif defined(ESP8266) && defined(SSL_AXTLS)
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning ESP8266 and SSL_AXTLS
  #endif
  
        _client.ssl->setCACert((const uint8_t *)_CA_cert, strlen(_CA_cert) + 1);
        
#elif defined(ESP8266) && ( defined(SSL_BARESSL) || defined(SSL_BEARSSL) )
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning ESP8266 and SSL_BEARSSL
  #endif
  
        _client.ssl->setTrustAnchors(_CA_cert);
        
#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)    //defined(SEEED_WIO_TERMINAL)
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning NETWORK_RTL8720DN  
  #endif
    
        _client.ssl->setCACert(_CA_cert); 
   
#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_WIFININA)
  // Do something here for WiFiNINA

#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_WIFI101)
  // Do something here for WiFi101

#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_PORTENTA_H7_WIFI)
  // Do something here for Portenta_H7 WiFi if SSL is supported

#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_PORTENTA_H7_ETHERNET)
  // Do something here for Portenta_H7 Ethernet if SSL is supported
  
#elif (WEBSOCKETS_NETWORK_TYPE == NETWORK_RP2040W_WIFI) && ( defined(SSL_BARESSL) || defined(SSL_BEARSSL) )
  // Do something here for RP2040W WiFi SSL  
  #if(_WEBSOCKETS_LOGLEVEL_>3)
    #warning RP2040W and SSL_BEARSSL
  #endif
  
        _client.ssl->setTrustAnchors(_CA_cert);   
    
#else

  #error setCACert not implemented
#endif

      }

////////////////////
#if defined(ESP32) /*|| (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)*/
      else if (!SSL_FINGERPRINT_IS_SET) 
      {
        // ESP32 has setInsecure() now from v1.0.6
        _client.ssl->setInsecure();    
      }
#elif defined(ESP8266)
      else if (!SSL_FINGERPRINT_IS_SET) 
      {
        _client.ssl->setInsecure();    
      }
////////////////////
       
#elif ( defined(SSL_BARESSL) || defined(SSL_BEARSSL) )

      else if (SSL_FINGERPRINT_IS_SET)
      {
        _client.ssl->setFingerprint(_fingerprint);
      }
      else
      {
        #if defined(ESP8266)      
        // ESP32 has setInsecure() now from v1.0.6
        _client.ssl->setInsecure();
        #endif        
      }
      
      if(_client_cert && _client_key) 
      {
        _client.ssl->setClientRSACert(_client_cert, _client_key);
        WSK_LOGWARN("[WS-Client] setting client certificate and key");
      }  
#endif    // defined(ESP32) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)
    }
    else
    {
      WSK_LOGWARN("[WS-Client] Connect ws...");

      if (_client.tcp)
      {
        delete _client.tcp;
        _client.tcp = NULL;
      }

      _client.tcp = new WEBSOCKETS_NETWORK_CLASS();
    }

#else   // HAS_SSL
    #if(_WEBSOCKETS_LOGLEVEL_>2)
      #warning Not HAS_SSL
    #endif
    
    _client.tcp = new WEBSOCKETS_NETWORK_CLASS();
#endif  // HAS_SSL

    if (!_client.tcp)
    {
      WSK_LOGERROR("[WS-Client] Creating Network class failed!");
      return;
    }

    WEBSOCKETS_YIELD();
       
#if defined(ESP32) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)
    // KH test SSL
    WSK_LOGINFO3("[WS-Client] Calling _client.tcp->connect, _host =", _host, ", port =", _port); 
    
    int _connectResult = _client.tcp->connect(_host.c_str(), _port, WEBSOCKETS_TCP_TIMEOUT);
    
    WSK_LOGINFO1("[WS-Client] Calling _client.tcp->connect, _connectResult =", _connectResult);
    
    if (_connectResult)

#else
    if (_client.tcp->connect(_host.c_str(), _port))
#endif
    {
      WSK_LOGDEBUG("[WS-Client] connectedCb");
      connectedCb();
      _lastConnectionFail = 0;
    }
    else
    {
      WSK_LOGDEBUG("[WS-Client] connectFailedCb");
      connectFailedCb();
      _lastConnectionFail = millis();
    }
  }
  else
  {
    //WSK_LOGDEBUG("[WS-Client] handleClientData");
    handleClientData();
    WEBSOCKETS_YIELD();

    if (_client.status == WSC_CONNECTED)
    {
      handleHBPing();
      handleHBTimeout(&_client);
    }
  }
}
#endif    // #if !( (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32_ASYNC) )

////////////////////////////////////////

/**
   set callback function
   @param cbEvent WebSocketServerEvent
*/
void WebSocketsClient::onEvent(WebSocketClientEvent cbEvent)
{
  _cbEvent = cbEvent;
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
bool WebSocketsClient::sendTXT(uint8_t * payload, size_t length, bool headerToPayload)
{
  if (length == 0)
  {
    length = strlen((const char *)payload);
  }

  if (clientIsConnected(&_client))
  {
    return sendFrame(&_client, WSop_text, payload, length, true, headerToPayload);
  }

  return false;
}

////////////////////////////////////////

bool WebSocketsClient::sendTXT(const uint8_t * payload, size_t length)
{
  return sendTXT((uint8_t *)payload, length);
}

////////////////////////////////////////

bool WebSocketsClient::sendTXT(char * payload, size_t length, bool headerToPayload)
{
  return sendTXT((uint8_t *)payload, length, headerToPayload);
}

////////////////////////////////////////

bool WebSocketsClient::sendTXT(const char * payload, size_t length)
{
  return sendTXT((uint8_t *)payload, length);
}

////////////////////////////////////////

bool WebSocketsClient::sendTXT(const String& payload)
{
  return sendTXT((uint8_t *)payload.c_str(), payload.length());
}

////////////////////////////////////////

bool WebSocketsClient::sendTXT(char payload)
{
  uint8_t buf[WEBSOCKETS_MAX_HEADER_SIZE + 2] = { 0x00 };
  buf[WEBSOCKETS_MAX_HEADER_SIZE]             = payload;
  return sendTXT(buf, 1, true);
}

////////////////////////////////////////

/**
   send binary data to client
   @param num uint8_t client id
   @param payload uint8_t
   @param length size_t
   @param headerToPayload bool  (see sendFrame for more details)
   @return true if ok
*/
bool WebSocketsClient::sendBIN(uint8_t * payload, size_t length, bool headerToPayload)
{
  if (clientIsConnected(&_client))
  {
    return sendFrame(&_client, WSop_binary, payload, length, true, headerToPayload);
  }

  return false;
}

////////////////////////////////////////

bool WebSocketsClient::sendBIN(const uint8_t * payload, size_t length)
{
  return sendBIN((uint8_t *)payload, length);
}

////////////////////////////////////////

/**
   sends a WS ping to Server
   @param payload uint8_t
   @param length size_t
   @return true if ping is send out
*/
bool WebSocketsClient::sendPing(uint8_t * payload, size_t length)
{
  if (clientIsConnected(&_client))
  {
    bool sent = sendFrame(&_client, WSop_ping, payload, length);

    if (sent)
      _client.lastPing = millis();

    return sent;
  }

  return false;
}

////////////////////////////////////////

bool WebSocketsClient::sendPing(const String& payload)
{
  return sendPing((uint8_t *)payload.c_str(), payload.length());
}

////////////////////////////////////////

/**
   disconnect one client
   @param num uint8_t client id
*/
void WebSocketsClient::disconnect()
{
  if (clientIsConnected(&_client))
  {
    WebSockets::clientDisconnect(&_client, 1000);
  }
}

////////////////////////////////////////

/**
   set the Authorization for the http request
   @param user const char
   @param password const char
*/
void WebSocketsClient::setAuthorization(const char * user, const char * password)
{
  if (user && password)
  {
    String auth = user;
    auth += ":";
    auth += password;
    _client.base64Authorization = base64_encode((uint8_t *)auth.c_str(), auth.length());
  }
}

////////////////////////////////////////

/**
   set the Authorization for the http request
   @param auth const char * base64
*/
void WebSocketsClient::setAuthorization(const char * auth)
{
  if (auth)
  {
    //_client.base64Authorization = auth;
    _client.plainAuthorization = auth;
  }
}

////////////////////////////////////////

/**
   set extra headers for the http request;
   separate headers by "\r\n"
   @param extraHeaders const char * extraHeaders
*/
void WebSocketsClient::setExtraHeaders(const char * extraHeaders)
{
  _client.extraHeaders = extraHeaders;
}

////////////////////////////////////////

/**
   set the reconnect Interval
   how long to wait after a connection initiate failed
   @param time in ms
*/
void WebSocketsClient::setReconnectInterval(const unsigned long& time)
{
  _reconnectInterval = time;
}

////////////////////////////////////////

bool WebSocketsClient::isConnected()
{
  return (_client.status == WSC_CONNECTED);
}

////////////////////////////////////////
////////////////////////////////////////

/**

   @param client WSclient_t *  ptr to the client struct
   @param opcode WSopcode_t
   @param payload  uint8_t
   @param length size_t
*/
void WebSocketsClient::messageReceived(WSclient_t * client, WSopcode_t opcode,
                                       uint8_t * payload, size_t length, bool fin)
{
  WStype_t type = WStype_ERROR;

  UNUSED(client);

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

  runCbEvent(type, payload, length);
}

////////////////////////////////////////

/**
   Disconnect an client
   @param client WSclient_t *  ptr to the client struct
*/
void WebSocketsClient::clientDisconnect(WSclient_t * client)
{
  bool event = false;

#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266)   || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32)    || \
    (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_WIFININA) || \
    (WEBSOCKETS_NETWORK_TYPE == NETWORK_WIFI101)   || (WEBSOCKETS_NETWORK_TYPE == NETWORK_RP2040W_WIFI)
  if (client->isSSL && client->ssl)
  {
    if (client->ssl->connected())
    {
      client->ssl->flush();
      client->ssl->stop();
    }

    event = true;
    delete client->ssl;
    client->ssl = NULL;
    client->tcp = NULL;
  }
#endif

  if (client->tcp)
  {
    if (client->tcp->connected())
    {
#if !( (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32_ASYNC) )
      client->tcp->flush();
#endif
      client->tcp->stop();
    }

    event = true;
    
#if ( (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32_ASYNC) )
    client->status = WSC_NOT_CONNECTED;
#else
    delete client->tcp;
#endif

    client->tcp = NULL;
  }

  client->cCode        = 0;
  client->cKey         = "";
  client->cAccept      = "";
  client->cVersion     = 0;
  client->cIsUpgrade   = false;
  client->cIsWebsocket = false;
  client->cSessionId   = "";

  client->status = WSC_NOT_CONNECTED;
  
  // KH, update 2.14.2
  _lastConnectionFail = millis();

  WSK_LOGDEBUG("[WS-Client] client disconnected.");

  if (event)
  {
    runCbEvent(WStype_DISCONNECTED, NULL, 0);
  }
}

////////////////////////////////////////

/**
   get client state
   @param client WSclient_t *  ptr to the client struct
   @return true = conneted
*/
bool WebSocketsClient::clientIsConnected(WSclient_t * client)
{
  if (!client->tcp)
  {
    return false;
  }

  if (client->tcp->connected())
  {
    if (client->status != WSC_NOT_CONNECTED) {
      return true;
    }
  }
  else
  {
    // client lost
    if (client->status != WSC_NOT_CONNECTED)
    {
      WSK_LOGWARN("[WS-Client] connection lost.");

      // do cleanup
      clientDisconnect(client);
    }
  }

  if (client->tcp)
  {
    // do cleanup
    clientDisconnect(client);
  }

  return false;
}

////////////////////////////////////////

#if !( (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32_ASYNC) )
/**
   Handle incomming data from Client
*/
void WebSocketsClient::handleClientData()
{
  if ((_client.status == WSC_HEADER || _client.status == WSC_BODY) && _lastHeaderSent + WEBSOCKETS_TCP_TIMEOUT < millis())
  {
    WSK_LOGINFO("[WS-Client][handleClientData] Header response timeout.. Disconnecting!");
    clientDisconnect(&_client);
    WEBSOCKETS_YIELD();
    return;
  }

  int len = _client.tcp->available();

  if (len > 0)
  {
    switch (_client.status)
    {
      case WSC_HEADER:
        {
          String headerLine = _client.tcp->readStringUntil('\n');         
          handleHeader(&_client, &headerLine);
        }
        
        break;
      case WSC_BODY: 
        {
          char buf[256] = { 0 };
          _client.tcp->readBytes(&buf[0], std::min((size_t)len, sizeof(buf)));
          String bodyLine = buf;         
          handleHeader(&_client, &bodyLine);
        } 
        
        break;  
      case WSC_CONNECTED:
        WebSockets::handleWebsocket(&_client);
        
        break;
      default:
        WebSockets::clientDisconnect(&_client, 1002);
        
        break;
    }
  }

  WEBSOCKETS_YIELD();
}
#endif

////////////////////////////////////////

/**
   send the WebSocket header to Server
   @param client WSclient_t *  ptr to the client struct
*/
void WebSocketsClient::sendHeader(WSclient_t * client)
{
  static const char * NEW_LINE = "\r\n";

  WSK_LOGINFO("[WS-Client] [sendHeader] Sending header...");

  uint8_t randomKey[16] = { 0 };

  for (uint8_t i = 0; i < sizeof(randomKey); i++)
  {
    randomKey[i] = random(0xFF);
  }

  client->cKey = base64_encode(&randomKey[0], 16);
   
  WSK_LOGINFO1("sendHeader: client->cKey = ", client->cKey);

  unsigned long start = micros();

  String handshake;
  bool ws_header = true;
  String url     = client->cUrl;

  if (client->isSocketIO)
  {
    if (client->cSessionId.length() == 0)
    {
#if USING_STICKY_SESSION_SIO
      #if(_WEBSOCKETS_LOGLEVEL_>2)
        #warning Using USING_STICKY_SESSION_SIO with transport=websocket
      #endif
      
      url += WEBSOCKETS_STRING("&transport=websocket");
#else      
      url += WEBSOCKETS_STRING("&transport=polling");
#endif      
      ws_header = false;
    }
    else
    {
      url += WEBSOCKETS_STRING("&transport=websocket&sid=");
      url += client->cSessionId;
    }
  }

  handshake = WEBSOCKETS_STRING("GET ");
  handshake += url + WEBSOCKETS_STRING(
                 " HTTP/1.1\r\n"
                 "Host: ");
  handshake += _host + ":" + _port + NEW_LINE;

  if (ws_header)
  {
    handshake += WEBSOCKETS_STRING(
                   "Connection: Upgrade\r\n"
                   "Upgrade: websocket\r\n"
                   "Sec-WebSocket-Version: 13\r\n"
                   "Sec-WebSocket-Key: ");
    handshake += client->cKey + NEW_LINE;

    if (client->cProtocol.length() > 0)
    {
      handshake += WEBSOCKETS_STRING("Sec-WebSocket-Protocol: ");
      handshake += client->cProtocol + NEW_LINE;
    }

    if (client->cExtensions.length() > 0)
    {
      handshake += WEBSOCKETS_STRING("Sec-WebSocket-Extensions: ");
      handshake += client->cExtensions + NEW_LINE;
    }
  }
  else
  {
    handshake += WEBSOCKETS_STRING("Connection: keep-alive\r\n");
  }

  // add extra headers; by default this includes "Origin: file://"
  if(client->extraHeaders.length() > 0)
  {
    handshake += client->extraHeaders + NEW_LINE;
  }

  handshake += WEBSOCKETS_STRING("User-Agent: arduino-WebSocket-Client\r\n");

  if (client->base64Authorization.length() > 0)
  {
    handshake += WEBSOCKETS_STRING("Authorization: Basic ");
    handshake += client->base64Authorization + NEW_LINE;
  }

  if (client->plainAuthorization.length() > 0)
  {
    handshake += WEBSOCKETS_STRING("Authorization: ");
    handshake += client->plainAuthorization + NEW_LINE;
  }

  handshake += NEW_LINE;

  WSK_LOGINFO1("[WS-Client] [sendHeader] Handshake:", handshake);

  write(client, (uint8_t *)handshake.c_str(), handshake.length());
  
#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)

	client->tcp->readStringUntil('\n', &(client->cHttpLine), std::bind(&WebSocketsClient::handleHeader, 
                                 this, client, &(client->cHttpLine)));
  //////
#endif

  WSK_LOGINFO1("[WS-Client] [sendHeader] Sending header... Done (us):", (micros() - start));

  _lastHeaderSent = millis();
}

////////////////////////////////////////

/**
   handle the WebSocket header reading
   @param client WSclient_t *  ptr to the client struct
*/

void WebSocketsClient::handleHeader(WSclient_t * client, String * headerLine)
{
  headerLine->trim();    // remove \r

  // this code handels the http body for Socket.IO requests
  if ( (headerLine->length() > 0) && (client->isSocketIO) && (client->status == WSC_BODY) && \
       (client->cSessionId.length() == 0) )
  {
    WSK_LOGINFO1("[WS-Client][handleHeader] socket.io json: ", headerLine->c_str());
    
    String sid_begin = WEBSOCKETS_STRING("\"sid\":\"");
    
    if (headerLine->indexOf(sid_begin) > -1) 
    {
      int start          = headerLine->indexOf(sid_begin) + sid_begin.length();
      int end            = headerLine->indexOf('"', start);
      client->cSessionId = headerLine->substring(start, end);
      
      WSK_LOGINFO1("[WS-Client][handleHeader] - cSessionId: ", client->cSessionId.c_str());
      
      // Trigger websocket connection code path
      *headerLine = "";
    }
  }     
       
  // handle HTTP header
  if (headerLine->length() > 0)   
  {
    WSK_LOGINFO1("[WS-Client][handleHeader] RX:", headerLine->c_str());

    if (headerLine->startsWith(WEBSOCKETS_STRING("HTTP/1.")))
    {
      // "HTTP/1.1 101 Switching Protocols"
      client->cCode = headerLine->substring(9, headerLine->indexOf(' ', 9)).toInt();
    }
    else if (headerLine->indexOf(':') >= 0)
    {
      String headerName  = headerLine->substring(0, headerLine->indexOf(':'));
      String headerValue = headerLine->substring(headerLine->indexOf(':') + 1);

      // remove space in the beginning  (RFC2616)
      if (headerValue[0] == ' ')
      {
        headerValue.remove(0, 1);
      }

      if (headerName.equalsIgnoreCase(WEBSOCKETS_STRING("Connection")))
      {
        if (headerValue.equalsIgnoreCase(WEBSOCKETS_STRING("upgrade")))
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
      else if (headerName.equalsIgnoreCase(WEBSOCKETS_STRING("Sec-WebSocket-Accept")))
      {
        client->cAccept = headerValue;
        client->cAccept.trim();    // see rfc6455
      }
      else if (headerName.equalsIgnoreCase(WEBSOCKETS_STRING("Sec-WebSocket-Protocol")))
      {
        client->cProtocol = headerValue;
      }
      else if (headerName.equalsIgnoreCase(WEBSOCKETS_STRING("Sec-WebSocket-Extensions")))
      {
        client->cExtensions = headerValue;
      }
      else if (headerName.equalsIgnoreCase(WEBSOCKETS_STRING("Sec-WebSocket-Version")))
      {
        client->cVersion = headerValue.toInt();
      }
      else if (headerName.equalsIgnoreCase(WEBSOCKETS_STRING("Set-Cookie")))
      {
        if (headerValue.indexOf(';') > -1)
        {
          client->cSessionId = headerValue.substring(headerValue.indexOf('=') + 1, headerValue.indexOf(";"));
        }
        else
        {
          client->cSessionId = headerValue.substring(headerValue.indexOf('=') + 1);
        }
      }
    }
    else
    {
      WSK_LOGINFO1("[WS-Client][handleHeader] Header error:", headerLine->c_str());
    }

		*headerLine = "";
		
#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)

    client->tcp->readStringUntil('\n', &(client->cHttpLine), std::bind(&WebSocketsClient::handleHeader, 
                                 this, client, &(client->cHttpLine)));
                            
#endif
  }
  else
  {
    WSK_LOGINFO("[WS-Client][handleHeader] Header read fin.");
    WSK_LOGINFO("[WS-Client][handleHeader] Client settings:");
    WSK_LOGINFO1("[WS-Client][handleHeader] - cURL:",          client->cUrl.c_str());
    WSK_LOGINFO1("[WS-Client][handleHeader] - cKey:",          client->cKey.c_str());
    WSK_LOGINFO("[WS-Client][handleHeader] Server header:");
    WSK_LOGINFO1("[WS-Client][handleHeader] - cCode:",         client->cCode);
    WSK_LOGINFO1("[WS-Client][handleHeader] - cIsUpgrade:",    client->cIsUpgrade);
    WSK_LOGINFO1("[WS-Client][handleHeader] - cIsWebsocket:",  client->cIsWebsocket);
    WSK_LOGINFO1("[WS-Client][handleHeader] - cAccept:",       client->cAccept.c_str());
    WSK_LOGINFO1("[WS-Client][handleHeader] - cProtocol:",     client->cProtocol.c_str());
    WSK_LOGINFO1("[WS-Client][handleHeader] - cExtensions:",   client->cExtensions.c_str());
    WSK_LOGINFO1("[WS-Client][handleHeader] - cVersion:",      client->cVersion);
    WSK_LOGINFO1("[WS-Client][handleHeader] - cSessionId:",    client->cSessionId.c_str());


    if(client->isSocketIO && client->cSessionId.length() == 0 && clientIsConnected(client)) 
    {
      WSK_LOGINFO("[WS-Client][handleHeader] Still missing cSessionId try Socket.IO");
      client->status = WSC_BODY;
      
      return; 
    } 
    else 
    {
      client->status = WSC_HEADER;
    }
        
    bool ok = (client->cIsUpgrade && client->cIsWebsocket);

    if (ok)
    {
      if ( (client->cCode == 101) || ( (client->cCode == 200)  && (client->isSocketIO) ) )
      {
        // Do nothing
      }
      else
      {
        // including 403 (forbidden)
        
        ok = false;

        WSK_LOGINFO1("[WS-Client][handleHeader] serverCode is not 101 :", client->cCode);

        clientDisconnect(client);
        _lastConnectionFail = millis();
      }  
    }

    if (ok)
    {
      if (client->cAccept.length() == 0)
      {
        ok = false;
      }
      else
      {
        // generate Sec-WebSocket-Accept key for check
        String sKey = acceptKey(client->cKey);

        if (sKey != client->cAccept)
        {
          WSK_LOGINFO("[WS-Client][handleHeader] Sec-WebSocket-Accept is wrong");

          ok = false;
        }
      }
    }

    if (ok)
    {
      WSK_LOGINFO("[WS-Client][handleHeader] Websocket connection init done.");

      headerDone(client);

      runCbEvent(WStype_CONNECTED, (uint8_t *)client->cUrl.c_str(), client->cUrl.length());
    }
    
#if !( (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32_ASYNC) )

    else if (client->isSocketIO) 
    {
      if (client->cSessionId.length() > 0) 
      {
        WSK_LOGINFO("[WS-Client][handleHeader] found cSessionId");

        if(clientIsConnected(client) && _client.tcp->available()) 
        {
          // read not needed data
          DEBUG_WEBSOCKETS("[WS-Client][handleHeader] still data in buffer (%d), clean up.\n", _client.tcp->available());
          
          while(_client.tcp->available() > 0) 
          {
            _client.tcp->read();
          }
        }

        sendHeader(client);
      }
    }
    
#endif

    else
    {
      WSK_LOGDEBUG("[WS-Client][handleHeader] no Websocket connection close.");

      _lastConnectionFail = millis();

      if (clientIsConnected(client))
      {
        write(client, "This is a webSocket client!");
      }

      clientDisconnect(client);
    }
  }
}

////////////////////////////////////////

void WebSocketsClient::connectedCb()
{
  WSK_LOGWARN3("[WS-Client][connectedCb] Connected to Host:",  _host, ", Port:", _port);

#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)

  _client.tcp->onDisconnect(std::bind([](WebSocketsClient * c, AsyncTCPbuffer * obj, WSclient_t * client) -> bool
  {
  	UNUSED (obj);
  	
    WSK_LOGWARN1("[WS-Client][connectedCb] Disconnect client :", client->num);

    client->status = WSC_NOT_CONNECTED;
    client->tcp    = NULL;

    // reconnect
    c->asyncConnect();

    return true;
  }, this, std::placeholders::_1, &_client));
#endif

  _client.status = WSC_HEADER;

#if !( (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32_ASYNC) )
  // set Timeout for readBytesUntil and readStringUntil
  _client.tcp->setTimeout(WEBSOCKETS_TCP_TIMEOUT);
#endif

#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266)   || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32) || \
    (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_RP2040W_WIFI)
  _client.tcp->setNoDelay(true);
#endif

#if defined(HAS_SSL)

#if (defined(SSL_AXTLS) || defined(ESP32) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_RTL8720DN)) && \
     ( (WEBSOCKETS_NETWORK_TYPE != NETWORK_WIFININA) && (WEBSOCKETS_NETWORK_TYPE != NETWORK_WIFI101) )

  if(_client.isSSL && SSL_FINGERPRINT_IS_SET)
  {
    if (!_client.ssl->verify(_fingerprint.c_str(), _host.c_str()))
    {
      WSK_LOGERROR("[WS-Client][connectedCb] Certificate mismatch");

      WebSockets::clientDisconnect(&_client, 1000);
      return;
    }
  }
#else
  if(_client.isSSL && SSL_FINGERPRINT_IS_SET)
  {
  }
#endif
  else if (_client.isSSL && !_CA_cert)
  {
#if (defined(SSL_BARESSL) || defined(SSL_BEARSSL) )
    // ESP32 has setInsecure() now
    _client.ssl->setInsecure(); 
#endif
  }

#endif    // HAS_SSL

  // send Header to Server
  sendHeader(&_client);
}

////////////////////////////////////////

void WebSocketsClient::connectFailedCb()
{
  WSK_LOGDEBUG3("[WS-Client][connectFailedCb] Failed connection to host:",  _host, ", port:", _port);
}

#if ( (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32_ASYNC) )

void WebSocketsClient::asyncConnect()
{
  WSK_LOGINFO("[WS-Client] asyncConnect...");

  AsyncClient * tcpclient = new AsyncClient();

  if (!tcpclient)
  {
    WSK_LOGINFO("[WS-Client] Creating AsyncClient class failed!");

    return;
  }

  tcpclient->onDisconnect([](void * obj, AsyncClient * c)
  {
  	UNUSED (obj);
  	
    c->free();
    delete c;
  });

  tcpclient->onConnect(std::bind([](WebSocketsClient * ws, AsyncClient * tcp)
  {
    ws->_client.tcp = new AsyncTCPbuffer(tcp);

    if (!ws->_client.tcp)
    {
      WSK_LOGINFO("[WS-Client] Creating Network class failed!");

      ws->connectFailedCb();
      return;
    }

    ws->connectedCb();
  }, this, std::placeholders::_2));

  tcpclient->onError(std::bind([](WebSocketsClient * ws, AsyncClient * tcp)
  {
  	UNUSED (tcp);
  	
    ws->connectFailedCb();

    // reconnect
    ws->asyncConnect();
  }, this, std::placeholders::_2));

  if (!tcpclient->connect(_host.c_str(), _port))
  {
    connectFailedCb();
    delete tcpclient;
  }
}

#endif

////////////////////////////////////////

/**
   send heartbeat ping to server in set intervals
*/
void WebSocketsClient::handleHBPing()
{
  if (_client.pingInterval == 0)
    return;

  uint32_t pi = millis() - _client.lastPing;

  if (pi > _client.pingInterval)
  {
    WSK_LOGWARN("[WS-Client] Sending HB ping");

    if (sendPing())
    {
      _client.lastPing     = millis();
      _client.pongReceived = false;
    }
    else
    {
      WSK_LOGERROR("[WS-Client] sending HB ping failed");
      WebSockets::clientDisconnect(&_client, 1000);
    }
  }
}

////////////////////////////////////////

/**
   enable ping/pong heartbeat process
   @param pingInterval uint32_t how often ping will be sent
   @param pongTimeout uint32_t millis after which pong should timout if not received
   @param disconnectTimeoutCount uint8_t how many timeouts before disconnect, 0=> do not disconnect
*/
void WebSocketsClient::enableHeartbeat(const uint32_t& pingInterval, const uint32_t& pongTimeout, 
                                       const uint8_t& disconnectTimeoutCount)
{
  WebSockets::enableHeartbeat(&_client, pingInterval, pongTimeout, disconnectTimeoutCount);
}

////////////////////////////////////////

/**
   disable ping/pong heartbeat process
*/
void WebSocketsClient::disableHeartbeat()
{
  _client.pingInterval = 0;
}

////////////////////////////////////////

#endif    // WEBSOCKETS_CLIENT_GENERIC_IMPL_H_
