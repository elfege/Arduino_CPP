void setup()
{
  // create OTA name
  const char *NameOTA = "AC_REMOTE";
  ArduinoOTA.setHostname(NameOTA);

  // setup default state of global variables
  isDebugEnabled = true;

  if (isDebugEnabled)
  { // setup debug serial port
    Serial.begin(115200);         // setup serial with a baud rate of 9600
    Serial.println("");
    Serial.println("setup..");  // print out 'setup..' on start
  }

  // setup hardware pins

  Serial.begin(115200);
  irsend.begin();


  pinMode(Signal, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(Asensor, INPUT);


  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();
  digitalWrite(LED, LOW); // turn on the LED

  //Serial.println("STARTING WEB SERVER");
  server.begin();

  // set default temp
  Serial.println("");



  // update smartthings hub
  
  acOFF();
  Refresh();
  Blink(10, 50);
}


