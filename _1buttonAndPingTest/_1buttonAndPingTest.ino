#include <SPI.h>
#include <Ethernet.h>
#include <ICMPPing.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; // mac address for ethernet shield
IPAddress ip(192, 168, 10, 190); // ip address for ethernet shield
byte gateway[] = {192, 168, 10, 1};   //your router's IP address
byte subnet[] = {255, 255, 255, 0 };    //subnet mask of the network
EthernetServer server(80);

IPAddress pingAddr(192, 168, 10, 110); // ip address to ping
int failogic = 0;
int alternatePress = 0;

SOCKET pingSocket = 0;

ICMPPing ping(pingSocket, (uint16_t)random(0, 255));


void setup()
{
  Ethernet.begin(mac, ip, gateway, subnet);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);

  server.begin();
}



void loop() {
  char buffer [256];
  ICMPEchoReply echoReply = ping(pingAddr, 4);
  if (echoReply.status == SUCCESS)
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
  }

  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    String buffer = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.print(c);
        buffer += c;
        if (c == '\n' && currentLineIsBlank) {

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();


          client.print("<form action=\"http://192.168.10.190/\" >");
          if (digitalRead(2)) {
            client.print("<p align=\"center\">Light 1 is <font color='green'>ON</font></p>");
          }
          else {
            client.print("<p align=\"center\">Light 1 is <font color='red'>OFF</font></p>");
          }
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"11\"> ON</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"10\">OFF</p>");
          if (digitalRead(8)) {
            client.print("<p align=\"center\">Light 2 is <font color='green'>ON</font></p>");
          }
          else {
            client.print("<p align=\"center\">Light 2 is <font color='red'>OFF</font></p>");
          }
          client.print("<p align=\"center\"><input type=\"submit\" value=\"Submit\"> </p></form>");

          break;
        }
        if (c == '\n') {

          currentLineIsBlank = true;
          buffer = "";
        }

        else if (c == '\r') {

          if (buffer.indexOf("GET /?status=11") >= 0 && failogic > 5)//if PC is OFF ping is unsuccessful so we want to turn it ON
          {
            digitalWrite(2, LOW);
            delay(200);
            digitalWrite(2, HIGH);

          }


          if (buffer.indexOf("GET /?status=10") >= 0 && failogic == 0)//if PC is ON ping is successful so we want to turn it Off
          {
            digitalWrite(2, LOW);
            delay(5500);
            digitalWrite(2, HIGH);

          }



          if (buffer.indexOf("GET /?status=51") >= 0) {
            digitalWrite(2, HIGH);
            digitalWrite(8, HIGH);
            digitalWrite(7, HIGH);
            digitalWrite(6, HIGH);
          }

          if (buffer.indexOf("GET /?status=50") >= 0) {
            digitalWrite(2, LOW);
            digitalWrite(8, LOW);
            digitalWrite(7, LOW);
            digitalWrite(6, LOW);
          }


        }
        else {

          currentLineIsBlank = false;
        }
      }
    }

    delay(1);

    client.stop();
  }
}


