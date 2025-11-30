boolean error = false;
boolean STOPALL = false; // stop self driving
boolean serverAtWork = false; // prevents overlaping of the webserver() function
boolean called = false; // interrupt counter calling
boolean published = false; // cr and cl results Serial print only once at arrival
boolean NoStopClient = false; // value that serves the purpose of holding off client.stop() command until refresh is done when shutdown has been requested
boolean docking = false;
boolean docked = false;
boolean ignoreobs = true;
boolean mvt = false;
boolean noturn = true; // tels speed adjustments this is not a turn so ok to adjust
boolean found = false; // boolean for IR dock search

boolean AllowOTA = false; // true disables webserver
boolean interruptDetached = false;
boolean logs = false;
boolean GotStuck = false;
boolean requireHeuristics = false; 
boolean allSet = false;
int IRconfirm = 0; // station iin view confirmation for better accuracy

boolean doReset; // reset requests boolean variable used by webserver

boolean FIR_obs;
boolean bump;
boolean FUSUP;
boolean Rcliff;
boolean obstacle;
boolean FrontIR;

boolean poweroffDONE = false;

int WetVal = 100;
int count = 100; // counter for webserver loop
boolean someIR = false; // IR signal search, this var allows to know that there is some signal, even though not the one expected yet

int positive = 0; // confirmation test for stuck()

unsigned long previousMillisBlink = 0;
unsigned long previousMillisConnect = 0;
unsigned long previousMillisPowerTest = 0;
unsigned long DELAYpowertest = 3600000;
boolean testAlreadyDone = false;
unsigned long timeCountToPwOff;
unsigned long Start;

int SLOW = 160;
int MEDIUM = 200;
int FAST = 255;
int reqSpeed = 0; // tels speed adjustments where to start from
int N = 0; // speed alteration margin


// stuck detection increments and threshold
int maxincr = 5;
int incr = 0; // must be higher than maxincr times in row for this functiont to return true
int thres = 100;
long ReadL = 0;
long ReadR = 0;
long secReadL = 0;
long secReadR = 0;
int DELAY = random(50, 500);
int SPEED = random(180, 200);

int DelayMill = 40000; // used to turn off the camera after docking

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

int defaultDist = 10; // in cm

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



int lastValR;
int valR;
int lastValL;
int valL;
boolean idR = lastValR == valR;
boolean idL = lastValL == valL;

int var = 5; // speed correction margin threshold

volatile int interruptCounter = 0;
int numberOfInterrupts = 0;


String test_thisMessage;

