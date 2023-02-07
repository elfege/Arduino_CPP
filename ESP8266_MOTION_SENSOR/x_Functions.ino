void messageCallout(String message)
{

  Serial.print("Received message: '");
  Serial.print(message);
  Serial.println("' ");

  // if message contents equals to 'on' then call on() function
  // else if message contents equals to 'off' then call off() function

  if (message.equals("refresh"))
  {
    Refresh();
  }
  if (message.equals("reset"))
  {
    Reset();
  }
}

void Refresh() {

  if (!active) {
    Serial.println("reporting motion inactive");
    smartthing.send("motion inactive");
  }
  delay(500);
  PING();
}


void Blink(int times, int lapse) {
  int c = 0;
  while (c < times) {
    digitalWrite(LED, 1);
    delay(lapse);
    digitalWrite(LED, 0);
    delay(lapse);
    c++;
  }
}


void Reset() {
  Serial.println("RESETING CONTROLLER");
  smartthing.send("reset reset");
  
  delay(1000);
  // ESP.restart(); // this shit triggers hardware watchdog so we need to use the reset pin instead of this command
 
  digitalWrite(rst, 0);
}

void PING() {
  if (Ping.ping(remote_ip)) {
    Serial.println("ping successful!!");
    smartthing.send("ping successful");
  } else {
    Serial.println("Error :(");
    smartthing.send("ping failed");
    delay(1000);
    Reset();
  }

}

