


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

#define CamAndArmsRelay       2      //12 // power relay that turns on Mebo's motherboard, cam and new motor controller
#define CHARGERelay          12 // charging power relay

#define LineLeftFront          39 // front left color and line detector
#define LineRightFront         36 // front right color and line detector
#define LineLeftRear           32 // rear left color and line detector
#define LineRightRear          33 // rear right color and line detector 

#define ENABLE_LEFT       23  //Left speed modulation 
#define MOTOR_LEFT1         22  //left motor
#define MOTOR_LEFT2         19

#define MOTOR_RIGHT1        21 // right motor 
#define MOTOR_RIGHT2        5
#define ENABLE_RIGHT      18 //Right speed modulation 

#define RIGHT             26 //// ENCODERS
#define LEFT              25


/// UART communication with ATMEGA2560
#define tx2               17
#define rx2               16


// analog pins
#define CliffSensor       A6 // IR distance sensor used for Cliff 34
#define PW                A4 // charging and battery current measurment 35 // A7 seems to be dead... or stuck or I don't know... spent 4 hours on that shit... 

#define RST               13 // 

// in order to allow for USB upload without pressing the damn button, add a 47uf capacitor between EN and ground
// and a 1kohm resistor between 5V (not 3.3, doesn't work for me...) and EN to allow reboot after flash
/// https://randomnerdtutorials.com/solved-failed-to-connect-to-esp32-timed-out-waiting-for-packet-header/

#define RECV_PIN          14 // IR pulse Receiver
#define LED               15




//GPIOs 34 to 39 are GPIOs – input only pins. These pins don’t have internal pull-ups or pull-down resistors.
//They can’t be used as outputs, so use these pins only as inputs:
//
//GPIO 34
//GPIO 35
//GPIO 36
//GPIO 37
//GPIO 38
//GPIO 39




/************INTERRUPT**********************/
struct leftEncoder {
  const uint8_t PIN;
  uint32_t cl;
  bool p; // p for "pressed"
};
struct rightEncoder {
  const uint8_t PIN;
  uint32_t cr;
   bool p; // p for "pressed"
};

//we create an instance of the Button structure and initialize pin number to LEFT, number of key presses to 0 and default pressed state to false.

leftEncoder EncL = {LEFT, 0, false};
rightEncoder EncR = {RIGHT, 0, false};

void IRAM_ATTR isrL() {
  //  EncR.cr += 1;
  //  int crtemp = EncR.cr;
  //  if (crtemp >= 100)
  //  {
  //    cr = EncR.cr / 100;
  //  }
  EncL.p = true;
}
void IRAM_ATTR isrR() {
  //  EncL.cl += 1;
  //  int cltemp = EncL.cl;
  //  if (cltemp >= 100)
  //  {
  //    cl = EncL.cl / 100;
  //  }
  EncR.p = true;
}
/************END OF INTERRUPT***************/
