
//zoomkat 9-5-11
//routerbot code
//for use with IDE 0021
//open serial monitor to see what the arduino receives
//use the \ slash to escape the " in the html 
//address will look like http://192.168.1.102:84/ when submited
//for use with W5100 based ethernet shields

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168, 10, 190 }; // ip in lan
byte gateway[] = { 192, 168, 10, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(80); //server port

String readString; 

//////////////////////

void setup(){

  pinMode(2, OUTPUT); //pin selected to control
  //start Ethernet
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();

  //enable serial data print 
  Serial.begin(9600); 
  Serial.println("servertest1"); // so I can keep track of what is loaded
}

void loop(){
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (readString.length() < 100) {

          //store characters to string 
          readString += c; 
          //Serial.print(c);
        } 

        //if HTTP request has ended
        if (c == '\n') {

          ///////////////
          Serial.println(readString); //print to serial monitor for debuging 

          //now output HTML data header
             if(readString.indexOf('?') >=0) { //don't send new page
               client.println("HTTP/1.1 204 Zoomkat");
               client.println();
               client.println();  
             }
             else {
          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println();

          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<TITLE>Arduino GET test page</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");

          client.println("<H1>WIFI_SSID's simple Arduino button</H1>");
          
          client.println("<a href=\"/?on\" target=\"inlineframe\">ON</a>"); 
          client.println("<a href=\"/?off\" target=\"inlineframe\">OFF</a>"); 

          //client.println("<IFRAME name=inlineframe src=\"res://D:/WINDOWS/dnserror.htm\" width=1 height=1\">");
          client.println("<IFRAME name=inlineframe style=\"display:none\" >");          
          client.println("</IFRAME>");

          client.println("</BODY>");
          client.println("</HTML>");
             }

          delay(1);
          //stopping client
          client.stop();

          ///////////////////// control arduino pin
       
           
          if(readString.indexOf("on") >0)//checks for on
          {
            digitalWrite(2, HIGH);    // set pin 4 high
            Serial.println("Led On");
          }              
          if(readString.indexOf("off") >0)//checks for off
          {
            digitalWrite(2, LOW);    // set pin 4 low
            Serial.println("Led Off");
          }
          //clearing string for next read
          readString="";

        }
      }
    }
  }
} 

