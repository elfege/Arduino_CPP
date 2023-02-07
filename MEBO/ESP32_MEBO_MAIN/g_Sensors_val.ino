
int frontIR_LVal() {
  cmd = "+FLIRVAL-";
  int t = 0;
  String received;

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);

  Start = millis();
  while (!Serial2.available())
  {
    ;; //MainFunctions();
  }

  received = UartReceive(cmd);
  //term.print("received before removal = "); term.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //term.print("received after removal = "); term.println(received);
  int result = received.toInt();
  //term.print("received to Int frontIR_LVal= "); term.println(received);

  return result;
}
int frontIR_RVal() {
   cmd = "+FRIRVAL-";
  int t = 0;
  String received;

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);

  Start = millis();
  while (!Serial2.available())
  {
    ;; //MainFunctions();
  }

  received = UartReceive(cmd);
  //term.print("received before removal = "); term.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //term.print("received after removal = "); term.println(received);
  int result = received.toInt();
  //term.print("received to Int frontIR_LVal= "); term.println(received);

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
    ;; //MainFunctions();
  }

  received = UartReceive(cmd);
  //term.print("received before removal = "); term.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //term.print("received after removal = "); term.println(received);
  int result = received.toInt();
  //term.print("received to Int = "); term.println(received);

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
    ;; //MainFunctions();
  }

  received = UartReceive(cmd);
  //term.print("received before removal = "); term.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //term.print("received after removal = "); term.println(received);
  int result = received.toInt();
  //term.print("received to Int = "); term.println(received);

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
    ;; //MainFunctions();
  }

  received = UartReceive(cmd);
  //term.print("received before removal = "); term.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //term.print("received after removal = "); term.println(received);
  int result = received.toInt();
  //term.print("received to Int = "); term.println(received);

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
    ;; //MainFunctions();
  }

  received = UartReceive(cmd);
  //term.print("received before removal = "); term.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //term.print("received after removal = "); term.println(received);
  int result = received.toInt();
  //term.print("received to Int = "); term.println(received);

  return result;
}

int fusupVAL() {
 return (fusupVAL_L() + fusupVAL_R()) / 2;
}

int fusupVAL_L() {
  cmd = "+fusupVAL_L-";
  int t = 0;
  String received;

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.

  UartSend(cmd);

  Start = millis();
  while (!Serial2.available())
  {
    ;; //MainFunctions();
  }

  received = UartReceive(cmd);

  //term.print("received before removal = "); term.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //term.print("received after removal = "); term.println(received);
  int result = received.toInt();
  //term.print("received to Int = "); term.println(received);

  return result;
}

int fusupVAL_R() { // THIS IS ALSO THE SENSOR ATTACHED TO THE FOREARM
  cmd = "+fusupVAL_R-";
  int t = 0;
  String received;

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.

  UartSend(cmd);

  Start = millis();
  while (!Serial2.available())
  {
    ;; //MainFunctions();
  }

  received = UartReceive(cmd);

  //term.print("received before removal = "); term.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //term.print("received after removal = "); term.println(received);
  int result = received.toInt();
  //term.print("received to Int = "); term.println(received);

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
    ;; //MainFunctions();
  }

  received = UartReceive(cmd);
  //term.print("received before removal = "); term.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //term.print("received after removal = "); term.println(received);
  int result = received.toInt();
  //term.print("received to Int = "); term.println(received);

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
    ;; //MainFunctions();
  }

  received = UartReceive(cmd);
  //term.print("received before removal = "); term.println(received);

  // remove the markers
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //term.print("received after removal = "); term.println(received);
  int result = received.toInt();
  //term.print("received to Int = "); term.println(received);

  return result;
}

float getvolts() {

  boolean WasTurnedOff = false;
  if (digitalRead(CHARGERelay) && !docking)
    // reversed logic on this relay
    // !docking is to prevent false positives in the boolean "haspower()"
  {
    digitalWrite(CHARGERelay, 0);
    WasTurnedOff = true;

    // set a delay to leave enough time to read the value
    unsigned long currentMStart = millis();
    while (millis() - currentMStart < 10) {
      MainFunctions(); // allow other requests to work in the mean time
    }
  }
  cmd = "+volts-";
  int t = 0;
  String received;

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);

  Start = millis();
  while (!Serial2.available())
  {
    ;; //MainFunctions();
  }

  received = UartReceive(cmd);
  //term.print("received before removal = "); term.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end

  //term.print("received analog signal from AtMega = "); term.println(received);
  float volts = received.toFloat() * (15 / 600.0);

  //mapfloat(received.toFloat(), 0.0, 1023.0, 0.0, 21.0);
  //term.print(volts); term.println(" Volts");

  String state = String(volts);
  _server.send(200, "text/plain", state);

  if (!digitalRead(CHARGERelay) && WasTurnedOff)
  {
    digitalWrite(CHARGERelay, 1); // turn off
  }

  return volts;
}

boolean haspower() {
  docking = true; // prevents power check, which turns on the switch, which would give a false positive
  
  return VoltsCheckHasPower() > 0;

}

int VoltsCheckHasPower() {
  cmd = "+volts-";
  int t = 0;
  String received;

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);

  Start = millis();
  while (!Serial2.available())
  {
    ;; //MainFunctions();
  }

  received = UartReceive(cmd);
  //term.print("received before removal = "); term.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end

  //term.print("received analog signal from AtMega = "); term.println(received);
  float volts = mapfloat(received.toFloat(), 16.0, 1023.0, 0.0, 21.0);
  //term.print(volts); term.println(" Volts");

  String state = String(volts);
  _server.send(200, "text/plain", state);

  return volts;
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

boolean ChargeIsOn()
{
  return !digitalRead(CHARGERelay);
}
