
void mainHandler()
{
  // WiFiHandler();
  httpHandler();
  blinker();
  yield();
}

void sendToHub(String var)
{
  st::Everything::sendSmartString(var);
}

boolean level_is_low()
{
  return digitalRead(sensor); // 1 = no water: input_pullup so 0 when up because sensor closes circuit (so current goes to GND)
}

void httpHandler()
{
  st::Everything::run();
  _server.handleClient();
  term.handleClient(); // WiFi terminal
  yield();
}

void blinker()
{
  // heartbeat-like blinking as long as water level is high
  
  if (!level_is_low())
  {

    unsigned long currentMillis = millis();

    switch (currentState)
    {
    case LUB:
      if (currentMillis - previousMillisBlink >= heartbeatInterval1)
      {
        digitalWrite(LED, HIGH); // LED ON
        previousMillisBlink = currentMillis;
        currentState = PAUSE_AFTER_LUB;
      }
      break;

    case PAUSE_AFTER_LUB:
      if (currentMillis - previousMillisBlink >= heartbeatInterval2)
      {
        digitalWrite(LED, LOW); // LED OFF
        previousMillisBlink = currentMillis;
        currentState = DUB;
      }
      break;

    case DUB:
      if (currentMillis - previousMillisBlink >= heartbeatInterval3)
      {
        digitalWrite(LED, HIGH); // LED ON
        previousMillisBlink = currentMillis;
        currentState = PAUSE_AFTER_DUB;
      }
      break;

    case PAUSE_AFTER_DUB:
      if (currentMillis - previousMillisBlink >= pauseInterval)
      {
        digitalWrite(LED, LOW); // LED OFF
        previousMillisBlink = currentMillis;
        currentState = LUB;
      }
      break;
    }
  }
  else{
    digitalWrite(LED, 1); // keep led off when level is low
  }
}

void messageCallout(String message)
{

  term.print("Received message: '");
  term.print(message);
  term.println("' ");
}