/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME  "usuario"
#define IO_KEY       "llave"

/******************************* WIFI **************************************/
#define WIFI_SSID "wifi_name"
#define WIFI_PASS "wifi_password"

#include "AdafruitIO_WiFi.h"


AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);