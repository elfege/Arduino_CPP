//******************************************************************************************
//st::Everything::callOnMsgSend() optional callback routine.  This is a sniffer to monitor
//    data being sent to ST.  This allows a user to act on data changes locally within the
//    Arduino sktech.
//******************************************************************************************


void messageCallout(String message)
{

  term.print("Received message: '");
  term.print(message);
  term.println("' ");

  ///  see if there is a request for a different speed

  String thisMessage = message;
  char this_char[thisMessage.length() + 1];
  thisMessage.toCharArray(this_char, sizeof(this_char));

  test_thisMessage = thisMessage;// for web debug

  int my_integer_data = atoi(this_char);

  // check if this message is a digit
  if (isDigit(message.charAt(0))) { //   -> TRUE
    term.print("my_integer_data = "); term.println(my_integer_data);

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
    term.println("on");
    meboOn();
    refresh();
  }

  if (message.equals("off"))
  {
    term.println("off");
    meboOff();
    refresh();
  }
  if (message.equals("fwd"))
  {
    term.println("fwd");
    fwd(Default, 0);
  }
  if (message.equals("refresh"))
  {
    term.println("refresh");
    refresh();
  }
  if (message.equals("rvs"))
  {
    term.println("rvs");
    rvs(Default, 0);

    //refresh();
  }
  if (message.equals("stop"))
  {
    term.println("stop");
    STOP();
    //refresh();
  }

  if (message.equals("left"))
  {
    term.println("left");
    left(Default);
    delay(200);
    STOP();
  }
  if (message.equals("right"))
  {
    term.println("right");
    right(Default);
    delay(200);
    STOP();
  }

  if (message.equals("auto"))
  {
    term.println("auto");
    roam();
    refresh();
  }
  if (message.equals("poweron"))
  {
    term.println("poweron");
    meboOn();
    refresh();
  }
  if (message.equals("poweroff"))
  {
    term.println("poweroff");
    meboOff();
    refresh();
  }
  if (message.equals("dock")) {
    term.println("dock");
    dock();
    refresh();
  }
  if (message.equals("charge")) {
    term.println("charge");
    charge();
  }
  if (message.equals("stopcharge")) {
    term.println("stopcharge");
    stopcharge();
    refresh(); // will check if it's ok to turn on the charging or if it's not docked and then not ok
  }
  if (message.equals("stopauto"))
  {
    term.println("stopauto");
    STOPALL = true;
    STOP();
    refresh();
  }

  if (message.equals("ignoreobs"))
  {
    term.println("ignoreobs");
    ignoreobs = true;
    refresh();
  }
  if (message.equals("dealwithobs"))
  {
    term.println("dealwithobs");
    ignoreobs = false;
    refresh();
  }

  if (message.equals("reset"))
  {
    term.println("reset");
    doReset = true;
    RstCtDwnStart = millis();
  }
  if (message.equals("followFwd"))
  {
    term.println("FollowLineFWD");
    FollowLineFWD();
  }
  if (message.equals("followRvs"))
  {
    term.println("FollowLineRVS");
    dock();
  }

  if (message.equals("checkdocked"))
  {
    term.println("checking if docked");

    if (docked) {
      //checkdock();
    }
    else {
      term.println("NOT NEAR DOCKING POSITION");
    }
    refresh();
  }

  //***********************************************************//
  // ARM commands responses
  //***********************************************************//
  if (message.equals("foreArmUp"))
  {
    term.println("foreArmUp");
    foreArmUp();
  }
  else if (message.equals("foreArmDown"))
  {
    term.println("foreArmDown");
    foreArmDown();
  }
  else if (message.equals("armUp"))
  {
    term.println("armUp");
    armUp();
  }
  else if (message.equals("armDown"))
  {
    term.println("armDown");
    armDown();
  }
  else if (message.equals("wristLeft"))
  {
    term.println("wristLeft");
    wristLeft();
  }
  else if (message.equals("wristRight"))
  {
    term.println("wristRight");
    wristRight();
  }
  else if (message.equals("closeClamp"))
  {
    term.println("closeClamp");
    closeClamp();
  }
  else if (message.equals("openClamp"))
  {
    term.println("openClamp");
    openClamp();
  }

}

//*****************************************************************************
// SMARTTHINGS CALLOUT  | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                      V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void callback(const String &message)
{

  term.print("CALLBACK Received message: '");
  term.print(message);
  term.println("' ");

}


void refresh() {
  String var = "NO COMMMAND";

  if (docked) {
    term.println("MEBO docked afd54e");
    var = "MEBO docked";
  }
  else {
    // check that it's docked
    term.println("CHECKING D.C. FLOW STATUS");
    stopcharge();
    delay(2000); // wait
    if (haspower()) {
      term.println("HAS POWER!!!");
      var = "MEBO docked"; // it was not docked but has power while charger off, so we know it's now docked
      docked = true;
      charge();
    }
    else {
      term.println("MEBO NOT docked efc4e _ NOT TURNING ON THE CHARGER");
      var = "MEBO NOTdocked";
    }
  }
  term.println("sending - >: " + String(var));
  //st::Everything::sendSmartString(var);

}
