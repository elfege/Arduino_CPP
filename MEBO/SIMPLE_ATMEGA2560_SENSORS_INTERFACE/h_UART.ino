
void UartReceive() {

  // method that allows to receive a command made of several characters
  // slower but working
  bool done = false;
  int i = 0;
  String received = ""; // clear the received string
  char c;

  while (Serial2.available() > 0)
  {
    delay(1);
    c = Serial2.read();
    // Serial.println(c);

    // accept only bytes that are the "+" start marker
    if (c == '+') {
      received += c;// parse the S in this string

      // build the string for as long as we don't read an "-" end marker and the first char is the start '+' marker
      while (c != '-' && received.charAt(0) == '+') {
        c = Serial2.read();
        received += c;
      }

      Serial.print("received: "); Serial.println(received);
      //count++;
      //Serial.print("count: "); Serial.println(count);
      break;
    }
  }

  // identify command

  if (received.length() != 0 && received.charAt(0) == '+' && received.charAt(received.length() - 1) == '-') {


    //***********************************************************//
    // Boolean values responses
    //***********************************************************//
    if (received.equals("+OBS-"))
    {
      response = OBSstr();

      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+BUMP-"))
    {
      response = BUMPstr();

      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+BUMP_L-"))
    {
      response = BUMPLstr();

      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+BUMP_R-"))
    {
      response = BUMPRstr();

      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+fusup-"))
    {
      response = fusupstr();

      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+FIR-"))
    {
      response = frontIRstr();

      //Serial.print("sent response: "); Serial.println(response);
    }

    else if (received.equals("+RRIR-"))
    {
      response = RearRightIRstr();

      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+rearOBS-"))
    {
      response = rearOBSstr();

      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+rightOBS-"))
    {
      response = rightOBSstr();

      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+leftOBS-"))
    {
      response = leftOBSstr();

      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+leftOBS-"))
    {
      response = leftOBSstr();

      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+sideOBS-"))
    {
      response = sideOBSstr();

      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+RearFrontOK-"))
    {
      response = RearFrontOKstr();

      //Serial.print("sent response: "); Serial.println(response);
    }

    //***********************************************************//
    // Integer values responses
    //***********************************************************//
    else if (received.equals("+fusupVAL-"))
    {
      int val = fusupVAL();
      //Serial.print("about to send: "); Serial.println(val);
      response = String(val);
      response = '+' + response + '-';

      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+fusRVAL-"))
    {
      int val = fusRVAL();
      //Serial.print("about to send: "); Serial.println(val);
      response = String(val);
      response = '+' + response + '-';

      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+fusLVAL-"))
    {
      int val = fusLVAL();
      //Serial.print("about to send: "); Serial.println(val);
      response = String(val);
      response = '+' + response + '-';

      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+FIRVAL-"))
    {
      int val = frontIRVal();
      //Serial.print("about to send: "); Serial.println(val);
      response = String(val);
      response = '+' + response + '-';

      //Serial.print("sent response: "); Serial.println(response);
    }

    else if (received.equals("+RRIRVal-"))
    {
      int val = RRIRVal();
      //Serial.print("about to send: "); Serial.println(val);
      response = String(val);
      response = '+' + response + '-';

      //Serial.print("+ent response: "); Serial.println(response);
    }
    else if (received.equals("+RLIRVal-"))
    {
      int val = RLIRVal();
      //Serial.print("about to send: "); Serial.println(val);
      response = String(val);
      response = '+' + response + '-';

      //Serial.print("+ent response: "); Serial.println(response);
    }
    else if (received.equals("+RIRVal-"))
    {
      int val = rightIRVal();
      //Serial.print("about to send: "); Serial.println(val);
      response = String(val);
      response = '+' + response + '-';

      //Serial.print("+ent response: "); Serial.println(response);
    }
    else if (received.equals("+LIRVal-"))
    {
      int val = leftIRVal();
      //Serial.print("about to send: "); Serial.println(val);
      response = String(val);
      response = '+' + response + '-';

      //Serial.print("sent response: "); Serial.println(response);
    }

    //***********************************************************//
    // ARM commands responses
    //***********************************************************//
    else if (received.equals("+foreArmUp-"))
    {
      foreArmUp();
      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+foreArmDown-"))
    {
      foreArmDown();
      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+armUp-"))
    {
      armUp();
      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+armDown-"))
    {
      armDown();
      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+wristLeft-"))
    {
      wristLeft();
      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+wristRight-"))
    {
      wristRight();
      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+closeClamp-"))
    {
      closeClamp();
      //Serial.print("sent response: "); Serial.println(response);
    }
    else if (received.equals("+openClamp-"))
    {
      openClamp();
      //Serial.print("sent response: "); Serial.println(response);
    }

    String cmdType = received;
    cmdType.remove(0 , 1); // remove 1 char at index 0
    cmdType.remove(cmdType.length() - 1 , 1); // remove 1 char at the end
    cmdType = '&' + cmdType + '*'; // add new markers
    String toSend = cmdType + response;

    if (received.length() > 0) {
      Serial.print("sending "); Serial.print(toSend);

      unsigned long startFlush = millis();
      while (Serial2.available() && Serial2.read() > 0){ //  && millis() - startFlush < 20) {

        Serial.print("flushing buffer: "); Serial.println(Serial.read());

      }
      UartSend(toSend);
    }
  }
}



void UartSend(String toSend) {

  // convert the string to char array
  // Set the Length (with one extra character for the null terminator)


  char Char[toSend.length() + 1];
  // parse the String's characters into a char array
  toSend.toCharArray(Char, sizeof(Char));
  Serial2.write(Char);
  delay(10); // let the Serial2 buffer get emptied
  //response = ""; // clear the response string

}
