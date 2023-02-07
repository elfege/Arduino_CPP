void feed(int qt) {

  String var = "feeder feeding";
  buildDebug(var);
  var = "switch on";
  buildDebug(var);
  //st::Everything::sendSmartString(var); // update switch status (needed for generic evt handlers)

  Serial.print("feeding.. quantity in millis = "); Serial.println(qt);

  digitalWrite(feeder, HIGH);
  Start = millis();
  while (millis() - Start < (qt * 1000) && millis() - Start < 10000) {
    currentlyFeeding = true; // stop all other activities in the main loop beside essentials
    // especially to prevent full() boolean to run, for it takes 500ms per loop!
    dontFeed = true; //  prevent scheduled feed from overlaping this feeding
    loop();
  }
  digitalWrite(feeder, LOW);
  currentlyFeeding = false;
  previousMillisFeed = millis(); // reset this counter so it doesn't overlap with st schedules
  dontFeed = false;

  Serial.println("END OF feeding..");
  var = "FEEDING DONE";
  buildDebug(var);
  var = "feeder ready";
   buildDebug(var); // update switch status (needed for generic evt handlers)
 
}

boolean Full() {
  bool result = false;
  if (analogRead(sensor) >= 500) // 0 means full
  {
    result = true;
  }

  return result;
}

void Refresh() {
  Blink(3, 50);

  String space = " ";
  String var = "quantity = " + space + qty;
  buildDebug(var);
  //st::Everything::sendSmartString(var);

  if (Full()) {
    String var = "tankLevel full";
    buildDebug(var);
    //st::Everything::sendSmartString(var);
  }
  if (!Full()) {
    
    var = "feeder empty";
    buildDebug(var);
    //st::Everything::sendSmartString(var);
  }

  delay(300);
  Refreshqty();

  Blink(3, 50);
}

void Refreshqty() // called withing refresh() and within getQty (necessary redundancy)
{
  if (!stopAll) // to allow ST to update with previously recorded value when controllers reboots
  {
    String var = String(qty);
    _server.send(200, "text/html", var); // send a first response to prevent "ERR_EMPTY_RESPONSE" code
    var = "quantity " + var; // build debug String
    buildDebug(var); // send debug to textarea AND ST
  }
  else
  {
    buildDebug("Waiting for end reboot");
  }
}
void sliderupdate() // necessary redundancy to allow for slider to refresh without overloading debug text area
{
  String var = String(qty);

  _server.send(200, "text/html", var); // for slider value to be updated
}

void buildDebug(String var)
{
  st::Everything::sendSmartString(var); // send the current value to smartthings
  var = debugData.concat(var + "\n"); // build the debug data string
}

void getDataDebug() // called by JS
{
  // send last updated value in getDebug()

  _server.send(200, "text/html", debugData);
  if (debugData.length() > 5000)
  {
    debugData = "";
  }
}
void getMac()
{
  String ip = WiFi.localIP().toString();
  String var = "Mac: " + WiFi.macAddress() + " IP:" + ip + " " +  String(WiFi.SSID());
  _server.send(200, "text/html", var);
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


  Serial.println("LOOP time = " + String(elapsed) + "ms");
  String result = "Time since last boot: ";

  if (dispalyDays == 1)
  {
    result = result + dispalyDays + " day";
    Serial.print(result);
  }
  else if (dispalyDays > 1)
  {
    result = result + dispalyDays + " days";
    Serial.print(result);
  }
  if (displayHours == 1)
  {
    result = result + displayHours + " hour ";
    Serial.print(result);
  }
  else if (displayHours > 1)
  {
    result = result + displayHours + " hours ";
    Serial.print(result);
  }
  if (displayMinutes == 1)
  {
    result = result + displayMinutes + " minute ";
    Serial.print(result);
  }
  else if (displayMinutes > 1)
  {
    result = result + displayMinutes + " minutes ";
    Serial.print(result);
  }
  if (displaySeconds == 1)
  {
    result = result + displaySeconds + " second ";
    Serial.print(result);
  }
  else if (displaySeconds > 1)
  {
    result = result + displaySeconds + " seconds ";
    Serial.print(result);
  }

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
  String var = "reset";
  buildDebug(var);
  //st::Everything::sendSmartString(var);
  Serial.println("RESETING ESP8266");

  // digitalWrite(RESET_PIN, HIGH);
  // Serial.println("FAILED....");
  delay(1500);
  esp_restart();
}

//*****************************************************************************
// SMARTTHINGS CALLOUT  | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                      V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void messageCallout(String message)
{

  String receivedMsg = "Received: '" + message + "' ";
  Serial.print(receivedMsg);
  Serial.print(message);
  buildDebug(receivedMsg);

  String var = "ERROR";

  ///  see if there is a request for a time delay or quantity

  if (message.equals("hoursUp")) {
    defaultProgramDelay++;
    if (defaultProgramDelay > 24) {
      defaultProgramDelay = 24;
    }
    programDelay = defaultProgramDelay * 1000 * 60 * 60;
    var = "hours " + defaultProgramDelay;
  }
  else if (message.equals("hoursDown")) {
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
  }
  else if (message.startsWith("quantityDown")) {
    qty--;
    if (qty < 1) {
      qty = 1;
    }
    var = "quantity " + String(qty);
  }
  else if (message.startsWith("updatequantity")) {
    
    qty = parseInteger(message); // update global variable
    Serial.println("quantity updated after reboot");
    var = "quantity " + String(qty);
  }

  else if (message.equals("feed"))
  {
    feed(qty);
    delay(500);
    var = "switch on";
  }
  else if (message.equals("stop"))
  {
    digitalWrite(feeder, 0);
    var = "feeder standby";
  }
  else if (message.equals("refresh"))
  {
    Serial.println("refreshing");
    var = "feeder refreshing";
    Refresh();
  }
  else if (message.equals("reset"))
  {

    Serial.println("reseting");
    var = "feeder reset";
    Reset();
  }

  else if (digitalRead(feeder) == 0 && Full()) {
    Serial.println("FULL -");
    var = "feeder ready";
  }
  else if (digitalRead(feeder) == 0 && !Full()) {
    var = "feeder empty";
  }
  else if (message.startsWith("updatehours")) {

    programDelay = defaultProgramDelay * 1000 * 60 * 60;
    Serial.println("defaultProgramDelay updated after reboot");
  }
  else if (message.startsWith("schedulefeed")) {

    int split_d = message.indexOf("d");
    String hour = message.substring(0, split_d);
    Serial.println("");
    Serial.print("schedule hour is: "); Serial.println(hour);
  }


  buildDebug(var); // also updates ST
  //st::Everything::sendSmartString(var);

}


int parseInteger(String thisMessage) {
  // Length (with one extra character for the null terminator)
  int str_len = thisMessage.length() + 1;
  // Prepare the character array (the buffer)
  char char_array[str_len];
  // Copy it over
  thisMessage.toCharArray(char_array, str_len);
  //Serial << "currentLine char =============================== " << currentLine << endl;


  int i = 0;
  int s = str_len; // sizeof(currentLine);
  //Serial << "s =============================== " << s << endl;
  String New = "";
  /// 255 as string = 23 bytes
  for ( s > 0; i <= 23; i++) {
    if (isDigit(thisMessage[i])) {
      New += thisMessage[i];
      Serial << "New =============================== " << New << endl;
    }
  }
  int n = New.toInt();  //convert String into a number

  return n;
}
