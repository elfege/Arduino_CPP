unsigned long loopTime = 0;

unsigned long previousMillis = 0; //needed for new timing mechanism
unsigned long previousMillisBlink = 0;
unsigned long runMillis = 0;
unsigned long operationTime = 0;

const int DELAY_SEC = 600; //in seconds (to facilitate debugging!)
int glw = 0; // glowing counter
int elapsed = 0;

boolean goUp = true;

boolean StopAll = false;
boolean isDebugEnabled;    // enable or disable debug in this example
boolean justbooted = true;
boolean logs = false;

boolean CurtainsAreOpen = true; 
boolean isRunning = false;

String stateSwitch = "";
String stateCurtain = "";
String debugData = "";
