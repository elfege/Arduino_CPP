void setup()
{
  // create OTA name
  const char *NameOTA = "CatsWaterTank";
  ArduinoOTA.setHostname(NameOTA);

  // setup default state of global variables
  isDebugEnabled = true;

  if (isDebugEnabled)
  { // setup debug serial port
    Serial.begin(9600);         // setup serial with a baud rate of 9600
    Serial.println("");
    Serial.println("setup..");  // print out 'setup..' on start
  }

  // setup hardware pins

  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(Signal, OUTPUT);
  digitalWrite(Signal, 0);
 
  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();
  digitalWrite(LED, LOW); // turn on the LED

  Serial.println("STARTING WEB SERVER");
  server.begin();

  // update smartthings hub
  Refresh();
  Blink(10, 50);
  
}
