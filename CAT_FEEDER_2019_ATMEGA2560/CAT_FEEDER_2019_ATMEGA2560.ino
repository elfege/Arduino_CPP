/* Mac address of this board: 
 *  
 */


//#include <Servo.h>
#include <SPI.h>
#include <Ethernet.h>
#include "index.h"  //Web page header file



#include <HttpClient.h>

//******************************************************************************************
// SmartThings Library for Arduino Ethernet W5100 Shield
//******************************************************************************************
#include <SmartThingsEthernetW5100.h>    //Library to provide API to the SmartThings Ethernet W5100 Shield

//******************************************************************************************
//st::Everything::callOnMsgSend() optional callback routine.  This is a sniffer to monitor 
//    data being sent to ST.  This allows a user to act on data changes locally within the 
//    Arduino sktech withotu having to rely on the ST Cloud for time-critical tasks.
//******************************************************************************************
void callback(const String &msg)
{
  //Uncomment if you want to use this function
  //Serial.print(F("ST_Anything_Miltiples Callback: Sniffed data = "));
  //Serial.println(msg);
  
  //TODO:  Add local logic here to take action when a device's value/state is changed
  
  //Masquerade as the ThingShield to send data to the Arduino, as if from the ST Cloud (uncomment and edit following line(s) as you see fit)
  //st::receiveSmartString("Put your command here!");  //use same strings that the Device Handler would send
}
