#include <ESP8266Ping.h>
#include <Servo.h>


//NodeMCU has weird pin mapping.
//Pin numbers written on the board itself do not correspond to ESP8266 GPIO pin numbers.
//We have constants defined to make using this board easier:
/*
  static const uint8_t D0   = 16; taken
  static const uint8_t D1   = 5;  taken
  static const uint8_t D2   = 4; taken
  static const uint8_t D3   = 0; taken
  static const uint8_t D4   = 2; taken
  static const uint8_t D5   = 14; taken
  static const uint8_t D6   = 12; taken
  static const uint8_t D7   = 13; taken
  static const uint8_t D8   = 15; taken
  static const uint8_t D9   = 3; RX
  static const uint8_t D10  = 1; TX

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



int ForceResetHours = 2;
int  DesiredPosLift;
int  DesiredPosBarrier1;
//int ForceResetTimeInMinutes = 2 ;
long ForceResetMill = ForceResetHours * (unsigned long int)hour;

bool pingTestIsRunning = false;
unsigned long timea;
unsigned long Val;
int countFailedPings = 0;
long millisValueAtTest = 0;

int count;

int blinks;
int blinksCount;
int lapse = 200;

int Speed = 10;
int StartPos;
int loadDown = 110;


// SERVOS' POSITIONS
// can opener lifter
int LiftUp = 160;
int LiftDown = 125;

// gate's positions
int Channel = 125;
int gateBack = 100;
int gateClosed = 0;
int gateHoldLoad = 20;
int gatePushAway = 180;

// pusher 1 positions
int pushit = 93;
int retract = 180;

// pusher 2 positions
int pushit2 = 180;
int retract2 = 50;


// barriers' positions

int barrier1Open = 40;
int barrier1Closed = 80;
int barrier2Open = 40;
int barrier2Closed = 90;

// WiFi parameters
const char* ssid = "WIFI_SSID2";
const char* password = "WIFI_PASSWORDGahsh4479#$";

static byte g_abyMyMacAddress[] = {0x02, 0x1A, 0x4B, 0x38, 0x1D, 0x8D};
IPAddress ip(192, 168, 10, 208);
byte gateway[] = { 192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network

WiFiServer server(80);



IPAddress pingAddr(192, 168, 10, 1); // ip address to ping


//Testing the DC Motors
//Define Pins
//Motor A
int const LED = D4; //  TX's LED (builtin led taken as input)
int const pwm = D3;
int const pinA1 = D5;
int const pinA2 = D6;
int const LockPin = D0; //input (GPIO16)
int const Sensor = A0; // sensor input



//speeds
int SpeedVal = 1023; //  1/4 of 12V = 3V speed 12v = 1023 = 256 + Amps' loss = +- 350
int SpeedValNow; // variable for monitoring changes in SpeedVal value
int slowmode;
int fastmode;

bool lockdone = false;
bool unlockdone = false;
bool ServoIsDone = false;
bool unloaddone = false;


Servo Lift;
Servo Barrier1;
Servo Gate;
Servo Push;
Servo Push2; // uses GPIO1 which is TX's
Servo Barrier2;

void(* resetFunc) (void) = 0;  // declare reset fuction at address 0

void setup()
{


  WiFi.mode(WIFI_STA); // turn off ai-thinker broadcast and set as station

  Serial.begin(9600);
  pinMode (LED, OUTPUT);
  pinMode (pwm, OUTPUT);
  pinMode (pinA1, OUTPUT);
  pinMode (pinA2, OUTPUT);
  pinMode (Sensor, INPUT);
  pinMode(LockPin, INPUT_PULLDOWN_16);

  digitalWrite(pinA1, LOW);
  digitalWrite(pinA2, LOW);

  Lift.attach(15);
  Barrier2.attach(13); // upper barrier
  Gate.attach(5);
  Push.attach(4);
  Push2.attach(1);// this is TX's port so needs to be disconnected upon upload
  Barrier1.attach(2); // lower barrier

  delay(100);

  Barrier1.write(barrier1Closed);
  Barrier2.write(barrier2Closed);
  Gate.write(gateHoldLoad);
  Push.write(retract);
  Push2.write(retract2);
  Lift.write(LiftUp);





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

}




void loop() {

  /*
    delay(3000);
    Barrier2.write(180);
    Barrier1.write(180);
    Serial.println("180");
    delay(5000);

    Barrier2.write(0);
    Barrier1.write(0);
    Serial.println("0");
    delay(5000);

    timea = millis();

    long ttplusmillis = (unsigned long int)ttatevent + (unsigned long int)tt;

    if (timea > (unsigned long int)ttplusmillis) {
    obstacle();
    ttatevent = millis();
    Serial.println("");
    }


       Serial.println("");
     Serial.print("millisValueAtTest: "); Serial.print(millisValueAtTest);
     Serial.println("");
     Serial.println("");
     Serial.print("TimeLimitInHoursMillVal: "); Serial.print(TimeLimitInHoursMillVal);
     Serial.println("");
     Serial.println("");
     Serial.print("time: "); Serial.print(timea);
     Serial.println("");
     Serial.print("Val: "); Serial.print(Val);
     Serial.println("");





  */

  // wait  so as not to send massive amounts of data
  delay(500);
  // do a ping test every x minutes
  Val = (unsigned long int)millisValueAtTest + (unsigned long int)TimeLimitInHoursMillVal;


  if (timea >= Val) {
    Serial.print("testing Internet connection");
    ping();
    delay(200);
    while (pingTestIsRunning) {
      yield();
    }

  }


  WiFiClient client = server.available();
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
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
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:

          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
          // and a content-type so the client knows what's coming, then a blank line:
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          // refresh the page automatically every 1 sec
          //client.println("Refresh: 5");
          //client.println();

          // the content of the HTTP response follows the header:
          client.println("<HTML>");
          client.println("<BODY style='background-color:black'>");
          client.println("<font style='color:green'>");
          client.println("<font style='font-family:electric toaster'>");
          client.println("<center>");
          client.println("<H1 style='font-size:400%;'>CAN OPENER</H1>");

          client.println("<br />");
          //client.println(WiFi.localIP());
          client.println("<br />");

          client.println("<strong>");
          client.println("<a href=\"/Unlock\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>   Unlock  </button></a>");
          if (!IsPressed()) {
            client.println("<H1 style='font-size:400%;'>LOCKED</H1>");
          }
          else {
            client.println("<H1 style='font-size:400%;'>UNLOCKED</H1>");
          }
          client.println("<br />");
          client.println("<a href=\"/Lock\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Lock   </button></a>");
          client.println("<br />");
          client.println("<a href=\"/on\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  RUN </button></a>");
          client.println("<a href=\"/off\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  STOP </button></a>");

          client.println("<br />");
          client.println("<a href=\"/fwd\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  FWD </button></a>");
          client.println("<a href=\"/bwd\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  BACKWD </button></a>");
          client.println("<br />");
          client.println("<br />");
          client.println("<a href=\"/lift\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  LIFT DEVICE UP  </button></a>");
          client.println("<a href=\"/holddown\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   HOLD DEVICE DOWN  </button></a>");
          client.println("<br />");
          client.println("<a href=\"/load\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   LOAD CAN  </button></a>");
          client.println("<a href=\"/unload\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   UNLOAD  </button></a>");

          client.println("<br />");
          client.println("<a href=\"/openGate\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   OPEN GATE </button></a>");
          client.println("<a href=\"/closeGate\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   CLOSE GATE  </button></a>");
          client.println("<br />");
          client.println("<br />");
          client.println("<a href=\"/push\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   PUSH  </button></a>");
          client.println("<br />");
          client.println("<br />");

          client.println("<a href=\"/barrier1open\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Barrier 1 Open  </button></a>");
          client.println("<a href=\"/barrier1closed\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Barrier 1 Closed  </button></a>");
          client.println("<br />");
          client.println("<a href=\"/barrier2open\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Barrier 2 Open  </button></a>");
          client.println("<a href=\"/barrier2closed\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Barrier 2 Closed </button></a>");
          client.println("<br />");

          client.println("<br />");
          client.println("<br />");

          client.println("<a href=\"/R\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   reset controller  </button></a>");


          client.println("</center>");
          client.println("<br />");
          client.println(F("<hr >"));
          client.print(F("<form method=get name=N>")); //n for fan

          client.println("<input type='range' min='0' max='1023' value='0' step='5' >");

          client.println(F("<input type=submit value=submit></form>"));

          client.println("<form method=get><br>Red:  <input type='range' min='0' max='1023' name=redVal value=redTemp oninput='On(this.value)'>");    //was onchange event

          client.println(F("<hr />"));
          client.println("</strong>");
          client.println("</BODY>");
          client.println("</HTML>");



          // Check to see if the client request was "GET /H" or "GET /L":
          if (currentLine.startsWith("GET /on")) {
            On();
          }
          if (currentLine.startsWith("GET /barrier2open")) {
            Barrier2.write(barrier2Open);
          }
          if (currentLine.startsWith("GET /barrier2closed")) {
            Barrier2.write(barrier2Closed);
          }
          if (currentLine.startsWith("GET /barrier1closed")) {
            Barrier1.write(barrier1Closed);
          }
          if (currentLine.startsWith("GET /barrier1open")) {
            Barrier1.write(barrier1Open);
          }
          if (currentLine.startsWith("GET /fwd")) {
            analogWrite(pwm, SpeedVal);
            digitalWrite (pinA1, HIGH);
            digitalWrite (pinA2, LOW);
          }
          if (currentLine.startsWith("GET /bwd")) {
            analogWrite(pwm, SpeedVal);
            digitalWrite (pinA1, LOW);
            digitalWrite (pinA2, HIGH);
          }
          if (currentLine.startsWith("GET /off")) {

            Off();
          }
          if (currentLine.startsWith("GET /Lock")) {
            Lift.write(LiftDown);
            delay(1000);
            Lock();
          }
          if (currentLine.startsWith("GET /Unlock")) {

            LiftItUp();

          }
          if (currentLine.startsWith("GET /lift")) {
            Lift.write(LiftUp);

          }
          if (currentLine.startsWith("GET /holddown")) {
            holdDown();


          }
          if (currentLine.startsWith("GET /load")) {

            LoadAndLock();

          }
          if (currentLine.startsWith("GET /unload")) {
            unload();
          }
          if (currentLine.startsWith("GET /openGate")) {

            Gate.write(180);

          }
          if (currentLine.startsWith("GET /closeGate")) {

            Gate.write(gateClosed);

          }
          if (currentLine.startsWith("GET /push")) {
            Push.write(180);

            Push2.write(retract2);

            delay(800);
            Push.write(90);
            Push2.write(pushit2);
            delay(800);
            Push.write(180);
            Push2.write(retract2);
            delay(100);



          }


          if (currentLine.startsWith("GET /R")) {

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
          client.stop();
          Serial.println("client disonnected");

          delay(1);
          //clearing string for next read
          Serial.println("String CLEARED");
          currentLine = "";

        }
      }
    }
  }
}

