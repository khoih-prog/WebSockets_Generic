/****************************************************************************************************************************
  WebSocketsClient_Generic.h - WebSockets Library for boards
  
  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.
  
  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license
   
  @original file WebSocketsClient.h
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
  
  Version: 2.14.0

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
  2.13.0  K Hoang      14/02/2022 Add support to ESP32_S3. Add PING and PONG SocketIO events
  2.14.0  K Hoang      17/02/2022 Suppress unnecessary warnings. Optimize code by passing by reference instead of value
 *****************************************************************************************************************************/

#pragma once

#ifndef WEBSOCKETS_CLIENT_GENERIC_H_
#define WEBSOCKETS_CLIENT_GENERIC_H_

#include "WebSockets_Generic.h"

class WebSocketsClient : protected WebSockets
{
  public:
#ifdef __AVR__
    typedef void (*WebSocketClientEvent)(WStype_t type, uint8_t * payload, size_t length);
#else
    typedef std::function<void(WStype_t type, uint8_t * payload, size_t length)> WebSocketClientEvent;
#endif

    WebSocketsClient();
    virtual ~WebSocketsClient();

    void begin(const char * host, const uint16_t& port, const char * url = "/", const char * protocol = "arduino");
    void begin(const String& host, const uint16_t& port, const String& url = "/", const String& protocol = "arduino");
    void begin(const IPAddress& host, const uint16_t& port, const char * url = "/", const char * protocol = "arduino");

    // KH
    void begin(const IPAddress& host, const uint16_t& port, const String& url = "/", const String& protocol = "arduino");

#if defined(HAS_SSL)
#ifdef SSL_AXTLS

    void beginSSL(const char * host, const uint16_t& port, const char * url = "/", const char * fingerprint = "", 
                  const char * protocol = "arduino");
    
    // KH
    void beginSSL(const IPAddress& host, const uint16_t& port, const String& url = "/", const String& fingerprint = "", 
                  const String& protocol = "arduino");
    //////
    
    void beginSSL(const String& host, const uint16_t& port, const String& url = "/", const String& fingerprint = "", 
                  const String& protocol = "arduino");

#else

    void beginSSL(const char * host, const uint16_t& port, const char * url = "/", const uint8_t * fingerprint = NULL, 
                  const char * protocol = "arduino");
    
    // KH
    void beginSSL(const IPAddress& host, const uint16_t& port, const String& url = "/", const String& fingerprint = "", 
                  const String& protocol = "arduino");
    //////
    
    void beginSslWithCA(const char * host, const uint16_t& port, const char * url = "/", BearSSL::X509List * CA_cert = NULL, 
                        const char * protocol = "arduino");
    
    // New in v2.3.5
    void setSSLClientCertKey(BearSSL::X509List * clientCert = NULL, BearSSL::PrivateKey * clientPrivateKey = NULL);
    
    void setSSLClientCertKey(const char * clientCert = NULL, const char * clientPrivateKey = NULL);
    //////

#endif    // SSL_AXTLS

    void beginSslWithCA(const char * host, const uint16_t& port, const char * url = "/", const char * CA_cert = NULL, const char * protocol = "arduino");

#endif    // HAS_SSL

    void beginSocketIO(const char * host, const uint16_t& port, const char * url = "/socket.io/?EIO=4", 
                       const char * protocol = "arduino");
                       
    void beginSocketIO(const String& host, const uint16_t& port, const String& url = "/socket.io/?EIO=4", 
                       const String& protocol = "arduino");
    
    // KH
    void beginSocketIO(const IPAddress& host, const uint16_t& port, const String& url = "/socket.io/?EIO=4", 
                       const String& protocol = "arduino");
    //////

#if defined(HAS_SSL)
    void beginSocketIOSSL(const char * host, const uint16_t& port, const char * url = "/socket.io/?EIO=4", 
                          const char * protocol = "arduino");
                          
    void beginSocketIOSSL(const String& host, const uint16_t& port, const String& url = "/socket.io/?EIO=4", 
                          const String& protocol = "arduino");
    
