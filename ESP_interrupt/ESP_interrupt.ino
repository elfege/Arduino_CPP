/* LED pin */
byte ledPin = 2;
/* pin that is attached to interrupt */
byte interruptPin = 14;
/* hold the state of LED when toggling */
volatile byte state = LOW;

void setup() {
  //Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  /* set the interrupt pin as input pullup*/
  pinMode(interruptPin, INPUT_PULLUP);
  /* attach interrupt to the pin
  function blink will be invoked when interrupt occurs
  interrupt occurs whenever the pin change value */
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
  //Serial.println("SETUP OK");
}

void loop() {
 // Serial.println(analogRead(interruptPin));
}

/* interrupt function toggle the LED */
void blink() {
  state = !state;
  digitalWrite(ledPin, state);
}
