#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>



// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xD1, 0xAD, 0xC5, 0xEF, 0xFE, 0xC1 };
byte ip[] = {192, 168, 10, 206};
byte gateway[] = { 192, 168, 10, 1 };
// the subnet:
byte subnet[] = { 255, 255, 255, 0 };

unsigned int localPort = 8888;      // local port to listen on

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetServer server = EthernetServer(8008);


void setup() {
  // initialize the ethernet device
  Ethernet.begin(mac, ip, gateway, subnet);

  // start listening for clients
  server.begin();

}

void loop() {
  byte neatoIP[] = {192, 168, 10, 22};
  unsigned int neatoPort = 8008;
  //char command[13] = "\fd\02 \01\01]";
  byte on[] = {0xfd,0x02,0x20,0x01,0x01,0x5D};
  byte off[] =  {0xfd,0x02,0x20,0x01,0x00,0x5D};
  
 // if an incoming client connects, there will be bytes available to read:
  EthernetClient client = server.available();
  if (client == true) {
    // read bytes from the incoming client and write them back
    // to any clients connected to the server:
    server.write(neatoIP);
    server.write(neatoPort);
     server.write(on);
    

}