void unload() {

  Unlock();
  while (!unlockdone) {
    yield();
  }
  Lift.write(LiftUp); // lift can opener

  delay(500);
  int a = 0;
  while (a == 0) {
    count = 0;

    while (obstacle() && count < 50) {

      // retract pusher
      Push.write(180);
      Push2.write(retract2);
      // push with gate
      delay(200);
      Gate.write(gatePushAway);
      delay(600);
      Gate.write(gateBack); //  retract gate
      // push with pushers
      Push.write(50);
      Push2.write(pushit2);
      delay(800);

      count++;


    }

    delay(2000);
    if (!obstacle()) {
      break;
    }
  }
  Gate.write(gateBack); // leave open
  // retract pushers
  Push.write(retract);
  Push2.write(retract2);
  delay(100);


}


void LoadAndLock() {

  delay(10);
  Barrier1.write(barrier1Closed); // close first barrier to hold the can
  delay(1000);
  Barrier2.write(barrier2Open); // open second barrier to pre-load a can
  delay(3000);
  Barrier2.write(barrier2Closed); // close second barrier
  Push2.write(retract2);

  Unlock();

  while (!unlockdone) {
    yield();
  }
  Barrier1.write(loadDown); // initialise from proper position
  delay(1500);
  Gate.write(gateHoldLoad); // close gate to prevent can from slipping away
  Push.write(retract); // retract pusher
  Push2.write(retract2);
  Lift.write(LiftUp); // lift can opener
  delay(1000);

  Barrier1.write(barrier1Open); // load the can into can opener
  delay(7000);

  Barrier1.write(barrier1Closed); // close loader

  count = 0;
  while (!obstacle() && count < 10) {

    delay(500);
    Push2.write(pushit2);// push
    Push.write(180);
    delay(1500);
    Push2.write(retract2); // retract pushers
    Push.write(120);
    delay(500);
    count++;
  }
  delay(1000);


  delay(500);
  Gate.write(Channel); // use back of the gate as channel
  delay(200); //
  Gate.write(gateHoldLoad); // close gate

  Push.write(retract); // retract pusher
  delay(500); //
  Push.write(pushit); // push again the can to adjust
  Push2.write(pushit2);
  delay(500); //
  Push.write(180); // retract pusher
  Push2.write(retract2);
  delay(500); //
  Push.write(110); // push again the can to adjust
  Push2.write(pushit2);
  delay(500); //

  Push.write(180);// retract pusher
  count = 0;
  while (count < 5) {

    delay(600); //
    Gate.write(Channel); // use gate's tail to push in place
    delay(600); //
    Gate.write(gateBack); //

    count++;
  }

  Gate.write(Channel); // gate to channel position
  delay(500); //
  Push.write(180); // retract pusher
  Push2.write(pushit2); // but keep pusher 2 in position

  Gate.write(Channel); // gate to channel position--
  delay(200);
  Push.write(pushit); // push again the can to adjust
  Push2.write(160); // same

  Gate.write(gateHoldLoad); // close gate
  delay(1000);

  count = 0;
  // tap down

  delay(600); //
  Lift.write(80); // lower can opener to bring can flat down
  delay(1000);
  Gate.write(Channel); // gate to channel position
  Lift.write(LiftUp); // lift back up
  delay(1000);
  Gate.write(gateBack); // gate back
  count++;

  Gate.write(gateHoldLoad); // close gate
  delay(500);
  Lift.write(LiftDown); // nown bring down and lock
  Push.write(retract); //
  delay(800);
  Lock();
  while (!lockdone) {
    yield();
  }

  delay(100);

}

