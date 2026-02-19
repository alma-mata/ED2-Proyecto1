/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME  "user"
#define IO_KEY       "password"

/******************************* WIFI **************************************/
#define WIFI_SSID "wifi"
#define WIFI_PASS "password"

#include "AdafruitIO_WiFi.h"


AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);