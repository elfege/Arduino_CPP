void loop() {
  webserver();

  smartthing.run();


  // refresh statuses every x minutes
  if (millis() - previousMillis > DELAY_SEC * 1000)
  {
    previousMillis = millis();
    smartthing.send("REFRESH");
    Refresh();
    Blink(5, 50);
   
  }
  if (millis() - previousMillisBlink > 1000)
  {
    previousMillisBlink = millis();
    //Refresh();
    Blink(2, 200);
//    int test = analogRead(Asensor);
//    Serial.println("");
//    Serial.print("Asensor = ");Serial.println(test);
//     Serial.println("");
//    Serial.print("isOn = ");Serial.println(isOn);
  
  }

delay(100);
}
