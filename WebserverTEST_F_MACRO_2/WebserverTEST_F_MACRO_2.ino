
//get submit box code
//for use with IDE 1.0
//open serial monitor to see what the arduino receives
//use the \ slash to escape the " in the html or use a '
//address will look like http://192.168.1.102:84 when submited
//for use with W5100 based ethernet shields
//note that the below bug fix may be required
// http://code.google.com/p/arduino/issues/detail?id=605

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0x02, 0x1A, 0x4A, 0x3E, 0xDC, 0x8E}; // fore the firewall: 02:1A:4A:3E:DC:8E
IPAddress ip(192, 168, 10, 208);
byte gateway[] = { 192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network
IPAddress pingAddr(192, 168, 10, 1); // ip address to ping
EthernetServer server(80);

String readString;

//////////////////////

void setup(){

 pinMode(5, OUTPUT); //pin selected to control
 pinMode(6, OUTPUT); //pin selected to control
 pinMode(7, OUTPUT); //pin selected to control
 pinMode(8, OUTPUT); //pin selected to control
 //start Ethernet
 Ethernet.begin(mac, ip, gateway, gateway, subnet);
 server.begin();

 //enable serial data print
 Serial.begin(9600);
 Serial.println(F("server text box test1")); // so I can keep track of what is loaded
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
         Serial.println(readString); //see what was captured

         //now output HTML data header

client.print(F(  //start F() macro
"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"
"<HTML>"
"<HEAD>"
"<meta name='apple-mobile-web-app-capable' content='yes' />"
"<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />"
"<TITLE>JAVA Page</TITLE>"
"</HEAD>"
"<BODY>"
"<H1>JAVA</H1>"
"<hr />"
"<br />"
"<FORM ACTION='/' method=get >"
"Enter Code: <INPUT TYPE=TEXT NAME='LED' VALUE='' SIZE='25' MAXLENGTH='50'><BR>"
"<br />"
"<input type=submit value='5 ON' style=width:100px;height:45px onClick=location.href='/?on8;'><input type=submit value='5 OFF' style=width:100px;height:45px onClick=location.href='/?off9;'><br />"
"<input type=submit value='6 ON' style=width:100px;height:45px onClick=location.href='/?on8;'><input type=submit value='6 OFF' style=width:100px;height:45px onClick=location.href='/?off9;'><br />"
"<input type=submit value='7 ON' style=width:100px;height:45px onClick=location.href='/?on8;'><input type=submit value='7 OFF' style=width:100px;height:45px onClick=location.href='/?off9;'><br />"
"<input type=submit value='8 ON' style=width:100px;height:45px onClick=location.href='/?on8;'><input type=submit value='8 OFF' style=width:100px;height:45px onClick=location.href='/?off9;'><br />"
"</FORM>"
"</BODY>"
"</HTML>"
));   //end F() macro

         delay(1);
         //stopping client
         client.stop();

         /////////////////////
         if(readString.indexOf("5") >0)//checks for on
         {
           digitalWrite(5, HIGH);    // set pin 5 high
           Serial.println(F("Led On"));
         }
         if(readString.indexOf("50") >0)//checks for off
         {
           digitalWrite(5, LOW);    // set pin 5 low
           Serial.println(F("Led Off"));
         }
         
         if(readString.indexOf("6") >0)//checks for on
         {
           digitalWrite(6, HIGH);    // set pin 6 high
           Serial.println(F("Led 6 On"));
         }
         if(readString.indexOf("60") >0)//checks for off
         {
           digitalWrite(6, LOW);    // set pin 6 low
           Serial.println(F("Led 6 Off"));
         }
         
         if(readString.indexOf("7") >0)//checks for on
         {
           digitalWrite(7, HIGH);    // set pin 7 high
           Serial.println(F("Led On"));
         }
         if(readString.indexOf("70") >0)//checks for off
         {
           digitalWrite(7, LOW);    // set pin 7 low
           Serial.println(F("Led Off"));
         }
         
         if(readString.indexOf("8") >0)//checks for on
         {
           digitalWrite(8, HIGH);    // set pin 8 high
           Serial.println(F("Led On"));
         }
         if(readString.indexOf("80") >0)//checks for off
         {
           digitalWrite(8, LOW);    // set pin 8 low
           Serial.println(F("Led Off"));
         }
         //clearing string for next read
         readString="";

       }
     }
   }
 }
}

