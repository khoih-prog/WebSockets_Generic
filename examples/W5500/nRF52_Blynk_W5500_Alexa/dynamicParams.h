/****************************************************************************************************************************
  dynamicParams.h for nRF52_Blynk_W5500_Alexa.ino
  For nRF52 using W5x00 Ethernet Shield/Module
  
  BlynkEthernet_WM is a library for Teensy, ESP, SAM DUE and SAMD boards, with Ethernet W5X00 or ENC28J60 shields,
  to enable easy configuration/reconfiguration and autoconnect/autoreconnect of Ethernet/Blynk
  AVR Mega and W5100 is not supported.
  Library modified from Blynk library v0.6.1 https://github.com/blynkkk/blynk-library/releases
  Built by Khoi Hoang https://github.com/khoih-prog/BlynkEthernet_WM
  Licensed under MIT license
  
  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.
  
  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license                  
 *****************************************************************************************************************************/

#ifndef dynamicParams_h
#define dynamicParams_h

#if USE_BLYNK_WM

#define USE_DYNAMIC_PARAMETERS      true

/////////////// Start dynamic Credentials ///////////////

//Defined in <BlynkEthernet_NRF52_WM.h>
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
