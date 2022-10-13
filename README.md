## WebSockets_Generic

[![arduino-library-badge](https://www.ardu-badge.com/badge/WebSockets_Generic.svg?)](https://www.ardu-badge.com/WebSockets_Generic)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/WebSockets_Generic.svg)](https://github.com/khoih-prog/WebSockets_Generic/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/WebSockets_Generic/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/WebSockets_Generic.svg)](http://github.com/khoih-prog/WebSockets_Generic/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>

---
---

## Table of Contents

* [Important notes about Portenta_H7 Ethernet](#Important-notes-about-Portenta_H7-Ethernet)
* [Why do we need this WebSockets_Generic library](#why-do-we-need-this-websockets_generic-library)
  * [Features](#features)
  * [Supported features of RFC6455](#supported-features-of-rfc6455)
  * [Important Notes](#important-notes)
  * [Currently supported Boards](#currently-supported-boards)
  * [Currently supported WiFi shields/modules](#currently-supported-wifi-shieldsmodules)
  * [Currently supported Ethernet shields/modules](#currently-supported-ethernet-shieldsmodules)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Packages' Patches](#packages-patches)
  * [1. For Adafruit nRF52840 and nRF52832 boards](#1-for-adafruit-nRF52840-and-nRF52832-boards)
  * [2. For Teensy boards](#2-for-teensy-boards)
  * [3. For Arduino SAM DUE boards](#3-for-arduino-sam-due-boards)
  * [4. For Arduino SAMD boards](#4-for-arduino-samd-boards)
      * [For core version v1.8.10+](#for-core-version-v1810)
      * [For core version v1.8.9-](#for-core-version-v189-)
  * [5. For Adafruit SAMD boards](#5-for-adafruit-samd-boards)
  * [6. For Seeeduino SAMD boards](#6-for-seeeduino-samd-boards)
  * [7. For STM32 boards](#7-for-stm32-boards) 
    * [7.1. For STM32 boards to use LAN8720](#71-for-stm32-boards-to-use-lan8720)
    * [7.2. For STM32 boards to use Serial1](#72-for-stm32-boards-to-use-serial1)
  * [8. For RP2040-based boards using Earle Philhower arduino-pico core](#8-for-rp2040-based-boards-using-earle-philhower-arduino-pico-core)
    * [8.1. To use BOARD_NAME](#81-to-use-board_name)
    * [8.2. To avoid compile error relating to microsecondsToClockCycles](#82-to-avoid-compile-error-relating-to-microsecondstoclockcycles)
  * [9. For Portenta_H7 boards using Arduino IDE in Linux](#9-for-portenta_h7-boards-using-arduino-ide-in-linux)
  * [10. For RTL8720DN boards using AmebaD core](#10-for-rtl8720dn-boards-using-amebad-core)
* [Libraries' Patches](#libraries-patches)
  * [1. For application requiring 2K+ HTML page](#1-for-application-requiring-2k-html-page)
  * [2. For Ethernet library](#2-for-ethernet-library)
  * [3. For EthernetLarge library](#3-for-ethernetlarge-library)
  * [4. For Etherne2 library](#4-for-ethernet2-library)
  * [5. For Ethernet3 library](#5-for-ethernet3-library)
  * [6. For UIPEthernet library](#6-for-uipethernet-library)
  * [7. For fixing ESP32 compile error](#7-for-fixing-esp32-compile-error)
  * [8. For fixing ESP8266 compile error](#8-for-fixing-esp8266-compile-error)
* [HOWTO Use analogRead() with ESP32 running WiFi and/or BlueTooth (BT/BLE)](#howto-use-analogread-with-esp32-running-wifi-andor-bluetooth-btble)
* [Important Notes](#important-notes-1)
  * [Limitations](#limitations)
  * [Limitations for Async](#limitations-for-async)
  * [Originally Supported Hardware](#originally-supported-hardware)
  * [wss / SSL](#wss--ssl)
  * [ESP Async TCP](#esp-async-tcp)
* [How to use](#how-to-use)
* [High Level Client API](#high-level-client-api)
* [Configure SIO_PING_INTERVAL, SIO_PONG_TIMEOUT and SIO_DISCONNECT_TIMEOUT_COUNT](#Configure-SIO_PING_INTERVAL-SIO_PONG_TIMEOUT-and-SIO_DISCONNECT_TIMEOUT_COUNT)
* [Examples](#examples)
  * [For Generic boards using W5x00 Ethernet shield](#for-generic-boards-using-w5x00-ethernet-shield)
  * [For Generic boards using WiFiNINA](#for-generic-boards-using-wifinina)
  * [For Generic boards using ENC28J60 Ethernet shield and EthernetENC library](#for-generic-boards-using-enc28j60-ethernet-shield-and-ethernetenc-library)
  * [For WiFiNINA](#for-wifinina)
  * [For WiFi101](#for-wifi101)
  * [For W5x00 Ethernet shield](#for-w5x00-ethernet-shield)
  * [For ENC28J60 Ethernet shield using UIPEthernet library](#for-enc28j60-ethernet-shield-using-uipethernet-library)
  * [For ESP32 board](#for-esp32-board)
  * [For ESP8266 board](#for-esp8266-board)
  * [For SeeedStudio WIO Terminal using Realtek RTL8720DN WiFi](#for-seeedstudio-wio-terminal-using-realtek-rtl8720dn-wifi) 
  * [For STM32 boards using built-in LAN8742A Ethernet](#for-stm32-boards-using-built-in-lan8742a-ethernet)
  * [For Teensy boards using ENC28J60 Ethernet shield and EthernetENC library](#for-teensy-boards-using-enc28j60-ethernet-shield-and-ethernetenc-library)
  * [For Teensy 4.1 boards using NativeEthernet](#for-teensy-41-boards-using-nativeethernet)
  * [For Teensy boards using W5x00 Ethernet shield](#for-teensy-boards-using-w5x00-ethernet-shield)
  * [For Teensy boards using WiFiNINA](#for-teensy-boards-using-wifinina)
  * [For Teensy 4.1 boards using QNEthernet](#for-teensy-41-boards-using-qnethernet)
  * [Nodejs Socket.IO Test Server to use with examples](#nodejs-socketio-test-server-to-use-with-examples)
  * [For WT32_ETH01 board](#for-wt32_eth01-board) **New**
  * [For Portenta_H7 board](#for-Portenta_H7-board). **New**
    * [Portenta_Ethernet](#Portenta_Ethernet).
    * [WiFi](#WiFi)
  * [For ESP8266 Async WebSocket](#For-ESP8266-Async-WebSocket) **New**
  * [For RP2040W boards using built-in WiFi](#For-RP2040W-boards-using-built-in-WiFi) **New**
* [Example Generic_WebSocketClient_WiFiNINA](#example-generic_websocketclient_wifinina)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. Generic_WebSocketClient_EthernetENC on NRF52840_FEATHER](#1-generic_websocketclient_ethernetenc-on-nrf52840_feather)
  * [2. Generic_Ethernet_Blinds on NRF52840_FEATHER with ENC28J60 using EthernetENC Library](#2-generic_ethernet_blinds-on-nrf52840_feather-with-enc28j60-using-ethernetenc-library)
  * [3. WebSocketClientSocketIO_W5500 on NRF52840_FEATHER with W5500 using Ethernet_Generic Library](#3-websocketclientsocketio_w5500-on-nrf52840_feather-with-w5500-using-Ethernet_Generic-library)
  * [4. Generic_WebSocketClientSocketIO_EthernetENC on NRF52840_FEATHER with ENC28J60 using EthernetENC Library](#4-generic_websocketclientsocketio_ethernetenc-on-nrf52840_feather-with-enc28j60-using-ethernetenc-library) 
  * [5. WIOTerminal_WebSocketClientSSL on SeeedStudio SAMD51 WIO_TERMINAL with Realtek RTL8720DN WiFi using Seeed_Arduino_rpcWiFi Library](#5-wioterminal_websocketclientssl-on-seeedstudio-samd51-wio_terminal-with-realtek-rtl8720dn-wifi-using-seeed_arduino_rpcwifi-library)
  * [6. Generic_WebSocketClientSSL_WiFiNINA on Arduino SAMD21 Nano-33-IoT with WiFiNINA using WiFiNINA_Generic Library](#6-generic_websocketclientssl_wifinina-on-arduino-samd21-nano-33-iot-with-wifinina-using-wifinina_generic-library)
  * [7. WebSocketClientSocketIO_NINA on Arduino SAMD21 Nano-33-IoT with WiFiNINA using WiFiNINA_Generic Library](#7-websocketclientsocketio_nina-on-arduino-samd21-nano-33-iot-with-wifinina-using-wifinina_generic-library)
    * [7.1 Client](#71-client)
    * [7.2 Server](#72-server)
  * [8. Generic_WebSocketClientSocketIO_W5500 on NRF52840_FEATHER with W5x00 using Ethernet_Generic Library](#8-generic_websocketclientsocketio_w5500-on-nrf52840_feather-with-w5x00-using-Ethernet_Generic-library)
    * [8.1 Client](#81-client)
    * [8.2 Server](#82-server)
  * [9. WebSocketClientSocketIO_W5500 on RASPBERRY_PI_PICO with W5x00 using Ethernet_Generic Library](#9-websocketclientsocketio_w5500-on-raspberry_pi_pico-with-w5x00-using-Ethernet_Generic-library)
    * [9.1 Client](#91-client)
    * [9.2 Server](#92-server)
  * [10. WebSocketClientSocketIO_W5500 on MBED RASPBERRY_PI_PICO with W5x00 using Ethernet_Generic Library](#10-websocketclientsocketio_w5500-on-mbed-raspberry_pi_pico-with-w5x00-using-Ethernet_Generic-library)
    * [10.1 Client](#101-client)
    * [10.2 Server](#102-server)
  * [11. WT32_ETH01_WebSocketClientSocketIO on WT32_ETH01](#11-wt32_eth01_websocketclientsocketio-on-wt32_eth01)
    * [11.1 Client](#111-client)
    * [11.2 Server](#112-server)
  * [12. Teensy_WebSocketClientSocketIO_QNEthernet on TEENSY 4.1 using QNEthernet](#12-teensy_websocketclientsocketio_qnethernet-on-teensy-41-using-qnethernet)
    * [12.1 Client](#121-client)
    * [12.2 Server](#122-server)
  * [13. Portenta_H7_WebSocketClientSocketIO on PORTENTA_H7_M7 with Ethernet](#13-Portenta_H7_WebSocketClientSocketIO-on-PORTENTA_H7_M7-with-Ethernet)
    * [13.1 Client](#131-client)
    * [13.2 Server](#132-server)
  * [14. WebSocketClientSocketIO_WiFi on Portenta_H7 using WiFi](#14-WebSocketClientSocketIO_WiFi-on-Portenta_H7-using-WiFi)
    * [14.1 Client](#141-client)
    * [14.2 Server](#142-server)
  * [15. Portenta_H7_WebSocketClient_Sticky_SocketIO on PORTENTA_H7_M7 using Ethernet](#15-Portenta_H7_WebSocketClient_Sticky_SocketIO-on-PORTENTA_H7_M7-using-Ethernet)
  * [16. ESP32_WebSocketClientSocketIO on ESP32S3_DEV](#16-ESP32_WebSocketClientSocketIO-on-ESP32S3_DEV)
  * [17. ESP8266_AsyncWSClient on ESP8266_NODEMCU_ESP12E](#17-ESP8266_AsyncWSClient-on-ESP8266_NODEMCU_ESP12E)
  * [18. WebSocketClientSocketIO_RP2040W on RP2040W using WiFi](#18-WebSocketClientSocketIO_RP2040W-on-RP2040W-using-WiFi) **New**
  * [19. Generic_WebSocketClientSocketIO_WiFiNINA on Nano_RP2040_Connect using WiFiNINA](#19-Generic_WebSocketClientSocketIO_WiFiNINA-on-Nano_RP2040_Connect-using-WiFiNINA) **New**
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License and credits](#license-and-credits)
* [Copyright](#copyright)

---
---

### Important notes about Portenta_H7 Ethernet

**Portenta_H7 Ethernet** doesn't work in mbed_portenta core v3.0.0, and has been fixed in core v3.0.1. 
Check [[Portenta_H7] Ethernet not working in core v3.0.0 #436](https://github.com/arduino/ArduinoCore-mbed/issues/436).

Please use mbed_portenta core **v2.8.0- or v3.0.1+ for Portenta_H7 Ethernet**.

---
---


### Why do we need this [WebSockets_Generic library](https://github.com/khoih-prog/WebSockets_Generic)

#### Features

Many Web services require WebSockets library, which is so far written only for ESP8266/ESP32 boards. The ESP boards rely on this [Markus Sattler's **WebSockets Library**](https://github.com/Links2004/arduinoWebSockets) to connect to Alexa via Sinric or SinricPro skills.

This [WebSockets_Generic library](https://github.com/khoih-prog/WebSockets_Generic) is a RFC6455-based WebSocket Server and Client for Arduino boards. It is based on and modified from [Markus Sattler's **WebSockets Library**](https://github.com/Links2004/arduinoWebSockets) to provide support to many more boards, such as **WT32_ETH01 (ESP32 + LAN8720), Arduino SAMD21, Adafruit SAMD21/SAMD51, nRF52, STM32F/L/H/G/WB/MP1, Teensy, SAM DUE, RP2040-based boards, etc.** and enable those boards to use WebSockets services, including voice-control Alexa along with Blynk. Those supported boards can also run **WebSockets Server.** The WebSockets can be used with **ESP’s WiFi, WiFiNINA, WiFi101, W5x00, ENC28J60 Ethernet and Teensy 4.1 NativeEthernet/QNEthernet.**


#### Supported features of RFC6455

 - text frame
 - binary frame
 - connection close
 - ping
 - pong
 - continuation frame

#### Important Notes

1. Currently, the WebSocketServer feature is usable only for ESP8266/ESP32 (ESP32, ESP32_S2, ESP32_S3 and ESP32_C3) and WT32_ETH01 (ESP32 + LAN8720).
2. The timeline when to fix and reintroduce the WebSocketServer feature to other boards is not determined yet.
3. Please use the new [**WebSockets2_Generic Library**](https://github.com/khoih-prog/WebSockets2_Generic) if WebSocketServer is necessary. See [Issue 2](https://github.com/khoih-prog/WebSockets_Generic/issues/2), [Issue 3](https://github.com/khoih-prog/WebSockets_Generic/issues/3) and [Issue 4](https://github.com/khoih-prog/WebSockets_Generic/issues/4)

---


#### Currently supported Boards

This [**WebSockets_Generic** library](https://github.com/khoih-prog/WebSockets_Generic) currently supports these following boards:

 1. **nRF52 boards**, such as **AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox, etc.**
 
 2. **SAMD21**
  - Arduino SAMD21: ZERO, MKRs, NANO_33_IOT, etc.
  - Adafruit SAMD21 (M0): ItsyBitsy M0, Feather M0, Feather M0 Express, Metro M0 Express, Circuit Playground Express, Trinket M0, PIRkey, Hallowing M0, Crickit M0, etc.
  - Seeeduino:  LoRaWAN, Zero, Femto M0, XIAO M0, Wio GPS Board, etc.
  
 3. **SAMD51**
  - Adafruit SAMD51 (M4): Metro M4, Grand Central M4, ItsyBitsy M4, Feather M4 Express, Trellis M4, Metro M4 AirLift Lite, MONSTER M4SK Express, Hallowing M4, etc.
  - Seeeduino: Wio Terminal, Grove UI Wireless
  
 4. **SAM DUE** 
 5. **Teensy (4.1, 4.0, 3.6, 3.5, 3,2, 3.1, 3.0)**
 6. **STM32F/L/H/G/WB/MP1 boards (with 32+K Flash)**
  - Nucleo-144
  - Nucleo-64
  - Discovery
  - Generic STM32F0, STM32F1, STM32F2, STM32F3, STM32F4, STM32F7 (with 64+K Flash): x8 and up
  - STM32L0, STM32L1, STM32L4
  - STM32G0, STM32G4
  - STM32H7
  - STM32WB
  - STM32MP1
  - LoRa boards
  - 3-D printer boards
  - Generic Flight Controllers
  - Midatronics boards

 7. **ESP32**
 
  - ESP32 boards, such as `ESP32_DEV`, etc.
  - ESP32_S2-based boards, such as `ESP32S2_DEV`, `ESP32_S2 Saola`, etc.
  - ESP32_C3-based boards, such as `ESP32C3_DEV`, etc. **New**
  - ESP32_S3 (ESP32S3_DEV, ESP32_S3_BOX, UM TINYS3, UM PROS3, UM FEATHERS3, etc.) **New**

 8. **ESP8266**
 
 9. RP2040-based boards, such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Arduino-mbed RP2040** v2.1.0+ core](https://github.com/arduino/ArduinoCore-mbed) or [**Earle Philhower's arduino-pico** core v1.5.1+](https://github.com/earlephilhower/arduino-pico).

10. **WT32_ETH01 boards** using ESP32-based boards and LAN8720 Ethernet

11. **Portenta_H7** using either `Murata WiFi` or `Vision-shield Ethernet`

12. **RP2040-based boards, such as Nano_RP2040_Connect, RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040**, using WiFiNINA or Ethernet

13. **RP2040W boards, such as RASPBERRY_PI_PICO_W**, using Cyw43439 WiFi or Ethernet

---

#### Currently supported WiFi shields/modules

1. WiFiNINA using [`WiFiNINA_Generic library`](https://github.com/khoih-prog/WiFiNINA_Generic)
2. WiFi101 using [`WiFi101 library v0.16.1+`](https://github.com/arduino-libraries/WiFi101)
3. U-Blox W101, W102 using [`WiFiNINA_Generic library`](https://github.com/khoih-prog/WiFiNINA_Generic)
4. ESP8266-AT command using [`WiFiEspAT library`](https://github.com/jandrassy/WiFiEspAT)
5. ESP8266/ESP32-AT command using [`ESP_AT_Lib library`](https://github.com/khoih-prog/ESP_AT_Lib) and [`ESP8266_AT_WebServer`](ESP8266_AT_WebServer)
6. ESP32 and ESP8266 WiFi
7. **Portenta_H7 built-in Murata WiFi**
8. **RP2040W built-in CYW43439 WiFi**

---

#### Currently supported Ethernet shields/modules

1. W5x00 using [`Ethernet_Generic`](https://github.com/khoih-prog/Ethernet_Generic) library
2. W5x00 using [`Ethernet`](https://www.arduino.cc/en/Reference/Ethernet), [`EthernetLarge`](https://github.com/OPEnSLab-OSU/EthernetLarge), [`Ethernet2`](https://github.com/adafruit/Ethernet2) or [`Ethernet3`](https://github.com/sstaub/Ethernet3) library
3. ENC28J60 using [`EthernetENC`](https://github.com/jandrassy/EthernetENC) or [`UIPEthernet`](https://github.com/UIPEthernet/UIPEthernet) library
4. LAN8720 Ethernet used in `WT32_ETH01` boards
5. `LAN8742A or LAN8720` using [`STM32Ethernet`](https://github.com/stm32duino/STM32Ethernet) and [`LwIP`](https://github.com/stm32duino/LwIP) libraries.
6. `Teensy 4.1` built-in Ethernet using [`NativeEthernet`](https://github.com/vjmuzik/NativeEthernet) library
7. `Teensy 4.1` built-in Ethernet using [`QNEthernet`](https://github.com/ssilverman/QNEthernet) library
8. Portenta_H7 using Ethernet from [Portenta Vision shields](https://store-usa.arduino.cc/products/arduino-portenta-vision-shield-ethernet)
  
<p align="center">
    <img src="https://github.com/khoih-prog/WebSockets_Generic/blob/master/pics/Portenta_Vision.jpg">
</p>


---
---

 
## Prerequisites

 1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
 2. [`Arduino AVR core 1.8.5+`](https://github.com/arduino/ArduinoCore-avr) for Arduino (Use Arduino Board Manager) AVR boards. [![GitHub release](https://img.shields.io/github/release/arduino/ArduinoCore-avr.svg)](https://github.com/arduino/ArduinoCore-avr/releases/latest)
 3. [`Teensy core v1.57+`](https://www.pjrc.com/teensy/td_download.html) for Teensy (4.1, 4.0, 3.6, 3.5, 3,2, 3.1, 3.0) boards.
 4. [`Arduino SAM DUE core v1.6.12+`](https://github.com/arduino/ArduinoCore-sam) for SAM DUE ARM Cortex-M3 boards.
 5. [`Arduino SAMD core 1.8.13+`](https://github.com/arduino/ArduinoCore-samd) for SAMD ARM Cortex-M0+ boards. [![GitHub release](https://img.shields.io/github/release/arduino/ArduinoCore-samd.svg)](https://github.com/arduino/ArduinoCore-samd/releases/latest)
 6. [`Adafruit SAMD core 1.7.10+`](https://github.com/adafruit/ArduinoCore-samd) for SAMD ARM Cortex-M0+ and M4 boards (Nano 33 IoT, etc.). [![GitHub release](https://img.shields.io/github/release/adafruit/ArduinoCore-samd.svg)](https://github.com/adafruit/ArduinoCore-samd/releases/latest)
 7. [`Seeeduino SAMD core 1.8.3+`](https://github.com/Seeed-Studio/ArduinoCore-samd) for SAMD21/SAMD51 boards (XIAO M0, Wio Terminal, etc.). [![Latest release](https://img.shields.io/github/release/Seeed-Studio/ArduinoCore-samd.svg)](https://github.com/Seeed-Studio/ArduinoCore-samd/releases/latest/)
 8. [`Adafruit nRF52 v1.3.0+`](https://github.com/adafruit/Adafruit_nRF52_Arduino) for nRF52 boards such as Adafruit NRF52840_FEATHER, NRF52832_FEATHER, NRF52840_FEATHER_SENSE, NRF52840_ITSYBITSY, NRF52840_CIRCUITPLAY, NRF52840_CLUE, NRF52840_METRO, NRF52840_PCA10056, PARTICLE_XENON, **NINA_B302_ublox**, etc. [![GitHub release](https://img.shields.io/github/release/adafruit/Adafruit_nRF52_Arduino.svg)](https://github.com/adafruit/Adafruit_nRF52_Arduino/releases/latest)
 9. [`ESP32 Core 2.0.5+`](https://github.com/espressif/arduino-esp32) for ESP32-based boards. [![Latest release](https://img.shields.io/github/release/espressif/arduino-esp32.svg)](https://github.com/espressif/arduino-esp32/releases/latest/)
10. [`ESP8266 Core 3.0.2+`](https://github.com/esp8266/Arduino) for ESP8266-based boards. [![Latest release](https://img.shields.io/github/release/esp8266/Arduino.svg)](https://github.com/esp8266/Arduino/releases/latest/). To use ESP8266 core 2.7.1+ for LittleFS.
11. [`ArduinoCore-mbed mbed_rp2040, mbed_nano, mbed_portenta core 3.3.0+`](https://github.com/arduino/ArduinoCore-mbed) for Arduino (Use Arduino Board Manager) **Portenta_H7, RP2040-based boards, such as Nano_RP2040_Connect, RASPBERRY_PI_PICO**. [![GitHub release](https://img.shields.io/github/release/arduino/ArduinoCore-mbed.svg)](https://github.com/arduino/ArduinoCore-mbed/releases/latest)
12. [`Earle Philhower's arduino-pico core v2.6.0+`](https://github.com/earlephilhower/arduino-pico) for RP2040-based boards such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, etc. [![GitHub release](https://img.shields.io/github/release/earlephilhower/arduino-pico.svg)](https://github.com/earlephilhower/arduino-pico/releases/latest)
13. [`Arduino Core for STM32 v2.3.0+`](https://github.com/stm32duino/Arduino_Core_STM32) for STM32F/L/H/G/WB/MP1 boards. [![GitHub release](https://img.shields.io/github/release/stm32duino/Arduino_Core_STM32.svg)](https://github.com/stm32duino/Arduino_Core_STM32/releases/latest). Use STM32 core `v2.2.0` for LAN8720 or compile error.
14. [`Arduino AmebaD core 3.1.4+`](https://github.com/ambiot/ambd_arduino) for Realtek RTL8720DN, RTL8722DM and RTL8722CSM. [![GitHub release](https://img.shields.io/github/release/ambiot/ambd_arduino.svg)](https://github.com/ambiot/ambd_arduino/releases/latest)


15. [`WiFiNINA_Generic library v1.8.14-6+`](https://github.com/khoih-prog/WiFiNINA_Generic) if for WiFiNINA. To install. check [![arduino-library-badge](https://www.ardu-badge.com/badge/WiFiNINA_Generic.svg?)](https://www.ardu-badge.com/WiFiNINA_Generic).
16. [`EthernetWebServer library v2.2.3+`](https://github.com/khoih-prog/EthernetWebServer) if necessary to use Ethernet modules/shields. To install, check [![arduino-library-badge](https://www.ardu-badge.com/badge/EthernetWebServer.svg?)](https://www.ardu-badge.com/EthernetWebServer)
17. [`EthernetWebServer_STM32 library v1.5.0+`](https://github.com/khoih-prog/EthernetWebServer_STM32) if necessary to use Ethernet modules/shields. To install, check [![arduino-library-badge](https://www.ardu-badge.com/badge/EthernetWebServer_STM32.svg?)](https://www.ardu-badge.com/EthernetWebServer_STM32)
18. [`WebServer_WT32_ETH01 library v1.5.1+`](https://github.com/khoih-prog/WebServer_WT32_ETH01) if necessary to use WT32_ETH01 boards. To install, check [![arduino-library-badge](https://www.ardu-badge.com/badge/WebServer_WT32_ETH01.svg?)](https://www.ardu-badge.com/WebServer_WT32_ETH01)
19. Depending on which Ethernet card you're using:
   - [`Ethernet_Generic library v2.6.1+`](https://github.com/khoih-prog/Ethernet_Generic) for W5100, W5200 and W5500/WIZ550io/WIZ850io/USR-ES1 with Wiznet W5500 chip.  [![GitHub release](https://img.shields.io/github/release/khoih-prog/Ethernet_Generic.svg)](https://github.com/khoih-prog/Ethernet_Generic/releases/latest)
   - [`EthernetENC library v2.0.3+`](https://github.com/jandrassy/EthernetENC) for ENC28J60. [![GitHub release](https://img.shields.io/github/release/jandrassy/EthernetENC.svg)](https://github.com/jandrassy/EthernetENC/releases/latest). **New and Better**
   - [`UIPEthernet library v2.0.12+`](https://github.com/UIPEthernet/UIPEthernet) for ENC28J60. [![GitHub release](https://img.shields.io/github/release/UIPEthernet/UIPEthernet.svg)](https://github.com/UIPEthernet/UIPEthernet/releases/latest)
   - [`STM32Ethernet library v1.3.0+`](https://github.com/stm32duino/STM32Ethernet) for built-in LAN8742A Ethernet on (Nucleo-144, Discovery). [![GitHub release](https://img.shields.io/github/release/stm32duino/STM32Ethernet.svg)](https://github.com/stm32duino/STM32Ethernet/releases/latest). To be used with [`LwIP library v2.1.2+`](https://github.com/stm32duino/LwIP). [![GitHub release](https://img.shields.io/github/release/stm32duino/LwIP.svg)](https://github.com/stm32duino/LwIP/releases/latest).
   - [`NativeEthernet Library version stable111+`](https://github.com/vjmuzik/NativeEthernet) for Teensy 4.1 built-in Ethernet.
   - [`QNEthernet Library version v0.15.0+`](https://github.com/ssilverman/QNEthernet) Teensy 4.1 built-in Ethernet. **New**
   
20. [`WiFiWebServer library v1.9.5+`](https://github.com/khoih-prog/WiFiWebServer) if necessary to use certain WiFi/WiFiNINA features. To install, check [![arduino-library-badge](https://www.ardu-badge.com/badge/WiFiWebServer.svg?)](https://www.ardu-badge.com/WiFiWebServer)
21. [`FlashStorage_SAMD library v1.3.2+`](https://github.com/khoih-prog/FlashStorage_SAMD) for SAMD21 and SAMD51 boards (ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit Itsy-Bitsy M4, etc.) if necessary to use certain features. [![GitHub release](https://img.shields.io/github/release/khoih-prog/FlashStorage_SAMD.svg)](https://github.com/khoih-prog/FlashStorage_SAMD/releases/latest)
22. [`FlashStorage_STM32 library v1.2.0+`](https://github.com/khoih-prog/FlashStorage_STM32) for STM32F/L/H/G/WB/MP1 boards. [![GitHub release](https://img.shields.io/github/release/khoih-prog/FlashStorage_STM32.svg)](https://github.com/khoih-prog/FlashStorage_STM32/releases/latest) if necessary to use certain features.
23. [`DueFlashStorage library v1.0.0+`](https://github.com/sebnil/DueFlashStorage) for SAM DUE if necessary to use certain features. To install, check [![arduino-library-badge](https://www.ardu-badge.com/badge/DueFlashStorage.svg?)](https://www.ardu-badge.com/DueFlashStorage).
24. [`Adafruit's LittleFS/InternalFS`](https://www.adafruit.com) for nRF52. Already included if you already installed Adafruit **nRF52 board package** from Boards Manager.
25. [`DoubleResetDetector_Generic v1.8.1+`](https://github.com/khoih-prog/DoubleResetDetector_Generic) if necessary to use some examples. To install. check [![arduino-library-badge](https://www.ardu-badge.com/badge/DoubleResetDetector_Generic.svg?)](https://www.ardu-badge.com/DoubleResetDetector_Generic) if necessary to use certain features.
26. [`Modified WiFi101 Library v0.16.1+`](https://github.com/khoih-prog/WiFi101) to use SAMD MKR1000, etc. boards with WiFi101.
27. [`SinricPro_Generic v2.8.5+`](https://github.com/khoih-prog/SinricPro_Generic) if necessary to use some examples. To install. check [![arduino-library-badge](https://www.ardu-badge.com/badge/SinricPro_Generic.svg?)](https://www.ardu-badge.com/SinricPro_Generic) if necessary to use certain features.
28. [`ESPAsyncTCP v1.2.2+`](https://github.com/me-no-dev/ESPAsyncTCP) for ESP8266 Async.

---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**WebSockets_Generic**](https://github.com/khoih-prog/WebSockets_Generic), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/WebSockets_Generic.svg?)](https://www.ardu-badge.com/WebSockets_Generic) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**WebSockets_Generic**](https://github.com/khoih-prog/WebSockets_Generic) page.
2. Download the latest release `WebSockets_Generic-master.zip`.
3. Extract the zip file to `WebSockets_Generic-master` directory 
4. Copy whole `WebSockets_Generic-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO:

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**WebSockets_Generic** library](https://registry.platformio.org/libraries/khoih-prog/WebSockets_Generic) by using [**Library Manager**](https://registry.platformio.org/libraries/khoih-prog/WebSockets_Generic/installation). Search for **WebSockets_Generic** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

### Packages' Patches

#### 1. For Adafruit nRF52840 and nRF52832 boards

**To be able to compile, run and automatically detect and display BOARD_NAME on nRF52840/nRF52832 boards**, you have to copy the whole [nRF52 Packages_Patches](Packages_Patches/adafruit/hardware/nrf52/1.3.0) directory into Adafruit nRF52 directory (~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0). 

Supposing the Adafruit nRF52 version is 1.3.0. These files must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/cores/nRF5/Udp.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/cores/nRF5/Print.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/cores/nRF5/Print.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/variants/NINA_B112_ublox/variant.cpp`
- **`~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/cores/nRF5/Udp.h`**

Whenever a new version is installed, remember to copy these files into the new version directory. For example, new version is x.yy.z
These files must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Udp.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Print.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Print.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.cpp`
- **`~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Udp.h`**

#### 2. For Teensy boards
 
 **To be able to compile and run on Teensy boards**, you have to copy the files in [**Packages_Patches for Teensy directory**](Packages_Patches/hardware/teensy/avr) into Teensy hardware directory (./arduino-1.8.19/hardware/teensy/avr/boards.txt). 

Supposing the Arduino version is 1.8.19. These files must be copied into the directory:

- `./arduino-1.8.19/hardware/teensy/avr/boards.txt`
- `./arduino-1.8.19/hardware/teensy/avr/cores/teensy/Stream.h`
- `./arduino-1.8.19/hardware/teensy/avr/cores/teensy3/Stream.h`
- `./arduino-1.8.19/hardware/teensy/avr/cores/teensy4/Stream.h`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
These files must be copied into the directory:

- `./arduino-x.yy.zz/hardware/teensy/avr/boards.txt`
- `./arduino-x.yy.zz/hardware/teensy/avr/cores/teensy/Stream.h`
- `./arduino-x.yy.zz/hardware/teensy/avr/cores/teensy3/Stream.h`
- `./arduino-x.yy.zz/hardware/teensy/avr/cores/teensy4/Stream.h`

#### 3. For Arduino SAM DUE boards
 
 **To be able to compile and run on SAM DUE boards**, you have to copy the whole [SAM DUE](Packages_Patches/arduino/hardware/sam/1.6.12) directory into Arduino sam directory (~/.arduino15/packages/arduino/hardware/sam/1.6.12). 

Supposing the Arduino SAM core version is 1.6.12. This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/sam/1.6.12/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/sam/x.yy.zz/platform.txt`

#### 4. For Arduino SAMD boards
 
 ***To be able to compile, run and automatically detect and display BOARD_NAME on Arduino SAMD (Nano-33-IoT, etc) boards***, you have to copy the whole [Arduino SAMD Packages_Patches](Packages_Patches/arduino/hardware/samd/1.8.13) directory into Arduino SAMD directory (~/.arduino15/packages/arduino/hardware/samd/1.8.13).
 
#### For core version v1.8.10+

Supposing the Arduino SAMD version is 1.8.13. Now only one file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/1.8.13/platform.txt`

Whenever a new version is installed, remember to copy this files into the new version directory. For example, new version is x.yy.zz

This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/x.yy.zz/platform.txt`
 
#### For core version v1.8.9-

Supposing the Arduino SAMD version is 1.8.9. These files must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/1.8.9/platform.txt`
- ***`~/.arduino15/packages/arduino/hardware/samd/1.8.9/cores/arduino/Arduino.h`***

Whenever a new version is installed, remember to copy these files into the new version directory. For example, new version is x.yy.z

These files must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/x.yy.z/platform.txt`
- ***`~/.arduino15/packages/arduino/hardware/samd/x.yy.z/cores/arduino/Arduino.h`***
 
 This is mandatory to fix the ***notorious Arduino SAMD compiler error***. See [Improve Arduino compatibility with the STL (min and max macro)](https://github.com/arduino/ArduinoCore-samd/pull/399)
 
```
 ...\arm-none-eabi\include\c++\7.2.1\bits\stl_algobase.h:243:56: error: macro "min" passed 3 arguments, but takes just 2
     min(const _Tp& __a, const _Tp& __b, _Compare __comp)
```

Whenever the above-mentioned compiler error issue is fixed with the new Arduino SAMD release, you don't need to copy the `Arduino.h` file anymore.

#### 5. For Adafruit SAMD boards
 
 ***To be able to compile, run and automatically detect and display BOARD_NAME on Adafruit SAMD (Itsy-Bitsy M4, etc) boards***, you have to copy the whole [Adafruit SAMD Packages_Patches](Packages_Patches/adafruit/hardware/samd/1.7.10) directory into Adafruit samd directory (~/.arduino15/packages/adafruit/hardware/samd/1.7.10). 

Supposing the Adafruit SAMD core version is 1.7.10. This file must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/samd/1.7.10/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/samd/1.7.10/cores/arduino/Print.h`
- `~/.arduino15/packages/adafruit/hardware/samd/1.7.10/cores/arduino/Print.cpp`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/samd/x.yy.zz/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/samd/x.yy.zz/cores/arduino/Print.h`
- `~/.arduino15/packages/adafruit/hardware/samd/x.yy.zz/cores/arduino/Print.cpp`

#### 6. For Seeeduino SAMD boards
 
 ***To be able to compile, run and automatically detect and display BOARD_NAME on Seeeduino SAMD (XIAO M0, Wio Terminal, etc) boards***, you have to copy the whole [Seeeduino SAMD Packages_Patches](Packages_Patches/Seeeduino/hardware/samd/1.8.3) directory into Seeeduino samd directory (~/.arduino15/packages/Seeeduino/hardware/samd/1.8.3). 

Supposing the Seeeduino SAMD core version is 1.8.3. This file must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.3/platform.txt`
- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.3/cores/arduino/Arduino.h`
- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.3/cores/arduino/Print.h`
- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.3/cores/arduino/Print.cpp`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/platform.txt`
- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/cores/arduino/Arduino.h`
- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/cores/arduino/Print.h`
- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/cores/arduino/Print.cpp`

#### 7. For STM32 boards

#### 7.1 For STM32 boards to use LAN8720

To use LAN8720 on some STM32 boards 

- **Nucleo-144 (F429ZI, NUCLEO_F746NG, NUCLEO_F746ZG, NUCLEO_F756ZG)**
- **Discovery (DISCO_F746NG)**
- **STM32F4 boards (BLACK_F407VE, BLACK_F407VG, BLACK_F407ZE, BLACK_F407ZG, BLACK_F407VE_Mini, DIYMORE_F407VGT, FK407M1)**

you have to copy the files [stm32f4xx_hal_conf_default.h](Packages_Patches/STM32/hardware/stm32/2.3.0/system/STM32F4xx) and [stm32f7xx_hal_conf_default.h](Packages_Patches/STM32/hardware/stm32/2.3.0/system/STM32F7xx) into STM32 stm32 directory (~/.arduino15/packages/STM32/hardware/stm32/2.3.0/system) to overwrite the old files.

Supposing the STM32 stm32 core version is 2.3.0. These files must be copied into the directory:

- `~/.arduino15/packages/STM32/hardware/stm32/2.3.0/system/STM32F4xx/stm32f4xx_hal_conf_default.h` for STM32F4.
- `~/.arduino15/packages/STM32/hardware/stm32/2.3.0/system/STM32F7xx/stm32f7xx_hal_conf_default.h` for Nucleo-144 STM32F7.

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz,
theses files must be copied into the corresponding directory:

- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/system/STM32F4xx/stm32f4xx_hal_conf_default.h`
- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/system/STM32F7xx/stm32f7xx_hal_conf_default.h


#### 7.2 For STM32 boards to use Serial1

**To use Serial1 on some STM32 boards without Serial1 definition (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.) boards**, you have to copy the files [STM32 variant.h](Packages_Patches/STM32/hardware/stm32/2.3.0) into STM32 stm32 directory (~/.arduino15/packages/STM32/hardware/stm32/2.3.0). You have to modify the files corresponding to your boards, this is just an illustration how to do.

Supposing the STM32 stm32 core version is 2.3.0. These files must be copied into the directory:

- `~/.arduino15/packages/STM32/hardware/stm32/2.3.0/variants/STM32F7xx/F765Z(G-I)T_F767Z(G-I)T_F777ZIT/NUCLEO_F767ZI/variant.h` for Nucleo-144 NUCLEO_F767ZI.
- `~/.arduino15/packages/STM32/hardware/stm32/2.3.0/variants/STM32L0xx/L052R(6-8)T_L053R(6-8)T_L063R8T/NUCLEO_L053R8/variant.h` for Nucleo-64 NUCLEO_L053R8.

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz,
theses files must be copied into the corresponding directory:

- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/variants/STM32F7xx/F765Z(G-I)T_F767Z(G-I)T_F777ZIT/NUCLEO_F767ZI/variant.h`
- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/variants/STM32L0xx/L052R(6-8)T_L053R(6-8)T_L063R8T/NUCLEO_L053R8/variant.h`

#### 8. For RP2040-based boards using [Earle Philhower arduino-pico core](https://github.com/earlephilhower/arduino-pico)

#### 8.1 To use BOARD_NAME

 **To be able to automatically detect and display BOARD_NAME on RP2040-based boards (RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040, GENERIC_RP2040, etc) boards**, you have to copy the file [RP2040 platform.txt](Packages_Patches/rp2040/hardware/rp2040/1.4.0) into rp2040 directory (~/.arduino15/packages/rp2040/hardware/rp2040/1.4.0). 

Supposing the rp2040 core version is 1.4.0. This file must be copied into the directory:

- `~/.arduino15/packages/rp2040/hardware/rp2040/1.4.0/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/rp2040/hardware/rp2040/x.yy.zz/platform.txt`

With core after v1.5.0, this step is not necessary anymore thanks to the PR [Add -DBOARD_NAME="{build.board}" #136](https://github.com/earlephilhower/arduino-pico/pull/136).

#### 8.2 To avoid compile error relating to microsecondsToClockCycles

Some libraries, such as [Adafruit DHT-sensor-library](https://github.com/adafruit/DHT-sensor-library), require the definition of microsecondsToClockCycles(). **To be able to compile and run on RP2040-based boards**, you have to copy the files in [**RP2040 Arduino.h**](Packages_Patches/rp2040/hardware/rp2040/1.4.0/cores/rp2040/Arduino.h) into rp2040 directory (~/.arduino15/packages/rp2040/hardware/rp2040/1.4.0).

Supposing the rp2040 core version is 1.4.0. This file must be copied to replace:

- `~/.arduino15/packages/rp2040/hardware/rp2040/1.4.0/cores/rp2040/Arduino.h`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied to replace:

- `~/.arduino15/packages/rp2040/hardware/rp2040/x.yy.zz/cores/rp2040/Arduino.h`

With core after v1.5.0, this step is not necessary anymore thanks to the PR [Add defs for compatibility #142](https://github.com/earlephilhower/arduino-pico/pull/142).


#### 9. For Portenta_H7 boards using Arduino IDE in Linux

  **To be able to upload firmware to Portenta_H7 using Arduino IDE in Linux (Ubuntu, etc.)**, you have to copy the file [portenta_post_install.sh](Packages_Patches/arduino/hardware/mbed_portenta/3.3.0/portenta_post_install.sh) into mbed_portenta directory (~/.arduino15/packages/arduino/hardware/mbed_portenta/3.3.0/portenta_post_install.sh). 
  
  Then run the following command using `sudo`
  
```
$ cd ~/.arduino15/packages/arduino/hardware/mbed_portenta/3.3.0
$ chmod 755 portenta_post_install.sh
$ sudo ./portenta_post_install.sh
```

This will create the file `/etc/udev/rules.d/49-portenta_h7.rules` as follows:

```
# Portenta H7 bootloader mode UDEV rules

SUBSYSTEMS=="usb", ATTRS{idVendor}=="2341", ATTRS{idProduct}=="035b", GROUP="plugdev", MODE="0666"
```

Supposing the ArduinoCore-mbed core version is 3.3.0. Now only one file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/mbed_portenta/3.3.0/portenta_post_install.sh`

Whenever a new version is installed, remember to copy this files into the new version directory. For example, new version is x.yy.zz

This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/mbed_portenta/x.yy.zz/portenta_post_install.sh`


#### 10. For RTL8720DN boards using AmebaD core
 
 To avoid compile error relating to PROGMEM, you have to copy the file [Realtek AmebaD core pgmspace.h](Packages_Patches/realtek/hardware/AmebaD/3.1.4/cores/ambd/avr/pgmspace.h) into Realtek AmebaD directory (~/.arduino15/packages/realtek/hardware/AmebaD/3.1.4/cores/ambd/avr/pgmspace.h). 

Supposing the Realtek AmebaD core version is 3.1.4. This file must be copied into the directory:

- `~/.arduino15/packages/realtek/hardware/AmebaD/3.1.4/cores/ambd/avr/pgmspace.h`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/realtek/hardware/AmebaD/x.yy.zz/cores/ambd/avr/pgmspace.h`


#### 11. For SAMD21 and SAMD51 boards using ArduinoCore-fab-sam core
 
 To avoid compile error relating to SAMD21/SAMD51, you have to copy the file [ArduinoCore-fab-sam core pgmspace.h](Packages_Patches/Fab_SAM_Arduino/hardware/samd/1.7.0/boards.txt) into `ArduinoCore-fab-sam` samd directory (~/.arduino15/packages/Fab_SAM_Arduino/hardware/samd/1.7.0/boards.txt). 

Supposing the `ArduinoCore-fab-sam` samd core version is 1.7.0. This file must be copied into the directory:

- `~/.arduino15/packages/Fab_SAM_Arduino/hardware/samd/1.7.0/boards.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/Fab_SAM_Arduino/hardware/samd/x.yy.zz/boards.txt`


#### 12. For Seeeduino RP2040 boards
 
 ***To be able to compile, run and automatically detect and display BOARD_NAME on Seeeduino RP2040 (XIAO RP2040, Wio RP2040 Mini) boards***, you have to copy the whole [Seeeduino RP2040 Packages_Patches](Packages_Patches/Seeeduino/hardware/rp2040/2.7.2) directory into Seeeduino samd directory (~/.arduino15/packages/Seeeduino/hardware/rp2040/2.7.2). 

Supposing the Seeeduino SAMD core version is 2.7.2. This file must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/rp2040/2.7.2/boards.txt`
- `~/.arduino15/packages/Seeeduino/hardware/rp2040/2.7.2/variants/Seeed_XIAO_RP2040/pins_arduino.h`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/boards.txt`
- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/variants/Seeed_XIAO_RP2040/pins_arduino.h`


---
---

### Libraries' Patches

#### 1. For application requiring 2K+ HTML page

If your application requires 2K+ HTML page, the current [`Ethernet library`](https://www.arduino.cc/en/Reference/Ethernet) must be modified if you are using W5200/W5500 Ethernet shields. W5100 is not supported for 2K+ buffer. If you use boards requiring different CS/SS pin for W5x00 Ethernet shield, for example ESP32, ESP8266, nRF52, etc., you also have to modify the following libraries to be able to specify the CS/SS pin correctly.

#### 2. For Ethernet library

To fix [`Ethernet library`](https://www.arduino.cc/en/Reference/Ethernet), just copy these following files into the [`Ethernet library`](https://www.arduino.cc/en/Reference/Ethernet) directory to overwrite the old files:
- [Ethernet.h](LibraryPatches/Ethernet/src/Ethernet.h)
- [Ethernet.cpp](LibraryPatches/Ethernet/src/Ethernet.cpp)
- [EthernetServer.cpp](LibraryPatches/Ethernet/src/EthernetServer.cpp)
- [w5100.h](LibraryPatches/Ethernet/src/utility/w5100.h)
- [w5100.cpp](LibraryPatches/Ethernet/src/utility/w5100.cpp)

You can also use the forked and modified library at [Patched Ethernet](https://github.com/khoih-prog/Ethernet)

#### 3. For EthernetLarge library

To fix [`EthernetLarge library`](https://github.com/OPEnSLab-OSU/EthernetLarge), just copy these following files into the [`EthernetLarge library`](https://github.com/OPEnSLab-OSU/EthernetLarge) directory to overwrite the old files:
- [EthernetLarge.h](LibraryPatches/EthernetLarge/src/EthernetLarge.h)
- [EthernetLarge.cpp](LibraryPatches/EthernetLarge/src/EthernetLarge.cpp)
- [EthernetServer.cpp](LibraryPatches/EthernetLarge/src/EthernetServer.cpp)
- [w5100.h](LibraryPatches/EthernetLarge/src/utility/w5100.h)
- [w5100.cpp](LibraryPatches/EthernetLarge/src/utility/w5100.cpp)

You can also use the forked and modified library at [Patched EthernetLarge](https://github.com/khoih-prog/EthernetLarge)

#### 4. For Ethernet2 library

To fix [`Ethernet2 library`](https://github.com/khoih-prog/Ethernet2), just copy these following files into the [`Ethernet2 library`](https://github.com/khoih-prog/Ethernet2) directory to overwrite the old files:

- [Ethernet2.h](LibraryPatches/Ethernet2/src/Ethernet2.h)
- [Ethernet2.cpp](LibraryPatches/Ethernet2/src/Ethernet2.cpp)

To add UDP Multicast support, necessary for the [**UPnP_Generic library**](https://github.com/khoih-prog/UPnP_Generic):

- [EthernetUdp2.h](LibraryPatches/Ethernet2/src/EthernetUdp2.h)
- [EthernetUdp2.cpp](LibraryPatches/Ethernet2/src/EthernetUdp2.cpp)

You can also use the forked and modified library at [Patched Ethernet2](https://github.com/khoih-prog/Ethernet2)

#### 5. For Ethernet3 library

5. To fix [`Ethernet3 library`](https://github.com/sstaub/Ethernet3), just copy these following files into the [`Ethernet3 library`](https://github.com/sstaub/Ethernet3) directory to overwrite the old files:
- [Ethernet3.h](LibraryPatches/Ethernet3/src/Ethernet3.h)
- [Ethernet3.cpp](LibraryPatches/Ethernet3/src/Ethernet3.cpp)

You can also use the forked and modified library at [Patched Ethernet3](https://github.com/khoih-prog/Ethernet3)

#### 6. For UIPEthernet library

***To be able to compile and run on nRF52 boards with ENC28J60 using UIPEthernet library***, you have to copy these following files into the UIPEthernet `utility` directory to overwrite the old files:

- [UIPEthernet.h](LibraryPatches/UIPEthernet/UIPEthernet.h)
- [UIPEthernet.cpp](LibraryPatches/UIPEthernet/UIPEthernet.cpp)
- [Enc28J60Network.h](LibraryPatches/UIPEthernet/utility/Enc28J60Network.h)
- [Enc28J60Network.cpp](LibraryPatches/UIPEthernet/utility/Enc28J60Network.cpp)

#### 7. For fixing ESP32 compile error

To fix [`ESP32 compile error`](https://github.com/espressif/arduino-esp32), just copy the following file into the [`ESP32`](https://github.com/espressif/arduino-esp32) cores/esp32 directory (e.g. ./arduino-1.8.13/hardware/espressif/cores/esp32) to overwrite the old file:
- [Server.h](LibraryPatches/esp32/cores/esp32/Server.h)

#### 8. For fixing ESP8266 compile error

To fix `ESP8266 compile error` such as

```
error: 'class EthernetClass' has no member named 'init'
Ethernet.init (USE_THIS_SS_PIN);
```

just rename the following file in ./arduino-1.8.13/hardware/esp8266com/esp8266/libraries/Ethernet directory

- From `Ethernet.h` to `Ethernet_ESP8266.h`

---
---

### HOWTO Use analogRead() with ESP32 running WiFi and/or BlueTooth (BT/BLE)

Please have a look at [**ESP_WiFiManager Issue 39: Not able to read analog port when using the autoconnect example**](https://github.com/khoih-prog/ESP_WiFiManager/issues/39) to have more detailed description and solution of the issue.

#### 1.  ESP32 has 2 ADCs, named ADC1 and ADC2

#### 2. ESP32 ADCs functions

- ADC1 controls ADC function for pins **GPIO32-GPIO39**
- ADC2 controls ADC function for pins **GPIO0, 2, 4, 12-15, 25-27**

#### 3.. ESP32 WiFi uses ADC2 for WiFi functions

Look in file [**adc_common.c**](https://github.com/espressif/esp-idf/blob/master/components/driver/adc_common.c#L61)

> In ADC2, there're two locks used for different cases:
> 1. lock shared with app and Wi-Fi:
>    ESP32:
>         When Wi-Fi using the ADC2, we assume it will never stop, so app checks the lock and returns immediately if failed.
>    ESP32S2:
>         The controller's control over the ADC is determined by the arbiter. There is no need to control by lock.
> 
> 2. lock shared between tasks:
>    when several tasks sharing the ADC2, we want to guarantee
>    all the requests will be handled.
>    Since conversions are short (about 31us), app returns the lock very soon,
>    we use a spinlock to stand there waiting to do conversions one by one.
> 
> adc2_spinlock should be acquired first, then adc2_wifi_lock or rtc_spinlock.


- In order to use ADC2 for other functions, we have to **acquire complicated firmware locks and very difficult to do**
- So, it's not advisable to use ADC2 with WiFi/BlueTooth (BT/BLE).
- Use ADC1, and pins GPIO32-GPIO39
- If somehow it's a must to use those pins serviced by ADC2 (**GPIO0, 2, 4, 12, 13, 14, 15, 25, 26 and 27**), use the **fix mentioned at the end** of [**ESP_WiFiManager Issue 39: Not able to read analog port when using the autoconnect example**](https://github.com/khoih-prog/ESP_WiFiManager/issues/39) to work with ESP32 WiFi/BlueTooth (BT/BLE).

---
---

### Important Notes

1. Code is restructured to provide flexibility to make it easy to support many more **WiFi/Ethernet** modules/shields in the future. Please delete the *.cpp files, replaced by *.hpp files, in the src directory, if *.cpp files still exist after installing new version.

2. For **Adafruit nRF52**, use the SPI's  pin as follows:

  - SS/CS     = 10
  - SPI_MOSI  = MO(SI)
  - SPI_MISO  = MI(SO)
  - SPI_SCK   = SCK

3. For **Adafruit SAMD21/SAMD51**, use the SPI's CS/SS pin as follows:

  - Itsy-Bitsy M0/M4, Feather M0 (Express), Hallowing M0 Express, Zero, Metro M0 => use CS = 16 = pin A2
  - Feather M4 (SAMD51)   => use SS/CS = 9
  - Grand Central M4      => use SS/CS = 53
  - Hallowing M4          => use SS/CS = 10
  - Metro M4 AirLift      => use SS/CS = 36

To know the default CS/SS pins of not listed boards, check the related `variant.h` files in 

`~/.arduino15/packages/adafruit/hardware/samd/x.y.zz/variants/board_name/variant.h`

4. For **Arduino SAM DUE**, use the SPI's  pin as follows:

  - SS/CS     = 10
  - SPI_MOSI  = 75 ( pin 4 @ [ICSP connector](pics/ICSP_connector.jpg) )
  - SPI_MISO  = 74 ( pin 1 @ [ICSP connector](pics/ICSP_connector.jpg) )
  - SPI_SCK   = 76 ( pin 3 @ [ICSP connector](pics/ICSP_connector.jpg) )
  
<p align="center">
    <img src="https://github.com/khoih-prog/WebSockets2_Generic/blob/master/pics/ICSP_connector.jpg">
</p>
  
---
---

#### Limitations

 - max input length is limited to the ram size and the `WEBSOCKETS_MAX_DATA_SIZE` define
 - max output length has no limit (the hardware is the limit)
 - Client send big frames with mask 0x00000000 (on AVR all frames)
 - continuation frame reassembly need to be handled in the application code

#### Limitations for Async

 - Functions called from within the context of the websocket event might not honor `yield()` and/or `delay()`.  See [this issue](https://github.com/Links2004/arduinoWebSockets/issues/58#issuecomment-192376395) for more info and a potential workaround.
 - wss / SSL is not possible.

---
---

### wss / SS

 supported for:
 
 - wss client on the ESP8266
 - wss / SSL is not natively supported in WebSocketsServer. However it is possible to achieve secure websockets
   by running the device behind an SSL proxy. See [Nginx](examples/Nginx/esp8266.ssl.reverse.proxy.conf) for a
   sample Nginx server configuration file to enable this.

### ESP Async TCP

This libary can run in Async TCP mode on the ESP.

The mode can be activated in the [WebSockets_Generic.h](src/WebSockets_Generic.h) (see WEBSOCKETS_NETWORK_TYPE define).

[ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP) library is required.

---
---

### How to use

In your code, select one of the currently supported network connection from the following list:

 1. **NETWORK_ESP8266_ASYNC** for ESP8266 Async
 2. **NETWORK_W5100** for W5x00 Ethernet
 3. **NETWORK_ENC28J60** for ENC28J60 Ethernet using UIPEthernet library
 4. **NETWORK_ESP32** for ESP32 WiFi
 5. **NETWORK_ESP32_ETH** for ESP32 Ethernet
 6. **NETWORK_WIFININA** for WiFiNINA
 7. **NETWORK_ETHERNET_ENC** for ENC28J60 Ethernet using EthernetENC library
 8. **NETWORK_RTL8720DN** for NETWORK_RTL8720DN Ethernet using [`Seeed_Arduino_rpcWiFi`](https://github.com/Seeed-Studio/Seeed_Arduino_rpcWiFi) and [`Seeed_Arduino_rpcUnified`](https://github.com/khoih-prog/Seeed_Arduino_rpcUnified) libraries
 9. **NETWORK_NATIVEETHERNET** for Teeensy 4.1 NativeEthernet
10. **NETWORK_LAN8742A** for STM32 with LAN8742A Ethernet using STM32Ethernet library
11. **NETWORK_WIFI101** for SAMD_MKR1000 and SAMD_MKRWIFI1010 using WiFi101 library
12. **NETWORK_QN_ETHERNET** for Teeensy 4.1 built-in Ethernet using [QNEthernet Library](https://github.com/ssilverman/QNEthernet) 
13. **NETWORK_PORTENTA_H7_WIFI**  for Portenta_H7 using `Murata` WiFi
14. **NETWORK_PORTENTA_H7_ETHERNET** for Portenta_H7 using Vision-shield Ethernet
15. **NETWORK_ESP32_ASYNC** for ESP32 using Async WebSockets
16. **NETWORK_RP2040W_WIFI** `RP2040W` using `CYW43439 WiFi` with `arduino-pico` core


then add `#define WEBSOCKETS_NETWORK_TYPE`  before `#include <WebSocketsClient_Generic.h>`

For example:

```cpp
#define WEBSOCKETS_NETWORK_TYPE   NETWORK_WIFININA

#include <WebSocketsClient_Generic.h>
```

### High Level Client API

 - `begin` : Initiate connection sequence to the WebSocket host.
 
```cpp
void begin(const char * host, uint16_t port, const char * url = "/", const char * protocol = "arduino");
void begin(String host, uint16_t port, String url = "/", String protocol = "arduino");
void begin(IPAddress host, uint16_t port, const char * url = "/", const char * protocol = "arduino");

void begin(IPAddress host, uint16_t port, String url = "/", String protocol = "arduino");
```

 - `beginSSL` : Initiate SSL connection sequence to the WebSocket host.
 
```cpp
#if defined(HAS_SSL)
#ifdef SSL_AXTLS

    void beginSSL(const char * host, uint16_t port, const char * url = "/", const char * fingerprint = "", const char * protocol = "arduino");
    void beginSSL(IPAddress host, uint16_t port, String url = "/", String fingerprint = "", String protocol = "arduino");
    void beginSSL(String host, uint16_t port, String url = "/", String fingerprint = "", String protocol = "arduino");

#else

    void beginSSL(const char * host, uint16_t port, const char * url = "/", const uint8_t * fingerprint = NULL, const char * protocol = "arduino");
    void beginSSL(IPAddress host, uint16_t port, String url = "/", String fingerprint = "", String protocol = "arduino");   
    void beginSslWithCA(const char * host, uint16_t port, const char * url = "/", BearSSL::X509List * CA_cert = NULL, const char * protocol = "arduino");

#endif    // SSL_AXTLS

    void beginSslWithCA(const char * host, uint16_t port, const char * url = "/", const char * CA_cert = NULL, const char * protocol = "arduino");

#endif    // HAS_SSL
```

 - `beginSocketIO` : Initiate connection sequence to the WebSocketIO host.
 
```cpp
void beginSocketIO(const char * host, uint16_t port, const char * url = "/socket.io/?EIO=3", const char * protocol = "arduino");
void beginSocketIO(String host, uint16_t port, String url = "/socket.io/?EIO=3", String protocol = "arduino");

void beginSocketIO(IPAddress host, uint16_t port, String url = "/socket.io/?EIO=3", String protocol = "arduino");
```

---

 - `onEvent`: Callback to handle for websocket events

```cpp
void onEvent(WebSocketClientEvent cbEvent);
```

 - `WebSocketClientEvent`: Handler for websocket events
 
```cpp
void (*WebSocketClientEvent)(WStype_t type, uint8_t * payload, size_t length)
```

Where `WStype_t type` is defined as:

```cpp
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
  WStype_PING,
  WStype_PONG,
} WStype_t;
```

---

### Configure SIO_PING_INTERVAL, SIO_PONG_TIMEOUT and SIO_DISCONNECT_TIMEOUT_COUNT

```cpp
//////////////////////////////
// New in v2.16.0
#define SIO_PING_INTERVAL                     90000L
#define SIO_PONG_TIMEOUT                      120000L
#define SIO_DISCONNECT_TIMEOUT_COUNT          10

//////////////////////////////
```

---
---

### Examples

#### For Generic boards using W5x00 Ethernet shield

1. [Generic_WebSocketClientSocketIO_W5500](examples/Generic/W5500/Generic_WebSocketClientSocketIO_W5500)
2. [Generic_WebSocketClientStompOverSockJs_W5500](examples/Generic/W5500/Generic_WebSocketClientStompOverSockJs_W5500)
3. [Generic_WebSocketClientStomp_W5500](examples/Generic/W5500/Generic_WebSocketClientStomp_W5500)
4. [Generic_WebSocketClient_W5500](examples/Generic/W5500/Generic_WebSocketClient_W5500).
5. [Generic_WebSocketServerAllFunctionsDemo_W5500](examples/Generic/W5500/Generic_WebSocketServerAllFunctionsDemo_W5500)
6. [Generic_WebSocketServerFragmentation_W5500](examples/Generic/W5500/Generic_WebSocketServerFragmentation_W5500)
7. [Generic_WebSocketServerHttpHeaderValidation_W5500](examples/Generic/W5500/Generic_WebSocketServerHttpHeaderValidation_W5500)
8. [Generic_WebSocketServer_LEDcontrol_W5500](examples/Generic/W5500/Generic_WebSocketServer_LEDcontrol_W5500)
9. [Generic_WebSocketServer_W5500](examples/Generic/W5500/Generic_WebSocketServer_W5500)

#### For Generic boards using WiFiNINA

 1. [Generic_WebSocketClientSocketIO_WiFiNINA](examples/Generic/WiFiNINA/Generic_WebSocketClientSocketIO_WiFiNINA)
 2. [Generic_WebSocketClientStompOverSockJs_WiFiNINA](examples/Generic/WiFiNINA/Generic_WebSocketClientStompOverSockJs_WiFiNINA)
 3. [Generic_WebSocketClientStomp_WiFiNINA](examples/Generic/WiFiNINA/Generic_WebSocketClientStomp_WiFiNINA)
 4. [Generic_WebSocketClient_WiFiNINA](examples/Generic/WiFiNINA/Generic_WebSocketClient_WiFiNINA).
 5. [Generic_WebSocketServerAllFunctionsDemo_WiFiNINA](examples/Generic/WiFiNINA/Generic_WebSocketServerAllFunctionsDemo_WiFiNINA)
 6. [Generic_WebSocketServerFragmentation_WiFiNINA](examples/Generic/WiFiNINA/Generic_WebSocketServerFragmentation_WiFiNINA)
 7. [Generic_WebSocketServerHttpHeaderValidation_WiFiNINA](examples/Generic/WiFiNINA/Generic_WebSocketServerHttpHeaderValidation_WiFiNINA)
 8. [Generic_WebSocketServer_LEDcontrol_WiFiNINA](examples/Generic/WiFiNINA/Generic_WebSocketServer_LEDcontrol_WiFiNINA)
 9. [Generic_WebSocketServer_WiFiNINA](examples/Generic/WiFiNINA/Generic_WebSocketServer_WiFiNINA)
10. [Generic_WebSocketClientSSL_WiFiNINA](examples/Generic/WiFiNINA/Generic_WebSocketClientSSL_WiFiNINA).

#### For Generic boards using ENC28J60 Ethernet shield and EthernetENC library

1. [Generic_WebSocketClientSocketIO_EthernetENC](examples/Generic/EthernetENC/Generic_WebSocketClientSocketIO_EthernetENC)
2. [Generic_WebSocketClientStompOverSockJs_EthernetENC](examples/Generic/EthernetENC/Generic_WebSocketClientStompOverSockJs_EthernetENC)
3. [Generic_WebSocketClientStomp_EthernetENC](examples/Generic/EthernetENC/Generic_WebSocketClientStomp_EthernetENC)
4. [Generic_WebSocketClient_EthernetENC](examples/Generic/EthernetENC/Generic_WebSocketClient_EthernetENC).
5. [Generic_WebSocketServerAllFunctionsDemo_EthernetENC](examples/Generic/EthernetENC/Generic_WebSocketServerAllFunctionsDemo_EthernetENC)
6. [Generic_WebSocketServerFragmentation_EthernetENC](examples/Generic/EthernetENC/Generic_WebSocketServerFragmentation_EthernetENC)
7. [Generic_WebSocketServerHttpHeaderValidation_EthernetENC](examples/Generic/EthernetENC/Generic_WebSocketServerHttpHeaderValidation_EthernetENC)
8. [Generic_WebSocketServer_LEDcontrol_EthernetENC](examples/Generic/EthernetENC/Generic_WebSocketServer_LEDcontrol_EthernetENC)
9. [Generic_WebSocketServer_EthernetENC](examples/Generic/EthernetENC/Generic_WebSocketServer_EthernetENC)

---

#### For WiFiNINA

1. [WebSocketClient_NINA](examples/WiFiNINA/WebSocketClient_NINA).
2. [WebSocketClientSocketIO_NINA](examples/WiFiNINA/WebSocketClientSocketIO_NINA)
3. [WebSocketClientStomp_NINA](examples/WiFiNINA/WebSocketClientStomp_NINA)
4. [WebSocketClientStompOverSockJs_NINA](examples/WiFiNINA/WebSocketClientStompOverSockJs_NINA)
5. [WebSocketClientSSL_NINA](examples/WiFiNINA/WebSocketClientSSL_NINA)
6. [WebSocketClient_Sticky_SocketIO_NINA](examples/WiFiNINA/WebSocketClient_Sticky_SocketIO_NINA). **New**

---

#### For WiFi101

1. [WebSocketClient_WiFi101](examples/WiFi101/WebSocketClient_WiFi101).
2. [WebSocketClientSocketIO_WiFi101](examples/WiFi101/WebSocketClientSocketIO_WiFi101)
3. [WebSocketClientStomp_WiFi101](examples/WiFi101/WebSocketClientStomp_WiFi101)
4. [WebSocketClientStompOverSockJs_WiFi101](examples/WiFi101/WebSocketClientStompOverSockJs_WiFi101)
5. [WebSocketClientSSL_WiFi101](examples/WiFi101/WebSocketClientSSL_WiFi101).

---

#### For W5x00 Ethernet shield

1. [WebSocketClient_W5500](examples/W5500/WebSocketClient_W5500).
2. [WebSocketClientSocketIO_W5500](examples/W5500/WebSocketClientSocketIO_W5500)
3. [WebSocketClientStomp_W5500](examples/W5500/WebSocketClientStomp_W5500)
4. [WebSocketClientStompOverSockJs_W5500](examples/W5500/WebSocketClientStompOverSockJs_W5500)
5. [WebSocketClient_Sticky_SocketIO_W5500](examples/W5500/WebSocketClient_Sticky_SocketIO_W5500). **New**

---

#### For ENC28J60 Ethernet shield using UIPEthernet library

1. [WebSocketClient_ENC](examples/ENC28J60/WebSocketClient_ENC).
2. [WebSocketClientSocketIO_ENC](examples/ENC28J60/WebSocketClientSocketIO_ENC)
3. [WebSocketClientStomp_ENC](examples/ENC28J60/WebSocketClientStomp_ENC)
4. [WebSocketClientStompOverSockJs_ENC](examples/ENC28J60/WebSocketClientStompOverSockJs_ENC)

---

#### For ESP32 board

1. [ESP32_WebSocketClient](examples/esp32/ESP32_WebSocketClient)
2. [ESP32_WebSocketClientSocketIO](examples/esp32/ESP32_WebSocketClientSocketIO) **New**
3. [ESP32_WebSocketClientSSL](examples/esp32/ESP32_WebSocketClientSSL)
4. [ESP32_WebSocketServer](examples/esp32/ESP32_WebSocketServer)

---

#### For ESP8266 board

 1. [ESP8266_WebSocketClient](examples/esp8266/ESP8266_WebSocketClient).
 2. [ESP8266_WebSocketClientSocketIO](examples/esp8266/ESP8266_WebSocketClientSocketIO)
 3. [ESP8266_WebSocketClientSocketIOack](examples/esp8266/ESP8266_WebSocketClientSocketIOack)
 4. [ESP8266_WebSocketClientSSL](examples/esp8266/ESP8266_WebSocketClientSSL).
 5. [ESP8266_WebSocketClientStomp](examples/esp8266/ESP8266_WebSocketClientStomp)
 6. [ESP8266_WebSocketClientStompOverSockJs](examples/esp8266/ESP8266_WebSocketClientStompOverSockJs)
 7. [ESP8266_WebSocketServer](examples/esp8266/ESP8266_WebSocketServer)
 8. [ESP8266_WebSocketServerAllFunctionsDemo](examples/esp8266/ESP8266_WebSocketServerAllFunctionsDemo)
 9. [ESP8266_WebSocketServerFragmentation](examples/esp8266/ESP8266_WebSocketServerFragmentation)
10. [ESP8266_WebSocketServerHttpHeaderValidation](examples/esp8266/ESP8266_WebSocketServerHttpHeaderValidation)
11. [ESP8266_WebSocketServer_LEDcontrol](examples/esp8266/ESP8266_WebSocketServer_LEDcontrol)
12. [ESP8266_WebSocketClient_Sticky_SocketIO](examples/esp8266/ESP8266_WebSocketClient_Sticky_SocketIO) **New**
13. [ESP8266_WebSocketClientOTA](examples/esp8266/ESP8266_WebSocketClientOTA) **New**
---

#### For SeeedStudio WIO Terminal using Realtek RTL8720DN WiFi

 1. [WIOTerminal_WebSocketClient](examples/WIO_Terminal/WIOTerminal_WebSocketClient)
 2. [WIOTerminal_WebSocketClientSSL](examples/WIO_Terminal/WIOTerminal_WebSocketClientSSL).
 3. [WIOTerminal_WebSocketServer](examples/WIO_Terminal/WIOTerminal_WebSocketServer)

---

#### For STM32 boards using built-in LAN8742A Ethernet

1. [STM32_WebSocketClient_LAN8742A](examples/STM32_LAN8742A/LAN8742A/STM32_WebSocketClient_LAN8742A)
2. [STM32_WebSocketClientSocketIO_LAN8742A](examples/STM32_LAN8742A/LAN8742A/STM32_WebSocketClientSocketIO_LAN8742A)
3. [STM32_WebSocketClientStomp_LAN8742A](examples/STM32_LAN8742A/LAN8742A/STM32_WebSocketClientStomp_LAN8742A)
4. [STM32_WebSocketClientStompOverSockJs_LAN8742A](examples/STM32_LAN8742A/LAN8742A/STM32_WebSocketClientStompOverSockJs_LAN8742A)
5. [STM32_WebSocketServerAllFunctionsDemo_LAN8742A](examples/STM32_LAN8742A/LAN8742A/STM32_WebSocketServerAllFunctionsDemo_LAN8742A)
6. [STM32_WebSocketServerFragmentation_LAN8742A](examples/STM32_LAN8742A/LAN8742A/STM32_WebSocketServerFragmentation_LAN8742A)
7. [STM32_WebSocketServerHttpHeaderValidation_LAN8742A](examples/STM32_LAN8742A/LAN8742A/STM32_WebSocketServerHttpHeaderValidation_LAN8742A)
8. [STM32_WebSocketServer_LAN8742A](examples/STM32_LAN8742A/LAN8742A/STM32_WebSocketServer_LAN8742A)
9. [STM32_WebSocketServer_LEDcontrol_LAN8742A](examples/STM32_LAN8742A/LAN8742A/STM32_WebSocketServer_LEDcontrol_LAN8742A)

---

#### For Teensy boards using ENC28J60 Ethernet shield and EthernetENC library

1. [Teensy_WebSocketClientSocketIO_EthernetENC](examples/Teensy/EthernetENC/Teensy_WebSocketClientSocketIO_EthernetENC)
2. [Teensy_WebSocketClientStompOverSockJs_EthernetENC](examples/Teensy/EthernetENC/Teensy_WebSocketClientStompOverSockJs_EthernetENC)
3. [Teensy_WebSocketClientStomp_EthernetENC](examples/Teensy/EthernetENC/Teensy_WebSocketClientStomp_EthernetENC)
4. [Teensy_WebSocketClient_EthernetENC](examples/Teensy/EthernetENC/Teensy_WebSocketClient_EthernetENC).
5. [Teensy_WebSocketServerAllFunctionsDemo_EthernetENC](examples/Teensy/EthernetENC/Teensy_WebSocketServerAllFunctionsDemo_EthernetENC)
6. [Teensy_WebSocketServerFragmentation_EthernetENC](examples/Teensy/EthernetENC/Teensy_WebSocketServerFragmentation_EthernetENC)
7. [Teensy_WebSocketServerHttpHeaderValidation_EthernetENC](examples/Teensy/EthernetENC/Teensy_WebSocketServerHttpHeaderValidation_EthernetENC)
8. [Teensy_WebSocketServer_LEDcontrol_EthernetENC](examples/Teensy/EthernetENC/Teensy_WebSocketServer_LEDcontrol_EthernetENC)
9. [Teensy_WebSocketServer_EthernetENC](examples/Teensy/EthernetENC/Teensy_WebSocketServer_EthernetENC)

#### For Teensy 4.1 boards using NativeEthernet

1. [Teensy_WebSocketClient_NativeEthernet](examples/Teensy/NativeEthernet/Teensy_WebSocketClient_NativeEthernet)
2. [Teensy_WebSocketClientStompOverSockJs_NativeEthernet](examples/Teensy/NativeEthernet/Teensy_WebSocketClientStompOverSockJs_NativeEthernet)
3. [Teensy_WebSocketClientStomp_NativeEthernet](examples/Teensy/NativeEthernet/Teensy_WebSocketClientStomp_NativeEthernet)
4. [Teensy_WebSocketClient_NativeEthernet](examples/Teensy/NativeEthernet/Teensy_WebSocketClient_NativeEthernet).
5. [Teensy_WebSocketServerAllFunctionsDemo_NativeEthernet](examples/Teensy/NativeEthernet/Teensy_WebSocketServerAllFunctionsDemo_NativeEthernet)
6. [Teensy_WebSocketServerFragmentation_NativeEthernet](examples/Teensy/NativeEthernet/Teensy_WebSocketServerFragmentation_NativeEthernet)
7. [Teensy_WebSocketServerHttpHeaderValidation_NativeEthernet](examples/Teensy/NativeEthernet/Teensy_WebSocketServerHttpHeaderValidation_NativeEthernet)
8. [Teensy_WebSocketServer_LEDcontrol_NativeEthernet](examples/Teensy/NativeEthernet/Teensy_WebSocketServer_LEDcontrol_NativeEthernet)
9. [Teensy_WebSocketServer_NativeEthernet](examples/Teensy/NativeEthernet/Teensy_WebSocketServer_NativeEthernet)


#### For Teensy boards using W5x00 Ethernet shield

1. [Teensy_WebSocketClientSocketIO_W5500](examples/Teensy/W5500/Teensy_WebSocketClientSocketIO_W5500)
2. [Teensy_WebSocketClientStompOverSockJs_W5500](examples/Teensy/W5500/Teensy_WebSocketClientStompOverSockJs_W5500)
3. [Teensy_WebSocketClientStomp_W5500](examples/Teensy/W5500/Teensy_WebSocketClientStomp_W5500)
4. [Teensy_WebSocketClient_W5500](examples/Teensy/W5500/Teensy_WebSocketClient_W5500).
5. [Teensy_WebSocketServerAllFunctionsDemo_W5500](examples/Teensy/W5500/Teensy_WebSocketServerAllFunctionsDemo_W5500)
6. [Teensy_WebSocketServerFragmentation_W5500](examples/Teensy/W5500/Teensy_WebSocketServerFragmentation_W5500)
7. [Teensy_WebSocketServerHttpHeaderValidation_W5500](examples/Teensy/W5500/Teensy_WebSocketServerHttpHeaderValidation_W5500)
8. [Teensy_WebSocketServer_LEDcontrol_W5500](examples/Teensy/W5500/Teensy_WebSocketServer_LEDcontrol_W5500)
9. [Teensy_WebSocketServer_W5500](examples/Teensy/W5500/Teensy_WebSocketServer_W5500)

#### For Teensy boards using WiFiNINA

 1. [Teensy_WebSocketClientSocketIO_WiFiNINA](examples/Teensy/WiFiNINA/Teensy_WebSocketClientSocketIO_WiFiNINA)
 2. [Teensy_WebSocketClientStompOverSockJs_WiFiNINA](examples/Teensy/WiFiNINA/Teensy_WebSocketClientStompOverSockJs_WiFiNINA)
 3. [Teensy_WebSocketClientStomp_WiFiNINA](examples/Teensy/WiFiNINA/Teensy_WebSocketClientStomp_WiFiNINA)
 4. [Teensy_WebSocketClient_WiFiNINA](examples/Teensy/WiFiNINA/Teensy_WebSocketClient_WiFiNINA).
 5. [Teensy_WebSocketServerAllFunctionsDemo_WiFiNINA](examples/Teensy/WiFiNINA/Teensy_WebSocketServerAllFunctionsDemo_WiFiNINA)
 6. [Teensy_WebSocketServerFragmentation_WiFiNINA](examples/Teensy/WiFiNINA/Teensy_WebSocketServerFragmentation_WiFiNINA)
 7. [Teensy_WebSocketServerHttpHeaderValidation_WiFiNINA](examples/Teensy/WiFiNINA/Teensy_WebSocketServerHttpHeaderValidation_WiFiNINA)
 8. [Teensy_WebSocketServer_LEDcontrol_WiFiNINA](examples/Teensy/WiFiNINA/Teensy_WebSocketServer_LEDcontrol_WiFiNINA)
 9. [Teensy_WebSocketServer_WiFiNINA](examples/Teensy/WiFiNINA/Teensy_WebSocketServer_WiFiNINA)
10. [Teensy_WebSocketClientSSL_WiFiNINA](examples/Teensy/WiFiNINA/Teensy_WebSocketClientSSL_WiFiNINA).

#### For Teensy 4.1 boards using QNEthernet

1. [Teensy_WebSocketClient_QNEthernet](examples/Teensy/QNEthernet/Teensy_WebSocketClient_QNEthernet)
2. [Teensy_WebSocketClientSocketIO_QNEthernet](examples/Teensy/QNEthernet/Teensy_WebSocketClientSocketIO_QNEthernet)
3. [Teensy_WebSocketServerAllFunctionsDemo_QNEthernet](examples/Teensy/QNEthernet/Teensy_WebSocketServerAllFunctionsDemo_QNEthernet)
4. [Teensy_WebSocketServerFragmentation_QNEthernet](examples/Teensy/QNEthernet/Teensy_WebSocketServerFragmentation_QNEthernet)
5. [Teensy_WebSocketServerHttpHeaderValidation_QNEthernet](examples/Teensy/QNEthernet/Teensy_WebSocketServerHttpHeaderValidation_QNEthernet)
6. [Teensy_WebSocketServer_LEDcontrol_QNEthernet](examples/Teensy/QNEthernet/Teensy_WebSocketServer_LEDcontrol_QNEthernet)
7. [Teensy_WebSocketServer_QNEthernet](examples/Teensy/QNEthernet/Teensy_WebSocketServer_QNEthernet)

---

#### Nodejs Socket.IO Test Server to use with examples

1. [app.js](examples/Nodejs_SocketIO_TestServer/app.js)
2. [SIO_server.sh](examples/Nodejs_SocketIO_TestServer/SIO_server.sh)
3. [HOWTO.md](examples/Nodejs_SocketIO_TestServer/HOWTO.md)
4. [package.json](examples/Nodejs_SocketIO_TestServer/package.json)
5. [package-lock.json](examples/Nodejs_SocketIO_TestServer/package-lock.json)

---

#### Nodejs_Sticky_SocketIO_TestServer to use with examples

1. [Multi_Sticky_SIO.js](examples/Nodejs_Sticky_SocketIO_TestServer/Multi_Sticky_SIO.js)
2. [Multi_Sticky_SIO_server.sh](examples/Nodejs_Sticky_SocketIO_TestServer/Multi_Sticky_SIO_server.sh)
3. [HOWTO.md](examples/Nodejs_Sticky_SocketIO_TestServer/HOWTO.md)
4. [package.json](examples/Nodejs_Sticky_SocketIO_TestServer/package.json)
5. [package-lock.json](examples/Nodejs_Sticky_SocketIO_TestServer/package-lock.json)

---

#### For WT32_ETH01 board

1. [WT32_ETH01_WebSocketClient](examples/WT32_ETH01/WT32_ETH01_WebSocketClient).
2. [WT32_ETH01_WebSocketClientSocketIO](examples/WT32_ETH01/WT32_ETH01_WebSocketClientSocketIO)
3. [WT32_ETH01_WebSocketClientSSL](examples/WT32_ETH01/WT32_ETH01_WebSocketClientSSL)
4. [WT32_ETH01_WebSocketServer](examples/WT32_ETH01/WT32_ETH01_WebSocketServer)
5. [WT32_ETH01_WebSocketClient_Sticky_SocketIO](examples/WT32_ETH01/WT32_ETH01_WebSocketClient_Sticky_SocketIO). **New**
6. [WT32_ETH01_SSL_SIO](examples/WT32_ETH01/WT32_ETH01_SSL_SIO). **New**

---


#### For Portenta_H7 board

##### Portenta_Ethernet

1. [Portenta_H7_WebSocketClient](examples/Portenta_H7/Ethernet/Portenta_H7_WebSocketClient).
2. [Portenta_H7_WebSocketClientSocketIO](examples/Portenta_H7/Ethernet/Portenta_H7_WebSocketClientSocketIO)
3. [Portenta_H7_WebSocketClient_Sticky_SocketIO](examples/Portenta_H7/Ethernet/Portenta_H7_WebSocketClient_Sticky_SocketIO). **New**

##### WiFi

1. [WebSocketClientSocketIO_WiFi](examples/Portenta_H7/WiFi/WebSocketClientSocketIO_WiFi)
2. [WebSocketClient_WiFi](examples/Portenta_H7/WiFi/WebSocketClient_WiFi)
3. [WebSocketClient_Sticky_SocketIO_WiFi](examples/Portenta_H7/WiFi/WebSocketClient_Sticky_SocketIO_WiFi). **New**

---

#### For ESP8266 Async WebSocket

1. [ESP8266_AsyncWSClient](examples/Async/ESP8266_AsyncWSClient)  **New**


#### For RP2040W boards using built-in WiFi

1. [WebSocketClient_RP2040W](examples/RP2040W/WebSocketClient_RP2040W)
2. [WebSocketClientSocketIO_RP2040W](examples/RP2040W/WebSocketClientSocketIO_RP2040W)
3. [WebSocketClientSSL_RP2040W](examples/RP2040W/WebSocketClientSSL_RP2040W)
4. [WebSocketClient_Sticky_SocketIO_RP2040W](examples/RP2040W/WebSocketClient_Sticky_SocketIO_RP2040W)


---
---

### Example [Generic_WebSocketClient_WiFiNINA](examples/Generic/WiFiNINA/Generic_WebSocketClient_WiFiNINA)

https://github.com/khoih-prog/WebSockets_Generic/blob/2c37735a9bc079161dbb02d4e9ffd56792821d9d/examples/Generic/WiFiNINA/Generic_WebSocketClient_WiFiNINA/Generic_WebSocketClient_WiFiNINA.ino#L18-L212


---
---

### Debug Terminal Output Samples


#### 1. [Generic_WebSocketClient_EthernetENC](examples/Generic/EthernetENC/Generic_WebSocketClient_EthernetENC) on NRF52840_FEATHER

```
Start Generic_WebSocketClient_EthernetENC on NRF52840_FEATHER
WebSockets_Generic v2.16.0
[WS] Board : NRF52840_FEATHER , setCsPin: 10
[WS] Default SPI pinout:
[WS] MOSI: 25
[WS] MISO: 24
[WS] SCK: 26
[WS] SS: 5
[WS] =========================
WebSockets Client @ IP address: 192.168.2.93
Connecting to WebSockets Server @ IP address: 192.168.2.140
[WS] [WS-Client][handleHeader] RX: HTTP/1.1 101 Switching Protocols
[WS] [WS-Client][handleHeader] RX: Server: arduino-WebSocketsServer
[WS] [WS-Client][handleHeader] RX: Upgrade: websocket
[WS] [WS-Client][handleHeader] RX: Connection: Upgrade
[WS] [WS-Client][handleHeader] RX: Sec-WebSocket-Version: 13
[WS] [WS-Client][handleHeader] RX: Sec-WebSocket-Accept: PJIpSp/QBZPtgaA5LIeNbtWz710=
[WS] [WS-Client][handleHeader] RX: Sec-WebSocket-Protocol: arduino
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL: /
[WS] [WS-Client][handleHeader] - cKey: XAtxzzeVt0HiQZ3cu85Y3A==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode: 101
[WS] [WS-Client][handleHeader] - cIsUpgrade: 1
[WS] [WS-Client][handleHeader] - cIsWebsocket: 1
[WS] [WS-Client][handleHeader] - cAccept: PJIpSp/QBZPtgaA5LIeNbtWz710=
[WS] [WS-Client][handleHeader] - cProtocol: arduino
[WS] [WS-Client][handleHeader] - cExtensions: 
[WS] [WS-Client][handleHeader] - cVersion: 13
[WS] [WS-Client][handleHeader] - cSessionId: 
[WSc] Connected to url: /
[WSc] get text: Connected

```
---

#### 2. [Generic_Ethernet_Blinds](https://github.com/khoih-prog/SinricPro_Generic/tree/master/examples/Generic/Blinds/Generic_Ethernet_Blinds) on NRF52840_FEATHER with ENC28J60 using EthernetENC Library

You can see the **NRF52840_FEATHER** board, with ENC28J60 using EthernetENC Library, connects to SinricPro service using [**SinricPro_Generic** library](https://github.com/khoih-prog/SinricPro_Generic). 

It also uses this [**WebSockets_Generic** library](https://github.com/khoih-prog/WebSockets_Generic) to communicate with [**SinricPro** service](https://sinric.pro/), to control Blinds via the `Sinric Pro` phone APP


```
Starting Generic_Ethernet_Blinds on NRF52840_FEATHER with ENC28J60 using EthernetENC Library
[SRP] =========================
[SRP] Default SPI pinout:
[SRP] MOSI: 25
[SRP] MISO: 24
[SRP] SCK: 26
[SRP] SS: 5
[SRP] =========================
[SRP] Use default CS/SS pin :  10
Index = 6
Connected!
[Ethernet]: IP-Address is 192.168.2.97
[SRP] Creating new device. No current DeviceId = 123456789012345678901234
[SRP] add(): Adding DeviceId = 123456789012345678901234
[SRP] begin(): App-Key = 12345678-1234-1234-1234-123456789012
[SRP] begin(): App-Secret = 12345678-1234-1234-1234-123456789012-12345678-1234-1234-1234-123456789012
[SRP] Websocket: Connecting to WebSocket Server:  ws.sinric.pro
Websocket: headers:
appkey:12345678-1234-1234-1234-123456789012
deviceids:123456789012345678901234
restoredevicestates:false
platform:NRF52840_FEATHER
version:2.8.4
Connected to SinricPro
[SRP] handleReceiveQueue(): Message(s) in receiveQueue = 1
[SRP] handleReceiveQueue(): Valid Signature. Processing message...
[SRP] extractTimestamp(): Got Timestamp = 1602031564
[SRP] handleReceiveQueue(): Message(s) in receiveQueue = 1
[SRP] handleReceiveQueue(): Valid Signature. Processing message...
[SRP] extractTimestamp(): Got Timestamp = 1602031575
[SRP] handleRequest(): handling request
{
  "header": {
    "payloadVersion": 2,
    "signatureVersion": 1
  },
  "payload": {
    "action": "setPowerState",
    "clientId": "sinricpro-web",
    "createdAt": 1602031575,
    "deviceAttributes": [],
    "deviceId": "123456789012345678901234",
    "replyToken": "4a2080b4-ca92-4fb7-aec1-3aa8d108a818",
    "type": "request",
    "value": {
      "state": "Off"
    }
  },
  "signature": {
    "HMAC": "PttsGuF4w4xfwzJHA6C5WDOmQmBgHXv9eTMSg8hYHXM="
  }
}
Device 123456789012345678901234 power turned off
[SRP] handleSendQueue(): Sending Number of Message(s) in sendQueue = 1
{
  "header": {
    "payloadVersion": 2,
    "signatureVersion": 1
  },
  "payload": {
    "action": "setPowerState",
    "clientId": "sinricpro-web",
    "createdAt": 1602031575,
    "deviceId": "123456789012345678901234",
    "message": "OK",
    "replyToken": "4a2080b4-ca92-4fb7-aec1-3aa8d108a818",
    "success": true,
    "type": "response",
    "value": {
      "state": "Off"
    }
  },
  "signature": {
    "HMAC": "42vn7js8X+KvuHHPZSrGeJt7dtwP8VVl9Y3cZsYtXOA="
  }
}
[SRP] handleSendQueue: Sending to WebSocket
[SRP] handleSendQueue(): Message sent.
[SRP] handleReceiveQueue(): Message(s) in receiveQueue = 1
[SRP] handleReceiveQueue(): Valid Signature. Processing message...
[SRP] extractTimestamp(): Got Timestamp = 1602031577
[SRP] handleRequest(): handling request
{
  "header": {
    "payloadVersion": 2,
    "signatureVersion": 1
  },
  "payload": {
    "action": "setPowerState",
    "clientId": "sinricpro-web",
    "createdAt": 1602031577,
    "deviceAttributes": [],
    "deviceId": "123456789012345678901234",
    "replyToken": "81a2433d-957d-481e-a9f7-2dea5c2e6c78",
    "type": "request",
    "value": {
      "state": "On"
    }
  },
  "signature": {
    "HMAC": "ir18LAhlUVCWHY5hA8QdGY8bzHE/s4XBeX8cwHTaPQI="
  }
}
Device 123456789012345678901234 power turned on
[SRP] handleSendQueue(): Sending Number of Message(s) in sendQueue = 1
{
  "header": {
    "payloadVersion": 2,
    "signatureVersion": 1
  },
  "payload": {
    "action": "setPowerState",
    "clientId": "sinricpro-web",
    "createdAt": 1602031577,
    "deviceId": "123456789012345678901234",
    "message": "OK",
    "replyToken": "81a2433d-957d-481e-a9f7-2dea5c2e6c78",
    "success": true,
    "type": "response",
    "value": {
      "state": "On"
    }
  },
  "signature": {
    "HMAC": "9Z1uVUjeaCf1T2Ol6Fv4wXkz8gaJvEYmmIRfRcDnOgA="
  }
}
[SRP] handleSendQueue: Sending to WebSocket
[SRP] handleSendQueue(): Message sent.
[SRP] handleReceiveQueue(): Message(s) in receiveQueue = 1
[SRP] handleReceiveQueue(): Valid Signature. Processing message...
[SRP] extractTimestamp(): Got Timestamp = 1602031578
[SRP] handleRequest(): handling request
{
  "header": {
    "payloadVersion": 2,
    "signatureVersion": 1
  },
  "payload": {
    "action": "setRangeValue",
    "clientId": "sinricpro-web",
    "createdAt": 1602031578,
    "deviceAttributes": [],
    "deviceId": "123456789012345678901234",
    "replyToken": "16d6fd59-dfd1-4017-ab51-ff8bb85fc806",
    "type": "request",
    "value": {
      "rangeValue": 100
    }
  },
  "signature": {
    "HMAC": "KfAwQ6z5XGvoZaspBVBP+CXnryW0YsEILqyxesHDhJo="
  }
}
Device 123456789012345678901234 set position to 100
[SRP] handleSendQueue(): Sending Number of Message(s) in sendQueue = 1
{
  "header": {
    "payloadVersion": 2,
    "signatureVersion": 1
  },
  "payload": {
    "action": "setRangeValue",
    "clientId": "sinricpro-web",
    "createdAt": 1602031578,
    "deviceId": "123456789012345678901234",
    "message": "OK",
    "replyToken": "16d6fd59-dfd1-4017-ab51-ff8bb85fc806",
    "success": true,
    "type": "response",
    "value": {
      "rangeValue": 100
    }
  },
  "signature": {
    "HMAC": "CNYG9Or+ZH+R2S1Ouvd2bqX8lT/8Siu23T8LycZVtTk="
  }
}
[SRP] handleSendQueue: Sending to WebSocket
[SRP] handleSendQueue(): Message sent.
[SRP] handleReceiveQueue(): Message(s) in receiveQueue = 1
[SRP] handleReceiveQueue(): Valid Signature. Processing message...
[SRP] extractTimestamp(): Got Timestamp = 1602031579
[SRP] handleRequest(): handling request
{
  "header": {
    "payloadVersion": 2,
    "signatureVersion": 1
  },
  "payload": {
    "action": "setRangeValue",
    "clientId": "sinricpro-web",
    "createdAt": 1602031579,
    "deviceAttributes": [],
    "deviceId": "123456789012345678901234",
    "replyToken": "864b1a6b-d539-49e0-bad8-48fa27d80d7f",
    "type": "request",
    "value": {
      "rangeValue": 0
    }
  },
  "signature": {
    "HMAC": "8CsQ57297F9lzrKBopsE8Xga2RdT9pgJpA8moGQFBTs="
  }
}
Device 123456789012345678901234 set position to 0
[SRP] handleSendQueue(): Sending Number of Message(s) in sendQueue = 1
{
  "header": {
    "payloadVersion": 2,
    "signatureVersion": 1
  },
  "payload": {
    "action": "setRangeValue",
    "clientId": "sinricpro-web",
    "createdAt": 1602031579,
    "deviceId": "123456789012345678901234",
    "message": "OK",
    "replyToken": "864b1a6b-d539-49e0-bad8-48fa27d80d7f",
    "success": true,
    "type": "response",
    "value": {
      "rangeValue": 0
    }
  },
  "signature": {
    "HMAC": "AF7bO1deIYIh3VLvVuLaRx6n1ACVYgk2Evl3NgiZ0pg="
  }
}
[SRP] handleSendQueue: Sending to WebSocket
[SRP] handleSendQueue(): Message sent.
```

---

#### 3. [WebSocketClientSocketIO_W5500](examples/W5500/WebSocketClientSocketIO_W5500) on NRF52840_FEATHER with W5500 using Ethernet_Generic Library

```
Start WebSocketClientSocketIO_W5500 on NRF52840_FEATHER with W5x00 using Ethernet_Generic Library
WebSockets_Generic v2.16.0
[WS] Board : NRF52840_FEATHER , setCsPin: 10
[WS] Default SPI pinout:
[WS] MOSI: 25
[WS] MISO: 24
[WS] SCK: 26
[WS] SS: 5
[WS] =========================
WebSockets Client @ IP address: 192.168.2.108
Connecting to WebSockets Server @ IP address: 192.168.2.30, port: 8080
["event_name",{"now":6671}]
[WS] [WS-Client][handleHeader] RX: HTTP/1.1 200 OK
[WS] [WS-Client][handleHeader] RX: Content-Type: text/plain; charset=UTF-8
[WS] [WS-Client][handleHeader] RX: Content-Length: 103
[WS] [WS-Client][handleHeader] RX: Access-Control-Allow-Credentials: true
[WS] [WS-Client][handleHeader] RX: Access-Control-Allow-Origin: file://
[WS] [WS-Client][handleHeader] RX: Set-Cookie: io=ivB-kiMefQhNrlkgAAAd; Path=/; HttpOnly; SameSite=Strict
[WS] [WS-Client][handleHeader] RX: Date: Thu, 08 Oct 2020 00:32:59 GMT
[WS] [WS-Client][handleHeader] RX: Connection: keep-alive
[WS] [WS-Client][handleHeader] RX: Keep-Alive: timeout=5
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL: /socket.io/?EIO=3
[WS] [WS-Client][handleHeader] - cKey: DT1hYS8BbVNsE++xI7B82A==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode: 200
[WS] [WS-Client][handleHeader] - cIsUpgrade: 0
[WS] [WS-Client][handleHeader] - cIsWebsocket: 1
[WS] [WS-Client][handleHeader] - cAccept: 
[WS] [WS-Client][handleHeader] - cProtocol: arduino
[WS] [WS-Client][handleHeader] - cExtensions: 
[WS] [WS-Client][handleHeader] - cVersion: 0
[WS] [WS-Client][handleHeader] - cSessionId: ivB-kiMefQhNrlkgAAAd
[IOc] Disconnected
[WS] [WS-Client][handleHeader] RX: HTTP/1.1 200 OK
[WS] [WS-Client][handleHeader] RX: Content-Type: text/plain; charset=UTF-8
[WS] [WS-Client][handleHeader] RX: Content-Length: 103
[WS] [WS-Client][handleHeader] RX: Access-Control-Allow-Credentials: true
[WS] [WS-Client][handleHeader] RX: Access-Control-Allow-Origin: file://
[WS] [WS-Client][handleHeader] RX: Set-Cookie: io=Lq1IQW-WBtymSZzzAAAe; Path=/; HttpOnly; SameSite=Strict
[WS] [WS-Client][handleHeader] RX: Date: Thu, 08 Oct 2020 00:32:59 GMT
[WS] [WS-Client][handleHeader] RX: Connection: keep-alive
[WS] [WS-Client][handleHeader] RX: Keep-Alive: timeout=5
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL: /socket.io/?EIO=3
[WS] [WS-Client][handleHeader] - cKey: onWidNpa6PhedJSy60Az5A==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode: 200
[WS] [WS-Client][handleHeader] - cIsUpgrade: 0
[WS] [WS-Client][handleHeader] - cIsWebsocket: 0
[WS] [WS-Client][handleHeader] - cAccept: 
[WS] [WS-Client][handleHeader] - cProtocol: arduino
[WS] [WS-Client][handleHeader] - cExtensions: 
[WS] [WS-Client][handleHeader] - cVersion: 0
[WS] [WS-Client][handleHeader] - cSessionId: Lq1IQW-WBtymSZzzAAAe
[WS] [WS-Client][handleHeader] RX: HTTP/1.1 101 Switching Protocols
[WS] [WS-Client][handleHeader] RX: Upgrade: websocket
[WS] [WS-Client][handleHeader] RX: Connection: Upgrade
[WS] [WS-Client][handleHeader] RX: Sec-WebSocket-Accept: 92pjBGiagarYGjMy1FsUcKOzrSk=
[WS] [WS-Client][handleHeader] RX: Sec-WebSocket-Protocol: arduino
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL: /socket.io/?EIO=3
[WS] [WS-Client][handleHeader] - cKey: i6rJQeEER2+zCTjRW4OjLg==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode: 101
[WS] [WS-Client][handleHeader] - cIsUpgrade: 1
[WS] [WS-Client][handleHeader] - cIsWebsocket: 1
[WS] [WS-Client][handleHeader] - cAccept: 92pjBGiagarYGjMy1FsUcKOzrSk=
[WS] [WS-Client][handleHeader] - cProtocol: arduino
[WS] [WS-Client][handleHeader] - cExtensions: 
[WS] [WS-Client][handleHeader] - cVersion: 0
[WS] [WS-Client][handleHeader] - cSessionId: Lq1IQW-WBtymSZzzAAAe
[IOc] Connected to url: /socket.io/?EIO=3
```

---

#### 4. [Generic_WebSocketClientSocketIO_EthernetENC](examples/Generic/EthernetENC/Generic_WebSocketClientSocketIO_EthernetENC) on NRF52840_FEATHER with ENC28J60 using EthernetENC Library

```
Start Generic_WebSocketClientSocketIO_EthernetENC on NRF52840_FEATHER with ENC28J60 using EthernetENC Library
WebSockets_Generic v2.16.0
[WS] Board : NRF52840_FEATHER , setCsPin: 10
[WS] Default SPI pinout:
[WS] MOSI: 25
[WS] MISO: 24
[WS] SCK: 26
[WS] SS: 5
[WS] =========================
WebSockets Client @ IP address: 192.168.2.86
Connecting to WebSockets Server @ IP address: 192.168.2.30, port: 8080
["event_name",{"now":9577}]
[WS] [WS-Client][handleHeader] RX: HTTP/1.1 200 OK
[WS] [WS-Client][handleHeader] RX: Content-Type: text/plain; charset=UTF-8
[WS] [WS-Client][handleHeader] RX: Content-Length: 103
[WS] [WS-Client][handleHeader] RX: Access-Control-Allow-Credentials: true
[WS] [WS-Client][handleHeader] RX: Access-Control-Allow-Origin: file://
[WS] [WS-Client][handleHeader] RX: Set-Cookie: io=W5HMvbfTz5Nbn2SgAAAV; Path=/; HttpOnly; SameSite=Strict
[WS] [WS-Client][handleHeader] RX: Date: Thu, 08 Oct 2020 00:23:11 GMT
[WS] [WS-Client][handleHeader] RX: Connection: keep-alive
[WS] [WS-Client][handleHeader] RX: Keep-Alive: timeout=5
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL: /socket.io/?EIO=3
[WS] [WS-Client][handleHeader] - cKey: tTIoy547t0bFK6IwotJ0Ww==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode: 200
[WS] [WS-Client][handleHeader] - cIsUpgrade: 0
[WS] [WS-Client][handleHeader] - cIsWebsocket: 1
[WS] [WS-Client][handleHeader] - cAccept: 
[WS] [WS-Client][handleHeader] - cProtocol: arduino
[WS] [WS-Client][handleHeader] - cExtensions: 
[WS] [WS-Client][handleHeader] - cVersion: 0
[WS] [WS-Client][handleHeader] - cSessionId: W5HMvbfTz5Nbn2SgAAAV
[IOc] Disconnected
[WS] [WS-Client][handleHeader] RX: HTTP/1.1 200 OK
[WS] [WS-Client][handleHeader] RX: Content-Type: text/plain; charset=UTF-8
[WS] [WS-Client][handleHeader] RX: Content-Length: 103
[WS] [WS-Client][handleHeader] RX: Access-Control-Allow-Credentials: true
[WS] [WS-Client][handleHeader] RX: Access-Control-Allow-Origin: file://
[WS] [WS-Client][handleHeader] RX: Set-Cookie: io=r1Kx0T7wZNQ9BHk9AAAW; Path=/; HttpOnly; SameSite=Strict
[WS] [WS-Client][handleHeader] RX: Date: Thu, 08 Oct 2020 00:23:11 GMT
[WS] [WS-Client][handleHeader] RX: Connection: keep-alive
[WS] [WS-Client][handleHeader] RX: Keep-Alive: timeout=5
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL: /socket.io/?EIO=3
[WS] [WS-Client][handleHeader] - cKey: BgGmlEiRrKzeEDEHHE48qA==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode: 200
[WS] [WS-Client][handleHeader] - cIsUpgrade: 0
[WS] [WS-Client][handleHeader] - cIsWebsocket: 0
[WS] [WS-Client][handleHeader] - cAccept: 
[WS] [WS-Client][handleHeader] - cProtocol: arduino
[WS] [WS-Client][handleHeader] - cExtensions: 
[WS] [WS-Client][handleHeader] - cVersion: 0
[WS] [WS-Client][handleHeader] - cSessionId: r1Kx0T7wZNQ9BHk9AAAW
[WS] [WS-Client][handleHeader] RX: HTTP/1.1 101 Switching Protocols
[WS] [WS-Client][handleHeader] RX: Upgrade: websocket
[WS] [WS-Client][handleHeader] RX: Connection: Upgrade
[WS] [WS-Client][handleHeader] RX: Sec-WebSocket-Accept: P76KK/oGpRX1UE1cAtk5jp31+Aw=
[WS] [WS-Client][handleHeader] RX: Sec-WebSocket-Protocol: arduino
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL: /socket.io/?EIO=3
[WS] [WS-Client][handleHeader] - cKey: 1uSPYGC/Asnl0DfyQcW5NQ==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode: 101
[WS] [WS-Client][handleHeader] - cIsUpgrade: 1
[WS] [WS-Client][handleHeader] - cIsWebsocket: 1
[WS] [WS-Client][handleHeader] - cAccept: P76KK/oGpRX1UE1cAtk5jp31+Aw=
[WS] [WS-Client][handleHeader] - cProtocol: arduino
[WS] [WS-Client][handleHeader] - cExtensions: 
[WS] [WS-Client][handleHeader] - cVersion: 0
[WS] [WS-Client][handleHeader] - cSessionId: r1Kx0T7wZNQ9BHk9AAAW
[IOc] Connected to url: /socket.io/?EIO=3
```

---

#### 5. [WIOTerminal_WebSocketClientSSL](examples/WIO_Terminal/WIOTerminal_WebSocketClientSSL) on SeeedStudio SAMD51 WIO_TERMINAL with Realtek RTL8720DN WiFi using Seeed_Arduino_rpcWiFi Library

```

Start WIOTerminal_WebSocketClientSSL on WIO_TERMINAL
WebSockets_Generic v2.16.0
WebSockets Client started @ IP address: 192.168.2.150
WebSockets Server : wss://account.duckdns.org
Connected to WebSockets Server @ wss://account.duckdns.org
[WS] [WS-Client] Connect wss...
[WS] [WS-Client] Calling _client.tcp->connect, _host = wss://account.duckdns.org , port = 443
[WS] [WS-Client] Calling _client.tcp->connect, _connectResult = 1
[WS] [WS-Client] connectedCb
[WS] [WS-Client][connectedCb] Connected to Host: wss://account.duckdns.org , Port: 443
[WS] [WS-Client] [sendHeader] Sending header...
[WS] [WS-Client] [sendHeader] Handshake: GET / HTTP/1.1
Host: wss://account.duckdns.org:443
Connection: Upgrade
Upgrade: websocket
Sec-WebSocket-Version: 13
Sec-WebSocket-Key: SfXbpj+U4TexBeIDuBiYog==
Sec-WebSocket-Protocol: arduino
Origin: file://
User-Agent: arduino-WebSocket-Client



```

---

#### 6. [Generic_WebSocketClientSSL_WiFiNINA](examples/Generic/WiFiNINA/Generic_WebSocketClientSSL_WiFiNINA) on Arduino SAMD21 Nano-33-IoT with WiFiNINA using WiFiNINA_Generic Library

```
Start Generic_WebSocketClientSSL_WiFiNINA on SAMD_NANO_33_IOT
WebSockets_Generic v2.16.0
Used/default SPI pinout:
MOSI:11
MISO:12
SCK:13
SS:10
Attempting to connect to SSID: HueNet1
SSID: HueNet1
WebSockets Client @ IP Address: 192.168.2.98
signal strength (RSSI):-13 dBm
WebSockets Server : wss://account.duckdns.org
Connected to WebSockets Server @ wss://account.duckdns.org
[WS] [WS-Client] Connect wss...
[WS] [WS-Client] connectedCb
[WS] [WS-Client][connectedCb] Connected to Host: wss://account.duckdns.org , Port: 443
[WS] [WS-Client] [sendHeader] Sending header...
[WS] [WS-Client] [sendHeader] Handshake: GET / HTTP/1.1
Host: wss://account.duckdns.org:443
Connection: Upgrade
Upgrade: websocket
Sec-WebSocket-Version: 13
Sec-WebSocket-Key: XOexobdyHBCcAnl9W4F+9w==
Sec-WebSocket-Protocol: arduino
Origin: file://
User-Agent: arduino-WebSocket-Client

```

---

#### 7. [WebSocketClientSocketIO_NINA](examples/WiFiNINA/WebSocketClientSocketIO_NINA) on Arduino SAMD21 Nano-33-IoT with WiFiNINA using WiFiNINA_Generic Library

#### 7.1 Client

```
Start WebSocketClientSocketIO_NINA on SAMD_NANO_33_IOT
WebSockets_Generic v2.16.0
Used/default SPI pinout:
MOSI:11
MISO:12
SCK:13
SS:10
Attempting to connect to SSID: HueNet1
SSID: HueNet1
WebSockets Client IP Address: 192.168.2.114
signal strength (RSSI):-18 dBm
Connecting to WebSockets Server @ IP address: 192.168.2.30, port: 8080
[WS] WebSockets_Generic v2.16.0
[WS] [wsIOc] found EIO=4 disable EIO ping on client
[WS] [WS-Client] Connect ws...
[WS] [WS-Client][connectedCb] Connected to Host:192.168.2.30, Port:8080
[WS] [WS-Client] [sendHeader] Sending header...
[WS] sendHeader: client->cKey = 7nb1qwQBvz58aozmAyBmag==
[WS] [WS-Client] [sendHeader] Handshake:GET /socket.io/?EIO=4&transport=polling HTTP/1.1
Host: 192.168.2.30:8080
Connection: keep-alive
Origin: file://
User-Agent: arduino-WebSocket-Client


[WS] [WS-Client] [sendHeader] Sending header... Done (us):2669
[WS] [WS-Client][handleHeader] RX:HTTP/1.1 200 OK
[WS] [WS-Client][handleHeader] RX:Content-Type: text/plain; charset=UTF-8
[WS] [WS-Client][handleHeader] RX:Content-Length: 97
[WS] [WS-Client][handleHeader] RX:Date: Mon, 24 May 2021 02:19:53 GMT
[WS] [WS-Client][handleHeader] RX:Connection: keep-alive
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL:/socket.io/?EIO=4
[WS] [WS-Client][handleHeader] - cKey:7nb1qwQBvz58aozmAyBmag==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode:200
[WS] [WS-Client][handleHeader] - cIsUpgrade:0
[WS] [WS-Client][handleHeader] - cIsWebsocket:1
[WS] [WS-Client][handleHeader] - cAccept:
[WS] [WS-Client][handleHeader] - cProtocol:arduino
[WS] [WS-Client][handleHeader] - cExtensions:
[WS] [WS-Client][handleHeader] - cVersion:0
[WS] [WS-Client][handleHeader] - cSessionId:
[WS] [WS-Client][handleHeader] Still missing cSessionId try Socket.IO
[WS] [WS-Client][handleHeader] socket.io json: 0{"sid":"WdrsvWrSeNmPDSlFAAAa","upgrades":["websocket"],"pingInterval":25000,"pingTimeout":20000}
[WS] [WS-Client][handleHeader] - cSessionId: WdrsvWrSeNmPDSlFAAAa
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL:/socket.io/?EIO=4
[WS] [WS-Client][handleHeader] - cKey:7nb1qwQBvz58aozmAyBmag==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode:200
[WS] [WS-Client][handleHeader] - cIsUpgrade:0
[WS] [WS-Client][handleHeader] - cIsWebsocket:1
[WS] [WS-Client][handleHeader] - cAccept:
[WS] [WS-Client][handleHeader] - cProtocol:arduino
[WS] [WS-Client][handleHeader] - cExtensions:
[WS] [WS-Client][handleHeader] - cVersion:0
[WS] [WS-Client][handleHeader] - cSessionId:WdrsvWrSeNmPDSlFAAAa
[WS] [WS-Client][handleHeader] found cSessionId
[WS] [WS-Client] [sendHeader] Sending header...
[WS] sendHeader: client->cKey = 6b1sIaeSY4sl6HQqSLZaKQ==
[WS] [WS-Client] [sendHeader] Handshake:GET /socket.io/?EIO=4&transport=websocket&sid=WdrsvWrSeNmPDSlFAAAa HTTP/1.1
Host: 192.168.2.30:8080
Connection: Upgrade
Upgrade: websocket
Sec-WebSocket-Version: 13
Sec-WebSocket-Key: 6b1sIaeSY4sl6HQqSLZaKQ==
Sec-WebSocket-Protocol: arduino
Origin: file://
User-Agent: arduino-WebSocket-Client


[WS] [WS-Client] [sendHeader] Sending header... Done (us):3458
[WS] [WS-Client][handleHeader] RX:HTTP/1.1 101 Switching Protocols
[WS] [WS-Client][handleHeader] RX:Upgrade: websocket
[WS] [WS-Client][handleHeader] RX:Connection: Upgrade
[WS] [WS-Client][handleHeader] RX:Sec-WebSocket-Accept: KJjOfxcGJBEkOTLzz/+pFRXs0Bc=
[WS] [WS-Client][handleHeader] RX:Sec-WebSocket-Protocol: arduino
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL:/socket.io/?EIO=4
[WS] [WS-Client][handleHeader] - cKey:6b1sIaeSY4sl6HQqSLZaKQ==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode:101
[WS] [WS-Client][handleHeader] - cIsUpgrade:1
[WS] [WS-Client][handleHeader] - cIsWebsocket:1
[WS] [WS-Client][handleHeader] - cAccept:KJjOfxcGJBEkOTLzz/+pFRXs0Bc=
[WS] [WS-Client][handleHeader] - cProtocol:arduino
[WS] [WS-Client][handleHeader] - cExtensions:
[WS] [WS-Client][handleHeader] - cVersion:0
[WS] [WS-Client][handleHeader] - cSessionId:WdrsvWrSeNmPDSlFAAAa
[WS] [WS-Client][handleHeader] Websocket connection init done.
[WS] [wsIOc] Connected to url:/socket.io/?EIO=4
[IOc] Connected to url: /socket.io/?EIO=4
[WS] [wsIOc] get pong
[IOc] Get PONG
[WS] [wsIOc] connected: len = 30
[WS] [wsIOc] data: {"sid":"QPel_hhsFl5pJ9ZpAAAb"}
["event_name",{"now":30001}]
[WS] [wsIOc] get ping send pong:3
[IOc] Get PING
[WS] [WS-Client] Sending HB ping
["event_name",{"now":30001}]
```

#### 7.2 Server

Using the Socket.IO Server in [**Nodejs_SocketIO_TestServer**](examples/Nodejs_SocketIO_TestServer)

```
kh@kh-Inspiron-3593:~/Downloads/Nodejs/New_SIO$ nodejs app
Server launched on port 8080
Connected
QPel_hhsFl5pJ9ZpAAAb
JWT token test:  { host: '192.168.2.30:8080',
  connection: 'keep-alive',
  origin: 'file://',
  'user-agent': 'arduino-WebSocket-Client' }
Message from Client :  { now: 30001 }

```

---

#### 8. [Generic_WebSocketClientSocketIO_W5500](examples/Generic/W5500/Generic_WebSocketClientSocketIO_W5500) on NRF52840_FEATHER with W5x00 using Ethernet_Generic Library


#### 8.1 Client

```
Start Generic_WebSocketClientSocketIO_W5500 on NRF52840_FEATHER with W5x00 using Ethernet_Generic Library
WebSockets_Generic v2.16.0
[WS] Board :NRF52840_FEATHER, setCsPin:10
[WS] Default SPI pinout:
[WS] MOSI:25
[WS] MISO:24
[WS] SCK:26
[WS] SS:5
[WS] =========================
_pinCS = 0
W5100 init, using SS_PIN_DEFAULT = 10, new ss_pin = 10, W5100Class::ss_pin = 10
W5100::init: W5500, SSIZE =8192
WebSockets Client @ IP address: 192.168.2.94
Connecting to WebSockets Server @ IP address: 192.168.2.30, port: 8080
[WS] WebSockets_Generic v2.16.0
[WS] [wsIOc] found EIO=4 disable EIO ping on client
[WS] [WS-Client][connectedCb] Connected to Host:192.168.2.30, Port:8080
[WS] [WS-Client] [sendHeader] Sending header...
[WS] sendHeader: client->cKey = A5uXYZ6TvNkcNshB9bd4mw==
[WS] [WS-Client] [sendHeader] Handshake:GET /socket.io/?EIO=4&transport=polling HTTP/1.1
Host: 192.168.2.30:8080
Connection: keep-alive
Authorization: 1234567890
User-Agent: arduino-WebSocket-Client


[WS] [WS-Client] [sendHeader] Sending header... Done (us):1953
[WS] [WS-Client][handleHeader] RX:HTTP/1.1 200 OK
[WS] [WS-Client][handleHeader] RX:Content-Type: text/plain; charset=UTF-8
[WS] [WS-Client][handleHeader] RX:Content-Length: 97
[WS] [WS-Client][handleHeader] RX:Date: Mon, 24 May 2021 02:44:22 GMT
[WS] [WS-Client][handleHeader] RX:Connection: keep-alive
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL:/socket.io/?EIO=4
[WS] [WS-Client][handleHeader] - cKey:A5uXYZ6TvNkcNshB9bd4mw==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode:200
[WS] [WS-Client][handleHeader] - cIsUpgrade:0
[WS] [WS-Client][handleHeader] - cIsWebsocket:1
[WS] [WS-Client][handleHeader] - cAccept:
[WS] [WS-Client][handleHeader] - cProtocol:arduino
[WS] [WS-Client][handleHeader] - cExtensions:
[WS] [WS-Client][handleHeader] - cVersion:0
[WS] [WS-Client][handleHeader] - cSessionId:
[WS] [WS-Client][handleHeader] Still missing cSessionId try Socket.IO
[WS] [WS-Client][handleHeader] socket.io json: 0{"sid":"NsqUuXlPnVoCSumYAAAc","upgrades":["websocket"],"pingInterval":25000,"pingTimeout":20000}
[WS] [WS-Client][handleHeader] - cSessionId: NsqUuXlPnVoCSumYAAAc
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL:/socket.io/?EIO=4
[WS] [WS-Client][handleHeader] - cKey:A5uXYZ6TvNkcNshB9bd4mw==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode:200
[WS] [WS-Client][handleHeader] - cIsUpgrade:0
[WS] [WS-Client][handleHeader] - cIsWebsocket:1
[WS] [WS-Client][handleHeader] - cAccept:
[WS] [WS-Client][handleHeader] - cProtocol:arduino
[WS] [WS-Client][handleHeader] - cExtensions:
[WS] [WS-Client][handleHeader] - cVersion:0
[WS] [WS-Client][handleHeader] - cSessionId:NsqUuXlPnVoCSumYAAAc
[WS] [WS-Client][handleHeader] found cSessionId
[WS] [WS-Client] [sendHeader] Sending header...
[WS] sendHeader: client->cKey = sQfvRYgeBW4Lg9NMIB977g==
[WS] [WS-Client] [sendHeader] Handshake:GET /socket.io/?EIO=4&transport=websocket&sid=NsqUuXlPnVoCSumYAAAc HTTP/1.1
Host: 192.168.2.30:8080
Connection: Upgrade
Upgrade: websocket
Sec-WebSocket-Version: 13
Sec-WebSocket-Key: sQfvRYgeBW4Lg9NMIB977g==
Sec-WebSocket-Protocol: arduino
Authorization: 1234567890
User-Agent: arduino-WebSocket-Client


[WS] [WS-Client] [sendHeader] Sending header... Done (us):3907
[WS] [WS-Client][handleHeader] RX:HTTP/1.1 101 Switching Protocols
[WS] [WS-Client][handleHeader] RX:Upgrade: websocket
[WS] [WS-Client][handleHeader] RX:Connection: Upgrade
[WS] [WS-Client][handleHeader] RX:Sec-WebSocket-Accept: KSsqUKjgfa1vum+jM9GAEuTogmY=
[WS] [WS-Client][handleHeader] RX:Sec-WebSocket-Protocol: arduino
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL:/socket.io/?EIO=4
[WS] [WS-Client][handleHeader] - cKey:sQfvRYgeBW4Lg9NMIB977g==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode:101
[WS] [WS-Client][handleHeader] - cIsUpgrade:1
[WS] [WS-Client][handleHeader] - cIsWebsocket:1
[WS] [WS-Client][handleHeader] - cAccept:KSsqUKjgfa1vum+jM9GAEuTogmY=
[WS] [WS-Client][handleHeader] - cProtocol:arduino
[WS] [WS-Client][handleHeader] - cExtensions:
[WS] [WS-Client][handleHeader] - cVersion:0
[WS] [WS-Client][handleHeader] - cSessionId:NsqUuXlPnVoCSumYAAAc
[WS] [WS-Client][handleHeader] Websocket connection init done.
[WS] [wsIOc] Connected to url:/socket.io/?EIO=4
[IOc] Connected to url: /socket.io/?EIO=4
[WS] [wsIOc] get pong
[IOc] Get PONG
[WS] [wsIOc] connected: len = 30
[WS] [wsIOc] data: {"sid":"cSx3YyFrDOfxUW8hAAAd"}
Message from Client :  { now: 30001 }

```

---

#### 8.2 Server

Using the Socket.IO Server in [**Nodejs_SocketIO_TestServer**](examples/Nodejs_SocketIO_TestServer)

```
kh@kh-Inspiron-3593:~/Downloads/Nodejs/New_SIO$ nodejs app
Server launched on port 8080
Connected
cSx3YyFrDOfxUW8hAAAd
JWT token test:  { host: '192.168.2.30:8080',
  connection: 'keep-alive',
  authorization: '1234567890',
  'user-agent': 'arduino-WebSocket-Client' }
Message from Client :  { now: 30001 }

```

---

#### 9. [WebSocketClientSocketIO_W5500](examples/W5500/WebSocketClientSocketIO_W5500) on RASPBERRY_PI_PICO with W5x00 using Ethernet_Generic Library

The following are debug terminal output when running example [WebSocketClientSocketIO_W5500](examples/W5500/WebSocketClientSocketIO_W5500) on RASPBERRY_PI_PICO with W5x00 and Ethernet_Generic Library using [**Earle Philhower's arduino-pico core** v1.12.0](https://github.com/earlephilhower/arduino-pico)


#### 9.1 Client

```
Start WebSocketClientSocketIO_W5500 on RASPBERRY_PI_PICO with W5x00 using Ethernet_Generic Library
WebSockets_Generic v2.16.0
[WS] Board :RASPBERRY_PI_PICO, setCsPin:17
[WS] Default SPI pinout:
[WS] MOSI:19
[WS] MISO:16
[WS] SCK:18
[WS] SS:17
[WS] =========================
WebSockets Client @ IP address: 192.168.2.119
Connecting to WebSockets Server @ IP address: 192.168.2.30, port: 8080
[WS] WebSockets_Generic v2.16.0
[WS] [wsIOc] found EIO=4 disable EIO ping on client
[WS] [WS-Client][connectedCb] Connected to Host:192.168.2.30, Port:8080
[WS] [WS-Client] [sendHeader] Sending header...
[WS] sendHeader: client->cKey = rqJZ543MlA+uTo5YdFR+qA==
[WS] [WS-Client] [sendHeader] Handshake:GET /socket.io/?EIO=4&transport=polling HTTP/1.1
Host: 192.168.2.30:8080
Connection: keep-alive
Authorization: 1234567890
User-Agent: arduino-WebSocket-Client


[WS] [WS-Client] [sendHeader] Sending header... Done (us):2184
[WS] [WS-Client][handleHeader] RX:HTTP/1.1 200 OK
[WS] [WS-Client][handleHeader] RX:Content-Type: text/plain; charset=UTF-8
[WS] [WS-Client][handleHeader] RX:Content-Length: 97
[WS] [WS-Client][handleHeader] RX:Date: Mon, 24 May 2021 04:29:00 GMT
[WS] [WS-Client][handleHeader] RX:Connection: keep-alive
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL:/socket.io/?EIO=4
[WS] [WS-Client][handleHeader] - cKey:rqJZ543MlA+uTo5YdFR+qA==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode:200
[WS] [WS-Client][handleHeader] - cIsUpgrade:0
[WS] [WS-Client][handleHeader] - cIsWebsocket:1
[WS] [WS-Client][handleHeader] - cAccept:
[WS] [WS-Client][handleHeader] - cProtocol:arduino
[WS] [WS-Client][handleHeader] - cExtensions:
[WS] [WS-Client][handleHeader] - cVersion:0
[WS] [WS-Client][handleHeader] - cSessionId:
[WS] [WS-Client][handleHeader] Still missing cSessionId try Socket.IO
[WS] [WS-Client][handleHeader] socket.io json: 0{"sid":"TCiWG1KEq2MHPezpAAAg","upgrades":["websocket"],"pingInterval":25000,"pingTimeout":20000}
[WS] [WS-Client][handleHeader] - cSessionId: TCiWG1KEq2MHPezpAAAg
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL:/socket.io/?EIO=4
[WS] [WS-Client][handleHeader] - cKey:rqJZ543MlA+uTo5YdFR+qA==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode:200
[WS] [WS-Client][handleHeader] - cIsUpgrade:0
[WS] [WS-Client][handleHeader] - cIsWebsocket:1
[WS] [WS-Client][handleHeader] - cAccept:
[WS] [WS-Client][handleHeader] - cProtocol:arduino
[WS] [WS-Client][handleHeader] - cExtensions:
[WS] [WS-Client][handleHeader] - cVersion:0
[WS] [WS-Client][handleHeader] - cSessionId:TCiWG1KEq2MHPezpAAAg
[WS] [WS-Client][handleHeader] found cSessionId
[WS] [WS-Client] [sendHeader] Sending header...
[WS] sendHeader: client->cKey = ADdto91YKZwqbZ0X0v5ceg==
[WS] [WS-Client] [sendHeader] Handshake:GET /socket.io/?EIO=4&transport=websocket&sid=TCiWG1KEq2MHPezpAAAg HTTP/1.1
Host: 192.168.2.30:8080
Connection: Upgrade
Upgrade: websocket
Sec-WebSocket-Version: 13
Sec-WebSocket-Key: ADdto91YKZwqbZ0X0v5ceg==
Sec-WebSocket-Protocol: arduino
Authorization: 1234567890
User-Agent: arduino-WebSocket-Client


[WS] [WS-Client] [sendHeader] Sending header... Done (us):2851
[WS] [WS-Client][handleHeader] RX:HTTP/1.1 101 Switching Protocols
[WS] [WS-Client][handleHeader] RX:Upgrade: websocket
[WS] [WS-Client][handleHeader] RX:Connection: Upgrade
[WS] [WS-Client][handleHeader] RX:Sec-WebSocket-Accept: gKekYE3itucoN6vDWPglIu3GU1U=
[WS] [WS-Client][handleHeader] RX:Sec-WebSocket-Protocol: arduino
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL:/socket.io/?EIO=4
[WS] [WS-Client][handleHeader] - cKey:ADdto91YKZwqbZ0X0v5ceg==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode:101
[WS] [WS-Client][handleHeader] - cIsUpgrade:1
[WS] [WS-Client][handleHeader] - cIsWebsocket:1
[WS] [WS-Client][handleHeader] - cAccept:gKekYE3itucoN6vDWPglIu3GU1U=
[WS] [WS-Client][handleHeader] - cProtocol:arduino
[WS] [WS-Client][handleHeader] - cExtensions:
[WS] [WS-Client][handleHeader] - cVersion:0
[WS] [WS-Client][handleHeader] - cSessionId:TCiWG1KEq2MHPezpAAAg
[WS] [WS-Client][handleHeader] Websocket connection init done.
[WS] [wsIOc] Connected to url:/socket.io/?EIO=4
[IOc] Connected to url: /socket.io/?EIO=4
[WS] [wsIOc] get pong
[IOc] Get PONG
[WS] [wsIOc] connected: len = 30
[WS] [wsIOc] data: {"sid":"kcgaqp1ovWXOgjCnAAAh"}
["event_name",{"now":30001}]
[WS] [wsIOc] get ping send pong:3
[IOc] Get PING
[WS] [WS-Client] Sending HB ping
["event_name",{"now":60002}]
[WS] [wsIOc] get ping send pong:3
[WS] [wsIOc] get ping send pong:3
["event_name",{"now":90003}]
[WS] [wsIOc] get ping send pong:3
[IOc] Get PING
[WS] [WS-Client] Sending HB ping
["event_name",{"now":120004}]
[WS] [wsIOc] get ping send pong:3
["event_name",{"now":150005}]
[WS] [wsIOc] get ping send pong:3

```

---

#### 9.2 Server

Using the Socket.IO Server in [**Nodejs_SocketIO_TestServer**](examples/Nodejs_SocketIO_TestServer)

```
kh@kh-Inspiron-3593:~/Downloads/Nodejs/New_SIO$ nodejs app
Server launched on port 8080
Connected
kcgaqp1ovWXOgjCnAAAh
JWT token test:  { host: '192.168.2.30:8080',
  connection: 'keep-alive',
  authorization: '1234567890',
  'user-agent': 'arduino-WebSocket-Client' }
Message from Client :  { now: 30001 }
Message from Client :  { now: 60002 }
Message from Client :  { now: 90003 }
Message from Client :  { now: 120004 }
Message from Client :  { now: 150005 }

```

---

#### 10. [WebSocketClientSocketIO_W5500](examples/W5500/WebSocketClientSocketIO_W5500) on MBED RASPBERRY_PI_PICO with W5x00 using Ethernet_Generic Library

The following are debug terminal output when running example [WebSocketClientSocketIO_W5500](examples/W5500/WebSocketClientSocketIO_W5500) on RASPBERRY_PI_PICO with W5x00 and Ethernet_Generic Library using [**Arduino-mbed RP2040** core](https://github.com/arduino/ArduinoCore-mbed)


#### 10.1 Client

```
Start WebSocketClientSocketIO_W5500 on RaspberryPi Pico with W5x00 using Ethernet_Generic Library
WebSockets_Generic v2.16.0
[WS] Board :RaspberryPi Pico, setCsPin:17
[WS] Default SPI pinout:
[WS] MOSI:19
[WS] MISO:16
[WS] SCK:18
[WS] SS:17
[WS] =========================
WebSockets Client @ IP address: 192.168.2.98
Connecting to WebSockets Server @ IP address: 192.168.2.30, port: 8080
[WS] WebSockets_Generic v2.16.0
[WS] [wsIOc] found EIO=4 disable EIO ping on client
[WS] [WS-Client][connectedCb] Connected to Host:192.168.2.30, Port:8080
[WS] [WS-Client] [sendHeader] Sending header...
[WS] sendHeader: client->cKey = fDyOMZVB7gvbnhOZclfWcA==
[WS] [WS-Client] [sendHeader] Handshake:GET /socket.io/?EIO=4&transport=polling HTTP/1.1
Host: 192.168.2.30:8080
Connection: keep-alive
Authorization: 1234567890
User-Agent: arduino-WebSocket-Client


[WS] [WS-Client] [sendHeader] Sending header... Done (us):3217
[WS] [WS-Client][handleHeader] RX:HTTP/1.1 200 OK
[WS] [WS-Client][handleHeader] RX:Content-Type: text/plain; charset=UTF-8
[WS] [WS-Client][handleHeader] RX:Content-Length: 97
[WS] [WS-Client][handleHeader] RX:Date: Mon, 24 May 2021 04:37:41 GMT
[WS] [WS-Client][handleHeader] RX:Connection: keep-alive
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL:/socket.io/?EIO=4
[WS] [WS-Client][handleHeader] - cKey:fDyOMZVB7gvbnhOZclfWcA==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode:200
[WS] [WS-Client][handleHeader] - cIsUpgrade:0
[WS] [WS-Client][handleHeader] - cIsWebsocket:1
[WS] [WS-Client][handleHeader] - cAccept:
[WS] [WS-Client][handleHeader] - cProtocol:arduino
[WS] [WS-Client][handleHeader] - cExtensions:
[WS] [WS-Client][handleHeader] - cVersion:0
[WS] [WS-Client][handleHeader] - cSessionId:
[WS] [WS-Client][handleHeader] Still missing cSessionId try Socket.IO
[WS] [WS-Client][handleHeader] socket.io json: 0{"sid":"7zb9ikut5ezr2H6kAAAi","upgrades":["websocket"],"pingInterval":25000,"pingTimeout":20000}
[WS] [WS-Client][handleHeader] - cSessionId: 7zb9ikut5ezr2H6kAAAi
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL:/socket.io/?EIO=4
[WS] [WS-Client][handleHeader] - cKey:fDyOMZVB7gvbnhOZclfWcA==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode:200
[WS] [WS-Client][handleHeader] - cIsUpgrade:0
[WS] [WS-Client][handleHeader] - cIsWebsocket:1
[WS] [WS-Client][handleHeader] - cAccept:
[WS] [WS-Client][handleHeader] - cProtocol:arduino
[WS] [WS-Client][handleHeader] - cExtensions:
[WS] [WS-Client][handleHeader] - cVersion:0
[WS] [WS-Client][handleHeader] - cSessionId:7zb9ikut5ezr2H6kAAAi
[WS] [WS-Client][handleHeader] found cSessionId
[WS] [WS-Client] [sendHeader] Sending header...
[WS] sendHeader: client->cKey = m1MElaS+NSouCUhWjSLPug==
[WS] [WS-Client] [sendHeader] Handshake:GET /socket.io/?EIO=4&transport=websocket&sid=7zb9ikut5ezr2H6kAAAi HTTP/1.1
Host: 192.168.2.30:8080
Connection: Upgrade
Upgrade: websocket
Sec-WebSocket-Version: 13
Sec-WebSocket-Key: m1MElaS+NSouCUhWjSLPug==
Sec-WebSocket-Protocol: arduino
Authorization: 1234567890
User-Agent: arduino-WebSocket-Client


[WS] [WS-Client] [sendHeader] Sending header... Done (us):4878
[WS] [WS-Client][handleHeader] RX:HTTP/1.1 101 Switching Protocols
[WS] [WS-Client][handleHeader] RX:Upgrade: websocket
[WS] [WS-Client][handleHeader] RX:Connection: Upgrade
[WS] [WS-Client][handleHeader] RX:Sec-WebSocket-Accept: r/1zdz7Sk+4eMliZJTebh6cH7uw=
[WS] [WS-Client][handleHeader] RX:Sec-WebSocket-Protocol: arduino
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL:/socket.io/?EIO=4
[WS] [WS-Client][handleHeader] - cKey:m1MElaS+NSouCUhWjSLPug==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode:101
[WS] [WS-Client][handleHeader] - cIsUpgrade:1
[WS] [WS-Client][handleHeader] - cIsWebsocket:1
[WS] [WS-Client][handleHeader] - cAccept:r/1zdz7Sk+4eMliZJTebh6cH7uw=
[WS] [WS-Client][handleHeader] - cProtocol:arduino
[WS] [WS-Client][handleHeader] - cExtensions:
[WS] [WS-Client][handleHeader] - cVersion:0
[WS] [WS-Client][handleHeader] - cSessionId:7zb9ikut5ezr2H6kAAAi
[WS] [WS-Client][handleHeader] Websocket connection init done.
[WS] [wsIOc] Connected to url:/socket.io/?EIO=4
[IOc] Connected to url: /socket.io/?EIO=4
[WS] [wsIOc] get pong
[IOc] Get PONG
[WS] [wsIOc] connected: len = 30
[WS] [wsIOc] data: {"sid":"7SXGNMJxjtzOWXClAAAj"}
["event_name",{"now":30001}]
[WS] [wsIOc] get ping send pong:3
[IOc] Get PING
[WS] [WS-Client] Sending HB ping
["event_name",{"now":60002}]
[WS] [wsIOc] get ping send pong:3
["event_name",{"now":90003}]
[WS] [wsIOc] get ping send pong:3
[WS] [wsIOc] get ping send pong:3
[IOc] Get PING
[WS] [WS-Client] Sending HB ping
["event_name",{"now":120004}]
```

---

#### 10.2 Server

Using the Socket.IO Server in [**Nodejs_SocketIO_TestServer**](examples/Nodejs_SocketIO_TestServer)

```
kh@kh-Inspiron-3593:~/Downloads/Nodejs/New_SIO$ nodejs app
Server launched on port 8080
Connected
7SXGNMJxjtzOWXClAAAj
JWT token test:  { host: '192.168.2.30:8080',
  connection: 'keep-alive',
  authorization: '1234567890',
  'user-agent': 'arduino-WebSocket-Client' }
Message from Client :  { now: 30001 }
Message from Client :  { now: 60002 }
Message from Client :  { now: 90003 }
Message from Client :  { now: 120005 }
```

---

#### 11. [WT32_ETH01_WebSocketClientSocketIO](examples/WT32_ETH01/WT32_ETH01_WebSocketClientSocketIO) on WT32_ETH01

The following are debug terminal output when running example [WT32_ETH01_WebSocketClientSocketIO](examples/WT32_ETH01/WT32_ETH01_WebSocketClientSocketIO) on WT32_ETH01

#### 11.1 Client

```
Starting WT32_ETH01_WebSocketClient on ESP32_DEV with ETH_PHY_LAN8720
WebServer_WT32_ETH01 v1.4.1
WebSockets_Generic v2.16.0
ETH MAC: A8:03:2A:A1:61:73, IPv4: 192.168.2.232, FULL_DUPLEX, 100Mbps
WebSockets Client started @ IP address: 192.168.2.232
Connecting to WebSockets Server @ IP address: 192.168.2.30, port: 8080
[WS] [WS-Client] Connect ws...
[IOc] Disconnected
[WS] [WS-Client] Connect ws...
[WS] [WS-Client][connectedCb] Connected to Host:192.168.2.30, Port:8080
[WS] [wsIOc] Connected to url:/socket.io/?EIO=4
[IOc] Connected to url: /socket.io/?EIO=4
[WS] [wsIOc] get pong
[IOc] Get PONG
[WS] [wsIOc] connected: len = 30
[WS] [wsIOc] data: {"sid":"cxEI-kMvZ1uKiyQ-AAAB"}
["event_name",{"now":30001}]
[WS] [wsIOc] get event: len = 53
[WS] [wsIOc] get data: ["Send Message io.emit Broadcasted : ",{"now":30001}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":30001}]
[WS] [wsIOc] get event: len = 33
[WS] [wsIOc] get data: ["Send Message : ",{"now":30001}]
[IOc] Get event: ["Send Message : ",{"now":30001}]
[WS] [wsIOc] get ping send pong:3
[IOc] Get PING
[WS] [WS-Client] Sending HB ping
["event_name",{"now":60002}]
[WS] [wsIOc] get event: len = 53
[WS] [wsIOc] get data: ["Send Message io.emit Broadcasted : ",{"now":60002}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":60002}]
[WS] [wsIOc] get event: len = 33
[WS] [wsIOc] get data: ["Send Message : ",{"now":60002}]
[IOc] Get event: ["Send Message : ",{"now":60002}]
[WS] [wsIOc] get ping send pong:3
["event_name",{"now":90003}]
[WS] [wsIOc] get event: len = 53
[WS] [wsIOc] get data: ["Send Message io.emit Broadcasted : ",{"now":90003}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":90003}]
[WS] [wsIOc] get event: len = 33
[WS] [wsIOc] get data: ["Send Message : ",{"now":90003}]
[IOc] Get event: ["Send Message : ",{"now":90003}]
[WS] [wsIOc] get ping send pong:3
[IOc] Get PING
[WS] [WS-Client] Sending HB ping
["event_name",{"now":120004}]
[WS] [wsIOc] get event: len = 54
[WS] [wsIOc] get data: ["Send Message io.emit Broadcasted : ",{"now":120004}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":120004}]
[WS] [wsIOc] get event: len = 34
[WS] [wsIOc] get data: ["Send Message : ",{"now":120004}]
[IOc] Get event: ["Send Message : ",{"now":120004}]
[WS] [wsIOc] get ping send pong:3
```

---

#### 11.2 Server

Using the Socket.IO Server in [**Nodejs_SocketIO_TestServer**](examples/Nodejs_SocketIO_TestServer)

```
kh@kh-Inspiron-3593:~/Downloads/Nodejs/New_SIO$ nodejs app
Server launched on port 8080
Connected
cxEI-kMvZ1uKiyQ-AAAB
JWT token test:  { host: '192.168.2.30:8080',
  connection: 'keep-alive',
  authorization: '1234567890',
  'user-agent': 'arduino-WebSocket-Client' }
Message from Client :  { now: 30001 }
Message from Client :  { now: 60002 }
Message from Client :  { now: 90003 }
Message from Client :  { now: 120004 }
```

---

#### 12. [Teensy_WebSocketClientSocketIO_QNEthernet](examples/Teensy/QNEthernet/Teensy_WebSocketClientSocketIO_QNEthernet) on TEENSY 4.1 using QNEthernet

The following are debug terminal output when running example [Teensy_WebSocketClientSocketIO_QNEthernet](examples/Teensy/QNEthernet/Teensy_WebSocketClientSocketIO_QNEthernet) on TEENSY 4.1 using [`QNEthernet`](https://github.com/ssilverman/QNEthernet) library


#### 12.1 Client

```
Starting Teensy_WebSocketClient_QNEthernet on TEENSY 4.1 using QNEthernet
WebSockets_Generic v2.16.0
=========== USE_QN_ETHERNET ===========
Initialize Ethernet using static IP => Connected! IP address:192.168.2.222
Connecting to WebSockets Server @ IP address: 192.168.2.30, port: 8080
[IOc] Connected to url: /socket.io/?EIO=4
["event_name",{"now":30001}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":30001}]
[IOc] Get event: ["Send Message : ",{"now":30001}]
["event_name",{"now":60002}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":60002}]
[IOc] Get event: ["Send Message : ",{"now":60002}]
["event_name",{"now":90003}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":90003}]
[IOc] Get event: ["Send Message : ",{"now":90003}]
```

---

#### 12.2 Server

Using the Socket.IO Server in [**Nodejs_SocketIO_TestServer**](examples/Nodejs_SocketIO_TestServer)

```
kh@kh-Inspiron-3593:~/Downloads/Nodejs/New_SIO$ ./SIO_server.sh 
Server launched on port 8080
Connected
bK733pVgbd3DCsy5AAAB
JWT token test:  { host: '192.168.2.30:8080',
  connection: 'keep-alive',
  authorization: '1234567890',
  'user-agent': 'arduino-WebSocket-Client' }
Message from Client :  { now: 30001 }
Message from Client :  { now: 60002 }
Message from Client :  { now: 90003 }
```

---

#### 13. [Portenta_H7_WebSocketClientSocketIO](examples/Portenta_H7/Ethernet/Portenta_H7_WebSocketClientSocketIO) on PORTENTA_H7_M7 with Ethernet

The following are debug terminal output when running example [Portenta_H7_WebSocketClientSocketIO](examples/Portenta_H7/Ethernet/Portenta_H7_WebSocketClientSocketIO) on **PORTENTA_H7_M7 with Ethernet** using Portenta_Ethernet Library


#### 13.1 Client

```
Start Portenta_H7_WebSocketClientSocketIO on PORTENTA_H7_M7 with Ethernet using Portenta_Ethernet Library
WebSockets_Generic v2.16.0
WebSockets Client @ IP address: 192.168.2.132
Connecting to WebSockets Server @ IP address: 192.168.2.30, port: 8080
[WS] [WS-Client][connectedCb] Connected to Host:192.168.2.30, Port:8080
[WS] [wsIOc] Connected to url:/socket.io/?EIO=4
[IOc] Connected to url: /socket.io/?EIO=4
[WS] [wsIOc] get pong
[IOc] Get PONG
[WS] [wsIOc] connected: len = 30
[WS] [wsIOc] data: {"sid":"RqchvIdY1DWbvxTpAAAB"}
["event_name",{"now":30001}]
[WS] [wsIOc] get event: len = 53
[WS] [wsIOc] get data: ["Send Message io.emit Broadcasted : ",{"now":30001}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":30001}]
[WS] [wsIOc] get event: len = 33
[WS] [wsIOc] get data: ["Send Message : ",{"now":30001}]
[IOc] Get event: ["Send Message : ",{"now":30001}]
[WS] [wsIOc] get ping send pong:3
[IOc] Get PING
[WS] [WS-Client] Sending HB ping
["event_name",{"now":60006}]
[WS] [wsIOc] get event: len = 53
[WS] [wsIOc] get data: ["Send Message io.emit Broadcasted : ",{"now":60006}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":60006}]
[WS] [wsIOc] get event: len = 33
[WS] [wsIOc] get data: ["Send Message : ",{"now":60006}]
[IOc] Get event: ["Send Message : ",{"now":60006}]
[WS] [wsIOc] get ping send pong:3
```

---

#### 13.2 Server

Using the Socket.IO Server in [**Nodejs_SocketIO_TestServer**](examples/Nodejs_SocketIO_TestServer)

```
kh@kh-Inspiron-3593:~/Downloads/Nodejs/New_SIO$ ./SIO_server.sh 
Server launched on port 8080
Connected
RqchvIdY1DWbvxTpAAAB
JWT token test:  { host: '192.168.2.30:8080',
  connection: 'keep-alive',
  authorization: '1234567890',
  'user-agent': 'arduino-WebSocket-Client' }
Message from Client :  { now: 30001 }
Message from Client :  { now: 60006 }

```

---

#### 14. [WebSocketClientSocketIO_WiFi](examples/Portenta_H7/WiFi/WebSocketClientSocketIO_WiFi) on Portenta_H7 using WiFi

The following are debug terminal output when running example [WebSocketClientSocketIO_WiFi](examples/Portenta_H7/WiFi/WebSocketClientSocketIO_WiFi) on Portenta_H7 using `Murata` WiFi


#### 14.1 Client

```
Start WebSocketClientSocketIO_WiFi on PORTENTA_H7_M7
WebSockets_Generic v2.16.0
Connecting to SSID: HueNet1
SSID: HueNet1
WebSockets Client IP Address: 192.168.2.130
signal strength (RSSI):-27 dBm
Connecting to WebSockets Server @ IP address: 192.168.2.30, port: 8080
[IOc] Connected to url: /socket.io/?EIO=4
["event_name",{"now":30001}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":30001}]
[IOc] Get event: ["Send Message : ",{"now":30001}]
["event_name",{"now":60002}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":60002}]
[IOc] Get event: ["Send Message : ",{"now":60002}]
["event_name",{"now":90003}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":90003}]
[IOc] Get event: ["Send Message : ",{"now":90003}]
["event_name",{"now":121501}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":121501}]
[IOc] Get event: ["Send Message : ",{"now":121501}]
["event_name",{"now":151503}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":151503}]
[IOc] Get event: ["Send Message : ",{"now":151503}]
```

---

#### 14.2 Server

Using the Socket.IO Server in [**Nodejs_SocketIO_TestServer**](examples/Nodejs_SocketIO_TestServer)

```
kh@kh-Inspiron-3593:~/Downloads/Nodejs/New_SIO$ ./SIO_server.sh 
Server launched on port 8080
Connected
nr4syN7HdG1PlCYrAAAF
JWT token test:  { host: '192.168.2.30:8080',
  connection: 'keep-alive',
  authorization: '1234567890',
  'user-agent': 'arduino-WebSocket-Client' }
Message from Client :  { now: 30001 }
Message from Client :  { now: 60002 }
Message from Client :  { now: 90003 }
Message from Client :  { now: 121501 }
Message from Client :  { now: 151503 }
Message from Client :  { now: 181506 }
```

---

#### 15. [Portenta_H7_WebSocketClient_Sticky_SocketIO](examples/Portenta_H7/Ethernet/Portenta_H7_WebSocketClient_Sticky_SocketIO) on PORTENTA_H7_M7 using Ethernet

The following are debug terminal output when running example [Portenta_H7_WebSocketClient_Sticky_SocketIO](examples/Portenta_H7/Ethernet/Portenta_H7_WebSocketClient_Sticky_SocketIO) on Portenta_H7 using Vision-shield Ethernet.


```
Start Portenta_H7_WebSocketClient_Sticky_SocketIO on PORTENTA_H7_M7 with Ethernet using Portenta_Ethernet Library
WebSockets_Generic v2.16.0
WebSockets Client @ IP address: 192.168.2.133
Connecting to WebSockets Server @ IP address: 192.168.2.30, port: 8080
[WS] WebSockets_Generic v2.16.0
[WS] [wsIOc] found EIO=4 disable EIO ping on client
[WS] [WS-Client][connectedCb] Connected to Host:192.168.2.30, Port:8080
[WS] [WS-Client] [sendHeader] Sending header...
[WS] sendHeader: client->cKey = Kp/b0RFGMw/WSNGn35/s3A==
[WS] [WS-Client] [sendHeader] Handshake:GET /socket.io/?EIO=4&transport=websocket HTTP/1.1
Host: 192.168.2.30:8080
Connection: keep-alive
Authorization: 1234567890
User-Agent: arduino-WebSocket-Client


[WS] [WS-Client] [sendHeader] Sending header... Done (us):712
[WS] [WS-Client][handleHeader] RX:HTTP/1.1 200 OK
[WS] [WS-Client][handleHeader] RX:Date: Sun, 12 Dec 2021 05:58:29 GMT
[WS] [WS-Client][handleHeader] RX:Connection: keep-alive
[WS] [WS-Client][handleHeader] RX:Keep-Alive: timeout=5
[WS] [WS-Client][handleHeader] RX:Transfer-Encoding: chunked
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL:/socket.io/?EIO=4
[WS] [WS-Client][handleHeader] - cKey:Kp/b0RFGMw/WSNGn35/s3A==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode:200
[WS] [WS-Client][handleHeader] - cIsUpgrade:0
[WS] [WS-Client][handleHeader] - cIsWebsocket:1
[WS] [WS-Client][handleHeader] - cAccept:
[WS] [WS-Client][handleHeader] - cProtocol:arduino
[WS] [WS-Client][handleHeader] - cExtensions:
[WS] [WS-Client][handleHeader] - cVersion:0
[WS] [WS-Client][handleHeader] - cSessionId:
[WS] [WS-Client][handleHeader] Still missing cSessionId try Socket.IO
[WS] [WS-Client][handleHeader] socket.io json: c
hello world

0
[WS] [WS-Client][handleHeader] RX:c
hello world

0
[WS] [WS-Client][handleHeader] Header error:c
hello world

0
[WS] [WS-Client][handleClientData] Header response timeout.. Disconnecting!
[IOc] Disconnected
[WS] [wsIOc] Disconnected!
[WS] [WS-Client][connectedCb] Connected to Host:192.168.2.30, Port:8080
[WS] [WS-Client] [sendHeader] Sending header...
[WS] sendHeader: client->cKey = dm6SsOCMRZwhNjKmQeo2lw==
[WS] [WS-Client] [sendHeader] Handshake:GET /socket.io/?EIO=4&transport=websocket HTTP/1.1
Host: 192.168.2.30:8080
Connection: keep-alive
Authorization: 1234567890
User-Agent: arduino-WebSocket-Client
```



---

#### 16. [ESP32_WebSocketClientSocketIO](examples/esp32/ESP32_WebSocketClientSocketIO) on ESP32S3_DEV

The following are debug terminal output when running example [ESP32_WebSocketClientSocketIO](examples/esp32/ESP32_WebSocketClientSocketIO) on **ESP32S3_DEV**


```
Start ESP32_WebSocketClientSocketIO on ESP32S3_DEV
WebSockets_Generic v2.16.0
Connecting to HueNet1
WebSockets Client started @ IP address: 192.168.2.83
Connecting to WebSockets Server @ IP address: 192.168.2.30, port: 8080
[WS] [WS-Client] beginSocketIO with IPAddress
[WS] [WS-Client] beginSocketIO with const char
[WS] WebSockets_Generic v2.12.0
[WS] [wsIOc] found EIO=4 disable EIO ping on client
[WS] [WS-Client] Connect ws...
[WS] [WS-Client] Calling _client.tcp->connect, _host =192.168.2.30, port =8080
[WS] [WS-Client] Calling _client.tcp->connect, _connectResult =1
[WS] [WS-Client] connectedCb
[WS] [WS-Client][connectedCb] Connected to Host:192.168.2.30, Port:8080
[WS] [WS-Client] [sendHeader] Sending header...
[WS] sendHeader: client->cKey = 8FGfIMaycpbZsGx1WcjYPg==
[WS] [WS-Client] [sendHeader] Handshake:GET /socket.io/?EIO=4&transport=polling HTTP/1.1
Host: 192.168.2.30:8080
Connection: keep-alive
Authorization: 1234567890
User-Agent: arduino-WebSocket-Client

[WS] [write] n:166, t:10047
[WS] [write] Write, Length :166, Left :0
[WS] [WS-Client] [sendHeader] Sending header... Done (us):32263
[WS] [WS-Client][handleHeader] RX:HTTP/1.1 200 OK
[WS] [WS-Client][handleHeader] RX:Content-Type: text/plain; charset=UTF-8
[WS] [WS-Client][handleHeader] RX:Content-Length: 97
[WS] [WS-Client][handleHeader] RX:Date: Mon, 14 Feb 2022 23:52:21 GMT
[WS] [WS-Client][handleHeader] RX:Connection: keep-alive
[WS] [WS-Client][handleHeader] RX:Keep-Alive: timeout=5
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL:/socket.io/?EIO=4
[WS] [WS-Client][handleHeader] - cKey:8FGfIMaycpbZsGx1WcjYPg==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode:200
[WS] [WS-Client][handleHeader] - cIsUpgrade:0
[WS] [WS-Client][handleHeader] - cIsWebsocket:1
[WS] [WS-Client][handleHeader] - cAccept:
[WS] [WS-Client][handleHeader] - cProtocol:arduino
[WS] [WS-Client][handleHeader] - cExtensions:
[WS] [WS-Client][handleHeader] - cVersion:0
[WS] [WS-Client][handleHeader] - cSessionId:
[WS] [WS-Client][handleHeader] Still missing cSessionId try Socket.IO
[WS] [WS-Client][handleHeader] socket.io json: 0{"sid":"p6g2ESTze400qtXcAAAE","upgrades":["websocket"],"pingInterval":25000,"pingTimeout":20000}
[WS] [WS-Client][handleHeader] - cSessionId: p6g2ESTze400qtXcAAAE
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL:/socket.io/?EIO=4
[WS] [WS-Client][handleHeader] - cKey:8FGfIMaycpbZsGx1WcjYPg==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode:200
[WS] [WS-Client][handleHeader] - cIsUpgrade:0
[WS] [WS-Client][handleHeader] - cIsWebsocket:1
[WS] [WS-Client][handleHeader] - cAccept:
[WS] [WS-Client][handleHeader] - cProtocol:arduino
[WS] [WS-Client][handleHeader] - cExtensions:
[WS] [WS-Client][handleHeader] - cVersion:0
[WS] [WS-Client][handleHeader] - cSessionId:p6g2ESTze400qtXcAAAE
[WS] [WS-Client][handleHeader] found cSessionId
[WS] [WS-Client] [sendHeader] Sending header...
[WS] sendHeader: client->cKey = 2btvEqbCEn69Vmh40QfIaA==
[WS] [WS-Client] [sendHeader] Handshake:GET /socket.io/?EIO=4&transport=websocket&sid=p6g2ESTze400qtXcAAAE HTTP/1.1
Host: 192.168.2.30:8080
Connection: Upgrade
Upgrade: websocket
Sec-WebSocket-Version: 13
Sec-WebSocket-Key: 2btvEqbCEn69Vmh40QfIaA==
Sec-WebSocket-Protocol: arduino
Authorization: 1234567890
User-Agent: arduino-WebSocket-Client

[WS] [write] n:315, t:10268
[WS] [write] Write, Length :315, Left :0
[WS] [WS-Client] [sendHeader] Sending header... Done (us):42597
[WS] [WS-Client][handleHeader] RX:HTTP/1.1 101 Switching Protocols
[WS] [WS-Client][handleHeader] RX:Upgrade: websocket
[WS] [WS-Client][handleHeader] RX:Connection: Upgrade
[WS] [WS-Client][handleHeader] RX:Sec-WebSocket-Accept: MokU7OGA/lZbCC0KeEZT6+rp5l0=
[WS] [WS-Client][handleHeader] RX:Sec-WebSocket-Protocol: arduino
[WS] [WS-Client][handleHeader] Header read fin.
[WS] [WS-Client][handleHeader] Client settings:
[WS] [WS-Client][handleHeader] - cURL:/socket.io/?EIO=4
[WS] [WS-Client][handleHeader] - cKey:2btvEqbCEn69Vmh40QfIaA==
[WS] [WS-Client][handleHeader] Server header:
[WS] [WS-Client][handleHeader] - cCode:101
[WS] [WS-Client][handleHeader] - cIsUpgrade:1
[WS] [WS-Client][handleHeader] - cIsWebsocket:1
[WS] [WS-Client][handleHeader] - cAccept:MokU7OGA/lZbCC0KeEZT6+rp5l0=
[WS] [WS-Client][handleHeader] - cProtocol:arduino
[WS] [WS-Client][handleHeader] - cExtensions:
[WS] [WS-Client][handleHeader] - cVersion:0
[WS] [WS-Client][handleHeader] - cSessionId:p6g2ESTze400qtXcAAAE
[WS] [WS-Client][handleHeader] Websocket connection init done.
[WS] [headerDone] Header Handling Done. Client:0
[WS] [wsIOc] Connected to url:/socket.io/?EIO=4
[WS] [sendFrame] ------- send message frame -------
[WS] [sendFrame] Client:0
[WS] fin:1
[WS] opCode:1
[WS] mask:1
[WS] length:6
[WS] headerToPayload:0
[WS] [sendFrame] Client: 0, text:2probe
[WS] [sendFrame] pack to one TCP package... Client:0
[WS] [write] n:12, t:10411
[WS] [write] Write, Length :12, Left :0
[WS] [sendFrame] ------- send message frame -------
[WS] [sendFrame] Client:0
[WS] fin:1
[WS] opCode:1
[WS] mask:1
[WS] length:1
[WS] headerToPayload:1
[WS] [sendFrame] Client: 0, text:5
[WS] [write] n:7, t:10433
[WS] [write] Write, Length :7, Left :0
[IOc] Connected to url: /socket.io/?EIO=4
[WS] [write] n:6, t:10435
[WS] [write] Write, Length :6, Left :0
[WS] [write] n:2, t:10446
[WS] [write] Write, Length :2, Left :0
[WS] [write] n:1, t:10447
[WS] [write] Write, Length :1, Left :0
[WS] [handleWebsocketWaitFor] Client: 0, size:2
[WS] cWsRXsize:0
[WS] [readCb] n:2, t:10459
[WS] [readCb] Receive Length =2, left =0
[WS] [handleWebsocketWaitFor][readCb] Client: 0, size:2
[WS] ok:1
[WS] [handleWebsocket] ------- read massage frame ------- Client:0
[WS] [handleWebsocket] Client: 0, fin:1
[WS] rsv1:0
[WS] rsv2:0
[WS] rsv3:0
[WS] opCode:1
[WS] [handleWebsocket] Client: 0, mask:0
[WS] payloadLen:6
[WS] [readCb] n:6, t:10491
[WS] [readCb] Receive Length =6, left =0
[WS] [handleWebsocketPayloadCb] Client: 0, text:3probe
[WS] [wsIOc] get pong
[IOc] Get PONG
[WS] [handleWebsocketWaitFor] Client: 0, size:2
[WS] cWsRXsize:0
[WS] [readCb] n:2, t:10512
[WS] [readCb] Receive Length =2, left =0
[WS] [handleWebsocketWaitFor][readCb] Client: 0, size:2
[WS] ok:1
[WS] [handleWebsocket] ------- read massage frame ------- Client:0
[WS] [handleWebsocket] Client: 0, fin:1
[WS] rsv1:0
[WS] rsv2:0
[WS] rsv3:0
[WS] opCode:1
[WS] [handleWebsocket] Client: 0, mask:0
[WS] payloadLen:32
[WS] [readCb] n:32, t:10544
[WS] [readCb] Receive Length =32, left =0
[WS] [handleWebsocketPayloadCb] Client: 0, text:40{"sid":"356v34mdH9BycjJpAAAF"}
[WS] [wsIOc] connected: len = 30
[WS] [wsIOc] data: {"sid":"356v34mdH9BycjJpAAAF"}
```

---

#### 17. [ESP8266_AsyncWSClient](examples/Async/ESP8266_AsyncWSClient) on ESP8266_NODEMCU_ESP12E

The following are debug terminal output when running example [ESP8266_AsyncWSClient](examples/Async/ESP8266_AsyncWSClient) on **ESP8266_NODEMCU_ESP12E**


```
Start ESP8266_AsyncWSClient on ESP8266_NODEMCU_ESP12E
WebSockets_Generic v2.16.0

WebSockets Client started @ IP address: 192.168.2.145
Connecting to WebSockets Server @ 192.168.2.30
Connected to WebSockets Server @ IP address: 192.168.2.30
["From Async ESP8266_NODEMCU_ESP12E",{"now":2070}]
[WS] [WS-Client][connectedCb] Connected to Host:192.168.2.30, Port:8080
[WSc] Connected to url: /
[WSc] get text: Connected
[WSc] get ping
["From Async ESP8266_NODEMCU_ESP12E",{"now":32075}]
[WSc] get text: ["From Async Async ESP8266_NODEMCU_ESP12E",{"now":32075}]
[WSc] get ping
["From Async Async ESP8266_NODEMCU_ESP12E",{"now":62077}]
[WSc] get text: ["From Async ESP8266_NODEMCU_ESP12E",{"now":62077}]
[WSc] get ping
["From Async ESP8266_NODEMCU_ESP12E",{"now":92079}]
[WSc] get text: ["From Async ESP8266_NODEMCU_ESP12E",{"now":92079}]
[WSc] get ping
["From Async ESP8266_NODEMCU_ESP12E",{"now":122081}]
[WSc] get text: ["From Async ESP8266_NODEMCU_ESP12E",{"now":122081}]
[WSc] get ping
["From Async ESP8266_NODEMCU_ESP12E",{"now":152083}]
[WSc] get text: ["From Async ESP8266_NODEMCU_ESP12E",{"now":152083}]
[WSc] get ping
["From Async ESP8266_NODEMCU_ESP12E",{"now":182085}]
[WSc] get text: ["From Async ESP8266_NODEMCU_ESP12E",{"now":182085}]
[WSc] get ping
["From Async ESP8266_NODEMCU_ESP12E",{"now":212087}]
[WSc] get text: ["From Async ESP8266_NODEMCU_ESP12E",{"now":212087}]
[WSc] get ping
["From Async ESP8266_NODEMCU_ESP12E",{"now":242089}]
[WSc] get text: ["From Async ESP8266_NODEMCU_ESP12E",{"now":242089}]
```

---

#### 18. [WebSocketClientSocketIO_RP2040W](examples/RP2040W/WebSocketClientSocketIO_RP2040W) on RP2040W using WiFi

The following are debug terminal output when running example [WebSocketClientSocketIO_RP2040W](examples/RP2040W/WebSocketClientSocketIO_RP2040W) on RP2040W using `CYW43439` WiFi


#### 18.1 Client

```
Start WebSocketClientSocketIO_RP2040W on RASPBERRY_PI_PICO_W
WebSockets_Generic v2.16.0
Connecting to SSID: HueNet1
SSID: HueNet1
WebSockets Client IP Address: 192.168.2.77
signal strength (RSSI):0 dBm
Connecting to WebSockets Server @ IP address: 192.168.2.30, port: 8080
[WS] [WS-Client] Connect ws...
[WS] [WS-Client][connectedCb] Connected to Host:192.168.2.30, Port:8080
[WS] [wsIOc] Connected to url:/socket.io/?EIO=4
[IOc] Connected to url: /socket.io/?EIO=4
[WS] [wsIOc] get pong
[IOc] Get PONG
[WS] [wsIOc] connected: len = 30
[WS] [wsIOc] data: {"sid":"M2cFjvQhLKXc6aB4AAAB"}
["event_name",{"now":30001}]
[WS] [wsIOc] get event: len = 53
[WS] [wsIOc] get data: ["Send Message io.emit Broadcasted : ",{"now":30001}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":30001}]
[WS] [wsIOc] get event: len = 33
[WS] [wsIOc] get data: ["Send Message : ",{"now":30001}]
[IOc] Get event: ["Send Message : ",{"now":30001}]
[WS] [wsIOc] get ping send pong:3
[IOc] Get PING
[WS] [WS-Client] Sending HB ping
["event_name",{"now":60002}]
[WS] [wsIOc] get event: len = 53
[WS] [wsIOc] get data: ["Send Message io.emit Broadcasted : ",{"now":60002}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":60002}]
[WS] [wsIOc] get event: len = 33
[WS] [wsIOc] get data: ["Send Message : ",{"now":60002}]
[IOc] Get event: ["Send Message : ",{"now":60002}]
[WS] [wsIOc] get ping send pong:3
[IOc] Get PING
```

---

#### 18.2 Server

Using the Socket.IO Server in [**Nodejs_SocketIO_TestServer**](examples/Nodejs_SocketIO_TestServer)

```
kh@kh-Inspiron-3593:~/Downloads/Nodejs/New_SIO$ ./SIO_server.sh 
Server launched on port 8080
Connected
M2cFjvQhLKXc6aB4AAAB
JWT token test:  {
  host: '192.168.2.30:8080',
  connection: 'keep-alive',
  authorization: '1234567890',
  'user-agent': 'arduino-WebSocket-Client'
}
Message from Client :  { now: 30001 }
Message from Client :  { now: 60002 }
```

---

#### 19. [Generic_WebSocketClientSocketIO_WiFiNINA](examples/Generic/WiFiNINA/Generic_WebSocketClientSocketIO_WiFiNINA) on Nano_RP2040_Connect using WiFiNINA

The following are debug terminal output when running example [Generic_WebSocketClientSocketIO_WiFiNINA](examples/Generic/WiFiNINA/Generic_WebSocketClientSocketIO_WiFiNINA) on `Nano_RP2040_Connect` using `WiFiNINA`


```
Start Generic_WebSocketClientSocketIO_WiFiNINA on Nano RP2040 Connect
WebSockets_Generic v2.16.0
Please upgrade the firmware
Attempting to connect to SSID: HueNet1
SSID: HueNet1
WebSockets Client IP Address: 192.168.2.105
signal strength (RSSI):-31 dBm
Connecting to WebSockets Server @ IP address: 192.168.2.30, port: 8080
[WS] [WS-Client] Connect ws...
[WS] [WS-Client][connectedCb] Connected to Host:192.168.2.30, Port:8080
[WS] [wsIOc] Connected to url:/socket.io/?EIO=4
[IOc] Connected to url: /socket.io/?EIO=4
[WS] [wsIOc] get pong
[IOc] Get PONG
[WS] [wsIOc] connected: len = 30
[WS] [wsIOc] data: {"sid":"m0VRTQ6jk6LggxnTAAAF"}
["event_name",{"now":30001}]
[WS] [wsIOc] get event: len = 57
[WS] [wsIOc] get data: ["Send Message socket.broadcast.emit : ",{"now":1950065}]
[IOc] Get event: ["Send Message socket.broadcast.emit : ",{"now":1950065}]
[WS] [wsIOc] get event: len = 55
[WS] [wsIOc] get data: ["Send Message io.emit Broadcasted : ",{"now":1950065}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":1950065}]
[WS] [wsIOc] get event: len = 53
[WS] [wsIOc] get data: ["Send Message io.emit Broadcasted : ",{"now":30001}]
[IOc] Get event: ["Send Message io.emit Broadcasted : ",{"now":30001}]
[WS] [wsIOc] get event: len = 33
[WS] [wsIOc] get data: ["Send Message : ",{"now":30001}]
[IOc] Get event: ["Send Message : ",{"now":30001}]
[WS] [wsIOc] get ping send pong:3
[IOc] Get PING

```

---
---

### Debug

Debug is enabled by default on Serial. To disable, add at the beginning of sketch

```cpp
/* Comment this out to disable prints and save space */
#define DEBUG_WEBSOCKETS_PORT         Serial

#define _WEBSOCKETS_LOGLEVEL_         4
```

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the board's core, dependent libraries or this library version.

Sometimes, the library will only work if you update the core to the newer or older version because some function compatibility.

---
---

### Issues

Submit issues to: [WebSockets_Generic issues](https://github.com/khoih-prog/WebSockets_Generic/issues)

---
---

### TO DO

1. Bug Searching and Killing
2. Support more types of boards using WiFi/Ethernet shields (Teensy, etc.)
3. Support more non-compatible Ethernet Libraries such as Ethernet_Shield_W5200, EtherCard, EtherSia
4. Support **ASYNC** mode for boards using communication libraries supporting Async (**STM32, Portenta, Teensy 4.1, ESP32, etc.**)

---

### DONE

 1. Add support to Arduino SAMD21 (ZERO, MKR, NANO_33_IOT, etc.)
 2. Add support to Adafruit SAMD21 (Itsy-Bitsy M0, Metro M0, Feather M0 Express, etc.).
 3. Add support to Adafruit SAMD51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.).
 4. Add support to Adafruit nRF52 ( Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox, etc.
 5. Add support to Seeeduino SAMD21/SAMD51 boards (SEEED_WIO_TERMINAL, SEEED_FEMTO_M0, SEEED_XIAO_M0, Wio_Lite_MG126, WIO_GPS_BOARD, SEEEDUINO_ZERO, SEEEDUINO_LORAWAN, SEEED_GROVE_UI_WIRELESS, etc.)
 6. Add support to STM32F/L/H/G/WB/MP1 (Nucleo-144, Nucleo-64, Nucleo-32, Discovery, STM32Fx, STM32H7, STM32Lx, STM32Gx, STM32WB, STM32MP1, etc.) having 64K+ Flash program memory.
 7. Add support to Ethernet W5x00, using either [`Ethernet`](https://www.arduino.cc/en/Reference/Ethernet), [`Ethernet2`](https://github.com/khoih-prog/Ethernet2), [`Ethernet3`](https://github.com/sstaub/Ethernet3) or [`EthernetLarge`](https://github.com/OPEnSLab-OSU/EthernetLarge) library
 8. Add support to WiFiNINA using WiFiNINA or [**WiFiNINA_Generic library**](https://github.com/khoih-prog/WiFiNINA_Generic).
 9. Add support to ENC28J60 using [`UIPEthernet`](https://github.com/UIPEthernet/UIPEthernet)
10. Add support to ENC28J60 using [`EthernetENC`](https://github.com/jandrassy/EthernetENC)
11. Add support to Teensy using W5x00, ENC28J60, WiFiNINA
12. Add support to Teensy 4.1 built-in Ethernet using [`NativeEthernet`](https://github.com/vjmuzik/NativeEthernet) library
13. Add support to STM32F/L/H/G/WB/MP1 using built-in LAN8742A and [`STM32Ethernet`](https://github.com/stm32duino/STM32Ethernet)
14. Add support to SeeedStudio WIO Terminal with **Realtek RTL8720DN WiFi** using [`Seeed_Arduino_rpcWiFi`](https://github.com/Seeed-Studio/Seeed_Arduino_rpcWiFi) and [`Seeed_Arduino_rpcUnified`](https://github.com/khoih-prog/Seeed_Arduino_rpcUnified) libraries
15. Add support to SAMD_MKR1000 and SAMD_MKRWIFI1010 using WiFi101
16. Fix breaking problem with SocketIO. 
17. Default to EIOv4
18. Add support to RP2040-based boards, such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Earle Philhower's arduino-pico core** v1.3.1+](https://github.com/earlephilhower/arduino-pico).
19. Add support to RP2040-based boards, such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Arduino-mbed RP2040** core](https://github.com/arduino/ArduinoCore-mbed).
20. Add support to **WT32_ETH01 boards** using ESP32-based boards and LAN8720 Ethernet
21. Add support to Teensy 4.1 built-in Ethernet using [`QNEthernet`](https://github.com/ssilverman/QNEthernet) library
22. Add support to **Portenta_H7**, using either `Murata WiFi` or `Vision-shield Ethernet`
23. Auto detect ESP32 core to use for WT32_ETH01
24. Fix bug in WT32_ETH01 examples to reduce connection time
25. Add option to use transport=websocket with sticky-session SIO server
26. Add SSL support to ESP32-based WT32_ETH01 boards
27. Add support to ESP32_S3, ESP32_C3
28. Add `sIOtype_PING` and `sIOtype_PING` SocketIO events.
29. Don't use `deprecated sha.h` for ESP32 core v1.0.6+.
30. Optimize code by passing by `reference` instead of `value`
31. Fix ASYNC bug for ESP8266 when using **NETWORK_ESP8266_ASYNC**
32. Use new [**Ethernet_Generic** library](https://github.com/khoih-prog/Ethernet_Generic) as default for W5x00.
33. Add support to **RP2040-based boards, such as Nano_RP2040_Connect, RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040**, using `WiFiNINA` or `Ethernet`
34. Add `WS` and `BearSSL WSS` support to `RP2040W` using `CYW43439 WiFi` with `arduino-pico` core
35. Optional user-defined 
- `WEBSOCKETS_TCP_TIMEOUT`, default 5,000ms
- `EIO_HEARTBEAT_INTERVAL`, default 20,000ms
- `SIO_PING_INTERVAL`, default 60,000ms
- `SIO_PONG_TIMEOUT`, default 90,000ms
- `SIO_DISCONNECT_TIMEOUT_COUNT`, default 5
 
---
---

### Contributions and Thanks

1. Based on and modified from [Markus Sattler's **arduinoWebSockets** library](https://github.com/Links2004/arduinoWebSockets)
2. Thanks to [Anson He](https://github.com/ansonhe97) for the PR [**Fix compile error for Wio Terminal**](https://github.com/khoih-prog/WebSockets_Generic/pull/5) to fix SeeedStudio **SEEED_WIO_TERMINAL** compile errors and leading to new version v2.3.3.
3. Thanks to [Jose A.Molina](https://github.com/Josua2012) for the enhancement request [**WiFi101 and Arduino MKR1000.** #10](https://github.com/khoih-prog/WebSockets_Generic/issues/10), and help testing, leading to new version v2.5.0, and v2.6.0
4. Thanks to [Ideen Kaffee Korner](https://github.com/iKK001) for 
  - the enhancement request [**Run websocket only mode for Socket.IO** #16](https://github.com/khoih-prog/WebSockets_Generic/issues/16), leading to new version v2.11.1
  - the issue [Ethernet with SSL #17](https://github.com/khoih-prog/WebSockets_Generic/issues/17), leading to new version v2.12.0 to support SSL for WT32_ETH01 using LAN8720 Ethernet
  - the issue [Ping events propagated to main code socketIO events #18](https://github.com/khoih-prog/WebSockets_Generic/issues/18), leading to new version v2.13.0 to add `sIOtype_PING` and `sIOtype_PING` SocketIO events

<table>
  <tr>
    <td align="center"><a href="https://github.com/Links2004"><img src="https://github.com/Links2004.png" width="100px;" alt="Links2004"/><br /><sub><b>⭐️⭐️ Markus Sattler</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/ansonhe97"><img src="https://github.com/ansonhe97.png" width="100px;" alt="ansonhe97"/><br /><sub><b>Anson He</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/Josua2012"><img src="https://github.com/Josua2012.png" width="100px;" alt="Josua2012"/><br /><sub><b>Jose A. Molina</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/iKK001"><img src="https://github.com/iKK001.png" width="100px;" alt="iKK001"/><br /><sub><b>⭐️ Ideen Kaffee Korner</b></sub></a><br /></td>
  </tr> 
</table>

---

### Contributing

If you want to contribute to this project:

- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---
---

### License and credits

- Most of the credits go to original author [**Markus Sattler**](https://github.com/Links2004).

- The library is licensed under [**GPLv3**](https://github.com/khoih-prog/WebSockets_Generic/blob/master/LICENSE)

- [libb64](http://libb64.sourceforge.net/) written by Chris Venter. It is distributed under Public Domain see [LICENSE](https://github.com/Links2004/arduinoWebSockets/blob/master/src/libb64/LICENSE).


---

## Copyright

1. Copyright (c) 2016- Markus Sattler
2. Copyright (c) 2020- Khoi Hoang
