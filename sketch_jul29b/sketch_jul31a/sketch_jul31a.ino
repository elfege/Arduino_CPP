void setup() {
  Serial.begin(115200);
  Serial.println("SETUP OK");
  analogReadResolution(8);
}

void loop() {
  Serial.println("analogRead(36) = "+String(analogRead(36)));
  delay(100);


}
