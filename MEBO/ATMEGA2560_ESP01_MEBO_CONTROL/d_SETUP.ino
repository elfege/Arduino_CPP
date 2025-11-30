
IRrecv irrecv(RECV_PIN);
decode_results results;



void setup()
{

  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, LOW);

  pinMode(RELAY2, OUTPUT); // priority to this relay in case loss of power to resume charge
  digitalWrite(RELAY2, 0);// which means that it is by default ON upon reset



  //******************************************************************************************
  // Setup the default values for the ADC.  Used for analog voltage reads.
  //  Notes:  analogReadResolution(12) sets the resolution for all pins. 12 = 0-4095, 11 = 0-2047, 10 = 0-1024, 9 = 0-512
  //          analogSetAttenuation(ADC_11db) sets the attenuation for all pins.  11db = 0-3.3v, 6dB range = 0-2.2v, 2.5db = 0-1.5v, 0db = 0-1v
  //          analogSetPinAttenuation(A7, ADC_11db) sets the attenuation for individual pins.
  //******************************************************************************************


  //*****************************************************************************
  //  Configure debug print output from each main class
  //  -Note: Set these to "false" if using Hardware Serial on pins 0 & 1
  //         to prevent communication conflicts with the ST Shield communications
  //*****************************************************************************
  st::Everything::debug = true;
  //st::Executor::debug = true;
  //  st::Device::debug = true;
  //  st::PollingSensor::debug = true;
  //  st::InterruptSensor::debug = true;

  //*****************************************************************************
  //Initialize the "Everything" Class
  //*****************************************************************************

  //Initialize the optional local callback routine (safe to comment out if not desired)
  //st::Everything::callOnMsgSend = callback;

  // call out function forward decalaration
  SmartThingsCallout_t messageCallout;

  //Create the SmartThings ESP32WiFi Communications Object
  //STATIC IP Assignment - Recommended
  bool enableDebug = true;
  st::Everything::SmartThing = new st::SmartThingsWiFiEsp(&Serial1, str_ssid, str_password, ip, serverPort, hubIp, hubPort, messageCallout);// st::receiveSmartString);


  //Initialize the Serial1 baudrate to match your ESP-01's baud rate (e.g. 9600, 57600, 115200)
  Serial1.begin(115200);

  // initialize serial for second ESP module
  Serial2.begin(115200);
  
 


  //Run the Everything class' init() routine which establishes WiFi communications with SmartThings Hub
  st::Everything::init();

  //*****************************************************************************
  //END OF I"Everything" Class initialization
  //*****************************************************************************

//  server.begin();

  // set the pins outputs
  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1, 0);
  pinMode(PW, INPUT); /// charging power measurment

  pinMode(PWM_L, OUTPUT); ////
  pinMode(PWM_LEFT1, OUTPUT);
  pinMode(PWM_LEFT2, OUTPUT);
  pinMode(PWM_R, OUTPUT);
  pinMode(PWM_RIGHT1, OUTPUT);
  pinMode(PWM_RIGHT2, OUTPUT);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(FIR, INPUT);      // front IR input
  pinMode(bumberPin, INPUT); // front bumper input
  pinMode(RRIR, INPUT); // Rear IR distance sensor as input
  pinMode(RLIR, INPUT); // Rear proximity sensor as input


  pinMode(LIR, INPUT); // left IR sensor
  pinMode(RIR, INPUT); // right IR sensor

  // color and cliff detectors
  pinMode(LineLeft, INPUT);
  pinMode(LineRight, INPUT);

  /// configure encoder pins
  pinMode(25, INPUT);

  //// ENCODERS

  pinMode(RIGHT, INPUT);
  pinMode(LEFT, INPUT);
  //  attachInterrupt(digitalPinToInterrupt(LEFT), counterL, FALLING);
  //  attachInterrupt(digitalPinToInterrupt(RIGHT), counterR, FALLING);


  //irrecv.enableIRIn(); // Start the receiver ---------------------------------------------------------------------------------------------------------------------



  Serial.println("READY.. ");
}






