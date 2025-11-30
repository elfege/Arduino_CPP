unsigned long loopTime = 0;

unsigned long previousMillis = 0; //needed for new timing mechanism
unsigned long previousMillisBlink = 0;
unsigned long previousMillisWater = 0;

const int DELAY_SEC = 600; //in seconds (to facilitate debugging!)
int glw = 0; // glowing counter
int elapsed = 0;

bool goUp = true;

boolean StopAll = false;
bool isDebugEnabled;    // enable or disable debug in this example
bool justbooted = true;
boolean logs = false;

boolean CurtainsAreOpen;

String debugData = "";
