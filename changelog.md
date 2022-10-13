## WebSockets_Generic

[![arduino-library-badge](https://www.ardu-badge.com/badge/WebSockets_Generic.svg?)](https://www.ardu-badge.com/WebSockets_Generic)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/WebSockets_Generic.svg)](https://github.com/khoih-prog/WebSockets_Generic/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/WebSockets_Generic/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/WebSockets_Generic.svg)](http://github.com/khoih-prog/WebSockets_Generic/issues)

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Release v2.16.0](#release-v2160)
  * [Release v2.15.0](#release-v2150)
  * [Release v2.14.2](#release-v2142)
  * [Release v2.14.1](#release-v2141)
  * [Release v2.14.0](#release-v2140)
  * [Release v2.13.0](#release-v2130)
  * [Major Release v2.12.0](#major-release-v2120)
  * [Release v2.11.1](#release-v2111)
  * [Major Release v2.11.0](#major-release-v2110)
  * [Release v2.10.1](#release-v2101)
  * [Major Release v2.10.0](#major-release-v2100)
  * [Major Release v2.9.0](#major-release-v290)
  * [Major Release v2.8.0](#major-release-v280)
  * [Major Release v2.7.0](#major-release-v270)
  * [Major Release v2.6.0](#major-release-v260)
  * [Release v2.5.1](#release-v251)
  * [Major Release v2.5.0](#major-release-v250)
  * [Release v2.4.1](#release-v241)
  * [Major Release v2.4.0](#major-release-v240)
  * [Release v2.3.4](#release-v234)
  * [Release v2.3.3](#release-v233)
  * [Release v2.3.2](#release-v232)
  * [Release v2.3.1](#release-v231)
  * [Release v2.2.3](#release-v223)
  * [Release v2.2.2](#release-v222)
  * [Release v2.2.1](#release-v221)
  * [Release v2.1.3](#release-v213)
* [Important Notes](#important-notes)
  * [Originally Supported Hardware](#originally-supported-hardware)
  * [New support from v2.10.0](#new-support-from-v2100)
  * [New support from v2.9.0](#new-support-from-v290)
  * [New support from v2.8.0](#new-support-from-v280)
  * [New support from v2.7.0](#new-support-from-v270)
  * [New support from v2.5.0](#new-support-from-v250)
  * [New support from v2.4.0](#new-support-from-v240)
  * [New support from v2.3.4](#new-support-from-v234)
  * [New support from v2.3.2](#new-support-from-v232)
  * [New support from v2.3.1](#new-support-from-v231)
  * [New support from v2.2.3](#new-support-from-v223)
  * [New support from v2.2.2](#new-support-from-v222)
  * [New support from v2.1.3](#new-support-from-v213)
---
---

## Changelog

### Release v2.16.0

1. Add support to **RP2040-based boards, such as Nano_RP2040_Connect, RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040**, using WiFiNINA or Ethernet
2. Add `WS` and `BearSSL WSS` support to `RP2040W` using `CYW43439 WiFi` with `arduino-pico` core
3. Optional user-defined 
- `WEBSOCKETS_TCP_TIMEOUT`, default 5,000ms
- `EIO_HEARTBEAT_INTERVAL`, default 20,000ms
- `SIO_PING_INTERVAL`, default 60,000ms
- `SIO_PONG_TIMEOUT`, default 90,000ms
- `SIO_DISCONNECT_TIMEOUT_COUNT`, default 5
4. Update `Packages' Patches`

### Release v2.15.0

1. Use new [Ethernet_Generic library](https://github.com/khoih-prog/Ethernet_Generic) as default for W5x00.
2. Sync with `arduinoWebSockets v2.3.6`
3. Add examples and rewrite all the examples to support new features
4. Update `Packages' Patches`


### Release v2.14.2

1. Fix Async bug for ESP8266 when using NETWORK_ESP8266_ASYNC
2. Add example [ESP8266_AsyncWSClient](examples/Async/ESP8266_AsyncWSClient)
3. Update `platformio.ini` to temporarily use ESP32 core v2.0.0+ and use [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP) library for ESP8266 Async
4. Update `Packages' Patches`

### Release v2.14.1

1. Fix setInsecure() bug for WIO_Terminal.
2. Update `Packages' Patches` for `Seeeduino` core. Be sure to use the `Packages_Patches` of this library version v2.14.1+ to avoid compiler error.

### Release v2.14.0

1. Suppress unnecessary warnings.
2. Don't use `deprecated sha.h` for ESP32 core v1.0.6+. Check [warning sha.h is deprecated, use sha_parallel_engine.h #738](https://github.com/Links2004/arduinoWebSockets/issues/738)
3. Optimize code by passing by `reference` instead of `value`
4. Modify nearly all examples
5. Update `Packages' Patches`

### Release v2.13.0

1. Add support to `ESP32_S3`.
2. Add `sIOtype_PING` and `sIOtype_PING` SocketIO events. Check [Ping events propagated to main code socketIO events #18](https://github.com/khoih-prog/WebSockets_Generic/issues/18)
3. Add [ESP32_WebSocketClientSocketIO](examples/ESP32_WebSocketClientSocketIO) example
4. Update `Packages' Patches`

### Major Release v2.12.0

1. Add SSL support to ESP32-based WT32_ETH01 boards using LAN8720 Ethernet
2. Add WT32_ETH01-related [WT32_ETH01_SSL_SIO example](examples/WT32_ETH01/WT32_ETH01_SSL_SIO)
3. Update `Packages' Patches`


### Release v2.11.1

1. Add option to use `transport=websocket` with sticky-session SIO server. Check [Run websocket only mode for Socket.IO #16](https://github.com/khoih-prog/WebSockets_Generic/issues/16)
2. Add some Sticky_SIO-related examples

### Major Release v2.11.0

1. Auto detect ESP32 core to use for WT32_ETH01
2. Fix bug in WT32_ETH01 examples to reduce connection time
3. Delete Blynk-related examples
4. Replace deprecated `echo.websocket.org` in examples.

### Release v2.10.1

1. Update `platform.ini` and `library.json` to use original `khoih-prog` instead of `khoih.prog` after PIO fix
2. Update `Packages' Patches`

### Major Release v2.10.0

1. Add support to **Portenta_H7**, using either WiFi or Vision-shield Ethernet
2. Add examples with new features

### Major Release v2.9.0

1. Add support to [QNEthernet Library](https://github.com/ssilverman/QNEthernet) for Teensy 4.1 built-in Ethernet
2. Update examples with new features

### Major Release v2.8.0

1. Add support to WT32_ETH01 (ESP32 + LAN8720) boards
2. Add examples with new features

### Major Release v2.7.0

1. Add support to RP2040-based boards, such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Earle Philhower's arduino-pico** v1.6.2+ core](https://github.com/earlephilhower/arduino-pico).
2. Add support to RP2040-based boards, such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Arduino-mbed RP2040** v2.1.0+ core](https://github.com/arduino/ArduinoCore-mbed).
3. Update Socket.IO Test Server to demonstrate the server message broadcast
4. Update examples with new features
5. Verify working with new ESP8266 core v3.0.0
6. Suppress all warnings possible.

### Major Release v2.6.0

1. Fix breaking problem with SocketIO. 
2. Add setExtraHeaders function to SocketIO
3. Clean possible compiler's warnings

### Release v2.5.1

1. Default to **EIO4 for Socket.IO**.
2. Permit changing reconnectInterval in Socket.IO. Default is 0.5s. Set in Socket.IO examples to 10s.
2. Better debug settings

### Major Release v2.5.0

1. Add support to SAMD_MKR1000 and SAMD_MKRWIFI1010 using WiFi101.  To be used with [**Forked WiFi101** library](https://github.com/khoih-prog/WiFi101)
2. Clean compiler's warnings
3. Add examples for WiFi101

### Release v2.4.1

1. Sync with v2.3.5 of original WebSockets library to adapt to ESP32 SSL changes
2. Update Packages' and Libraries' Patches 
3. Add `Bug Report` and `Feature Request` templates

### Major Release v2.4.0

1. Add support to Teensy 4.1 using NativeEthernet.
2. Add support to STM32F/L/H/G/WB/MP1 using built-in LAN8742A.
3. Sync with v2.3.4 of original WebSockets library
4. Add Teensy, Teensy 4.1 NativeEthernet and STM32 LAN8742A examples.
5. Fix bugs in and optimize examples.

### Release v2.3.4

1. Add SSL support to boards using **WiFiNINA (Nano-33-IoT, etc.)**
2. Add SSL support to boards using **Realtek RTL8720DN WiFi and Seeed_Arduino_rpcWiFi (WIO_Terminal, etc.)**
3. Upgrade WS and WSS examples.

### Release v2.3.3

1. Fix compile errors for boards (nRF52, SAMD, etc.) using libraries with lib64, such as [**EthernetWebServer**](https://github.com/khoih-prog/EthernetWebServer)
2. Fix SeeedStudio **SEEED_WIO_TERMINAL** compile errors. See [**Fix compile error for Wio Terminal**](https://github.com/khoih-prog/WebSockets_Generic/pull/5)
3. Add file to SeeedStudio SAMD Packages' Patches.
4. Fix compiler warnings for duplications in WS_LOG with [WiFiWebServer Library](https://github.com/khoih-prog/WiFiWebServer)


### Release v2.3.2

1. Add support to **Realtek RTL8720DN WiFi** using [**Seeed_Arduino_rpcWiFi**](https://github.com/Seeed-Studio/Seeed_Arduino_rpcWiFi) and [**Seeed_Arduino_rpcUnified**](https://github.com/khoih-prog/Seeed_Arduino_rpcUnified) libraries. This RTL8720DN supports Dual-Band 2.4GHz / 5GHz Wi-Fi (802.11 a/b/g/n) as well as BLE/BLE 5.0 and is currently used in SeeedStudio **SEEED_WIO_TERMINAL**.
2. Add examples for SeeedStudio **SEEED_WIO_TERMINAL** and update all examples
3. Add Version string definition

### Release v2.3.1

1. Sync with v2.3.1 of original [**WebSockets library**](https://github.com/Links2004/arduinoWebSockets). 
2. Add support to ENC28J60 using [**EthernetENC library**](https://github.com/jandrassy/EthernetENC).
3. Add and update examples

#### Release v2.2.3

1. Add support to all **STM32F/L/H/G/WB/MP1 (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.)**
2. Add support to **Seeeduino SAMD21/SAMD51 boards (SEEED_WIO_TERMINAL, SEEED_FEMTO_M0, SEEED_XIAO_M0, Wio_Lite_MG126, WIO_GPS_BOARD, SEEEDUINO_ZERO, SEEEDUINO_LORAWAN, SEEED_GROVE_UI_WIRELESS, etc.)**
3. Add support to **W5x00's Ethernet2, Ethernet3, EthernetLarge Libraries.**
4. Add sample Packages_Patches for **STM32 stm32** (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8)
5. Add Packages' Patches and Ethernet Library Patches.

#### Release v2.2.2

1. Add support to **Teensy** boards, such as **Teensy 4.1, 4.0. 3.6, 3.5, 3.2/3.1, 3.0, LC.**.
2. Add support to **STM32 (Nucleo-144, Nucleo-64, Nucleo-32, Discovery, STM32F1, STM32F3, STM32F4, STM32H7, STM32L0, etc.)**.
3. Add support to **SAM DUE**
4. Add WebSocketServer examples to those supported boards.

#### Release v2.2.1

1. Bump up to sync with v2.2.1 of original WebSockets library

#### Release v2.1.3

1. Add support to **nRF52** boards, such as **AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, etc.**.
2. Add support to **SAMD51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.)**.
3. Add support to **SAMD21 (ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit CIRCUITPLAYGROUND_EXPRESS, etc.)**.


---
---

### Important Notes

#### Originally Supported Hardware

 - ESP8266 [Arduino for ESP8266](https://github.com/esp8266/Arduino/)
 - ESP32 [Arduino for ESP32](https://github.com/espressif/arduino-esp32)
 - ESP31B
 - Particle with STM32 ARM Cortex M3
 - ATmega328 with Ethernet Shield (ATmega branch)
 - ATmega328 with ENC28J60 (ATmega branch)
 - ATmega2560 with Ethernet Shield (ATmega branch)
 - ATmega2560 with ENC28J60 (ATmega branch)

---

#### New support from v2.10.0

1. **Portenta_H7**, using either WiFi or Vision-shield Ethernet

#### New support from v2.9.0

1. Teensy 4.1 built-in Ethernet using [QNEthernet Library](https://github.com/ssilverman/QNEthernet) 

#### New support from v2.8.0

1. **WT32_ETH01 boards** using ESP32-based boards and LAN8720 Ethernet

#### New support from v2.7.0

1. RP2040-based boards, such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Earle Philhower's arduino-pico core**](https://github.com/earlephilhower/arduino-pico).
2. RP2040-based boards, such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Arduino-mbed RP2040** core](https://github.com/arduino/ArduinoCore-mbed).

#### New support from v2.5.0

1. **SAMD_MKR1000 and SAMD_MKRWIFI1010** using WiFi101

#### New support from v2.4.0

1. **STM32F/L/H/G/WB/MP1 (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.)** using Built-in LAN8742A Ethernet
2. **Teensy 4.1** using NativeEthernet
3. **Teensy** using W5x00, ENC28J60 Ethernet, WiFiNINA

#### New support from v2.3.4

1. **SSL support** to WiFiNINA
2. **SSL support** to boards using **Realtek RTL8720DN WiFi and Seeed_Arduino_rpcWiFi (WIO_Terminal, etc.)**
3. **Teensy** using W5x00, ENC28J60 Ethernet, WiFiNINA

#### New support from v2.3.2

1. **Realtek RTL8720DN WiFi** using [**Seeed_Arduino_rpcWiFi**](https://github.com/Seeed-Studio/Seeed_Arduino_rpcWiFi) and [**Seeed_Arduino_rpcUnified**](https://github.com/khoih-prog/Seeed_Arduino_rpcUnified) libraries. This RTL8720DN supports Dual-Band 2.4GHz / 5GHz Wi-Fi (802.11 a/b/g/n) as well as BLE/BLE 5.0 and is currently used in SeeedStudio **SEEED_WIO_TERMINAL**.

#### New support from v2.3.1

1. **ENC28J60's EthernetENC Library.**

#### New support from v2.2.3

1. **STM32F/L/H/G/WB/MP1 (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.)**
2. **Seeeduino SAMD21/SAMD51 boards (SEEED_WIO_TERMINAL, SEEED_FEMTO_M0, SEEED_XIAO_M0, Wio_Lite_MG126, WIO_GPS_BOARD, SEEEDUINO_ZERO, SEEEDUINO_LORAWAN, SEEED_GROVE_UI_WIRELESS, etc.)**
3. **W5x00's Ethernet2, Ethernet3, EthernetLarge Libraries.**

#### New support from v2.2.2

1. **Teensy** boards, such as **Teensy 4.1, 4.0. 3.6, 3.5, 3.2/3.1, 3.0, LC.**.
2. **STM32 (Nucleo-144, Nucleo-64, Nucleo-32, Discovery, STM32F1, STM32F3, STM32F4, STM32H7, STM32L0, etc.)**.
3. **SAM DUE**
4. WebSocketServer for those supported boards.

#### New support from v2.1.3
 
1. **nRF52** boards, such as **AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B30_ublox, etc.**
2. **SAM51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.) and SAM DUE**.
3. **SAMD21 (ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit CIRCUITPLAYGROUND_EXPRESS, etc.)**

#### Note:

  Version 2.0.0 and up is not compatible with AVR/ATmega, check ATmega branch.
  
  Version 2.3.0 has API changes for the ESP8266 BearSSL (may break existing code)

  Arduino for AVR not supports std namespace of c++.
