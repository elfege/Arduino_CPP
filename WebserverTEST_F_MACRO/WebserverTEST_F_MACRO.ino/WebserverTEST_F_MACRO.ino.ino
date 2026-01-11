#include <ICMPPing.h>
#include <util.h>

#include <Servo.h>
#include <SPI.h>
#include <Ethernet.h>



byte mac[] = {0x02, 0x1A, 0x4A, 0x3E, 0xDC, 0x8E}; // fore the firewall: 02:1A:4A:3E:DC:8E
IPAddress ip(192, 168, 10, 208);
byte gateway[] = { 192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network
IPAddress pingAddr(192, 168, 10, 1); // ip address to ping
EthernetServer server(80);

/*
  SOCKET pingSocket = 0;
  char buffer [256];
  ICMPPing ping(pingSocket, (uint16_t)random(0, 255));


  /// timer values
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
  // timer related ping values
  bool pingTestIsRunning = false;
  long millisValueAtTest = 0;
*/

String currentLine = "";   // make a String to hold incoming data from the client

int  DesiredPosDegrees; // outdated variable for unconventional servos that may be in use at some point in this project (using servo function defined below)

int const LED = 13; // ATMega built in led

int const pwmOpener;
int const MotorOpener1;
int const MotorOpener2;
int const LockPin; //input (GPIO16)
int const CanPresenceSensor;  // Distance Sensor sensor Analog input detects can's presence

int const pwmArm ;
int const MotorArm1;
int const MotorArm2;
int const CanGrabbDetect; //input (GPIO16)
int const ArmSensor; // Distance Sensor Analog input detects Arm's proximity


bool ReverseDone = false;
bool ForwardDone = false;
bool ServoIsDone = false;
bool CommandFromBringPlate = false;
bool GrabItDone = false;
bool OnreverseDone = false;
bool lockdone = false;
bool unlockdone = false;
bool unloaddone = false;


int count;

int blinks;
int blinksCount;
int lapse = 200;



int ArmToCan = 170;


Servo Lift;
Servo Barrier1;
Servo Gate;
Servo Push;
Servo Push2; //
Servo Barrier2;

Servo Wrist;
Servo Tower;
Servo Clamp;
Servo Arm;

void(* resetFunc) (void) = 0;  // declare reset fuction at address 0


//speeds
int SpeedVal = 1023; //  1/4 of 12V = 3V speed 12v = 1023 = 256 + Amps' loss = +- 350
int SpeedValNow; // variable for monitoring changes in SpeedVal value
int slowmode;
int fastmode;


//////////// SERVOS' POSITIONS for the can opener///////////////////////

int  DesiredPosLift;
int  DesiredPosBarrier1;
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


//////////// SERVOS' POSITIONS FOR THE CAN SERVER///////////////////////

int Speed = 10;
int StartPos;
int loadDown = 110;
int DesiredPos;
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


void setup() {
  Serial.begin(9600);
  pinMode (LED, OUTPUT);
  pinMode (pwmArm, OUTPUT);
  pinMode (MotorArm1, OUTPUT);
  pinMode (MotorArm2, OUTPUT);
  pinMode (ArmSensor, INPUT);
  pinMode(LockPin, INPUT);  // note to myself: used to be INPUT_PULLDOWN_16 on ESP8266 don't forget to reverse values in the coresponding boolean function
  pinMode (pwmOpener, OUTPUT);
  pinMode (MotorOpener1, OUTPUT);
  pinMode (MotorOpener2, OUTPUT);
  pinMode (CanPresenceSensor, INPUT);
  pinMode(CanGrabbDetect, INPUT);// note to myself: used to be INPUT_PULLDOWN_16 on ESP8266 don't forget to reverse values in the coresponding boolean function

  // make sure motors are off
  digitalWrite(MotorOpener1, LOW);
  digitalWrite(MotorOpener1, LOW);
  digitalWrite(MotorArm1, LOW);
  digitalWrite(MotorArm2, LOW);

  Lift.attach(15);
  Barrier2.attach(13); // upper barrier
  Gate.attach(5);
  Push.attach(4);
  Push2.attach(1);// this is TX's port so needs to be disconnected upon upload
  Barrier1.attach(2); // lower barrier

  Wrist.attach(14);
  Clamp.attach(0);
  Tower.attach(5);
  Arm.attach(4);

  delay(50);


  Barrier1.write(barrier1Closed);
  Barrier2.write(barrier2Closed);
  Gate.write(gateHoldLoad);
  Push.write(retract);
  Push2.write(retract2);
  Lift.write(LiftUp);

  Wrist.write(WristDefault);
  Clamp.write(ClampMidOpen);
  Arm.write(ArmCenter);
  Tower.write(70);
  Forward();

  Serial.println("Initializing Ethernet Protocol");
  Ethernet.begin(mac, ip);
  Serial.println("Starting Ethernet Server");
  server.begin();

  Serial.println(Ethernet.localIP());


}

void loop() {
  //CanPresence();
  //candetected();

  EthernetClient client = server.available();

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

        //if HTTP request has ended
        if (c == '\n') {
          
          Serial.println("");
          Serial.print("currentLine: "); Serial.print(currentLine);//see what was captured
/*
          if (!IsPressed()) {
            client.println("<H3 style='font-size:400%;'>CAN OPENER IS LOCKED</H3>");
          }
          else {
            client.println("<H3 style='font-size:400%;'>CAN OPENER IS UNLOCKED</H3>");
          }
          */
          client.print(F(

                         "HTTP/1.1 200 OKOK\r\nContent-Type: text/html\r\n\r\n"
                         
                         // refresh the page automatically every 1 sec
                         "Refresh: 1"
                         "<br />"

                         // the content of the HTTP response follows the header:
                         "<HTML>"
                         "<BODY style='background-color:black'>"
                         "<font style='color:green'>"
                         "<font style='font-family:electric toaster'>"

                         "<br />"

                         "<strong>"

                         "<hr >"
                         "<H1 style='font-size:400%;'>CAN OPENER</H1>"
                         "<hr />"
                         "<a href=\"/Unlock\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>   Unlock  </button></a>"

                         "<br />"
                         "<a href=\"/Lock\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Lock   </button></a>"
                         "<br />"
                         "<a href=\"/on\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  RUN </button></a>"
                         "<a href=\"/off\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  STOP </button></a>"

                         "<br />"
                         "<a href=\"/fwd\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  FWD </button></a>"
                         "<a href=\"/bwd\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  BACKWD </button></a>"
                         "<br />"
                         "<br />"
                         "<a href=\"/lift\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  LIFT DEVICE UP  </button></a>"
                         "<a href=\"/holddown\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   HOLD DEVICE DOWN  </button></a>"
                         "<br />"
                         "<a href=\"/load\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   LOAD CAN  </button></a>"
                         "<a href=\"/unload\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   UNLOAD  </button></a>"

                         "<br />"
                         "<a href=\"/openGate\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   OPEN GATE </button></a>"
                         "<a href=\"/closeGate\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   CLOSE GATE  </button></a>"
                         "<br />"
                         "<br />"
                         "<a href=\"/push\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   PUSH  </button></a>"
                         "<br />"
                         "<br />"

                         "<a href=\"/barrier1open\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Barrier 1 Open  </button></a>"
                         "<a href=\"/barrier1closed\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Barrier 1 Closed  </button></a>"
                         "<br />"
                         "<a href=\"/barrier2open\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Barrier 2 Open  </button></a>"
                         "<a href=\"/barrier2closed\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Barrier 2 Closed </button></a>"
                         "<br />"
                         "<hr>"

                         "<hr />"
                         "<hr >"
                         "<H1 style='font-size:400%;'>CAN SERVER</H1>"
                         "<hr />"

                         "<strong>"
                         "<a href=\"/forward\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>   Forward  </button></a>"
                         "<a href=\"/reverse\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Reverse   </button></a>"
                         "<br />"
                         "<br />"
                         "<a href=\"/openclamp\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>   Open Clamp </button></a>"
                         "<a href=\"/closeclamp\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Close Clamp  </button></a>"
                         "<a href=\"/clampfullclose\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Fully Close Clamp  </button></a>"
                         "<br />"
                         "<br />"
                         "<a href=\"/wristgrab\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>   Wrist Grab Position </button></a>"
                         "<a href=\"/wristpour\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Wrist Pour Position  </button></a>"
                         "<br />"
                         "<br />"
                         "<center>"
                         "<hr />"
                         "<a href=\"/gogetcan\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'> Serve Food </button></a>"
                         "<hr />"
                         "<hr />"
                         "<center />"
                         "<center>"
                         "<hr />"
                         "<a href=\"/armup\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Arm Up </button></a>"
                         "<a href=\"/armcenter\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Arm Center </button></a>"
                         "<a href=\"/armdown\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Arm Down </button></a>"
                         "<br />"
                         "<br />"
                         "<a href=\"/towerleft\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Tower Left </button></a>"
                         "<a href=\"/towercenter\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Tower Center </button></a>"
                         "<a href=\"/towerright\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   Tower Right </button></a>"
                         "<br />"
                         "<a href=\"/wristminus\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   wrist - 5 </button></a>"
                         "<a href=\"/wristplus\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   wrist + 5 </button></a>"

                         "<hr />"
                         "<center />"
                         "<br />"
                         "<br />"
                         "<a href=\"/R\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   reset controller  </button></a>"


                         //"</center>"

                         "</strong>"
                         "</BODY>"
                         "</HTML>"

                       )); // end of F macro

          delay(1);
          client.stop();
          Serial.println("client disonnected");

          // Check request:
          Serial.println("READING COMMAND: "); Serial.print(currentLine);
          if (currentLine.startsWith("GET /on")) {
            On();
          }
          else if (currentLine.startsWith("GET /off")) {

            Off();
          }
          else if (currentLine.startsWith("GET /barrier2open")) {
            Barrier2.write(barrier2Open);
          }
          else if (currentLine.startsWith("GET /barrier2closed")) {
            Barrier2.write(barrier2Closed);
          }
          else if (currentLine.startsWith("GET /barrier1closed")) {
            Barrier1.write(barrier1Closed);
          }
          else if (currentLine.startsWith("GET /barrier1open")) {
            Barrier1.write(barrier1Open);
          }
          else if (currentLine.startsWith("GET /fwd")) {
            analogWrite(pwmOpener, SpeedVal);
            digitalWrite (MotorOpener1, HIGH);
            digitalWrite (MotorOpener2, LOW);
          }
          else if (currentLine.startsWith("GET /bwd")) {
            analogWrite(pwmOpener, SpeedVal);
            digitalWrite (MotorOpener1, LOW);
            digitalWrite (MotorOpener2, HIGH);
          }

          else if (currentLine.startsWith("GET /Lock")) {
            Lift.write(LiftDown);
            delay(1000);
            Lock();
          }
          else if (currentLine.startsWith("GET /Unlock")) {

            LiftItUp();

          }
          else if (currentLine.startsWith("GET /lift")) {
            Lift.write(LiftUp);

          }
          else if (currentLine.startsWith("GET /holddown")) {
            holdDown();


          }
          else if (currentLine.startsWith("GET /load")) {

            LoadAndLock();

          }
          else if (currentLine.startsWith("GET /unload")) {
            unload();
          }
          else if (currentLine.startsWith("GET /openGate")) {

            Gate.write(180);

          }
          else if (currentLine.startsWith("GET /closeGate")) {

            Gate.write(gateClosed);

          }
          else if (currentLine.startsWith("GET /push")) {
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




//////////////////// FUNCTIONS AND ACTIONS


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
  analogWrite(pwmArm, SpeedVal);
  digitalWrite (MotorArm1, HIGH);
  digitalWrite (MotorArm2, LOW);
  delay(10000);
  StopArm();

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
  analogWrite(pwmArm, SpeedVal);
  digitalWrite (MotorArm1, LOW);
  digitalWrite (MotorArm2, HIGH);
  delay(10000);
  StopArm();
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
  while (obstacleArm()) {
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
  analogWrite(pwmArm, SpeedVal);
  digitalWrite (MotorArm1, HIGH);
  digitalWrite (MotorArm2, LOW);

  // slowly bring tower into position to avoid spilling
  while (TowerPos < 180) {
    TowerPos++;
    Tower.write(TowerPos);
    delay(100);
  }

  delay(3000);
  StopArm();

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
  while (count < 5) {
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
  analogWrite(pwmArm, SpeedVal);
  digitalWrite (MotorArm1, HIGH);
  digitalWrite (MotorArm2, LOW);
  while (!obstacleArm()) {
    yield();
  }
  StopArm();

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

  Clamp.write(ClampClosed);

}

void Reverse()
{
  ReverseDone = false;
  //Arm.writeMicroseconds(ArmCenter);
  //Tower.write(90);
  analogWrite(pwmArm, SpeedVal);
  digitalWrite (MotorArm1, HIGH);
  digitalWrite (MotorArm2, LOW);
  while (!obstacleArm()) {
    yield();
  }
  StopArm();
  ReverseDone = true;
}

void Forward()
{

  ForwardDone = false;
  //Arm.writeMicroseconds(ArmCenter);
  //Tower.write(90);

  analogWrite(pwmArm, SpeedVal);

  digitalWrite (MotorArm1, LOW);
  digitalWrite (MotorArm2, HIGH);

  ForwardDone = true;


}

void StopArm()
{

  digitalWrite (MotorArm1, LOW);
  digitalWrite (MotorArm2, LOW);

}

boolean candetected() {
  bool result;

  if (digitalRead(CanGrabbDetect) == HIGH) {
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

boolean obstacleArm() {
  bool result;

  if (analogRead(ArmSensor) < 100) {
    result = true;
    Serial.print("OBSTACLE");
    StopArm(); // in any case stop it
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


/*
  void PingTest() {
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
*/

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

    while (CanPresence() && count < 50) {

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
    if (!CanPresence()) {
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
  while (!CanPresence() && count < 10) {

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
  StopOpener();
  Unlock();

  Lift.write(LiftUp);
}

void StopOpener() {
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
  analogWrite(pwmOpener, 511); // too fast induces gripp loss
  digitalWrite (MotorOpener1, HIGH);
  digitalWrite (MotorOpener2, LOW);

}

void Off()
{

  digitalWrite (MotorOpener1, LOW);
  digitalWrite (MotorOpener2, LOW);

}

boolean CanPresence() {
  bool result;
  String yesno = "";

  if (analogRead(CanPresenceSensor) < 500) {
    result = true;
    yesno = "YES";
  }
  else {
    result = false;
    yesno = "NO";
  }
  Serial.println("");
  Serial.print("CanPresence: "); Serial.print(yesno);

  /*
    Serial.println("");
    Serial.print("Sensor Read: "); Serial.print(analogRead(CanPresenceSensor));
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
  StopOpener();
  unlockdone = false;
  analogWrite(pwmOpener, SpeedVal);
  count = 0;
  digitalWrite (MotorOpener1, LOW);
  digitalWrite (MotorOpener2, HIGH);
  while (!IsPressed() && count < 10) {
    count++;
    delay(100);
  }
  digitalWrite (MotorOpener1, LOW);
  digitalWrite (MotorOpener2, LOW);
  unlockdone = true;
}

void Lock()
{
  lockdone = false;
  //Off(); // reset the ports status
  delay(100);
  analogWrite(pwmOpener, SpeedVal);
  count = 0;
  digitalWrite (MotorOpener1, HIGH);
  digitalWrite (MotorOpener2, LOW);
  while (IsPressed() && count < 10) {
    count++;
    delay(1);
  }
  delay(1500);
  digitalWrite (MotorOpener1, LOW);
  digitalWrite (MotorOpener2, LOW);
  lockdone = true;
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



