//******************************************************************************************
//st::Everything::callOnMsgSend() optional callback routine.  This is a sniffer to monitor
//    data being sent to ST.  This allows a user to act on data changes locally within the
//    Arduino sktech.
//******************************************************************************************

void messageCallout(String message) {

  term.print("Received message: '");
  term.print(message);
  term.println("' ");

  ///  see if there is a request for a different speed

  String thisMessage = message;

  // check if this message is a digit
  if (isDigit(message.charAt(0))) {  //   -> TRUE

    char this_char[thisMessage.length() + 1];
    thisMessage.toCharArray(this_char, sizeof(this_char));

    int my_integer_data = atoi(this_char);

    term.print("my_integer_data = ");
    term.println(my_integer_data);

    ledcWrite(COSINUS_PWM, my_integer_data);
  }

  else if (message.equals("on")) {
    feed();
  }
  else if (message.equals("clean")){
    cleanOpener();
  }
  else if (message.equals("pause")) {
    paused = !paused;
  }
  else if (message.equals("feed")) {
    feed();
  } else if (message.equals("off")) {
    Reset();
  } else if (message.equals("refresh")) {
    term.println("calling refresh()");
    Refresh();
  } else if (message.equals("load")) {
    load();
  } else if (message.equals("unload")) {
    extract();
  } else if (message.equals("lock")) {
    lock();
  } else if (message.equals("unlock")) {
    unlock();
  } else if (message.equals("push")) {
    push();
  } else if (message.equals("pull")) {
    pull();
  } else if (message.equals("verticalpull")) {
    verticalPull();
  } else if (message.equals("verticalpush")) {
    verticalPush();
  } else if (message.equals("verticaldownstep")) {
    verticalDown();
    delay(10);
    verticalStop();
  } else if (message.equals("verticalupstep")) {
    verticalUp();
    delay(10);
    verticalStop();
  } else if (message.equals("cosinusplus")) {
    cosinusPush();
    delay(10);
    ledcWrite(pwmChannelCos, 0);
  } else if (message.equals("cosinuspush")) {
    cosinusPush();
  } else if (message.equals("cosinuspull")) {
    cosinusPull();
  } else if (message.equals("cosinusminus")) {
    cosinusPull();
    delay(10);
    ledcWrite(pwmChannelCos, 0);
  } else if (message.equals("toggledebug")) {
    Logs = !Logs;
    String response = Logs ? "DEBUG ENABLED" : "DEBUG DISABLED";
    sendToHub(response);
  } else if (message.equals("opencan")) {
    openCan();
  } else if (message.equals("reset")) {
    Reset();
  }

  /* else if (message.equals("")) {
    ();
    } else if (message.equals("")) {
    ();
    }*/
}

void Refresh() {
  Serial.println("REFRESH CALLED");
  refreshrequest = true;
  getStates(true);
}
