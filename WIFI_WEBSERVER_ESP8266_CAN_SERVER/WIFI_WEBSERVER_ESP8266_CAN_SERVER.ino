#include <ESP8266Ping.h>
#include <Servo.h>


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



long seconds = 1000;
long minutes = (unsigned long int)seconds * 60;
long hour = (unsigned long int)3600 * (unsigned long int)seconds;
int desiredTimeInHours = 1; // time in hours
int desiredTimeInMinutes = 120; // time in minutes
long TimeLimitInHoursMillVal = (unsigned long int)desiredTimeInHours * (unsigned long int)hour;
long TimeLimitInMinutesMillVal = desiredTimeInMinutes * (unsigned long int)minutes;
int elapsed;// variable redefined below with time()

long tt = 1000;
long ttatevent;

int showValue1;
int posClamp = 150;

int ForceResetHours = 2;
int  DesiredPosDegrees;

//int ForceResetTimeInMinutes = 2 ;
long ForceResetMill = ForceResetHours * (unsigned long int)hour;


bool pingTestIsRunning = false;
bool ReverseDone = false;
bool ForwardDone = false;
bool ServoIsDone = false;
bool CommandFromBringPlate = false;
bool GrabItDone = false;
bool OnreverseDone = false;

unsigned long timea;
unsigned long Val;
int countFailedPings = 0;
long millisValueAtTest = 0;

int count;

int blinks;
int blinksCount;
int lapse = 200;

int DesiredPos;
int Speed = 10;
int StartPos;
int WristDefault = 0;
int WristGrab = WristDefault;
int WristPour = 145;
int ArmCenter = 150;
int ArmDown = 125;
int ArmUp = 170;
int ClampOpen = 0;
int ClampMidOpen = 40;
int ClampClosed = 55;
int ClampFullClose = 70;
int TowerToCan = 0;
int TowerToPlate = 180;
int TowerToServe = 70;
int HoldPlateDown = 150;

int ArmToCan = 170;


int degrees;

// WiFi parameters
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORDGahsh4479#$";

