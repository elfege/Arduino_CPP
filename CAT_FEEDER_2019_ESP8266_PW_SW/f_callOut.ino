
void messageCallout(String message)
{

  Serial.print("Received message: '");
  Serial.print(message);
  Serial.println("' ");


  if (message.equals("on")) {
    On();
  }
  if (message.equals("off")) {
    Off();
  }
  if (message.equals("reboot")) {
    Reboot();
  }
  if (message.equals("reset")) {
    Reset();
  }
  if (message.equals("refresh")) {
    refresh();
  }
}

void refresh()
{
  String state = digitalRead(power) ? "switch on" : "switch off";
  //smartthing.send(state);
}

void sendGET(String val) //client function to send/receive GET request data.
{
  Serial.println("sendGET " + val);
  WiFiClient client;
  byte myserver[] = {192, 168, 10, 240};

  if (client.connect(myserver, 80)) {  //starts client connection, checks for connection

    Serial.println("Sending packet: 'Get/" + String(val) + "'");
    client.println("GET /" + String(val) + " HTTP/1.0"); //+ String(val)); //download text
    client.println(); //end of get request
  }
  else {
    Serial.println("connection failed"); //error message if no client connect
    Serial.println();
  }

  String str = "";
  unsigned long St = millis();
  while (client.connected() && !client.available() && millis() - St < 4000)
  {
    delay(1);
  }
  if (client.connected() && !client.available())
  {
    Serial.println("CLIENT DIDN'T RESPOND");
  }
  while (client.available()) { //connected or data available
    char c = client.read(); //gets byte from ethernet buffer
    Serial.print(c); //prints byte to serial monitor
    str += c;
  }
  Serial.print("RECEIVED FROM ATMEGA: "); Serial.println(str);

  client.stop(); //stop client
  Serial.println("sendGet client disconnected");
  Serial.println("==================");

}
