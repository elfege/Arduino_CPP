

void UartSend(String cmdType) {

  flushdata();

  // convert the string to char array
  // Set the Length (with one extra character for the null terminator)
  char Char[cmd.length() + 1];
  // parse the String's characters into a char array
  cmd.toCharArray(Char, sizeof(Char));

  //term.print("Sending : "); term.println(Char);
  Serial2.write(Char);
  delay(10); // let the serial buffer get emptied

  flushdata();
}


String UartReceive(String TheCmd) {
  // method that allows to receice a command made of several characters
  // slower but working

  int i = 0;
  String cmdType = ""; // received cmd in the response
  String received = "";
  String totalReceived = "";
  String value = "";
  String cmdTypeTemp;
  String TheCmdTemp;

  char c;

  //c = Serial2.read();
  //term.print(c);


  while (Serial2.available() > 0)
  {
    delay(10);
    c = Serial2.read();

    // start building the string only when you receive the "&" start marker


    if (c == '&') {

      cmdType += c;
      totalReceived += c; // build a whole string containing everything for this command

      while (c != '-' && totalReceived.length() < 200)
      {
        // between & and * we get the command type
        // this loop won't stop until finding matching cmds

        cmdTypeTemp = cmdType; // store this value as a temporary variable for future comparisons
        TheCmdTemp = TheCmd;

        while (cmdTypeTemp != TheCmdTemp && totalReceived.length() < 200) /// wait until you got the proper cmd type, or break if it didn't work after reaching a certain lenght
        {
          cmdType = '&' ; // reinitialize to '&' for next attempt

          while (cmdType.charAt(0) == '&' && c != '*' && totalReceived.length() < 200)  // * is terminator for command type
          {
            c = Serial2.read();
            cmdType += c; // build the returned command type string
            totalReceived += c; // continue to build a whole string containing everything for this command

          }
          // remove starter and terminator character so you can compare those two values
          cmdTypeTemp.remove(0 , 1); // remove 1 char at index 0
          cmdTypeTemp.remove(cmdTypeTemp.length() - 1 , 1); // remove 1 char at the end
          TheCmdTemp.remove(0 , 1); // remove 1 char at index 0
          TheCmdTemp.remove(TheCmdTemp.length() - 1 , 1); // remove 1 char at the end

        }
        if (cmdTypeTemp == TheCmdTemp) {
          // term.println("SUCCESS!");
        }
        else {
          term.print("WRONG CMD ............received cmdType = "); term.print(cmdTypeTemp); term.print(" after sending TheCmd = "); term.println(TheCmdTemp);
          //          send it again
          //UartSend(TheCmd);
          term.print("ESP AND ATMega ARE OUT OF SYNC");
          Reset(); // we want to reset because this means ATMega and ESP got out of sync. Temporary workaround, to be resolved at some point.
          break;
        }

        //        Serial.println("totalReceived = " + totalReceived);
        c = Serial2.read();
        value += c;

        // between + and - we get the returned value for this cmd type
        while (value.charAt(0) == '+' && c != '-' && value.length() < 200)
        { // get the rest of the string to parse the value data '-' being terminator for value AND for the whole string
          c = Serial2.read();
          value += c; // parse the characters to a string
          totalReceived += c; // continue to build a whole string containing everything for this command
        }
      }
    }
  }
  //  Serial.println("Returning " + value);
  return value;
}
void UartReceivWTD() {

  int i = 0;
  String received = ""; // clear the received string
  char c;

  while (Serial2.available() > 0)
  {
    delay(1);
    c = Serial2.read();
    // Serial.println(c);

    // accept only bytes that have the "+" start marker
    if (c == '&') {
      received += c;// parse the & in this string

      // build the string for as long as we don't read an "*" end marker and the first char is the start '&' marker
      while (c != '*' && received.charAt(0) == '&') {
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
  if (received.length() != 0 && received.charAt(0) == '&' && received.charAt(received.length() - 1) == '*') {

    Serial.print("Received "); Serial.print(received); Serial.println(" from Atmega");
    if (received.equals("&w*"))
    {
      // this is a watchdog request

      Serial.println("RECVEIVED WATCHDOG REQUEST '" + received + "'");
      Serial.println("SENDING RESPONSE AS '+wOk-");
      // respond in kind
      UartSend("+wOk-");
    }
  }
}

void flushdata()
{
  unsigned long startFlush = millis();
  while (Serial.read() > 0) {// && millis() - startFlush < 20) {
    // flush the Serial buffer
    Serial.print("flushing buffer: "); term.println(Serial.read());
    flushing = true;
    //MainFunctions();
  }
  flushing = false;
}
