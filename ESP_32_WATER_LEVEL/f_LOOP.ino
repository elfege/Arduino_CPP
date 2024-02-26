void loop()
{
  ArduinoOTA.handle();

  if (!stopAll)
  {
    mainHandler();
    checkAndSend();
  }
}

void checkAndSend()
{
  bool currentState = level_is_low();

  if (currentState != lastState)
  { // If the state has changed
    if (currentState)
    {
      sendToHub("water dry");
      term.println("WATER IS LOW! (DRY STATE)");
    }
    else
    {
      sendToHub("water wet");
      term.println("WATER LEVEL OK (WET STATE)");
    }
    lastState = currentState; // Update the last state to the current state
  }
}
