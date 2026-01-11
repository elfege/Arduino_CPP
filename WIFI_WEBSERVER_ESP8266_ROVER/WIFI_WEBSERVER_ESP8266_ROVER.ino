#include <ESP8266Ping.h>
#include <ESP8266WiFi.h>

int seconds = 1000;
long minutes = seconds * 60;
long hour = (unsigned long int)3600 * (unsigned long int)seconds;
int desiredTimeInHours = 1; // time in hours
int desiredTimeInMinutes = 1; // time in minutes
long TimeLimitInHours = (unsigned long int)desiredTimeInHours * (unsigned long int)hour;
long TimeLimitInMinutesMillVal = (unsigned long int)desiredTimeInMinutes * (unsigned long int)minutes;
int elapsed;// variable redefined below with time()

int ForceResetHours = 2;
//int ForceResetTimeInMinutes = 2 ;
long ForceResetMill = ForceResetHours * (unsigned long int)hour;


bool pingTestIsRunning = false;
int timea = 0;
long millisValueAtTest = 0;
// unsigned long time;

/* ports are:
    12
    13
    14
    16

*/

int gpioA = 12;
int gpioB = 13;
int gpioC = 14;
int gpioD = 16;


bool isOn = false;
bool Aux1isOn = false;
bool Aux2isOn = false;
bool Aux3isOn = false;

int resetcount = 0;

int count;
int Status;

// WiFi parameters
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORDGahsh4479#$";

WiFiServer server(80);
void(* resetFunc) (void) = 0;  // declare reset fuction at address 0

