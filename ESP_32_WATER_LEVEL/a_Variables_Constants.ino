unsigned long millipreviousMillisBlink = millis(); 
unsigned long heartbeat = millis();

unsigned long previousMillisBlink = 0;
unsigned long heartbeatInterval1 = 100;  // First beat "lub" duration
unsigned long heartbeatInterval2 = 200;  // Time between "lub" and "dub"
unsigned long heartbeatInterval3 = 600;  // Second beat "dub" duration
unsigned long pauseInterval = 800;       // Time after "dub" before the next "lub"

enum HeartState {
  LUB,
  PAUSE_AFTER_LUB,
  DUB,
  PAUSE_AFTER_DUB
};

HeartState currentState = LUB;

bool lastState = level_is_low(); // Initialize with the current state on startup


bool OTArequest = false;
bool stopAll = false;