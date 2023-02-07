

void UartSend(String cmd) {

  // convert the string to char array
  // Set the Length (with one extra character for the null terminator)
  char Char[cmd.length() + 1];
  // parse the String's characters into a char array
  cmd.toCharArray(Char, sizeof(Char));

  //Serial.print("Sending : "); Serial.println(Char);
  Serial2.write(Char);
  delay(100); // let the serial buffer get emptied
}


void UartReceive() {

  // method that allows to receice a command made of several characters
  // slower but working

  int i = 0;
  String received = "";
  char c;

  if (Serial2.available() > 0) {
    delay(1); //allows time to receive

    c = Serial2.read();
    if (c == 's') {
      received += c;

      for (Serial2.available() > 0 ; (c != 'e' && i < 6); i++) {
        //delay(1);
        c = Serial2.read();
        // Serial.println(c);
        received += c;
        //Serial.println(received);

        // accept only packets that have start and end markers
        if (received.length() > 0 && received.charAt(0) == 's' && received.charAt(received.length() - 1) == 'e' ) {
          //received.remove(0 , 0); // remove the empty character to facilitate further processing
          //          Serial.println("");
          //          Serial.print("Received: "); Serial.println(received); // print out the result

          identifyAndExec(received);// sent as a string
          count++;
          Serial.print("count: "); Serial.println(count);
          break;

        }
      }
    }
  }
}


String identifyAndExec(String str)
{

  if (str == "s0000e") {
    fwd(reqSpeed, dista);

  } if (str == "s0001e") {
    rvs(reqSpeed, dista);

  } if (str == "s0002e") {
    Serial.print("COMMAND #"); Serial.println(str);

  } if (str == "s0003e") {
    Serial.print("COMMAND #"); Serial.println(str);

  } if (str == "s0004e") {
    Serial.print("COMMAND #"); Serial.println(str);

  } if (str == "s0005e") {
    Serial.print("COMMAND #"); Serial.println(str);

  } if (str == "s0006e") {
    Serial.print("COMMAND #"); Serial.println(str);

  } if (str == "s0007e") {
    Serial.print("COMMAND #"); Serial.println(str);

  } if (str == "s0008e") {
    Serial.print("COMMAND #"); Serial.println(str);

  }
}

String Acknowledge(String str)
{

  if (str == "a0000e") {
    Serial.print("COMMAND #"); Serial.println(str);
  } if (str == "a0001e") {
    Serial.print("COMMAND #"); Serial.println(str);

  } if (str == "a0002e") {
    Serial.print("COMMAND #"); Serial.println(str);

  } if (str == "a0003e") {
    Serial.print("COMMAND #"); Serial.println(str);

  } if (str == "a0004e") {
    Serial.print("COMMAND #"); Serial.println(str);

  } if (str == "a0005e") {
    Serial.print("COMMAND #"); Serial.println(str);

  } if (str == "s0006e") {
    Serial.print("COMMAND #"); Serial.println(str);

  } if (str == "a0007e") {
    Serial.print("COMMAND #"); Serial.println(str);

  } if (str == "a0008e") {
    Serial.print("COMMAND #"); Serial.println(str);

  }
}



