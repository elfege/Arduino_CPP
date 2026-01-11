/*
 * https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
 */

#define LED                 D7 // D4 is GPIO2 built-in led but used by boot so we use D7(GPIO13)

 // sensor input
#define touch_sensor        A0
#define contact_sensor      D6

// motor control // must be VARIABLES, not constants
int pwm = D5; // speed modulation 
int MA = D1; // motor
int MB = D2; // motor
