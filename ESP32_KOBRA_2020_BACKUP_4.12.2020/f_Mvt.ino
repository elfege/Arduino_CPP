

void fwd() {

  ledcWrite(pwmChannelL, rqSp);
  ledcWrite(pwmChannelR, rqSp);
  digitalWrite(MOTOR_LEFT1,  B);
  digitalWrite(MOTOR_LEFT2,  A);
  digitalWrite(MOTOR_RIGHT1, B);
  digitalWrite(MOTOR_RIGHT2, A);

}

void rvs() {

  ledcWrite(pwmChannelL, rqSp);
  ledcWrite(pwmChannelR, rqSp);
  digitalWrite(MOTOR_LEFT1,  A);
  digitalWrite(MOTOR_LEFT2,  B);
  digitalWrite(MOTOR_RIGHT1, A);
  digitalWrite(MOTOR_RIGHT2, B);
}

void left() {

  ledcWrite(pwmChannelL, rqSp);
  ledcWrite(pwmChannelR, rqSp);
  digitalWrite(MOTOR_LEFT1,  B);
  digitalWrite(MOTOR_LEFT2,  A);
  digitalWrite(MOTOR_RIGHT1, A);
  digitalWrite(MOTOR_RIGHT2, B);


}

void right() {

  ledcWrite(pwmChannelL, rqSp);
  ledcWrite(pwmChannelR, rqSp);
  digitalWrite(MOTOR_LEFT1,  A);
  digitalWrite(MOTOR_LEFT2,  B);
  digitalWrite(MOTOR_RIGHT1, B);
  digitalWrite(MOTOR_RIGHT2, A);

}


void MotorsStop() {

  ledcWrite(pwmChannelL, 0);
  ledcWrite(pwmChannelR, 0);
  digitalWrite(MOTOR_LEFT1,  0);
  digitalWrite(MOTOR_LEFT2,  0);
  digitalWrite(MOTOR_RIGHT1, 0);
  digitalWrite(MOTOR_RIGHT2, 0);
  //trasckstop();
}

void tracskforward() {
  tracksAreOn = true;
  tracksonmillis = millis();
  digitalWrite(TRACK_MOTOR_A, 1);
  digitalWrite(TRACK_MOTOR_B, 0);

}
void tracskbackward() {
  tracksAreOn = true;
  tracksonmillis = millis();
  digitalWrite(TRACK_MOTOR_A, 0);
  digitalWrite(TRACK_MOTOR_B, 1);

}

void trasckstop()
{
  tracksAreOn = false;
  digitalWrite(TRACK_MOTOR_A, 0);
  digitalWrite(TRACK_MOTOR_B, 0);

}
