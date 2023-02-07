#define LED         D3
#define CHARGERin   D1 // input reads value from ESP32
#define CHARGER     D7 // charger switch, when ESP32 is on it correlates its pin's state
#define CAM         D8 // 4 seconds off, 1 second on, cam needs to be fully on to be turned off. Remebmer: 4v through from cam
#define rst         D0  // D0 (has to be D0 because has no pullup resistor) pin to built-in reset pin with 470ohm resistor
#define power       D5
#define ESP32rst    D6 // port to reset ESP32 after power on LOW = reset connected to ESP32 EN port
