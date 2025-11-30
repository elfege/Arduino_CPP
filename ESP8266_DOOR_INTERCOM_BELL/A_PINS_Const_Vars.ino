const char *NameOTA = "INTERCOM";

//*************************************************************************************************
// NodeMCU v1.0 ESP8266-12e Pin Definitions
// These comments show the mapping between "D" labels on the board and actual GPIO numbers.
// The Arduino ESP8266 core already defines these, so we don't need to redefine them.
//*************************************************************************************************
// #define D0 16  // no internal pullup resistor
// #define D1  5
// #define D2  4  // builtin led on some boards
// #define D3  0  // BOOT CRITICAL: must not be pulled LOW during power on/reset
// #define D4  2  // BOOT CRITICAL: must not be pulled LOW during power on/reset (also onboard LED)
// #define D5 14
// #define D6 12
// #define D7 13
// #define D8 15  // BOOT CRITICAL: must not be pulled HIGH during power on/reset

//*****************************************************************************
// HARDWARE PIN ASSIGNMENTS
// These define which physical pins control the intercom relays
//*****************************************************************************
#define TALK D5       // Output: TALK relay - simulates pressing the "answer" button
#define DOOR D7       // Output: DOOR relay - simulates pressing the "unlock" button
#define LED D4        // Output: Onboard LED for status indication
#define sensorPin A0  // Input: Analog voltage sensor detecting intercom LCD activation

//*****************************************************************************
// RELAY ACTIVE STATES
// The relays are "active low" - meaning they activate when the pin is LOW (0V)
// PRESS = LOW = relay activated = button pressed
// RELEASE = HIGH = relay deactivated = button released
//*****************************************************************************
const int PRESS = 0;    // LOW signal activates the relay (presses the button)
const int RELEASE = 1;  // HIGH signal deactivates the relay (releases the button)

//*****************************************************************************
// SIGNAL DETECTION SETTINGS
//*****************************************************************************
int sensorValue = 0;   // Current analog reading from voltage sensor (0-1023)
int threshold = 200;   // Signal threshold: values >= this mean "bell is ringing"
                       // The intercom LCD powers on when someone rings, raising voltage

//*****************************************************************************
// STATE MACHINE
//*****************************************************************************
// 
// WHY USE A STATE MACHINE?
// ------------------------
// Previously, the code used multiple boolean flags:
//   - got_signal
//   - switch_door_operation_is_running  
//   - runsim
//   - cmdFromHttp
//
// PROBLEM: These flags can get out of sync. For example:
//   1. got_signal becomes true (bell detected)
//   2. Operation starts, switch_door_operation_is_running = true
//   3. During wait(), an HTTP request comes in for /simulator
//   4. That sets runsim = true
//   5. Now we have conflicting states - are we handling a real bell or a test?
//
// SOLUTION: Use ONE variable that can only be in ONE state at a time.
// This is called a "state machine" - the system is always in exactly one
// defined state, and transitions between states are explicit and controlled.
//
// Think of it like a traffic light: it's either RED, YELLOW, or GREEN.
// It can't be RED and GREEN at the same time. Same principle here.
//
//*****************************************************************************


// This is THE master state variable. All logic checks this instead of
// multiple booleans. Initialized to IDLE (monitoring mode).
SystemState currentState = STATE_IDLE;

// Helper function to convert state to readable string (for debugging)
// This is defined here as a declaration, implemented in X_Functions.ino
String stateToString(SystemState state);

//*****************************************************************************
// TIMING VARIABLES
// All use millis() for non-blocking time tracking
//*****************************************************************************
unsigned long lastMillisBlink = 0;    // LED blink timing
unsigned long lastRefresh = 0;        // Hub state refresh timing
unsigned long WiFiLostMillis = 0;     // WiFi reconnection timing
unsigned long lastSignalLog = 0;      // Serial/term logging throttle
unsigned long operationStartTime = 0; // When current operation began (for timeout protection)

//*****************************************************************************
// CONFIGURATION
//*****************************************************************************
const unsigned long OPERATION_TIMEOUT_MS = 30000;  // Max time for any operation (safety limit)
const unsigned long REFRESH_INTERVAL_MS = 300000;  // Hub refresh interval (5 minutes)
const unsigned long SIGNAL_LOG_INTERVAL_MS = 1000; // How often to log signal value

//*****************************************************************************
// SYSTEM FLAGS
// These are "mode" flags that modify behavior but don't represent operational state
//*****************************************************************************
bool wifiLostAware = false;  // True when we've detected WiFi loss and are waiting to reconnect
bool StopAll = false;        // True during OTA update - stops all normal operations
bool debug = false;          // True enables verbose debug output

//*****************************************************************************
// TERMINATION LOOP COUNTER
// Used by terminate() to limit retry attempts when hanging up the call
//*****************************************************************************
int terminateAttempts = 0;           // Current attempt count
const int MAX_TERMINATE_ATTEMPTS = 5; // Give up after this many tries

//*****************************************************************************
// REMOVED VARIABLES (for reference - these are now handled by state machine)
//*****************************************************************************
// bool got_signal = false;                    // REPLACED BY: currentState == STATE_SIGNAL_DETECTED
// bool switch_door_operation_is_running = false; // REPLACED BY: currentState == STATE_OPERATING
// bool runsim = false;                        // REPLACED BY: currentState == STATE_TEST_REQUESTED
// bool cmdFromHttp = false;                   // REMOVED: not needed with state machine
// unsigned long cmdFromHttpMillis = millis(); // REMOVED: not needed
// bool refreshrequest = false;                // REMOVED: not needed
// bool sent = false;                          // REMOVED: send_event is disabled anyway
// String lastMessage;                         // REMOVED: not used
// int lastloop = 5;                           // REPLACED BY: terminateAttempts
// int Value = HIGH;                           // REMOVED: unused
// int frequency = 60000;                      // REMOVED: unused (PWM frequency)
// unsigned long lastSignalDebug = millis();   // REMOVED: duplicate of lastSignalLog
// unsigned long lastRead = millis();          // REMOVED: unused
