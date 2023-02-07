

void mapSpeed() {
  // calculate best adjustment margin for the required speed
  //the higher the speed, the lower the speed alteration margin must be:
  N = map(rqSp, 160, 200, 2, 4); // at 255 you can't add speed, so make sure no adjustment happens beyond a max safe speed of 230
}

void makedist(float dist, int rqSp, String directio) {

  //  if (HttpDistReq) {
  //    dist = httpdist;
  //  }
  //term.print( "requested distance in cm = " ); term.println( dist );
  float ratio = 10.00 / 8.00; // 10 bits = 8 cm
  //term.print( "ratio is = " ); term.println( ratio );

  float convertedDist = ratio * dist;

  //term.print( "requested distance in bits = " ); term.println( convertedDist );

  limit = /*cl + */ convertedDist; // set the bitdistance as a limit

  //  term.print("cl start:  "); term.println(cl);


  //delay(1); // necessary to allow wheels to run and change state to prevent GotStuck false positive

  GotStuck = false;
  bumped = false;
  // STOPALL = false; // only for tests

  cr = 0;
  cl = 0;
//  term.print("cr start:  "); term.println(cr);
//  term.print("cl start:  "); term.println(cl);
  ledcWrite(pwmChannelL, rqSp);
  ledcWrite(pwmChannelR, rqSp);

  if (directio == "fwd")
  {
    simpleFwd();
  }
  else
  {
    simpleRvs();
  }

  unsigned long start = millis();
  while (cr < limit && cl < limit && millis() - start < 20000 && !STOPALL && !GotStuck && mvt && !bumped)
  {
    stuck();
    BUMP();
    //        term.println(cr);
    //        term.println(cl);
    //   delay(1);
  }

  STOP(); // resets values


  if (STOPALL) {
    term.println("STOPPED BY STOPALL = TRUE" );
  }
  if (bumped) {
    term.println("STOPPED BY bumped = TRUE" );
    bumped = false;
  }
  if (GotStuck) {
    term.println("STOPPED BY GotStuck = TRUE" );
    GotStuck = false;
  }

  //STOPALL = false; // only for tests
  //GotStuck = false;

  limit = 0; // reset this value

  //term.print( "end of makdist" );
  //  cl = 0;
  //  EncR.cr = 0;

}

int MatchD2SREAR(int Close, int far, int slow, int fast) {
  int d = 0; //RLIRVal();
  int e = 0; //RRIRVal();

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
  int d = frontIR_RVal() + frontIR_LVal() / 2;
  int Map = map(d, 1024, 0, 130, 180); // the closer, the slower

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
