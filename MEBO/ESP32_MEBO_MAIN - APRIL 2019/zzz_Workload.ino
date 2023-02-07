
void workLoad (void) {

  N = 5;

  /// this is run by core 0 and count is done in CPU_0_task1
  // if (mvt && noturn) { // only if no mvt and this is not a turn request
  // SPEED ADJUSTMENTS

  if (mvt && noturn && rqSp <= 220 && encodersOk) {
    //    Serial.println(".");
    if (cl < cr) {
      // increase cl sligthly
      if (dcL < rqSp + N) { // retain a margin N to allow modification up to rqSp + N
        dcL++;
                Serial.println("dcL++");
      }
      if (dcR > rqSp - N) { // reduce speed here only if it's been lowered below request earlier, but not lower than requested - N
        dcR--;
                Serial.println("dcR--");
      }
      ledcWrite(pwmChannelR, dcR);
      ledcWrite(pwmChannelL, dcL);
    }

    else if (cr < cl) {
      // increase cr sligthly
      if (dcR < rqSp + N) { // retain a margin N to allow modification up to rqSp + N
        dcR++;
        //        Serial.println("dcR++");
      }
      if (dcL > rqSp - N) { // reduce speed here only if it's been lowered below requested level earlier, but not lower than requested - N
        dcL--;
        //        Serial.println("dcL--");
      }
      ledcWrite(pwmChannelR, dcR);
      ledcWrite(pwmChannelL, dcL);
    }
  }
  else if (!makeEncHappen) // reset only if there hasn't been a counting request that was required to execute without speed adjustments
  {
    //    cl = 0;
    //    cr = 0;
  }
//  delay(1);
}
