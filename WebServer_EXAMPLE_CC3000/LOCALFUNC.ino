
//*****************************************************************************
// Local Functions  | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************

void messageCallout(String message)
{
  // if debug is enabled print out the received message
  if (isDebugEnabled)
  {
    Serial.print("Received message: '");
    Serial.print(message);
    Serial.println("' ");
  }

  // if message contents equals to 'on' then call On() function
  // else if message contents equals to 'off' then call Off() function
  if (message.equals("switch on"))
  {
    On();
  }
  else if (message.equals("switch off"))
  {
    Off();
  }
  else if (message.equals("switch sleep"))
  {
    Sleep();
  }
}

///// LOCAL FUNCTIONS ////
void On() {
ShutDown = false;
  if (!isOn()) {
    digitalWrite(switchPin, 1);
    delay(500);
    digitalWrite(switchPin, 0);


    Serial.println("");
    Serial.println("");
    Serial.println("COMPUTER TURNED ON");
  }
  else {
    Serial.println("ALREADY RUNNING");
  }
}

void Off() {

  if (isOn()) {
    digitalWrite(switchPin, 1);
    while (isOn()) {
      yield();
    }
    delay(500);
    digitalWrite(switchPin, 0);

    Serial.println("");
    Serial.println("");
    Serial.println("COMPUTER TURNED OFF");
    //smartthing.send("switch off");       // send message to cloud
  }
  else {
    Serial.println("ALREADY OFF");
  }
  ShutDown = true;
}


void Sleep() {
ShutDown = false;
  if (isOn()) {
    digitalWrite(switchPin, 1);
    delay(500);
    digitalWrite(switchPin, 0);

    Serial.println("");
    Serial.println("");
    Serial.println("COMPUTER PUT TO SLEEP");
    ////smartthing.send("switch sleep");       // send message to cloud
  }
  else {
    Serial.println("ALREADY OFF");
  }

}

boolean isOn() {
  bool result;
  if (analogRead(ASensor) >= 1020) {
    result = true;
  }
  else {
    result = false;
  }
  Serial.println(result);
  return result;
}

void Reset() {
  

 
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

