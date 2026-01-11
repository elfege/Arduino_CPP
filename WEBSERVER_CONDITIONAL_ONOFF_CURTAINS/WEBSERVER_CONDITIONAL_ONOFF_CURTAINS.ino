#include <SPI.h>
#include <Ethernet.h>
#include <ICMPPing.h>

byte mac[] = {0xDE, 0xE4, 0x2D, 0x7E, 0xFF, 0xB1};
IPAddress ip(192, 168, 10, 193);
byte gateway[] = { 192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network
IPAddress pingAddr(192, 168, 10, 1); // ip address to ping
EthernetServer server(80);
//EthernetClient client();

long seconds = 1000;
long minutes = seconds * 60;
long hour = (unsigned long int)3600 * (unsigned long int)seconds;
long desiredTimeInHours = 1; // time in hours
long desiredTimeInMinutes = 10; // time in minutes
long TimeLimitInHours = (unsigned long int)desiredTimeInHours * (unsigned long int)hour;
long TimeLimitInMinutesMillVal = (unsigned long int)desiredTimeInMinutes * (unsigned long int)minutes;

unsigned long time;
unsigned long Val = (unsigned long int)time + (unsigned long int)TimeLimitInMinutesMillVal;


int ForceResetHours = 12;
//int ForceResetTimeInMinutes = 2 ;
unsigned long ForceResetMill = ForceResetHours * (unsigned long int)hour;

bool pingTestIsRunning = false;
long millisValueAtTest = 0;

//ping settings
//int count;

SOCKET pingSocket = 0;
char buffer [256];
ICMPPing ping(pingSocket, (uint16_t)random(0, 255));

void(* resetFunc) (void) = 0;  // declare reset fuction at address 0

void setup()
{
  digitalWrite(2, HIGH);  // open orange wire
  digitalWrite(5, HIGH); // close yellow wire
  digitalWrite(8, HIGH); // stop red wire
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(8, OUTPUT);
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  PingTest();

}

int PingTest() {
  pingTestIsRunning = true;

  ICMPEchoReply echoReply = ping(pingAddr, 4);
  if (echoReply.status == SUCCESS)
  {

    Serial.println("SUCCESS!");

  }
  else
  {
    sprintf(buffer, "Echo request failed; %d", echoReply.status);
    Serial.println("RESET");
    delay(300);
    resetFunc();

  }
  Serial.println(buffer);
  delay(500);
  millisValueAtTest = millis();
  Val = (unsigned long int)millisValueAtTest + (unsigned long int)TimeLimitInMinutesMillVal;
  Serial.println("Val = "); Serial.print(Val);
  pingTestIsRunning = false;

}

int OpenCurtains()
{

  digitalWrite(2, LOW); // Opening Curtains
  delay(200);
  digitalWrite(2, HIGH);
  /*delay(15000);
    stopall();
  */
}


int CloseCurtains()
{
  digitalWrite(5, LOW); // Closing Curtains
  delay(200);
  digitalWrite(5, HIGH);
  /*delay(15000);
    stopall();
  */
}
int stopall()
{

  digitalWrite(8, LOW); // stop
  delay(200);
  digitalWrite(8, HIGH); // stop

}
void loop() {
  time = millis();
  /*
    Serial.println("");
    Serial.print("Val = "); Serial.print(Val);
    Serial.println("");

    //prints time since program started
    Serial.print("Current Elapsed Time: "); Serial.print(time); Serial.print(" milliseconds");


    Serial.println("");
    Serial.print("desired Time In Minutes: "); Serial.print(desiredTimeInMinutes);
    Serial.println("");
    Serial.print("That is "); Serial.print(TimeLimitInMinutesMillVal); Serial.print(" Milliseconds");
    Serial.println("");
    Serial.print("millisValueAtTest: "); Serial.print(millisValueAtTest);
    Serial.println("");
    Serial.println("");

    delay(1000);
  */

  // do a ping test every x minutes

  if (time >= Val) {
    Serial.print("testing connection");
    PingTest();
    delay(200);
    while (pingTestIsRunning) {
      ;
    }
  } 
  else if(time > ForceResetMill){
    resetFunc();
  }

  EthernetClient client = server.available();
  if (client) {

    boolean currentLineIsBlank = true;
    boolean CurtainsAreOpen = true;
    String buffer = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
        buffer += c;
        if (c == '\n' && currentLineIsBlank) {

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();

          client.print("<form action=\"http://192.168.10.193:80/\" >");
          if (CurtainsAreOpen == true) {
            client.print("<p align=\"center\">LIVING curtains are <font color='green'>OPEN</font></p>");
          }
          else if (CurtainsAreOpen == false) {
            client.print("<p align=\"center\">LIVING Curtains are <font color='red'>CLOSED</font></p>");
          }
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"12\">STOP</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"11\">CLOSE CURTAIN</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"10\">OPEN CURTAIN</p>");
          client.print("<p align=\"center\"><input type=\"submit\" value=\"Submit\"> </p></form>");

          break;
        }
        if (c == '\n') {

          currentLineIsBlank = true;
          buffer = "";
        }
        else if (c == '\r') {
          if (buffer.indexOf("GET /?status=12") >= 0)
          {

            stopall();

          }
          if (buffer.indexOf("GET /?status=11") >= 0)
          {
            CurtainsAreOpen = false;
            CloseCurtains();

          }
          if (buffer.indexOf("GET /?status=10") >= 0)
          {
            CurtainsAreOpen = true;
            OpenCurtains();

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




