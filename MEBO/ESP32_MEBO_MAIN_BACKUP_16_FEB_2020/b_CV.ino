boolean stopAll = false;

boolean watchdogInitOk = false;
boolean noblink = false;
boolean STOPALL = false; // stop self driving
boolean OTArequest = false;
boolean published = false; // cr and cl results Serial print only once at arrival
boolean NoStopClient = false; // value that serves the purpose of holding off client.stop() command until refresh is done when shutdown has been requested

boolean rearleft = false;
boolean frontright = false;

boolean isSelfDriving = false;
boolean docking = false;
int sPeeD = 170; // default docking speed
int startingSpeed = sPeeD;

boolean docked = false;
boolean ignoreobs = true;
boolean encodersOk = true; // false is you don't want encoders based speed adjustments despite going forward or reverse (mvt = true) MUST ALWAYS BE RESET TO TRUE AFTER USE!!!
boolean makeEncHappen = false; // true if you want encoders despite turning
boolean mvt = false;
boolean noturn = true; // tels speed adjustments this is not a turn so ok to adjust
boolean found = false; // boolean for IR dock search
boolean Signal = false;
boolean gotsignal = false;
boolean gotstation = false;

boolean alreadyranback = false;
boolean flushing = false; // when buffer is being flushed, allow to stop all movements

boolean AllowOTA = false; // true disables webserver
boolean interruptDetached = false;
boolean logs = false;
boolean resultStuck = false; // global value susceptible to be changed back to false by stuck() if and only if noturn and mvt are true
boolean GotStuck = false; // this one can be set to true by stuck() but not to false, false set inside functions calling stuck()
boolean noStuck = false; // for when you don't want stuck to be triggered
boolean PreRecDone = false; // tells if a prerecord has been set already, default must be false




boolean check = true; // this one is called by any other function telling main loop to not run Stuck() test, mostly to prevent redundencies in SelfDrive()

boolean runIRreceiver = false;
boolean allSet = false; // allows to turn on the IR decoder port only once requested and not after that.
int IRconfirm = 0; // station iin view confirmation for better accuracy

boolean doReset; // reset requests boolean variable used by webserver
unsigned long resetDelay = 2000; // default delay before a reset is triggered after request

boolean HttpReq = false;
boolean HttpDistReq = false;

boolean FIR_obs = false;
boolean bumped = false;
boolean bumpedR = false;
boolean bumpedL = false;
boolean FUSUP = false;
boolean obstacle = false;
boolean FrontIR = false;

boolean rightDone = false;
boolean leftDone = true;

String cmd; // cmds sent through uart to Atmega
String MvtState = "READY";

boolean poweroffDONE = false;


int WetVal = 100;
int count = 100; // counter

boolean moreToTheRight;
boolean moreToTheLeft; // compensators

int incomingByte;      // a variable to read incoming serial data into

// counters for docks A.I. trees
int hasSearched = 0;
int hastriedunstuck = 0;
int hastried = 0;
int hastriedOBS = 0;
int method = 0; // binary tree A.I.

int positive = 0; // confirmation test for stuck()

 // encoders reading B4 and AFter
int B4L = 0;
int B4R = 0;
int AFL = 0;
int AFR = 0;


/////////// TIMERS ////////////////
unsigned long lastMillis = 0;
unsigned long previousMillisBlink = 0;
unsigned long previousCurrentMeasure = 0;
unsigned long timeCountToPwOff;
unsigned long Start;
unsigned long lastST_Stop = 0;
unsigned long RstCtDwnStart = 0;
unsigned long RecTime = 0; // stuck timer
unsigned long MvtStateMillis = 0; // for turn delays
unsigned long loopTime = 0;
unsigned long StartSearch = 0; // docking operations timer

int elapsed = 0;

bool goUp = true;
int glw = 0; // glowing counter

int httpdist = 0;
int N = 0; // speed alteration margin



int SLOW = 150;
int MEDIUM = 170;
int FAST = 200;
int rqSp = 0; // tels speed adjustments where to start from

// stuck detection increments and threshold
int maxincr = 5;
int incr = 0; // must be higher than maxincr times in row for this functiont to return true
int thres = 100;
long ReadL = 0;
long ReadR = 0;
long secReadL = 0;
long secReadR = 0;
int manageGotStuck = 0; // counter

int DELAY = random(50, 500);
int SPEED = 150; //random(180, 200);

int DelayMill = 40000; // used to turn off the camera after docking

// 1/0 values for motors

int A = 0;
int B = 1;


/// DC MOTOR SETTINGS


int Default = MEDIUM;
int dutyCycle = Default; // for all cases when there's no need to differentiate left and right wheels
int dcL = Default; // current value to be refreshed by speed corrections on left wheel
int dcR = Default; // current value to be refreshed by speed corrections on right wheel

// Setting PWM properties
const int freq = 30000;
const int pwmChannelL = 0;
const int pwmChannelR = 1;
const int pwmChannelGlow = 2; // GLOWING LED...
const int resolution = 8;


//speed modifications allowances
int ModifR5X = 0;
int ModifL5X = 0;

int defaultDist = 10; // in cm

//ultrasonic variables
long duration;
int distance;


//encoders
uint32_t cr = 0; // counter for left wheel
uint32_t cl = 0; // counter for right wheel
int limit;

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
String currentLine = "";
