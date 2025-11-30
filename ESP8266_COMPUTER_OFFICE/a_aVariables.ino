boolean goUp;
boolean StopAll = false;
boolean logs = false;
boolean isDebugEnabled = true;    // enable or disable debug
boolean userOverride = false;
boolean allowshutDownWhenFail; // value to be updated by the device driver on the hub end or at next refresh
boolean noSleep = false;
boolean STOP = false; // required when "off" command sent more than once within while() time
boolean OnOffRunning = false;
boolean new_state = false;


int glw;
int elapsed = 0;

unsigned long OTHER_DELAY = 1000 * 60 * 60 * 10; // 10 hours
unsigned long DELAY_SEC = 600; // 10 minutes refresh
unsigned long DELAY_SLEEP = 120000;
unsigned long DELAY_FAILSAFE = 600000;
unsigned long DELAY_NEWATTEMPT = 60000;
unsigned long lastStateRefreshMillis = millis();
unsigned long last_message_sent_millis = millis();

unsigned long previousMillisBlink = 0;
unsigned long lastOnMillis = 0;
unsigned long offTimer = 30; // in minutes
unsigned long previousMillis = 0; //needed for new timing mechanism - DanO
unsigned long previousMillis_override_RESET = 0;
unsigned long bootTime = millis();
unsigned long millisHeartBit = millis();


String var = "";
String last_state = "off";
String last_state_sent = last_state;
