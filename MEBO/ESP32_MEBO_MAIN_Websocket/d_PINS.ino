

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

#define MEBORelay            2 //12 // power relay that turns on Mebo's motherboard, cam and new motor controller
#define CHARGERelay          12 // charging power relay

#define LineLeft          39 // left color and line detector
#define LineRight         36 // right color and line detector

#define ENABLE_LEFT       23  //Left speed modulation 
#define MOTOR_LEFT1         22  //left motor
#define MOTOR_LEFT2         19

#define MOTOR_RIGHT1        21 // right motor 
#define MOTOR_RIGHT2        5
#define ENABLE_RIGHT      18 //Right speed modulation 

#define RIGHT             25 //// ENCODERS
#define LEFT              26


/// UART communication with ATMEGA2560
#define tx2               17
#define rx2               16


// analog pins
#define CliffSensor       A6 // IR distance sensor used for Cliff 34
#define PW                A7 // charging and battery current measurment 35

#define RST               4

int RECV_PIN = 14; // IR pulse Receiver

//GPIOs 34 to 39 are GPIOs – input only pins. These pins don’t have internal pull-ups or pull-down resistors. 
//They can’t be used as outputs, so use these pins only as inputs:
//
//GPIO 34
//GPIO 35
//GPIO 36
//GPIO 37
//GPIO 38
//GPIO 39
