int degree2ms(int degrees)
{

  return map(degrees, 0, 180, 500, 2500);

  //Serial.println("");
  // Serial.print(degrees);Serial.print("converted to: ");Serial.print(microval);
  // return microval;
  //return map(degrees, 0, 180, 500, 2500);
}


//float degree2ms(float degrees)
//{
//  //return 500.0 + ((degrees * 100.0) / 9.0);
//  return 500.0 + degrees * 100.0 / 9.0; // works
//  // 500 - 2500 so 1000 is a 90 step so 100 is 9 degrees step
//  // if we think from a 180 degree frame of reference
//  //
//  // 500 = 0 degree
//  // 2500 = 180 degrees
//  // 180/2 = 90
//  // so 1000 us => 90 degrees
//  // so 100 us = 9 degrees
//  // therefore, we have:
//  // return 500.0 + degrees * 100.0 / 9.0; // kinda works
//
//}
