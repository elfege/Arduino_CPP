---
title: ESP8266 Door Intercom Controller
description: Project history and session notes
---

<!-- markdownlint-disable MD025 -->

# ESP8266 Door Intercom Controller

ESP8266-based (NodeMCU v1.0) door intercom automation system. Monitors analog voltage from an intercom's LCD power line and automatically unlocks the building door when someone rings the doorbell.

**Hardware**: NodeMCU v1.0 (ESP8266-12E)

**Pins**:

- `A0` - Analog voltage sensor (intercom LCD power detection)
- `D4` (GPIO2) - Onboard LED (status indicator)
- `D5` (GPIO14) - TALK relay control
- `D7` (GPIO13) - DOOR relay control

**Signal threshold**: 200 (analog value 0-1023)

**Workflow**: Monitor A0 for voltage spike → Press TALK (answer) → Press DOOR (unlock) → Press TALK (hang up)

**File Structure**:

```
ESP8266_DOOR_INTERCOM_BELL/
├── ESP8266_DOOR_INTERCOM_BELL.ino  // Main file, includes only
├── A_Types.h                        // State machine enum definition
├── A_PINS_Const_Vars.ino           // Pin definitions, constants, globals
├── C_WiFi.ino                      // WiFi configuration, credentials
├── C_XML.ino                       // HTTP endpoint handlers
├── D_Setup.ino                     // setup() and initialization
├── E_Loop.ino                      // loop(), state machine, monitoring
├── X_Functions.ino                 // All other functions
├── index.h                         // Web UI (PROGMEM HTML)
└── index.html                      // Web UI source (edit this, copy to .h)
```

---

## November 30, 2025 | 22:00 (EDT) | State Machine Refactor & UI Redesign

### Issues Identified

1. Intermittent UI loading failures - page white or timeout every 2-3 reboots
2. OTA discovery failures - device wouldn't appear in Arduino IDE
3. OTA upload failures - slow or failing mid-transfer
4. Race conditions - multiple boolean flags getting out of sync
5. Post-OTA instability - device unstable until hard reset

### Root Causes

#### WiFi Stack Starvation

`monitor_signal()` called `delay(10)` which blocks WiFi stack. 12KB HTML page transmission needs multiple TCP packets - blocking prevents completion. Same issue affected mDNS (OTA discovery) and OTA handshake.

#### Race Conditions

Multiple booleans (`got_signal`, `switch_door_operation_is_running`, `runsim`, `cmdFromHttp`) could conflict. HTTP requests during `wait()` could trigger functions that modify shared state mid-operation.

#### Post-OTA Instability

`ESP.restart()` (soft reset) leaves memory/state potentially corrupted. `ESP.reset()` (hard reset) clears everything.

### Fixes Applied

#### WiFi Stack Fix

Replaced `delay(10)` with `wait(50)` in `monitor_signal()`. The `wait()` function services HTTP and yields to WiFi stack.

#### Enhanced wait() Function

```cpp
void wait(int timeMs) {
  unsigned long start = millis();
  while (millis() - start < (unsigned long)timeMs) {
    ArduinoOTA.handle();
    httpHandler();
    yield();
  }
}
```

#### OTA Completion Fix

Added `delay(100)` before `ESP.restart()` in `onEnd()` to allow final TCP ACK transmission.

#### State Machine Implementation

Replaced multiple booleans with single enum in `A_Types.h`:

```cpp
enum SystemState {
  STATE_IDLE,
  STATE_SIGNAL_DETECTED,
  STATE_OPERATING,
  STATE_TERMINATING,
  STATE_TEST_REQUESTED
};
```

All functions check `currentState` before executing. State changes via `transitionTo()` for logging and timeout tracking.

#### State Protection on HTTP Handlers

Functions return 503 if system is busy:

```cpp
if (currentState == STATE_OPERATING || currentState == STATE_TERMINATING) {
  _server.send(503, "text/html", "BUSY");
  return;
}
```

#### Operation Timeout Safety

Auto-reset to IDLE if operation exceeds 30 seconds.

#### Hard Reset Endpoint

Added `/hardreset` endpoint calling `ESP.reset()` for post-OTA recovery.

#### UI Redesign

Complete rewrite: dark industrial theme, state machine display, hard reset with modal confirmation, auto-poll toggle, toast notifications, collapsible terminal, responsive layout, vanilla JS + axios (no jQuery).

### Variables Removed

```cpp
// Replaced by state machine
bool got_signal;
bool switch_door_operation_is_running;
bool runsim;
bool cmdFromHttp;

// Unused
bool refreshrequest;
bool sent;
String lastMessage;
int lastloop;
int Value;
int frequency;
```

### Key Learnings

1. ESP8266 WiFi stack needs regular servicing - use `wait()` not `delay()`
2. State machines prevent race conditions - single source of truth
3. Forward declarations required when functions call each other before definition
4. Enums must be in `.h` files to be visible across `.ino` files
5. `ESP.reset()` (hard) may be needed after OTA, not just `ESP.restart()` (soft)

### TODO

- [ ] Re-implement Hubitat integration (HTTP POST instead of SmartThings library)
- [ ] `send_event()` currently disabled
- [ ] Test actual doorbell operation (requires physical access)

### Notes

Arduino IDE 2.x error "No monitor available for the port protocol network" after OTA is cosmetic - upload succeeds if terminal shows 100%.