void setup(void)
{


  // Start Serial
  Serial.begin(115200);
  delay(10);
  pinMode(gpioA, OUTPUT);// Initialize gpioA as an output
  pinMode(gpioB, OUTPUT);
  pinMode(gpioC, OUTPUT);
  pinMode(gpioD, OUTPUT);


  delay(200);
  digitalWrite(gpioA, 1); // to allow rover's batteries' PCB to reset after power loss 
  digitalWrite(gpioB, 0);
  digitalWrite(gpioC, 0);
  digitalWrite(gpioD, 0);

  
  delay(2000);
  digitalWrite(gpioA, 0); // keep rover's off by default after reset
  digitalWrite(gpioB, 0);
  digitalWrite(gpioC, 0);
  digitalWrite(gpioD, 0);




  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(count);
    count++;
    if (count >= 20) {
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
  
  // wait  so as not to send massive amounts of data
  delay(500);

  // reset the board every x hours
  //Serial.println("Forced Reset time elapsed? ");
  if (millisValueAtTest > ForceResetMill) {
    Serial.println("Forced Reset time elapsed, reseting board");
    resetFunc();
  }
*/
  // do a ping test every x minutes
  int Val = (unsigned long int)millisValueAtTest + (unsigned long int)TimeLimitInMinutesMillVal;

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

            // the content of the HTTP response follows the header:
            client.println("<HTML>");
            client.println("<BODY style='background-color:black'>");
            client.println("<font style='color:green'>");
            client.println("<font style='font-family:electric toaster'>");
            client.println("<center>");
            client.println("<H1 style='font-size:400%;'>ROVER POWER CONTROL</H1>");

            if (isOn) {
              client.println("<br />");
              client.println("<H2 style='font-size:100%;'>ROVER IS ON</H2>");
            }
            else {
              client.println("<br />");
              client.println("<H3 style='font-size:100%;'>ROVER IS OFF</H3>");
            }
            if (Aux1isOn) {
              client.println("<br />");
              client.println("<H2 style='font-size:100%;'>Aux1 is ON</H2>");
            } else {
              client.println("<br />");
              client.println("<H3 style='font-size:100%;'>Aux1 is OFF</H3>");
            }
            if (Aux2isOn) {
              client.println("<br />");
              client.println("<H2 style='font-size:100%;'>Aux2 is ON</H2>");
            } else {
              client.println("<br />");
              client.println("<H3 style='font-size:100%;'>Aux2 is OFF</H3>");
            }
            if (Aux3isOn) {
              client.println("<br />");
              client.println("<H2 style='font-size:100%;'>Aux3 is ON</H2>");
            } else {
              client.println("<br />");
              client.println("<H3 style='font-size:100%;'>Aux3 is OFF</H3>");
            }

            client.println("<br />");
            client.println("<br />");
            client.println("<br />");
            client.println("<strong>");
            //inverted command values here due to power on after power loss feature
            client.println("<a href=\"/L\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:50px; position:center; top:530px; left:300px;'>   ROVER  ON  </button></a>");
            client.println("<br />");
            client.println("<a href=\"/H\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   ROVER OFF   </button></a>");
            client.println("<br />");
            client.println("<br />");
            client.println("<a href=\"/H1\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  AUX1  ON  </button></a>");
            client.println("<br />");
            client.println("<a href=\"/L1\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  AUX1 OFF </button></a>");
            client.println("<br />");
            client.println("<br />");
            client.println("<a href=\"/H2\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  AUX2  ON  </button></a>");
            client.println("<br />");
            client.println("<a href=\"/L2\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  AUX2 OFF </button></a>");
            client.println("<br />");
            client.println("<br />");
            client.println("<a href=\"/H3\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  AUX3  ON  </button></a>");
            client.println("<br />");
            client.println("<a href=\"/L3\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  AUX3 OFF </button></a>");
            client.println("<br />");
            client.println("<br />");
            client.println("<a href=\"/HALL\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  ALL  ON  </button></a>");
            client.println("<br />");
            client.println("<a href=\"/LALL\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  ALL OFF </button></a>");

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
          isOn = true;
          TurnOn();
          
        }
        if (currentLine.endsWith("GET /L")) {
          isOn = false;
          TurnOff();
          
        }
        if (currentLine.endsWith("GET /H1")) {

          Aux1isOn = true;
          Aux1On();
        }
        if (currentLine.endsWith("GET /L1")) {

          Aux1isOn = false;
          Aux1Off();
        }
        if (currentLine.endsWith("GET /H2")) {

          Aux2isOn = true;
          Aux2On();
        }
        if (currentLine.endsWith("GET /L2")) {

          Aux2isOn = false;
          Aux2Off();
        }
        if (currentLine.endsWith("GET /H3")) {

          Aux3isOn = true;
          Aux3On();
        }
        if (currentLine.endsWith("GET /L3")) {

          Aux3isOn = false;
          Aux3Off();
        }
        if (currentLine.endsWith("GET /HALL")) {
          isOn = true;
          Aux1isOn = true;
          Aux2isOn = true;
          Aux3isOn = true;
          Aux3On();
          Aux2On();
          Aux1On();
          TurnOn();
        }
        if (currentLine.endsWith("GET /LALL")) {
          isOn = false;
          Aux1isOn = false;
          Aux2isOn = false;
          Aux3isOn = false;
          Aux3Off();
          Aux2Off();
          Aux1Off();
          TurnOff();
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

void Reset() {
  Serial.println("");
  Serial.print("Reseting in 3 seconds");
  delay(3000);
  resetFunc();
}

void TurnOn() {

  digitalWrite(gpioA, 0); // Turn On

}

void TurnOff() {

  digitalWrite(gpioA, 1);// Turn Off

}

void Aux1On() {
  digitalWrite(gpioB, 0);
}

void Aux1Off() {
  digitalWrite(gpioB, 1);
}

void Aux2On() {
  digitalWrite(gpioC, 0);
}
void Aux2Off() {
  digitalWrite(gpioC, 1);
}
void Aux3On() {
  digitalWrite(gpioD, 0);
}
void Aux3Off() {
  digitalWrite(gpioD, 1);
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
    Serial.println("PING FAILED. Reseting board");
    Serial.println("");

    resetFunc();


  }

  pingTestIsRunning = false;


}


