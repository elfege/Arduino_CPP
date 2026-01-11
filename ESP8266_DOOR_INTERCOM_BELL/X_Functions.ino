//*****************************************************************************
// MAIN DOOR OPERATION
//*****************************************************************************
// This is the core sequence that runs when the doorbell rings:
//   1. Press TALK button (answer the intercom)
//   2. Wait for intercom system to connect
//   3. Press DOOR button (unlock the building door)
//   4. Wait for door to unlock
//   5. Release DOOR button
//   6. Transition to TERMINATING to hang up
//
// STATE REQUIREMENT: Must be in STATE_OPERATING when called
//*****************************************************************************

void executeMainOperation() {
  // GUARD: Only run if we're in the correct state
  // This prevents accidental re-entry if something calls this function twice
  if (currentState != STATE_OPERATING) {
    term.println("executeMainOperation() called but state is " + stateToString(currentState) + " - aborting");
    return;
  }
  
  term.println("=== DOOR OPERATION START ===");
  
  //--- Step 1: Press TALK to answer the intercom ---
  term.println("Step 1: Pressing TALK button (answering intercom)");
  digitalWrite(TALK, PRESS);
  wait(1000);  // Hold for 1 second - simulates picking up the phone
  
  //--- Step 2: Release TALK ---
  term.println("Step 2: Releasing TALK button");
  digitalWrite(TALK, RELEASE);
  wait(1500);  // Give intercom system time to establish connection
  
  //--- Step 3: Press DOOR to unlock ---
  term.println("Step 3: Pressing DOOR button (unlocking door)");
  digitalWrite(DOOR, PRESS);
  wait(5000);  // Hold for 5 seconds - typical door unlock duration
  
  //--- Step 4: Release DOOR ---
  term.println("Step 4: Releasing DOOR button");
  digitalWrite(DOOR, RELEASE);
  wait(500);  // Brief pause before termination
  
  term.println("=== DOOR OPERATION COMPLETE ===");
  
  // Transition to termination phase (hang up the call)
  transitionTo(STATE_TERMINATING);
}

//*****************************************************************************
// CALL TERMINATION
//*****************************************************************************
// After unlocking the door, we need to "hang up" the intercom call.
// This is done by pressing TALK briefly (like hanging up a phone).
// 
// The intercom may keep the connection open for a while, so we:
//   1. Check if signal is still active (call still connected)
//   2. If active, press TALK to try hanging up
//   3. Repeat up to MAX_TERMINATE_ATTEMPTS times
//   4. Give up and return to IDLE either way
//
// STATE REQUIREMENT: Must be in STATE_TERMINATING when called
//*****************************************************************************

void handleTermination() {
  // Check if intercom signal is still active
  sensorValue = analogRead(A0);
  bool callStillActive = (sensorValue >= threshold);
  
  // If call has ended OR we've tried too many times, return to IDLE
  if (!callStillActive) {
    term.println("Call terminated successfully (signal dropped)");
    transitionTo(STATE_IDLE);
    return;
  }
  
  if (terminateAttempts >= MAX_TERMINATE_ATTEMPTS) {
    term.println("Max termination attempts reached - returning to IDLE anyway");
    transitionTo(STATE_IDLE);
    return;
  }
  
  // Try to hang up: press and release TALK
  term.println("Termination attempt " + String(terminateAttempts + 1) + "/" + String(MAX_TERMINATE_ATTEMPTS));
  term.println("  Signal still active: " + String(sensorValue));
  
  digitalWrite(TALK, PRESS);
  wait(1000);
  digitalWrite(TALK, RELEASE);
  wait(1500);
  
  terminateAttempts++;
}

//*****************************************************************************
// HTTP ENDPOINT: /talk
//*****************************************************************************
// Manually trigger the TALK button via HTTP request.
// Used for testing or manual intercom interaction.
//
// BEHAVIOR:
// - If TALK is already pressed, release it
// - Otherwise, press it for 500ms then release
//
// STATE PROTECTION: Refuses to run if an operation is in progress
//*****************************************************************************

