#include <Servo.h>

#include <SPI.h>
#include <Ethernet.h>.
#include <ICMPPing.h>




long seconds = 1000;
long minutes = (unsigned long int)seconds * 60;
long hour = (unsigned long int)3600 * (unsigned long int)seconds;
int desiredTimeInHours = 1; // time in hours
int desiredTimeInMinutes = 120; // time in minutes
long TimeLimitInHoursMillVal = (unsigned long int)desiredTimeInHours * (unsigned long int)hour;
long TimeLimitInMinutesMillVal = desiredTimeInMinutes * (unsigned long int)minutes;
int elapsed;// variable redefined below with time()



int ForceResetHours = 2;
//int ForceResetTimeInMinutes = 2 ;
long ForceResetMill = ForceResetHours * (unsigned long int)hour;


bool pingTestIsRunning = false;
unsigned long time;
unsigned long Val;
int countFailedPings = 0;
long millisValueAtTest = 0;

int count;

static byte g_abyMyMacAddress[] = {0x02, 0x1A, 0x4B, 0x38, 0x1D, 0x8D};
IPAddress ip(192, 168, 10, 208);
byte gateway[] = { 192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network
IPAddress myserver(192, 168, 10, 110);
EthernetServer server(80);
EthernetClient client(); // see "http://forum.arduino.cc/index.php?topic=90210.0" to finish this. trying to implement http get /?alloff...


SOCKET pingSocket = 0;
char buffer [256];
ICMPPing ping(pingSocket, (uint16_t)random(0, 255));
IPAddress pingAddr(192, 168, 10, 1); // ip address to ping


//Testing the DC Motors
//Define Pins
//Motor A
int pwm = 5;
int pinA1 = 2;
int pinA2 = 3;
int LockPin = 7;


//speeds
int SpeedVal = 150; //  speed
int SpeedValNow; // variable for monitoring changes in SpeedVal value
int slowmode;
int fastmode;


Servo Lift;
int ServoPos = 0;

void(* resetFunc) (void) = 0;  // declare reset fuction at address 0

void setup()
{


  Serial.begin(9600);
  pinMode (pwm, OUTPUT);
  pinMode (pinA1, OUTPUT);
  pinMode (pinA2, OUTPUT);
  pinMode(LockPin, INPUT);
  //pinMode(22, OUTPUT);
  Lift.attach(28);
  Lift.write(1);
  delay(500);
  Lift.write(180);

  digitalWrite(pinA1, LOW);
  digitalWrite(pinA2, LOW);

  Ethernet.begin(g_abyMyMacAddress, ip, gateway, subnet);
  server.begin();
  Serial.println(Ethernet.localIP());

  Serial.println(seconds);
  Serial.println(minutes);
  Serial.println(hour);
  Serial.println(desiredTimeInHours);
  Serial.println(TimeLimitInHoursMillVal);
  Serial.println(TimeLimitInMinutesMillVal);


}


void loop() {

  Serial.println("");
  bool pressed = digitalRead(LockPin);
  Serial.print("PRESSED? : "); Serial.print(pressed);
  Serial.println("");

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

    // wait  so as not to send massive amounts of data
    delay(500);
  */



  // do a ping test every x minutes
  Val = (unsigned long int)millisValueAtTest + (unsigned long int)TimeLimitInHoursMillVal;

  Serial.println("");
  Serial.print("millisValueAtTest: "); Serial.print(millisValueAtTest);
  Serial.println("");
  Serial.println("");
  Serial.print("TimeLimitInHoursMillVal: "); Serial.print(TimeLimitInHoursMillVal);
  Serial.println("");
  Serial.println("");
  Serial.print("time: "); Serial.print(time);
  Serial.println("");
  Serial.print("Val: "); Serial.print(Val);
  Serial.println("");

  if (time >= Val) {
    Serial.print("testing Internet connection");
    PingIt();
    delay(200);
    while (pingTestIsRunning) {
      ;
    }

  }


  EthernetClient client = server.available();
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            // refresh the page automatically every 1 sec
            client.println("Refresh: 1");
            client.println();

            // the content of the HTTP response follows the header:
            client.println("<HTML>");
            client.println("<BODY style='background-color:black'>");
            client.println("<font style='color:green'>");
            client.println("<font style='font-family:electric toaster'>");
            client.println("<center>");
            client.println("<H1 style='font-size:400%;'>CAN OPENER</H1>");

            client.println("<br />");
            client.println(Ethernet.localIP());
            client.println("<br />");

            client.println("<strong>");
            client.println("<a href=\"/Unlock\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:50px; position:center; top:530px; left:300px;'>   Unlock  </button></a>");
            client.println("<br />");
            client.println("<a href=\"/Lock\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Lock   </button></a>");
            client.println("<br />");
            client.println("<a href=\"/on\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  RUN </button></a>");
            client.println("<br />");
            client.println("<a href=\"/off\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  STOP </button></a>");
            client.println("<br />");
            client.println("<a href=\"/lift\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   UP  </button></a>");
            client.println("<br />");
            client.println("<a href=\"/release\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   RELEASE  </button></a>");
            client.println("<br />");
            client.println("<a href=\"/R\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   reset controller  </button></a>");


            client.println("</center>");
            client.println("</strong>");
            client.println("</BODY>");
            client.println("</HTML>");



            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /on")) {

          On();
        }
        if (currentLine.endsWith("GET /off")) {

          Off();
        }
        if (currentLine.endsWith("GET /Lock")) {

          Lock();

        }
        if (currentLine.endsWith("GET /Unlock")) {
          Unlock();
        }
        if (currentLine.endsWith("GET /lift")) {
          ServoPos = 1;
          RunServo();
        }
        if (currentLine.endsWith("GET /release")) {

          ServoPos = 180;
          RunServo();
        }

        if (currentLine.endsWith("GET /R")) {

          String Message1 = "<a style='font-size:400%;'>This page will refresh in </a>";
          String refreshtime = "<a style='font-size:400%;'>30</a>";
          String Message2 = "<a style='font-size:400%;'> seconds</a>";
          String MessageFinal = Message1 + refreshtime + Message2;


          // the content of the HTTP response follows the header:
          client.println("<HTML>");
          client.println("<BODY style='background-color:black'>");
          client.println("<font style='color:green'>");
          client.println("<font style='font-family:electric toaster'>");
          client.println("<center>");
          client.println("<H1 style='font-size:400%;'>RESETING CONTROLER</H1>");
          client.println();
          client.print(MessageFinal);


          //client.print("<meta http-equiv='refresh' content='10; url=http://philonyc.com/' />");


          String url1 = "<meta http-equiv='refresh' content='20; url=http://";
          String url2 = "'/>";
          String UrlFinal = url1 + Ethernet.localIP() + url2 ;
          Serial.println(UrlFinal);
          client.print(UrlFinal);


          client.println();

          client.println("</center>");
          client.println("</strong>");
          client.println("</BODY>");
          client.println("</HTML>");

          client.stop();

          Reset();
          break;

        }

      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }

  delay(100);
}

