void on() {
  digitalWrite(POWER, HIGH);
  digitalWrite(POWER_B, HIGH);
  // digitalWrite(POWER_C, HIGH);
}

void off() {
  digitalWrite(POWER, LOW);
  digitalWrite(POWER_B, LOW);
  // digitalWrite(POWER_C, LOW);
}




void sendHub(String var)  // called within Arduino C
{
  _server.send(200, "text/html", var);
  if(debug) term.println("smartthing.send()");
  smartthing.send(var);
}
void Refresh() {
  if(debug) term.println("REFRESH CALLED!");
  refreshrequest = true;
  getStates(true);
}
void ledBlink() {
  int duration = digitalRead(POWER) ? 500 : 5000;
  if (millis() - lastMillisBlink >= duration) {
    digitalWrite(LED, 1);
    wait(100);
    digitalWrite(LED, 0);
    lastMillisBlink = millis();
  }
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


/*****************************************************************************
                              SMARTTHINGS CALLOUT  
*****************************************************************************/
void messageCallout(String message) {
  String mess = "Received message: '" + String(message) + "' ";

  lastMessage = message;  // update global
  if(debug) term.println(mess);

  if (message.equals("refresh")) {
    Refresh();
  }
  if (message.equals("on")) {
    on();
  }
  if (message.equals("off")) {
    off();
  }

  if (message.equals("reset")) {
    Reset();
  }
}


void wait(int time) {
  unsigned long Start = millis();
  while (millis() - Start < time) {
    httpHandler();
  }
}

void Reset() {
  serialStarted = false;
  debug = true;
  serialHandler();
  if(debug) term.println("*****************************************************************************");
  if(debug) term.println("                               RESETTING ESP                                ");
  if(debug) term.println("*****************************************************************************");
  unsigned long s = millis();
  while (millis() - s < 100) {
    httpHandler();
  }
  hardReset();
}

void hardReset() {
  ESP.reset();  // ESP.reset is hard reset (not ESP.restart())
}

void WiFiHandler() {
  if (WiFi.status() != WL_CONNECTED && !wifiLostAware) {
    if(debug) term.println("******************* LOST WIFI CONNEXION ********************* ");
    WiFiLostMillis = millis();
    wifiLostAware = true;
  }
  if (millis() - WiFiLostMillis > 5000 && wifiLostAware) {
    WiFiConnect();
    if (WiFi.status() != WL_CONNECTED) {
      if(debug) term.println("******************* ESP DID NOT RECONNECT ********************* ");
      Reset();
    } else {
      if(debug) term.println("************************************************** ");
      if(debug) term.println("       ESP SUCCESSFULLY RECONNECTED TO WIFI!       ");
      if(debug) term.println("************************************************** ");
    }
    wifiLostAware = false;
  }
}
void WiFiConnect() {

  // Configures static IP address
  if (!WiFi.config(ip, gateway, subnet, dnsserver, secondaryDNS)) {
    if(debug) term.println("STA Failed to configure");
  }

  if(debug) term.println("\nConnecting to WiFi");
  WiFi.begin(ssidChar, passwordChar);
  unsigned long S = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - S < 10000) {
    delay(500);
    if(debug) term.print('.');
  }
  if (WiFi.status() != WL_CONNECTED && millis() - S > 10000) {
    if(debug) term.println("WIFI CONNECTION TO " + String(ssidChar) + " FAILED!");

    if(debug) term.println("Attempting connection to " + String(ssidChar2));

    if (!WiFi.config(ip, gateway, subnet, dnsserver, secondaryDNS)) {
      if(debug) term.println("STA Failed to configure");
    }
    WiFi.begin(ssidChar2, passwordChar);
    S = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - S < 10000) {
      delay(500);
      if(debug) term.print('.');
    }
    if (WiFi.status() != WL_CONNECTED && millis() - S > 10000) {
      if(debug) term.println("WIFI CONNECTION TO " + String(ssidChar2) + " FAILED TOO!");
      ESP.reset();
    }
  }
}


