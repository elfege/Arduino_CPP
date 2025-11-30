/* IDENTICAL TO THE OTHER WINDOW USING SAME THREADED ROD BASED ACTUATOR */
boolean stopped = false;
boolean STOP = false;
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
boolean userSpeedInputRequested = false;
boolean finalPush = false;
boolean speedAdjusted = false;
boolean openingUpdateDone = false;
boolean closingUpdateDone = false;
boolean lastClosingReportWindowWasStillOpen = false;
boolean lastOpeningReportWindowWasStillClosed = false;
boolean ntpFailed = false;
boolean stopNtp = false;
boolean bootRestore = false;
boolean speedOverride = false;

unsigned long NTP_UPDATE_DELAY = 240000;
unsigned long DELAY_REFRESH = 1800 * 1000; // in minutes
unsigned long operationStartStamp = 0;     // operation time measurement
unsigned long stepcount = 0;
unsigned long maxSteps = 11450;
unsigned long previousMillis = 0; // needed for new timing mechanism
unsigned long previousMillisReset = 0;
unsigned long ResetTime = 1000 * 60 * 60 * 6; // 6 hours
unsigned long previousMillisBlink = 0;
unsigned long LedOn = 0;
unsigned long previousMillisMotors = 0;
unsigned long lastSpeedAdjustment = millis(); // this will not be refreshed until a speed adjustment is required
unsigned long previousMillisContactClosed = 0;
unsigned long previousMillisContactOpen = 0;
unsigned long previousMillisEvts = 0;
unsigned long previousMillisStop = 0;
unsigned long previousMillisST = 0;
unsigned long previousMillisServer = 0;
unsigned long finalPushMillis = millis();
unsigned long loopTime = 0;
unsigned long lastBoot = millis();
unsigned long lastTimeDisplay = 0;
unsigned long closingcmdMillis = 0;
unsigned long previousMillisOpen = millis();
unsigned long emergencySpeedMillis = 0;
unsigned long lastSpeedRequestMillis = 0;
unsigned long RequestDelay = 5 * 60 * 1000;
unsigned long lastReadMillis = millis();
unsigned long timeLastClosingUpdate = millis();
unsigned long timeLastOpeningUpdate = millis();
unsigned long lastNTP = millis();
unsigned long millisBoot = millis();
unsigned long lastLogMillis = millis();
unsigned long last_refresh_millis = 60001 + Millis();

String debugData = "";
String previousDebugData = "";
String previousDebugDataCmd = "";
previousDebugDataCmd_ss = "";
String previousDebugDataCmdBits = "";
String previousDebugDataCmdLevel = "";
String timeOfLastBoot = "";
String lastMessage = "";
String timerResult = "";
String lastCmd = "";
String dir = "A";
String windowLastState = "";
String lastBootDate = "";

bool isDebugEnabled; // enable or disable debug in this example

int lastMaxOpDuration = 240000; // default duration before speed needs to be reajusted.
int shortDelayAdjust = 30000;   // shorter delay after first adjustment
int speedIncrement = 10;
int speedAdjustmentOccurences = 0; // used to substact last time spent adjusting from counted operation duration
int elapsed = 0;
int evtscount = 0;
int TOUCH = 0;
int itopen = 0;
int wtdpin;
int readM = digitalRead(D0);
int events = 0;
int ntpFailures = 0;

/// DC MOTOR SETTINGS

// int FINALSPEED = 410 + 20; // will be updated by learning process // closing speed is an RTC variable (see lib page)
int LEARNING_SPEED_LIMIT = 700;
int ULTRASLOW_RESTORE = 500; // DEPRECATED March 2022 //value to be restored after the program had to increase default speed (deprecated, using FINALSPEED instead)
int SLOW = 700;
int FAST = 1023;
// int speedVal = ULTRASLOW; // default speed //  this is also a global val to be set upon request from ST
int lastSpeedVal = speedVal; // for when it needs to resume
int PUSH = 200;
int Default = FAST;
int frequency = 30000; //
