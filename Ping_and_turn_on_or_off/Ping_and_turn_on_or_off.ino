/*
  Ping Example

  This example sends an ICMP pings every 500 milliseconds, sends the human-readable
  result over the serial port.

  Circuit:
   Ethernet shield attached to pins 10, 11, 12, 13

  created 30 Sep 2010
  by Blake Foster

  //Modified by William Fecke
  //25 Jan 2014
  added a logic statement to trip power on pc if ping failed.

*/

#include <SPI.h>
#include <Ethernet.h>
#include <ICMPPing.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; // max address for ethernet shield
byte ip[] = {192, 168, 10, 190}; // ip address for ethernet shield
IPAddress pingAddr(192, 168, 10, 110); // ip address to ping
int failogic = 0;
int alternatePress = 0;



SOCKET pingSocket = 0;

char buffer [256];
ICMPPing ping(pingSocket, (uint16_t)random(0, 255));

void setup()
{
  // start Ethernet
  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  
}

void loop()
{
  ICMPEchoReply echoReply = ping(pingAddr, 4);
  if (echoReply.status != SUCCESS) // failed ping = computer is OFF and we want to turn it on
  {
    sprintf(buffer,
            "Reply[%d] from: %d.%d.%d.%d: bytes=%d time=%ldms TTL=%d",
            echoReply.data.seq,
            echoReply.addr[0],
            echoReply.addr[1],
            echoReply.addr[2],
            echoReply.addr[3],
            REQ_DATASIZE,
            millis() - echoReply.data.time,
            echoReply.ttl);
    failogic = 0; // resets the logic count if machine starts to respond to the pings
    alternatePress = 0;
  }
  else
  {
    failogic++; //this will increment the counter for every failed ping
    sprintf(buffer, "Echo request failed; %d", echoReply.status);
  }
  Serial.println(buffer);
  if (failogic > 5) // failed pings will trip the if statement and cause the arduino to power on the computer
  {
    digitalWrite(2, LOW); // Equivalent to releasing the power button after pressing it
    delay(100);
    digitalWrite(2, HIGH);
  }
}










