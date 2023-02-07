
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

#define LED               15
#define ACTUATOR_A        22
#define ACTUATOR_B        21
#define ACTR_ENABLE_PWM   19

#define OPENER_A          22
#define OPENER_B          21
#define OPN_ENABLE_PWM    19

#define US1_trig          16
#define US1_echo          17

#define US2_trig          26
#define US2_echo          25

#define IRsensor1         39
