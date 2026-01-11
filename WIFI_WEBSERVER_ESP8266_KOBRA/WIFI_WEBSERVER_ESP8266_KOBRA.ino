#include <ESP8266Ping.h>



int seconds = 1000;
long minutes = seconds * 60;
long hour = (unsigned long int)3600 * (unsigned long int)seconds;
int desiredTimeInHours = 10; // time in hours
int desiredTimeInMinutes = 1; // time in minutes
long TimeLimitInHours = (unsigned long int)desiredTimeInHours * (unsigned long int)hour;
long TimeLimitInMinutesMillVal = (unsigned long int)desiredTimeInMinutes * (unsigned long int)minutes;
int elapsed;// variable redefined below with time()

int ForceResetHours = 2;
//int ForceResetTimeInMinutes = 2 ;
long ForceResetMill = ForceResetHours * (unsigned long int)hour;


bool pingTestIsRunning = false;
int timea = 0;
int countFailedPings = 0;
long millisValueAtTest = 0;

//NodeMCU has weird pin mapping.
//Pin numbers written on the board itself do not correspond to ESP8266 GPIO pin numbers.
//We have constants defined to make using this board easier:
/*
  static const uint8_t D0   = 16;
  static const uint8_t D1   = 5;
  static const uint8_t D2   = 4;
  static const uint8_t D3   = 0;
  static const uint8_t D4   = 2;
  static const uint8_t D5   = 14;
  static const uint8_t D6   = 12;
  static const uint8_t D7   = 13;
  static const uint8_t D8   = 15;
  static const uint8_t D9   = 3;
  static const uint8_t D10  = 1;
*/
//If you want to use NodeMCU pin 5, use D5 for pin number, and it will be translated to 'real' GPIO pin 14

int const LED = D4; //  TX's LED (builtin led taken as input)
int const RELAY = D5;  // RELAY output


int const Run = D0; // isRunning input (GPIO16)
int const Awk = A0; // awake input



// FYI LED is D0...


bool wakingup = false;
bool turnOffRequested = false;
bool Alreadyrestarted = false;
bool reseting = false;
int resetcount = 0;

int count;
int AWAKE;
int AwakeVolt = 600;
int RUNNING;


int blinks;
int blinksCount;
int lapse = 200;


// WiFi parameters
const char* ssid = "WIFI_SSID2";
const char* password = "WIFI_PASSWORDGahsh4479#$";

WiFiServer server(80);
void(* resetFunc) (void) = 0;  // declare reset fuction at address 0

String IP;

void setup(void)
{
WiFi.mode(WIFI_STA); // turn off ai-thinker broadcast and set as station

  // Start Serial
  Serial.begin(115000);
  delay(10);
  Serial.println("SERIAL STARTED");
  pinMode(LED, OUTPUT); // LED

  pinMode(RELAY, OUTPUT); // Relay
  digitalWrite(RELAY, 1); // off


  pinMode(Awk, INPUT); // awake input

  pinMode(Run, INPUT_PULLDOWN_16); // isRunning input


  // Connect to WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {


    digitalWrite(LED, 1);
    delay(50);
    digitalWrite(LED, 0);
    delay(50);
    count++;
    Serial.println(count);
    if (count >= 200) {
      Serial.println("RESETING");
      delay(500);
      resetFunc();
    }


  }
  Serial.println("");
  Serial.println("WiFi connected");


  // Print the IP address
  Serial.println(WiFi.localIP());
 
  server.begin();
  Serial.println("Server started");

  // led signals successful connection

  lapse = 200;
  blinks = 4;
  Blink();


}

boolean isAwake() {
  bool resultAwake;
  //Serial.println("isAwake function called");
  //Serial.println("");
  //Serial.print("AwakeVal = "); Serial.print(AwakeVal);
  int AwakeVal = analogRead(Awk);

  if (AwakeVal > AwakeVolt) {
    resultAwake = true;
  }
  else {
    resultAwake = false;
  }
  return resultAwake;
}

