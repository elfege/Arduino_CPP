//******************************************************************************************
//st::Everything::callOnMsgSend() optional callback routine.  This is a sniffer to monitor
//    data being sent to ST.  This allows a user to act on data changes locally within the
//    Arduino sktech.
//******************************************************************************************
void callback(const String &msg)
{
  //  String strTemp = msg;
  //  Serial.print(F("ST_Anything Callback: Sniffed data = "));
  //  Serial.println(msg);

  //TODO:  Add local logic here to take action when a device's value/state is changed

  //Masquerade as the ThingShield to send data to the Arduino, as if from the ST Cloud (uncomment and edit following line)
  //st::receiveSmartString("Put your command here!");  //use same strings that the Device Handler would send
  //  if (strTemp.startsWith("temperature1"))
  //  {
  //    strTemp.remove(0,13);
  //    Serial.println(strTemp);
  //  }
  //  if (strTemp.startsWith("humidity1"))
  //  {
  //    strTemp.remove(0,10);
  //    Serial.println(strTemp);
  //  }
}
