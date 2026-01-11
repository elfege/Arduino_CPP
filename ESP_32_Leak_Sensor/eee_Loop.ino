

void loop()
{
  ArduinoOTA.handle();

  if (!stopAll)
  {

    st::Everything::run();


    if (analogRead(PIN_WATER_1) > WetVal && millis() - previousMillisDeclaration > 5000) // declare wet immediately when it occurs
    {
      val = "water wet";
      st::Everything::sendSmartString(val);
      Serial.println("water wet");
      declaredWet = true;
      previousMillisDeclaration = millis();
      Blink(10, 20);
    }
    else if (declaredWet) // declare dry as soon as possible
    {
      val = "water dry";
      st::Everything::sendSmartString(val);
      declaredWet = false;
      Serial.println("water dry");

    }
    else if (millis() - previousMillisDeclaration > 60000) // periodic dry status declaration
    {
      val = "water dry";
      st::Everything::sendSmartString(val);
      Serial.println("water dry");

      previousMillisDeclaration = millis();

      Blink(2, 200);
    }
    if (millis() - previousMillisBlink > 1000)
    {
      Blink(2, 200);
      previousMillisBlink = millis();
    }

  }

}