void talk() {
  // GUARD: Don't allow during active operation
  if (currentState == STATE_OPERATING || currentState == STATE_TERMINATING) {
    term.println("talk() blocked - operation in progress (state: " + stateToString(currentState) + ")");
    _server.send(503, "text/html", "BUSY - operation in progress");
    return;
  }
  
  term.println("HTTP /talk - current state: " + String(digitalRead(TALK) == PRESS ? "PRESSED" : "RELEASED"));
  
  // Toggle behavior: if pressed, release; if released, press
  if (digitalRead(TALK) == PRESS) {
    digitalWrite(TALK, RELEASE);
    _server.send(200, "text/html", "TALK RELEASED");
  } else {
    digitalWrite(TALK, PRESS);
    _server.send(200, "text/html", "TALKING");
    wait(500);
    // Note: TALK will be released by the safety line in loop()
  }
}

//*****************************************************************************
// HTTP ENDPOINT: /door
//*****************************************************************************
// Manually trigger the DOOR unlock button via HTTP request.
//
// WARNING: This only works if an intercom call is active!
// The intercom system requires an active call before DOOR works.
// Pressing DOOR without a call does nothing.
//
// STATE PROTECTION: Refuses to run if an operation is in progress
//*****************************************************************************

void unlock_door() {
  // GUARD: Don't allow during active operation
  if (currentState == STATE_OPERATING || currentState == STATE_TERMINATING) {
    term.println("unlock_door() blocked - operation in progress (state: " + stateToString(currentState) + ")");
    _server.send(503, "text/html", "BUSY - operation in progress");
    return;
  }
  
  term.println("HTTP /door - unlocking door");
  
  // Ensure DOOR starts in released state
  if (digitalRead(DOOR) == PRESS) {
    term.println("  DOOR was pressed - releasing first");
    digitalWrite(DOOR, RELEASE);
    wait(500);
  }
  
  // Press DOOR button
  digitalWrite(DOOR, PRESS);
  _server.send(200, "text/html", "OPENING");
  wait(5000);  // Hold for 5 seconds
  
  // Release DOOR button
  digitalWrite(DOOR, RELEASE);
  term.println("  DOOR released");
}

//*****************************************************************************
// HTTP ENDPOINT: /simulator (test mode)
//*****************************************************************************
// Triggers a simulated doorbell event for testing.
// This will run the full door operation sequence without an actual doorbell.
//
// STATE PROTECTION: Only works from IDLE state
//*****************************************************************************

void test() {
  if (currentState != STATE_IDLE) {
    term.println("test() blocked - not in IDLE state (current: " + stateToString(currentState) + ")");
    _server.send(503, "text/html", "BUSY - not in IDLE state");
    return;
  }
  
  term.println("TEST REQUESTED via HTTP");
  transitionTo(STATE_TEST_REQUESTED);
}

//*****************************************************************************
// POWER OFF - Safety function
//*****************************************************************************
// Ensures all relays are released. Called when returning to IDLE.
// Also sends state update to hub (currently disabled).
//*****************************************************************************

void PowerOff() {
  bool needsAction = (digitalRead(TALK) == PRESS || digitalRead(DOOR) == PRESS);
  
  if (needsAction) {
    term.println("PowerOff: Releasing all relays");
    digitalWrite(TALK, RELEASE);
    digitalWrite(DOOR, RELEASE);
    send_event("switch off");  // Currently disabled - see send_event()
  }
}

//*****************************************************************************
// HTTP/NETWORK HANDLER
//*****************************************************************************
// Services all network-related tasks:
// - HTTP server requests
// - WiFi terminal (for remote debugging)
// - Yields to WiFi stack
//
// This is called from wait() and loop() to ensure network stays responsive.
//*****************************************************************************

