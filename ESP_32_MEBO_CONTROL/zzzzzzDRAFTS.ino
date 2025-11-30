////#include "webserver.h"
//
//#define LED 2
//
//void workLoad (void) {
//
//  /// this run by core 0
//
//
//
//  VL = digitalRead(LEFT);
//  VR = digitalRead(RIGHT);
//  int N = 20; // speed alteration margin
//  mvt = digitalRead(PWM_LEFT1) ||  digitalRead(PWM_LEFT2);
//
//  if (mvt)
//  {
//    if (VL != VLB4) {
//      cl++;
//    }
//    if (VR != VRB4) {
//      cr++;
//    }
//
//    VLB4 = digitalRead(LEFT);
//    VRB4 = digitalRead(RIGHT);
//    //    Serial << "workload on CPU # " << xPortGetCoreID()  << endl;
//
//    // SPEED ADJUSTMENTS
//    if (noturn) { // only if this is not a turn request
//      if (cl < cr) {
//        //Serial.println("ADL");
//        // increase cl sligthly
//        if (dutyCycleL < reqSpeed + N) { // retain a margin N to allow modification up to reqSpeed + N
//          // tu peux augmenter, mais pas plus que reqSpeed + N
//          dutyCycleL  += 1;
//        }
//        if (dutyCycleR > reqSpeed - N) { // reduce speed here only if it's been lowered below request earlier but not lower than requested - N
//          // tu peux réduire, mais pas en deçà que reqSpeed - N
//          dutyCycleR  -= 1;
//        }
//      }
//      else if (cr < cl) {
//        //Serial.println("ADR");
//        // increase cr sligthly
//        if (dutyCycleR < reqSpeed + N) { // retain a margin N to allow modification down to reqSpeed - N
//          // tu peux augmenter, mais pas plus que reqSpeed + N
//          dutyCycleR  += 1;
//        }
//        if (dutyCycleL > reqSpeed - N) { // reduce speed here only if it's been lowered below request earlier but not lower than requested - N
//          // tu peux réduire, mais pas en deçà que reqSpeed - N
//          dutyCycleL  -= 1;
//        }
//      }
//      ledcWrite(pwmChannelR, dutyCycleR);
//      ledcWrite(pwmChannelL, dutyCycleL);
//    }
//  }
//  else {
//    if (!published) {
//      Serial << "cr arrival = " << cr  << endl;
//      Serial << "cl arrival = " << cl  << endl;
//      Serial << "dutyCycleR arrival = " << dutyCycleR  << endl;
//      Serial << "dutyCycleL arrival = " << dutyCycleL  << endl;
//      published = true;
//    }
//    cl = 0;
//    cr = 0;
//  }
//
//}
//
//
//

