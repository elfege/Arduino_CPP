void onOff() {
  CMD(power);
}
void nextMode()
{
  CMD(modeSw);
  delay(200);
  currentMode = getCurrentMode();
  term.println("currentMode = " + currentMode);

}
void cool()
{
  STOP = false;
  int S = 1000;
  unsigned long s = millis();
  term.println("analogRead(cooling) = "+String(analogRead(cooling)));
  int READ = analogRead(cooling);
  while (READ < S && millis() - s < 30000 && !STOP)
  {
    CMD(modeSw);
    delay(100);
    READ = analogRead(cooling);
    master();
  }
  if (analogRead(cooling) > S) {
    currentMode = "cool";
    term.println("cool command successful!");
  }
  else
  {
    term.println("FAILED TO SEND CMD COOL");
    term.println("returned value: " + currentMode);
  }
}
String getCurrentMode() {
  int S = 500;
  oneDebug = true;
  logs();
  String result = analogRead(cooling) > S ? "cool" : analogRead(heating) > S ? "heat" : analogRead(dehumid) > S ? "dehumidifier" : analogRead(fanonly) > S ? "fanOnly" : "ERROR";

  return result;
}
String getFanMode() {
  int S = 500;
  String fanMode = "ERROR";
  return fanMode = analogRead(fanauto) > S ? "auto" : analogRead(fanlow) > S ? "low" : analogRead(fanmed) > S ? "med" : analogRead(fanhigh) > S ? "fanhigh" : "ERROR";

}
void stopRequest() {
  STOP = true;
}
void lower() {
  term.println("NEXT MODE CMD");
  CMD(lowerTemp);
}
void higher() {
  CMD(raiseTemp);
}
void FANSPEED()
{
  CMD(fanSpeed);
}

void CMD(int port) {
  
  digitalWrite(port, 0);
  delay(500);
  digitalWrite(port, 1);
}




//*****************************************************************************
// HUB CALLOUT  | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                      V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void messageCallout(String message)
{

  String receivedMsg = "Received: '" + message + "' ";
  term.print(receivedMsg);
  term.print(message);


}
