#define DELAY_CMD 1000 // TOO HIGH OF A VALUE MAY TRIGGER WTD!

String cmd; // heatingSetpoint or coolingSetpoint
String StringForRefresh; // "heatingSetpoint + space + value // for when the device refreshes its status
String StringTemp = "";
String timerResult = "";
String lastSetPoint = ""; // setpoint to be remembered for the hub's driver setpoint display
String lastSetpointRequested = ""; // setpoint to be remebered to reinstante boost setpoint after a normal setpoint request.
// it must become identical to boost setpoint for master ()
// to know that something's changed and boost setpoint needs to be reinstated
String requiredSetpointForBoost = "";
String lastMode = "";
String lastActiveMode = ""; // for when fan is set back to auto


const int DELAY_SEC = 240; //in seconds (to facilitate debugging!)
unsigned long ResetTimer = 1000 * 60 * 60 * 4; // 4 hours reset timer
unsigned long previousMillis = 0; //needed for new timing mechanism
unsigned long previousMillisBlink = 0;
unsigned long previousMillisSetpoint = 0;
unsigned long loopTime = 0;
unsigned long requestTurboMillis = 0;
unsigned long stopAllRequest = 0;
unsigned long lastTurboActive = millis();
unsigned long lastNTP = millis();
unsigned long millisBoot = millis();
unsigned long lastLogMillis = millis();
unsigned long lastModeRequest = millis();
unsigned long fanDurationWhenOff = 10 * 60 * 1000;
unsigned long offRequestMillis = fanDurationWhenOff;


const int DELAY_SEC_5mn = 600;

boolean ShutDown = false;
boolean isDebugEnabled;    // enable or disable debug in this example
boolean logs = false;
boolean turboIsOn = false;
boolean  requestTurbo = false;
boolean tempRequest = false;
boolean ledToggled = false;
boolean ntpFailed = false;
boolean stopNtp = false;
boolean fanCirculate = false;
boolean bootRestore = false;

bool RefreshPage = true;
String str1;
String str2;
String str3;
String strFinal;
String debugData = "";
String lastBootDate = "";


bool isOff = false;
bool StopAll = false;
bool onrequested = false;

bool goUp = true;
int glw = 0; // glowing counter
int ntpFailures = 0;

int elapsed = 0;

int khz = 38000; //NB Change this default value as neccessary to the correct modulation frequency

/***************************Turn ON the AC*****************************************/

#define onHex 0xA1826AFF

uint32_t ON[199] PROGMEM = {4368, 4428, 544, 1640, 516, 576, 516, 1668, 516, 580, 516, 576, 516, 580, 516, 576, 520, 1664, 516, 1668, 520, 576, 516, 576, 520, 576, 516, 576, 516, 580, 516, 1664, 544, 556, 516, 580, 516, 1668, 516, 1664, 544, 552, 516, 1668, 516, 1660, 544, 552, 516, 584, 536, 1648, 516, 1664, 516, 1660, 520, 1664, 516, 1664, 516, 1664, 516, 1664, 516, 1668, 516, 1668, 516, 1664, 516, 1664, 516, 1668, 516, 1664, 516, 1664, 516, 1664, 516, 1664, 520, 580, 512, 1664, 520, 1660, 520, 576, 516, 576, 516, 580, 516, 580, 512, 580, 516, 5252, 4400, 4384, 516, 576, 516, 1668, 516, 580, 516, 1668, 516, 1664, 520, 1660, 516, 1664, 516, 580, 520, 576, 516, 1668, 520, 1660, 544, 1640, 512, 1668, 516, 1664, 512, 580, 516, 1672, 516, 1668, 516, 580, 516, 576, 516, 1664, 516, 580, 516, 576, 516, 1668, 516, 1664, 516, 584, 512, 580, 516, 576, 516, 580, 516, 576, 516, 580, 516, 576, 516, 580, 516, 580, 516, 580, 516, 576, 516, 580, 516, 576, 520, 576, 516, 576, 516, 580, 516, 1668, 520, 576, 516, 580, 512, 1664, 520, 1664, 512, 1664, 516, 1664, 520, 1664, 516};

// Turn OFF the AC
//Decoded SAMSUNG: A1026EFF(32 bits)
uint32_t  OFF[199] PROGMEM = {4432, 4344, 580, 1604, 576, 516, 580, 1604, 
580, 516, 576, 540, 556, 536, 556, 536, 528, 1648, 532, 552, 576, 520, 
576, 516, 576, 1604, 576, 540, 548, 536, 560, 520, 580, 516, 580, 540,
548, 1608, 580, 1600, 580, 516, 576, 1604, 580, 540, 556, 1604, 580, 
536, 556, 1604, 580, 1600, 580, 1604, 576, 1620, 528, 1652, 528, 1632, 
548, 1636, 576, 1604, 580, 1608, 576, 1604, 580, 1600, 576, 1604, 580, 
1600, 580, 1600, 580, 1604, 576, 1608, 572, 1612, 576, 1620, 560, 1620, 
528, 1632, 584, 1620, 528, 544, 584, 512, 548, 548, 544, 5240, 4404, 4384, 
576, 520, 576, 1608, 576, 540, 552, 1624, 564, 1604, 548, 1632, 580, 1600, 
580, 516, 580, 1604, 580, 1604, 576, 1604, 544, 552, 576, 1608, 576, 1620,
532, 1648, 532, 1648, 532, 1656, 532, 548, 580, 536, 560, 1600, 580, 536, 
552, 1608, 576, 532, 532, 1660, 560, 520, 580, 512, 576, 516, 580, 516, 576,
520, 576, 516, 576, 520, 576, 520, 576, 520, 580, 512, 584, 540, 520, 568, 
556, 540, 552, 532, 532, 548, 580, 540, 556, 544, 520, 564, 560, 536, 556,
540, 556, 520, 576, 1616, 532, 1636, 544, 1636, 548}; //works on C&H... 

unsigned long offHex = 0xA1026CFF;

// TOGGLE THE LED
uint32_t  TOGGLELED[199] PROGMEM = {4432, 4360, 572, 1608, 576, 524, 568, 1612, 572, 524, 572, 524, 572, 520, 572, 1608, 576, 524, 572, 524, 572, 524, 572, 524, 568, 524, 572, 1604, 576, 520, 572, 524, 572, 524, 572, 1608, 576, 1608, 572, 1604, 576, 1608, 572, 1608, 572, 1608, 576, 1604, 576, 1608, 576, 1612, 576, 1604, 576, 1604, 576, 1608, 572, 1604, 576, 1608, 576, 1604, 576, 1608, 576, 1612, 576, 1604, 576, 1604, 576, 1608, 572, 1608, 576, 1604, 576, 1604, 580, 1604, 576, 524, 548, 1628, 576, 1604, 580, 1604, 572, 524, 544, 1636, 576, 520, 548, 1636, 516, 5248, 4404, 4380, 580, 520, 548, 1632, 576, 524, 544, 1636, 572, 1608, 576, 1608, 572, 524, 568, 1616, 572, 1616, 572, 1608, 572, 1608, 576, 1604, 572, 524, 572, 1608, 548, 1636, 572, 1608, 576, 524, 572, 524, 544, 548, 548, 548, 544, 548, 548, 548, 572, 520, 548, 548, 548, 552, 544, 548, 548, 548, 544, 548, 548, 548, 544, 548, 548, 548, 544, 552, 544, 552, 572, 524, 572, 520, 560, 536, 572, 520, 548, 548, 572, 520, 548, 548, 548, 1632, 576, 524, 544, 548, 548, 548, 544, 1636, 576, 524, 544, 1636, 576, 524, 512};

