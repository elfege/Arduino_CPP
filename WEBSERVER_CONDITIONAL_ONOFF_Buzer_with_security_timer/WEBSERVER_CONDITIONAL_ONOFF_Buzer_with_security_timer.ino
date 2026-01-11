#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEA
};
IPAddress ip(192, 168, 10, 192);
byte gateway[] = { 192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network
IPAddress myserver(192, 168, 10, 110);
EthernetServer server(80);
EthernetClient client(); // see "http://forum.arduino.cc/index.php?topic=90210.0" to finish this. trying to implement http get /?alloff...

int Value = HIGH;

void setup()
{

  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(8, HIGH);

  digitalWrite(9, HIGH); // to transistor

  pinMode(8, INPUT);
  pinMode(9, OUTPUT);
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();

}

int PowerOn()
{
  if (digitalRead(8) == Value) // if buzer is pressed
  {

    digitalWrite(2, LOW); //  press talk button
    delay(200);
    digitalWrite(2, HIGH); // release
    delay(1000);

    // leaving some time to open manually the door
    delay(10000);
    digitalWrite(2, LOW); //  press talk button again to turn off the system
    delay(200);
    digitalWrite(2, HIGH); // release

    // now we wait for someone to press a second time 2 seconds the system shut down
    delay(2000);

    if (digitalRead(8) != Value) // if buzzer is not pressed pressed
    {
      // do nothing
    }

    else {

      digitalWrite(2, LOW); //  press talk button
      delay(200);
      digitalWrite(2, HIGH); // release
      delay(1000);

      digitalWrite(4, LOW); // activate the buzzer for 2 seconds
      delay(2000);
      digitalWrite(4, HIGH); // release
    }
  }

  else {
    digitalWrite(2, HIGH); //
    digitalWrite(4, HIGH); //
  }

}

int PowerOff()
{
  digitalWrite(2, HIGH); //    buzer is off on HIGH
  digitalWrite(4, HIGH); //

}

int PowerOnSOLO()
{

  digitalWrite(2, LOW); //
  delay(2000);
  digitalWrite(2, HIGH);
  digitalWrite(4, LOW); //
  delay(3000);
  digitalWrite(4, HIGH);
}

int PowerOffSOLO()
{
  digitalWrite(2, HIGH); //    buzer is off on HIGH
  digitalWrite(4, HIGH); //

}



void loop() {

  if (digitalRead(8) == Value) // buzzer is on

  {
    PowerOn();
  }
  else if (digitalRead(8) != Value) {
    PowerOff();

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


          client.print("<form action=\"http://207.38.192.24:8002/\" >");
          if (digitalRead(8) == HIGH) {
            client.print("<p align=\"center\">Buzer is being<font color='green'>PRESSED</font></p>");
          }
          else {
            client.print("<p align=\"center\">Buzer is <font color='red'>idle</font></p>");
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
            PowerOffSOLO();
          }
          if (buffer.indexOf("GET /?status=10") >= 0)
          {
            PowerOnSOLO();
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
    }
  }
}




