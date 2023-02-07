

void fwd() {
  rqSp = 254; // Hbridge seems to not hold the current properly causing brownouts for now
  chargeoff();
  mvt = true;
  //server.send(200, "text/html", "forward");
  motorsOnMillis = millis();
  ledcWrite(pwmChannelL, rqSp);
  ledcWrite(pwmChannelR, rqSp);
  if (!STOPALL)
  {
    digitalWrite(MOTOR_LEFT1,  B);
    digitalWrite(MOTOR_LEFT2,  A);
    digitalWrite(MOTOR_RIGHT1, A);
    digitalWrite(MOTOR_RIGHT2, B);
  }
  else
  {
    Serial.println("STOPALL ENABLED");
    Blink(10, 30);
  }

}

void rvs() {
  rqSp = 254; // Hbridge seems to not hold the current properly causing brownouts for now
  chargeoff();
  mvt = true;
 // server.send(200, "text/html", "rerverse");
  motorsOnMillis = millis();
  ledcWrite(pwmChannelL, rqSp);
  ledcWrite(pwmChannelR, rqSp);
  if (!STOPALL)
  {
    digitalWrite(MOTOR_LEFT1,  A);
    digitalWrite(MOTOR_LEFT2,  B);
    digitalWrite(MOTOR_RIGHT1, B);
    digitalWrite(MOTOR_RIGHT2, A);
  }else
  {
    Serial.println("STOPALL ENABLED");
    Blink(10, 30);
  }
}

void left() {
  rqSp = 254; // Hbridge seems to not hold the current properly causing brownouts for now
  chargeoff();
  mvt = true;
 // server.send(200, "text/html", "left");
  motorsOnMillis = millis();
  ledcWrite(pwmChannelL, rqSp);
  ledcWrite(pwmChannelR, rqSp);
  if (!STOPALL)
  {
    digitalWrite(MOTOR_LEFT1,  A);
    digitalWrite(MOTOR_LEFT2,  B);
    digitalWrite(MOTOR_RIGHT1, A);
    digitalWrite(MOTOR_RIGHT2, B);
  }else
  {
    Serial.println("STOPALL ENABLED");
    Blink(10, 30);
  }
}

void right() {
  rqSp = 254; // Hbridge seems to not hold the current properly causing brownouts for now
  chargeoff();
  mvt = true;
 // server.send(200, "text/html", "right");
  motorsOnMillis = millis();
  ledcWrite(pwmChannelL, rqSp);
  ledcWrite(pwmChannelR, rqSp);
  if (!STOPALL)
  {
    digitalWrite(MOTOR_LEFT1,  B);
    digitalWrite(MOTOR_LEFT2,  A);
    digitalWrite(MOTOR_RIGHT1, B);
    digitalWrite(MOTOR_RIGHT2, A);
  }else
  {
    Serial.println("STOPALL ENABLED");
    Blink(10, 30);
  }

}


void STOP() {
  chargeoff();
  mvt = false;
  server.send(200, "text/html", "stop");
  ledcWrite(pwmChannelL, 0);
  ledcWrite(pwmChannelR, 0);
  digitalWrite(MOTOR_LEFT1,  0);
  digitalWrite(MOTOR_LEFT2,  0);
  digitalWrite(MOTOR_RIGHT1, 0);
  digitalWrite(MOTOR_RIGHT2, 0);
  //trackstop();
}

void tracskforward() {
  mvt = true;
  server.send(200, "text/html", "tracskforward");
  tracksOnmillis = millis();
  digitalWrite(TRACK_MOTOR_A, 1);
  digitalWrite(TRACK_MOTOR_B, 0);
  delay(200);// out of safety always spin these by increments of 200 millis
  trackstop();

}
void tracskbackward() {
  mvt = true;
  server.send(200, "text/html", "tracskbackward");
  tracksOnmillis = millis();
  digitalWrite(TRACK_MOTOR_A, 0);
  digitalWrite(TRACK_MOTOR_B, 1);
  delay(200);// out of safety always spin these by increments of 200 millis
  trackstop();

}

void trackstop()
{
  server.send(200, "text/html", "trackstop");
  digitalWrite(TRACK_MOTOR_A, 0);
  digitalWrite(TRACK_MOTOR_B, 0);
  mvt = false;
}

boolean tracksAreOn()
{
  boolean result = digitalRead(TRACK_MOTOR_A) || digitalRead(TRACK_MOTOR_B);
  return result;
}
boolean motorsAreOn()
{
  return digitalRead(MOTOR_RIGHT2) || digitalRead(MOTOR_RIGHT1) || digitalRead(MOTOR_LEFT1) || digitalRead(MOTOR_LEFT2);
}
