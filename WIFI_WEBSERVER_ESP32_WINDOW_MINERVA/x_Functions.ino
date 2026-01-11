
void Open() {
  opening = true;
  closing = false;
  stopped = false;

  SpeedVal = SLOW; // resets to silent mode every new opeartion
  
  modifRequested = false; 
  
  String var = "opening";
  st::Everything::sendSmartString(var);
  stopped = false;
  Serial.println("FORWARD");

  dutyCycle = SLOW; // starting speed
  ledcWrite(pwmChannel, dutyCycle);

  delay(10);

  digitalWrite(PinMotor1,  A);
  digitalWrite(PinMotor2,  B);

  Refresh();
  while (dutyCycle <= SpeedVal && !stopped && !closing && !modifRequested) {

    ledcWrite(pwmChannel, dutyCycle);
    Serial.print("open with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle += 8;

    loop();
    delay(100);
  }
  if(modifRequested){
    // making sure new req is taken into account
    ledcWrite(pwmChannel, SpeedVal);
  }


}

void Close() {
  closing = true;
  stopped = false;
  opening = false;
  modifRequested = false;
   SpeedVal = SLOW; // resets to silent mode every new opeartion

  String var = "closing";
  st::Everything::sendSmartString(var);
  Serial.println("REVERSE");

  dutyCycle = SLOW; // starting point
  ledcWrite(pwmChannel, dutyCycle);
  
  delay(10);
  
  digitalWrite(PinMotor1,  B);
  digitalWrite(PinMotor2,  A);
  

  Refresh();

  while (dutyCycle < SpeedVal && !stopped && !opening && !modifRequested) {

    ledcWrite(pwmChannel, dutyCycle);
    Serial.print("close with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle += 8;

    delay(100);
    loop();
  }
  if(modifRequested){
    // making sure new req is taken into account
    ledcWrite(pwmChannel, SpeedVal);
  }


}

void Stop()
{
  stopped = true;
  closing = false;
  opening = false;
  

  ledcWrite(pwmChannel, 0);
  digitalWrite(PinMotor1,  0);
  digitalWrite(PinMotor2,  0);
  Serial.println("STOP");
  stopped = true;
  
  String var = "stop";
  st::Everything::sendSmartString(var);
}



boolean isOpen() {

  if (digitalRead(PinMotor2) == B) {
    return true;
  }
  else {
    return false;
  }

}


void Refresh() {

  if (!isOpen() && !stopped) {
    Serial.println("reporting closed");
    String var = "closed";
    st::Everything::sendSmartString(var);
  }
  else if (isOpen() && !stopped)
  {
    Serial.println("reporting open");
    String var = "open";
    st::Everything::sendSmartString(var);
  }
  else if (stopped) {
    Serial.println("reporting stopped");
    String var = "stopped";
    st::Everything::sendSmartString(var);
  }

  Blink(5, 50);
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
  st::Everything::sendSmartString(var);
  Serial.println("RESETING ESP8266");

  digitalWrite(RESET_PIN, HIGH);
  Serial.println("FAILED....");
  delay(2000);
  ESP.restart();
}

//*****************************************************************************
// SMARTTHINGS CALLOUT  | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                      V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void messageCallout(String message)
{

  Serial.print("Received message: '");
  Serial.print(message);
  Serial.println("' ");


  ///  see if there is a request for a different speed

  String thisMessage = message;
  char this_char[thisMessage.length() + 1];
  thisMessage.toCharArray(this_char, sizeof(this_char));

  test_thisMessage = thisMessage;// for web debug

  int my_integer_data = atoi(this_char);

  // check if this message is a digit

  //if (isDigit(my_integer_data)) {
  if (isDigit(message.charAt(0))) { //   -> TRUE
    //if(my_integer_data >= 0){
    Serial.print("my_integer_data = "); Serial.println(my_integer_data);

    // now store this value so next open or close request will apply this value
    SpeedVal = my_integer_data;

    // infor ST of this value has been taken into consideration
    String var = "thisMessage";
    st::Everything::sendSmartString(var);

    // modulate speed accordingly in case it's already running (any future open() cmd will start with latest value
    modifRequested = true;
    ledcWrite(pwmChannel, SpeedVal);

  }


  if (message.equals("open"))
  {
    Serial.println("opening");
    Open();
    Refresh();
  }
  else if (message.equals("close"))
  {
    Serial.println("closing");
    Close();
    Refresh();
  }
  else if (message.equals("stop"))
  {
    Serial.println("stopping");
    Stop();
    Refresh();
  }
  else if (message.equals("refresh"))
  {
    Serial.println("refreshing");
    Refresh();
  }
  else if (message.equals("reset"))
  {
    Serial.println("reseting");
    Reset();
  }



}







