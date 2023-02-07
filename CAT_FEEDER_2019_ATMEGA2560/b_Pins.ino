// "RESERVED" pins for W5100 Ethernet Shield - best to avoid
#define PIN_4_RESERVED            4  //reserved by W5100 Shield on both UNO and MEGA
#define PIN_1O_RESERVED          10  //reserved by W5100 Shield on both UNO and MEGA
#define PIN_11_RESERVED          11  //reserved by W5100 Shield on UNO
#define PIN_12_RESERVED          12  //reserved by W5100 Shield on UNO
#define PIN_13_RESERVED          13  //reserved by W5100 Shield on UNO
#define PIN_50_RESERVED          50  //reserved by W5100 Shield on MEGA
#define PIN_51_RESERVED          51  //reserved by W5100 Shield on MEGA
#define PIN_52_RESERVED          52  //reserved by W5100 Shield on MEGA
#define PIN_53_RESERVED          53  //reserved by W5100 Shield on MEGA

// servos : LEAVE ports 22 - 30 for servos only
Servo gate;                
Servo canopArm;            // arm that holds the can opener
Servo uh;              // holds cans from pushing the next one
Servo lp;      // lower pusher, the one that pushes the cans onto the tray and also a second holder    
Servo c2;  // pusher located on the exit track
Servo c1;  //    
Servo elev; // pin 37 elevator
Servo canReceiver;
Servo lidRemover;

//reset pin
#define rst                 12

// Analog sensors
#define LED                 14
#define canLoad             A0 // IR sensor on the flat tray where can is to be opened


// digital sensors

#define pressed             6
#define chamber1echo        23
#define chamber1trig        25



//HBridge
#define M1                  40 // provide 6V to motor
#define M2                  42

#define LA1                  43 // provide 6V to Linear Actuator on the track (not a fucking servo...)
#define LA2                  45

//relays
#define power               8
#define pwCoordination      9
