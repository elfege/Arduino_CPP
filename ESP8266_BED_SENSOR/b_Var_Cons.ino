boolean StopAll = false;
boolean RefreshPage = true;
boolean isDebugEnabled;    // enable or disable debug in this example
boolean justbooted = true;
boolean logs = false;

bool goUp = true;
int glw = 0; // glowing counter


unsigned long ResetTimer = 1000 * 60 * 60 * 4; // 4 hours reset timer
unsigned long previousMillis = 0;
unsigned long previousMillisRefresh = 0;
unsigned long previousMillisBlink = 0;
unsigned long previousMillisTvState = 0;
unsigned long loopTime = 0;
unsigned long lastUpdateMillis = millis();

const int DELAY_SEC = 240; //in seconds (to facilitate debugging!)
const int DELAY_SEC_5mn = 300;

int elapsed = 0;
int counter = 0;

String debugData = "";
String lastState = "";
String currentState = "";
String msgState = "";