// TURBO MODE ON
uint32_t  TURBO[199] PROGMEM = {4424, 4388, 564, 1624, 528, 548, 580, 1604, 580, 536, 556, 516, 548, 568, 556, 1624, 564, 516, 544, 552, 576, 520, 576, 536, 556, 516, 580, 1604, 576, 516, 576, 516, 580, 1604, 580, 1604, 580, 1604, 580, 1600, 580, 1600, 580, 1600, 580, 1604, 576, 1600, 580, 1604, 580, 1608, 576, 1604, 576, 1604, 576, 1616, 532, 1652, 528, 1652, 532, 1648, 532, 1644, 544, 1644, 532, 1636, 544, 1636, 544, 1632, 580, 1600, 580, 1600, 580, 1600, 580, 1600, 580, 520, 576, 1604, 544, 1632, 580, 1604, 576, 516, 576, 1608, 576, 520, 576, 520, 540, 5220, 4436, 4348, 580, 516, 576, 1608, 576, 516, 576, 1608, 580, 1600, 580, 1600, 576, 520, 576, 1608, 580, 1608, 576, 1604, 580, 1600, 576, 1604, 576, 516, 580, 1604, 576, 1604, 576, 520, 576, 540, 556, 536, 556, 520, 576, 516, 580, 536, 556, 516, 544, 552, 544, 564, 532, 552, 576, 516, 576, 516, 580, 516, 576, 540, 556, 540, 520, 548, 576, 520, 580, 540, 552, 536, 556, 524, 572, 520, 576, 516, 580, 540, 552, 532, 560, 520, 580, 1608, 576, 544, 548, 536, 528, 552, 576, 1604, 580, 516, 576, 1608, 580, 1600, 548};

// PURE HEAT: set to actual heat, at max heat, for smart home management facilitation. Auto, on these models, kinda sucks. 
uint32_t PUREHEAT[199] PROGMEM = {4424, 4348, 576, 1604, 572, 524, 568, 1612, 572, 524, 568, 524, 540, 556, 564, 528, 568, 1608, 572, 1612, 572, 524, 568, 524, 568, 1608, 572, 1608, 572, 524, 568, 1612, 572, 1608, 572, 524, 568, 1608, 572, 1608, 572, 1604, 572, 1608, 572, 524, 568, 524, 568, 528, 568, 1608, 572, 1604, 572, 1608, 572, 1604, 572, 1608, 572, 1608, 568, 1608, 572, 1608, 568, 1616, 572, 1608, 568, 1608, 572, 1608, 572, 1604, 572, 1604, 572, 1608, 572, 1608, 572, 528, 568, 1604, 572, 1608, 572, 524, 568, 524, 568, 524, 544, 552, 568, 1608, 568, 5188, 4420, 4352, 576, 524, 568, 1608, 576, 524, 564, 1612, 576, 1604, 572, 1604, 572, 1608, 572, 524, 568, 528, 568, 1612, 568, 1608, 572, 524, 568, 528, 564, 1608, 572, 524, 568, 528, 568, 1612, 572, 524, 568, 524, 568, 524, 568, 528, 568, 1608, 568, 1608, 572, 1608, 572, 524, 568, 528, 568, 524, 568, 524, 540, 552, 572, 520, 572, 524, 568, 524, 568, 528, 568, 524, 568, 524, 568, 528, 568, 524, 568, 524, 568, 524, 568, 528, 568, 1612, 572, 524, 568, 524, 568, 1608, 572, 1604, 572, 1608, 572, 1608, 572, 524, 568}; 
// PURE COOL: set to actual cool, at max cool, for smart home management facilitation. Auto, on these models, kinda sucks. 
uint32_t PURECOOL[199] PROGMEM = {4408, 4380, 540, 1636, 536, 560, 536, 1644, 540, 556, 536, 556, 540, 552, 540, 556, 536, 1640, 536, 1648, 512, 584, 536, 556, 536, 1636, 516, 1664, 540, 556, 536, 560, 536, 556, 536, 560, 536, 1640, 544, 1636, 540, 556, 536, 556, 536, 556, 540, 556, 536, 556, 536, 1644, 540, 1636, 544, 1636, 540, 1640, 536, 1640, 512, 1668, 536, 1640, 544, 1640, 536, 1644, 544, 1636, 540, 1636, 516, 1664, 540, 1640, 540, 1636, 540, 1640, 540, 1636, 516, 584, 540, 1636, 540, 1640, 536, 1640, 512, 1664, 516, 580, 540, 1640, 540, 556, 560, 5216, 4364, 4412, 540, 556, 536, 1644, 540, 556, 536, 1644, 540, 1636, 544, 1636, 540, 1636, 540, 560, 536, 556, 540, 1640, 540, 1640, 512, 580, 540, 556, 536, 1636, 540, 1640, 540, 1640, 540, 1640, 512, 584, 540, 552, 540, 1636, 540, 1636, 540, 1636, 544, 1636, 512, 1668, 512, 584, 540, 556, 536, 556, 532, 560, 536, 556, 540, 552, 540, 556, 536, 556, 536, 560, 536, 556, 536, 556, 536, 556, 540, 556, 536, 556, 536, 556, 540, 556, 536, 1644, 540, 556, 536, 556, 536, 556, 540, 552, 540, 1636, 544, 552, 536, 1648, 560}; 


// Temperatures // all temperatures are based on AUTO mode

