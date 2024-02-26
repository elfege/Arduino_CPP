

const int DELAY_SEC = 120; //in seconds (to facilitate debugging!)
unsigned long previousMillis = 0; //needed for new timing mechanism
unsigned long previousMillisBlink = 0;
unsigned long Start;
unsigned long previousMillisFeed = 0;
unsigned long loopTime = 0;
unsigned long lastBoot = millis();
unsigned long lastTimeDisplay = 0;
int elapsed = 0;
int defaultProgramDelay = 6; // default value in hours 0 means never (condition set in loop())

// 1 hours = 1 * 1000 = 1000 millis / 1 second * 60 = 60000 millis / 1minute * 60 = 3600 000 millis() = 1 hours
unsigned long programDelay = defaultProgramDelay * 1000 * 60 * 60;

int wtdpin;
int qty = 1;
int MaxQty = 15;
 
boolean isDebugEnabled;    // enable or disable debug in this example
boolean dontFeed = false;
boolean currentlyFeeding = false;
boolean stopFulltest = false;
boolean watchdogInitOk = false;
boolean stopAll = false;

boolean wasempty = true;

String debugData = "";
String previousDebugData = "";
String formattedDate;
String dayStamp;
String timeStamp;
String TheTime;
