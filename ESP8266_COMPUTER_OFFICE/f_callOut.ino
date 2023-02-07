
void messageCallout(String message)
{
  // if debug is enabled print out the received message
  if (isDebugEnabled)
  {
    Serial.print("Received message: '");
    Serial.print(message);
    Serial.println("' ");
  }

  // if message contents equals to 'on' then call on() function
  // else if message contents equals to 'off' then call off() function
  if (message.equals("override")) {
    Serial.println("OVERRIDE");
    userOverride = !userOverride;
    String var = userOverride ? "Override Active" : "Override Inactive";
    _server.send(200, "text/html", var);
    buildDebug(var);
  }
  if (message.equals("on"))
  {
    STOP = true; // in case while() loop is running 
    unsigned long s = millis();
    while(OnOffRunning && millis() - s < 5000)
    {
      master(); // waiting for OnOffRunning to return false
    }
    if(OnOffRunning) // if still true means STOP didn't break the on() or off() inner loops within timeframe
    {
      Serial.println("TIMEOUT RESET");
      Reset();
    }
    on();
  }
  else if (message.equals("off"))
  {
    STOP = true; // in case while() loop is running 
    off();
  }
  else if (message.equals("sleep"))
  {
    STOP = true; // in case while() loop is running 
    off();
  }
  else if (message.equals("ALLOWSHUTDOWN"))
  {
    allowshutDownWhenFail = true;
    String var = "SHUTDOWN ALLOWED";
    buildDebug(var);
  }
  else if (message.equals("FORBIDSHUTDOWN"))
  {
    allowshutDownWhenFail = false;
    String var = "SHUTDOWN FORBIDDEN";
    buildDebug(var);
  }
  
  else if (message.equals("refresh"))
  {
    refresh();
  }
}
