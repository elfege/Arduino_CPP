#ifndef A_TYPES_H
#define A_TYPES_H

// State machine states
// "enum" creates a custom type with named values.
// Under the hood, these are just integers (0, 1, 2, 3, 4), but using names
// makes the code self-documenting and prevents magic number errors.
enum SystemState {
  STATE_IDLE,              // 0: Normal monitoring mode - watching for doorbell signal
  STATE_SIGNAL_DETECTED,   // 1: Bell signal detected, preparing to start operation
  STATE_OPERATING,         // 2: Door operation in progress - ignore new triggers
  STATE_TERMINATING,       // 3: Hanging up the intercom call
  STATE_TEST_REQUESTED     // 4: Test/simulation was requested via HTTP
};


#endif