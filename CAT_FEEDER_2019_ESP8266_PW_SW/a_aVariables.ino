boolean goUp;
boolean isDebugEnabled = true;    // enable or disable debug in this example
boolean StopAll = false;
boolean logs = false;

int glw;
int elapsed = 0;

unsigned long DELAY_SEC = 120; // 2 minutes without charging if not requested
unsigned long previousMillisBlink = 0;
unsigned long lastOnMillis = 0;
unsigned long offTimer = 30; // in minutes
String debugData = "";
