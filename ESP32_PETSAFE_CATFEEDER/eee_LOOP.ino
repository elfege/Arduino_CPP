void loop() {
  loopTime = millis();
  digitalWrite(watchDogPin, 0);
  wtdpin = digitalRead(watchDogPin);

  if (!stopAll) 
  {
  master();
  }
  ArduinoOTA.handle();

  delay(10);
  digitalWrite(watchDogPin, 1);
  delay(50);// give time to core0 to notice the change of state
  wtdpin = digitalRead(watchDogPin);
  elapsed = millis() - loopTime;
//  Serial.println("loop duration = " + String(elapsed));
}

void master()
{
  
  _server.handleClient();
  st::Everything::run();

  
    if (!currentlyFeeding) {
      // refresh statuses every x minutes
      if (millis() - previousMillis > DELAY_SEC * 60000)
      {
        previousMillis = millis();
        Refresh();
        Blink(5, 50);
      }

      if (millis() - previousMillisBlink > 1000 && !Full())
      {
        previousMillisBlink = millis();
        Serial.println("TANK IS EMPTY");
        Blink(5, 50);
      }
      else {
        digitalWrite(LED, 1);
      }

     /*if (Full() && wasempty) {
        wasempty = false; // manage immediate report without overflooding the buffer
        Serial.println("FULL");
        String var = "feeder ready";
        buildDebug(var);
        //st::Everything::sendSmartString(var);
      }
      else if (!Full() && !wasempty) {
        wasempty = true;
        Serial.println("EMPTY");
        String var = "feeder empty";
        buildDebug(var);
        //st::Everything::sendSmartString(var);
      }*/
    }
  delay(10);
}
