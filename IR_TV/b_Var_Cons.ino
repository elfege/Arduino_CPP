const char *NameOTA = "TV_REMOTE_ESP8266";

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
unsigned long lastCurrentLogMilli = 0; 
unsigned long loopTime = 0;

const int DELAY_SEC = 240; //in seconds (to facilitate debugging!)
const int DELAY_SEC_5mn = 300;

int elapsed = 0;


String debugData = "";
String lastRequestedState = "";
String currentState = "";
String msgState = "";


int khz = 38; //NB Change this default value as neccessary to the correct modulation frequency

//uint32_t  TVTOG[99] PROGMEM = {4500,4450,600,1600,600,1650,550,1650,600,550,550,550,550,550,550,550,600,500,600,1650,550,1650,600,1650,550,550,550,550,600,500,600,500,600,550,550,550,550,1650,600,550,550,550,550,550,550,550,600,500,600,550,550,1650,550,550,600,1650,550,1650,550,1650,600,1650,550,1650,600,1650,550};
//uint32_t  ATVMAINBUT[99] PROGMEM = {9000,4400,600,550,550,1650,550,1650,550,1650,550,550,600,1600,600,1600,600,1600,600,1600,600,1600,600,1600,600,550,550,550,550,550,600,500,600,1600,600,550,550,550,550,1650,600,1600,600,1600,600,500,600,1600,600,550,550,1650,550,1650,550,550,600,1600,600,500,600,1600,600,1600,600,550,600};
//uint32_t  ATVMENU[99] PROGMEM = {9050,4350,600,550,550,1650,550,1650,600,1600,600,500,600,1600,600,1600,600,1600,600,1650,550,1650,550,1650,550,550,600,500,600,500,600,550,550,1650,550,1650,600,1600,600,500,600,500,600,550,550,550,550,550,600,500,600,1600,600,1600,600,550,550,1650,600,500,600,1600,600,1600,600,550,550};
//uint32_t  ATVPLAY[99] PROGMEM = {9000,4350,600,550,550,1650,600,1600,600,1600,600,500,600,1600,600,1600,600,1600,600,1600,600,1600,600,1600,600,550,600,500,600,500,600,550,550,1650,600,1600,600,500,600,1600,600,500,600,550,550,550,600,500,600,500,600,1600,600,1600,600,550,600,1600,600,500,600,1600,600,1600,600,550,600};

uint32_t  rawZeroBit[3] PROGMEM = {8950, 2200, 600}; // emulates holding a button (after pressing the button)
