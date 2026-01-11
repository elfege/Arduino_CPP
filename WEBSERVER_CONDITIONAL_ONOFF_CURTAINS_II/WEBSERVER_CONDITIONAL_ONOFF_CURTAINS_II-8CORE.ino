#include <SPI.h>
#include <Ethernet.h>

static byte mac[] = {0x02, 0x1A, 0x4A, 0x37, 0x0C, 0x6C};
static IPAddress ip(192, 168, 10, 194);
EthernetServer server(80);
EthernetClient client(); // see "http://forum.arduino.cc/index.php?topic=90210.0" to finish this. trying to implement http get /?alloff...

void setup()
{
  digitalWrite(2, LOW);  // open orange wire
  digitalWrite(5, LOW); // close yellow wire
  digitalWrite(8, LOW); // stop red wire
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(8, OUTPUT);
  Ethernet.begin(mac, ip);
  server.begin();
}

int OpenCurtains()
{

  digitalWrite(2, HIGH); // Opening Curtains using Solid state relays so low is first state. 
  delay(200);
  digitalWrite(2, LOW);
  /*delay(15000);
    stopall();
  */
}


int CloseCurtains()
{
  digitalWrite(5, HIGH); // Closing Curtains
  delay(200);
  digitalWrite(5, LOW);
  /*delay(15000);
    stopall();
  */
}
int stopall()
{

  digitalWrite(8, HIGH); // stop
  delay(200);
  digitalWrite(8, LOW); // stop

}
void loop() {

  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    boolean CurtainsAreOpen = true;
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


          client.print("<form action=\"http://192.168.10.194:80/\" >");
          if (CurtainsAreOpen == true) {
            client.print("<p align=\"center\">BEDROOM curtains are <font color='green'>OPEN</font></p>");
          }
          else if (CurtainsAreOpen == false) {
            client.print("<p align=\"center\">BEDROOM Curtains are <font color='red'>CLOSED</font></p>");
          }
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"12\">STOP</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"11\">CLOSE CURTAIN</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"10\">OPEN CURTAIN</p>");
          client.print("<p align=\"center\"><input type=\"submit\" value=\"Submit\"> </p></form>");

          break;
        }
        if (c == '\n') {

          currentLineIsBlank = true;
          buffer = "";
        }
        else if (c == '\r') {
          if (buffer.indexOf("GET /?status=12") >= 0)
          {

            stopall();

          }
          if (buffer.indexOf("GET /?status=11") >= 0)
          {
            CurtainsAreOpen = false;
            CloseCurtains();

          }
          if (buffer.indexOf("GET /?status=10") >= 0)
          {
            CurtainsAreOpen = true;
            OpenCurtains();

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




