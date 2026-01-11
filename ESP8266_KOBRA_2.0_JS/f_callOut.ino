
void messageCallout(String message)
{

  Serial.print("Received message: '");
  Serial.print(message);
  Serial.println("' ");


  if(message.equals("on")){
    On();
  }
  if(message.equals("off")){
    Off();
  }
   if(message.equals("reboot")){
    Reboot();
  }
  if(message.equals("reset")){
    Reset();
  }
  if(message.equals("refresh")){
    refresh();
  }
  if(message.equals("togglebattery")){
    getChargeState();
  }

}

void refresh()
{
  String state = digitalRead(power) ? "switch on" : "switch off";
  smartthing.send(state);
}
