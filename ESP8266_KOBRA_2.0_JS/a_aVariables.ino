boolean goUp;
boolean isDebugEnabled = true;    // enable or disable debug in this example
boolean StopAll = false;
boolean logs = false;
boolean camIsOn;

int glw;
int elapsed = 0;

unsigned long DELAY_SEC = 120; // 2 minutes without charging if not requested
unsigned long DELAY_24   = 1000 * 60 * 60 * 24; // power cycle the charger every 24 hours if no activity
unsigned long previousMillis = 0;
unsigned long previousMillisBlink = 0;
unsigned long previousMillisCharge = 0;
unsigned long previousMillisChargeCycle = 0;

String debugData = "";
