void loop() {

  if (!StopAll) {
    if (debug) {
      term.println("POWER ==> " + String(digitalRead(POWER)));
      term.println("POWER_B ==> " + String(digitalRead(POWER_B)));
      // term.println("POWER_C ==> "+String(digitalRead(POWER_C)));
      term.println("LED   ==> " + String(digitalRead(LED)));
      wait(1000);
    }
    ledBlink();
    // Blink(2, 100);
    httpHandler();
    WiFiHandler();
    serialHandler();
  } else {
    ArduinoOTA.handle();
  }

  yield();
}

void httpHandler() {
  smartthing.run();
  _server.handleClient();          // JS webpage and wifi terminal
  if (debug) term.handleClient();  // WiFi terminal
  ArduinoOTA.handle();
  yield();
}

void serialHandler() {
  if (!serialStarted && debug) {
    serialStarted = true;
    term.begin(_server);  // conditioned to debug boolean to preserve GPIO1 / LED
    if (debug) term.println("WiFi Terminal ok!");
    Serial.begin(115200);  // setup serial with a baud rate of 115200
    term.link(Serial);
  }
  if (serialStarted && !debug) {
    Serial.end();
    serialStarted = false;
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);
  }
}