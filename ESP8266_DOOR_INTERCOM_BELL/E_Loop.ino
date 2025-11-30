//*****************************************************************************
// MAIN LOOP
//*****************************************************************************
// This runs continuously after setup() completes.
// ESP8266 requires that the loop doesn't block for too long, otherwise:
//   - WiFi stack starves (connections drop, mDNS fails)
//   - Watchdog timer triggers a reset
//   - OTA updates can't be received
//
// The yield() at the end gives the ESP8266 a chance to handle WiFi tasks.
//*****************************************************************************
// Forward declarations
void transitionTo(SystemState newState);
void forceIdle();
void monitorSignal();
void executeMainOperation();
void handleTermination();

void loop() {
  // ALWAYS handle OTA first - this allows firmware updates even if other code is stuck
  ArduinoOTA.handle();
  
  // StopAll is set to true during OTA upload (in OTAConfig onStart callback)
  // When true, we skip all normal operations to give OTA maximum resources
  if (!StopAll) {
    master();                         // Main state machine logic
    WiFiHandler();                    // Check/restore WiFi connection
    httpHandler();                    // Process any pending HTTP requests
    digitalWrite(TALK, RELEASE);      // Safety: ensure TALK relay is released
  }
  
  // yield() is critical on ESP8266 - it lets the WiFi stack process packets
  // Without this, network operations would fail
  yield();
}

//*****************************************************************************
// MASTER STATE MACHINE
//*****************************************************************************
// This function implements the "state machine" pattern.
// 
// CONCEPT: Instead of checking multiple boolean flags that could conflict,
// we check ONE variable (currentState) and handle each possible state.
//
// The "switch" statement is like a multi-way if/else, but cleaner.
// It checks the value of currentState and jumps to the matching "case".
//
// STATE TRANSITIONS:
//   IDLE ──[signal detected]──> SIGNAL_DETECTED
//   IDLE ──[test requested]───> TEST_REQUESTED  
//   SIGNAL_DETECTED ──────────> OPERATING (immediately)
//   TEST_REQUESTED ───────────> OPERATING (immediately)
//   OPERATING ──[complete]────> TERMINATING
//   TERMINATING ──[complete]──> IDLE
//   ANY STATE ──[timeout]─────> IDLE (safety reset)
//
//*****************************************************************************

void master() {
  
  // SAFETY TIMEOUT: If we've been in any non-IDLE state too long, force reset to IDLE
  // This prevents the system from getting "stuck" if something goes wrong
  if (currentState != STATE_IDLE) {
    if (millis() - operationStartTime > OPERATION_TIMEOUT_MS) {
      term.println("TIMEOUT: Operation exceeded " + String(OPERATION_TIMEOUT_MS / 1000) + "s - forcing reset to IDLE");
      forceIdle();  // Reset everything and return to monitoring
      return;       // Exit master() for this loop iteration
    }
  }
  
  // STATE MACHINE: Handle each possible state
  switch (currentState) {
    
    //-------------------------------------------------------------------------
    // STATE_IDLE: Normal monitoring mode
    // - Read the analog sensor looking for doorbell signal
    // - Blink LED to show we're alive
    // - Periodically refresh hub state
    //-------------------------------------------------------------------------
    case STATE_IDLE:
      monitorSignal();  // Check if someone rang the bell
      ledBlink();       // Visual heartbeat
      
      // Periodic hub refresh (every REFRESH_INTERVAL_MS)
      if (millis() - lastRefresh > REFRESH_INTERVAL_MS) {
        Refresh();
      }
      break;
    
    //-------------------------------------------------------------------------
    // STATE_SIGNAL_DETECTED: Bell signal was detected
    // - Transition immediately to OPERATING
    // - This state exists mainly for logging/debugging clarity
    //-------------------------------------------------------------------------
    case STATE_SIGNAL_DETECTED:
      term.println("Signal confirmed - starting door operation");
      term.println("  Signal value: " + String(sensorValue));
      transitionTo(STATE_OPERATING);
      executeMainOperation();  // This will transition to TERMINATING when done
      break;
    
    //-------------------------------------------------------------------------
    // STATE_TEST_REQUESTED: Test/simulation was requested via HTTP /simulator
    // - Same as SIGNAL_DETECTED but for manual testing
    //-------------------------------------------------------------------------
    case STATE_TEST_REQUESTED:
      term.println("TEST MODE - starting simulated door operation");
      transitionTo(STATE_OPERATING);
      executeMainOperation();
      break;
    
    //-------------------------------------------------------------------------
    // STATE_OPERATING: Door unlock operation is in progress
    // - This state is mostly handled inside executeMainOperation()
    // - If we reach here, operation should be complete
    //-------------------------------------------------------------------------
    case STATE_OPERATING:
      // executeMainOperation() handles the transition to TERMINATING
      // If we're still in OPERATING, something is wrong - the function should
      // have transitioned us. This is a safety catch.
      term.println("WARNING: Unexpected STATE_OPERATING in master() - check executeMainOperation()");
      break;
    
    //-------------------------------------------------------------------------
    // STATE_TERMINATING: Hanging up the intercom call
    // - Repeatedly press TALK to end the call
    // - Check if signal has dropped (call ended successfully)
    //-------------------------------------------------------------------------
    case STATE_TERMINATING:
      handleTermination();
      break;
    
    //-------------------------------------------------------------------------
    // DEFAULT: Should never happen - unknown state
    // - Log error and reset to IDLE for safety
    //-------------------------------------------------------------------------
    default:
      term.println("ERROR: Unknown state " + String(currentState) + " - resetting to IDLE");
      forceIdle();
      break;
  }
}

