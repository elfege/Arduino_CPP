#include <ICMPPing.h>
#include <util.h>

#include <Servo.h>
#include <SPI.h>


#include <Adafruit_CC3000.h>
#include <Adafruit_CC3000_Server.h>
#include <SPI.h>

//#define DEBUG  //  uncomment for serial debug output

// These are the pins for the CC3000 breakout board, plus the hardware SPI pins
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// CC3000 Status LEDs
#define redLED                 2  //  CC3000 not intialized
#define yellowLED             12  //  Attempting to connect to WLAN
#define greenLED              11  //  WiFi Connected
#define glowLED                6  //  Proof-of-life  Must be a PWM pin

// Create CC3000 instance
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2);
// Wireless network configuration
#define WLAN_SSID       "WIFI_SSID"        // cannot be longer than 32 characters!
#define WLAN_PASS       "WIFI_PASSWORDGahsh4479#$"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2
#define     WLAN_ATTEMPTS   4

// The port to listen for incoming TCP connections 
#define LISTEN_PORT         80

// Server instance
Adafruit_CC3000_Server www(LISTEN_PORT);


byte gateway[] = { 192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network
IPAddress pingAddr(192, 168, 10, 1); // ip address to ping
// EthernetServer server(80);


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


String currentLine = "";   // make a String to hold incoming data from the client

int  DesiredPosDegrees; // outdated variable for unconventional servos that may be in use at some point in this project (using servo function defined below)


bool ReverseDone = false;
bool ForwardDone = false;
bool ServoIsDone = false;
bool CommandFromBringPlate = false;
bool GrabItDone = false;
bool OnreverseDone = false;
bool lockdone = false;
bool unlockdone = false;
bool unloaddone = false;
bool doitall = false;

bool doitallhasrun = false;


int count;
int countping;
int increment;
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
Servo Spin;
Servo Spin2;
Servo Wash;
Servo Elbow;
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
int LiftUp = 150;
int LiftDown = 70;

// gate's positions
int Channel = 125;
int gateBack = 110;
int gateClosed = 6;
int gateHoldLoad = 20;
int gatePushAway = 160;

// pusher 1 positions
int pushit = 60;
int retract = 180;

// pusher 2 positions
int pushit2 = 180;
int retract2 = 50;


// barriers' positions

int barrier1Open = 40;
int barrier1Closed = 80;
int barrier2Open = 40;
int barrier2Closed = 90;


// I/O ports
int const LED = 20; //

int const pwmOpener = 5 ;
int const MotorOpener1 = 16 ;
int const MotorOpener2 = 17;
int const LockPin = 8; // can opener locked position detection
int const CanPresenceSensor1 = A0;  // Distance Sensor sensor Analog input detects can's presence
int const CanPresenceSensor2 = A1;

int const CanGrabbDetect = 9; // digital detection of can being grabbed by the clamp
int const CanOnTrackSensor = A2; // Distance Sensor Analog input detects when can is not far enough from can opener's gate
int const PlateDetect = A3; // Distance Sensor Analog input detects Arm's proximity


//////////// SERVOS' POSITIONS FOR THE CAN SERVER///////////////////////

int Speed = 10;
int StartPos;
int loadDown = 110;
int DesiredPos;
int WristDefault = 0;
int WristGrab = WristDefault;
int WristPour = 180;
int ArmCenter = 100;
int ArmDown = 40;
int ArmUp = 150;

int ClampOpen = 0;
int ClampMidOpen = 40;
int ClampClosed = 55;
int ClampFullClose = 70;
int TowerRight = 180;
int TowerLeft = 0;
int TowerCenter = 90;
int HoldPlateDown = 150;
int ElbowCenter = 55;
int ElbowUp = 130;
int ElbowDown = 0;

int ElbowCatch = 160;
int ArmCatch = 20;
int TowerCatch = 170;
int WristCatch = 0;

int ElbowReadyToCatch = ElbowCatch;
int ArmReadyToCatch = 80;
int TowerReadyToCatch = TowerCatch;
int WristReadyToCatch = WristCatch;


void setup() {

  Serial.begin(115200); 
  delay(10);
  
  Serial.println("Setting I/O Pins");
  pinMode (LED, OUTPUT);

  pinMode (CanOnTrackSensor, INPUT);
  pinMode (PlateDetect, INPUT);
  pinMode(LockPin, INPUT_PULLUP);  // note to myself: used to be INPUT_PULLDOWN_16 on ESP8266 don't forget to reverse values in the coresponding boolean function
  pinMode (pwmOpener, OUTPUT);
  pinMode (MotorOpener1, OUTPUT);
  pinMode (MotorOpener2, OUTPUT);
  pinMode (CanPresenceSensor1, INPUT);
  pinMode (CanPresenceSensor2, INPUT);
  pinMode(CanGrabbDetect, INPUT_PULLUP);// note to myself: used to be INPUT_PULLDOWN_16 on ESP8266 don't forget to reverse values in the coresponding boolean function

  // make sure motors are off
  digitalWrite(MotorOpener1, LOW);
  digitalWrite(MotorOpener1, LOW);

  Serial.println("Attaching Servos");
  Lift.attach(26);
  Gate.attach(27);
  Barrier1.attach(24); // lower barrier
  Barrier2.attach(25); // upper barrier
  Push.attach(22);
  Push2.attach(23);// this is TX's port so needs to be disconnected upon upload
  Wrist.attach(38);
  Clamp.attach(39);
  Tower.attach(36);
  Arm.attach(37);
  Elbow.attach(42);
  Spin.attach(40);
  Spin2.attach(41);
  Wash.attach(2);

  delay(50);
  Serial.println("Positioning Servos");

  Spin.write(90);
  Spin2.write(90);

  Wash.write(0);

  Barrier1.write(barrier1Closed);
  Barrier2.write(barrier2Closed);
  Gate.write(gateHoldLoad);
  Push.write(retract);
  Push2.write(retract2);
  Lift.write(LiftUp);
  Elbow.write(100);
  Wrist.write(WristGrab);
  Clamp.write(ClampMidOpen);
  Arm.write(90);
  Tower.write(100);


  Serial.println("SETUP DONE");

  
}

void loop() {

  digitalWrite(LED, 1);

// the following "gaurd code" does the initial connect to the WLAN and then
  // checks the connect state each time throught loop(). 
  // If the CC3000 disconnects from the WLAN it will automatically attempt to
  // reconnect, waiting for the WLAN to recover or come back into range if necessary.
  if (!cc3000.checkConnected())      // make sure we are connected to wireless network
  {
    Serial.println("No connection");
    if (!init_network())    // try a cold start connect to WLAN
    
    {
      delay(2000);     // if we couldn't connect, try again later
      return;
    }
    // put any code that needs to be run after a sucessful WLAN connect/reconnecrt here
    www.begin(); // start the REST interface web server
    
      Serial.println("Listening for connections ...");
    
  }

  

  if (!doitallhasrun) {
     doitallhasrun = true;
     doitall = true;
     Serial.println("NOT doing it all");
     //LoadAndLock();
  }

  /*
      CanPresence();
    candetected();
    CanIsOnTrack();
    IsPressed();
    delay(1000);
  */


 // Create a client connection
  Adafruit_CC3000_ClientRef client = www.available();
  
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
          blinks = 5;
          lapse = 10;
          Blink();
          Serial.println("");
          Serial.print("currentLine: "); Serial.print(currentLine);//see what was captured

          client.fastrprintln("HTTP/1.1 200 OKOK\r\nContent-Type: text/html\r\n\r\n");
          client.fastrprintln(   "<HTML>");
          client.fastrprintln(   "<BODY style='background-color:grey'>");
          client.fastrprintln(    "<font style='color:black'>");
          client.fastrprintln(    "<font style='font-family:electric toaster'>");
          client.fastrprintln(     "<strong>");
          if (!IsPressed()) {
            client.println("<H3 style='font-size:100%;'>CAN OPENER IS LOCKED</H3>");
          }
          else {
            client.println("<H3 style='font-size:100%;'>CAN OPENER IS UNLOCKED</H3>");
          }
          client.fastrprintln("Current Increment Value = ");
          client.print(increment);
          client.fastrprintln(" Tower Position = ");
          client.print(Tower.read());
          client.fastrprintln(" Arm Position = ");
          client.print(Arm.read());
          client.fastrprintln(" Elbow Position = ");
          client.print(Elbow.read());
          client.fastrprintln(" Wrist Position = ");
          client.print(Wrist.read());
          client.fastrprintln(" Clamp Position = ");
          client.print(Clamp.read());
          client.fastrprintln(F(

                         "<HEADER >"
                         "<H3 style='font-size:150%;'>CAN OPENER</H3>"

                         "<input type=submit value='RUN ALL' style=width:120px;height:30px onClick=location.href='/DoAll'>"
                         //"<span style='display:inline-block; width: 20;'></span>"
                         "<input type=submit value='SPIN TEST' style=width:120px;height:30px onClick=location.href='/spintest'>"
                         "</HEADER >"

                         "<input type=submit value=UNLOCK style=width:120px;height:30px onClick=location.href='/Unlock'>"
                         "<input type=submit value=LOCK style=width:120px;height:30px onClick=location.href='/Lock'> "
                         "<br />"
                         "<input type=submit value=RUN style=width:120px;height:30px onClick=location.href='/on'>"
                         "<input type=submit value=STOP style=width:120px;height:30px onClick=location.href='/off'> "
                         "<br />"
                         "<input type=submit value='Lift Up' style=width:120px;height:30px onClick=location.href='/lift'>"
                         "<input type=submit value='Hold Down' style=width:120px;height:30px onClick=location.href='/holddown'> "
                         "<br />"
                         "<input type=submit value='LOAD A CAN' style=width:120px;height:30px onClick=location.href='/load'>"
                         "<input type=submit value=UNLOAD style=width:120px;height:30px onClick=location.href='/unload'> "
                         "<br />"
                         "<input type=submit value='OPEN GATE' style=width:120px;height:30px onClick=location.href='/openGate'>"
                         "<input type=submit value='CLOSE GATE' style=width:120px;height:30px onClick=location.href='/closeGate'> "
                         "<br />"

                         "<input type=submit value='Open Barrier 1' style=width:120px;height:30px onClick=location.href='/barrier1open'>"
                         "<input type=submit value='Close Barrier 1' style=width:120px;height:30px onClick=location.href='/barrier1closed'> "
                         "<br />"
                         "<input type=submit value='Open Barrier 2' style=width:120px;height:30px onClick=location.href='/barrier2open'>"
                         "<input type=submit value='Close Barrier 2' style=width:120px;height:30px onClick=location.href='/barrier2closed'> "
                         "<br />"
                         "<input type=submit value='PUSH CAN' style=width:120px;height:30px onClick=location.href='/push'>"

                         "<H3 style='font-size:150%;'>CAN SERVER</H3>"

                         "<input type=submit value='Get Ready To Catch' style=width:160px;height:30px onClick=location.href='/getready'>"
                         "<input type=submit value='Catch The Can' style=width:160px;height:30px;color:red onClick=location.href='/catch'>"
                         "<input type=submit value='Pour The Food' style=width:160px;height:30px;color:red onClick=location.href='/pourthefood'>"

                         "<hr>"
                         "<input type=submit value='GRAB' style=width:120px;height:30px onClick=location.href='/wristgrab'>"
                         "<input type=submit value='POUR' style=width:120px;height:30px onClick=location.href='/wristpour'> "
                         "<br />"
                         "<input type=submit value='OPEN CLAMP' style=width:120px;height:30px onClick=location.href='/openclamp'>"
                         "<input type=submit value='CLOSE CLAMP' style=width:120px;height:30px onClick=location.href='/closeclamp'> "
                         "<input type=submit value='full close' style=width:120px;height:30px onClick=location.href='/clampfullclose'> "
                         "<br />"
                         "<input type=submit value='ARM UP' style=width:120px;height:30px onClick=location.href='/armup'>"
                         "<input type=submit value='ARM CENTER' style=width:120px;height:30px onClick=location.href='/armcenter'> "
                         "<input type=submit value='ARM DOWN' style=width:120px;height:30px onClick=location.href='/armdown'> "
                         "<br />"
                         "<input type=submit value='ELBOW UP' style=width:120px;height:30px onClick=location.href='/elbowup'>"
                         "<input type=submit value='ELBOW CENTER' style=width:120px;height:30px onClick=location.href='/elbowcenter'> "
                         "<input type=submit value='ELBOW DOWN' style=width:120px;height:30px onClick=location.href='/elbowdown'> "
                         "<br />"
                         "<input type=submit value='TOWER LEFT' style=width:120px;height:30px onClick=location.href='/towerleft'>"
                         "<input type=submit value='TOWER CENTER' style=width:120px;height:30px onClick=location.href='/towercenter'> "
                         "<input type=submit value='TOWER RIGHT' style=width:120px;height:30px onClick=location.href='/towerright'> "
                         "<br />"
                         "<input type=submit value='WRIST + ' style=width:120px;height:30px onClick=location.href='/wristplus'>"
                         "<input type=submit value='WRIST - ' style=width:120px;height:30px onClick=location.href='/wristminus'> "
                         "<br />"
                         "<input type=submit value='TOWER + ' style=width:120px;height:30px onClick=location.href='/towerplus'>"
                         "<input type=submit value='TOWER - ' style=width:120px;height:30px onClick=location.href='/towerminus'> "
                         "<br />"
                         "<input type=submit value='ELBOW + ' style=width:120px;height:30px onClick=location.href='/elbowplus'>"
                         "<input type=submit value='ELBOW - ' style=width:120px;height:30px onClick=location.href='/elbowminus'> "
                         "<br />"
                         "<input type=submit value='ARM + ' style=width:120px;height:30px onClick=location.href='/armplus'>"
                         "<input type=submit value='ARM - ' style=width:120px;height:30px onClick=location.href='/armminus'> "
                         "<br />"
                         "<input type=submit value='CLAMP + ' style=width:120px;height:30px onClick=location.href='/clampplus'>"
                         "<input type=submit value='CLAMP - ' style=width:120px;height:30px onClick=location.href='/clampminus'> "
                         "<br />"
                         "<input type=submit value='1' style=width:30px;height:30px onClick=location.href='/+1increment'>"
                         "<input type=submit value='2' style=width:30px;height:30px onClick=location.href='/+2increment'>"
                         "<input type=submit value='3' style=width:30px;height:30px onClick=location.href='/+3increment'>"
                         "<input type=submit value='4' style=width:30px;height:30px onClick=location.href='/+4increment'>"
                         "<input type=submit value='5' style=width:30px;height:30px onClick=location.href='/+5increment'>"
                         "<input type=submit value='6' style=width:30px;height:30px onClick=location.href='/+6increment'>"
                         "<input type=submit value='7' style=width:30px;height:30px onClick=location.href='/+7increment'>"
                         "<input type=submit value='8' style=width:30px;height:30px onClick=location.href='/+8increment'>"
                         "<br />"
                         "<input type=submit value='9' style=width:30px;height:30px onClick=location.href='/+9increment'>"
                         "<input type=submit value='10'style=width:30px;height:30px onClick=location.href='/+10increment'>"


                         "<hr />"
                         "<input id = 'servo' type = 'range' min = 0 max = 180 onchange = 'sendPosition(this.id, this.value);'>"


                         "</strong>"
                         "</BODY>"
                         "</HTML>"

                       )); // end of F macro


          delay(10);
          client.close();
          Serial.println("client disonnected");

          // Check request:
          Serial.println("READING COMMAND: "); Serial.print(currentLine);
          if (currentLine.startsWith("GET /on")) {
            On();
          }
          else if (currentLine.startsWith("GET /off")) {

            Off();
          }

          else if (currentLine.startsWith("GET /spintest")) {

            Spin.write(1);
            Spin2.write(1);
            count = 0;
            while (count < 10) {
              Serial.println("SPIN TEST");
              delay(1000);
              count++;
            }
            Spin.write(90);
            Spin2.write(90);
            Serial.println("SPIN TEST TERMINATED");
          }
          else if (currentLine.startsWith("GET /getready")) {
            ReadyToCatch();
          }
          else if (currentLine.startsWith("GET /catch")) {
            Catch();
          }
          else if (currentLine.startsWith("GET /pourthefood")) {
            PourTheFood();
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
            Serial.println("LOCK COMMAND SENT");
            Lift.write(LiftDown);
            delay(1000);
            Lock();
          }
          else if (currentLine.startsWith("GET /Unlock")) {
            Serial.println("UNLOCK COMMAND SENT");
            LiftItUp();

          }
          else if (currentLine.startsWith("GET /lift")) {
            Lift.write(LiftUp);

          }
          else if (currentLine.startsWith("GET /holddown")) {
            holdDown();


          }
          else if (currentLine.startsWith("GET /DoAll")) {
            doitall = true;
            LoadAndLock();

          }
          else if (currentLine.startsWith("GET /load")) {
            doitall = false;
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
            Push2.write(pushit2);

            delay(800);
            Push2.write(retract2);
            Push.write(pushit);
            delay(800);
            Push.write(180);

          }
          else if (currentLine.startsWith("GET /elbowup")) {

            Elbow.write(ElbowUp);
          }
          else if (currentLine.startsWith("GET /elbowcenter")) {

            Elbow.write(ElbowCenter);
          }
          else if (currentLine.startsWith("GET /elbowdown")) {

            Elbow.write(ElbowDown);
          }
          else if (currentLine.startsWith("GET /towerleft")) {
            Serial.println("towerleft COMMAND SENT");

            Tower.write(TowerLeft);
          }
          else if (currentLine.startsWith("GET /towerright")) {
            Serial.println("towerright COMMAND SENT");

            Tower.write(TowerRight);
          }
          else if (currentLine.startsWith("GET /towercenter")) {
            Serial.println("towerright COMMAND SENT");

            Tower.write(TowerCenter);
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
          else if (currentLine.startsWith("GET /openclamp")) {
            Clamp.write(ClampOpen);
          }
          else if (currentLine.startsWith("GET /closeclamp")) {
            Clamp.write(ClampClosed);
          }
          else if (currentLine.startsWith("GET /clampfullclose")) {
            Clamp.write(ClampFullClose);
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
          else if (currentLine.startsWith("GET /clampminus")) {
            int current = Clamp.read();
            int newpos = current - increment;
            Clamp.write(newpos);

          }
          else if (currentLine.startsWith("GET /clampplus")) {
            int current = Clamp.read();
            int newpos = current + increment;
            Clamp.write(newpos);

          }
          else if (currentLine.startsWith("GET /wristminus")) {
            int current = Wrist.read();
            int newpos = current - increment;
            Wrist.write(newpos);

          }
          else if (currentLine.startsWith("GET /wristplus")) {
            int current = Wrist.read();
            int newpos = current + increment;
            Wrist.write(newpos);

          }
          else if (currentLine.startsWith("GET /towerminus")) {
            int current = Tower.read();
            int newpos = current - increment;
            Tower.write(newpos);

          }
          else if (currentLine.startsWith("GET /towerplus")) {
            int current = Tower.read();
            int newpos = current + increment;
            Tower.write(newpos);

          }
          else if (currentLine.startsWith("GET /elbowplus")) {
            int current = Elbow.read();
            int newpos = current + increment;
            Elbow.write(newpos);

          }
          else if (currentLine.startsWith("GET /elbowminus")) {
            int current = Elbow.read();
            int newpos = current - increment;
            Elbow.write(newpos);

          }
          else if (currentLine.startsWith("GET /armplus")) {
            int current = Arm.read();
            Serial.println("");
            Serial.print("Current arm pos = "); Serial.print(current);
            Serial.println("");
            Serial.print("Current Increment Val = "); Serial.print(increment);
            int newpos = current + increment;
            Arm.write(newpos);
            Serial.println("");
            Serial.print("New arm pos = "); Serial.print(Arm.read());

          }
          else if (currentLine.startsWith("GET /armminus")) {
            int current = Arm.read();
            Serial.println("");
            Serial.print("Current arm pos = "); Serial.print(current);
            Serial.println("");
            Serial.print("Current Increment Val = "); Serial.print(increment);
            int newpos = current - increment;
            Arm.write(newpos);
            Serial.println("");
            Serial.print("New arm pos = "); Serial.print(Arm.read());

          }
          else if (currentLine.startsWith("GET /+1increment")) {
            increment = 1;
            Serial.println("");
            Serial.print("New Increment Val = "); Serial.print(increment);
          }
          else if (currentLine.startsWith("GET /+2increment")) {
            increment = 2;
            Serial.println("");
            Serial.print("New Increment Val = "); Serial.print(increment);
          }
          else if (currentLine.startsWith("GET /+3increment")) {
            increment = 3;
            Serial.println("");
            Serial.print("New Increment Val = "); Serial.print(increment);
          }
          else if (currentLine.startsWith("GET /+4increment")) {
            increment = 4;
            Serial.println("");
            Serial.print("New Increment Val = "); Serial.print(increment);
          }
          else if (currentLine.startsWith("GET /+5increment")) {
            increment = 5;
            Serial.println("");
            Serial.print("New Increment Val = "); Serial.print(increment);
          }
          else if (currentLine.startsWith("GET /+6increment")) {
            increment = 6;
            Serial.println("");
            Serial.print("New Increment Val = "); Serial.print(increment);
          }
          else if (currentLine.startsWith("GET /+7increment")) {
            increment = 7;
            Serial.println("");
            Serial.print("New Increment Val = "); Serial.print(increment);
          }
          else if (currentLine.startsWith("GET /+8increment")) {
            increment = 8;
            Serial.println("");
            Serial.print("New Increment Val = "); Serial.print(increment);
          }
          else if (currentLine.startsWith("GET /+9increment")) {
            increment = 9;
            Serial.println("");
            Serial.print("New Increment Val = "); Serial.print(increment);
          }
          else if (currentLine.startsWith("GET /+10increment")) {
            increment = 10;
            Serial.println("");
            Serial.print("New Increment Val = "); Serial.print(increment);
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
  delay(1);
}




//////////////////// ACTIONS


void OpenClamp()
{
  Clamp.write(ClampOpen);
}


void CloseClamp()
{
  Clamp.write(ClampClosed);
}




void Reset() {

  Serial.println("client disonnected");
  Serial.println("");
  Serial.print("Reseting in 3 seconds");
  delay(3000);
  resetFunc();
}



void PingTest() {

  countping = 0;
  pingTestIsRunning = true;
  bool result = false;

  ICMPEchoReply echoReply = ping(pingAddr, 4);
  if (echoReply.status == SUCCESS)
  {

    Serial.println("SUCCESS!");
    blinks = 5;
    lapse = 100;
    Blink();
    

  }
  else
  {
    sprintf(buffer, "Echo request failed; %d", echoReply.status);
    Serial.println("RESET");
    Serial.println("");
    delay(100);
    blinks = 2;
    lapse = 1000;
    Blink();
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


void unload() {

  Unlock();
  Spin.write(80);
  Spin2.write(0);
  while (!unlockdone) {
    yield();
  }
  Lift.write(LiftUp); // lift can opener

  delay(500);
  int a = 0;
  while (a == 0) {
    count = 0;

    while (CanPresence() && count < 2) {
      Gate.write(Channel); //   gate as channel
      // retract pusher
      Push.write(retract);
      Push2.write(retract2);
      // push with gate
      delay(200);

      // push with pushers
      Push.write(pushit);
      Push2.write(pushit2);
      delay(800);
      count++;
    }

    delay(200);
    if (!CanPresence()) {
      break;
    }
    else {

      while (CanPresence() && count < 4) {
        int read1 = analogRead(CanPresenceSensor1); // left sensor
        int read2 = analogRead(CanPresenceSensor2); // right sensor
        Serial.println("");
        Serial.print("CanPresenceSensor1: ");  Serial.print(read1);
        Serial.println("");
        Serial.print("CanPresenceSensor2: ");  Serial.print(read2);
        if (read1 < 500 && read2 > 500) {
          // CanPresenceSensor2 > 500 means can has moved away from right sensor
          // but CanPresenceSensor1 < 500  means it's still there
          gatePushAway = 180; // can moved sufficiently forward, give it a final blow
          Serial.println("");
          Serial.println("FINAL BLOW");

        }

        delay(200);
        Gate.write(gatePushAway);
        delay(600);
        Gate.write(Channel); //  set gate into channel position
        count++;
      }
      Gate.write(180);
      delay(500);
      Gate.write(gateBack);
      Gate.write(180);
    }
  }
  gatePushAway = 150; // restore default value
  Gate.write(gateBack); // leave open
  // retract pushers
  Push.write(retract);
  Push2.write(retract2);
  delay(100);
  Spin.write(90);
  delay(3000);
  Gate.write(180);

  while (CanIsOnTrack()) {
    delay(10);
  }
  Spin2.write(90);
  Gate.write(gateBack); // leave open


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

  Gate.write(gateClosed); // push can back inside
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
  while (count < 2) {

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

  Gate.write(gateClosed); // close gate

  count = 0;

  Gate.write(gateHoldLoad); // close gate
  delay(500);

  Push2.write(pushit2);// keep the whole thing tight
  Gate.write(gateClosed); // keep the whole thing tight

  if (doitall) {
    On(); // holdown, lock and run the can opener
    delay(45000); // wait long enough so we're sure the can is open
    StopOpener();

    // set arm and tower in position
    ReadyToCatch();
    // offload and send the can into arm
    unload();
    delay(500);
    Catch();

  }
  else {
    Serial.println("DONE LOADING");
  }
}



void holdDown() {

  delay(10);
  Unlock();
  Push.write(pushit);
  Gate.write(gateClosed);
  Push2.write(pushit2);
  delay(50);
  int pos = Lift.read();
  int desiredPos = pos - 40;

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

void ReadyToCatch() {
  Elbow.write(ElbowReadyToCatch);
  Arm.write(ArmReadyToCatch);
  delay(1000);
  Tower.write(TowerReadyToCatch);
  Wrist.write(WristReadyToCatch);
  Clamp.write(ClampOpen);

}

void Catch() {

  ReadyToCatch();
  delay(200);
  count = 0;
  Spin2.write(1);
  while (CanIsOnTrack() && count < 200) {
    count++;
  }
  Spin2.write(90);

  Clamp.write(ClampOpen);
  int ELBdesiredPos = ElbowCatch;
  int ArmdesiredPos = ArmCatch;
  int TowerdesiredPos = TowerCatch;
  int WristdesiredPos =  WristGrab;
  int ELBpos = Elbow.read();
  int Armpos = Arm.read();
  int Towerpos = Tower.read();
  int Wristpos = Wrist.read();
  int a = 1;


  while (a == 1) {
    if (ELBpos > ELBdesiredPos) {
      ELBpos --;
    }
    else if (ELBpos < ELBdesiredPos)
    {
      ELBpos ++;
    }
    Elbow.write(ELBpos);
    if (Armpos > ArmdesiredPos) {
      Armpos --;
    }
    else if (Armpos < ArmdesiredPos)
    {
      Armpos ++;
    }
    Arm.write(Armpos);
    if (Towerpos > TowerdesiredPos) {
      Towerpos --;
    }
    else  if (Towerpos < TowerdesiredPos)
    {
      Towerpos ++;
    }
    Tower.write(Towerpos);
    if (Wristpos > WristdesiredPos) {
      Wristpos --;
    }
    else if (Wristpos < WristdesiredPos)
    {
      Wristpos ++;
    }
    Wrist.write(Wristpos);


    delay(100);
    if (ELBpos == ELBdesiredPos && Armpos == ArmdesiredPos && Towerpos == TowerdesiredPos) {
      break;
    }

  }
  /// secure grab
  int countSecureCatch = 0;
  bool startover = false;
  while (!candetected()) {
    Clamp.write(ClampMidOpen);
    delay(500);

    ELBpos = Elbow.read();
    Armpos = Arm.read();
    // record those values :
    int ELBposRec = ELBpos;
    int ArmposRec = Armpos;
    ELBdesiredPos = ELBpos - 20;
    ArmdesiredPos = Armpos + 20;
    count = 0;
    while (ELBpos != ELBdesiredPos && count < 20) {
      if (ELBpos > ELBdesiredPos ) {
        ELBpos --;
      }
      else if (ELBpos < ELBdesiredPos)
      {
        ELBpos ++;
      }
      Elbow.write(ELBpos);
      if (Armpos > ArmdesiredPos) {
        Armpos --;
      }
      else if (Armpos < ArmdesiredPos)
      {
        Armpos ++;
      }
      Arm.write(Armpos);
      delay(100);
      count++;
    }

    countSecureCatch++;
    if (countSecureCatch > 3) {
      startover = true;
      break;
    }
    Clamp.write(ClampClosed);
    delay(400);
    Elbow.write(ELBposRec);
    Arm.write(ArmposRec);
    delay(100);

  }

  // go to plate
  if (!startover) {
    BringToPlate();
  }
  else {
    Clamp.write(ClampOpen);
    delay(400);
    startOver();
  }
}

void startOver() {
  Serial.print("FAILURE, STARTING OVER");
  Catch();
}

void BringToPlate() {

  int ELBpos = Elbow.read();
  int Armpos = Arm.read();
  int Towerpos = Tower.read();
  int Wristpos = Wrist.read();

  int ELBdesiredPos = 92;
  int ArmdesiredPos = 92;
  int TowerdesiredPos = 95;
  int WristdesiredPos = 14;
  count = 0;
  Armpos = Armpos + 15;
  Arm.write(Armpos);
  delay(2000);
  while (count < 200) {

    if (ELBpos > ELBdesiredPos ) {
      ELBpos --;
    }
    else if (ELBpos < ELBdesiredPos)
    {
      ELBpos ++;
    }
    Elbow.write(ELBpos);
    if (Armpos > ArmdesiredPos) {
      Armpos -= 1;
    }
    else if (Armpos < ArmdesiredPos)
    {
      Armpos += 1;
    }
    Arm.write(Armpos);
    if (Towerpos > TowerdesiredPos) {
      Towerpos --;
    }
    else  if (Towerpos < TowerdesiredPos)
    {
      Towerpos ++;
    }
    Tower.write(Towerpos);
    if (Wristpos > WristdesiredPos) {
      Wristpos --;
    }
    else  if (Wristpos < WristdesiredPos)
    {
      Wristpos ++;
    }
    Wrist.write(Wristpos);

    delay(100);
    if (ELBpos == ELBdesiredPos && Wristpos == WristdesiredPos
        && Towerpos == TowerdesiredPos && Armpos == ArmdesiredPos) {
      break;
    }
  }
  delay(2000);
  PourTheFood();
}

void BringAPlate() {
  // get into start position
  Clamp.write(ClampClosed);
  delay(200);
  Tower.write(TowerCenter);
  Arm.write(ArmCenter);
  Elbow.write(ElbowCenter);


  delay(1000);
  Clamp.write(ClampOpen);
  delay(200);
  Arm.write(ArmDown);
  Elbow.write(ElbowDown);


  int ELBpos = Elbow.read();
  int Armpos = Arm.read();
  int Towerpos = Tower.read();
  int Wristpos = Wrist.read();

  int ELBdesiredPos = 92;
  int ArmdesiredPos = 92;
  int TowerdesiredPos = 95;
  int WristdesiredPos = 14;
  int ClampdesiredPos = 50;
  count = 0;
  Armpos = Armpos + 15;
  Arm.write(Armpos);
  delay(2000);
  while (count < 200) {

    if (ELBpos > ELBdesiredPos ) {
      ELBpos --;
    }
    else if (ELBpos < ELBdesiredPos)
    {
      ELBpos ++;
    }
    Elbow.write(ELBpos);
    if (Armpos > ArmdesiredPos) {
      Armpos -= 1;
    }
    else if (Armpos < ArmdesiredPos)
    {
      Armpos += 1;
    }
    Arm.write(Armpos);
    if (Towerpos > TowerdesiredPos) {
      Towerpos --;
    }
    else  if (Towerpos < TowerdesiredPos)
    {
      Towerpos ++;
    }
    Tower.write(Towerpos);
    if (Wristpos > WristdesiredPos) {
      Wristpos --;
    }
    else  if (Wristpos < WristdesiredPos)
    {
      Wristpos ++;
    }
    Wrist.write(Wristpos);

    delay(100);
    if (ELBpos == ELBdesiredPos && Wristpos == WristdesiredPos
        && Towerpos == TowerdesiredPos && Armpos == ArmdesiredPos) {
      break;
    }
  }



}

void PourTheFood() {
  Serial.println("POURING");

  Elbow.write(125);
  Wrist.write(WristPour);
  delay(4000);
  Arm.write(30);
  count = 0;
  while (count < 10) {
    Arm.write(40);
    delay(100);
    Arm.write(50);
    delay(100);
    count++;
  }
  count = 0;
  while (count < 6) {
    Arm.write(50);
    delay(200);
    Arm.write(20);
    delay(200);
    count++;
  }
  delay(2000);
  Wrist.write(WristGrab);
  BringToTrash();
}

void BringToTrash() {
  Arm.write(ArmCenter);
  Elbow.write(ElbowUp);
  delay(500);
  Tower.write(TowerLeft);

  delay(1000);
  Clamp.write(ClampOpen);
  delay(4000);
  Tower.write(TowerCenter);
  Clamp.write(ClampClosed);
  
}

//////////////////////// BOOLEANS
boolean candetected() {
  bool result;

  if (digitalRead(CanGrabbDetect) == LOW) {
    result = true;
    Serial.println("");
    Serial.println("CAN IN CLAMP!");

    // Clamp.write(ClampClosed);
  }
  else {
    result = false;
    Serial.println("");
    Serial.print("NO CAN IN CLAMP!");

    //Clamp.write(ClampOpen);
  }
  return result;
  delay(100);
}

boolean CanIsOnTrack() {
  bool result;

  if (analogRead(CanOnTrackSensor) < 100) {
    result = true;
    Serial.println("");
    Serial.print("CAN ONTO TRACK");
  }
  else {
    result = false;
    Serial.println("");
    Serial.print("NO CAN ON TRACK");
  }
  Serial.println("");

  /*
    Serial.println("");
    Serial.print("DistanceSensor Read: "); Serial.print(analogRead(DistanceSensor));
  */
  return result;
}

boolean PlateInView() {
  bool result;

  if (analogRead(PlateDetect) < 100) {
    result = true;
    Serial.println("");
    Serial.print("PLATE IN VIEW!");
  }
  else {
    result = false;
    Serial.println("");
    Serial.print("NO PLATE");
  }
  Serial.println("");

  /*
    Serial.println("");
    Serial.print("DistanceSensor Read: "); Serial.print(analogRead(DistanceSensor));
  */
  return result;
}

boolean CanPresence() {
  bool result;
  String yesno = "";

  if (analogRead(CanPresenceSensor1) < 500 || analogRead(CanPresenceSensor2) < 500) {
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
    Serial.print("Sensor Read: "); Serial.print(analogRead(CanPresenceSensor1));
  */

  return result;
}

boolean IsPressed()
{

  bool pressed = false;
  String yesno = "";
  if (digitalRead(LockPin) == LOW) {
    pressed = true;
    yesno = "YES";
  }
  else {
    pressed = false;
    yesno = "NO";
  }

  Serial.println("");
  Serial.print("Lock sensor pressed: "); Serial.print(yesno);

  return pressed;
}


///////////////////////////// SERVOS MANAGEMENT /////////////////////////////

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

/////////////////// CC3000 FUNCTIONS

int8_t init_network(void)
{
  cc3000.reboot();
  // Set up the CC3000, connect to the access point, and get an IP address.
  #ifdef DEBUG
    Serial.println(F("Initializing CC3000..."));
  #endif
  digitalWrite(greenLED, LOW);  // green off
  digitalWrite(redLED, HIGH);   // red on
  digitalWrite(yellowLED, LOW); // yellow off
  //digitalWrite(ledPin, LOW);    // on-board LED off

  if (!cc3000.begin())  // fatal
  {                     // the code below will never execute since the call hangs when it fails
    #ifdef DEBUG
      Serial.println(F("Couldn't begin()"));
    #endif
    while(1);
  }
  digitalWrite(redLED, LOW);      // red off
  digitalWrite(yellowLED, HIGH);  // yellow on

  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY, WLAN_ATTEMPTS)) 
  {
    #ifdef DEBUG
      //Serial.println(F("Failed to connect to AP!"));
    #endif
    return false; // fail. Leaves yellow LED on
  }

  digitalWrite(yellowLED, LOW);  // successful connect so turn off yellow LED

  #ifdef DEBUG
    Serial.println(F("Connected to Wireless Network!"));
    Serial.println(F("Request DHCP..."));
  #endif
  while (!cc3000.checkDHCP())
  {
    delay(100);
  }
  #ifdef DEBUG
    Serial.println(F("Got IP"));
  #endif
  digitalWrite(greenLED, HIGH);  //  and turn on green LED
  return true;  // success
}

// Glow status LED
void glow_led() 
{
  unsigned long i = millis();
  int j = i % 2000;
  if (j > 1000) 
  { 
      j = 2000 - j;
  }
  analogWrite(glowLED, j/4);
}

