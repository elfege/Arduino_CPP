
void workLoad (void) {

  N = 8;

  /// this is run by core 0 and count is done in CPU_0_task1
  // if (mvt && noturn) { // only if no mvt and this is not a turn request
  // SPEED ADJUSTMENTS

  if (mvt && noturn && rqSp <= 220 && encodersOk) {
    //    Serial.println(".");
    if (cl < cr) { // cl is the amount of increments, if it has changed less than cr, then it means cr is going faster
      dcL++;
      //        Serial.println("dcL++");
    }
    else if (cr < cl) {
      // increase cr sligthly
      dcR++;
      //        Serial.println("dcR++");
    }
    ledcWrite(pwmChannelR, dcR);
    ledcWrite(pwmChannelL, dcL);
    if (dcR > rqSp + N || dcL > rqSp + N || dcR < rqSp - N || dcL < rqSp - N)
    {
      dcR = rqSp;
      dcL = rqSp;
    }
  }
  else if (!makeEncHappen) // reset only if there hasn't been a counting request that was required to execute without speed adjustments
  {
    //    cl = 0;
    //    cr = 0;
  }

}
