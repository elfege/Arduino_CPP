const int DELAY_SEC = 30; //in seconds
const int DELAY_PIR = 240; // report interval
const int DELAY_SHORT = 10; // report inactive 10 seconds after reporting active
unsigned long previousMillis = 0; //needed for new timing mechanism
unsigned long previousMillisRefresh = 0;
unsigned long previousMillisBlink = 0;
unsigned long previousMillisTime = 0;
unsigned long previousMillisSinceLastActive = 0;
unsigned long loopTime = 0;
unsigned long lastMessage = 0;

int val;
int elapsed = 0;
int glw;
int glowDelay = 1; // 0 disables glow()
int c;
int resendDelay = 2000;

boolean justbooted = false;
boolean noblink = false;
boolean isDebugEnabled = true;
boolean active = false;
boolean StopAll = false;
boolean inactive = false;
boolean goUp;

boolean logs = false;

String debugData = "";
String preserved = "";