boolean isRunning() {
  bool resultRun;
  //Serial.println("isRunning function called");
  int isRunVal = digitalRead(Run);
  //Serial.println("");
  //Serial.print("isRunVal = "); Serial.print(isRunVal);
  if (isRunVal == HIGH) {
    resultRun = true;
  }
  else {
    resultRun = false;
  }
  return resultRun;
}



void loop() {

 String IP = WiFi.localIP().toString();

  timea = millis();

  // wait  so as not to send massive amounts of data
  delay(100);


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

  WiFiClient client = server.available();   // listen for incoming clients

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
            client.println("<H1 style='font-size:400%;'>KOBRA</H1>");

            client.println(IP);


            client.println("<br />");
            client.println("<br />");
            client.println("<br />");
            client.println("<strong>");
            client.println("<a href=\"/H\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:50px; position:center; top:530px; left:300px;'>   start  </button></a>");
            client.println("<br />");
            client.println("<a href=\"/L\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   stop   </button></a>");

            client.println("<br />");
            client.println("<a href=\"/R\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   reset controller  </button></a>");
            client.println("<br />");

            client.println("<a href='http://www.elfege.com/'\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  TEST </button></a>");
            client.println("<br />");

            /*
                        String Address = WiFi.localIP().toString();
                        String url1 = "<meta http-equiv='refresh' content='5; url=http://";
                        String url2 = "'/>";
                        String UrlFinal = url1 + Address + url2 ;
                        Serial.println(UrlFinal);
                        client.print(UrlFinal);
            */

            client.println();

            client.println("</center>");
            client.println("</strong>");
            client.println("</BODY>");
            client.println("</HTML>");



            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          Alreadyrestarted = false;
          On();
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
          String UrlFinal = url1 + IP + url2 ;
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


}

void Blink() {
  blinksCount = 0;

  while (blinksCount < blinks) {
    digitalWrite(LED, 1); // led off
    delay(lapse);
    digitalWrite(LED, 0); // led on
    delay(lapse);
    blinksCount++;
  }
  digitalWrite(LED, 0); // led on


}

void Reset() {
  WiFiClient client = server.available();   // listen for incoming clients
  delay(1000);
  client.stop();
  Serial.println("client disonnected");
  Serial.println("");
  Serial.print("Reseting in 3 seconds");
  blinks = 60;
  lapse = 50;
  Blink();
  resetFunc();
}

void On() {
  turnOffRequested = false;

  blinks = 10;
  lapse = 20;
  Blink();

  digitalWrite(RELAY, 1); // Turn On


}

void restartRobot() {

  digitalWrite(RELAY, 0);// press and hold
  Alreadyrestarted = true;
  blinks = 5;
  lapse = 1000;
  Blink();


  digitalWrite(RELAY, 1);// release button

  On();

  Alreadyrestarted = true;

}

void TurnOff() {
  turnOffRequested = true;

  digitalWrite(RELAY, 0);// Turn Off
  blinks = 2;
  lapse = 1000;
  Blink();

}

void WakeUp() {

  On();

  delay(2000);
  wakingup = false;

}

void ping() {
  pingTestIsRunning = true;
  IPAddress ip (192, 168, 10, 1); // The remote ip to ping

  bool ret = Ping.ping(ip);

  int avg_time_ms = Ping.averageTime();

  if (ret) {

    Serial.println("PING SUCCESS! Average time is: ");
    Serial.print(avg_time_ms);
    Serial.println("");


    pingTestIsRunning = false;
    millisValueAtTest = millis();

  }
  else {

    if (countFailedPings <= 1) {
      countFailedPings++;
      Serial.println("PING FAILED, waiting to try again");
      waitAndTryAgain();
    }
    else {
      Serial.println("PING FAILED 3 times. Reseting board");
      Serial.println("");

      resetFunc();
    }
  }

  pingTestIsRunning = false;

}

void waitAndTryAgain() {
  delay(10000);
  ping();
}



