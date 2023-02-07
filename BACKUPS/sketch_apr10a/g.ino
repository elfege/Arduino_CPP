

void Blink(int times, int lapse) {
  int c = 0;
  while (c < times) {
    ledcWrite(pwmChannelGlow, 1024);
    delay(lapse);
    ledcWrite(pwmChannelGlow, 0);
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
    ledcWrite(pwmChannelGlow, glw);
    //Serial.print("GLOW "); Serial.println(i);
  }
  else if (!goUp && glw > 0) {
    glw -= 5;
    ledcWrite(pwmChannelGlow, glw);
    //Serial.print("GLOW "); Serial.println(i);
  }
}
