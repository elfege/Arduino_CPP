boolean testRun = false;
boolean STOP = false;
boolean isBusy= false;
boolean loaded = false;
boolean isDebugEnabled = true;
boolean goingup = false;
boolean goingdown = true;
boolean hasdetected = false;
boolean findcanReceiverBusy = false;
boolean logs = false;
boolean alreadyHolding = false;
boolean trashactionOk = true;
boolean feederEmpty = false;
boolean debug = false;

int holdCans           = 60; // pushers / holders holding position
int pullIn             = 40; // pushers / holders pull back in position (no holding pos)
int pullInLP           = 45; // lower pusher must not fully retract or can get stuck behind wall
int pullOut            = 80; // pull out / hold cans 
int keepCanFromInward  = 72; // // prevent the can from sliding away inward // happens mostly when a can has bumps
int extractCan         = 180;
int previousTimeOutVal = 0;

int pushCan           = 65; // push can into position
int gateClosed        = 65; // closed and drags can into position
int gateOpen          = 55; // void compre
int gateHold          = 160; // gate helps holding falling cans // needed so upper can doesn't fall beyond upper holder
int gateSligtOpen     = gateClosed-25;
int gateSlightRelease = 90;
int holdUp            = 80;
int holdDown          = 90;
int receiverup        = 0; // receiving position
int receiverflat      = 30; // eating position
int receivertrash     = 100; // disposal position

int distance = 0; // needs to be globalfor debug purpose

String pwstate = "off";
String lastState = "";


unsigned long offTimer = 30; // in minutes
unsigned long millisStop = millis();
unsigned long downMillis = millis();
unsigned long upMillis = millis();
unsigned long tlimStart = millis();
unsigned long millisRefresh = millis();
unsigned long lastOnMillis = millis();
unsigned long lastCheck = millis();
unsigned long lastDebugMillis = millis();
