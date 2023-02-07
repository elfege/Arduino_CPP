int const LED = D8; //   (GPIO9)S2
int const Signal =  D5; // (IR LED OUTPUT)
int const Asensor = A0;

#define rst       D6 // to reset pin with 220ohm resistor

IRsend irsend(Signal);  // Set the GPIO to be used for sending the message.


//******************************************************************************************
//NodeMCU ESP8266 Pin Definitions (makes it much easier as these match the board markings)
//******************************************************************************************

//#define D0 16 //no internal pullup resistor
//#define D1  5
//#define D2  4
//#define D3  0 //must not be pulled low during power on/reset, toggles value during boot // must not be used as input
//#define D4  2 //must not be pulled low during power on/reset, toggles value during boot  // must not be used as input
//#define D5 14
//#define D6 12
//#define D7 13
//#define D8 15 //must not be pulled high during power on/reset //PIO15 is always pulled low, so you can’t use the internal pull-up resistor
//#define D9  3
//#define D10 1
