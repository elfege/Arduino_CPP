//digital Pins

#define trigPinUPL           30   // ultrasonic trigger UP 
#define echoPinUPL           31   // ultrasonic echo UP
#define trigPinUPR           41   // ultrasonic trigger UP 
#define echoPinUPR           40   // ultrasonic echo UP


#define trigPinFR           47   // ultrasonic trigger Front RIGHT 
#define echoPinFR           46   // ultrasonic echo Front RIGHT

#define trigPinFL           45   // ultrasonic trigger Front LEFT 
#define echoPinFL           44   // ultrasonic echo Front LEFT

#define BL         22  // Bumper pin LEFT
#define BR         23  // Bumper pin RIGHT


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

// ARMS SENSORS
#define ForeUpS            13
#define ForeDownS          12
#define ArmDownS           11
#define ArmUpS             10
#define ClampOpenS          9
#define ClampClosedS        7
#define Wrist_L_EndS           6
#define Wrist_R_EndS           5
#define Wrist_Middle           4
#define Arm_Center             3


// ANALOG PORTS
#define PW                A10  // charging port input power detection
#define FLIR              A0  // front left IR sensor
#define FRIR              A4  // front right IR sensor
#define RLIR              A6  // IR distanceUP sensor rear left side
#define RRIR              A5  // IR distanceUP sensor rear right side


#define RIR              A7 // Right IR sensor  (GPIO 33)
#define LIR              A1 // Left IR sensor  (GPIO 36 aka SENSOR_VP port)
