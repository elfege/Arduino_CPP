
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
  if (message.equals("ignorecommand")) {
    ignorecommand = true;
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
  else if (message.equals("refresh"))
  {
    Refresh();
  }
}


void refresh()
{
  String state = isOn() ? "switch on" : "switch off";
  buildDebug(state);
}
