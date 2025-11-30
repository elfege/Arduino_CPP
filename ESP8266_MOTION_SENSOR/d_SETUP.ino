void setup()
{
  pinMode(rst, OUTPUT);
  digitalWrite(rst, 1);

  // create OTA name
  const char *NameOTA = "Motion_SensorHALLWAY";
  ArduinoOTA.setHostname(NameOTA);

  // setup default state of global variables
  isDebugEnabled = true;

  // setup hardware pins

  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  pinMode (ASensor, INPUT);


  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();
  digitalWrite(LED, LOW); // turn on the LED

  // Start the server
  server.begin();
  Serial.println("Server started");

  //Refresh();
  Blink(20, 30);

  smartthing.send("REBOOT SUCCESSFUL");
}
