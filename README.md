## WebSockets_Generic

[![arduino-library-badge](https://www.ardu-badge.com/badge/WebSockets_Generic.svg?)](https://www.ardu-badge.com/WebSockets_Generic)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/WebSockets_Generic.svg)](https://github.com/khoih-prog/WebSockets_Generic/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/WebSockets_Generic/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/WebSockets_Generic.svg)](http://github.com/khoih-prog/WebSockets_Generic/issues)


WebSocket Server and Client for Arduino based on RFC6455.

#### New in v2.1.3

1. Add support to ***nRF52*** boards, such as ***AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B30_ublox, etc.***
2. Add support to ***SAM51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.) and SAM DUE***.
3. Add support to ***SAMD21 (ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit CIRCUITPLAYGROUND_EXPRESS, etc.)***
4. Add support to ***Teensy (4.0, 3.6, 3.5, 3,2, 3.1, 3.0, LC)***
5. Add support to ***STM32F1, STM32F2, STM32F4, STM32F7 with more than 32KB flash memory.***


##### Supported features of RFC6455 #####
 - text frame
 - binary frame
 - connection close
 - ping
 - pong
 - continuation frame

##### Limitations #####
 - max input length is limited to the ram size and the ```WEBSOCKETS_MAX_DATA_SIZE``` define
 - max output length has no limit (the hardware is the limit)
 - Client send big frames with mask 0x00000000 (on AVR all frames)
 - continuation frame reassembly need to be handled in the application code

 ##### Limitations for Async #####
 - Functions called from within the context of the websocket event might not honor `yield()` and/or `delay()`.  See [this issue](https://github.com/Links2004/arduinoWebSockets/issues/58#issuecomment-192376395) for more info and a potential workaround.
 - wss / SSL is not possible.

##### Supported Hardware #####

 - ESP8266 [Arduino for ESP8266](https://github.com/esp8266/Arduino/)
 - ESP32 [Arduino for ESP32](https://github.com/espressif/arduino-esp32)
 - ESP31B
 - Particle with STM32 ARM Cortex M3
 - ATmega328 with Ethernet Shield (ATmega branch)
 - ATmega328 with enc28j60 (ATmega branch)
 - ATmega2560 with Ethernet Shield (ATmega branch)
 - ATmega2560 with enc28j60 (ATmega branch)
 
##### New support from v2.1.3
 
 - ***nRF52*** boards, such as ***AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B30_ublox, etc.***
 - ***SAM51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.) and SAM DUE***.
 - ***SAMD21 (ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit CIRCUITPLAYGROUND_EXPRESS, etc.)***
 - ***Teensy (4.0, 3.6, 3.5, 3,2, 3.1, 3.0, LC)***
 - ***STM32F1, STM32F2, STM32F4, STM32F7 with more than 32KB flash memory.***

###### Note: ######

  version 2.0 and up is not compatible with AVR/ATmega, check ATmega branch.

  Arduino for AVR not supports std namespace of c++.

### wss / SSL ###
 supported for:
 - wss client on the ESP8266
 - wss / SSL is not natively supported in WebSocketsServer however it is possible to achieve secure websockets
   by running the device behind an SSL proxy. See [Nginx](examples/Nginx/esp8266.ssl.reverse.proxy.conf) for a
   sample Nginx server configuration file to enable this.

### ESP Async TCP ###

This libary can run in Async TCP mode on the ESP.

The mode can be activated in the ```WebSockets.h``` (see WEBSOCKETS_NETWORK_TYPE define).

[ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP) libary is required.


### High Level Client API ###

 - `begin` : Initiate connection sequence to the websocket host.
 
```
void begin(const char *host, uint16_t port, const char * url = "/", const char * protocol = "arduino");
void begin(String host, uint16_t port, String url = "/", String protocol = "arduino");
```

 - `onEvent`: Callback to handle for websocket events

```
void onEvent(WebSocketClientEvent cbEvent);
```

 - `WebSocketClientEvent`: Handler for websocket events
 
```
void (*WebSocketClientEvent)(WStype_t type, uint8_t * payload, size_t length)
```

Where `WStype_t type` is defined as:

```
typedef enum 
{
  WStype_ERROR,
  WStype_DISCONNECTED,
  WStype_CONNECTED,
  WStype_TEXT,
  WStype_BIN,
	WStype_FRAGMENT_TEXT_START,
	WStype_FRAGMENT_BIN_START,
	WStype_FRAGMENT,
	WStype_FRAGMENT_FIN,
} WStype_t;
```

### Issues ###

Submit issues to: [WebSockets_Generic issues](https://github.com/khoih-prog/WebSockets_Generic/issues)

[![Join the chat at https://gitter.im/Links2004/arduinoWebSockets](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/Links2004/arduinoWebSockets?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

### Contributions and thanks

1. Based on and modified from [Markus Sattler's WebSockets](https://github.com/Links2004/arduinoWebSockets)

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library


### License and credits ###

- All credits go to original author [Markus Sattler](https://github.com/Links2004)

- The library is licensed under [LGPLv2.1](https://github.com/Links2004/arduinoWebSockets/blob/master/LICENSE) and [MIT](https://github.com/khoih-prog/WebSockets_Generic/blob/master/LICENSE)

- [libb64](http://libb64.sourceforge.net/) written by Chris Venter. It is distributed under Public Domain see [LICENSE](https://github.com/Links2004/arduinoWebSockets/blob/master/src/libb64/LICENSE).