static byte g_abyMyMacAddress[] = {0x02, 0x1A, 0x4B, 0x38, 0x1D, 0x8D};
IPAddress ip(192, 168, 10, 210);
byte gateway[] = { 192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network

WiFiServer server(80);



IPAddress pingAddr(192, 168, 10, 1); // ip address to ping


//Testing the DC Motors
//Define Pins
//Motor A
int const LED = D4; //  TX's LED (builtin led taken as input)

int const pwm = D6;
int const pinA1 = D7;
int const pinA2 = D8;
int const CanDetect = D0; //input (GPIO16)
int const DistanceSensor = A0; // DistanceSensor input

int pulse = 1500;


String currentLine = "";                // make a String to hold incoming data from the client

//speeds
int SpeedVal = 1023; //  1/4 of 12V = 3V speed 12v = 1023 = 256 + Amps' loss = +- 350
int SpeedValNow; // variable for monitoring changes in SpeedVal value
int slowmode;
int fastmode;


Servo Wrist;
Servo Tower;

Servo Clamp;
Servo Arm;

void(* resetFunc) (void) = 0;  // declare reset fuction at address 0

void setup()
{


  WiFi.mode(WIFI_STA); // turn off ai-thinker broadcast and set as station

  Serial.begin(9600);
  //pinMode (LED, OUTPUT);
  pinMode (pwm, OUTPUT);
  pinMode (pinA1, OUTPUT);
  pinMode (pinA2, OUTPUT);
  pinMode (DistanceSensor, INPUT);
  pinMode(CanDetect, INPUT_PULLDOWN_16);

  // non standard servo pin


  /*
    // forward
    Serial.println("FORWARD 2 SECONDS");
    digitalWrite (pinA1, LOW);
    digitalWrite (pinA2, HIGH);
    delay(2000);
    digitalWrite (pinA1, LOW);
    digitalWrite (pinA2, LOW);
  */

  Wrist.attach(14);
  Clamp.attach(0);
  Tower.attach(5);
  Arm.attach(4);


  delay(10);

  Wrist.write(WristDefault);

  Clamp.write(ClampMidOpen);

  Arm.write(ArmCenter);
  Tower.write(70);
  Forward();


  WiFi.begin(ssid, password);
  lapse = 50;
  while (WiFi.status() != WL_CONNECTED) {
    yield();

    count++;
    Serial.println(count);
    if (count >= 500) {
      Serial.println("RESETING");
      delay(500);
      //resetFunc();

    }

  }


  server.begin();
  Serial.println(WiFi.localIP());

  Serial.println(seconds);
  Serial.println(minutes);
  Serial.println(hour);
  Serial.println(desiredTimeInHours);
  Serial.println(TimeLimitInHoursMillVal);
  Serial.println(TimeLimitInMinutesMillVal);

  // led signals successful boot

  lapse = 200;
  blinks = 4;
  Blink();


  Serial.println("");
  Serial.print("SETUP DONE");

}




void loop() {
  //obstacle();
  //candetected();


  timea = millis();

  // TO DO: // set wrist to a position which keeps it straight with each Arm's new position



  // do a ping test every x minutes
  Val = (unsigned long int)millisValueAtTest + (unsigned long int)TimeLimitInHoursMillVal;

  if (timea >= Val) {
    Serial.print("testing Internet connection");
    ping();
    delay(200);
    while (pingTestIsRunning) {
      ;
    }
  }


  WiFiClient client = server.available();
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port

    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor

        //read char by char HTTP request
        if (currentLine.length() < 100) {

          //store characters to string
          currentLine += c;

        }
        Serial.println("");
        Serial.print("currentLine: "); Serial.print(currentLine);

        //if HTTP request has ended
        if (c == '\n') {

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
          client.println("<HEAD>");
          client.println("<center>");
          client.println("<H4 style='font-size:400%;'>CAN SERVER</H4>");
          client.println("<hr />");
          client.println("</center>");
          client.println("</HEAD>");

          client.println("<br />");
          //client.println(WiFi.localIP());
          client.println("<br />");

          client.println("<strong>");
          client.print("<a href=\"/forward\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>   Forward  </button></a>");
          client.print("<a href=\"/reverse\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Reverse   </button></a>");
          client.println("<br />");
          client.println("<br />");
          client.print("<a href=\"/openclamp\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>   Open Clamp </button></a>");
          client.print("<a href=\"/closeclamp\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Close Clamp  </button></a>");
          client.print("<a href=\"/clampfullclose\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Fully Close Clamp  </button></a>");
          client.println("<br />");
          client.println("<br />");
          client.print("<a href=\"/wristgrab\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>   Wrist Grab Position </button></a>");
          client.print("<a href=\"/wristpour\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Wrist Pour Position  </button></a>");
          client.println("<br />");
          client.println("<br />");
          client.println("<center>");
          client.println("<hr />");
          client.print("<a href=\"/gogetcan\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'> Serve Food </button></a>");
          client.println("<hr />");
          client.println("<hr />");
          client.println("<center />");
          client.println("<center>");
          client.println("<hr />");
          client.print("<a href=\"/armup\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Arm Up </button></a>");
          client.print("<a href=\"/armcenter\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Arm Center </button></a>");
          client.print("<a href=\"/armdown\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Arm Down </button></a>");
          client.println("<br />");
          client.println("<br />");
          client.print("<a href=\"/towerleft\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Tower Left </button></a>");
          client.print("<a href=\"/towercenter\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Tower Center </button></a>");
          client.print("<a href=\"/towerright\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Tower Right </button></a>");
          client.println("<br />");
          client.print("<a href=\"/wristminus\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   wrist - 5 </button></a>");
          client.print("<a href=\"/wristplus\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   wrist + 5 </button></a>");

          client.println("<hr />");
          client.println("<center />");
          client.println("<br />");
          client.println("<br />");
          client.print("<a href=\"/R\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   reset controller  </button></a>");


          //client.println("</center>");

          client.println("</strong>");
          client.println("</BODY>");
          client.println("</HTML>");

          delay(1);
          client.stop();
          Serial.println("client disonnected");

          // Check request:
          Serial.println("READING COMMAND: "); Serial.print(currentLine);

          if (currentLine.startsWith("GET /forward")) {
            Serial.println("forward COMMAND SENT");
            Forward();
          }
          else if (currentLine.startsWith("GET /towerleft")) {
            Serial.println("towerleft COMMAND SENT");

            Tower.write(180);
          }
          else if (currentLine.startsWith("GET /towerright")) {
            Serial.println("towerright COMMAND SENT");

            Tower.write(0);
          }
          else if (currentLine.startsWith("GET /towercenter")) {
            Serial.println("towerright COMMAND SENT");

            Tower.write(90);
          }
          else if (currentLine.startsWith("GET /gogetcan")) {
            GoGetCan();
          }
          else if (currentLine.startsWith("GET /armup")) {
            Arm.write(ArmUp);
          }
          else if (currentLine.startsWith("GET /armcenter")) {
            Arm.write(ArmCenter);
          }
          else if (currentLine.startsWith("GET /armdown")) {
            Arm.write(ArmDown);
          }
          else if (currentLine.startsWith("GET /reverse")) {
            Reverse();
          }
          else if (currentLine.startsWith("GET /openclamp")) {
            Clamp.write(ClampOpen);
          }
          else if (currentLine.startsWith("GET /closeclamp")) {
            Clamp.write(ClampClosed);
          }
          else if (currentLine.startsWith("GET /clampfullclose")) {
            Clamp.write(ClampFullClose);
          }
          else if (currentLine.startsWith("GET /gotoplate")) {
            GotoPlate();
          }
          else if (currentLine.startsWith("GET /grabthecan")) {
            CloseClamp();
          }
          else if (currentLine.startsWith("GET /wristgrab")) {
            Wrist.write(WristGrab);
          }
          else if (currentLine.startsWith("GET /wristpour")) {
            Wrist.write(WristPour);
          }
          else if (currentLine.startsWith("GET /bringplate")) {
            //BringPlate();
          }
          else if (currentLine.startsWith("GET /wristminus")) {
            int current = Wrist.read();
            int newpos = current - 5;
            Wrist.write(newpos);
          }
          else if (currentLine.startsWith("GET /wristplus")) {
            int current = Wrist.read();
            int newpos = current + 5;
            Wrist.write(newpos);
          }


          else if (currentLine.startsWith("GET /R")) {

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
            String UrlFinal = url1 + WiFi.localIP() + url2 ;
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


          delay(1);
          //clearing string for next read
          Serial.println("String CLEARED");
          currentLine = "";

        }

      }
    }
  }
}

