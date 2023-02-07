//digital Pins
#define RELAY1            6 // power relay that turns on Mebo's motherboard, cam and new motor controller
#define RELAY2            28 // charging power relay


#define trigPinUPL           40   // ultrasonic trigger UP 
#define echoPinUPL           41   // ultrasonic echo UP
#define trigPinUPR           43   // ultrasonic trigger UP 
#define echoPinUPR           42   // ultrasonic echo UP


#define trigPinFR           47   // ultrasonic trigger Front RIGHT 
#define echoPinFR           46   // ultrasonic echo Front RIGHT

#define trigPinFL           45   // ultrasonic trigger Front LEFT 
#define echoPinFL           44   // ultrasonic echo Front LEFT

#define BL         22  // Bumper pin LEFT
#define BR         23  // Bumper pin RIGHT


// ANALOG PORTS
#define PW                A15  // charging port input power detection
#define FIR               A0  // front IR sensor
#define RLIR              A6  // IR distanceUP sensor rear left side
#define RRIR              A5  // IR distanceUP sensor rear right side


#define RIR              A7 // Right IR sensor  (GPIO 33)
#define LIR              A1 // Left IR sensor  (GPIO 36 aka SENSOR_VP port)


/// ARMS MOTORS
#define ForeArm1         33
#define ForeArm2         35
#define Clamp1           37
#define Clamp2           39

#define Arm1             32
#define Arm2             34
#define Wrist1           36
#define Wrist2           38

#define RST               8 // to NP2222a then to reset port
