//*****************************************************************************
// HTTP ENDPOINT REGISTRATION
//*****************************************************************************
// Registers all HTTP endpoints with the web server.
// Called once during setup().
//
// ENDPOINTS:
//   /           - Serves the main HTML page
//   /getstates  - Returns current system state (for AJAX polling)
//   /debug      - Toggles debug mode
//   /reset      - Restarts the ESP8266
//   /simulator  - Triggers a test door operation
//   /talk       - Manually activates TALK button
//   /door       - Manually activates DOOR button
//   /getip      - Returns the device's IP address
//   /getmac     - Returns the device's MAC address
//   /refresh    - Forces a hub state refresh
//*****************************************************************************

void initXMLhttp() {
  
  //--- Main page ---
  // Serves the HTML UI from PROGMEM (flash memory, not RAM)
  _server.on("/", []() {
    term.println("HTTP / - Serving main page");
    _server.send_P(200, "text/html", MAIN_page);
  });

  //--- State query (AJAX) ---
  // Called by JavaScript to update the UI with current states
  _server.on("/getstates", []() {
    getStates(false, "javascript");
  });

  //--- Debug toggle ---
  _server.on("/debug", []() {
    debug = !debug;
    String response = "DEBUG is " + String(debug ? "ACTIVE" : "OFF");
    term.println("HTTP /debug - " + response);
    _server.send(200, "text/html", response);
  });

  //--- System reset ---
  _server.on("/reset", []() {
    term.println("HTTP /reset - Resetting ESP");
    _server.send(200, "text/html", "Resetting...");
    Reset();  // Note: response may not complete before reset
  });

  //--- Test/Simulator ---
  _server.on("/simulator", []() {
    test();  // test() handles its own response
  });

  //--- Manual TALK button ---
  _server.on("/talk", []() {
    talk();  // talk() handles its own response
  });

  //--- Manual DOOR button ---
  _server.on("/door", []() {
    unlock_door();  // unlock_door() handles its own response
  });

  //--- IP address query ---
  _server.on("/getip", []() {
    String localIp = WiFi.localIP().toString();
    if (debug) term.println("HTTP /getip - " + localIp);
    _server.send(200, "text/html", localIp);
  });

  //--- MAC address query ---
  _server.on("/getmac", []() {
    String mac = WiFi.macAddress();
    if (debug) term.println("HTTP /getmac - " + mac);
    _server.send(200, "text/html", mac);
  });

  //--- Force hub refresh ---
  _server.on("/refresh", []() {
    term.println("HTTP /refresh");
    Refresh();
    _server.send(200, "text/html", "Refreshed");
  });
}

//*****************************************************************************
// STATE REPORT GENERATOR
//*****************************************************************************
// Generates a comma-separated state string for:
//   1. The web UI (via AJAX /getstates endpoint)
//   2. The Hubitat hub (when smartthingReq is true)
//
// PARAMETERS:
//   smartthingReq - If true, also sends state to Hubitat hub
//   origin        - String identifying who requested (for logging)
//
// FORMAT for web UI:
//   "debug ACTIVE,bell INACTIVE,door CLOSED,talk TALK,state IDLE,"
//
// FORMAT for Hubitat:
//   Individual events sent via send_event() (currently disabled)
//*****************************************************************************

void getStates(bool smartthingReq, String origin) {
  // Build state string for web UI
  // Using char arrays instead of String concatenation would be more memory
  // efficient, but this is clearer and runs infrequently enough to be OK
  
  String sep = ",";
  
  // States for web UI display
  String debugState = "debug " + String(debug ? "ACTIVE" : "OFF");
  String bellState = "bell " + String(sensorValue >= threshold ? "ACTIVE" : "INACTIVE");
  String doorState = "door " + String(digitalRead(DOOR) == PRESS ? "OPENING" : "CLOSED");
  String talkState = "talk " + String(digitalRead(TALK) == PRESS ? "TALKING" : "TALK");
  String systemState = "state " + stateToString(currentState);  // NEW: show state machine state
  
  // Combine for HTTP response
  String webResponse = debugState + sep + bellState + sep + doorState + sep + talkState + sep + systemState + sep;
  _server.send(200, "text/html", webResponse);
  
  //--- Hub updates (only when requested or on refresh interval) ---
  bool shouldUpdateHub = smartthingReq || (millis() - lastRefresh > 600000);
  
  if (shouldUpdateHub) {
    // Hubitat expects lowercase event values
    String hubDoor = "door " + String(digitalRead(DOOR) == PRESS ? "open" : "closed");
    String hubSwitch = "switch " + String(digitalRead(DOOR) == PRESS ? "on" : "off");
    String hubBell = "bell " + String(sensorValue >= threshold ? "active" : "inactive");
    String hubTalk = "talk " + String(digitalRead(TALK) == PRESS ? "talking" : "idle");
    String hubAccel = "acceleration " + String(sensorValue >= threshold ? "active" : "inactive");
    
    // Send each state as separate event
    // Note: send_event() is currently disabled (returns immediately)
    term.println("Updating hub with current states:");
    
    term.println("  -> " + hubDoor);
    send_event(hubDoor);
    
    term.println("  -> " + hubSwitch);
    send_event(hubSwitch);
    
    term.println("  -> " + hubBell);
    send_event(hubBell);
    
    term.println("  -> " + hubTalk);
    send_event(hubTalk);
    
    term.println("  -> " + hubAccel);
    send_event(hubAccel);
  }
}