void RunServo() {
  ServoIsDone = false;
  delay(20);
  StartPos = Lift.read();
  int pos = StartPos;

  while (Lift.read() < DesiredPosLift) {
    pos++;
    Lift.write(pos);
    delay(Speed);
    Serial.println("LiftLoop1");
  }
  while (Lift.read() > DesiredPosLift) {
    pos--;
    Lift.write(pos);
    delay(Speed);
    Serial.println("LiftLoop2");
  }
  ServoIsDone = true;
}

void RunLoad() {
  ServoIsDone = false;
  delay(20);
  StartPos = Barrier1.read();
  int pos = StartPos;

  while (Barrier1.read() < DesiredPosBarrier1) {
    pos++;
    Barrier1.write(pos);
    delay(Speed);
    //Serial.println("Barrier1Loop1");
  }
  while (Barrier1.read() > DesiredPosBarrier1) {
    pos--;
    Barrier1.write(pos);
    delay(Speed);
    //Serial.println("Barrier1Loop2");
  }
  ServoIsDone = true;
}

void holdDown() {

  delay(10);
  Unlock();
  Push.write(pushit);
  Gate.write(gateClosed);
  Push2.write(pushit2);
  delay(50);
  int pos = Lift.read();
  int desiredPos = pos - 30;

  while (Lift.read() > desiredPos) {
    pos--;
    Lift.write(pos);
    delay(50);
  }

  delay(50);
  Push.write(retract);
  Lock();
  delay(100);

}
void LiftItUp() {
  Stop();
  Unlock();

  Lift.write(LiftUp);
}

