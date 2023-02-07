boolean stopped = false;
boolean StopAll = false; // for OTA
boolean closing = false;
boolean opening = false;
boolean modifRequested = false;
boolean wasclosed = true;
boolean NoTouchOpen = true;
boolean watchdogInitOk = false;
boolean consideredClosed = false;
boolean emergency = false;
boolean logs = false;
boolean emergencySpeed = false;


int DELAY_SEC = 600; //in seconds
unsigned long previousMillis = 0; //needed for new timing mechanism
unsigned long previousMillisReset = 0;
unsigned long ResetTime = 1000 * 60 * 60 * 6; // 6 hours
unsigned long previousMillisBlink = 0;
unsigned long LedOn = 0;
unsigned long previousMillisMotors = 0;
unsigned long previousMillisContactClosed = 0;
unsigned long previousMillisContactOpen = 0;
unsigned long previousMillisEvts = 0;
unsigned long previousMillisST = 0;
unsigned long previousMillisServer = 0;
unsigned long loopTime = 0;
unsigned long lastBoot = millis();
unsigned long lastTimeDisplay = 0;
unsigned long closingcmdMillis = 0;
unsigned long previousMillisOpen = millis();
unsigned long emergencySpeedMillis = 0;


String debugData = "";
String previousDebugData = "";
String previousDebugDataCmd = "";
String previousDebugDataCmdBits = "";
String previousDebugDataCmdLevel = "";
String timeOfLastBoot = "";
String lastMessage = "";
String timerResult = "";
String lastCmd = "";

bool isDebugEnabled;    // enable or disable debug in this example

int elapsed = 0;
int evtscount = 0;
int TOUCH = 0;
int itopen = 0;
int wtdpin;



/// DC MOTOR SETTINGS

int ULTRASLOW = 600; //PWM signals on ESP8266 have 10-bit resolution
int FAST = 1024;
int SpeedVal = ULTRASLOW; // default speed //  this is also a global val to be set upon request from ST
int lastSpeedVal = SpeedVal; // for when it needs to resume
int PUSH = 50;
int frequency = 60000; // 

// 1/0 values for motors
int A = 0;
int B = 1;
