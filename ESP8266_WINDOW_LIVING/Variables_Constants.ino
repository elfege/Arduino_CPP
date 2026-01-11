/* NOT IDENTICAL ACCROSS DEVICES! (all tabs! for now...)*/

const char *NameOTA = "WINDOW_LIVING";
boolean armMovImplemented = true; // DIFFERENT FOR EACH DEVICE DEPENDING ON STATE OF DEVELOPMENT // UPDATE WHEN REQUIRED

boolean contactClosed;
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
boolean updateRTCdone = false;
boolean interruptChanged = false;
boolean EMstop = false;
boolean reloadPage = true;
boolean inFailSafeMode = false; // not the RTC var so it can be reset to 0 and this one takes over during the fail safe mode
boolean speedOverride = false;

unsigned long NTP_UPDATE_DELAY = 240000;
unsigned long DELAY_REFRESH = 1800 * 1000; // in minutes
unsigned long PUSH_DELAY = 30000;
unsigned long previousMillis = 0; // needed for new timing mechanism
unsigned long previousMillisReset = 0;
unsigned long ResetTime = 1000 * 60 * 60 * 6; // 6 hours
unsigned long previousMillisBlink = 0;
unsigned long LedOn = 0;
unsigned long previousMillisMotors = 0;
unsigned long previousMillisContactClosed = 0;
unsigned long previousMillisContactOpen = 0;
unsigned long previousMillisEvts = 0;
unsigned long previousMillisStop = 0;
unsigned long previousMillisST = 0;
unsigned long previousMillisServer = 0;
unsigned long finalPushMillis = millis();
unsigned long lastSpeedAdjustment = millis(); // this will not be refreshed until a speed adjustment is required
unsigned long loopTime = 0;
unsigned long bootStart = millis();
unsigned long lastTimeDisplay = 0;
unsigned long closingcmdMillis = 0;
unsigned long previousMillisOpen = millis();
unsigned long lastSpeedRequestMillis = 0;
unsigned long RequestDelay = 5 * 60 * 1000;
unsigned long timeLastClosingUpdate = millis();
unsigned long timeLastOpeningUpdate = millis();
unsigned long lastNTP = millis();
unsigned long millisBoot = millis();
unsigned long lastLogMillis = millis();
unsigned long METHOD_2_LONG_DELAY = 6 * 60 * 1000;
unsigned long lastHubRefresh;
unsigned long operationTime = 240000;

String debugData = "";
String previousDebugData = "";
String previousDebugDataCmd = "";
String previousDebugDataCmdBits = "";²
String previousDebugDataCmdLevel = "";
String timeOfbootStart = "";
String lastMessage = "";
String timerResult = "";
String lastCmd = "";
String windowLastState = "";
String bootStartDate = "";
String lastOpenCloseOp = "";

bool isDebugEnabled; // enable or disable debug in this example

int lastMaxOpDuration = 240000; // default duration before speed needs to be reajusted.
int shortDelayAdjust = 60000;   // shorter delay after first adjustment
int speedIncrement = 1;
int speedAdjustmentOccurences = 0; // used to substact last time spent adjusting from counted operation duration
int elapsed = 0;
int evtscount = 0;
int TOUCH = 0;
int itopen = 0;
int wtdpin;
int events = 0;
int ntpFailures = 0;

/// DC MOTOR SETTINGS

// int ULTRASLOW = 500; //PWM signals on ESP8266 have 10-bit resolution

int SLOW = 800;
int FAST = 1023;
int ULTRASLOW_RESTORE = ULTRASLOW;
int speedVal = ULTRASLOW; // default speed //  this is also a global val to be set upon request from ST
// int lastSpeedVal = speedVal; // for when it needs to resume // NOW RTC

int Default = FAST;
int frequency = 60000; //

// 1/0 values for motors
int A = 0;
int B = 1;
