/*
   https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
*/
// sensor input
#define touch_sensor        D7
//#define armMov            D7 // interrupt // NOT IMPLEMENTED YET
#define contact_sensor      D6

// motor control
#define MA           D1 // motor
#define MB           D2 // motor
#define pwm          D5 // speed modulation
#define LED          D3 // D4 is GPIO2 built-in led...pulled up by default, boot fails if pulled low.

#define FAN          D8

/*
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
  D8  GPIO15  pulled to GND OK  SPI (CS)
  Boot fails if pulled HIGH
  RX  GPIO3 OK  RX pin  HIGH at boot
  TX  GPIO1 TX pin  OK  HIGH at boot debug output at boot, boot fails if pulled LOW
  A0  ADC0  Analog Input  X
*/
