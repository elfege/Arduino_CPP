#include <Wire.h>
#define CG (B1010000)
boolean resetSuccess = false;
int isReset = 13;
byte byteArray [] = {01, 01, 01, 60, 60, 60, 60, 60, 60, 8, 8, 8, 33, 33, 33, 255};

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // initialize pins:    pinMode(isReset, OUTPUT);
  digitalWrite(isReset, LOW);
  Wire.begin();        // join i2c bus (address optional for master)
}

void loop() {
  if (resetSuccess) {
    Serial.println("RESET SUCCESS");
    delay (2000);      // our work is done - pause for a while     resetSuccess = false;
  }
  else {
    Serial.println("RESET FAILED");
    resetCartridge();
    resetSuccess = verifyCartridge();
    digitalWrite(isReset, resetSuccess);
  }
}

void resetCartridge() {
  for (unsigned int i = 3; i < sizeof byteArray; i++) {
    Wire.beginTransmission(CG);
    Wire.write(i);
    Wire.write(byteArray[i]);
    Wire.endTransmission();
    delay(4);
  }
}

void movePointerTo(int deviceAddr, int memoryAddr) {
  Wire.beginTransmission(deviceAddr);
  Wire.write(memoryAddr);
  Wire.endTransmission();
}

boolean verifyCartridge() {
  boolean success = true;

  movePointerTo(CG, 3);
  Wire.requestFrom(CG, 3);
  while (Wire.available()) {
    if (Wire.read() == 60 && success == true) {
      // looking good so far
    } else {
      success = false;
    }
  }
  return success;
}
