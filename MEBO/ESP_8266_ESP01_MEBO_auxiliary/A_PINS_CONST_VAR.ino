/* Setting GPIO3 (RX) as an OUTPUT is NOT recommended if you can avoid it because
  it is easy to short out when next re-programming. Note carefully: If you are using
  GPIO3 (RX) as an output, add a small resistor say 330R in series to prevent accidentally
  shorting it out when you attach your programming leads.*/

/*
  The GPIO0 and GPIO2 determine what mode the module starts up in and the TX/RX pins
  are used to program the module and for Serial I/O, commonly used for debugging.
*/


//GPIO1 (TX) and GPIO3 (RX) Serial
//GPIO2 (TX2) Serial1

#define POWER 0
#define POWER_B 2
// #define POWER_C 3

// DON'T USE PIN 2 => used by Serial1

#define LED 1 // GPIO1 is also used for TX and, therefore, for debug. So all Serial outputs are conditional


const char *NameOTA = "MEBO_AUXILIARY_POWER";

unsigned long previousMillisBlink = 0;
unsigned long Start = 0;

bool wifiLostAware = false;
bool StopAll = false;
bool debug = false;
bool refreshrequest = false;
bool serialStarted = false;


String lastMessage;

unsigned long lastMillisBlink = millis();
unsigned long lastRefresh = millis();
unsigned long WiFiLostMillis = millis();
unsigned long lastSignalDebug = millis();
unsigned long cmdFromHttpMillis = millis();