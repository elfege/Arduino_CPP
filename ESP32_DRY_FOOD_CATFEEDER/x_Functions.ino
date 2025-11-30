void feed(int qt) {

  String var = "feeder feeding";
  sendToHub(var);
  var = "switch on";
  sendToHub(var);
  //st::Everything::sendSmartString(var); // update switch status (needed for generic evt handlers)

  term.print("feeding.. quantity in millis = ");
  term.println(qt);

  digitalWrite(feeder, HIGH);
  Start = millis();
  while (millis() - Start < (qt * 1000) && millis() - Start < 10000) {
    currentlyFeeding = true;  // stop all other activities in the main loop beside essentials
    // especially to prevent full() boolean to run, for it takes 500ms per loop!
    dontFeed = true;  //  prevent scheduled feed from overlaping this feeding
    loop();
  }
  digitalWrite(feeder, LOW);
  currentlyFeeding = false;
  previousMillisFeed = millis();  // reset this counter so it doesn't overlap with st schedules
  dontFeed = false;

  term.println("END OF feeding..");
  var = "FEEDING DONE";
  sendToHub(var);
  var = "feeder ready";
  sendToHub(var);  // update switch status (needed for generic evt handlers)
}

bool Full() {
  const int numSamples = 50; // Number of times to sample the sensor
  int nonZeroCount = 0;     // Count of non-zero readings

  for (int i = 0; i < numSamples; i++) {
    int r = analogRead(sensor);
    // term.println("analogRead(sensor) =>" + String(r));
    if (r != 0) { // Check if reading is non-zero
      nonZeroCount++;
      if (nonZeroCount >= 1) { // one non-zero reading means full
        return true;
      }
    }
    // Optional delay between readings to allow sensor to stabilize
    delay(10);
  }

  // If we reach here, the sensor readings are consistent with being not full
  return false;
}

void Refresh() {
  Blink(3, 50);

  String space = " ";
  String var = "quantity = " + space + qty;
  sendToHub(var);
  //st::Everything::sendSmartString(var);

  if (Full()) {
    String var = "tankLevel full";
    sendToHub(var);
    //st::Everything::sendSmartString(var);
  }
  if (!Full()) {

    var = "feeder empty";
    sendToHub(var);
    //st::Everything::sendSmartString(var);
  }

  delay(300);
  Refreshqty();

  Blink(3, 50);
}

void Refreshqty()  // called withing refresh() and within getQty (necessary redundancy)
{
  if (!stopAll)  // to allow ST to update with previously recorded value when controllers reboots
  {
    String var = String(qty);
    _server.send(200, "text/html", var);  // send a first response to prevent "ERR_EMPTY_RESPONSE" code
    var = "quantity " + var;              // build debug String
    sendToHub(var);                       // send debug to textarea AND ST
  } else {
    term.println("Waiting for end reboot");
  }
}
void sliderupdate()  // necessary redundancy to allow for slider to refresh without overloading debug text area
{
  String var = String(qty);

  _server.send(200, "text/html", var);  // for slider value to be updated
}

void sendToHub(String var) {
  st::Everything::sendSmartString(var);
}

void getDataDebug()  // called by JS
{
  // send last updated value in getDebug()

  _server.send(200, "text/html", debugData);
  if (debugData.length() > 5000) {
    debugData = "";
  }
}
void getMac() {
  String ip = WiFi.localIP().toString();
  String var = "Mac: " + WiFi.macAddress() + " IP:" + ip + " " + String(WiFi.SSID());
  _server.send(200, "text/html", var);
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

  String result = "Time since last boot: ";

  if (dispalyDays == 1) {
    result = result + dispalyDays + " day";
  } else if (dispalyDays > 1) {
    result = result + dispalyDays + " days";
  }
  if (displayHours == 1) {
    result = result + displayHours + " hour ";
  } else if (displayHours > 1) {
    result = result + displayHours + " hours ";
  }
  if (displayMinutes == 1) {
    result = result + displayMinutes + " minute ";
  } else if (displayMinutes > 1) {
    result = result + displayMinutes + " minutes ";
  }
  if (displaySeconds == 1) {
    result = result + displaySeconds + " second ";
  } else if (displaySeconds > 1) {
    result = result + displaySeconds + " seconds ";
  }

  // term.print(result);
  _server.send(200, "text/html", result);
  return result;
}


