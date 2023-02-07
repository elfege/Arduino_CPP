#include "HardwareSerial.h"
HardwareSerial Serial2(2);

void setup() {
  Serial.begin(115200); 
  Serial2.begin(115200, SERIAL_8N1, rx2, tx2); // pins 16 rx2, 17 tx2, 19200 bps, 8 bits no parity 1 stop bit
  

}

void loop() {
 Serial.print("OBS = "); Serial.println(OBS());

}

boolean OBS() {
  cmd = "sOBSe";
  int t = 0;
  String received = "";

  UartSend(cmd);

  
  while (!Serial2.available())
  {
    delay(1);
  }

  received = UartReceive();
  Serial.print("received OBS val: "); Serial.println(received);

  if (received == "s1e") {
    return true;
  }
  else if (received == "s0e") {
    return false;
  }
}
