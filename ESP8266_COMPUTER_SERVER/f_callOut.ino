
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
    on();
  }
  else if (message.equals("off"))
  {
    off();
  }
  else if (message.equals("sleep"))
  {
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
