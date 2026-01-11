#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 10, 190);
byte gateway[] = { 192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network
IPAddress myserver(192, 168, 10, 110);
EthernetServer server(80);
EthernetClient client(); // see "http://forum.arduino.cc/index.php?topic=90210.0" to finish this. trying to implement http get /?alloff...

int Value = HIGH ;

void setup()
{
  digitalWrite(2, Value);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(8, INPUT);
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();

}

int PowerOn()
{

  if (digitalRead(8) == LOW) // if computer is off, then we want to turn it on
  {
    Serial.println("powering on");
    digitalWrite(2, LOW); // pressing the power button
    delay(500);
    digitalWrite(2, Value); // Equivalent to releasing the power button after pressing it
  }
  else
  {
    Serial.println("Already on");
  }
}

int PowerOff()
{
  if (digitalRead(8) == Value) // Computer is ON and we want to turn it Off
  {
    delay(20000); // time for smartthings to send ispy alloff command.
    digitalWrite(2, LOW); // pressing the power button
    delay(200); //
    digitalWrite(2, Value); // Equivalent to releasing the power button after pressing it
    delay(1000); // sometimes computer's button only wakes it up from idle mode so pressing one more time
    digitalWrite(2, LOW); // pressing the power button
    delay(200); //
    digitalWrite(2, Value); // Equivalent to releasing the power button after pressing it
    Serial.println("Waiting 40 seconds to check PC's power status");
    delay(40000); // waiting to try again in case it disn't work
    if (digitalRead(8) == Value)
    {
      digitalWrite(2, LOW); // pressing the power button a second time
      delay(200); //
      digitalWrite(2, Value); // Equivalent to releasing the power button after pressing it
      
      delay(40000); // time to shut down before trying again.
    }
  }
  if (digitalRead(8) == Value) // let's check if it worked, if not try again.
  {
    Serial.println("something went wrong, turning off PC by pressing button for 5 seconds to avoid idled bios state");
    digitalWrite(2, LOW); // pressing the power button a second time
      delay(6000); //
      digitalWrite(2, Value); // Equivalent to releasing the power button after pressing it
  } 
}


void loop() {
  /*String toprint = "";
  toprint += "Count is: ";
  toprint += count;
  Serial.println(toprint);*/

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
          if (digitalRead(8) == Value) {
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
  delay(100);
}




