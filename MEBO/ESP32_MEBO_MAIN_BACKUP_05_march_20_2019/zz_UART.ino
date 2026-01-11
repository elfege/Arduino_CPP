

void UartSend(String cmdType) {

  flushdata();

  // convert the string to char array
  // Set the Length (with one extra character for the null terminator)
  char Char[cmd.length() + 1];
  // parse the String's characters into a char array
  cmd.toCharArray(Char, sizeof(Char));

  //Serial.print("Sending : "); Serial.println(Char);
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
  //Serial.print(c);


  while (Serial2.available() > 0)
  {
    delay(10);
    c = Serial2.read();

    // start building the string only when you receive the "&" start marker


    if (c == '&') {

      cmdType += c;
      totalReceived += c; // build a whole string containing everything for this command

      while (c != '-' && totalReceived.length() < 500)
      {

        // between & and * we get the command type
        // this loop won't stop until finding matching cmds

        cmdTypeTemp = cmdType; // store this value as a temporary variable for future comparisons
        TheCmdTemp = TheCmd;

        while (cmdTypeTemp != TheCmdTemp && totalReceived.length() < 200) /// wait until you got the proper cmd type, or break if it didn't work after reaching a certain lenght
        {
          cmdType = '&' ; // reinitialize to '&' for next attempt

          while (cmdType.charAt(0) == '&' && c != '*')  // * is terminator for command type
          {
            c = Serial2.read();
            cmdType += c; // build the returned command type string
            totalReceived += c; // continue to build a whole string containing everything for this command
            //Serial.println(totalReceived);


          }
          // remove starter and terminator character so you can compare those two values
          cmdTypeTemp.remove(0 , 1); // remove 1 char at index 0
          cmdTypeTemp.remove(cmdTypeTemp.length() - 1 , 1); // remove 1 char at the end
          TheCmdTemp.remove(0 , 1); // remove 1 char at index 0
          TheCmdTemp.remove(TheCmdTemp.length() - 1 , 1); // remove 1 char at the end

        }
        if (cmdTypeTemp == TheCmdTemp) {
         // Serial.println("SUCCESS!");
        }
        else {
          Serial.print("WRONG CMD ............received cmdType = "); Serial.print(cmdTypeTemp); Serial.print("after sending TheCmd = "); Serial.println(TheCmdTemp);
//          send it again
          UartSend(TheCmd);
          break;
        }

        c = Serial2.read();
        value += c;

        // between + and - we get the returned value for this cmd type
        while (value.charAt(0) == '+' && c != '-') { // get the rest of the string to parse the value data '-' being terminator for value AND for the whole string
          c = Serial2.read();
          value += c; // parse the characters to a string
          totalReceived += c; // continue to build a whole string containing everything for this command
          //Serial.println(totalReceived);
          
        }
      }
    }
  }
  return value;
}

void flushdata()
{
  unsigned long startFlush = millis();
  while (Serial.read() > 0) {// && millis() - startFlush < 20) {
    // flush the Serial buffer
    Serial.print("flushing buffer: "); Serial.println(Serial.read());
    flushing = true;
    //loop();
  }
  flushing = false;
}
