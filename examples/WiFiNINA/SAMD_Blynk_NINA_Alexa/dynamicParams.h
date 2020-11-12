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
