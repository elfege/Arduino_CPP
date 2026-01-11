#include <ESP8266Ping.h>


#include <ESP8266WiFi.h>


int gpioA = 0;
int gpioB = 2;
int gpioC = 4;
int gpioD = 5;
int gpioE = 12;
int gpioF = 13;
int gpioG = 14;
int gpioH = 15;
int gpioI = 16;

//int TX = 1;
//int RX = 3;


// WiFi parameters
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORDGahsh4479#$";

WiFiServer server(80);


void setup(void)
{
  // Start Serial
  Serial.begin(9600);
  delay(10);
  pinMode(gpioA, OUTPUT);// Initialize gpioA as an output
  pinMode(gpioB, OUTPUT);
  pinMode(gpioC, OUTPUT);


  digitalWrite(gpioA, 0);
  digitalWrite(gpioB, 0);

  //digitalWrite(TX, 0);


  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(10);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Print the IP address
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Server started");
  ping();

}

void loop() {
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
            client.println("<H1 style='font-size:400%;'>8CORE COMPUTER SWITCH </H1>");



            if (isOn()) {
              client.println("<br />");
              client.println("<H3 style='font-size:100%;'>8 CORE IS ON</H3>");
            }
            else {
              client.println("<br />");
              client.println("<H3 style='font-size:100%;'>8 CORE IS OFF</H3>");
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

          TurnOn();

        }
        if (currentLine.endsWith("GET /L")) {
          TurnOff();
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

bool isOn(){
  
}

void TurnOn() {
  ping();
  if (!isOn) {
    digitalWrite(gpioA, 1); // Turn Off
    digitalWrite(gpioB, 1);

    //digitalWrite(TX, 1);
    Serial.println("");

    delay(500);
    digitalWrite(gpioA, 0); // Turn On
    digitalWrite(gpioB, 0);

    //digitalWrite(TX, 0);
    Serial.println("");
    Serial.println("ON COMMAND sent");
  }
  else {
    Serial.println("Already ON");
  }

}

void TurnOff() {
  ping();

  int count = 0;

  while (isOn && count < 5) {

    count++;
    digitalWrite(gpioA, 1); // Turn Off
    digitalWrite(gpioB, 1);

    //digitalWrite(TX, 1);
    Serial.println("");

    delay(500);
    digitalWrite(gpioA, 0); // Turn On
    digitalWrite(gpioB, 0);

    //digitalWrite(TX, 0);
    Serial.println("");
    Serial.println("OFF COMMAND SENT ------------------------wainting 30seconds");
    delay(30000);
    ping();
  }

}

void ping() {

  IPAddress ip (192, 168, 10, 110); // The remote ip to ping

  bool ret = Ping.ping(ip);

  int avg_time_ms = Ping.averageTime();

  if (ret) {
    
    Serial.println("PING SUCCESS! Average time is: ");

    Serial.print(avg_time_ms);
    Serial.println("");
  }
  else {
    
    Serial.println("PING FAILED");
    Serial.println("");
  }
}
