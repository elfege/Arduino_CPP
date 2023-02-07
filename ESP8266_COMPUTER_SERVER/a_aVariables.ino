boolean goUp;
boolean StopAll = false;
boolean logs = false;
boolean shutingDown = false;
boolean isDebugEnabled = true;    // enable or disable debug
boolean userOverride = false;
boolean allowshutDownWhenFail; // value to be updated by the device driver on the hub end or at next refresh
boolean noSleep = false;
int glw;
int elapsed = 0;

unsigned long OTHER_DELAY = 1000 * 60 * 60 * 10; // 10 hours
unsigned long DELAY_SEC = 600; // 10 minutes refresh 
unsigned long previousMillisBlink = 0;
unsigned long lastOnMillis = 0;
unsigned long offTimer = 30; // in minutes
unsigned long previousMillis = 0; //needed for new timing mechanism - DanO
unsigned long previousMillis_override_RESET = 0;
unsigned long bootTime = millis();

String debugData = "";
String var = "";
