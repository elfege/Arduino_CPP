

void UartSend(String cmdType) {

  unsigned long startFlush = millis();
  while (Serial2.available() && Serial.read() > 0) {// && millis() - startFlush < 20) {
    // flush the Serial buffer
    Serial.print("flushing buffer: "); Serial.println(Serial.read());
    flushing = true;
    loop();
  }
  flushing = false;

  // convert the string to char array
  // Set the Length (with one extra character for the null terminator)
  char Char[cmd.length() + 1];
  // parse the String's characters into a char array
  cmd.toCharArray(Char, sizeof(Char));

  //Serial.print("Sending : "); Serial.println(Char);
  Serial2.write(Char);
  delay(10); // let the serial buffer get emptied

  //Serial2.flush();  //Waits for the transmission of outgoing serial data to complete.
}


String UartReceive(String TheCmd) {

  // method that allows to receice a command made of several characters
  // slower but working

  int i = 0;
  String cmdType = ""; // received cmd in the response
  String received = "";
  String value = "";
  String cmdTypeTemp;
  String TheCmdTemp;

  char c;

  //c = Serial2.read();
  //Serial.print(c);


  while (Serial2.available() > 0)
  {
    delay(1);
    c = Serial2.read();

    // start building the string only when you receive the "&" start marker

    if (c == '&') {
      cmdType += c;
      // between & and * we get the command type
      while (cmdType.charAt(0) == '&' && c != '*') { // * is terminator for command type
        c = Serial2.read();
        cmdType += c; // build the returned command type string
      }
      c = Serial2.read();
      value += c;
      // between + and -  we get the value
      while (value.charAt(0) == '+' && c != '-') { // get the rest of the string to parse the value data '-' being terminator for value and the whole string
        c = Serial2.read();
        value += c; // build the returned value as string
      }
      cmdTypeTemp = cmdType;
      TheCmdTemp = TheCmd;
      cmdTypeTemp.remove(0 , 1); // remove 1 char at index 0
      cmdTypeTemp.remove(cmdTypeTemp.length() - 1 , 1); // remove 1 char at the end
      TheCmdTemp.remove(0 , 1); // remove 1 char at index 0
      TheCmdTemp.remove(TheCmdTemp.length() - 1 , 1); // remove 1 char at the end


      //      Serial << "TheCmd = " << TheCmd << endl;
      //      Serial << "cmdType = " << cmdType << endl;
      //      Serial << "value = " << value << endl;


      if (cmdTypeTemp == TheCmdTemp) {
        //        Serial.println("");
        //        Serial.print("++++++++++++++++++++++ cmdTypeTemp = "); Serial.print(cmdTypeTemp); Serial.print("  TheCmd = "); Serial.println(TheCmdTemp);
        //        Serial.println("");
        delay(1);
        return value;
      }
      else {
        Serial.println("");
        Serial.print("WRONG CMD ............cmdType = "); Serial.print(cmdType); Serial.print("  TheCmd = "); Serial.println(TheCmd);
        Serial.println("");
      }
      break;
    }
    loop();
  }
}
