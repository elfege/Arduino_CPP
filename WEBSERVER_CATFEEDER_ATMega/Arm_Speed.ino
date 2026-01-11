
void ArmSpeed(int desiredVal, int DELAY) {


  int pos = Arm.read();

  delay(10);
  
  while (pos != desiredVal) {
    if (pos < desiredVal)
    {
      pos++;
      Arm.write(pos);
    }
    else
    {
      pos--;
      Arm.write(pos);
    }
    delay(DELAY);
  }


}



