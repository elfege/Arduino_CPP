
const int DELAY_SEC = 120; //in seconds
const int DELAY_PIR = 30; 
unsigned long previousMillis = 0; //needed for new timing mechanism
unsigned long previousMillisRefresh = 0;
unsigned long previousMillisBlink = 0;
unsigned long previousMillisSinceLastActive = 0;

int val;

bool isDebugEnabled = true;
bool active = false;

