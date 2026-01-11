#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAC, 0xBA, 0xEE, 0x3F, 0xED};
IPAddress ip(192, 168, 10, 203);
byte gateway[] = { 192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network
IPAddress myserver(192, 168, 10, 110);
EthernetServer server(80);
EthernetClient client(); // see "http://forum.arduino.cc/index.php?topic=90210.0" to finish this. trying to implement http get /?alloff...

int opened = 0;
int closed = 0;
int stoped = 0;
int slowmode = 1;

void setup()
{
  digitalWrite(2, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(8, HIGH); // controls the slow/fast mode switch (different porwer sources)
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(8, OUTPUT);
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();

}

int Open()
{
  digitalWrite(2, LOW); //
  digitalWrite(4, LOW); //

  closed = 0;
  opened = 1;
  stoped = 0;

}

int Close()
{
  digitalWrite(2, HIGH); //
  digitalWrite(4, HIGH); //

  closed = 1;
  opened = 0;
  stoped = 0;

}

int Stop()
{
  digitalWrite(2, LOW); //
  digitalWrite(4, HIGH); //

  stoped = 1;

}

int SlowMode() {
  digitalWrite(8, HIGH);
  slowmode = 1;

}

int FastMode() {
  digitalWrite(8, LOW);
  slowmode = 0;

}




void loop() {

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


          client.print("<form action=\"http://192.168.10.203/\" >");
          if (opened == 1) {
            client.print("<p align=\"center\">Window BEDROOM is <font color='green'>OPEN</font></p>");
          }
          else if (closed == 1) {
            client.print("<p align=\"center\">Window BEDROOM is <font color='red'>closed</font></p>");
          }
          if (stoped == 1) {
            client.print("<p align=\"center\"><font color='blue'>STOPED</font> by user</p>");
          }
          else {
            client.print("<p align=\"center\"><font color='blue'>READY</font></p>");
          }
          if (slowmode == 1) {
            client.print("<p align=\"center\">SPEED is in <font color='blue'>SLOW MODE</font></p>");
          }
          else {
            client.print("<p align=\"center\">SPEED is in <font color='blue'>FAST MODE</font></p>");
          }
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"14\">FAST MODE</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"13\">SLOW MODE</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"12\">STOP</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"11\">CLOSE</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"10\">OPEN</p>");

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
            Close();
          }
          if (buffer.indexOf("GET /?status=10") >= 0)
          {
            Open();
          }
          if (buffer.indexOf("GET /?status=12") >= 0)
          {
            Stop();
          }
          if (buffer.indexOf("GET /?status=13") >= 0)
          {
            SlowMode();
          }
          if (buffer.indexOf("GET /?status=14") >= 0)
          {
            FastMode();
          }

        }
        else {

          currentLineIsBlank = false;
        }
      }
    }

    {
      delay(500);
      client.stop();
      server.begin();
    }
  }
}




