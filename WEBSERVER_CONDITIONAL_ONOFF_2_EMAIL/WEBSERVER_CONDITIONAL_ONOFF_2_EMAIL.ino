#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 10, 190);
byte gateway[] = { 192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network
IPAddress myserver(192, 168, 10, 110);
char server[] = " smtp.1and1.com";
int port = 587;

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
  Serial.println(F("Ready. Press 'e' to send."));
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


/// EMAIL LOOPS 

void email()
{
  byte inChar;
 
  inChar = Serial.read();
 
  if(inChar == 'e')
  {
      if(sendEmail()) Serial.println(F("Email sent"));
      else Serial.println(F("Email failed"));
  }
}
 
byte sendEmail()
{
  byte thisByte = 0;
  byte respCode;
 
  if(client.connect(server,port) == 1) {
    Serial.println(F("connected"));
  } else {
    Serial.println(F("connection failed"));
    return 0;
  }
 
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending hello"));
// replace 1.2.3.4 with your Arduino's ip
  client.println("EHLO 1.2.3.4");
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending auth login"));
  client.println("auth login");
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending User"));
// Change to your base64 encoded user
  client.println("xxxx");
 
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending Password"));
// change to your base64 encoded password
  client.println("yyyy");
 
  if(!eRcv()) return 0;
 
// change to your email address (sender)
  Serial.println(F("Sending From"));
  client.println("MAIL From: <me@mydomain.com>");
  if(!eRcv()) return 0;
 
// change to recipient address
  Serial.println(F("Sending To"));
  client.println("RCPT To: <you@yourdomain.com>");
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending DATA"));
  client.println("DATA");
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending email"));
 
// change to recipient address
  client.println("To: You <you@yourdomain.com>");
 
// change to your address
  client.println("From: Me <me@mydomain.com>");
 
  client.println("Subject: Arduino email test\r\n");
 
  client.println("This is from my Arduino!");
 
  client.println(".");
 
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending QUIT"));
  client.println("QUIT");
  if(!eRcv()) return 0;
 
  client.stop();
 
  Serial.println(F("disconnected"));
 
  return 1;
}
 
byte eRcv()
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;
 
  while(!client.available()) {
    delay(1);
    loopCount++;
 
    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }
 
  respCode = client.peek();
 
  while(client.available())
  {  
    thisByte = client.read();    
    Serial.write(thisByte);
  }
 
  if(respCode >= '4')
  {
    efail();
    return 0;  
  }
 
  return 1;
}
 
 
void efail()
{
  byte thisByte = 0;
  int loopCount = 0;
 
  client.println(F("QUIT"));
 
  while(!client.available()) {
    delay(1);
    loopCount++;
 
    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return;
    }
  }
 
  while(client.available())
  {  
    thisByte = client.read();    
    Serial.write(thisByte);
  }
 
  client.stop();
 
  Serial.println(F("disconnected"));
}