    // KH
    void beginSocketIOSSL(const IPAddress& host, const uint16_t& port, const String& url = "/socket.io/?EIO=4", 
                          const String& protocol = "arduino");
    //////
    
    void beginSocketIOSSLWithCA(const char * host, const uint16_t& port, const char * url = "/socket.io/?EIO=4", 
                                const char * CA_cert = NULL, const char * protocol = "arduino");
    
#if defined(SSL_BARESSL)
    void beginSocketIOSSLWithCA(const char * host, const uint16_t& port, const char * url = "/socket.io/?EIO=4", 
                                BearSSL::X509List * CA_cert = NULL, const char * protocol = "arduino");
#endif    // SSL_BARESSL
    
#endif    // HAS_SSL

#if(WEBSOCKETS_NETWORK_TYPE != NETWORK_ESP8266_ASYNC)
    void loop();
#else
    // Async interface not need a loop call
    void loop() __attribute__((deprecated)) {}
#endif

    void onEvent(WebSocketClientEvent cbEvent);

    bool sendTXT(uint8_t * payload, size_t length = 0, bool headerToPayload = false);
    bool sendTXT(const uint8_t * payload, size_t length = 0);
    bool sendTXT(char * payload, size_t length = 0, bool headerToPayload = false);
    bool sendTXT(const char * payload, size_t length = 0);
    bool sendTXT(const String& payload);
    bool sendTXT(char payload);

    bool sendBIN(uint8_t * payload, size_t length, bool headerToPayload = false);
    bool sendBIN(const uint8_t * payload, size_t length);

    bool sendPing(uint8_t * payload = NULL, size_t length = 0);
    bool sendPing(const String& payload);

    void disconnect();

    void setAuthorization(const char * user, const char * password);
    void setAuthorization(const char * auth);

    void setExtraHeaders(const char * extraHeaders = NULL);

    void setReconnectInterval(const unsigned long& time);

    void enableHeartbeat(const uint32_t& pingInterval, const uint32_t& pongTimeout, const uint8_t& disconnectTimeoutCount);
    void disableHeartbeat();

    bool isConnected();

  protected:
    String _host;
    uint16_t _port;

#if defined(HAS_SSL)

#ifdef SSL_AXTLS
    String _fingerprint;
    const char * _CA_cert;
    
    #define SSL_FINGERPRINT_IS_SET      (_fingerprint.length())    
    #define SSL_FINGERPRINT_NULL        ""
#else
    const uint8_t * _fingerprint;
    BearSSL::X509List * _CA_cert;
    BearSSL::X509List * _client_cert;
    BearSSL::PrivateKey * _client_key;
    
    #define SSL_FINGERPRINT_IS_SET      (_fingerprint != NULL)    
    #define SSL_FINGERPRINT_NULL        NULL
#endif

#endif

    WSclient_t _client;

    WebSocketClientEvent _cbEvent;

    unsigned long _lastConnectionFail;
    unsigned long _reconnectInterval;
    unsigned long _lastHeaderSent;

    void messageReceived(WSclient_t * client, WSopcode_t opcode, uint8_t * payload, size_t length, bool fin);

    void clientDisconnect(WSclient_t * client);
    bool clientIsConnected(WSclient_t * client);

#if(WEBSOCKETS_NETWORK_TYPE != NETWORK_ESP8266_ASYNC)
    void handleClientData();
#endif

    void sendHeader(WSclient_t * client);
       
    void handleHeader(WSclient_t * client, String& headerLine); 

    void connectedCb();
    void connectFailedCb();

    void handleHBPing();    // send ping in specified intervals

#if(WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)
    void asyncConnect();
#endif

    /**
           called for sending a Event to the app
           @param type WStype_t
           @param payload uint8_t
           @param length size_t
    */
    virtual void runCbEvent(WStype_t type, uint8_t * payload, size_t length)
    {
      if (_cbEvent)
      {
        _cbEvent(type, payload, length);
      }
    }
};

#include "WebSocketsClient_Generic-Impl.h"

#endif    // WEBSOCKETS_CLIENT_GENERIC_H_
