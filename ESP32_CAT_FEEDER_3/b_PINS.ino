
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



#define LED               5
#define ACTUATOR_A        22
#define ACTUATOR_B        21
#define ACTR_ENABLE_PWM   19

#define OPENER_A          12
#define OPENER_B          13
#define OPEN_ENABLE_PWM   14

// #define SMALL_PUSHER_A    33 // small pusher that fully extracts the can after master arm
// #define SMALL_PUSHER_B     4 //

#define servo_pwm         33
#define SMALL_PUSHER_PIN  4

#define vertPush_A        27
#define vertPush_B        16

#define COSINUS_A         18
#define COSINUS_B         2
#define COSINUS_PWM       23

#define deformSensor      A7  // GPIO35  | impedance sensor: detects chassis deformation due to can's resistance to arm's push.
#define wheel             32 //  GPIO32 | ADDED 2.2Kohm pull up resistor / wheel sensor: detects the can is spinning during opening operation

#define IR1_pile          A6 // GPIO34 |
#define IR2_pile          A3 // GPIO39 |

#define candown           36 // ADDED 2.2Kohm pull up resistor // limit switch that detects the can's presence AND positive LIFT
#define canSensor         15 // limit switch sensor that detects that the can has been pushed all the way under the opener
#define eatingZoneSensor  17 // limit switch that detects the can is ready to be compressed and/or has been deformed
#define lockSensor        25 
#define pusherRetract     26 // limit switch that detects if the master pusher is fully retracted. 

 

/* https://randomnerdtutorials.com/esp32-pinout-reference-gpios/

https://www.instructables.com/ESP32-Internal-Details-and-Pinout/
RECAP
GPIO	Input	Output	Notes
0	pulled up	OK	outputs PWM signal at boot
1	TX pin	OK	debug output at boot
2	OK	OK	connected to on-board LED
3	OK	RX pin	HIGH at boot
4	OK	OK	
5	OK	OK	outputs PWM signal at boot
6	x	x	connected to the integrated SPI flash
7	x	x	connected to the integrated SPI flash
8	x	x	connected to the integrated SPI flash
9	x	x	connected to the integrated SPI flash
10	x	x	connected to the integrated SPI flash
11	x	x	connected to the integrated SPI flash
12	OK	OK	boot fail if pulled high
13	OK	OK	
14	OK	OK	outputs PWM signal at boot
15	OK	OK	outputs PWM signal at boot
16	OK	OK	
17	OK	OK	
18	OK	OK	
19	OK	OK	
21	OK	OK	
22	OK	OK	
23	OK	OK	
25	OK	OK	
26	OK	OK	
27	OK	OK	
32	OK	OK	
33	OK	OK	
34	OK		input only
35	OK		input only
36	OK		input only
39	OK		input only

*/
