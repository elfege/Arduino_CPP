//******************************************************************************************
//st::Everything::callOnMsgSend() optional callback routine.  This is a sniffer to monitor
//    data being sent to ST.  This allows a user to act on data changes locally within the
//    Arduino sktech.
//******************************************************************************************


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
  if (isDigit(message.charAt(0))) { //   -> TRUE
    Serial.print("my_integer_data = "); Serial.println(my_integer_data);

    // now store this value so next fwd or rvs request will apply this value
    reqSpeed = my_integer_data;

    // now store this value so next open or close request will apply this value
    int ThisVal = my_integer_data;

    // modulate speed accordingly in case it's already running (any future run cmd will start with latest value)
    
    ledcWrite(pwmChannelL, reqSpeed);
    ledcWrite(pwmChannelR, reqSpeed);

    // set it as new default
    Default = reqSpeed;

  }

  if (message.equals("on"))
  {
    Serial.println("on");
    poweron();
    refresh();
  }

  if (message.equals("off"))
  {
    Serial.println("off");
    poweroff();
    refresh();
  }
  if (message.equals("fwd"))
  {
    Serial.println("fwd");
    fwd(Default, 0);
    refresh();
  }
  if (message.equals("rvs"))
  {
    Serial.println("rvs");
    rvs(Default, 0);
    refresh();
  }
  if (message.equals("stop"))
  {
    Serial.println("stop");
    STOP();
    refresh();
  }

  if (message.equals("left"))
  {
    Serial.println("left");
    left(Default);
    refresh();
  }
  if (message.equals("right"))
  {
    Serial.println("right");
    right(Default);
    refresh();
  }

  if (message.equals("auto"))
  {
    Serial.println("auto");
    selfdrive();
    refresh();
  }
  if (message.equals("poweron"))
  {
    Serial.println("poweron");
    poweron();
    refresh();
  }
  if (message.equals("poweroff"))
  {
    Serial.println("poweroff");
    poweroff();
    refresh();
  }
  if (message.equals("dock")) {
    Serial.println("dock");
    dock();
    refresh();
  }
  if (message.equals("charge")) {
    Serial.println("charge");
    charge();
    refresh();
  }
  if (message.equals("stopcharge")) {
    Serial.println("stopcharge");
    stopcharge();
    refresh();
  }
  if (message.equals("stopauto"))
  {
    Serial.println("stopauto");
    STOPALL = true;
    STOP();
    refresh();
  }


  if (message.equals("charge"))
  {
    Serial.println("charge");
    charge();
    refresh();
  }
  if (message.equals("stopcharge"))
  {
    Serial.println("stopcharge");
    stopcharge();
    refresh();
  }

  if (message.equals("ignoreobs"))
  {
    Serial.println("ignoreobs");
    ignoreobs = true;
    refresh();
  }
  if (message.equals("dealwithobs"))
  {
    Serial.println("dealwithobs");
    ignoreobs = false;
    refresh();
  }

  if (message.equals("reset"))
  {
    Serial.println("reset");
    Reset();
  }
    if (message.equals("followFwd"))
  {
    Serial.println("FollowLineFWD");
    FollowLineFWD();
  }
    if (message.equals("followRvs"))
  {
    Serial.println("FollowLineRVS");
    FollowLineRVS();
  }

  if (message.equals("checkdocked"))
  {
    Serial.println("checking if docked");
    
    if(docked){
      checkdock();
    }
    else {
      Serial.println("NOT NEAR DOCKING POSITION");
    }
    refresh();
  }

}

//*****************************************************************************
// SMARTTHINGS CALLOUT  | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                      V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void callback(const String &message)
{

  Serial.print("CALLBACK Received message: '");
  Serial.print(message);
  Serial.println("' ");




}