uint32_t  Deg62[199] PROGMEM = {4372, 4420, 520, 1660, 520, 576, 516, 1668, 516, 580, 516, 576, 516, 580, 516, 576, 516, 1668, 516, 1668, 520, 576, 516, 580, 516, 576, 516, 576, 520, 576, 516, 1668, 516, 580, 516, 580, 516, 1668, 516, 1664, 516, 580, 516, 576, 520, 576, 516, 580, 516, 576, 520, 1664, 516, 1664, 516, 1664, 520, 1660, 520, 1660, 520, 1664, 516, 1664, 516, 1664, 516, 1672, 516, 1664, 516, 1664, 516, 1664, 516, 1664, 516, 1664, 516, 1664, 520, 1664, 516, 580, 516, 1664, 516, 1664, 516, 580, 516, 1664, 520, 1660, 520, 576, 516, 580, 564, 5204, 4372, 4412, 516, 580, 516, 1668, 516, 576, 516, 1668, 516, 1664, 516, 1664, 520, 1660, 520, 580, 516, 580, 516, 1664, 520, 1664, 516, 1660, 520, 1660, 520, 1664, 516, 576, 516, 1672, 516, 1668, 516, 580, 516, 576, 516, 1664, 516, 1664, 516, 1664, 516, 1664, 516, 1668, 516, 580, 516, 576, 520, 576, 516, 576, 516, 580, 516, 580, 512, 580, 516, 580, 516, 580, 516, 576, 520, 576, 516, 576, 520, 576, 516, 580, 516, 576, 516, 580, 516, 1672, 516, 580, 512, 580, 516, 1664, 516, 576, 520, 576, 516, 1664, 516, 1668, 516};
uint32_t  Deg63[199] PROGMEM = {4428, 4356, 516, 1664, 516, 580, 516, 1668, 516, 576, 516, 580, 516, 576, 516, 580, 516, 1668, 516, 1668, 520, 576, 516, 580, 516, 576, 516, 580, 516, 576, 520, 1664, 520, 580, 516, 580, 516, 1668, 516, 1664, 520, 580, 512, 580, 516, 580, 516, 576, 516, 1668, 516, 1668, 520, 1664, 516, 1664, 516, 1664, 520, 1664, 516, 1664, 516, 1664, 516, 1668, 516, 1672, 516, 1664, 516, 1664, 520, 1664, 516, 1664, 516, 1664, 520, 1664, 516, 1668, 516, 580, 516, 1664, 516, 1668, 516, 576, 516, 1668, 520, 1664, 516, 580, 516, 1668, 516, 5260, 4372, 4416, 516, 580, 516, 1668, 516, 580, 516, 1668, 516, 1664, 516, 1668, 516, 1660, 520, 580, 516, 580, 516, 1672, 516, 1664, 516, 1664, 516, 1664, 516, 1668, 516, 576, 516, 1672, 516, 1672, 516, 576, 516, 580, 516, 1664, 564, 1616, 516, 1668, 516, 1664, 516, 580, 516, 580, 516, 580, 516, 576, 520, 576, 516, 576, 520, 576, 516, 580, 516, 580, 516, 580, 516, 580, 516, 576, 516, 580, 516, 576, 516, 580, 516, 580, 516, 580, 516, 1664, 520, 576, 516, 580, 516, 1664, 516, 580, 516, 580, 516, 1664, 516, 580, 516};
uint32_t  Deg64[199] PROGMEM = {4428, 4360, 576, 1604, 572, 524, 572, 1612, 572, 524, 572, 524, 568, 524, 572, 524, 572, 1608, 576, 1608, 576, 524, 572, 524, 568, 524, 572, 524, 572, 520, 572, 1612, 576, 524, 572, 524, 572, 1612, 572, 1608, 576, 520, 572, 524, 572, 524, 568, 1612, 572, 528, 568, 1612, 572, 1608, 576, 1604, 576, 1604, 576, 1604, 576, 1608, 572, 1608, 572, 1612, 572, 1616, 572, 1608, 576, 1604, 576, 1604, 576, 1608, 572, 1608, 572, 1608, 576, 1608, 572, 528, 568, 1608, 572, 1608, 572, 528, 568, 1612, 572, 1608, 576, 1612, 568, 1608, 568, 5212, 4432, 4352, 576, 524, 572, 1612, 572, 524, 572, 1608, 576, 1608, 572, 1604, 576, 1608, 572, 528, 568, 528, 568, 1612, 576, 1604, 576, 1608, 572, 1608, 572, 1608, 576, 520, 572, 1616, 572, 1612, 572, 528, 568, 524, 572, 1608, 572, 1608, 572, 1608, 576, 520, 572, 1612, 576, 524, 572, 524, 568, 524, 572, 524, 572, 520, 572, 524, 572, 520, 572, 524, 572, 528, 568, 524, 572, 524, 572, 520, 572, 524, 568, 524, 572, 524, 572, 524, 572, 1612, 576, 520, 572, 524, 572, 1604, 576, 524, 572, 520, 572, 524, 572, 524, 564};
uint32_t  Deg65[199] PROGMEM = {4400, 4384, 572, 1612, 568, 528, 544, 1636, 572, 528, 568, 528, 540, 552, 544, 552, 564, 1616, 568, 1620, 568, 528, 544, 552, 540, 556, 540, 552, 544, 552, 564, 1616, 572, 528, 544, 556, 564, 1616, 572, 1612, 568, 528, 540, 556, 540, 552, 540, 1640, 548, 1636, 572, 1616, 548, 1632, 572, 1612, 568, 1612, 568, 1612, 572, 1612, 568, 1612, 568, 1616, 568, 1616, 572, 1612, 568, 1612, 544, 1636, 572, 1612, 568, 1616, 540, 1636, 572, 1612, 572, 528, 544, 1636, 548, 1632, 572, 528, 540, 1640, 544, 1636, 572, 1608, 572, 528, 556, 5220, 4400, 4384, 572, 528, 540, 1640, 572, 528, 560, 1620, 568, 1616, 560, 1620, 544, 1636, 568, 532, 540, 556, 540, 1640, 516, 1668, 568, 1612, 540, 1644, 544, 1636, 544, 552, 544, 1640, 544, 1644, 568, 532, 540, 552, 540, 1640, 568, 1612, 568, 1612, 544, 552, 544, 552, 544, 556, 540, 552, 540, 556, 540, 552, 540, 556, 540, 552, 544, 552, 540, 556, 540, 556, 540, 556, 540, 556, 540, 552, 540, 556, 540, 552, 544, 552, 540, 556, 540, 1644, 560, 536, 544, 552, 540, 1640, 544, 552, 540, 556, 540, 552, 544, 1640, 560};
uint32_t  Deg66[199] PROGMEM = {4400, 4380, 576, 1608, 572, 524, 568, 1612, 576, 524, 568, 524, 572, 524, 568, 528, 568, 1612, 572, 1612, 572, 528, 568, 524, 568, 528, 568, 524, 572, 524, 568, 1612, 576, 524, 572, 524, 572, 1612, 572, 1608, 572, 524, 572, 524, 568, 1608, 572, 524, 572, 524, 572, 1612, 576, 1608, 572, 1608, 572, 1608, 572, 1612, 572, 1604, 576, 1608, 572, 1612, 572, 1612, 576, 1604, 576, 1608, 572, 1608, 572, 1608, 572, 1608, 572, 1612, 572, 1608, 572, 528, 568, 1608, 576, 1608, 572, 524, 572, 1612, 572, 524, 568, 528, 568, 524, 556, 5208, 4432, 4356, 572, 528, 568, 1612, 572, 524, 572, 1612, 572, 1608, 572, 1608, 572, 1608, 572, 528, 572, 524, 572, 1612, 572, 1604, 576, 1608, 572, 1608, 572, 1608, 572, 528, 568, 1612, 576, 1612, 572, 524, 572, 524, 568, 1608, 576, 1604, 572, 528, 568, 1612, 572, 1612, 572, 524, 576, 520, 568, 524, 572, 524, 568, 524, 572, 524, 568, 524, 572, 524, 572, 528, 568, 524, 568, 528, 568, 524, 572, 524, 568, 524, 572, 524, 568, 528, 568, 1612, 576, 524, 568, 528, 568, 1608, 572, 524, 572, 1608, 576, 1608, 572, 1612, 556};
uint32_t  Deg67[199] PROGMEM = {4428, 4360, 572, 1608, 572, 528, 568, 1612, 572, 524, 572, 524, 568, 524, 572, 524, 568, 1612, 572, 1616, 572, 524, 572, 524, 568, 524, 572, 524, 568, 524, 572, 1612, 572, 528, 572, 524, 568, 1612, 572, 1608, 576, 524, 568, 524, 572, 1608, 572, 524, 572, 1612, 572, 1616, 572, 1608, 572, 1608, 572, 1608, 572, 1608, 572, 1608, 572, 1608, 572, 1608, 576, 1612, 572, 1608, 572, 1608, 576, 1604, 576, 1604, 576, 1604, 576, 1608, 572, 1608, 572, 528, 572, 1604, 576, 1604, 572, 528, 568, 1612, 572, 524, 572, 524, 568, 1608, 556, 5216, 4428, 4356, 576, 520, 572, 1612, 572, 524, 572, 1608, 576, 1608, 572, 1608, 572, 1608, 576, 524, 568, 528, 572, 1608, 576, 1604, 576, 1608, 572, 1608, 572, 1608, 576, 520, 572, 1616, 572, 1612, 576, 520, 572, 524, 572, 1608, 572, 1608, 572, 524, 568, 1616, 572, 524, 572, 528, 568, 524, 572, 524, 568, 524, 572, 524, 572, 524, 568, 524, 572, 528, 568, 528, 568, 524, 572, 524, 568, 524, 572, 524, 568, 524, 572, 524, 572, 524, 572, 1608, 572, 524, 572, 524, 568, 1608, 572, 524, 572, 1612, 572, 1604, 576, 528, 552};
uint32_t  Deg68[199] PROGMEM = {4432, 4348, 576, 1604, 576, 524, 572, 1608, 576, 520, 576, 520, 572, 524, 572, 520, 572, 1608, 576, 1612, 576, 520, 572, 524, 572, 520, 572, 524, 572, 520, 576, 1604, 576, 524, 572, 524, 572, 1612, 572, 1604, 576, 524, 572, 520, 576, 1604, 576, 1604, 576, 524, 572, 1608, 576, 1604, 580, 1600, 576, 1604, 580, 1604, 576, 1604, 576, 1604, 576, 1604, 580, 1608, 576, 1604, 580, 1604, 576, 1604, 576, 1608, 576, 1604, 576, 1604, 576, 1608, 576, 524, 572, 1604, 576, 1604, 576, 520, 576, 1608, 576, 520, 576, 1608, 576, 1604, 516, 5264, 4432, 4356, 576, 520, 572, 1608, 580, 520, 572, 1608, 576, 1608, 576, 1600, 580, 1600, 580, 520, 576, 520, 576, 1608, 576, 1604, 576, 1604, 576, 1604, 576, 1604, 576, 524, 572, 1608, 576, 1612, 576, 520, 576, 520, 572, 1604, 580, 1600, 580, 520, 572, 520, 576, 1604, 576, 524, 572, 524, 572, 520, 576, 520, 572, 520, 576, 520, 572, 520, 576, 520, 576, 524, 572, 520, 572, 524, 572, 520, 576, 520, 572, 520, 572, 524, 572, 524, 572, 1612, 576, 520, 576, 520, 572, 1604, 576, 524, 572, 1608, 576, 520, 576, 524, 508};
uint32_t  Deg69[199] PROGMEM = {4372, 4432, 572, 1608, 572, 524, 568, 1616, 572, 524, 568, 524, 572, 524, 568, 524, 572, 1612, 568, 1620, 568, 528, 568, 524, 568, 528, 568, 524, 572, 524, 568, 1616, 568, 528, 568, 528, 572, 1612, 572, 1608, 572, 524, 572, 524, 568, 1612, 572, 1608, 572, 1612, 572, 1616, 572, 1608, 572, 1608, 572, 1608, 572, 1612, 568, 1612, 568, 1616, 568, 1612, 572, 1616, 572, 1608, 572, 1608, 576, 1608, 572, 1608, 572, 1612, 568, 1612, 572, 1612, 568, 528, 572, 1608, 572, 1608, 572, 524, 572, 1616, 568, 524, 572, 1616, 568, 524, 516, 5264, 4424, 4360, 576, 524, 572, 1612, 568, 528, 568, 1616, 572, 1608, 572, 1608, 572, 1612, 572, 524, 572, 524, 572, 1612, 572, 1612, 568, 1612, 572, 1608, 572, 1608, 572, 524, 568, 1620, 572, 1616, 568, 524, 572, 524, 572, 1608, 568, 1612, 572, 524, 568, 528, 568, 524, 572, 528, 568, 524, 572, 524, 572, 524, 568, 524, 568, 528, 568, 524, 572, 528, 564, 528, 572, 524, 568, 528, 568, 524, 568, 524, 572, 524, 568, 528, 568, 528, 568, 1612, 572, 524, 568, 528, 568, 1612, 568, 528, 568, 1616, 568, 528, 564, 1620, 516};
uint32_t  Deg70[199] PROGMEM = {4368, 4436, 576, 1608, 576, 520, 576, 1608, 576, 520, 572, 524, 572, 524, 572, 520, 576, 1604, 576, 1612, 576, 524, 544, 548, 548, 548, 572, 524, 572, 520, 572, 1612, 576, 524, 548, 548, 572, 1608, 576, 1608, 576, 520, 548, 1636, 576, 520, 548, 548, 544, 552, 572, 1608, 580, 1604, 576, 1604, 576, 1604, 576, 1608, 576, 1604, 576, 1604, 576, 1608, 576, 1608, 580, 1604, 576, 1604, 576, 1604, 576, 1608, 576, 1604, 576, 1604, 576, 1608, 580, 520, 544, 1632, 576, 1608, 576, 524, 540, 552, 544, 1636, 576, 520, 544, 552, 512, 5260, 4400, 4384, 576, 524, 544, 1636, 580, 520, 544, 1636, 576, 1608, 576, 1604, 576, 1604, 576, 524, 544, 556, 540, 1640, 576, 1604, 576, 1604, 580, 1604, 572, 1608, 576, 524, 544, 1640, 572, 1612, 576, 524, 544, 552, 540, 1636, 576, 524, 540, 1640, 576, 1604, 576, 1608, 576, 524, 544, 552, 544, 548, 544, 552, 544, 552, 544, 548, 544, 552, 544, 552, 548, 548, 544, 548, 544, 552, 544, 552, 544, 548, 544, 552, 544, 548, 544, 556, 544, 1636, 580, 520, 544, 548, 544, 1636, 572, 1608, 572, 524, 544, 1640, 576, 1608, 512};
uint32_t  Deg71[199] PROGMEM = {4340, 4432, 516, 1668, 540, 560, 536, 1644, 512, 584, 540, 556, 536, 560, 536, 556, 540, 1640, 512, 1676, 540, 556, 540, 556, 536, 556, 540, 556, 536, 556, 540, 1644, 512, 588, 536, 560, 536, 1644, 540, 1640, 540, 556, 540, 1644, 540, 556, 540, 556, 536, 1644, 536, 1648, 540, 1640, 536, 1644, 544, 1636, 540, 1644, 540, 1636, 544, 1640, 540, 1640, 516, 1672, 540, 1640, 544, 1636, 540, 1644, 540, 1640, 540, 1640, 536, 1644, 540, 1644, 540, 560, 536, 1640, 536, 1644, 540, 556, 540, 556, 540, 1636, 540, 560, 540, 1644, 556, 5216, 4392, 4392, 540, 556, 540, 1644, 540, 556, 540, 1644, 536, 1644, 540, 1640, 540, 1640, 540, 560, 536, 560, 540, 1640, 540, 1644, 540, 1640, 540, 1640, 540, 1640, 540, 560, 536, 1644, 544, 1644, 540, 560, 536, 556, 540, 1640, 540, 556, 540, 1644, 512, 1668, 544, 556, 536, 560, 540, 552, 540, 556, 540, 556, 536, 556, 540, 556, 540, 556, 536, 560, 536, 560, 536, 556, 540, 556, 536, 556, 540, 556, 536, 560, 536, 556, 540, 556, 540, 1640, 540, 556, 540, 556, 540, 1636, 544, 1640, 540, 556, 536, 1644, 544, 556, 560};
uint32_t  Deg72[199] PROGMEM = {4376, 4424, 572, 1608, 572, 524, 572, 1612, 572, 520, 576, 520, 572, 524, 572, 520, 572, 1612, 572, 1616, 572, 520, 572, 524, 572, 520, 572, 524, 572, 524, 572, 1612, 572, 524, 572, 524, 572, 1612, 572, 1608, 572, 524, 572, 1612, 572, 524, 572, 1612, 576, 520, 572, 1612, 572, 1608, 572, 1608, 572, 1608, 576, 1604, 576, 1608, 572, 1608, 576, 1608, 572, 1612, 576, 1604, 576, 1608, 572, 1608, 572, 1608, 572, 1608, 572, 1608, 572, 1612, 572, 524, 572, 1608, 572, 1608, 576, 520, 572, 520, 576, 1604, 576, 1608, 572, 1612, 512, 5264, 4428, 4360, 572, 524, 572, 1612, 572, 524, 572, 1612, 572, 1608, 572, 1608, 572, 1612, 572, 524, 572, 524, 568, 1616, 572, 1608, 572, 1608, 572, 1608, 576, 1608, 572, 520, 572, 1616, 572, 1616, 572, 524, 572, 520, 572, 1608, 572, 524, 572, 1612, 572, 520, 572, 1616, 572, 524, 572, 524, 572, 520, 572, 524, 572, 520, 572, 524, 572, 520, 576, 524, 568, 528, 568, 524, 568, 528, 572, 520, 516, 576, 572, 524, 572, 524, 572, 524, 572, 1612, 576, 520, 572, 524, 572, 1608, 572, 1608, 572, 520, 576, 520, 572, 524, 516};
uint32_t  Deg73[199] PROGMEM = {4428, 4380, 584, 1596, 584, 512, 584, 1600, 584, 512, 584, 512, 580, 516, 580, 512, 580, 1604, 580, 1604, 584, 516, 580, 512, 584, 512, 580, 516, 580, 512, 584, 1600, 584, 512, 584, 516, 580, 1600, 588, 1596, 584, 516, 580, 1600, 584, 512, 584, 1600, 584, 1600, 584, 1600, 588, 1596, 584, 1596, 584, 1600, 584, 1592, 588, 1596, 584, 1596, 584, 1600, 584, 1604, 584, 1596, 584, 1596, 584, 1600, 584, 1596, 584, 1600, 584, 1596, 584, 1596, 588, 512, 584, 1596, 584, 1596, 584, 512, 584, 512, 584, 1596, 584, 1596, 584, 516, 512, 5260, 4444, 4348, 584, 512, 584, 1600, 584, 512, 584, 1600, 584, 1596, 584, 1600, 584, 1596, 584, 516, 584, 512, 584, 1600, 584, 1596, 588, 1592, 588, 1596, 584, 1596, 588, 512, 580, 1600, 588, 1600, 588, 512, 580, 516, 580, 1596, 584, 512, 584, 1600, 584, 512, 584, 512, 584, 512, 584, 512, 584, 512, 580, 512, 584, 512, 584, 508, 588, 508, 584, 512, 584, 512, 584, 512, 584, 512, 584, 508, 584, 512, 584, 508, 588, 508, 584, 512, 584, 1600, 588, 508, 584, 512, 584, 1596, 584, 1596, 588, 508, 584, 512, 584, 1600, 512};
uint32_t  Deg74[199] PROGMEM = {4424, 4376, 572, 1608, 572, 528, 568, 1612, 572, 528, 540, 552, 568, 528, 568, 524, 568, 1612, 572, 1616, 572, 528, 568, 524, 568, 528, 568, 524, 572, 524, 568, 1616, 572, 524, 576, 524, 564, 1616, 572, 1608, 572, 528, 568, 1612, 576, 1608, 572, 524, 568, 528, 568, 1616, 572, 1612, 572, 1608, 572, 1608, 576, 1608, 572, 1608, 572, 1608, 572, 1612, 572, 1616, 572, 1608, 572, 1608, 576, 1608, 572, 1608, 572, 1608, 572, 1608, 576, 1608, 572, 528, 568, 1608, 576, 1608, 572, 524, 572, 524, 568, 524, 572, 524, 512, 584, 564, 5204, 4428, 4356, 576, 524, 568, 1616, 572, 524, 568, 1612, 572, 1612, 572, 1608, 572, 1608, 572, 528, 568, 528, 568, 1616, 572, 1608, 572, 1608, 572, 1612, 568, 1612, 572, 524, 568, 1616, 572, 1616, 572, 524, 568, 524, 572, 1608, 572, 528, 564, 528, 568, 1608, 576, 1608, 572, 528, 568, 528, 568, 524, 572, 520, 572, 524, 568, 528, 568, 524, 572, 524, 572, 524, 572, 524, 568, 528, 568, 524, 572, 524, 572, 520, 572, 524, 568, 528, 572, 1612, 572, 528, 568, 524, 568, 1612, 572, 1608, 572, 1608, 572, 1608, 572, 1612, 572};
uint32_t  Deg75[199] PROGMEM = {4428, 4372, 572, 1608, 576, 520, 572, 1608, 576, 524, 544, 548, 572, 524, 576, 520, 572, 1604, 580, 1608, 572, 524, 572, 524, 544, 548, 576, 520, 544, 548, 576, 1608, 548, 552, 572, 524, 572, 1608, 572, 1608, 576, 520, 548, 1636, 576, 1604, 572, 524, 544, 1640, 572, 1612, 576, 1604, 576, 1608, 572, 1608, 576, 1604, 572, 1608, 576, 1608, 572, 1608, 552, 1636, 576, 1604, 576, 1604, 576, 1604, 576, 1604, 572, 1608, 552, 1632, 572, 1608, 560, 540, 544, 1632, 576, 1608, 576, 520, 572, 520, 548, 548, 544, 552, 572, 1604, 516, 5256, 4432, 4352, 576, 524, 568, 1612, 576, 524, 544, 1636, 576, 1604, 572, 1608, 576, 1604, 576, 524, 548, 548, 572, 1608, 576, 1604, 548, 1636, 572, 1608, 572, 1608, 576, 520, 544, 1640, 576, 1608, 576, 524, 572, 520, 548, 1632, 576, 520, 572, 524, 568, 1608, 576, 524, 544, 552, 544, 548, 548, 548, 572, 520, 544, 552, 540, 552, 544, 548, 572, 524, 572, 524, 572, 524, 544, 548, 548, 548, 572, 524, 568, 524, 544, 548, 572, 524, 576, 1604, 576, 524, 544, 548, 576, 1604, 572, 1608, 548, 1632, 576, 1604, 572, 528, 508};
uint32_t  Deg76[199] PROGMEM = {4432, 4360, 572, 1608, 576, 520, 544, 1640, 544, 552, 544, 548, 548, 548, 544, 552, 544, 1636, 572, 1612, 576, 520, 548, 548, 544, 552, 544, 548, 548, 548, 572, 1612, 576, 520, 548, 548, 548, 1636, 572, 1608, 576, 520, 544, 1636, 576, 1604, 576, 1604, 576, 524, 576, 1604, 568, 1612, 576, 1604, 576, 1608, 572, 1608, 576, 1600, 552, 1632, 548, 1632, 580, 1608, 572, 1608, 580, 1600, 568, 1616, 576, 1604, 572, 1608, 548, 1632, 576, 1604, 560, 540, 548, 1632, 576, 1600, 580, 520, 544, 548, 544, 552, 544, 1636, 580, 1604, 512, 5268, 4404, 4384, 572, 524, 544, 1640, 548, 548, 544, 1636, 580, 1600, 552, 1632, 548, 1632, 548, 552, 544, 552, 544, 1636, 552, 1632, 576, 1604, 548, 1632, 552, 1632, 572, 524, 544, 1640, 548, 1640, 576, 520, 544, 552, 544, 1632, 580, 516, 544, 552, 544, 548, 544, 1640, 576, 524, 544, 552, 540, 556, 544, 548, 544, 552, 544, 548, 548, 548, 540, 556, 544, 552, 544, 548, 544, 552, 544, 548, 548, 548, 544, 552, 540, 552, 544, 552, 548, 1636, 576, 520, 544, 552, 544, 1632, 576, 1604, 580, 1604, 576, 520, 544, 556, 564};
uint32_t  Deg77[199] PROGMEM = {4368, 4436, 576, 1608, 572, 524, 572, 1612, 572, 524, 572, 524, 568, 524, 572, 524, 572, 1608, 576, 1612, 572, 524, 572, 524, 572, 524, 568, 524, 572, 524, 572, 1608, 576, 524, 572, 524, 572, 1612, 576, 1608, 572, 524, 572, 1608, 576, 1608, 572, 1608, 576, 1608, 572, 1616, 572, 1608, 576, 1604, 576, 1604, 576, 1608, 572, 1608, 572, 1608, 576, 1608, 576, 1612, 572, 1608, 576, 1604, 576, 1608, 576, 1604, 576, 1604, 576, 1608, 572, 1608, 576, 528, 568, 1608, 576, 1608, 572, 524, 572, 520, 572, 524, 572, 1612, 572, 524, 560, 5216, 4432, 4356, 576, 524, 568, 1612, 572, 524, 572, 1612, 572, 1608, 572, 1608, 576, 1608, 572, 524, 572, 524, 572, 1616, 568, 1608, 576, 1604, 576, 1608, 572, 1608, 576, 520, 572, 1612, 576, 1612, 576, 520, 572, 528, 568, 1608, 572, 524, 572, 524, 568, 524, 572, 524, 572, 528, 568, 524, 568, 528, 568, 524, 572, 524, 568, 524, 572, 524, 572, 524, 572, 524, 572, 524, 572, 520, 572, 524, 572, 524, 568, 524, 572, 524, 572, 524, 572, 1612, 576, 524, 568, 524, 572, 1608, 572, 1608, 572, 1608, 576, 524, 568, 1616, 560};
uint32_t  Deg78[199] PROGMEM = {4424, 4360, 572, 1608, 572, 524, 572, 1608, 576, 524, 568, 524, 572, 524, 572, 520, 572, 1608, 576, 1612, 576, 520, 572, 524, 572, 520, 572, 524, 572, 524, 568, 1612, 572, 528, 568, 528, 572, 1608, 576, 1608, 572, 1608, 572, 524, 568, 524, 572, 524, 572, 524, 572, 1612, 572, 1608, 576, 1604, 576, 1604, 576, 1608, 572, 1608, 576, 1604, 576, 1608, 572, 1612, 580, 1600, 576, 1608, 576, 1604, 576, 1604, 576, 1604, 576, 1608, 572, 1612, 572, 524, 572, 1608, 576, 1604, 576, 1604, 576, 524, 572, 1608, 576, 524, 568, 528, 564, 5204, 4432, 4352, 576, 524, 576, 1608, 576, 520, 572, 1612, 572, 1608, 576, 1604, 576, 1604, 576, 524, 572, 524, 572, 1612, 576, 1604, 576, 1608, 572, 1604, 580, 1604, 572, 524, 572, 1612, 576, 1612, 576, 524, 568, 524, 576, 520, 568, 1612, 576, 1608, 572, 1608, 576, 1608, 572, 524, 572, 524, 572, 524, 568, 524, 548, 548, 568, 524, 572, 524, 544, 552, 568, 528, 572, 524, 568, 524, 572, 524, 544, 552, 568, 524, 572, 524, 568, 528, 568, 1616, 572, 524, 572, 524, 568, 524, 572, 1612, 572, 524, 572, 1608, 576, 1612, 568};
uint32_t  Deg79[199] PROGMEM = {4416, 4388, 572, 1612, 544, 552, 540, 1640, 544, 556, 540, 552, 544, 552, 540, 552, 544, 1640, 568, 1616, 544, 556, 540, 552, 544, 552, 540, 556, 540, 552, 512, 1672, 544, 552, 544, 552, 544, 1640, 544, 1636, 544, 1636, 544, 552, 544, 552, 508, 584, 544, 1644, 544, 1640, 544, 1636, 548, 1636, 544, 1636, 544, 1636, 552, 1632, 544, 1636, 512, 1672, 544, 1644, 544, 1636, 544, 1636, 544, 1636, 548, 1636, 544, 1636, 544, 1636, 548, 1636, 544, 556, 540, 1640, 544, 1636, 544, 1640, 544, 552, 540, 1644, 544, 552, 540, 1644, 564, 5208, 4400, 4388, 544, 552, 544, 1636, 548, 552, 540, 1640, 544, 1640, 540, 1640, 544, 1636, 544, 556, 540, 556, 540, 1640, 544, 1636, 516, 1668, 544, 1636, 544, 1640, 544, 552, 540, 1644, 544, 1640, 544, 556, 540, 556, 540, 552, 540, 1640, 544, 1640, 540, 1640, 512, 588, 540, 556, 540, 552, 540, 556, 540, 556, 536, 556, 540, 556, 536, 556, 540, 556, 540, 556, 540, 556, 540, 552, 544, 552, 540, 556, 540, 552, 540, 556, 540, 556, 540, 1640, 544, 552, 540, 556, 540, 552, 544, 1640, 544, 552, 540, 1644, 536, 564, 560};
uint32_t  Deg80[199] PROGMEM = {4384, 4388, 544, 1636, 548, 552, 540, 1640, 544, 552, 540, 556, 540, 552, 540, 556, 540, 1640, 544, 1640, 544, 552, 540, 556, 540, 552, 540, 556, 540, 552, 540, 1640, 548, 552, 540, 556, 540, 1640, 544, 1640, 544, 1636, 544, 552, 540, 556, 540, 1636, 544, 556, 540, 1640, 544, 1636, 552, 1628, 544, 1636, 544, 1636, 572, 1608, 544, 1640, 540, 1640, 544, 1640, 548, 1636, 568, 1612, 544, 1636, 548, 1632, 572, 1612, 544, 1636, 544, 1636, 544, 556, 544, 1632, 548, 1636, 544, 1636, 544, 552, 540, 1640, 544, 1636, 544, 1640, 556, 5232, 4400, 4384, 544, 556, 540, 1640, 544, 552, 544, 1640, 544, 1636, 544, 1636, 544, 1636, 544, 556, 540, 556, 540, 1640, 548, 1632, 544, 1640, 544, 1632, 544, 1640, 568, 528, 540, 1640, 548, 1640, 544, 556, 536, 556, 540, 556, 540, 1640, 544, 1636, 544, 552, 544, 1640, 544, 556, 540, 552, 544, 552, 540, 556, 540, 552, 540, 556, 540, 552, 540, 556, 540, 556, 540, 556, 540, 552, 540, 556, 540, 552, 540, 556, 540, 552, 540, 556, 544, 1640, 544, 556, 536, 556, 540, 552, 544, 1640, 544, 552, 540, 556, 540, 556, 556};
uint32_t  Deg81[199] PROGMEM = {4400, 4392, 576, 1608, 576, 520, 544, 1636, 576, 524, 544, 548, 544, 552, 544, 548, 544, 1636, 576, 1612, 576, 520, 544, 552, 544, 548, 548, 548, 544, 552, 540, 1640, 576, 524, 544, 552, 540, 1640, 576, 1604, 580, 1600, 580, 520, 544, 548, 544, 1636, 548, 1632, 580, 1608, 548, 1632, 548, 1632, 548, 1632, 576, 1604, 552, 1628, 552, 1632, 548, 1632, 580, 1608, 548, 1632, 576, 1604, 576, 1604, 576, 1608, 576, 1604, 544, 1636, 576, 1604, 576, 524, 544, 1636, 576, 1604, 548, 1632, 576, 520, 544, 1640, 576, 1604, 572, 524, 512, 5268, 4404, 4380, 580, 520, 544, 1636, 576, 524, 540, 1640, 576, 1604, 576, 1604, 576, 1604, 548, 548, 544, 552, 544, 1640, 576, 1604, 576, 1604, 576, 1604, 576, 1604, 548, 548, 544, 1640, 576, 1612, 572, 524, 544, 548, 544, 552, 544, 1636, 576, 1604, 580, 520, 544, 552, 544, 552, 544, 548, 544, 552, 544, 552, 544, 548, 548, 548, 540, 552, 544, 552, 544, 552, 548, 548, 544, 548, 548, 548, 540, 552, 544, 552, 540, 552, 548, 552, 572, 1608, 580, 520, 544, 548, 544, 552, 572, 1608, 580, 520, 572, 520, 548, 1636, 512};
uint32_t  Deg82[199] PROGMEM = {4432, 4364, 572, 1608, 572, 524, 572, 1612, 572, 524, 568, 528, 568, 524, 568, 528, 540, 1640, 568, 1616, 572, 528, 540, 552, 544, 552, 544, 552, 540, 552, 544, 1640, 544, 556, 540, 552, 544, 1640, 544, 1636, 544, 1636, 548, 552, 540, 1640, 544, 556, 540, 556, 540, 1644, 564, 1616, 544, 1640, 544, 1636, 544, 1636, 544, 1636, 544, 1640, 544, 1636, 544, 1644, 544, 1636, 516, 1668, 544, 1636, 544, 1636, 544, 1640, 540, 1640, 540, 1644, 544, 556, 540, 1640, 540, 1640, 544, 1636, 544, 552, 540, 556, 540, 556, 540, 552, 560, 5212, 4396, 4388, 516, 584, 540, 1640, 544, 552, 544, 1640, 544, 1636, 544, 1636, 548, 1636, 544, 556, 540, 556, 540, 1640, 544, 1636, 544, 1640, 544, 1636, 544, 1636, 516, 584, 540, 1644, 544, 1640, 544, 556, 540, 556, 540, 552, 540, 1640, 544, 556, 540, 1640, 548, 1636, 544, 556, 544, 552, 540, 556, 536, 556, 540, 556, 540, 552, 540, 556, 540, 556, 540, 556, 540, 556, 540, 552, 540, 556, 540, 552, 540, 556, 540, 552, 540, 556, 540, 1644, 544, 556, 540, 552, 540, 556, 540, 1640, 548, 1632, 544, 1640, 540, 1644, 560};
uint32_t  Deg83[199] PROGMEM = {4388, 4388, 544, 1640, 540, 556, 540, 1640, 540, 560, 540, 552, 540, 556, 540, 556, 540, 1640, 544, 1644, 544, 552, 540, 552, 544, 552, 540, 556, 540, 552, 540, 1644, 544, 556, 540, 556, 540, 1644, 544, 1636, 544, 1636, 544, 552, 540, 1644, 544, 552, 540, 1644, 544, 1644, 540, 1640, 544, 1636, 544, 1636, 544, 1636, 544, 1640, 544, 1636, 544, 1640, 544, 1640, 548, 1636, 544, 1636, 544, 1636, 548, 1636, 544, 1636, 544, 1636, 544, 1640, 544, 556, 540, 1640, 540, 1640, 512, 1668, 544, 552, 544, 552, 544, 552, 540, 1640, 560, 5216, 4400, 4384, 544, 556, 540, 1640, 544, 556, 540, 1640, 544, 1636, 544, 1640, 544, 1636, 544, 552, 544, 556, 540, 1640, 544, 1636, 544, 1640, 540, 1640, 544, 1636, 544, 556, 540, 1644, 544, 1644, 540, 560, 536, 556, 540, 552, 544, 1640, 540, 556, 540, 1644, 544, 552, 544, 556, 540, 552, 540, 556, 540, 556, 540, 552, 540, 556, 540, 552, 540, 556, 544, 556, 540, 552, 540, 556, 540, 552, 540, 556, 540, 552, 544, 552, 540, 556, 540, 1640, 544, 556, 540, 552, 544, 552, 540, 1640, 544, 1640, 540, 1640, 544, 556, 560};
uint32_t  Deg84[199] PROGMEM = {4428, 4356, 576, 1608, 572, 524, 572, 1612, 572, 524, 572, 520, 572, 524, 568, 528, 568, 1608, 576, 1612, 572, 524, 568, 528, 568, 528, 568, 524, 568, 524, 572, 1608, 576, 524, 572, 524, 572, 1612, 572, 1608, 576, 1604, 576, 524, 572, 1608, 576, 1604, 576, 524, 572, 1608, 572, 1608, 576, 1604, 576, 1604, 576, 1608, 572, 1608, 576, 1604, 576, 1608, 572, 1616, 568, 1608, 576, 1608, 572, 1608, 572, 1608, 572, 1608, 572, 1608, 572, 1612, 572, 528, 568, 1608, 572, 1608, 576, 1604, 576, 524, 568, 524, 572, 1608, 572, 1608, 572, 5216, 4428, 4356, 572, 528, 568, 1612, 572, 524, 572, 1612, 572, 1608, 576, 1604, 572, 1612, 572, 528, 568, 528, 568, 1612, 576, 1608, 572, 1608, 572, 1608, 572, 1608, 572, 524, 572, 1612, 572, 1616, 572, 524, 568, 528, 572, 524, 568, 1612, 572, 524, 568, 528, 568, 1612, 572, 528, 568, 524, 568, 528, 544, 548, 568, 528, 512, 580, 568, 528, 568, 528, 572, 524, 572, 524, 568, 524, 572, 524, 568, 524, 572, 524, 568, 528, 568, 528, 568, 1612, 576, 524, 568, 524, 572, 524, 568, 1616, 572, 1604, 572, 528, 568, 528, 564};
uint32_t  Deg85[199] PROGMEM = {4376, 4428, 572, 1608, 572, 524, 572, 1612, 576, 520, 576, 516, 576, 520, 576, 520, 572, 1608, 576, 1612, 576, 520, 572, 520, 572, 524, 572, 524, 572, 520, 572, 1612, 576, 524, 568, 528, 572, 1612, 572, 1608, 572, 1608, 576, 520, 572, 1612, 576, 1604, 576, 1608, 576, 1612, 572, 1608, 576, 1604, 576, 1608, 572, 1608, 576, 1608, 572, 1608, 572, 1612, 572, 1616, 572, 1608, 572, 1608, 576, 1604, 576, 1608, 572, 1608, 576, 1604, 576, 1608, 572, 524, 572, 1608, 576, 1604, 576, 1608, 576, 520, 572, 520, 572, 1608, 576, 520, 516, 5268, 4428, 4356, 576, 520, 572, 1612, 576, 520, 572, 1612, 572, 1608, 576, 1604, 576, 1608, 568, 528, 572, 524, 572, 1612, 572, 1608, 576, 1604, 576, 1608, 572, 1608, 572, 524, 572, 1612, 576, 1612, 576, 520, 572, 524, 572, 520, 572, 1616, 572, 520, 572, 524, 572, 524, 572, 524, 576, 516, 576, 520, 572, 524, 572, 520, 572, 524, 572, 524, 572, 524, 572, 524, 572, 524, 572, 520, 576, 520, 568, 524, 572, 524, 572, 524, 572, 524, 572, 1608, 572, 524, 572, 520, 576, 520, 572, 1612, 572, 1608, 576, 520, 572, 1616, 516};

