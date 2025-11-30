
void workLoad (void) {

  N = 5;

  /// this is run by core 0 and count is done in Task1
  // if (mvt && noturn) { // only if no mvt and this is not a turn request
  // SPEED ADJUSTMENTS
  if (mvt && noturn && rqSp <= 220) {
    if (cl < cr) {
      // increase cl sligthly
      if (dcL < rqSp + N) { // retain a margin N to allow modification up to rqSp + N
        dcL++;
      }
      if (dcR > rqSp - N) { // reduce speed here only if it's been lowered below request earlier, but not lower than requested - N
        dcR--;
      }
      ledcWrite(pwmChannelR, dcR);
      ledcWrite(pwmChannelL, dcL);
    }

    else if (cr < cl) {
      // increase cr sligthly
      if (dcR < rqSp + N) { // retain a margin N to allow modification up to rqSp + N
        dcR++;
      }
      if (dcL > rqSp - N) { // reduce speed here only if it's been lowered below requested level earlier, but not lower than requested - N
        dcL--;
      }
      ledcWrite(pwmChannelR, dcR);
      ledcWrite(pwmChannelL, dcL);
    }

  }
  else {
    cl = 0;
    cr = 0;
  }

}
