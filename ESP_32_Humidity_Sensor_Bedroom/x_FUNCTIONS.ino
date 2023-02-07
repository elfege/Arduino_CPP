int humidityLevel() {
  return analogRead(PIN_TEMPERATUREHUMIDITY_1);
}


void Sleep() {
}

void Reset() {
  Blink(5, 500);
  ESP.restart();
}

void Blink(int times, int lapse) {
  int c = 0;
  while (c < times) {
    digitalWrite(LED, 1);
    delay(lapse);
    digitalWrite(LED, 0);
    delay(lapse);
    c++;
  }
}