//*****************************************************************************
// SIGNAL MONITORING
//*****************************************************************************
// Reads the analog voltage sensor and checks if doorbell is ringing.
// 
// HOW IT WORKS:
// The intercom's LCD display is powered when someone rings the doorbell.
// We tap into that power line with a voltage divider, feeding it to A0.
// When voltage exceeds threshold, someone is at the door.
//
// WHY wait(50)?
// - Calling analogRead() too rapidly destabilizes the ESP8266 WiFi stack
// - wait() services HTTP requests and OTA during the delay
// - 50ms is fast enough to catch any doorbell press
//*****************************************************************************

void monitorSignal() {
  // wait() is our "safe delay" - it keeps servicing network while waiting
  // also required by ESP8266 wifi stack. 
  wait(50);
  
  // Read analog voltage (0-1023, where 1023 = 3.3V)
  sensorValue = analogRead(A0);
  
  // Check if signal exceeds threshold (doorbell is ringing)
  if (sensorValue >= threshold) {
    term.println("DOORBELL DETECTED! Value: " + String(sensorValue));
    transitionTo(STATE_SIGNAL_DETECTED);
    return;
  }
  
  // Periodic logging (throttled to avoid spam)
  if (millis() - lastSignalLog > SIGNAL_LOG_INTERVAL_MS) {
    term.println("signal: " + String(sensorValue));
    lastSignalLog = millis();
  }
}

//*****************************************************************************
// STATE TRANSITION HELPER
//*****************************************************************************
// Centralizes all state transitions for:
// - Consistent logging
// - Timestamp tracking
// - Future expansion (e.g., state change callbacks)
//
// Always use this instead of directly setting currentState!
//*****************************************************************************

void transitionTo(SystemState newState) {
  term.println("STATE: " + stateToString(currentState) + " -> " + stateToString(newState));
  
  // Record when we entered this state (for timeout tracking)
  if (newState != STATE_IDLE) {
    operationStartTime = millis();
  }
  
  // Reset termination counter when starting fresh
  if (newState == STATE_TERMINATING) {
    terminateAttempts = 0;
  }
  
  currentState = newState;
}

//*****************************************************************************
// FORCE IDLE - Emergency reset to safe state
//*****************************************************************************
// Called on timeout or error to ensure system returns to monitoring mode.
// Releases all relays and resets all state.
//*****************************************************************************

void forceIdle() {
  term.println("FORCE IDLE: Releasing all relays, resetting state");
  
  // Ensure both relays are released (buttons not pressed)
  digitalWrite(TALK, RELEASE);
  digitalWrite(DOOR, RELEASE);
  
  // Reset counters
  terminateAttempts = 0;
  
  // Return to monitoring mode
  currentState = STATE_IDLE;
}
