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


  st::Everything::sendSmartString(var);

}
