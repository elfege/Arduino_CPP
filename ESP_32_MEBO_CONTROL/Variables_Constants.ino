boolean error = false;
boolean STOPALL = false; // stop self driving
boolean serverAtWork = false; // prevents overlaping of the webserver() function
boolean called = false; // interrupt counter calling

int WetVal = 100;
int count = 100; // counter for webserver loop

unsigned long previousMillisBlink = 0;
unsigned long previousMillisConnect = 0;
unsigned long START;

int SLOW = 165;
int MEDIUM = 220;
int FAST = 255;


// 1/0 values for motors

int A = 0;
int B = 1;


/// DC MOTOR SETTINGS


int Default = SLOW;
int dutyCycle = Default; // for all cases when there's no need to differentiate left and right wheels
int dutyCycleL = Default; // current value to be refreshed by speed corrections on left wheel
int dutyCycleR = Default; // current value to be refreshed by speed corrections on right wheel

// Setting PWM properties
const int freq = 30000;
const int pwmChannelL = 0;
const int pwmChannelR = 1;
const int resolution = 8;


//speed modifications allowances
int ModifR5X = 0;
int ModifL5X = 0;

int defaultDist = 128;

//ultrasonic variables
long duration;
int distance;


//encoders
unsigned long cr = 0; // counter for left wheel
unsigned long cl = 0; // counter for right wheel


int VRB4;// R wheel returned value before change
int VLB4;// L wheel returned value before change

int VR; //refreshed value of right wheel
int VL; //refreshed value of left wheel

boolean sameR = VR == VRB4;
boolean sameL = VL == VLB4;



int lastValR;
int valR;
int lastValL;
int valL;
boolean idR = lastValR == valR;
boolean idL = lastValL == valL;

int var = 5; // speed correction margin threshold

volatile int interruptCounter = 0;
int numberOfInterrupts = 0;


