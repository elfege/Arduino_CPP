#include <SPI.h>
#include <Ethernet.h>
#include <ICMPPing.h>


byte mac[] = {0xDE, 0x34, 0x2D, 0x7E, 0x6F, 0xDD};
IPAddress ip(192, 168, 10, 204);
byte gateway[] = { 192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network
IPAddress pingAddr(192, 168, 10, 1); // ip address to ping
EthernetServer server(80);

int closed = 0;
int opened = 0;
int stoped = 0;

long seconds = 1000;
long minutes = seconds * 60;
long hour = (unsigned long int)3600 * (unsigned long int)seconds;
long desiredTimeInHours = 10; // time in hours // if ping test didn't fail for 10 hours, reset anyway
long desiredTimeInMinutes = 10; // time in minutes MUST NOT be less than needed time to open windows
long TimeLimitInHoursMillVal = (unsigned long int)desiredTimeInHours * (unsigned long int)hour;
long TimeLimitInMinutesMillVal = (unsigned long int)desiredTimeInMinutes * (unsigned long int)minutes;

unsigned long time;
unsigned long TimeB4Ping = (unsigned long int)time + (unsigned long int)TimeLimitInMinutesMillVal;
unsigned long TimeB4Reset = (unsigned long int)time + (unsigned long int)TimeLimitInHoursMillVal;
int ForceResetHours = 2;
//int ForceResetTimeInMinutes = 2 ;
long ForceResetMill = ForceResetHours * (unsigned long int)hour;
bool pingTestIsRunning = false;
long millisValueAtTest = 0;


//ping settings
int count;
SOCKET pingSocket = 0;
char buffer [256];
ICMPPing ping(pingSocket, (uint16_t)random(0, 255));


void(* resetFunc) (void) = 0;  // declare reset fuction at address 0

void setup()
{
  digitalWrite(2, LOW);
  digitalWrite(4, HIGH);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);

  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  delay(500);
  PingTest();
}

int Open()
{

  digitalWrite(2, LOW); //
  delay(200);
  digitalWrite(4, LOW); //
  opened = 1;
  closed = 0;
  stoped = 0;

}

int Close()
{

  digitalWrite(2, HIGH); //
  delay(200);
  digitalWrite(4, HIGH); //
  closed = 1 ;
  opened = 0;
  stoped = 0;
 
}

int Stop()
{
  digitalWrite(2, LOW); //
  delay(100);
  digitalWrite(4, HIGH); //
  stoped = 1;
 
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
    Serial.println("");
    delay(500);
    digitalWrite(2, LOW);
    digitalWrite(4, HIGH);
    resetFunc();


  }
  Serial.println(buffer);
  delay(500);
  millisValueAtTest = millis();
  TimeB4Ping = (unsigned long int)time + (unsigned long int)TimeLimitInMinutesMillVal;
  Serial.println("TimeB4Ping = when total millis will reach : "); Serial.print(TimeB4Ping);
  pingTestIsRunning = false;

}

void loop() {

  time = millis();

  /*
    Serial.print("Current Elapsed Time: "); Serial.print(time); Serial.print(" milliseconds");
    //prints time since program started
    Serial.println("");
    Serial.print("desired Time In Minutes: "); Serial.print(desiredTimeInMinutes);
    Serial.println("");
    Serial.print("That is "); Serial.print(TimeLimitInMinutesMillVal); Serial.print(" Milliseconds");
    Serial.println("");
    Serial.print("millisValueAtTest: "); Serial.print(millisValueAtTest);
    Serial.println("");
    Serial.println("");
  */

  // do a ping test every x minutes
  // will reset if failed

  if (time >= TimeB4Ping) {
    Serial.print("testing connection");
    PingTest();
    delay(200);
    while (pingTestIsRunning) {
      ;
    }

    // if ping test didn't fail for 10 hours, reset anyway
    if (time >= TimeB4Reset) {
      Serial.print("Scheduled Reset");
      digitalWrite(2, LOW);
      digitalWrite(4, HIGH);
      resetFunc();
    }

  }

  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
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


          client.print("<form action=\"http://192.168.10.204/\" >");
          if ( opened == 1) {
            client.print("<p align=\"center\">Window is <font color='green'>OPEN</font></p>");
          }
          else if (closed == 1) {
            client.print("<p align=\"center\">Window is <font color='red'>CLOSED</font></p>");
          }
          else if (stoped == 1) {
            client.print("<p align=\"center\">Window is <font color='red'>STOPED</font></p>");
          }
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"12\">STOP</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"11\">CLOSE</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"10\">OPEN</p>");

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
            Close();

          }
          if (buffer.indexOf("GET /?status=10") >= 0)
          {
            Open();

          }
          if (buffer.indexOf("GET /?status=12") >= 0)
          {
            Stop();
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




