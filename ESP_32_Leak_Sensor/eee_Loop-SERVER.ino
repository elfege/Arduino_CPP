

void loop()
{
  ArduinoOTA.handle();
  //webserver();
  //*****************************************************************************
  //Execute the Everything run method which takes care of "Everything"
  //*****************************************************************************
  st::Everything::run();

  if (millis() - previousMillisBlink > 1000)
  {
    //    String TEST = "TEST TEST";
    //    st::Everything::sendSmartString(TEST);
    //    Serial.println("SEND TEST");

    Blink(2, 200);
    delay(200);

    previousMillisBlink = millis();
  }
}