//*****************************************************************************
// Local Functions  | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
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

//*****************************************************************************

void Reset() {

  term.println("*****************************************************************************");
  term.println("                               RESETTING ESP                                ");
  term.println("*****************************************************************************");
  String var = "reset";
  sendToHub(var);
  unsigned long s = millis();
  while (millis() - s < 500) {
    httpHandler();
  }
  esp_restart();
}



//*****************************************************************************
// SMARTTHINGS CALLOUT  | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                      V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void messageCallout(String message) {

  String receivedMsg = "Received: '" + message + "' ";
  term.print(receivedMsg);
  term.print(message);
  term.println(receivedMsg);

  String var = "ERROR";

  ///  see if there is a request for a time delay or quantity

  if (message.equals("hoursUp")) {
    defaultProgramDelay++;
    if (defaultProgramDelay > 24) {
      defaultProgramDelay = 24;
    }
    programDelay = defaultProgramDelay * 1000 * 60 * 60;
    var = "hours " + defaultProgramDelay;
  } else if (message.equals("hoursDown")) {
    defaultProgramDelay--;
    if (defaultProgramDelay < 0) {
      defaultProgramDelay = 0;
    }
    programDelay = defaultProgramDelay * 1000 * 60 * 60;
    var = "hours " + defaultProgramDelay;
  }

  else if (message.startsWith("quantityUp")) {
    qty++;
    if (qty > 15) {
      qty = 15;
    }
    var = "quantity " + String(qty);
  } else if (message.startsWith("quantityDown")) {
    qty--;
    if (qty < 1) {
      qty = 1;
    }
    var = "quantity " + String(qty);
  } else if (message.startsWith("updatequantity")) {

    qty = parseInteger(message);  // update global variable
    term.println("quantity updated after reboot");
    var = "quantity " + String(qty);
  }

  else if (message.equals("feed")) {
    feed(qty);
    delay(500);
    var = "switch on";
  } else if (message.equals("stop")) {
    digitalWrite(feeder, 0);
    var = "feeder standby";
  } else if (message.equals("refresh")) {
    term.println("refreshing");
    var = "feeder refreshing";
    Refresh();
  } else if (message.equals("reset")) {

    term.println("reseting");
    var = "feeder reset";
    Reset();
  }

  else if (digitalRead(feeder) == 0 && Full()) {
    term.println("FULL -");
    var = "feeder ready";
  } else if (digitalRead(feeder) == 0 && !Full()) {
    var = "feeder empty";
  } else if (message.startsWith("updatehours")) {

    programDelay = defaultProgramDelay * 1000 * 60 * 60;
    term.println("defaultProgramDelay updated after reboot");
  } else if (message.startsWith("schedulefeed")) {

    int split_d = message.indexOf("d");
    String hour = message.substring(0, split_d);
    term.println("");
    term.print("schedule hour is: ");
    term.println(hour);
  }


  sendToHub(var);  // also updates ST
  //st::Everything::sendSmartString(var);
}


int parseInteger(String thisMessage) {
  // Length (with one extra character for the null terminator)
  int str_len = thisMessage.length() + 1;
  // Prepare the character array (the buffer)
  char char_array[str_len];
  // Copy it over
  thisMessage.toCharArray(char_array, str_len);


  int i = 0;
  int s = str_len;  // sizeof(currentLine);
  String New = "";
  /// 255 as string = 23 bytes
  for (s > 0; i <= 23; i++) {
    if (isDigit(thisMessage[i])) {
      New += thisMessage[i];
      term.println("New == " + String(New));
    }
  }
  int n = New.toInt();  //convert String into a number

  return n;
}
