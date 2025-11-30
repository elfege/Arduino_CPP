
void On()
{
  digitalWrite(power, 1);
  
  String state = "KOBRA ESP32 MAIN CONTROLLER IS NOW BOOTING";
  //_server.send(200, "text/plain", state);
  buildDebug("switch on");
  delay(1000);
  remoteReset(); 
}

void Off()
{
  digitalWrite(power, 0);
  String state = "KOBRA IS OFF";
  //_server.send(200, "text/plain", state);
  buildDebug("switch off");
}
void remoteReset()
{
  Serial.println("REMOTE RESET");
  delay(1000);
  digitalWrite(ESP32rst, 0); // pull down ESP32 'EN' port to reset it
  Serial.println("ESP32rst = "+ String(digitalRead(ESP32rst)));
  delay(10);
  digitalWrite(ESP32rst, 1);
  delay(1);
  Serial.println("ESP32rst = "+ String(digitalRead(ESP32rst)));
  buildDebug("REMOTE RESET");
}


void Reboot()
{
  digitalWrite(power, 0);
  buildDebug("switch off"); 
  delay(5000);
  digitalWrite(power, 1);
  remoteReset();
  buildDebug("switch on");
}

void chargeon()
{
  Serial.println("TURNING ON CHARGER");
  digitalWrite(CHARGER, 0);
  buildDebug("battery charging");
  
}

void chargeoff()
{
  Serial.println("TURNING OFF CHARGER");
  digitalWrite(CHARGER, 1);
  buildDebug("battery notcharging");
  previousMillisCharge = millis();// will turn charge back on after 2 minutes if ESP32 is off

}


void buildDebug(String var)
{
  Serial.println("building debug");
  smartthing.send(var); // send the current value to smartthings
  debugData.concat(var + "\n"); // build the debug data string
  Serial.println("SENDING " + var + "TO HUB");
  
}
void getDataDebug() // called by JS
{
  // send last updated value in getDebug()


  if (debugData.length() > 1)
  {
    _server.send(200, "text/html", debugData);
    if (debugData.length() > 2000)
    {
      debugData = "";
      logs = false; // stop sensor logs
      buildDebug("DATA RESET");
    }
  }
  else
  {
    Serial.println("debugData string empty");
    _server.send(200, "text/html", "");
  }
}
void getMac()
{
  String ip = WiFi.localIP().toString();
  String var = "Mac: " + WiFi.macAddress() + " IP:" + ip + " " + String(WiFi.SSID());
  _server.send(200, "text/html", var);
}

void log()
{

  String debug1 = "";
  String debug2 = "";
  buildDebug(debug1);
  buildDebug(debug2);
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


  //Serial.println("LOOP time = " + String(elapsed) + "ms");
  String result = "Time since last boot: ";

  if (dispalyDays == 1)
  {
    result = result + dispalyDays + " day";
    //Serial.print(result);
  }
  else if (dispalyDays > 1)
  {
    result = result + dispalyDays + " days";
    //Serial.print(result);
  }
  if (displayHours == 1)
  {
    result = result + displayHours + " hour ";
    //Serial.print(result);
  }
  else if (displayHours > 1)
  {
    result = result + displayHours + " hours ";
    //Serial.print(result);
  }
  if (displayMinutes == 1)
  {
    result = result + displayMinutes + " minute ";
    //Serial.print(result);
  }
  else if (displayMinutes > 1)
  {
    result = result + displayMinutes + " minutes ";
    //Serial.print(result);
  }
  if (displaySeconds == 1)
  {
    result = result + displaySeconds + " second ";
    //Serial.print(result);
  }
  else if (displaySeconds > 1)
  {
    result = result + displaySeconds + " seconds ";
    //Serial.print(result);
  }

  _server.send(200, "text/html", result);
  return result;
}

void glow() {
  int s = 255;

  if (glw >= s) {
    goUp = false;
  }
  else if (glw < 1) {
    goUp = true;
  }

  if (goUp && glw < s) {
    glw++;
    analogWrite(LED, glw);
    //Serial.print("GLOW "); Serial.println(i);

  }
  else if (!goUp && glw > 0) {
    glw--;
    analogWrite(LED, glw);
    //Serial.print("GLOW "); Serial.println(i);
  }
  //delay(1);
}

void Blink(int times, int lapse) {
  int c = 0;
  while (c < times) {
    digitalWrite(LED, 1);
    delay(lapse);
    digitalWrite(LED, 0);
    delay(lapse);
    c++;
  }
}

void Reset(){
  Serial.println("POWER RESET REQUESTED!");
//  delay(1500);
//  digitalWrite(rst, 0);
ESP.reset(); // hard reset
}
