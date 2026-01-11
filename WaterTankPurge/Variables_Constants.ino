const int DELAY_SEC = 120; //in seconds 
unsigned long previousMillis = 0; //needed for new timing mechanism
unsigned long previousMillisBlink = 0;
unsigned long previousMillisPURGE = 0;
unsigned long previousMillisPURGEduration = 0;


int seconds = 1000;
unsigned long hour = (unsigned long int)3600 * (unsigned long int)seconds;
int desiredTime = 6;
unsigned long TimeLimit = (unsigned long int)desiredTime * (unsigned long int)hour;
int elapsed = 0 ;// variable redefined below with time()


bool cancel = false; // allows to stop pumping operation
bool Running = false;
bool isDebugEnabled;    // enable or disable debug 

int purging;

// to differentiate blinking modes while stby and running

int occ = 1; 
int lapse = 200; 
int DELAY = 500; 
