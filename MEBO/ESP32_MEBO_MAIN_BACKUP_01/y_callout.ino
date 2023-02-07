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
    rqSp = my_integer_data;

    // now store this value so next open or close request will apply this value
    int ThisVal = my_integer_data;

    // modulate speed accordingly in case it's already running (any future run cmd will start with latest value)

    ledcWrite(pwmChannelL, rqSp);
    ledcWrite(pwmChannelR, rqSp);

    // set it as new default
    if (rqSp <= 200) {
      Default = rqSp;
    }
    else {
      Default = 200; // faster might fry the motors
    }

  }

  if (message.equals("on"))
  {
    Serial.println("on");
    meboOn();
    refresh();
  }

  if (message.equals("off"))
  {
    Serial.println("off");
    meboOff();
    refresh();
  }
  if (message.equals("fwd"))
  {
    Serial.println("fwd");
    fwd(Default, 0);
    //refresh(); // slows down cliff response !
  }
  if (message.equals("refresh"))
  {
    Serial.println("refresh");
    refresh();
  }
  if (message.equals("rvs"))
  {
    Serial.println("rvs");
    rvs(Default, 0);

    //refresh();
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
    delay(200);
    STOP();
  }
  if (message.equals("right"))
  {
    Serial.println("right");
    right(Default);
    delay(200);
    STOP();
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
    meboOn();
    refresh();
  }
  if (message.equals("poweroff"))
  {
    Serial.println("poweroff");
    meboOff();
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
  }
  if (message.equals("stopcharge")) {
    Serial.println("stopcharge");
    stopcharge();
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
    doReset = true;
    RstCtDwnStart = millis();
  }
  if (message.equals("followFwd"))
  {
    Serial.println("FollowLineFWD");
    FollowLineFWD();
  }
  if (message.equals("followRvs"))
  {
    Serial.println("FollowLineRVS");
    dock();
  }

  if (message.equals("checkdocked"))
  {
    Serial.println("checking if docked");

    if (docked) {
      //checkdock();
    }
    else {
      Serial.println("NOT NEAR DOCKING POSITION");
    }
    refresh();
  }

  //***********************************************************//
  // ARM commands responses
  //***********************************************************//
  if (message.equals("foreArmUp"))
  {
    Serial.println("foreArmUp");
    foreArmUp();
  }
  else if (message.equals("foreArmDown"))
  {
    Serial.println("foreArmDown");
    foreArmDown();
  }
  else if (message.equals("armUp"))
  {
    Serial.println("armUp");
    armUp();
  }
  else if (message.equals("armDown"))
  {
    Serial.println("armDown");
    armDown();
  }
  else if (message.equals("wristLeft"))
  {
    Serial.println("wristLeft");
    wristLeft();
  }
  else if (message.equals("wristRight"))
  {
    Serial.println("wristRight");
    wristRight();
  }
  else if (message.equals("closeClamp"))
  {
    Serial.println("closeClamp");
    closeClamp();
  }
  else if (message.equals("openClamp"))
  {
    Serial.println("openClamp");
    openClamp();
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


void refresh() {
  String var = "NO COMMMAND";

  if (docked) {
    Serial.println("MEBO docked afd54e");
    var = "MEBO docked";
  }
  else {
    // check that it's docked
    stopcharge();
    delay(2000); // wait
    if (haspower()) {
      Serial.println("HAS POWER!!!");
      var = "MEBO docked";
      docked = true;
      charge();
    }
    else {
      Serial.println("MEBO NOT docked efc4e");
      var = "MEBO NOTdocked";
    }
  }
  Serial << "sending - >: " << var << endl;
  st::Everything::sendSmartString(var);

}
