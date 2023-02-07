
boolean STOPALL = false; // stop self driving
boolean breakIt = false; // stop any inside loop without killing a whole process
boolean OTArequest = false;
boolean published = false; // cr and cl results Serial print only once at arrival
boolean NoStopClient = false; // value that serves the purpose of holding off client.stop() command until refresh is done when shutdown has been requested
boolean docking = false;
boolean docked = false;
boolean ignoreobs = true;
boolean mvt = false;
boolean noturn = true; // tels speed adjustments this is not a turn so ok to adjust
boolean found = false; // boolean for IR dock search
boolean Signal = false;

boolean alreadyranback = false;
boolean flushing = false; // when buffer is being flushed, allow to stop all movements

boolean AllowOTA = false; // true disables webserver
boolean interruptDetached = false;
boolean logs = false;
boolean resultStuck = false; // global value susceptible to be changed back to false by stuck() if and only if noturn and mvt are true
boolean GotStuck = false; // this one can be set to true by stuck() but not to false, false set inside functions calling stuck()
boolean check = false; // this one is called by any other function telling main loop to not run Stuck() test, mostly to prevent redundencies in SelfDrive()

boolean euristics = false;
boolean tests = false; // when logs are on do not run euristics' loops after triggering IR interrupt to allow IR loops to not both run in loop and in logs
boolean allSet = false;
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


String cmd; // cmds sent through uart to Atmega
String MvtState = "READY"; 

boolean poweroffDONE = false;

boolean flwln = false; // to allow FollowLineFWD() to cancle cliff sensing

boolean CliffDetect = false; // global boolean for cliff due to core 0 not being able to request a value from analog port
boolean disableCliff = false;
boolean cliffOFF = false; // permanent off no automatic reactivation

int WetVal = 100;
int count = 100; // counter for webserver loop

int incomingByte;      // a variable to read incoming serial data into

// counters for docks A.I. trees
int hasSearched = 0;
int hastriedunstuck = 0;
int hastried = 0;
int hastriedOBS = 0;
int methods = 0; // binary tree A.I.

int positive = 0; // confirmation test for stuck()

unsigned long lastMillis = 0;
unsigned long previousMillisBlink = 0;
unsigned long previousMillisConnect = 0;
unsigned long previousMillisPowerTest = 0;
unsigned long DELAYpowertest = 3600000;
boolean testAlreadyDone = false;
unsigned long timeCountToPwOff;
unsigned long Start;
unsigned long RstCtDwnStart = 0;


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
int DELAY = random(50, 500);
int SPEED = 180; //random(180, 200);

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
String currentLine = "";