int indexArray = 12; // the index number for temperatures
int lastRecordedTemp = 0; // the last index number sent from user
String currentMode = ""; // last picked mode: cool, heat, fanonly, auto
String lastFanMode = ""; // the last fan speed

// create an array of temperatures
uint32_t * TempArray[][99] PROGMEM = {{0}, {Deg62}, {Deg63}, {Deg64}, {Deg65}, {Deg66}, {Deg67}, {Deg68}, {Deg69}, {Deg70}, {Deg71}, {Deg72}, {Deg73}, {Deg74}, {Deg75}, {Deg76}, {Deg77}, {Deg78}, {Deg79}, {Deg80}, {Deg81}, {Deg82}, {Deg83}, {Deg84}, {Deg85}};

uint32_t * newTemp = TempArray[indexArray - 1][99]; // index 12 -> 73°F

//uint32_t * lastSetPoint = newTemp; // now using lastSetPoint as String so if we uncomment this we need to rename this variable

//uint32_t newTempData = DataTempArray[indexArray - 1];

// set an array of corresponding int temps and data array
uint32_t IntTempArray[] = {0, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85};
int TempVal = IntTempArray[indexArray - 1];


// FAN AUTO COOL
uint32_t  FANAUTOC[99] PROGMEM = {4300, 4400, 550, 1600, 550, 550, 550, 1600, 550, 550, 550, 500, 600, 500, 550, 550, 550, 1600, 550, 1600, 600, 500, 550, 1600, 550, 550, 550, 500, 600, 500, 550, 550, 550, 500, 600, 500, 600, 1550, 600, 1600, 550, 500, 600, 1600, 550, 1600, 550, 1600, 550, 550, 550, 1600, 550, 1600, 550, 1600, 600, 1550, 600, 1550, 600, 1600, 550, 1600, 550, 1600, 550, 1600, 600, 1600, 550, 1600, 550, 1600, 550, 1600, 600, 1550, 600, 1550, 600, 1600, 550, 500, 600, 1600, 550, 500, 600, 500, 550, 550, 550, 500, 600, 500, 550, 550, 500};

