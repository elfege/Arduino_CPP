boolean stopped = false;
boolean closing = false; 
boolean opening = false;
boolean modifRequested= false;

const int DELAY_SEC = 120; //in seconds (to facilitate debugging!)
unsigned long previousMillis = 0; //needed for new timing mechanism
unsigned long previousMillisBlink = 0;
 
bool isDebugEnabled;    // enable or disable debug in this example



/// DC MOTOR SETTINGS
int const ULTRASLOW = 160;
int const SLOW = 405;
int const MEDIUM = 450;
int const FAST = 480;
int SpeedVal = SLOW; // default speed //  this is also a global val to be set upon request from ST

char test_this_char[] = "";
String  test_thisMessage; // just for debug over web page


// Setting PWM properties

int Default = FAST;
int dutyCycle = 0; // value at startup

// 1/0 values for motors
int A = 0;
int B = 1;


// Setting PWM properties
const int freq = 25000;
const int pwmChannel = 0;
const int resolution = 8;

