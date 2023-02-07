void loop() {

  ArduinoOTA.handle();

  checkWiFi();

  httpHandler();

  mainHandler();

  delay(10);
  yield();
}

void httpHandler() {
  _server.handleClient();
  term.handleClient();  // WiFi terminal
}

void checkWiFi() {
  if (WiFi.status() != WL_CONNECTED && !wifiLostAware) {
    Serial.println("******************* LOST WIFI CONNEXION ********************* ");
    WiFiLostMillis = millis();
    wifiLostAware = true;
  }
  if (millis() - WiFiLostMillis > 5000 && wifiLostAware) {
    WiFiConnect();
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("******************* ESP DID NOT RECONNECT ********************* ");
      Reset();
    } else {
      Serial.println("************************************************** ");
      Serial.println("       ESP SUCCESSFULLY RECONNECTED TO WIFI!       ");
      Serial.println("************************************************** ");
    }
    wifiLostAware = false;
  }
}


void Reset() {
  term.println("*****************************************************************************");
  term.println("                               RESETTING ESP                                ");
  term.println("*****************************************************************************");
  unsigned long s = millis();
  while (millis() - s < 100)
  {
    httpHandler();
  }
  esp_restart();
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
    Serial.println("WIFI CONNECTION TO " + String(ssidChar) + " FAILED!");

    Serial.println("Attempting connection to " + String(ssidChar2));

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
      Serial.println("WIFI CONNECTION TO " + String(ssidChar2) + " FAILED TOO!");
      esp_restart();
    }
  }
}
