#include <SPI.h>
#include <Ethernet.h>



byte mac[] = {
  0xDE, 0xA2, 0xBE, 0xFE, 0xFE, 0xEA
};
byte ip[] = {192, 168, 10, 192};
byte gateway[] = { 192, 168, 10, 12 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network
EthernetServer server(80);
EthernetClient client(); 


void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(8, INPUT);
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  server.begin();
  Ethernet.begin(ip, mac, gateway, subnet);
}



int EverythingLOW()
{
  if (digitalRead(8) == LOW)
  {
    digitalWrite(2, LOW); //
    delay(2000);
    digitalWrite(4, LOW); //
  }
}


int OpenDoor()
{
  if (digitalRead(8) == HIGH) // Somone rang the bell
  {
    digitalWrite(2, HIGH); // pressing the talk button
    delay(3000);            // time for com to be established
    digitalWrite(2, HIGH); // pressing the talk button again, turning off the system (safety measure)
    delay(1000);
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
            OpenDoor();
          }

        }

      }
      if (buffer.indexOf("GET /?status=10") >= 0)
      {
        EverythingLOW();
      }


      else {

        currentLineIsBlank = false;
      }
    }

    {
      delay(500);
      client.stop();
    }
  }
}


