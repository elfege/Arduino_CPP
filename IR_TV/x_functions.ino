

void toggle() {  // state updated in loop()

  irsend.sendSAMSUNG(0xE0E040BF, 32);  // TV toggle cmd
  delay(DELAY_BETWEEN_COMMANDS);

  if (lastRequestedState == "on") {
    unsigned long st = millis();
    while (millis() - st < 2000) {
      http_handle();
    }
    menu();  // turn on the apple TV
  }

  //  lastRequestedState = currentState; // let loop() update these values
}


void Select() {  // center button
  irsend.sendNEC(0x77E13AD6, 32);
  delay(DELAY_BETWEEN_COMMANDS);
  //appleTvMainMenu();
}
void appleTvOff() {
  // long press on play = standby
  int del = 1;
  appleTvMainMenu();
  delay(1000);

  irsend.sendNEC(0x77E1FAD6, 32);  // play button cmd
  delay(170);
  unsigned long Start = millis();
  while (millis() - Start < 6000)  //hold for 5 seconds (3rd gen apple tv only, no way with 4th!)
  {
    irsend.sendRaw(rawZeroBit, 3, khz);  // emulates holding the button
    delay(del);
  }
}

void appletvplaypause() {
  irsend.sendNEC(0x77E1FAD6, 32);  // play button cmd
}
void appleTvMainMenu() {
  int del = 100;
  irsend.sendNEC(0x77E1C0D6, 32);  // menu button cmd
  delay(170);
  unsigned long Start = millis();
  while (millis() - Start < 6000)  //hold for 5 seconds (3rd gen apple tv only, no way with 4th!)
  {
    irsend.sendRaw(rawZeroBit, 3, khz);  // emulate holding the button
    delay(del);
  }
}
void menu() {
  irsend.sendNEC(0x77E1C0D6, 32);  // menu button cmd
  delay(DELAY_BETWEEN_COMMANDS);
}
void up() {
  irsend.sendNEC(0x77E150A8, 32);
  delay(DELAY_BETWEEN_COMMANDS);
}
void down() {
  irsend.sendNEC(0x77E130A8, 32);
  delay(DELAY_BETWEEN_COMMANDS);
}
void left() {
  irsend.sendNEC(0x77E190A8, 32);
  delay(DELAY_BETWEEN_COMMANDS);
}
void right() {
  irsend.sendNEC(0x77E160A8, 32);
  delay(DELAY_BETWEEN_COMMANDS);
}
void getPowerState() {
  lastRequestedState = tvIsOn() ? "off" : "on";  // if on, then it's an off request and reciprocally This var allows to turn on Apple TV by calling menu()
  toggle();
  delay(2000);
  String state = tvIsOn() ? "TV IS ON" : "TV IS OFF";

  _server.send(200, "text/plain", state);
}
void getPowerStateOnly() {
  String state = tvIsOn() ? "TV IS ON" : "TV IS OFF";
  _server.send(200, "text/plain", state);
}

boolean tvIsOn() {
  int read = analogRead(Asensor);
  if (millis() - lastCurrentLogMilli >= 500) {
    term.println("usb current sensor: " + String(read));
    lastCurrentLogMilli = millis();
  }
  return read >= 1024;
}

void ircmd(uint32_t* val) {

  irsend.sendRaw(val, 99, khz);

  //delay(1000);
  // don't send any event from here
}

void Refresh() {

  String var = "error";
  if (tvIsOn()) {
    var = "switch on";
    lastRequestedState = "on";
  } else {
    var = "switch off";
    lastRequestedState = "off";
  }
  sendData(var);
}

void getMac() {
  String ip = WiFi.localIP().toString();
  String var = "Mac: " + WiFi.macAddress() + " IP:" + ip + " " + String(WiFi.SSID());
  _server.send(200, "text/html", var);
}


void log() {

  String debug1 = "";
  String debug2 = "";
  sendData(debug1);
  sendData(debug2);
}

//*****************************************************************************
// Local Functions  | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************


void messageCallout(String message) {


  if (message.equals("on")) {
    lastRequestedState = "on";
    if (!tvIsOn()) {
      toggle();
      Select();
    }
  } else if (message.equals("off")) {
    lastRequestedState = "off";
    if (tvIsOn()) {
      toggle();
      //appleTvOff();
    }
  } else if (message.equals("appletvplaypause")) {
    appletvplaypause();
  } else if (message.equals("up")) {
    up();
  } else if (message.equals("down")) {
    down();
  } else if (message.equals("left")) {
    left();
  } else if (message.equals("left")) {
    left();
  } else if (message.equals("menu")) {
    menu();
  } else if (message.equals("mainmenu")) {
    appleTvMainMenu();
  }

  sendData("received message " + message);
  Refresh();
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

void glow() {
  int s = 1024;  //ESP8266 uses 10-bit resolution for PWM generation

  if (glw >= s) {
    goUp = false;
  } else if (glw < 1) {
    goUp = true;
  }

  if (goUp && glw < s) {
    glw++;
    analogWrite(LED, glw);
    //Serial.print("GLOW "); term.println(i);

  } else if (!goUp && glw > 0) {
    glw--;
    analogWrite(LED, glw);
    //Serial.print("GLOW "); term.println(i);
  }
  //  delay(5); // delay must be set in the scope where it's called from
}


void Reset() {

  sendData("RESET_CMD RECEIVED!");
  unsigned long stt = millis();
  while (millis() - stt < 1000)  // give it time to send the debug message
  {
    loop();
  }
  ESP.reset();  // ESP.reset is hard reset
}


String TimeInfos() {
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

  if (dispalyDays == 1) {
    result = result + dispalyDays + " day";
    Serial.print(result);
  } else if (dispalyDays > 1) {
    result = result + dispalyDays + " days";
    Serial.print(result);
  }
  if (displayHours == 1) {
    result = result + displayHours + " hour ";
    Serial.print(result);
  } else if (displayHours > 1) {
    result = result + displayHours + " hours ";
    Serial.print(result);
  }
  if (displayMinutes == 1) {
    result = result + displayMinutes + " minute ";
    Serial.print(result);
  } else if (displayMinutes > 1) {
    result = result + displayMinutes + " minutes ";
    Serial.print(result);
  }
  if (displaySeconds == 1) {
    result = result + displaySeconds + " second ";
    Serial.print(result);
  } else if (displaySeconds > 1) {
    result = result + displaySeconds + " seconds ";
    Serial.print(result);
  }

  _server.send(200, "text/html", result);
  return result;
}

void sendData(String var) {
  term.println("var = " + var);
  smartthing.send(var);  // send the current value to smartthings
  _server.send(200, "text/html", var);
}