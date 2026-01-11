// NOTE: Create secrets.h from secrets.h.example with your WiFi credentials
#include "secrets.h"

// Include the Arduino Stepper Library
#include <Stepper.h>

// Number of steps per output rotation
const int stepsPerRevolution = 1200;

// Create Instance of Stepper library
Stepper myStepper(stepsPerRevolution, 8, 9, 11, 12);

#include <Adafruit_CC3000.h>
#include <SPI.h>
#include "utility/debug.h"
#include "utility/socket.h"
#include <Adafruit_CC3000.h>
#include <Adafruit_CC3000_Server.h>
#include <SPI.h>



/* --- CC3000 constants --- */
// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                         SPI_CLOCK_DIVIDER); // you can change this clock speed
#define WLAN_SSID       WIFI_SSID          // Set in secrets.h
#define WLAN_PASS       WIFI_PASSWORD      // Set in secrets.h
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2
#define LISTEN_PORT           80
#define MAX_ACTION            10      // Maximum length of the HTTP action that can be parsed.

#define MAX_PATH              64      // Maximum length of the HTTP request path that can be parsed.
                                      // There isn't much memory available so keep this short!

#define BUFFER_SIZE           MAX_ACTION + MAX_PATH + 20  // Size of buffer for incoming request data.
                                                          // Since only the first line is parsed this
                                                          // needs to be as large as the maximum action
                                                          // and path plus a little for whitespace and
                                                          // HTTP version.

#define TIMEOUT_MS            500    // Amount of time in milliseconds to wait for
                                     // an incoming request to finish.  Don't set this
                                     // too high or your server could be slow to respond.
Adafruit_CC3000_Server httpServer(LISTEN_PORT); //(LISTEN_PORT);e
uint8_t buffer[BUFFER_SIZE+1];
int bufindex = 0;
char action[MAX_ACTION+1];
char path[MAX_PATH+1];

/* --- Webpage variables --- */
String req;     // Will handle the request of the client.
String reqsub;  // Request substring to for keyword "on" or "off".
char webpage[404] = "<!DOCTYPE HTML><html><body><center><h1>Wireless Controller via Wi-Fi</h1><p>by e-Gizmo</p><br><form method=\"get\"><input type=\"button\"style=\"height:100px;width:100px\" value=\"ON\"onclick=\"window.location='/?on'\">&nbsp;<input type=\"button\"style=\"height:100px;width:100px\"value=\"OFF\"onclick=\"window.location='/?off'\"></form></center></body></html>";


void(* SoftReset) (void) = 0;
long count = 0;
int on;
