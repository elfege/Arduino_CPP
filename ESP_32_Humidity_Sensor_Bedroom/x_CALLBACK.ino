//******************************************************************************************
//st::Everything::callOnMsgSend() optional callback routine.  This is a sniffer to monitor
//    data being sent to ST.  This allows a user to act on data changes locally within the
//    Arduino sktech.
//******************************************************************************************


void messageCallout(String message)
{

  Serial.print("Received message: '");
  Serial.print(message);
  Serial.println("' ");

  ///  see if there is a request for a different speed


  if (message.equals("reset"))
  {

    Reset();
  }


}


void refresh() {
  Serial << "sending: " << humidityLevel() << endl;
  String var = String(humidityLevel());


  Serial << "sending: " << var << endl;
  st::Everything::sendSmartString(var);

}


