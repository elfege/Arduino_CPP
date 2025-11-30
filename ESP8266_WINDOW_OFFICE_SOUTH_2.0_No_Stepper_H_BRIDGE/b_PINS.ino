//*************************************************************************************************
//NodeMCU v1.0 ESP8266-12e Pin Definitions (makes it much easier as these match the board markings)
//*************************************************************************************************
//#define LED_BUILTIN 16
//#define BUILTIN_LED 16
//
//#define D0 16  //no internal pullup resistor
//#define D1  5
//#define D2  4
//#define D3  0  //must not be pulled low during power on/reset, toggles value during boot
//#define D4  2  //must not be pulled low during power on/reset, toggles value during boot
//#define D5 14
//#define D6 12
//#define D7 13
//#define D8 15  //must not be pulled high during power on/reset
//*****************************************************************************
/// @file
/// @brief
///   Arduino SmartThings Ethernet ESP8266 WiFi Window Control August 2019
///
///
//*****************************************************************************
//  https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/

/* IDENTICAL TO THE OTHER WINDOW USING SAME THREADED ROD BASED ACTUATOR */

#define C D7
#define D D6
#define FAN D3

#define LED                 D4 // D4 is GPIO2 built-in led...pulled up by default, boot fails if pulled low .

// sensor input
#define contact_sensor_open           D1 // limit switch located at the end of the track (!isOpen() and isOpen())
#define contact_sensor_fullyOpen      D2 // limit switch located near the motor
#define rotor               D0 // IR limit sensor detects the rotation of the rotor

// motor control // must be VARIABLES, not constants
int pwm = D5; //speed modulation


/*OFFICE_SOUTH
   Label GPIO  Input Output  Notes
  D0  GPIO16  no interrupt  no PWM or I2C support HIGH at boot
  used to wake up from deep sleep
  D1  GPIO5 OK  OK  often used as SCL (I2C)
  D2  GPIO4 OK  OK  often used as SDA (I2C)
  D3  GPIO0 pulled up OK  connected to FLASH button, boot fails if pulled LOW
  D4  GPIO2 pulled up OK  HIGH at boot connected to on-board LED, boot fails if pulled LOW
  D5  GPIO14  OK  OK  SPI (SCLK)
  D6  GPIO12  OK  OK  SPI (MISO)
  D7  GPIO13  OK  OK  SPI (MOSI)
  D8  GPIO15  pulled to GND OK  SPI (CS)  Boot fails if pulled HIGH
  RX  GPIO3 OK  RX pin  HIGH at boot
  TX  GPIO1 TX pin  OK  HIGH at boot debug output at boot, boot fails if pulled LOW
  A0  ADC0  Analog Input  X
*/
