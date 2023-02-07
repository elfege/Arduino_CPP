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

#define CHARGERelay         13 // charging power relay // !!! boot fails if 12 is pulled high
#define POWER               23 // power to motors and cameras
#define ENABLE_LEFT         19  //Left speed modulation 
#define MOTOR_LEFT1         18  //left motor
#define MOTOR_LEFT2         5

#define MOTOR_RIGHT1        17 // right motor 
#define MOTOR_RIGHT2        16
#define ENABLE_RIGHT        4 //Right speed modulation 

#define RIGHT               26 //// ENCODERS
#define LEFT                25


/// UART communication with ATMEGA2560
#define tx2                 17
#define rx2                 16


// analog pins
//#define PW                A4 // charging and battery current measurment 35 // A7 seems to be dead... or stuck or I don't know... spent 4 hours on that shit... 

//#define RST               13 // 

// in order to allow for USB upload without pressing the damn button, add a 47uf capacitor between EN and ground
// and a 1kohm resistor between 5V (not 3.3, doesn't work for me...) and EN to allow reboot after flash
/// https://randomnerdtutorials.com/solved-failed-to-connect-to-esp32-timed-out-waiting-for-packet-header/

#define RECV_PIN          14 // IR pulse Receiver
#define LED               15

#define watchDogPin       27


//GPIOs 34 to 39 are GPIOs – input only pins. These pins don’t have internal pull-ups or pull-down resistors.
//They can’t be used as outputs, so use these pins only as inputs:
//
//GPIO 34
//GPIO 35
//GPIO 36
//GPIO 37
//GPIO 38
//GPIO 39
