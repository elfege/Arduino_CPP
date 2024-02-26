void on()
{
  OnOffRunning = true;
  if (!is_on())
  {
    digitalWrite(switchPin, 1);
    delay(500);
    digitalWrite(switchPin, 0);
    term.println("");
    term.println("");
    term.println("COMPUTER TURNED ON");
  }
  else
  {
    term.println("ALREADY RUNNING");
  }
  update_state();

  OnOffRunning = false;
  
}

void off()
{
  // NB: NEVER USE A RETURN STATEMENT IN THIS METHOD
  OnOffRunning = true;
  if (!noSleep)
  {
    term.println("Putting computer to sleep");

    if (is_on())
    {
      digitalWrite(switchPin, 1);
      delay(500);
      digitalWrite(switchPin, 0);
      unsigned long s = millis();
      while (millis() - s < 70000 && !STOP && is_on())
      {
        master();
      }
      if (millis() - s > 70000 && !STOP && is_on() && !userOverride)
      {
        // failed so try again
        digitalWrite(switchPin, 1);
        delay(500);
        digitalWrite(switchPin, 0);
      }
      STOP = false; // reset this boolean after using it
    }
    else
    {
      term.println("Computer already off or sleeping");
    }
  }
  else
  {
    term.println("SLEEP OVERRIDE - noSleep BOOL TRUE");
  }
  update_state();
  OnOffRunning = false;
}
void update_state()
{
  if (is_on())
  {
    last_state = "on";
    send_data("switch on");
    send_data("computer running");
  }
  else
  {
    last_state = "off";
    send_data("switch off");
    send_data("computer sleeping");
  }
}
void getPowerState()
{
  toggle_power();
  String state = is_on() ? "computer running" : "computer sleeping";
  _server.send(200, "text/plain", state);
  send_data(state);
}

void getPowerStateOnly()
{
  String state = is_on() ? "computer running" : "computer sleeping";
  _server.send(200, "text/plain", state);
  send_data(state);
}

void toggle_power()
{
  if (is_on())
  {
    STOP = true; // in case while() loop is running
    off();
  }
  else
  {
    STOP = true; // in case while() loop is running
    on();
  }
}

void hard_reboot()
{
  digitalWrite(switchPin, 1);
  delay(5000);
  digitalWrite(switchPin, 0);
  delay(5000);
  digitalWrite(switchPin, 1);
  delay(500);
  digitalWrite(switchPin, 0);
}
void shutDown()
{
  digitalWrite(switchPin, 1);
  delay(5000);
  digitalWrite(switchPin, 0);
}

void refresh()
{
  term.println("REFRESHING");

  last_state = is_on() ? "on" : "off";

  if (!is_on())
  {
    term.println("reporting 'computer sleeping'");
    send_data("switch off");
    send_data("computer sleeping");
    send_data("status sleeping");
  }
  else
  {
    term.println("reporting 'computer on'");
    send_data("switch on");
    send_data("computer running");
    send_data("status running");
  }

  String var = "";
  var = allowshutDownWhenFail ? "SHUTDOWN ALLOWED" : "SHUTDOWN FORBIDDEN";
  send_data(var);
  var = userOverride ? "Override Active" : "Override Inactive";
  //  _server.send(200, "text/html", var); // has to be called from js
  send_data(var);
  noSleep = false; // only place where this value is reset
}
/////////////////// BOOLEANS /////////////////////////
boolean is_on()
{
  bool result;
  if (analogRead(ASensor) >= 1020)
  {
    result = true;
  }
  else
  {
    result = false;
  }
  // term.println(result);
  return result;
}

void send_data(String var)
{
  
  new_state = last_state == "off" && var == "on" || last_state == "on" && var == "off";

  //refresh state every minute or if it changed
  if (millis() - last_message_sent_millis >= 60000 || new_state)
  {
    term.println("sending " + var + " to the hub");
    smartthing.send(var); // send the current value to smartthings
    last_message_sent_millis = millis();
  }

}


void getMac()
{
  String ip = WiFi.localIP().toString();
  var = "Mac: " + WiFi.macAddress() + " IP:" + ip + " " + String(WiFi.SSID());
  _server.send(200, "text/html", var);
}

void log()
{

  String debug1 = "";
  String debug2 = "";
  send_data(debug1);
  send_data(debug2);
}

String TimeInfos()
{
  unsigned long totalTimeSeconds = millis() / 1000;
  unsigned long totalTimeMinutes = millis() / 1000 / 60;
  unsigned long TotalTimeHours = millis() / 1000 / 60 / 60;
  unsigned long TotalTimeDays = millis() / 1000 / 60 / 60 / 24;

  unsigned long displaySeconds = (millis() - (totalTimeMinutes * 60 * 1000)) / 1000;
  unsigned long displayMinutes = (millis() - (TotalTimeHours * 1000 * 60 * 60)) / 1000 / 60;
  unsigned long displayHours = (millis() - (TotalTimeDays * 1000 * 60 * 60 * 24)) / 1000 / 60 / 60;
  unsigned long dispalyDays = TotalTimeDays;

  term.println("LOOP time = " + String(elapsed) + "ms");
  String result = "Time since last boot: ";

  if (dispalyDays == 1)
  {
    result = result + dispalyDays + " day";
    term.println(result);
  }
  else if (dispalyDays > 1)
  {
    result = result + dispalyDays + " days";
    term.println(result);
  }
  if (displayHours == 1)
  {
    result = result + displayHours + " hour ";
    term.println(result);
  }
  else if (displayHours > 1)
  {
    result = result + displayHours + " hours ";
    term.println(result);
  }
  if (displayMinutes == 1)
  {
    result = result + displayMinutes + " minute ";
    term.println(result);
  }
  else if (displayMinutes > 1)
  {
    result = result + displayMinutes + " minutes ";
    term.println(result);
  }
  if (displaySeconds == 1)
  {
    result = result + displaySeconds + " second ";
    term.println(result);
  }
  else if (displaySeconds > 1)
  {
    result = result + displaySeconds + " seconds ";
    term.println(result);
  }

  _server.send(200, "text/html", result);
  return result;
}

void glow(int DL)
{
  int s = 1024;

  if (glw >= s)
  {
    goUp = false;
  }
  else if (glw < 1)
  {
    goUp = true;
  }

  if (goUp && glw < s)
  {
    glw++;
    analogWrite(LED, glw);
    // term.print("GLOW "); term.println(i);
  }
  else if (!goUp && glw > 0)
  {
    glw--;
    analogWrite(LED, glw);
    // term.print("GLOW "); term.println(i);
  }
  delay(DL);
}

void Blink(int times, int lapse)
{
  int c = 0;
  while (c < times)
  {
    digitalWrite(LED, 1);
    delay(lapse);
    digitalWrite(LED, 0);
    delay(lapse);
    c++;
  }
}

void Reset()
{
  ESP.reset();
}
