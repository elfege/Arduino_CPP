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

//GPIO 5 (must be HIGH during boot)
//GPIO 12 (must be LOW during boot)
//GPIO 15 (must be HIGH during boot)

#define CHARGER             13 // charging power relay sends signal to ESP8266 // !!! boot fails if 12 is pulled high
#define POWERCamAndMotors   23 // power to motors and cameras

#define ENABLE_LEFT         19  //Left speed modulation 
#define MOTOR_LEFT1         18  //left motor
#define MOTOR_LEFT2         5

#define MOTOR_RIGHT1        17 // right motor 
#define MOTOR_RIGHT2        16
#define ENABLE_RIGHT        4 //Right speed modulation 

#define TRACK_MOTOR_A       3
#define TRACK_MOTOR_B       21


// analog pins
#define PW                  A0 // charging and battery current measurment GPIO36 

Servo tower;
#define towerPin            14
Servo arm;
#define armPin              9
Servo wrist;
#define wristPin            25
Servo forearm;
#define forearmPin          26
Servo clamp;
#define clampPin            17

// in order to allow for USB upload without pressing the damn button, add a 47uf capacitor between EN and ground
// and a 1kohm resistor between 5V (not 3.3, doesn't work for me...) and EN to allow reboot after flash
/// https://randomnerdtutorials.com/solved-failed-to-connect-to-esp32-timed-out-waiting-for-packet-header/

#define LED                 22 //


/*
https://randomnerdtutorials.com/esp32-pinout-reference-gpios/

GPIOs 34 to 39 are GPIOs – input only pins. These pins don’t have internal pull-ups or pull-down resistors.
They can’t be used as outputs, so use these pins only as inputs:

GPIO 34
GPIO 35
GPIO 36
GPIO 37
GPIO 38
GPIO 39

Strapping Pins
The ESP32 chip has the following strapping pins:

GPIO 0
GPIO 2
GPIO 4
GPIO 5 (must be HIGH during boot)
GPIO 12 (must be LOW during boot)
GPIO 15 (must be HIGH during boot)
These are used to put the ESP32 into bootloader or flashing mode. On most development boards with built-in USB/Serial,
you don’t need to worry about the state of these pins. The board puts the pins in the right state for flashing or boot 
mode. More information on the ESP32 Boot Mode Selection can be found here.

However, if you have peripherals connected to those pins, you may have trouble trying to upload new code, 
flashing the ESP32 with new firmware or resetting the board. If you have some peripherals connected to the 
strapping pins and you are getting trouble uploading code or flashing the ESP32, it may be because 
those peripherals are preventing the ESP32 to enter the right mode. Read the Boot Mode Selection 
documentation to guide you in the right direction. After resetting, flashing, or booting, those pins work as expected.
*/
