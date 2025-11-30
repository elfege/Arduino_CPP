//******************************************************************************************
//st::Everything::callOnMsgSend() optional callback routine.  This is a sniffer to monitor
//    data being sent to ST.  This allows a user to act on data changes locally within the
//    Arduino sktech.
//******************************************************************************************
 

void callback(const String &msg)
{
    String strTemp = msg;
    Serial.print(F("ST_Anything Callback: Sniffed data = "));
    Serial.println(msg);

  if(msg == "water1 wet"){
   // st::SmartThingsESP32WiFi::send(strTemp);
  }

  //Serial.println("Water Sensor Analog Value = "); Serial.print(analogRead(PIN_WATER_1));
  //Serial.println("Water Sensor Digital Value = "); Serial.print(digitalRead(PIN_WATER_1));


  //  if(digitalRead(

  //Masquerade as the ThingShield to send data to the Arduino, as if from the ST Cloud (uncomment and edit following line)
  // st::receiveSmartString("Put your command here!");  //use same strings that the Device Handler would send
 
}
