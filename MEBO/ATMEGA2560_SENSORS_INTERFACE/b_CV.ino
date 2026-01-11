String response; // responses sent through uart to ESP32


int distanceUP;
int distanceFR;
int distanceFL;

unsigned long Start;
unsigned long logsMillis = millis();
unsigned long logsStartedMillis = millis();
unsigned long setupTime = millis();
unsigned long wtdResponseMillis = millis();
unsigned long lastWtdCall = millis();
boolean stoparm = false;
boolean logs = false;
