#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,10, 190);
byte gateway[] = { 
  192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 
  255, 255, 255, 0 };    //subnet mask of the network

EthernetServer server(80);

void setup()
{
  Serial.begin(9600);    
  pinMode(2, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT); 
  pinMode(6, OUTPUT); 
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
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
        buffer+=c;       
        if (c == '\n' && currentLineIsBlank) {

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();


          client.print("<form action=\"http://192.168.10.190/\" >");
          if (digitalRead(2)){  
            client.print("<p align=\"center\">Light 1 is <font color='green'>ON</font></p>");
          }
          else{
            client.print("<p align=\"center\">Light 1 is <font color='red'>OFF</font></p>");
          }
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"11\"> ON</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"10\">OFF</p>");
          if (digitalRead(8)){  
            client.print("<p align=\"center\">Light 2 is <font color='green'>ON</font></p>");
          }
          else{
            client.print("<p align=\"center\">Light 2 is <font color='red'>OFF</font></p>");
          }
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"21\"> ON</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"20\">OFF</p>");
          if (digitalRead(7)){  
            client.print("<p align=\"center\">Light 3 is <font color='green'>ON</font></p>");
          }
          else{
            client.print("<p align=\"center\">Light 3 is <font color='red'>OFF</font></p>");
          }
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"31\"> ON</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"30\">OFF</p>");
          if (digitalRead(6)){  
            client.print("<p align=\"center\">Light 4 is <font color='green'>ON</font></p>");
          }
          else{
            client.print("<p align=\"center\">Light 4 is <font color='red'>OFF</font></p>");
          }
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"41\"> ON</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"40\">OFF</p>");
          client.print("<p align=\"center\"><input type=\"submit\" value=\"Submit\"> </p></form>");

          break;
        }
        if (c == '\n') {

          currentLineIsBlank = true;
          buffer="";       
        } 
        else if (c == '\r') {            
          if(buffer.indexOf("GET /?status=11")>=0)
            digitalWrite(2,HIGH);  

          if(buffer.indexOf("GET /?status=10")>=0)
            digitalWrite(2,LOW); 

          if(buffer.indexOf("GET /?status=21")>=0)
            digitalWrite(8,HIGH);  

          if(buffer.indexOf("GET /?status=20")>=0)
            digitalWrite(8,LOW); 

          if(buffer.indexOf("GET /?status=31")>=0)
            digitalWrite(7,HIGH);  

          if(buffer.indexOf("GET /?status=30")>=0)
            digitalWrite(7,LOW); 

          if(buffer.indexOf("GET /?status=41")>=0)
            digitalWrite(6,HIGH);  

          if(buffer.indexOf("GET /?status=40")>=0)
            digitalWrite(6,LOW);

          if(buffer.indexOf("GET /?status=51")>=0){
            digitalWrite(2,HIGH);
            digitalWrite(8,HIGH); 
            digitalWrite(7,HIGH); 
            digitalWrite(6,HIGH); 
          }

          if(buffer.indexOf("GET /?status=50")>=0) {
            digitalWrite(2,LOW);   
            digitalWrite(8,LOW); 
            digitalWrite(7,LOW); 
            digitalWrite(6,LOW); 
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

