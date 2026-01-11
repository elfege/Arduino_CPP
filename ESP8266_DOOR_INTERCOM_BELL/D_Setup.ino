//*****************************************************************************
// SETUP - Runs once at boot
//*****************************************************************************
// Initializes all hardware, network, and services.
// Order matters! Some components depend on others being ready.
//*****************************************************************************

void setup() {
  //--- Hardware pins ---
  pinConfig();
  Blink(10, 50);  // Quick LED burst to show we're starting

  //--- Serial debug output ---
  Serial.begin(115200);
  term.link(Serial);  // WiFiTerm echoes to Serial for USB debugging

  //--- OTA hostname (must be set before WiFi for mDNS) ---
  ArduinoOTA.setHostname(NameOTA);

  //--- WiFi Terminal (remote serial over WiFi) ---
  term.begin(_server);
  term.println("WiFiTerm ready");

  //--- HTTP endpoints ---
  initXMLhttp();
  _server.begin();
  term.println("HTTP server started on port 80");

  //--- OTA update service ---
  OTAConfig();

  //--- Initial state report to hub ---
  Refresh();

  //--- Startup complete indicator ---
  Blink(3, 500);  // Slow blinks = ready
  term.println("=== SETUP COMPLETE ===");
  term.println("State: " + stateToString(currentState));
}

//*****************************************************************************
// PIN CONFIGURATION
//*****************************************************************************
// Sets up GPIO pins for relay control and LED.
// Initializes outputs to safe state (relays released).
//*****************************************************************************

void pinConfig() {
  pinMode(DOOR, OUTPUT);
  pinMode(TALK, OUTPUT);
  pinMode(LED, OUTPUT);
  // A0 (sensorPin) is always analog input on ESP8266, no pinMode needed

  // Initialize to safe state - all relays released
  digitalWrite(DOOR, RELEASE);
  digitalWrite(TALK, RELEASE);
  digitalWrite(LED, LOW);
}

//*****************************************************************************
// OTA (Over-The-Air) UPDATE CONFIGURATION
//*****************************************************************************
// Enables firmware updates over WiFi without physical USB connection.
//
// CALLBACKS:
//   onStart    - Called when upload begins
//   onEnd      - Called when upload completes successfully
//   onProgress - Called during upload with percentage
//   onError    - Called if upload fails
//
// CRITICAL: During OTA upload, normal operations must stop (StopAll = true)
// to give maximum resources to the upload process.
//*****************************************************************************

void OTAConfig() {

  //--- Upload starting ---
  ArduinoOTA.onStart([]() {
    term.println("OTA: Upload starting...");
    StopAll = true;  // Stop all normal operations during upload
  });

  //--- Upload complete ---
  ArduinoOTA.onEnd([]() {
    term.println("OTA: Upload complete!");
    delay(100);  // Allow final TCP ACK to transmit to IDE
                 // ESP.restart();
    ESP.reset();  // Reboot into new firmware // Hard reset (clears all memory WARNING: CLEARS RTC MEMORY AS WELL!)
  });

  //--- Upload progress ---
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    unsigned int percent = progress / (total / 100);
    term.printf("OTA: %u%%\r\n", percent);
    term.handleClient();                   // Keep terminal responsive during upload
    digitalWrite(LED, !digitalRead(LED));  // Blink LED during upload
  });

  //--- Upload error ---
  ArduinoOTA.onError([](ota_error_t error) {
    term.printf("OTA ERROR [%u]: ", error);

    switch (error) {
      case OTA_AUTH_ERROR: term.println("Auth Failed"); break;
      case OTA_BEGIN_ERROR: term.println("Begin Failed"); break;
      case OTA_CONNECT_ERROR: term.println("Connect Failed"); break;
      case OTA_RECEIVE_ERROR: term.println("Receive Failed"); break;
      case OTA_END_ERROR: term.println("End Failed"); break;
      default: term.println("Unknown Error"); break;
    }

    // Ensure LED is in safe state (not left LOW which would affect boot)
    digitalWrite(LED, HIGH);

    // Don't reset StopAll here - let the system recover naturally
    // or user can manually reset
  });

  ArduinoOTA.begin();
  term.println("OTA: Ready (hostname: " + String(NameOTA) + ")");
}
