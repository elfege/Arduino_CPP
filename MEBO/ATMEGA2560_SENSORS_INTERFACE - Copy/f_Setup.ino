void setup() {

  Serial.begin(115200); // debug serial port

  //Initialize the Serial2 baudrate to match your ESP-01's baud rate (e.g. 9600, 57600, 115200)
  Serial1.begin(115200);

  // initialize Serial2 for second ESP module
  Serial2.begin(115200);




  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);
  pinMode(trigPinUPL, OUTPUT);         //  ultrasonic trigger
  pinMode(echoPinUPL, INPUT);          //  ultrasonic echo
  pinMode(trigPinUPR, OUTPUT);         //  ultrasonic trigger
  pinMode(echoPinUPR, INPUT);          //  ultrasonic echo
  pinMode(trigPinFL, OUTPUT);         //  ultrasonic trigger
  pinMode(echoPinFL, INPUT);          //  ultrasonic echo
  pinMode(trigPinFR, OUTPUT);         //  ultrasonic trigger
  pinMode(echoPinFR, INPUT);          //  ultrasonic echo

  pinMode(BL, INPUT_PULLUP);        // Bumper pin LEFT
  pinMode(BR, INPUT_PULLUP);        // Bumper pin RIGHT

  // ARM SENSORS
  pinMode(ForeDownS, INPUT_PULLUP); // digital IR proximity sensor
  pinMode(ForeUpS, INPUT_PULLUP); // switch sensor reversed logic
  pinMode(ArmDownS, INPUT_PULLUP); // switch sensor reversed logic
  pinMode(ArmUpS, INPUT_PULLUP); // switch sensor reversed logic
  pinMode(ClampOpenS, INPUT_PULLUP); // switch sensor reversed logic
  pinMode(ClampClosedS, INPUT_PULLUP); // switch sensor reversed logic
  pinMode(Wrist_L_EndS, INPUT_PULLUP); // switch sensor reversed logic
  pinMode(Wrist_R_EndS, INPUT_PULLUP); // switch sensor reversed logic
  pinMode(Wrist_Middle, INPUT_PULLUP); // switch sensor reversed logic
  pinMode(Arm_Center, INPUT_PULLUP); // switch sensor reversed logic


  // ANALOG PORTS
  pinMode(PW, INPUT);               // charging port input power detection
  pinMode(FIR, INPUT);              // front IR sensor
  pinMode(RLIR, INPUT);              // IR distanceUP sensor rear left side
  pinMode(RRIR, INPUT);              // IR distanceUP sensor rear right side


  pinMode(RIR, INPUT);
  pinMode(LIR, INPUT);






  //*****************************************************************************
  //  Configure debug print output from each main class
  //  -Note: Set these to "false" if using Hardware Serial on pins 0 & 1
  //         to prevent communication conflicts with the ST Shield communications
  //*****************************************************************************
  //st::Everything::debug = true;
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
  //SmartThingsCallout_t messageCallout;

  //Create the SmartThings ESP32WiFi Communications Object
  //STATIC IP Assignment - Recommended
  //bool enableDebug = true;
  //st::Everything::SmartThing = new st::SmartThingsWiFiEsp(&Serial1, str_ssid, str_password, ip, serverPort, hubIp, hubPort, messageCallout);// st::receiveSmartString);


  //Initialize the Serial1 baudrate to match your ESP-01's baud rate (e.g. 9600, 57600, 115200)
  Serial1.begin(115200);

  // initialize serial for second ESP module
  Serial2.begin(115200);




  //Run the Everything class' init() routine which establishes WiFi communications with SmartThings Hub
  //st::Everything::init();

  //*****************************************************************************
  //END OF I"Everything" Class initialization
  //*****************************************************************************


  delay(100);
  Serial.println("READY..");
}
