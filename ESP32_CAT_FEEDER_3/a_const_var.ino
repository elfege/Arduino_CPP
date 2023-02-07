/**************************** CONSTANTS *****************************/
// Setting PWM properties
const int freq = 30000;
const int pwmChannelActuator = 4; // leave 1, 2 and 3 to servo lib
const int pwmChannelCanOpener = 5;
const int pwmChannelCos = 6;
const int resolution = 8;
const int maxSpeedOpener = 200; //absolute max (6v motor!)


/**************************** VARIABLES *****************************/
bool stopAll = false;
bool OTArequest = false;
bool wifiLostAware = false;
bool Logs = false;
bool pushing = false;
bool pulling = false;
bool smallisout = false;  // updated at setup()
bool masterisout = true;// precaution
bool stopped = true;
bool extracting = false;
bool feeding = false;
bool loading = false;
bool compressing = false;
bool goingdown = false;
bool poppinglid = false;
bool scriptactive = true;
bool checkCompressionReady = true;
bool adjusting = false;
bool canopening = false;
bool gotimpact = false;
bool overrideImpact = false;
bool cosinuspushing = false;
bool cosinuspulling = false;
bool recording = false;
bool deformOverride = false;
bool problem = false;
bool deformation = false;
bool spin = false; // for openCan() only
bool hasSpun = false; // for after loading the can
bool refreshrequest = false;
bool paused = false;
bool waspaused = false;
bool canDetectedByLimitSwitchDuringLoad = false;


unsigned long previousMillisBlink = millis();
unsigned long WiFiLostMillis = millis();
unsigned long millisLogs = millis();
unsigned long lastActionMillis = millis();
unsigned long cosinusMillis = 0;
unsigned long lastRefresh = 0;
unsigned long extractionStart = 0;
unsigned long heartbeat = 0;
unsigned long problemMillis = 0;

unsigned long pushTime = 240000;

int pos = 0;
int minUs = 500;
int maxUs = 2500;

Servo SMALL_PUSHER;
ESP32PWM pwm;

int deformCorrAttempts = 0;


int cosMax = 3000; // in ms
int speedValCosinus = 255;
int speedValOpener = 184; 



#define IRval 2000;
