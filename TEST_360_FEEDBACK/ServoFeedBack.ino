int adcval() {

  int  A = pulseIn(pulse, HIGH);
  int Max = 1020;
  int Min = 39;
  int val = map(A, Min, Max, 0, 1024);
  if (A < Min) {
    val = 0;
  }
  if (A > Max) {
    val = 1024;
  }

  if (val < 1024 && val > 0) {
    sincelast = abs(val - previous);
    total = total + sincelast;

    Serial.println("");
    Serial.print("previous val: "); Serial.print(previous);
    Serial.println("");
    Serial.print("sincelast: +"); Serial.print(sincelast);
    Serial.println("");
    Serial.print("new val: "); Serial.print(val);
    Serial.println("");
    Serial.print("new total: "); Serial.print(total);

    
  }
 recordPos(val);


  return total;
}

int recordPos(int t) {

  previous = t;
  //Serial.println("");
  //Serial.print("recorded = "); Serial.print(t);

}