void httpHandler() {
  _server.handleClient();  // Process pending HTTP requests
  term.handleClient();     // Process WiFi terminal connections
  yield();                 // Let ESP8266 WiFi stack do its work
}

//*****************************************************************************
// HUB EVENT SENDER (CURRENTLY DISABLED)
//*****************************************************************************
// Sends state updates to Hubitat hub.
// Disabled because SmartThings library has been removed.
// TODO: Re-implement with simpler HTTP POST when ready.
//*****************************************************************************

void send_event(String var) {
  // DISABLED - SmartThings library removed
  // To re-enable, implement HTTP POST to Hubitat's Maker API
  return;
  
  // Original code for reference:
  // term.println("Sending event message: " + var);
  // smartthing.send(var);
  // wait(1000);
}

//*****************************************************************************
// HUB STATE REFRESH
//*****************************************************************************
// Sends current state to Hubitat hub(s).
// Called periodically from master() and on demand via HTTP /refresh.
//*****************************************************************************

void Refresh() {
  term.println("REFRESH: Sending state to hub");
  getStates(true, "refresh");
  lastRefresh = millis();
}

//*****************************************************************************
// LED HEARTBEAT BLINK
//*****************************************************************************
// Blinks the onboard LED once per second to indicate the system is running.
// Non-blocking implementation using millis().
//*****************************************************************************

void ledBlink() {
  if (millis() - lastMillisBlink >= 1000) {
    digitalWrite(LED, !digitalRead(LED));  // Toggle LED state
    lastMillisBlink = millis();
  }
}

//*****************************************************************************
// BLOCKING LED BLINK (for startup indication)
//*****************************************************************************
// Blinks LED a specific number of times with set interval.
// Uses wait() to keep network responsive during blinks.
//*****************************************************************************

void Blink(int times, int lapse) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED, HIGH);  // LED on (active high on most NodeMCU)
    wait(lapse);
    digitalWrite(LED, LOW);   // LED off
    wait(lapse);
  }
}

//*****************************************************************************
// SMARTTHINGS/HUBITAT MESSAGE CALLBACK
//*****************************************************************************
// Called when a message is received from the hub.
// Currently not connected (SmartThings library disabled).
// Kept for future re-implementation.
//*****************************************************************************

void messageCallout(String message) {
  term.println("Hub message received: '" + message + "'");
  
  // Route message to appropriate handler
  if (message.equals("refresh")) {
    Refresh();
  } else if (message.equals("open") || message.equals("on")) {
    unlock_door();
  } else if (message.equals("close") || message.equals("off")) {
    PowerOff();
  } else if (message.equals("reset")) {
    Reset();
  } else if (message.equals("talk")) {
    talk();
  } else if (message.equals("simulator")) {
    test();
  } else {
    term.println("  Unknown message - ignored");
  }
}

//*****************************************************************************
// SAFE WAIT FUNCTION
//*****************************************************************************
// Replaces delay() with a network-friendly version.
//
// WHY NOT USE delay()?
// delay() blocks everything - no HTTP requests processed, no OTA handled,
// WiFi stack gets starved. After a few seconds, connections drop.
//
// wait() instead:
// - Loops until the time has passed
// - Each iteration services OTA, HTTP, and WiFi
// - Network stays responsive during the wait
//
// USE THIS instead of delay() everywhere except WiFiConnect()
//*****************************************************************************

void wait(int timeMs) {
  unsigned long start = millis();
  while (millis() - start < (unsigned long)timeMs) {
    ArduinoOTA.handle();  // Allow OTA updates during wait
    httpHandler();        // Service HTTP requests
    yield();              // Feed the WiFi stack
  }
}

//*****************************************************************************
// ESP RESET
//*****************************************************************************
// Performs a software reset of the ESP8266.
// Gives HTTP a moment to send response before resetting.
//*****************************************************************************

