.//******************************************************************************************
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

//digital Pins
#define RELAY1            12 // power relay that turns on Mebo's motherboard, cam and new motor controller
#define RELAY2            27 // charging power relay


#define trigPin           2   // GPIO2 ultrasonic trigger
#define echoPin           5   // GPIO5 ultrasonic echo
#define bumberPin         15  // Bumper pin


#define LineLeft          23 // left color and cliff detector
#define LineRight         4  // right color and cliff detector

#define ENABLE_LEFT       13  //Left speed modulation 
#define PWM_LEFT1         22  //left motor
#define PWM_LEFT2         19

#define PWM_RIGHT1        21 // right motor 
#define PWM_RIGHT2        17
#define ENABLE_RIGHT      18 //Right speed modulation 

#define RIGHT             25 //// ENCODERS
#define LEFT              26

#define RECV_PIN          14 // IR pulse Receiver 

// analog pins

#define PW                32 //  power charger input voltage
#define FIR               39  // front IR sensor
#define RLIR              34  // IR distance sensor rear left side
#define RRIR              35  // IR distance sensor rear right side


#define RIR               33 // Right IR sensor  (GPIO 33)
#define LIR               36 // Left IR sensor  (GPIO 36 aka SENSOR_VP port)


//GPIOs 34 to 39 are GPIs – input only pins. These pins don’t have internal pull-ups or pull-down resistors. 
//They can’t be used as outputs, so use these pins only as inputs:
//
//GPIO 34
//GPIO 35
//GPIO 36
//GPIO 37
//GPIO 38
//GPIO 39







