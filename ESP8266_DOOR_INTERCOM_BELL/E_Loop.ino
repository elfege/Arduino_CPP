void loop() {

  master();
}


void master() {
  if (!StopAll) {
    ledBlink();
    httpHandler();
    WiFiHandler();
    if(!cmdFromHttp) 
    {
      mainOp();
    }
    else if(millis() - cmdFromHttpMillis > 10000 && cmdFromHttp)
    {
      term.println("cmdFromHttp RESET");
      cmdFromHttp = false;
    }
  } else {
    ArduinoOTA.handle();
  }
  yield();
  delay(10);
}

void mainOp() {
  sensorValue = analogRead(A0);
  delay(10);  //absolutely required! too many analogRead()'s crashes the wifi
  
  voltage = runsim ? Val + 1 : sensorValue * (Multiplier / 2048.0);

   if (voltage >= Val && runpower == 0)  // bell's ringing
  {
    smartthing.send("acceleration active");
    wait(500);
    if(!runsim && !runpower) PowerOn();    // !runpower prevents operation from runing over and over while voltage returns positive value
    runsim = false;
  } else {
    PowerOff();
  }

  if (/*(*/debug /*|| voltage >= Val)*/ && millis() - lastSignalDebug >= 1000) { //seems to be delaying or bugging or something, the event declaration
    term.println("Signal is: " + String(sensorValue));
    term.print("Voltage is:");
    term.println(voltage);
    term.println("THRESHOLD = "+String(Val));
    lastSignalDebug = millis();
  }


 

  // Making sure com is terminated
  //close(); //conditioned with globals: float voltage, int lastloop and bool runpower /* WAITING TO IMPLEMENT THIS CALL NO TIME NOW TO TEST IT */
  while (voltage >= Val && lastloop <= 4 && runpower != 0) {

    digitalWrite(TALK, PRESS);  // press talk button to terminate communication
    wait(200);
    digitalWrite(TALK, RELEASE);  // release talk button
    wait(200);
    lastloop++;
    httpHandler();
  }

}