void messageCallout(String message)
{
  // if debug is enabled print out the received message

  Serial.print("Received message: '");
  Serial.print(message);
  Serial.println("' ");


  // if message contents equals to 'on' then call on() function
  // else if message contents equals to 'off' then call off() function
  if (message.equals("on")) {
    RUN();
  }
  if(message.equals("refresh")){
    Refresh();
  }
  if (message.equals("off")) {
    Off();
  }
  if (message.equals("load")) {
    loadAcan();
  }
  else if (message.equals("unload")) {
    unload();
  }
  else if (message.equals("lock")) {
    lock();
  }
  else if (message.equals("unlock")) {
    unlock();
  }
  else if (message.equals("opencan")) {
    openCan();
  }
  else if (message.equals("canopdown")) {
    canOpDown();
  }
  else if (message.equals("canopup")) {
    canOpUp();
  }
  else if (message.equals("retractlowerpusher")) {
    retractlowerpusher();
  }
  else if (message.equals("pushlowerpusher")) {
    pushlowerpusher();
  }
  else if (message.equals("retractupperholder")) {
    retractupperholder();
  }
  else if (message.equals("pushupperholder")) {
    pushupperholder();
  }
  else if (message.equals("closegate")) {
    closegate();
  }
  else if (message.equals("opengate")) {
    opengate();
  }
  else if (message.equals("gatehold")) {
    gateholdcan();
  }
  else if (message.equals("extractcan")) {
    extractcan();
  }
  else if (message.equals("run")) {
    RUN();
  }
  else if (message.equals("stop")) {
    millisStop = millis();
    STOP = true;
  }
  else if (message.equals("C1push")) {
    c1.write(180);
  }
  else if (message.equals("C1retract")) {
    c1.write(0);
  }
  else if (message.equals("C2push")) {
    c2.write(180);
  }
  else if (message.equals("C2retract")) {
    c2.write(0);
  }
  else if (message.equals("testload")) {
    testLoadAllCans();
  }

  else if (message.equals("reset")) {
    Reset();
  }
}

void Refresh(){
  Serial.println("REFRESH CALLED");
  smartthing.send("switch " + pwstate);
  String feederStatus = feederEmpty ? "status empty":"status full"; // not very accurate, need to add a sensor on C2 chamber
  smartthing.send(feederStatus);
}
