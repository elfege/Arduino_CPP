void workLoad (void) ;                // prototype is required

void Task1( void * parameter )
{
  for (;;) {

    /// encoder counters (modifs made by Core 0)

    VLB4 = digitalRead(LEFT);
    VRB4 = digitalRead(RIGHT);

    if (mvt && noturn) // only if no mvt, and this is not a turn request
    {
      // count each input
      if (VL != VLB4) {
        cl++;
        //Serial.println("L+");
      }
      if (VR != VRB4) {
        cr++;
        //Serial.println("R+");
      }
      //delayMicroseconds(10);
      VL = digitalRead(LEFT);
      VR = digitalRead(RIGHT);

      workLoad();// modify speeds accordingly
    }
    else {

      // reset values for next count and to allow stuck() to run a new positive test
      //cl = 0;
      //cr = 0;
    }
    //    //Serial.print( "Task 1 complete running on Core "  );
   
    delay(3);
  }
}
