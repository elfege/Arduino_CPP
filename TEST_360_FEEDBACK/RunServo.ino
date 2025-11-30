void ServoRun() {
  steps = ConvertMMtoSteps(distInmm);
  Serial.println("");
  Serial.print("NEW REQUEST");


  previous = 0;
  delay(1);
  startpos = adcval();
  int desiredPos = startpos + steps;
  Serial.println("");
  Serial.print("START === "); Serial.print(startpos);
  Serial.println("");
  Serial.print("DESIRED POS === "); Serial.print(desiredPos);

  total = 0;
  Serial.println("");
  Serial.print("TOTAL === "); Serial.print(adcval());
  myservo.write(forward);

  while (adcval() < desiredPos) {
    ;
  }

  myservo.write(stopit);
  Serial.println("");
  Serial.print("END TOTAL === "); Serial.print(total);
  Serial.println("");

}

int ConvertMMtoSteps(int distInmm) {

  // 1024 = a full revolution = 60.3mm
  int distInSteps = map(distInmm, 0, circ, 0, 1024);
  Serial.println("");
  Serial.print("distInSteps = "); Serial.print(distInSteps);
  return distInSteps;

}