void Stop() {
  Off();
}

void On()
{

  Push2.write(pushit2);
  Push.write(retract);
  Gate.write(gateClosed);
  delay(500); // time to have the can properly centered
  holdDown(); // bring can opener onto the can
  Lock(); // lock it
  
  // run the motor
  analogWrite(pwm, 511); // too fast induces gripp loss
  digitalWrite (pinA1, HIGH);
  digitalWrite (pinA2, LOW);
 
}

void Off()
{

  digitalWrite (pinA1, LOW);
  digitalWrite (pinA2, LOW);

}

boolean obstacle() {
  bool result;

  if (analogRead(A0) < 500) {
    result = true;
  }
  else {
    result = false;
  }
  Serial.println("");
  Serial.print("obstacle: "); Serial.print(result);
  /*
    Serial.println("");
    Serial.print("Sensor Read: "); Serial.print(analogRead(A0));
  */
  return result;
}

boolean IsPressed()
{

  bool pressed = false;
  if (digitalRead(LockPin) == HIGH) {
    pressed = true;

  }
  else {
    pressed = false;
  }
  return pressed;

  delay(5);
}

void Unlock()
{
  Stop();
  unlockdone = false;
  analogWrite(pwm, SpeedVal);
  count = 0;
  digitalWrite (pinA1, LOW);
  digitalWrite (pinA2, HIGH);
  while (!IsPressed() && count < 10) {
    count++;
    delay(100);
  }
  digitalWrite (pinA1, LOW);
  digitalWrite (pinA2, LOW);
  unlockdone = true;
}

void Lock()
{
  lockdone = false;
  //Off(); // reset the ports status
  delay(100);
  analogWrite(pwm, SpeedVal);
  count = 0;
  digitalWrite (pinA1, HIGH);
  digitalWrite (pinA2, LOW);
  while (IsPressed() && count < 10) {
    count++;
    delay(1);
  }
  delay(1500);
  digitalWrite (pinA1, LOW);
  digitalWrite (pinA2, LOW);
  lockdone = true;
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
