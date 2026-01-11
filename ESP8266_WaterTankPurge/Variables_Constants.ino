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

unsigned long maxTimeout = millis();

unsigned long previousMillisPURGE = 0;
unsigned long previousMillisRun = 0;

unsigned long MAX_RUN_AFTER_USER_REQUEST = 60000;


int regularPurgeDuration = 70 * 1000; // run for n seconds every n hours

int desiredTime = 3; // 0 cancels out this feature // run purge every n hours for n(regularPurgeDuration) seconds
int seconds = 1000;

unsigned long nonStopPurgeLimit = 1 * 60 * 60 * 1000; // in hours

unsigned long nonStopPurgeMillis = millis();
unsigned long triggerMillis = 0;
unsigned long Hour = 3600 * seconds;
unsigned long schedule = desiredTime * Hour; 

unsigned long lastNTP = millis();
unsigned long millisBoot = millis();
unsigned long lastLogMillis = millis();

int wetv = 0;
int lastHwetv = 0;

int elapsed = 0 ;// variable redefined below with time()

bool scheduledRunning = false;
boolean goUp;
bool isDebugEnabled;    // enable or disable debug in this example

boolean StopAll = false;
boolean wetTriggered = false;
boolean nonStopPurging = false;
boolean ntpFailed = false;
boolean stopNtp = false;

boolean clocked = false;

int glw;

int purging;

// to differentiate blinking modes while stby and running


boolean stopped = false;
boolean closing = false;
boolean opening = false;
boolean modifRequested = false;
boolean stopAll = false;
boolean wasclosed = true;
boolean NoTouchOpen = true;
boolean watchdogInitOk = false;
boolean consideredClosed = false;
boolean emergency = false;
boolean logs = false;
boolean emergencySpeed = false;
boolean pumping = false;
boolean onRequestedByUser = false;

String timeOfLastBoot = "";
String lastMessage = "";
String timerResult = "";
String lastCmd = "";
String wetState = "dry";
String lastBootDate = "";


int evtscount = 0;
int TOUCH = 0;
int itopen = 0;
int wtdpin;

/// DC MOTOR SETTINGS

int ULTRASLOW = 470; 
int FAST = 1024;
int SpeedVal = ULTRASLOW; // default speed //  this is also a global val to be set upon request from ST
int lastSpeedVal = SpeedVal; // for when it needs to resume
int PUSH = 100;
int Default = FAST;
int dutyCycle = 0; // value at startup

// 1/0 values for motors
int A = 0;
int B = 1;

// Setting PWM properties
int freq = 60000;
int pwmChannel = 0;
int resolution = 10; 

int ntpFailures = 0;
