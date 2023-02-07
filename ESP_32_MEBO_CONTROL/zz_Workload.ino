

void workLoad (void) {

  /// this is run by core 0 and count is done in Task1
  // if (mvt && noturn) { // only if no mvt and this is not a turn request
  // SPEED ADJUSTMENTS
  if (mvt && noturn && N != 0) {
    if (cl < cr) {
      // increase cl sligthly
      if (dutyCycleL < reqSpeed + N) { // retain a margin N to allow modification up to reqSpeed + N
        dutyCycleL++;
      }
      if (dutyCycleR > reqSpeed - N) { // reduce speed here only if it's been lowered below request earlier, but not lower than requested - N
        dutyCycleR--;
      }
    }

    if (cr < cl) {
      // increase cr sligthly
      if (dutyCycleR < reqSpeed + N) { // retain a margin N to allow modification up to reqSpeed + N
        dutyCycleR++;
      }
      if (dutyCycleL > reqSpeed - N) { // reduce speed here only if it's been lowered below requested level earlier, but not lower than requested - N
        dutyCycleL--;
      }
    }
    ledcWrite(pwmChannelR, dutyCycleR);
    ledcWrite(pwmChannelL, dutyCycleL);
  }
  else {
    cl = 0;
    cr = 0;
  }

}