void RunServo() {

  Lift.write(ServoPos);
}

void On()
{
  analogWrite(pwm, SpeedVal);
  delay(500);
  digitalWrite (pinA1, LOW );
  digitalWrite (pinA2, HIGH);
  isBeingPressed(); // just for test purpose
  SpeedValNow = SpeedVal; // recording SpeedVal Current Value
}

void Off()
{

  analogWrite(pwm, SpeedVal);
  digitalWrite (pinA1, LOW);
  digitalWrite (pinA2, LOW);

}

boolean isBeingPressed()
{
  bool result = false;

  int ReadTest = digitalRead(LockPin);
  Serial.println("");
  Serial.print("LockPin is "); Serial.print("----------------> "); Serial.print(ReadTest);
  Serial.println("");
  if (ReadTest == HIGH) {
    result = true;
  }
  else {
    result = false;
  }
  return result;
}

void Unlock()
{
  Off(); // reset the ports status
  analogWrite(pwm, SpeedVal);
  count = 0;
  digitalWrite (pinA1, HIGH);
  digitalWrite (pinA2, LOW);
  while (!isBeingPressed() && count < 25) {
    count++;
    delay(100);
  }
  Off();
}

void Lock()
{
  Off(); // reset the ports status
  analogWrite(pwm, SpeedVal);
  count = 0;
  digitalWrite (pinA1, LOW);
  digitalWrite (pinA2, HIGH);
  while (isBeingPressed() && count < 30) {
    count++;
    delay(100);
  }

  Off();
}

void Reset() {

  Serial.println("client disonnected");
  Serial.println("");
  Serial.print("Reseting in 3 seconds");
  delay(3000);
  resetFunc();
}


void PingIt() {
  pingTestIsRunning = true;
  ICMPEchoReply echoReply = ping(pingAddr, 4);
  if (echoReply.status == SUCCESS)
  {
    sprintf(buffer,
            "Reply[%d] from: %d.%d.%d.%d: bytes=%d time=%ldms TTL=%d",
            echoReply.data.seq,
            echoReply.addr[0],
            echoReply.addr[1],
            echoReply.addr[2],
            echoReply.addr[3],
            REQ_DATASIZE,
            millis() - echoReply.data.time,
            echoReply.ttl);

    Serial.println("SUCCESS!");
    millisValueAtTest = millis();

  }
  else
  {
    sprintf(buffer, "Echo request failed; %d", echoReply.status);
    Reset();
  }
  Serial.println(buffer);
  delay(500);
  pingTestIsRunning = false;

}

