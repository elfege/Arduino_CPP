unsigned long millipreviousMillisBlink = millis(); 
unsigned long heartbeat = millis();

unsigned long lastStateMillis = millis();
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

#define TEMP_SENSOR_PIN 34  // Using GPIO 34

float temperature = 0.0;
float lastReportedTemp = 0.0;
unsigned long lastTempCheck = 0;
const unsigned long tempCheckInterval = 60000;  // Check temperature every 60 seconds

// Thermistor characteristics
const float THERMISTOR_B = 3950;
const float THERMISTOR_T0 = 298.15;
const float THERMISTOR_R0 = 10000;
const float SERIES_RESISTOR = 22000;

const float TEMP_THRESHOLD = 1.0;  // 1°F threshold for reporting
const float TEMP_OFFSET = -8.5;  // Offset to calibrate temperature (adjust this value)