unsigned long previousMillisTest = millis();

void loop() {

  // serial_read();
  if (millis() - lastStateCheck >= 10000) {
    state_check();
  }
  state_blink();
  httpHandler();
  delay(10);  // required with analogRead()
}

void httpHandler() {
  st::Everything::run();
  term.handleClient(); 
  ArduinoOTA.handle();
  _server.handleClient();
  yield();
}

int blinked = 0;
void state_check() {

  if (!currentlyFeeding) {
    // refresh statuses every x minutes
    if (millis() - previousMillis > DELAY_SEC * 60000 && blinked < 5) {

      Refresh();
      if (millis() - previousMillisBlink > 500) {
        previousMillisBlink = millis();
        digitalWrite(LED, !digitalRead(LED));
      }
      blinked += 1;
      if (blinked >= 5) {
        previousMillis = millis();
        blinked = 0;
      }
    }


    Serial.println(String(Full() ? "TANK FULL" : "TANK EMPTY"));


    if (Full() && wasempty) {
      wasempty = false;  // manage immediate report without overflooding the buffer
      term.println("NEW STATE: FULL");
      String var = "feeder ready";
      sendToHub(var);
      //st::Everything::sendSmartString(var);
    } else if (!Full() && !wasempty) {
      wasempty = true;
      term.println("NEW STATE: EMPTY");
      String var = "feeder empty";
      sendToHub(var);
      //st::Everything::sendSmartString(var);
    }
  }

  lastStateCheck = millis();
}


void state_blink() {

  if (!Full()) {
    if (millis() - previousMillisBlink > 200) {
      if (millis() - previousMillisEmptyLog >= 2000) {
        previousMillisEmptyLog = millis();
      }
      previousMillisBlink = millis();
      digitalWrite(LED, !digitalRead(LED));
    }
  } else {
    if (millis() - previousMillisBlink > 1000) {
      previousMillisBlink = millis();
      digitalWrite(LED, !digitalRead(LED));
    }
  }
}

void serial_read() {
  // Check if data is available to read from the serial input
  if (Serial.available() > 0) {
    // Read the incoming string from the serial port
    String incomingCommand = Serial.readStringUntil('\n');

    // Trim any whitespace or newline characters
    incomingCommand.trim();

    // Compare the incoming string to the reset command
    if (incomingCommand.equals("reset")) {
      // Trigger the Reset function
      Reset();
    } else if (incomingCommand.equals("init_st")) {
      Serial.println("Initializing ST...");
      delay(1000);
      EverythingInit();
    } else {
      // If the command is not "reset", you can echo it back or handle other commands
      Serial.println("Unknown command: " + incomingCommand);
    }
  }
}
