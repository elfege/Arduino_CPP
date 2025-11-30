
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
  char this_char[thisMessage.length() + 1];
  thisMessage.toCharArray(this_char, sizeof(this_char));

  int my_integer_data = atoi(this_char);

  // check if this message is a digit
  if (isDigit(message.charAt(0))) {  //   -> TRUE
    term.print("my_integer_data = ");
    term.println(my_integer_data);

    ledcWrite(ACTR_ENABLE_PWM, my_integer_data);
  }
}
