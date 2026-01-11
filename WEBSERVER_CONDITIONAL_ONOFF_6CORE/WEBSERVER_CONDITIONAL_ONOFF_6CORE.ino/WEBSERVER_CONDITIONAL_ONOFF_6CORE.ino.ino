#include <SPI.h>
#include <Ethernet.h>

static byte g_abyMyMacAddress[] = {0x02, 0x1B, 0xFB, 0x38, 0x0C, 0x5C};
IPAddress ip(192, 168, 10, 191);
byte gateway[] = { 192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network
IPAddress myserver(192, 168, 10, 110);
EthernetServer server(80);
EthernetClient client(); // see "http://forum.arduino.cc/index.php?topic=90210.0" to finish this. trying to implement http get /?alloff...


void setup()
{
  digitalWrite(2, LOW);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(8, INPUT);
  Ethernet.begin(g_abyMyMacAddress, ip, gateway, subnet);
  server.begin();

}

int PowerOn()
{
  if (digitalRead(8) == LOW) // if computer is off, then we want to turn it on
  {
    digitalWrite(2, HIGH); // pressing the power button
    delay(500);
    digitalWrite(2, LOW); // Equivalent to releasing the power button after pressing it
  }
}

int PowerOff()
{
  while (digitalRead(8) == HIGH) // Computer is ON and we want to turn it Off
  {

    delay(3000); // time for smartthings to send ispy alloff command.
    digitalWrite(2, HIGH); // pressing the power button
    delay(1000); //
    digitalWrite(2, LOW); // Equivalent to releasing the power button after pressing it
    delay(30000);
  }
}



void loop() {

  Serial.println(digitalRead(8));
  delay(1000);

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


          client.print("<form action=\"http://192.168.10.191:80/\" >");
          if (digitalRead(8) == HIGH) {
            client.print("<p align=\"center\">Computer is <font color='green'>ON</font></p>");
          }
          else {
            client.print("<p align=\"center\">Computer is <font color='red'>OFF</font></p>");
          }
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"11\">OFF</p>");
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
      delay(500);
      client.stop();
      server.begin();
    }
  }
}