void GotoPlate()
{
  Reverse();
  while (!ReverseDone) {
    yield();
  }

  Arm.write(ArmCenter);

  delay(500);
  Tower.write(TowerToPlate);
}

void GrabIt() {
  GrabItDone = false;
  int a = 0;

  // reverse for 10 secs
  analogWrite(pwm, SpeedVal);
  digitalWrite (pinA1, HIGH);
  digitalWrite (pinA2, LOW);
  delay(10000);
  Stop();

  while (!candetected()) {
    // start position
    int TowerPos = 90;

    int DesiredArmPos = ArmDown;
    int ArmPosStart = ArmUp;

    Arm.write(ArmPosStart);
    Tower.write(TowerPos);
    Clamp.write(ClampOpen);
    delay(400);

    // move the arm slowly toward can until detection

    int ArmNewPos = ArmPosStart;
    int WristOldPos = Wrist.read();
    int WristNewPos;

    while (Tower.read() > 20 ) {
      Clamp.write(ClampOpen);
      TowerPos--;
      Serial.println("");
      Serial.print("TowerPos = "); Serial.print(TowerPos);
      Tower.write(TowerPos);
      delay(100);

      // slowly lower the arm
      if (Arm.read() > DesiredArmPos) {
        ArmNewPos -= 1;
        Wrist.write(WristGrab); // make sure wrist is ok

        Arm.write(ArmNewPos);

        Serial.println("");
        Serial.print("ArmNewPos = "); Serial.print(ArmNewPos);

      }
      delay(50);
      if (Tower.read() <= 30) {
        // first grab and bring backward
        Clamp.write(ClampClosed);
        delay(1000);
        break;
      }
    }

    if (candetected()) {
      GrabItDone = true;
      break;
    }
  }
  // forward back for 10 secs
  analogWrite(pwm, SpeedVal);
  digitalWrite (pinA1, LOW);
  digitalWrite (pinA2, HIGH);
  delay(10000);
  Stop();
  GrabItDone = true;
}


