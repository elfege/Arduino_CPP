
int frontIRVal() {
  cmd = "+FIRVAL-";
  int t = 0;
  String received;
  
  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);
  
  Start = millis();
  while (!Serial2.available())
  {
    ;; //loop();
  }

  received = UartReceive(cmd);
  //Serial.print("received before removal = "); Serial.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //Serial.print("received after removal = "); Serial.println(received);
  int result = received.toInt();
  //Serial.print("received to Int = "); Serial.println(received);

  return result;
}

int RRIRVal() {
  cmd = "+RRIRVal-";
  int t = 0;
  String received;

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);

  Start = millis();
  while (!Serial2.available())
  {
    ;; //loop();
  }

  received = UartReceive(cmd);
  //Serial.print("received before removal = "); Serial.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //Serial.print("received after removal = "); Serial.println(received);
  int result = received.toInt();
  //Serial.print("received to Int = "); Serial.println(received);

  return result;
}

int RLIRVal() {
  cmd = "+RLIRVal-";
  int t = 0;
  String received;

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);

  Start = millis();
  while (!Serial2.available())
  {
    ;; //loop();
  }

  received = UartReceive(cmd);
  //Serial.print("received before removal = "); Serial.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //Serial.print("received after removal = "); Serial.println(received);
  int result = received.toInt();
  //Serial.print("received to Int = "); Serial.println(received);

  return result;
}

int rightIRVal() {
  cmd = "+RIRVal-";
  int t = 0;
  String received;

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);

  Start = millis();
  while (!Serial2.available())
  {
    ;; //loop();
  }

  received = UartReceive(cmd);
  //Serial.print("received before removal = "); Serial.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //Serial.print("received after removal = "); Serial.println(received);
  int result = received.toInt();
  //Serial.print("received to Int = "); Serial.println(received);

  return result;
}

int leftIRVal() {
  cmd = "+LIRVal-";
  int t = 0;
  String received;

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);

  Start = millis();
  while (!Serial2.available())
  {
    ;; //loop();
  }

  received = UartReceive(cmd);
  //Serial.print("received before removal = "); Serial.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //Serial.print("received after removal = "); Serial.println(received);
  int result = received.toInt();
  //Serial.print("received to Int = "); Serial.println(received);

  return result;
}

int fusupVAL() {
  cmd = "+fusupVAL-";
  int t = 0;
  String received;

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.

  UartSend(cmd);

  Start = millis();
  while (!Serial2.available())
  {
    ;; //loop();
  }

  received = UartReceive(cmd);

  //Serial.print("received before removal = "); Serial.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //Serial.print("received after removal = "); Serial.println(received);
  int result = received.toInt();
  //Serial.print("received to Int = "); Serial.println(received);

  return result;
}

int fusRVAL() {
  cmd = "+fusRVAL-";
  int t = 0;
  String received;

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);

  Start = millis();
  while (!Serial2.available())
  {
    ;; //loop();
  }

  received = UartReceive(cmd);
  //Serial.print("received before removal = "); Serial.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //Serial.print("received after removal = "); Serial.println(received);
  int result = received.toInt();
  //Serial.print("received to Int = "); Serial.println(received);

  return result;
}

int fusLVAL() {
  cmd = "+fusLVAL-";
  int t = 0;
  String received;

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);
  Start = millis();
  while (!Serial2.available())
  {
    ;; //loop();
  }

  received = UartReceive(cmd);
  //Serial.print("received before removal = "); Serial.println(received);

  // remove the markers
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //Serial.print("received after removal = "); Serial.println(received);
  int result = received.toInt();
  //Serial.print("received to Int = "); Serial.println(received);

  return result;
}
