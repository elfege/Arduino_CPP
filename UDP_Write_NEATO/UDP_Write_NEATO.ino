#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>



// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xD1, 0xAD, 0xC5, 0xEF, 0xFE, 0xC1 };
IPAddress ip(192, 168, 10, 206);

unsigned int localPort = 8888;      // local port to listen on

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // start the Ethernet and UDP:
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);

}

void loop() {
  IPAddress neatoIP = (192, 168, 10, 22);
  unsigned int neatoPort = 8008;
  //char command[13] = "\fd\02 \01\01]";
  byte on[] = {0xfd,0x02,0x20,0x01,0x01,0x5D};
  byte off[] =  {0xfd,0x02,0x20,0x01,0x00,0x5D};
  
 
  Udp.beginPacket(neatoIP, neatoPort);
  Udp.write(on, 6);
  Udp.endPacket();

  delay(2000); 

    Udp.beginPacket(neatoIP, neatoPort);
  Udp.write(off, 6);
  Udp.endPacket();

}