// FAN AUTO HEAT
uint32_t  FANAUTOH[99] PROGMEM = {4350, 4350, 600, 1550, 600, 500, 550, 1600, 600, 500, 550, 550, 550, 500, 600, 500, 600, 1550, 600, 1600, 550, 500, 600, 1600, 550, 500, 600, 500, 550, 550, 550, 1600, 550, 1600, 600, 500, 550, 1600, 550, 1600, 600, 500, 550, 1600, 600, 1550, 600, 1550, 600, 500, 600, 1550, 600, 1600, 550, 1600, 550, 1600, 550, 1600, 600, 1550, 600, 1600, 550, 1600, 550, 1600, 550, 1600, 600, 1550, 600, 1600, 550, 1600, 550, 1600, 550, 1600, 600, 1550, 600, 500, 550, 1600, 600, 500, 550, 550, 550, 500, 600, 500, 550, 1600, 550, 550, 500};



// FAN HIGH HEAT LIST
/*uint32_t * FANHIGHH[][99] PROGMEM = {{0}, {FHH62}, {FHH63}, {FHH64}, {FHH65}, {FHH66},
  {FHH67}, {FHH68}, {FHH69}, {FHH70}, {FHH71}, {FHH72}, {FHH73}, {FHH74},
  {FHH75}, {FHH76}, {FHH77}, {FHH78}, {FHH79}, {FHH80}
  };


  // FAN MED HEAT LIST
  uint32_t * FANMEDH[][99] PROGMEM = {{0}, {FMH62}, {FMH63}, {FMH64}, {FMH65}, {FMH66},
  {FMH67}, {FMH68}, {FMH69}, {FMH70}, {FMH71}, {FMH72}, {FMH73}, {FMH74},
  {FMH75}, {FMH76}, {FMH77}, {FMH78}, {FMH79}, {FMH80}
  };


  // FAN LOW HEAT LIST
  uint32_t * FANLOWH[][99] PROGMEM = {{0}, {FLH62}, {FLH63}, {FLH64}, {FLH65}, {FLH66},
  {FLH67}, {FLH68}, {FLH69}, {FLH70}, {FLH71}, {FLH72}, {FLH73}, {FLH74},
  {FLH75}, {FLH76}, {FLH77}, {FLH78}, {FLH79}, {FLH80}
  };*/


