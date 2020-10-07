/****************************************************************************************************************************
  dynamicParams.h for SAMD_Blynk_WM_Alexa.ino
  For SAMD21/SAMD51 using WiFiNINA Shield/Module
  
  Blynk_WiFiNINA_WM is a library for the Mega, Teensy, SAM DUE, nRF52, STM32 and SAMD boards
  (https://github.com/khoih-prog/Blynk_WiFiNINA_WM) to enable easy configuration/reconfiguration and
  autoconnect/autoreconnect of WiFiNINA/Blynk
  
  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.
  
  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license
  Version: 2.3.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  2.1.3   K Hoang      15/05/2020 Initial porting to support SAMD21, SAMD51, nRF52 boards, such as AdaFruit Feather nRF52832,
                                  nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, etc.
  2.2.1   K Hoang      18/05/2020 Bump up to sync with v2.2.1 of original WebSockets library
  2.2.2   K Hoang      25/05/2020 Add support to Teensy, SAM DUE and STM32. Enable WebSocket Server for new supported boards.
  2.2.3   K Hoang      02/08/2020 Add support to W5x00's Ethernet2, Ethernet3, EthernetLarge Libraries. 
                                  Add support to STM32F/L/H/G/WB/MP1 and Seeeduino SAMD21/SAMD51 boards.
  2.3.1   K Hoang      07/10/2020 Sync with v2.3.1 of original WebSockets library. Add ENC28J60 EthernetENC library support
 *****************************************************************************************************************************/

#ifndef dynamicParams_h
#define dynamicParams_h

#if USE_BLYNK_WM

#define USE_DYNAMIC_PARAMETERS      true

/////////////// Start dynamic Credentials ///////////////

//Defined in <BlynkSimpleWiFiNINA_SAMD_WM.h>
/**************************************
  #define MAX_ID_LEN                5
  #define MAX_DISPLAY_NAME_LEN      16

  typedef struct
  {
  char id             [MAX_ID_LEN + 1];
  char displayName    [MAX_DISPLAY_NAME_LEN + 1];
  char *pdata;
  uint8_t maxlen;
  } MenuItem;
**************************************/

#if USE_DYNAMIC_PARAMETERS

#define MAX_SINRIC_API_KEY_LEN      36
char SINRIC_API_KEY  [MAX_SINRIC_API_KEY_LEN + 1]   = "****";

#define MAX_SINRIC_DEVICE_ID_LEN        24
char SINRIC_Device_ID_1   [MAX_SINRIC_DEVICE_ID_LEN + 1]  = "****";

MenuItem myMenuItems [] =
{
  { "sapi", "SINRIC API Key",  SINRIC_API_KEY,      MAX_SINRIC_API_KEY_LEN },
  { "sid1", "Device_ID1",      SINRIC_Device_ID_1,  MAX_SINRIC_DEVICE_ID_LEN },
};

uint16_t NUM_MENU_ITEMS = sizeof(myMenuItems) / sizeof(MenuItem);  //MenuItemSize;

#else

MenuItem myMenuItems [] = {};

uint16_t NUM_MENU_ITEMS = 0;

#endif    //USE_DYNAMIC_PARAMETERS

/////// // End dynamic Credentials ///////////

#else

// Not use WM
#define MAX_SINRIC_API_KEY_LEN      36
char SINRIC_API_KEY  [MAX_SINRIC_API_KEY_LEN + 1]   = "****";

#define MAX_SINRIC_DEVICE_ID_LEN        24
char SINRIC_Device_ID_1   [MAX_SINRIC_DEVICE_ID_LEN + 1]  = "****";

#endif      //USE_BLYNK_WM

#endif      //dynamicParams_h
