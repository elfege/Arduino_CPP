#include <SPI.h>
#include <Ethernet.h>

static byte g_abyMyMacAddress[] = {0x02, 0x1A, 0x4B, 0x38, 0x1D, 0x8D};
IPAddress ip(192, 168, 10, 207);
byte gateway[] = { 192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network
IPAddress myserver(192, 168, 10, 110);
EthernetServer server(80);
EthernetClient client(); // see "http://forum.arduino.cc/index.php?topic=90210.0" to finish this. trying to implement http get /?alloff...


//Testing the DC Motors
//Define Pins
//Motor A
int pwm = 5;
int pinA1 = 2;
int pinA2 = 3;

//speeds
int SpeedVal = 50; // default value is medium speed
int SpeedValNow; // variable for monitoring changes in SpeedVal value
int slowmode;
int fastmode;


void setup()
{
  digitalWrite(2, LOW);
  Serial.begin(9600);
  pinMode (pwm, OUTPUT);
  pinMode (pinA1, OUTPUT);
  pinMode (pinA2, OUTPUT);
  pinMode(8, INPUT);
  Ethernet.begin(g_abyMyMacAddress, ip, gateway, subnet);
  server.begin();

}

void On()
{
  analogWrite(pwm, SpeedVal);
  delay(500);
  digitalWrite (pinA1, LOW);
  digitalWrite (pinA2, HIGH);
  closed = 0;
  opened = 1;
  stopped = 0;
  SpeedValNow = SpeedVal; // recording SpeedVal Current Value
}

void Off()
{


}

void LockIn()
{

}


void loop() {

  timea = millis();
  /*
    Serial.print("Current Elapsed Time: "); Serial.print(timea); Serial.print(" milliseconds");
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
  // wait  so as not to send massive amounts of data
  delay(500);


  // do a ping test every x minutes
  int Val = (unsigned long int)millisValueAtTest + (unsigned long int)TimeLimitInHours;

  if (timea >= Val) {
    Serial.print("testing Internet connection");
    ping();
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
            client.println("<H1 style='font-size:400%;'>NEATO Vacuum Control</H1>");

            if (isAwake()) {

              if (isRunning()) {
                client.println("<br />");
                client.println("<H3 style='font-size:100%;'>NEATO Vacuum is CLEANING</H3>");
              }
              else {
                client.println("<br />");
                client.println("<H3 style='font-size:100%;'>NEATO Vacuum is ON but not cleaning</H3>");
              }
            }
            else {
              client.println("<H3 style='font-size:100%;'>NEATO Vacuum in SLEEP MODE</H3>");
            }
            if (wakingup && !isAwake()) {
              client.println("<br />");
              client.println("<H3 style='font-size:100%;'>Waking up NEATO Vacuum</H3>");
            }
            else if (isAwake() && !isRunning()) {
              client.println("<br />");
              client.println("<H3 style='font-size:100%;'>NEATO is AWAKE</H3>");
            }

            client.println("<br />");
            client.println("<br />");
            client.println("<br />");
            client.println("<strong>");
            client.println("<a href=\"/H\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:50px; position:center; top:530px; left:300px;'>   start  </button></a>");
            client.println("<br />");
            client.println("<a href=\"/L\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   stop   </button></a>");
            client.println("<br />");
            client.println("<a href=\"/W\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  wake up </button></a>");
            client.println("<br />");
            client.println("<a href=\"/boot\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  RESTART Neato </button></a>");

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
        if (currentLine.endsWith("GET /H")) {
          Alreadyrestarted = false;
          TurnOn();
        }
        if (currentLine.endsWith("GET /L")) {

          TurnOff();

        }
        if (currentLine.endsWith("GET /boot")) {
          restartRobot();

        }
        if (currentLine.endsWith("GET /W")) {
          wakingup = true;

          if (!isAwake()) {
            WakeUp();
          }

        }
        if (currentLine.endsWith("GET /R")) {

          client.println("<H1 style='font-size:100%;'>RESETING power controller</H1>");
          Reset();
          break;

        }

      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }


}



