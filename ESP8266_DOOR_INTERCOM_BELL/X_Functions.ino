void PowerOn() {
  term.println("POWER ON");
  lastloop = 0;
  runpower = 1;
  term.println("pressing talk button");
  digitalWrite(TALK, PRESS);  // talk button to activate intercom
  wait(2000);                 // wait n seconds to simulate time needed to answer/say something over the intercom
  term.println("releasing talk button");
  digitalWrite(TALK, RELEASE);  // release talk button

  wait(200);
  term.println("opening DOOR");
  digitalWrite(DOOR, PRESS);  // press the door's button
  wait(3000);
  digitalWrite(DOOR, RELEASE);  // release door's button

  term.println("Looking at videofeed");
  wait(10000);  // leaves time to look at the videofeed

  term.println("now pressing talk buttont to terminate com");
  digitalWrite(TALK, PRESS);  // press talk button to terminate com
  wait(200);
  digitalWrite(TALK, RELEASE);  // release talk button
  wait(200); //wait long enough for voltage measurement to take effect on the next cycle
  runpower = 0;//inform main loop it's done
  
}

void talk() {
  term.println("TALK STATE: " + String(digitalRead(TALK)));
  if (digitalRead(TALK) == PRESS) {
    cmdFromHttp = false;
    term.println("TALK RELEASED");
    digitalWrite(TALK, RELEASE);
    _server.send(200, "text/html", "TALK");
  } else {
    cmdFromHttp = true;
    cmdFromHttpMillis = millis();
    term.println("TALK PRESSED");
    digitalWrite(TALK, PRESS);
    _server.send(200, "text/html", "TALKING");
  }
  Refresh();
}

void open() {
  term.println("DOOR STATE: " + String(digitalRead(DOOR)));
  if (digitalRead(DOOR) == PRESS) {
    cmdFromHttp = false;
    term.println("DOOR RELEASED");
    digitalWrite(DOOR, RELEASE);
    _server.send(200, "text/html", "DOOR");
  } else {
    cmdFromHttp = true;
    cmdFromHttpMillis = millis();
    term.println("DOOR PRESSED");
    digitalWrite(DOOR, PRESS);
    _server.send(200, "text/html", "OPENING");
  }
  Refresh();
}
void close() {
  while (voltage >= Val && lastloop <= 4 && runpower != 0) {

    digitalWrite(TALK, PRESS);  // press talk button to terminate communication
    wait(200);
    digitalWrite(TALK, RELEASE);  // release talk button
    wait(200);
    lastloop++;
    httpHandler();
  }
  Refresh();
}
void on() {
  open();
}
void off() {
  close();
}

void PowerOff() {
  // term.println("digitalRead(TALK) ==> "+String(digitalRead(TALK)));
  //  term.println("digitalRead(DOOR) ==> "+String(digitalRead(DOOR)));
  if (digitalRead(TALK) == PRESS || digitalRead(DOOR) == PRESS) {
    term.println("POWER OFF");
    digitalWrite(TALK, RELEASE);  //    bell is off on RELEASE
    digitalWrite(DOOR, RELEASE);  //
    Refresh();
  }
}

void httpHandler() {
  smartthing.run();
  _server.handleClient();  // JS webpage and wifi terminal
  term.handleClient();     // WiFi terminal
  ArduinoOTA.handle();
  yield();
}

void sendHub(String var)  // called within Arduino C
{
  _server.send(200, "text/html", var);

  term.println("smartthing.send()");
  smartthing.send(var);
}


void Refresh() {
  term.println("REFRESH CALLED!");
  refreshrequest = true;
  getStates(true);
}


void ledBlink() {
  if (millis() - lastMillisBlink >= 1000) {
    digitalWrite(LED, !digitalRead(LED));
    lastMillisBlink = millis();
  }
}
void Blink(int times, int lapse) {
  int c = 0;
  while (c < times) {
    digitalWrite(LED, 1);
    wait(lapse);
    digitalWrite(LED, 0);
    wait(lapse);
    c++;
  }
}

void test() {
  term.println("SIMLATION ON");
  runsim = true;
  

  // _server.send(200, "text/html", "simulator TESTING,");
  // sendToHub("bell active");
  // sendToHub("acceleration active");
  wait(500);
  Refresh();
  term.println("SIMULATION OFF");
  _server.send(200, "text/html", "simulator TEST,");
}

/*****************************************************************************
                              SMARTTHINGS CALLOUT  
*****************************************************************************/
void messageCallout(String message) {
  String mess = "Received message: '" + String(message) + "' ";

  lastMessage = message;  // update global
  term.println(mess);

  if (message.equals("refresh")) {
    Refresh();
  }
  if (message.equals("on")) {
    on();
  }
  if (message.equals("off")) {
    off();
  }
  if (message.equals("open")) {
    open();
  }
  if (message.equals("close")) {
    close();
  }
  if (message.equals("reset")) {
    Reset();
  }
  if (message.equals("talk")) {
    talk();
  }
  if (message.equals("simulator")) {
    test();
  }
}


void wait(int time) {
  unsigned long Start = millis();
  while (millis() - Start < time) {
    httpHandler();
  }
}

void Reset() {
  term.println("*****************************************************************************");
  term.println("                               RESETTING ESP                                ");
  term.println("*****************************************************************************");
  unsigned long s = millis();
  while (millis() - s < 100) {
    httpHandler();
  }
  ESP.restart();
}

void WiFiHandler() {
  if (WiFi.status() != WL_CONNECTED && !wifiLostAware) {
    term.println("******************* LOST WIFI CONNEXION ********************* ");
    WiFiLostMillis = millis();
    wifiLostAware = true;
  }
  if (millis() - WiFiLostMillis > 5000 && wifiLostAware) {
    WiFiConnect();
    if (WiFi.status() != WL_CONNECTED) {
      term.println("******************* ESP DID NOT RECONNECT ********************* ");
      Reset();
    } else {
      term.println("************************************************** ");
      term.println("       ESP SUCCESSFULLY RECONNECTED TO WIFI!       ");
      term.println("************************************************** ");
    }
    wifiLostAware = false;
  }
}
void WiFiConnect() {

  // Configures static IP address
  if (!WiFi.config(ip, gateway, subnet, dnsserver, secondaryDNS)) {
    term.println("STA Failed to configure");
  }

  term.println("\nConnecting to WiFi");
  WiFi.begin(ssidChar, passwordChar);
  unsigned long S = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - S < 10000) {
    delay(500);
    term.print('.');
  }
  if (WiFi.status() != WL_CONNECTED && millis() - S > 10000) {
    term.println("WIFI CONNECTION TO " + String(ssidChar) + " FAILED!");

    term.println("Attempting connection to " + String(ssidChar2));

    if (!WiFi.config(ip, gateway, subnet, dnsserver, secondaryDNS)) {
      term.println("STA Failed to configure");
    }
    WiFi.begin(ssidChar2, passwordChar);
    S = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - S < 10000) {
      delay(500);
      term.print('.');
    }
    if (WiFi.status() != WL_CONNECTED && millis() - S > 10000) {
      term.println("WIFI CONNECTION TO " + String(ssidChar2) + " FAILED TOO!");
      ESP.reset();
    }
  }
}