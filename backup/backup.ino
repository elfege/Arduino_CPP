#include <SPI.h>
#include <Ethernet.h>
#include <ICMPPing.h>



byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 10, 190);
byte gateway[] = {
  192, 168, 10, 1
};   //your router's IP address
byte subnet[] = {
  255, 255, 255, 0
};    //subnet mask of the network

EthernetServer server(80);



void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
}

int PowerOn()
{
  SOCKET pingSocket = 0;
  char buffer [256];
  ICMPPing ping(pingSocket, (uint16_t)random(0, 255));
  IPAddress pingAddr(192, 168, 10, 110); // ip address to ping
  int failogic = 0;
  ICMPEchoReply echoReply = ping(pingAddr, 4);
  if (echoReply.status == SUCCESS) // failed ping = computer is OFF and we want to turn it on
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
  }
  else
  {
    failogic++; //this will increment the counter for every failed ping
    sprintf(buffer, "Echo request failed; %d", echoReply.status);
  }
  Serial.println(buffer);
  if (failogic > 5) // failed pings will trigger the if statement and cause the arduino to power on the computer
  {
    digitalWrite(2, LOW); // Equivalent to releasing the power button after pressing it
    delay(1000);
    digitalWrite(2, HIGH);
  }
  if (failogic == 0) // Successful ping so computer is ALREADY ON so do nothin
  {
    delay(1);
  }
}




int PowerOff()
{
  SOCKET pingSocket = 0;
  char buffer [256];
  ICMPPing ping(pingSocket, (uint16_t)random(0, 255));
  IPAddress pingAddr(192, 168, 10, 110); // ip address to ping
  int failogic = 0;
  ICMPEchoReply echoReply = ping(pingAddr, 4);
  if (echoReply.status == SUCCESS) // failed ping = computer is OFF and we want to turn it on
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
  }
  else
  {
    failogic++; //this will increment the counter for every failed ping
    sprintf(buffer, "Echo request failed; %d", echoReply.status);
  }
  Serial.println(buffer);
  if (failogic == 0) // Successful pings will trip the if statement and cause the arduino to power OFF the computer
  {
    digitalWrite(2, LOW); // Equivalent to releasing the power button after pressing it
    delay(1000);
    digitalWrite(2, HIGH);
  }
  if (failogic > 5) // Failed ping so computer is ALREADY off so do nothin
  {
    delay(1);
  }

}

void loop()
{

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
            client.print("<p align=\"center\">Computer is <font color='green'>OFF</font></p>");
          }
          else {
            client.print("<p align=\"center\">Computer is <font color='red'>ON</font></p>");
          }
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"11\"> OFF</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"10\">ON</p>");
          client.print("<p align=\"center\"><input type=\"submit\" value=\"Submit\"> </p></form>");

          break;
        }
        if (c == '\n') {

          currentLineIsBlank = true;
          buffer = "";
        }
        else if (c == '\r') {
          if (buffer.indexOf("GET /?status=11") >= 0)
          {
            PowerOff();
          }
          if (buffer.indexOf("GET /?status=10") >= 0)
          {
            PowerOn();
          }

        }
        else {

          currentLineIsBlank = false;
        }
      }
    }

    {
      delay(1);
      client.stop();
    }


  }
}



