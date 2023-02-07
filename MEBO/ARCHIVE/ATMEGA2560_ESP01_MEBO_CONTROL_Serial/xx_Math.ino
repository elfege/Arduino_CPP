#include <math.h>

void mapSpeed() {
  // calculate best adjustment margin for the required speed
  //the higher the speed, the lower the speed alteration margin must be:
  N = map(rqSp, 160, 200, 2, 4); // at 255 you can't add speed, so make sure no adjustment happens beyond a max safe speed of 230
}

void makedist(float dist) {

  if(HttpDistReq){
    dist = httpdist;
  }
  Serial << "requested distance in cm = " << dist << endl;
  float ratio = 10.00 / 8.00; // 10 bits = 8 cm
  Serial << "ratio is = " << ratio << endl;

  float convertedDist = ratio * dist;

  Serial << "requested distance in bits = " << convertedDist << endl;

  int limit = cl + convertedDist; // set the bitdistance as a limit

  Serial.print("cl start:  "); Serial.println(cl);
  Serial.print("cr start:  "); Serial.println(cr);

  delay(1); // necessary to allow wheels to run and change of state

  GotStuck = false;
  // STOPALL = false; // only for tests

  while (cr < limit && cl < limit && !STOPALL && !GotStuck && mvt && !bumped) {
    loop();
  }
  
  
  if(STOPALL){
    Serial << "STOPALL IS TRUE" << endl;
  }
  if(!mvt){
    Serial << "mvt is FALSE" << endl;
  }
  if(bumped){
    Serial << "bumped IS TRUE" << endl;
    bumped = false;
  }
  if(GotStuck){
    Serial << "GotStuck IS TRUE" << endl;
    GotStuck = false;
  }
  mvt = false;
  noturn = false; // stop encoders and related tests such as stuck()
  
  //STOPALL = false; // only for tests
  //GotStuck = false;

  
  Serial << "cr ARRIVAL = " << cr  << endl;
  Serial << "cl ARRIVAL = " << cl  << endl;
  STOP();
  Serial << "end of makdist" <<  endl;
  //  cl = 0;
  //  cr = 0;

}

int MatchD2SREAR(int Close, int far, int slow, int fast) {
  int d = analogRead(RLIR);
  int e = analogRead(RRIR);

  int vald = _max(d, e);

  int Map = map(vald, Close, far, slow, fast); // the closer, the slower / consider only the sensor which returns the closest(highest) value

  // round to the closest five (183 --> 185)
  Map = round10(Map);

  if (Map > 180) {
    Map = 180;
  }

  delay(1);
  return Map;

}


int MatchD2SFRONT() {
  int d = analogRead(FIR);
  int Map = map(d, 2000, 0, 150, 200); // the closer, the slower

  // round to the closest five (183 --> 185)
  Map = round10(Map);

  if (Map > 200) {
    Map = 200;
  }

  delay(10);
  return Map;

}


int round10(int n) {
  int result = (n / 10 + (n % 10 > 2)) * 10;

  return result;
}

