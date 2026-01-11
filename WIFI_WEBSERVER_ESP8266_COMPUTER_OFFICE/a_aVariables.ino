boolean goUp;
boolean StopAll = false;
boolean logs = false;
boolean ShutDown = false;
boolean isDebugEnabled = true;    // enable or disable debug
boolean ignorecommand = false;

int glw;
int elapsed = 0;

unsigned long OTHER_DELAY = 42000; // 12 hours
unsigned long DELAY_SEC = 600; // 10 minutes refresh 
unsigned long previousMillisBlink = 0;
unsigned long lastOnMillis = 0;
unsigned long offTimer = 30; // in minutes
unsigned long previousMillis = 0; //needed for new timing mechanism - DanO
unsigned long previousMillis_IGNORE_RESET = 0;

String debugData = "";
String var = "";
