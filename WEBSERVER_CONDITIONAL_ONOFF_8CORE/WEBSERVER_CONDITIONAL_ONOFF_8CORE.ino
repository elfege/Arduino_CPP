#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 10, 190);
byte gateway[] = { 192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network
IPAddress myserver(192, 168, 10, 110);
EthernetServer server(80);
EthernetClient client(); // see "http://forum.arduino.cc/index.php?topic=90210.0" to finish this. trying to implement http get /?alloff...


int Pin = A0;

void setup()
{
  digitalWrite(2, HIGH);
  
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  
   
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
 

}



int PowerOn()
{
  if (digitalRead(Pin) == 0) // if computer is off, then we want to turn it on
  {
    digitalWrite(2, LOW); // pressing the power button
    delay(500);
    digitalWrite(2, HIGH); // Equivalent to releasing the power button after pressing it
  }
}

int PowerOff()
{
  while(digitalRead(Pin) == HIGH) // Computer is ON and we want to turn it Off
  {

    delay(500); // time for smartthings to send ispy alloff command.

    //we always want to check power status to not press the button while it's already off (port 8 connected to fan power)
    if (digitalRead(Pin) == HIGH) {
      digitalWrite(2, LOW); // pressing the power button
      delay(200); // just to wake computer out of idle mode during which it won't receive standby command from power button, for some reason...
      digitalWrite(2, HIGH); // releasing the power
      delay(30000);
      //  checking first if it hasn't turned off yet

      if (digitalRead(Pin) == HIGH) {

        digitalWrite(2, LOW); // pressing the power button
        delay(500); //
        digitalWrite(2, HIGH); //  releasing the power button
        delay(40000);
      }
      // still on?? Damn!! Let's push that damn button again!
      if (digitalRead(Pin) == HIGH) {

        digitalWrite(2, LOW); // pressing the power button
        delay(200); //
        digitalWrite(2, HIGH); //  releasing the power button
        delay(200);
        digitalWrite(2, LOW); // pressing the power button
        delay(200); //
        digitalWrite(2, HIGH); //  releasing the power button
        delay(200);
        digitalWrite(2, LOW); // pressing the power button
        delay(200); //
        digitalWrite(2, HIGH); //  releasing the power button
        delay(200);
        digitalWrite(2, LOW); // pressing the power button
        delay(200); //
        digitalWrite(2, HIGH); //  releasing the power button
        delay(200);
      }
      // now if it's still on let's do a hard turn off.
      if (digitalRead(Pin) == HIGH) {

        digitalWrite(2, LOW); // pressing the power button
        delay(5000); //
        digitalWrite(2, HIGH); //  releasing the power button
        delay(200);
      }
    }
    else
    {
      break;
    }
  }
}


void loop() {
Serial.print("Digital Port value is: ");
Serial.print(digitalRead(Pin));
Serial.println("");

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
          if (digitalRead(Pin) == HIGH) {
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




