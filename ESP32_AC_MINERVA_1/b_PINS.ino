//******************************************************************************************
//Define which Arduino Pins will be used for each device
//******************************************************************************************
//"RESERVED" pins for ESP32 - best to avoid
#define PIN_0_RESERVED             0  //reserved ESP32 boot/program upload
#define PIN_1_RESERVED             1  //reserved ESP32 for TX0
#define PIN_3_RESERVED             3  //reserved ESP32 for RX0
#define PIN_6_RESERVED             6  //reserved ESP32 for flash
#define PIN_7_RESERVED             7  //reserved ESP32 for flash
#define PIN_8_RESERVED             8  //reserved ESP32 for flash
#define PIN_9_RESERVED             9  //reserved ESP32 for flash
#define PIN_10_RESERVED           10  //reserved ESP32 for flash
#define PIN_11_RESERVED           11  //reserved ESP32 for flash

#define LED                       2

/* TOUTCH BUTTONS */
#define modeSw                    18
#define lowerTemp                 19
#define power                     13
#define raiseTemp                 26
#define fanSpeed                  25

/* FEEDBACKS */
#define cooling                   A0  //(GPIO 36)  //22 //34
#define dehumid                   A6  //(GPIO 34) //35
#define fanonly                   A16 //(GPIO 14)16 //36
#define fanlow                    A17 //(GPIO 27) //39                 
#define fanmed                    A4  //(GPIO 32)
#define fanhigh                   A5  //(GPIO 33) 
#define fanauto                   A7  //(GPIO 35) 17
#define heating                   A3  //(GPIO 39)//21

//****************************************************************************************************************************
//NodeMCU-32s ESP32 Pin Definitions (just for reference from ..hardware\espressif\esp32\variants\nodemcu-32s\pins_arduino.h)
//****************************************************************************************************************************
//#define LED_BUILTIN 2
//#define BUILTIN_LED 2
//
//#define A0 = 36;
//#define A3 = 39;
//#define A4 = 32;
//#define A5 = 33;
//#define A6 = 34;
//#define A7 = 35;
//#define A10 = 4;
//#define A11 = 0;
//#define A12 = 2;
//#define A13 = 15;
//#define A14 = 13;
//#define A15 = 12;
//#define A16 = 14;
//#define A17 = 27;
//#define A18 = 25;
//#define A19 = 26;

/*

  GPIO  Input   Output    Notes
  0     pulled  upOK      outputs PWM signal at boot
  1     TX pin  OK        debug output at boot
  2     OK      OK        connected to on-board LED
  3     OK      RX        pin  HIGH at boot
  4     OK      OK
  5     OK      OK        outputs PWM signal at boot
  6     x       x         connected to the integrated SPI flash
  7     x       x         connected to the integrated SPI flash
  8     x       x         connected to the integrated SPI flash
  9     x       x         connected to the integrated SPI flash
  10    x       x         connected to the integrated SPI flash
  11    x       x         connected to the integrated SPI flash
  12    OK      OK        boot fail if pulled high
  13    OK      OK
  14    OK      OK        outputs PWM signal at boot
  15    OK      OK        outputs PWM signal at boot
  16    OK      OK
  17    OK      OK
  18    OK      OK
  19    OK      OK
  21    OK      OK
  22    OK      OK
  23    OK      OK
  25    OK      OK
  26    OK      OK
  27    OK      OK
  32    OK      OK
  33    OK      OK
  34    OK    input only
  35    OK    input only
  36    OK    input only
  39    OK    input only
*/
