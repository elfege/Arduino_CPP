/*
   https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
*/

#define LED                 D4 // D4 is GPIO2 built-in led...pulled up by default, boot fails if pulled low .

// sensor input
#define contact_sensor_open           D2
#define contact_sensor_fullyOpen      D8 // D8 pulled down by default

// motor control // must be VARIABLES, not constants
//int pwm = D7; //speed modulation
int Step = D3; //Step of stepper motor driver
int Dir  = D5; // Direction of stepper motor driver


/*BEDROOM
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
