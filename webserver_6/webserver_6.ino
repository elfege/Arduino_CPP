#include <SPI.h>
#include <Ethernet.h>

#include "Wire.h"
#define DS1307_ADDRESS 0x68
byte zero = 0x00; //workaround for issue #527
static int dd;
static int mm;
static int yy;
static int s;
static int h;
static int m;
static int wkDay;
int CENTURY=2000;
int RTC_ERROR_FLAG=0;


// MAC address from Ethernet shield sticker under board
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 10, 190); // IP address, may need to change depending on network
EthernetServer server(80);  // create a server at port 80

String HTTP_req;          // stores the HTTP request
boolean LED_status = 0;   // state of LED, off by default

void setup()
{
  Wire.begin();  
  Ethernet.begin(mac, ip);  // initialize Ethernet device
  server.begin();           // start to listen for clients
  Serial.begin(9600);       // for diagnostics
  pinMode(8, OUTPUT);       // LED on pin 2
}

void loop()
{
  //getRTCDateTime();
  EthernetClient client = server.available();  // try to get client

  if (client) {  // got client?
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {   // client data available to read
        char c = client.read(); // read 1 byte (character) from client
        HTTP_req += c;  // save the HTTP request 1 char at a time
        // last line of client request is blank and ends with \n
        // respond to client only after last line received
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          // send web page
          client.println("<!DOCTYPE html>");
          client.println("<html>");
          client.println("<head>");
          client.println("<title>Arduino LED Control</title>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h1>EDIT</h1>");
          client.println("<p>Click to switch LED on and off.</p>");
          client.println("<form method=\"get\">");
          ProcessCheckbox(client);
          client.println("<br />");
          client.println("<br />");
          client.print("LOCAL DATE :"); 
          client.print(dd);
          client.print("/");
          client.print(mm);
          client.print("/");
          client.print(yy); 
          client.println("<br />");   
          client.print("LOCAL TIME :"); 
          client.print(h);
          client.print("/");
          client.print(m);
          client.print("/");
          client.print(s); 
          client.println("<br />");
          client.println("</html>");
          break;




          client.println("</form>");
          client.println("</body>");
          client.println("</html>");
          Serial.print(HTTP_req);
          HTTP_req = "";    // finished with request, empty string
          break;
        }
        // every line of text received from the client ends with \r\n
        if (c == '\n') {
          // last character on line of received text
          // starting new line with next character read
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // a text character was received from client
          currentLineIsBlank = false;
        }
      } // end if (client.available())
    } // end while (client.connected())
    delay(1);      // give the web browser time to receive the data
    client.stop(); // close the connection
  } // end if (client)
}

// switch LED and send back HTML for LED checkbox
void ProcessCheckbox(EthernetClient cl)
{
  /*  if (HTTP_req.indexOf("LED2=2") > -1) {  // see if checkbox was clicked
        // the checkbox was clicked, toggle the LED
        if (LED_status) {
            LED_status = 0;
        }
        else {
            LED_status = 1;
        }
    }
    
    if (LED_status) {    // switch LED on
        digitalWrite(8, HIGH);
        // checkbox is checked
        cl.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" \
        onclick=\"submit();\" checked>LED2");
    }
    else {              // switch LED off
        digitalWrite(8, LOW);
        // checkbox is unchecked
        cl.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" \
        onclick=\"submit();\">LED2");
    }
    
    */
    if (HTTP_req.indexOf("LED2=2") > -1) 
    { 
     if (LED_status) {
            LED_status = 0;
        }
        else {
            LED_status = 1;
        }
    }
    
    if (LED_status) 
    { 
      rtcWrite(s,m,h,2,dd,mm, yy);
      cl.print("IN SET MODE");
      //setDateTime();
       digitalWrite(8, HIGH);
      cl.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" \
        onclick=\"submit();\" checked>LED2");
      
    }else
    {
     cl.print("read mode");
       digitalWrite(8, LOW);
      getRTCDateTime();
       cl.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" \
        onclick=\"submit();\">LED2");
    }
    
    
}