uint32_t * FanSpeed = {0};//FANHIGHH[indexArray - 1][99] PROGMEM; // default to FAN HIGH HEAT 73 // TO DO: fan heat auto 76



/////////// NOT IMPLEMENTED YET! All values need to be updated in all FAN COOL sub-ARRAYS
// FAN HIGH COOL LIST
/*uint32_t * FANHIGHC[][99] PROGMEM = {{0}, {FHC62}, {FHC63}, {FHC64},
  {FHC65}, {FHC66}, {FHC67}, {FHC68}, {FHC69}, {FHC70},
  {FHC71}, {FHC72}, {FHC73}, {FHC74}, {FHC75}, {FHC76},
  {FHC77}, {FHC78}, {FHC79}, {FHC80}
  };


  // FAN MED COOL LIST
  uint32_t * FANMEDC[][99] PROGMEM = {{0},{FMC62}, {FMC63}, {FMC64}, {FMC65}, {FMC66},
  {FMC67}, {FMC68}, {FMC69}, {FMC70}, {FMC71}, {FMC72}, {FMC73}, {FMC74},
  {FMC75}, {FMC76}, {FMC77}, {FMC78}, {FMC79}, {FMC80}
  };

  // FAN LOW COOL LIST
  uint32_t * FANLOWC[][99] PROGMEM = {{0}, {FLC62}, {FLC63}, {FLC64}, {FLC65}, {FLC66},
  {FLC67}, {FLC68}, {FLC69}, {FLC70}, {FLC71}, {FLC72}, {FLC73}, {FLC74},
  {FLC75}, {FLC76}, {FLC77}, {FLC78}, {FLC79}, {FLC80}
  };

*/