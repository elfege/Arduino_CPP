

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

void glow() {
  int s = 512;

  if (glw >= s) {
    goUp = false;
  }
  else if (glw <= 1) {
    goUp = true;
  }

  if (goUp && glw < s) {
    glw += 5;
    //ledcWrite(pwmChannelGlow, glw);
    //Serial.print("GLOW "); Serial.println(i);
  }
  else if (!goUp && glw > 0) {
    glw -= 5;
    //ledcWrite(pwmChannelGlow, glw);
    //Serial.print("GLOW "); Serial.println(i);
  }
}

void Reset() {
  Serial.println("*****************************************************************************");
  Serial.println("                               RESTARTING ESP                                ");
  Serial.println("*****************************************************************************");
  //Blink(10, 50);
  esp_restart();
}
