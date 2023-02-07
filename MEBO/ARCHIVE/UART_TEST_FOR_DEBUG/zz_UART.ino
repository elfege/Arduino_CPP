

void UartSend(String cmd) {

  // convert the string to char array
  // Set the Length (with one extra character for the null terminator)
  char Char[cmd.length() + 1];
  // parse the String's characters into a char array
  cmd.toCharArray(Char, sizeof(Char));

  //Serial.print("Sending : "); Serial.println(Char);
  Serial2.write(Char);
  delay(10); // let the serial buffer get emptied
}


String UartReceive() {

  // method that allows to receice a command made of several characters
  // slower but working

  int i = 0;
  String received = "";
  char c;
 
 while (Serial2.available() > 0)
  {
    delay(1);
    c = Serial2.read();
    // Serial.println(c);
       

    // accept only bytes that are the "s" start marker
    if (c == 's') {
      received += c;// parse the S in this string
      
      // build the string for as long as we don't read an "e" end marker and the first char is the start 's' marker
      while (c != 'e' && received.charAt(0) == 's') {
        c = Serial2.read();
        received += c;
      }
           
      Serial.print("received: "); Serial.println(received);
      //received = ""; // clear the string
      //Serial.print("count: "); Serial.println(count);
      break;
    }
  }
  
  return received;
}

