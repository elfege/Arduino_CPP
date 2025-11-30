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
  
  bool currentState = level_is_low(); // Get the current state

  // Check if the state has changed and the last change was at least 3 seconds ago
  if (currentState != lastState && millis() - lastStateMillis > 3000)
  {
    if (currentState) // If the sensor reports LOW state (water dry)
    {
      sendToHub("water dry");
      term.println("WATER IS LOW! (DRY STATE)");
    }
    else // If the sensor reports HIGH state (water wet)
    {
      sendToHub("water wet");
      term.println("WATER LEVEL OK (WET STATE)");
    }

    lastState = currentState;        // Update the last state
    lastStateMillis = millis();      // Update the time of the last state change
  }
}
