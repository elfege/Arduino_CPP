
void ElbowSpeed(int desiredVal, int DELAY) {


  int pos = Elbow.read();

  delay(10);
  
  while (pos != desiredVal) {
    if (pos < desiredVal)
    {
      pos++;
      Elbow.write(pos);
    }
    else
    {
      pos--;
      Elbow.write(pos);
    }
    delay(DELAY);
  }


}



