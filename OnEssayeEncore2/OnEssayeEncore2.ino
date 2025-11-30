 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Oled display attached to pins 8, 9, 

 created 21.05.2014
 by Mihaly Krich
 http://www.mypi.hu

 */

#include <SPI.h>         
#include <Ethernet.h>
#include <ICMPPing.h>


byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; // max address for ethernet shield
byte ip[] = {192,168,10,190}; // ip address for ethernet shield
byte pingAddr[] = {192,168,10,110}; // ip address to ping

SOCKET pingSocket = 0;
ICMPEchoReply echoReply = ping(pingAddr, 4)

char buffer [256];

void setup() 
{
  // start Ethernet
  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  delay(10);
}

void loop()
{ ICMPPing ping(pingSocket);
  ping(4, pingAddr, buffer);
  if (echoReply.status != SUCCESS)


  

  delay(500);
  break; 
}


