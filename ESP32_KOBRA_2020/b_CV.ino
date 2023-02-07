
boolean STOPALL = false; // stop self driving
boolean OTArequest = false;

boolean alreadyranback = false;
boolean flushing = false; // when buffer is being flushed, allow to stop all movements
boolean blinking = false;
boolean logs = false;
boolean mvt = false;
boolean doReset; // reset requests boolean variable used by webserver
unsigned long resetDelay = 2000; // default delay before a reset is triggered after request

boolean HttpReq = false;

boolean clientisconnected = false;
boolean docking = false;

String cmd; // cmds sent through uart to Atmega
String MvtState = "READY";

boolean poweroffDONE = false;


int WetVal = 100;
int count = 100; // counter

boolean moreToTheRight;
boolean moreToTheLeft; // compensators

int incomingByte;      // a variable to read incoming serial data into
int disconnectCounter = 0;

// counters for docks A.I. trees
int hasSearched = 0;
int hastriedunstuck = 0;
int hastried = 0;
int hastriedOBS = 0;
int method = 0; // binary tree A.I.

int positive = 0; // confirmation test for stuck()

/////////// TIMERS ////////////////

unsigned long lastClientConnection = millis();
unsigned long lastWifiConnection = millis();
unsigned long lastMillis = 0;
unsigned long previousMillisBlink = 0;
unsigned long tracksOnmillis = 0;
unsigned long motorsOnMillis = 0;
unsigned long previousCurrentMeasure = 0;
unsigned long Start;
unsigned long lastST_Stop = 0;
unsigned long RstCtDwnStart = 0;
unsigned long RecTime = 0; // stuck timer
unsigned long MvtStateMillis = 0; // for turn delays
unsigned long loopTime = 0;
unsigned long StartSearch = 0; // docking operations timer

int elapsed = 0;

int lastProgress = 0; // OTA

int pos = 0;

bool goUp = true;
int glw = 0; // glowing counter

int httpdist = 0;
int N = 0; // speed alteration margin



int SLOW = 150;
int MEDIUM = 170;
int FAST = 200;
int rqSp = 0; // initial motors speed 
// stuck detection increments and threshold
int maxincr = 5;
int incr = 0; // must be higher than maxincr times in row for this functiont to return true
int thres = 180;
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
const int pwmChannelL = 5; // must leave channel 0 for servo library
const int pwmChannelR = 6;
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



int lastSSID = 1;
int lastValR;
int valR;
int lastValL;
int valL;
boolean idR = lastValR == valR;
boolean idL = lastValL == valL;

int var = 5; // speed correction margin threshold


String currentLine = "";