void GoGetCan() {

  int ArmPos;
  // clamp closed for less mess
  Clamp.write(ClampClosed);
  // get the rail into proper position
  Arm.write(180);
  Tower.write(90);

  Forward();
  while (obstacle()) {
    yield();
  }
  delay(3000);

  Clamp.write(ClampOpen);
  Wrist.write(WristGrab);

  OnreverseDone = false;
  GrabIt();
  //  try to get the can into the midst of the clamp
  while (!GrabItDone) {
    yield();
  }
  ///// END OF GRAB /////


  Wrist.write(WristGrab); //
  // go to plate slowly
  delay(1500);
  int TowerPos = Tower.read();
  Wrist.write(20); // keep wrist straight
  delay(400);
  Arm.write(180);
  // reverse and get Arm into position
  analogWrite(pwm, SpeedVal);
  digitalWrite (pinA1, HIGH);
  digitalWrite (pinA2, LOW);

// slowly bring tower into position to avoid spilling
  while (TowerPos < 180) {
    TowerPos++;
    Tower.write(TowerPos);
    delay(100);
  }

  delay(3000);
  Stop();

  // pour it
  Wrist.write(WristPour); //
  delay(1500);
  // pour again
  Wrist.write(WristGrab); // upside
  delay(1500);
  Wrist.write(WristPour); // downside
  delay(1500);

  // shake it out
  count = 0;
  while(count < 5){
  Arm.write(ArmUp);
  delay(2000);
  Arm.write(160);
  delay(2000);
  count++;
  }
 
 Arm.write(ArmCenter);
  delay(5000); // wait long enough for most of the food to fall off
  Arm.write(ArmUp);
  delay(2000);
  Wrist.write(WristGrab); // upside

  // prepare to ditch
  Arm.write(ArmUp);
  Tower.write(180);
  delay(500);

  // reverse fully
  analogWrite(pwm, SpeedVal);
  digitalWrite (pinA1, HIGH);
  digitalWrite (pinA2, LOW);
  while (!obstacle()) {
    yield();
  }
  Stop();

  // release can
  delay(1500);
  Clamp.write(ClampMidOpen);
  delay(1500);
  Clamp.write(ClampClosed);

  // go back to initial position
  Clamp.write(ClampClosed);
  Forward();

  Arm.write(160);
  Clamp.write(ClampFullClose); // fully close clamp to avoid track jamming
  delay(45000);
  // slowly bring the tower back to avoid punching a cat...
  TowerPos = Tower.read();
  while (TowerPos > 70) {
    TowerPos--;
    Tower.write(TowerPos);
    delay(10);
  }
  Arm.write(150);
}


//// SERVOS MANAGEMENT

int degree2ms(int len)
{
  Serial.println("");
  Serial.print("DesiredPosDegrees converted");
  Serial.println("");

  return 500 + len * 200 / 18;
  //int microValue = map(DesiredPosDegrees, 0,270,500,2500);


}


