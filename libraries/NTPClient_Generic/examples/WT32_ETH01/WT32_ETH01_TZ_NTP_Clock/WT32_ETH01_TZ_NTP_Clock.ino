/****************************************************************************************************************************
  WT32_ETH01_TZ_NTP_Clock.ino
  
  For AVR, ESP8266/ESP32, SAMD21/SAMD51, nRF52, STM32, SAM DUE, WT32_ETH01,  boards using 
  a) Ethernet W5x00, ENC28J60, LAN8742A
  b) WiFiNINA
  c) ESP8266/ESP32 WiFi
  d) ESP8266/ESP32-AT-command WiFi
  e) WT32_ETH01 (ESP32 + LAN8720)
  f) RTL8720DN

  Based on and modified from Arduino NTPClient Library (https://github.com/arduino-libraries/NTPClient)
  to support other boards such as ESP8266/ESP32, SAMD21, SAMD51, Adafruit's nRF52 boards, SAM DUE, RTL8720DN, etc.
  and Ethernet/WiFi/WiFiNINA shields
  
  Copyright (C) 2015 by Fabrice Weinberg and licensed under MIT License (MIT)

  Built by Khoi Hoang https://github.com/khoih-prog/NTPClient_Generic
  Licensed under MIT license
 *****************************************************************************************************************************/

#include "defines.h"

//////////////////////////////////////////

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include <Timezone_Generic.h>             // https://github.com/khoih-prog/Timezone_Generic

// US Eastern Time Zone (New York, Detroit)
TimeChangeRule myDST = {"EDT", Second, Sun, Mar, 2, -240};    // Daylight time = UTC - 4 hours
TimeChangeRule mySTD = {"EST", First, Sun, Nov, 2, -300};     // Standard time = UTC - 5 hours
Timezone *myTZ;

// If TimeChangeRules are already stored in EEPROM, comment out the three
// lines above and uncomment the line below.
//Timezone myTZ(100);       // assumes rules stored at EEPROM address 100

TimeChangeRule *tcr;        // pointer to the time change rule, use to get TZ abbrev

//////////////////////////////////////////

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include <NTPClient_Generic.h>          // https://github.com/khoih-prog/NTPClient_Generic

int status = WL_IDLE_STATUS;      // the Wifi radio's status

// A UDP instance to let us send and receive packets over UDP
WiFiUDP ntpUDP;

// NTP server
//World
//char timeServer[] = "time.nist.gov";
// Canada
char timeServer[] = "0.ca.pool.ntp.org";
//char timeServer[] = "1.ca.pool.ntp.org";
//char timeServer[] = "2.ca.pool.ntp.org";
//char timeServer[] = "3.ca.pool.ntp.org";
// Europe
//char timeServer[] = ""europe.pool.ntp.org";

#define TIME_ZONE_OFFSET_HRS            (-4)
#define NTP_UPDATE_INTERVAL_MS          60000L

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, timeServer, (3600 * TIME_ZONE_OFFSET_HRS), NTP_UPDATE_INTERVAL_MS);

static bool gotCurrentTime = false;

// format and print a time_t value, with a time zone appended.
void printDateTime(time_t t, const char *tz)
{
  char buf[32];
  char m[4];    // temporary storage for month string (DateStrings.cpp uses shared buffer)
  strcpy(m, monthShortStr(month(t)));
  sprintf(buf, "%.2d:%.2d:%.2d %s %.2d %s %d %s",
          hour(t), minute(t), second(t), dayShortStr(weekday(t)), day(t), m, year(t), tz);
  Serial.println(buf);
}

void update_Time(void)
{
  // Just get the correct time once
  if (timeClient.updated())
  {
    Serial.println("********UPDATED********");

    // set the system time to UTC
    // warning: assumes that compileTime() returns US EDT
    // adjust the following line accordingly if you're in another time zone
    setTime(timeClient.getUTCEpochTime());
    
    gotCurrentTime = true;
  }
}

void displayClock(void)
{
  time_t utc = now();

  Serial.println("============================");

  time_t local = myTZ->toLocal(utc, &tcr);

  printDateTime(utc, "UTC");
  printDateTime(local, tcr -> abbrev);
}

void check_status(void)
{
  // Update first time after 5s
  static ulong checkstatus_timeout  = 5000L;
  static ulong TimeDisplay_timeout   = 0;

  static ulong current_millis;

// Display every 10s
#define TIME_DISPLAY_INTERVAL       (10000L)

// Update RTC every hour if got correct time from NTP
#define TIME_UPDATE_INTERVAL         (3600 * 1000L)

// Retry updating RTC every 5s if haven't got correct time from NTP
#define TIME_RETRY_INTERVAL          (3 * 1000L)

  current_millis = millis();

  if ((current_millis > TimeDisplay_timeout) || (TimeDisplay_timeout == 0))
  {
    if (gotCurrentTime)
      displayClock();
      
    TimeDisplay_timeout = current_millis + TIME_DISPLAY_INTERVAL;
  }

  // Update Time every TIME_UPDATE_INTERVAL (3600) seconds.
  if ((current_millis > checkstatus_timeout))
  {
    update_Time();
    
    if (gotCurrentTime)
    {
      Serial.println("Time updated. Next update in seconds : " + String(TIME_UPDATE_INTERVAL/1000, DEC));
      checkstatus_timeout = current_millis + TIME_UPDATE_INTERVAL;
    }
    else
    {
      Serial.println("Retry Time update in seconds : " + String(TIME_RETRY_INTERVAL/1000, DEC));
      checkstatus_timeout = current_millis + TIME_RETRY_INTERVAL;
    }   
  }
}

//////////////////////////////////////////

void setup()
{
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  Serial.print("\nStarting WT32_ETH01_TZ_NTP_Clock on "); Serial.print(ARDUINO_BOARD);
  Serial.print(" with "); Serial.println(SHIELD_TYPE);
  Serial.println(WEBSERVER_WT32_ETH01_VERSION);
  Serial.println(NTPCLIENT_GENERIC_VERSION);

  // To be called before ETH.begin()
  WT32_ETH01_onEvent();

  //bool begin(uint8_t phy_addr=ETH_PHY_ADDR, int power=ETH_PHY_POWER, int mdc=ETH_PHY_MDC, int mdio=ETH_PHY_MDIO, 
  //           eth_phy_type_t type=ETH_PHY_TYPE, eth_clock_mode_t clk_mode=ETH_CLK_MODE);
  //ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_TYPE, ETH_CLK_MODE);
  ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER);

  // Static IP, leave without this line to get IP via DHCP
  //bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = 0, IPAddress dns2 = 0);
  ETH.config(myIP, myGW, mySN, myDNS);

  WT32_ETH01_waitForConnect();;
  
  Serial.print(F("WT32_ETH01_TZ_NTP_Clock started @ IP address: "));
  Serial.println(ETH.localIP());

  myTZ = new Timezone(myDST, mySTD);

  timeClient.begin();
}

void loop()
{
  timeClient.update();
  check_status();
}
