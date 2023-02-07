//******************************************************************************************
//Define which Arduino Pins will be used for each device
//******************************************************************************************
//"RESERVED" pins for ESP-01 - best to avoid
#define PIN_18_RESERVED          18  //reserved ESP-01 on MEGA
#define PIN_19_RESERVED          19  //reserved ESP-01 on MEGA

#define RESET_PIN         3 // pin 3 connected reset pin 

//digital Pins
#define RELAY1            12 // power relay that turns on Mebo's motherboard, cam and new motor controller
#define RELAY2            28 // charging power relay


#define trigPin           2   // GPIO2 ultrasonic trigger
#define echoPin           5   // GPIO5 ultrasonic echo
#define bumberPin         15  // Bumper pin


#define LineLeft          23 // left color and cliff detector
#define LineRight         4  // right color and cliff detector

#define PWM_L       30  //Left speed modulation 
#define PWM_LEFT1         22  //left motor
#define PWM_LEFT2         23

#define PWM_RIGHT1        24 // right motor 
#define PWM_RIGHT2        25
#define PWM_R     31 //Right speed modulation 

#define RIGHT             27 //// ENCODERS
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