void RunServoTower() {
  ServoIsDone = false;
  delay(20);
  StartPos = Tower.read();
  int pos = StartPos;

  while (Tower.read() < DesiredPos) {
    pos++;
    Tower.write(pos);
    delay(Speed);
    //Serial.println("LiftLoop1");
  }
  while (Tower.read() > DesiredPos) {
    pos--;
    Tower.write(pos);
    delay(Speed);
    //Serial.println("LiftLoop2");
  }
  ServoIsDone = true;
}



void OpenClamp()
{

  Clamp.write(ClampOpen);


}


void CloseClamp()
{



}

void Reverse()
{
  ReverseDone = false;
  //Arm.writeMicroseconds(ArmCenter);
  //Tower.write(90);
  analogWrite(pwm, SpeedVal);
  digitalWrite (pinA1, HIGH);
  digitalWrite (pinA2, LOW);
  while (!obstacle() && !candetected()) {
    yield();
  }
  Stop();
  ReverseDone = true;
}

void Forward()
{

  ForwardDone = false;
  //Arm.writeMicroseconds(ArmCenter);
  //Tower.write(90);

  analogWrite(pwm, SpeedVal);

  digitalWrite (pinA1, LOW);
  digitalWrite (pinA2, HIGH);

  ForwardDone = true;


}

void Stop()
{

  digitalWrite (pinA1, LOW);
  digitalWrite (pinA2, LOW);

}

boolean candetected() {
  bool result;

  if (digitalRead(CanDetect) == HIGH) {
    result = true;

    Serial.println("CAN DETECTED!");

    // Clamp.write(ClampClosed);
  }
  else {
    result = false;

    //Serial.print("NO CAN DETECTED!");

    //Clamp.write(ClampOpen);
  }
  return result;
  delay(100);
}

boolean obstacle() {
  bool result;

  if (analogRead(DistanceSensor) < 100) {
    result = true;
    Serial.print("OBSTACLE");
    Stop(); // in any case stop it
  }
  else {
    result = false;
    Serial.print("NO OBSTACLE");
  }
  Serial.println("");

  /*
    Serial.println("");
    Serial.print("DistanceSensor Read: "); Serial.print(analogRead(DistanceSensor));
  */
  return result;
}


void Reset() {

  Serial.println("client disonnected");
  Serial.println("");
  Serial.print("Reseting in 3 seconds");
  delay(3000);
  resetFunc();
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

void RunServoArm_MS() {

  ServoIsDone = false;
  delay(20);
  int  ArmRead = Arm.read();
  int ArmPos = degree2ms(ArmRead);
  Serial.println("");
  Serial.print(" BEFORE CONVERSION = "); Serial.print(DesiredPosDegrees);
  DesiredPos = degree2ms(DesiredPosDegrees); // convert DesiredPosDegrees from degrees to microseconds (0-270 / 500-2500)


  Serial.println("");
  Serial.print(" AFTER CONVERSION = "); Serial.print(DesiredPos);
  delay(10);



  while (ArmPos > DesiredPos) {


    Serial.println("");
    Serial.print("ArmRead= "); Serial.print(Arm.read());
    Serial.println("");
    Serial.print("ArmPos = "); Serial.print(ArmPos);

    Arm.writeMicroseconds(ArmPos);
    //delayMicroseconds(2500);

    delay(200);
    ArmPos -= 100;
  }
  delay(1000);

  while (ArmPos < DesiredPos) {

    Serial.println("");
    Serial.print("ArmRead= "); Serial.print(Arm.read());
    Serial.println("");
    Serial.print("ArmPos = "); Serial.print(ArmPos);

    Arm.writeMicroseconds(ArmPos);
    //delayMicroseconds(2500);

    delay(200);
    ArmPos += 100;
  }

  ServoIsDone = true;
}


