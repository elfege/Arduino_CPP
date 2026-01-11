void loop() {
  //  Serial.println("");
  //  Serial.print(" Sensor Value = "); Serial.print(analogRead(ASensor));
  //  delay(500);

  //webserver();
  smartthing.run();

  // refresh statuses every x minutes
  if (millis() - previousMillis > DELAY_SEC * 1000)
  {
    previousMillis = millis();
    Refresh();
  }

// purge the water tank/ activate pump
if (millis() - (unsigned long)previousMillisPURGE > (unsigned long)TimeLimit)
  {
    previousMillisPURGE = millis();
    On();
  }
  
  Blink(occ, lapse);
  delay(DELAY);


 
}
