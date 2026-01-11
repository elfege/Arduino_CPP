
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

    // accept only bytes that have the "+" start marker
    if (c == '+') {
      received += c;// parse the S in this string

      // build the string for as long as we don't read an "-" end marker and the first char is the start '+' marker
      while (c != '-' && received.charAt(0) == '+') {
        c = Serial2.read();
        received += c;
      }

      //Serial.print("received: "); Serial.println(received);
      //Serial.println("");
      //count++;
      //Serial.print("count: "); Serial.println(count);
      break;
    }
  }

  // call  methods corresponding to this cmd string
  if (received.length() != 0 && received.charAt(0) == '+' && received.charAt(received.length() - 1) == '-') {
    //***********************************************************//
    // Boolean values responses
    //***********************************************************//
    if (received.equals("+wOk-"))
    {
     Serial.println("ESP responded");
     wtdResponseMillis = millis();
    }
    else if (received.equals("+logstrue-"))
    {
      Serial.println("STARTING DEBUG");
      logs = true;
      logsStartedMillis = millis();
    }
    else if (received.equals("+logsfalse-"))
    {
      Serial.println("STOPPING DEBUG");
      logs = false;
    }
    else if (received.equals("+OBS-"))
    {
      response = OBSstr();
    }
    else if (received.equals("+BUMP-"))
    {
      response = BUMPstr();
    }
    else if (received.equals("+BUMP_L-"))
    {
      response = BUMPLstr();
    }
    else if (received.equals("+BUMP_R-"))
    {
      response = BUMPRstr();
    }
    else if (received.equals("+fusup-"))
    {
      response = fusupstr();
    }
    else if (received.equals("+FIR-"))
    {
      response = frontIRstr();
    }
    else if (received.equals("+RRIR-"))
    {
      response = RearRightIRstr();
    }
    else if (received.equals("+rearOBS-"))
    {
      response = rearOBSstr();
    }
    else if (received.equals("+rightOBS-"))
    {
      response = rightOBSstr();
    }
    else if (received.equals("+leftOBS-"))
    {
      response = leftOBSstr();
    }
    else if (received.equals("+leftOBS-"))
    {
      response = leftOBSstr();
    }
    else if (received.equals("+sideOBS-"))
    {
      response = sideOBSstr();
    }
    else if (received.equals("+RearFrontOK-"))
    {
      response = RearFrontOKstr();
    }
    else if (received.equals("+haspower-"))
    {
      response = haspowerstr();
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
    }
    else if (received.equals("+fusupVAL_L-"))
    {
      int val = fusupVAL_L();
      //Serial.print("about to send: "); Serial.println(val);
      response = String(val);
      response = '+' + response + '-';
    }
    else if (received.equals("+fusupVAL_R-"))
    {
      int val = fusupVAL_R();
      //Serial.print("about to send: "); Serial.println(val);
      response = String(val);
      response = '+' + response + '-';
    }
    else if (received.equals("+fusRVAL-"))
    {
      int val = fusRVAL();
      //Serial.print("about to send: "); Serial.println(val);
      response = String(val);
      response = '+' + response + '-';
    }
    else if (received.equals("+fusLVAL-"))
    {
      int val = fusLVAL();
      //Serial.print("about to send: "); Serial.println(val);
      response = String(val);
      response = '+' + response + '-';
    }
    else if (received.equals("+FRIRVAL-"))
    {
      int val = frontIR_RVal();
      //Serial.print("about to send: "); Serial.println(val);
      response = String(val);
      response = '+' + response + '-';
    }
    else if (received.equals("+FLIRVAL-"))
    {
      int val = frontIR_LVal();
      //Serial.print("about to send: "); Serial.println(val);
      response = String(val);
      response = '+' + response + '-';
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
    }
    else if (received.equals("+volts-"))
    {
      int val = getvolts();
      //Serial.print("about to send: "); Serial.println(val);
      response = String(val);
      response = '+' + response + '-';
    }

    //***********************************************************//
    // ARM commands responses
    //***********************************************************//
    else if (received.equals("+stoparm-"))
    {
      stoparm = true;
    }
    else if (received.equals("+foreArmUp-"))
    {
      foreArmUp();

    }
    else if (received.equals("+foreArmDown-"))
    {
      //Serial.print("received: "); Serial.println(received);
      foreArmDown();
      //
    }
    else if (received.equals("+armUp-"))
    {
      armUp();

    }
    else if (received.equals("+armDown-"))
    {
      armDown();

    }
    else if (received.equals("+armUpStep-"))
    {
      armUpStep();

    }
    else if (received.equals("+armDownStep-"))
    {
      armDownStep();

    }
    else if (received.equals("+armCenter-"))
    {
      armCenter();

    }
    else if (received.equals("+wristLeft-"))
    {
      wristLeft();

    }
    else if (received.equals("+wristMiddle-"))
    {
      wristMiddle();

    }
    else if (received.equals("+wristRight-"))
    {
      wristRight();

    }
    else if (received.equals("+wristrightstep-"))
    {
      wristRightStep();

    }
    else if (received.equals("+wristleftstep-"))
    {
      wristLeftStep();

    }
    else if (received.equals("+closeClamp-"))
    {
      closeClamp();

    }
    else if (received.equals("+openClamp-"))
    {
      openClamp();

    }

    String cmdType = received;
    cmdType.remove(0 , 1); // remove 1 char at index 0
    cmdType.remove(cmdType.length() - 1 , 1); // remove 1 char at the end
    cmdType = '&' + cmdType + '*'; // add new markers
    String toSend = cmdType + response;



    if (received.length() > 0) {
      //Serial.println("");
      //      Serial.print("sending "); Serial.print(cmdType);Serial.print(" after receiving: ");Serial.println(received);

      unsigned long startFlush = millis();
      while (Serial2.available() && Serial2.read() > 0) { //  && millis() - startFlush < 20) {
        Serial.print("flushing buffer: "); Serial.println(Serial.read());
      }
      // Serial.print("SENDING: ");Serial.println(toSend);
      UartSend(toSend);
    }
  }
}



void UartSend(String toSend) {

  // convert the string to char array
  // Set the Length (with one extra character for the null terminator)

  unsigned long startFlush = millis();
  while (Serial2.available() && Serial.read() > 0) {// && millis() - startFlush < 20) {
    // flush the Serial buffer
    Serial.print("flushing buffer: "); Serial.println(Serial.read());
  }

  char Char[toSend.length() + 1];
  // parse the String's characters into a char array
  toSend.toCharArray(Char, sizeof(Char));
  //Serial.print("WRITING: ");Serial.println(Char);
  Serial2.write(Char);
  delay(10); // let the Serial2 buffer get emptied
  //response = ""; // clear the response string

}