void Reset() {
  term.println("*****************************************************************************");
  term.println("                           RESETTING ESP8266                                ");
  term.println("*****************************************************************************");
  
  // Brief pause to allow HTTP response to send
  unsigned long start = millis();
  while (millis() - start < 100) {
    httpHandler();
  }
  
  ESP.restart();
}

//*****************************************************************************
// WIFI CONNECTION HANDLER
//*****************************************************************************
// Monitors WiFi connection and attempts reconnection if lost.
// Called from main loop.
//*****************************************************************************

void WiFiHandler() {
  // Detect WiFi disconnection
  if (WiFi.status() != WL_CONNECTED && !wifiLostAware) {
    term.println("*** WIFI DISCONNECTED ***");
    WiFiLostMillis = millis();
    wifiLostAware = true;
  }
  
  // After 5 seconds of disconnection, attempt reconnect
  if (wifiLostAware && millis() - WiFiLostMillis > 5000) {
    WiFiConnect();
    
    if (WiFi.status() != WL_CONNECTED) {
      term.println("*** WIFI RECONNECTION FAILED - RESETTING ***");
      Reset();
    } else {
      term.println("*** WIFI RECONNECTED SUCCESSFULLY ***");
    }
    
    wifiLostAware = false;
  }
}

//*****************************************************************************
// WIFI CONNECTION
//*****************************************************************************
// Attempts to connect to WiFi, trying primary SSID first, then backup.
// Uses blocking delay() here because network isn't available yet anyway.
//*****************************************************************************

void WiFiConnect() {
  // Configure static IP
  if (!WiFi.config(ip, gateway, subnet, dnsserver, secondaryDNS)) {
    term.println("WiFi: Static IP configuration failed");
  }
  
  // Try primary SSID
  term.println("WiFi: Connecting to " + String(ssidChar));
  WiFi.begin(ssidChar, passwordChar);
  
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
    delay(500);  // OK to use delay() here - no network yet
    term.print(".");
  }
  
  // If primary failed, try backup SSID
  if (WiFi.status() != WL_CONNECTED) {
    term.println("\nWiFi: Primary SSID failed, trying " + String(ssidChar2));
    
    if (!WiFi.config(ip, gateway, subnet, dnsserver, secondaryDNS)) {
      term.println("WiFi: Static IP configuration failed");
    }
    
    WiFi.begin(ssidChar2, passwordChar);
    start = millis();
    
    while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
      delay(500);
      term.print(".");
    }
    
    // If both failed, hard reset
    if (WiFi.status() != WL_CONNECTED) {
      term.println("\nWiFi: Both SSIDs failed - hard reset");
      ESP.reset();  // Hard reset (vs restart which is soft reset)
    }
  }
  
  term.println("\nWiFi: Connected! IP: " + WiFi.localIP().toString());
}

//*****************************************************************************
// STATE TO STRING HELPER
//*****************************************************************************
// Converts SystemState enum to human-readable string for logging.
//*****************************************************************************

String stateToString(SystemState state) {
  switch (state) {
    case STATE_IDLE:            return "IDLE";
    case STATE_SIGNAL_DETECTED: return "SIGNAL_DETECTED";
    case STATE_OPERATING:       return "OPERATING";
    case STATE_TERMINATING:     return "TERMINATING";
    case STATE_TEST_REQUESTED:  return "TEST_REQUESTED";
    default:                    return "UNKNOWN(" + String(state) + ")";
  }
}

//*****************************************************************************
// LEGACY FUNCTION STUBS
//*****************************************************************************
// These existed in the old code but are no longer needed.
// Kept as empty stubs in case something still calls them.
//*****************************************************************************

void on() {
  term.println("on() - DEPRECATED, use unlock_door()");
}

void off() {
  term.println("off() - DEPRECATED, use PowerOff()");
}

void close() {
  term.println("close() - DEPRECATED, handled by state machine");
}
