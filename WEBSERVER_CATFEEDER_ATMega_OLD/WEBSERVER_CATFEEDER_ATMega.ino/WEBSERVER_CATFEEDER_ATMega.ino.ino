#include <ICMPPing.h>
#include <util.h>

#include <Servo.h>
#include <SPI.h>
#include <Ethernet.h>


byte mac[] = {0x02, 0x1A, 0x4E, 0x3F, 0xDC, 0x8E}; // fore the firewall: 02:1A:4A:3E:DC:8E
IPAddress ip(192, 168, 10, 208);
byte gateway[] = { 192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network
IPAddress pingAddr(192, 168, 10, 1); // ip address to ping
EthernetServer server(80);


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

long TimeLimitInSecondsMillVal = 30 * (unsigned long int)seconds;
unsigned long time;
unsigned long TimeB4Tests = (unsigned long int)time + (unsigned long int)TimeLimitInSecondsMillVal;

unsigned long TimeB4Ping = (unsigned long int)time + (unsigned long int)TimeLimitInMinutesMillVal;
unsigned long TimeB4Reset = (unsigned long int)time + (unsigned long int)TimeLimitInHoursMillVal;
int ForceResetHours = 2;
//int ForceResetTimeInMinutes = 2 ;
long ForceResetMill = ForceResetHours * (unsigned long int)hour;
// timer related ping values
bool pingTestIsRunning = false;
long millisValueAtTest = 0;

bool platehasbeenpushed = true;

bool plateseenPassing = false;

bool  justcatchit = false;
bool simplebringtotrash = false;
bool ignore = false;
bool TryAgain = false;
bool BREAK = false;
bool NeedGrabber = false;
int locateTries = 0;
bool locatetest = false;
bool ForwardHasRun = false;

bool extracted = true;// defautl is true to avoid extractor getting out of track

bool totalfailure = false;


bool powerIsOn = false;

int tryagainPos;

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
bool doitallTest = false;

bool doitallhasrun = false;


int count;

int countping;
int increment;
int blinks;
int blinksCount;
int lapse = 200;
int delayservo = 20;

int failed = 0;
int countSecureCatch = 0;
int calTRY = 0;
int failedPush = 0;

int ArmToCan = 170;

int restartReverse = 0;

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
int LiftDown = 65;

// gate's positions
int Channel = 125;
int gateBack = 110;
int gateClosed = 0;
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
int const LED = 13; //

int const PWSW = 46;
int const pwmOpener = 5 ;
int const MotorOpener1 = 16 ;
int const MotorOpener2 = 17;
int const LockPin = 8; // can opener locked position detection
int const CanPresenceSensor1 = A0;  // Distance Sensor sensor Analog input detects can's presence
int const CanPresenceSensor2 = A1;

//int const CanGrabbDetect = 9; // digital detection of can being grabbed by the clamp
int const CanOnTrackSensor = A2; // Distance Sensor Analog input detects when can is not far enough from can opener's gate
int const CanGrabbDetect2 = A3; // Distance Sensor Analog input detects Arm's proximity

int const CanExtractorSensorIN = A9; // no more pullup digital port available...
int const CanExtractorSensorOUT = A10; // no more pullup digital port available...
int const CanExtractorMotor1 = 11;
int const CanExtractorMotor2 = 12;
int const pwmCanExtractor = 10 ;


//// plate extractor
int const pwmArm = 6;
int const ArmMotor1 = 14 ;
int const ArmMotor2 = 15;
int const pwmPEXT = 9;
int const PlateExtractorMotor1 = 4;
int const PlateExtractorMotor2 = 45;
int const PlateSensor = 19;
int const PlateSensorIR = A15;
int const PlateGrabSensor = A13;
int const LastPlate = A5;
int const PlateOnTheWay = A11;
int const IRArmSensor = A14;


int device;
int DesiredPos;
int deviceport;

int PEXTSpeed = 255;

int extractorStop = 0;
int extractorRun = 180;
int extractorReverse = 1;
int PlateBarrierClosed = 6;
int PlateBarrierOpen = 60;

int ArmLiftDown = 0;
int ArmLiftCenter = 30;
int ArmLiftUp = 60;


//////////// SERVOS' POSITIONS FOR THE CAN SERVER///////////////////////

int Speed = 10;
int StartPos;
int loadDown = 110;
int WristDefault = 4;
int WristGrab = WristDefault;
int WristPour = 180;
int ArmCenter = 100;
int ArmDown = 40;
int ArmUp = 120;

int ClampOpen = 0;
int ClampMidOpen = 40;
int ClampClosed = 65;
int ClampFullClose = 75;
int TowerRight = 180;
int TowerLeft = 0;
int TowerCenter = 100;
int HoldPlateDown = 150;
int ElbowCenter = 50;
int ElbowUp = 130;
int ElbowDown = 0;

int GrabberClosed =  160;
int GrabberOpen = 80;

int closeTrash = 10;
int openTrash = 40;

int ElbowCatch = 170;
int ArmCatch = 28;
int TowerCatch = 175;
int WristCatch = 0;

int ElbowReadyToCatch = ElbowCatch;
int ArmReadyToCatch = 80;
int TowerReadyToCatch = TowerCatch;
int WristReadyToCatch = WristCatch;


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
//Servo PlateExtractor;
Servo PlateBarrier;
Servo ArmLift;
Servo TrashCan;
Servo Grabber;


void(* resetFunc) (void) = 0;  // declare reset fuction at address 0

void setup() {

  delay( 500 );   // allow some time (500 ms) after powerup and sketch start,
  Serial.begin(9600);
  delay(100);

  Serial.println("Initializing Ethernet Protocol");
  Ethernet.begin(mac, ip);
  Serial.println("Starting Ethernet Server");
  server.begin();

  delay(50);

  Serial.println(Ethernet.localIP());

  Serial.println("Setting I/O Pins");
  pinMode (LED, OUTPUT);
  delay(1);
  digitalWrite(LED, 1);

  pinMode (PlateExtractorMotor1, OUTPUT);
  pinMode (PlateExtractorMotor2, OUTPUT);
  pinMode (pwmPEXT, OUTPUT);
  pinMode (CanOnTrackSensor, INPUT);
  pinMode (CanGrabbDetect2, INPUT); // little IR proximity sensor inside the clamp to work in parallel with contact sensor "CanGrabbDetect"
  pinMode (LockPin, INPUT_PULLUP);  // note to myself: used to be INPUT_PULLDOWN_16 on ESP8266 don't forget to reverse values in the coresponding boolean function
  pinMode (pwmOpener, OUTPUT);
  pinMode (MotorOpener1, OUTPUT);
  pinMode (MotorOpener2, OUTPUT);
  pinMode (PWSW, OUTPUT);
  delay(1);
  digitalWrite(PWSW, 1); // sw off by default

  pinMode (pwmCanExtractor, OUTPUT);
  pinMode (CanExtractorMotor1, OUTPUT);
  pinMode (CanExtractorMotor2, OUTPUT);
  pinMode (CanExtractorSensorIN, INPUT);// Arm retracted sensor ANALOG used as digital
  pinMode (CanExtractorSensorOUT, INPUT);// Arm Extended sensor ANALOG used as digital


  pinMode (CanPresenceSensor1, INPUT);
  pinMode (CanPresenceSensor2, INPUT);

  // pinMode(CanGrabbDetect, INPUT_PULLUP);// note to myself: used to be INPUT_PULLDOWN_16 on ESP8266 don't forget to reverse values in the coresponding boolean function
  pinMode (ArmMotor1, OUTPUT);
  pinMode (ArmMotor2, OUTPUT);
  pinMode (PlateSensor, INPUT_PULLUP);
  pinMode (PlateSensorIR, INPUT);

  pinMode (PlateGrabSensor, INPUT);
  pinMode (PlateOnTheWay, INPUT);
  pinMode (IRArmSensor, INPUT);


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
  //PlateExtractor.attach(4);
  PlateBarrier.attach(3);
  ArmLift.attach(18);
  TrashCan.attach(21);
  Grabber.attach(43);

  delay(10);
  blinks = 5;
  lapse = 100;
  Blink();


  Serial.println("Positioning Servos");
  Tower.write(TowerCenter);

  Barrier1.write(barrier1Closed);

  Barrier2.write(barrier2Closed);

  Gate.write(Channel);
  Push.write(retract);
  Push2.write(retract2);
  Lift.write(LiftUp);

  Wrist.write(WristGrab);
  Clamp.write(ClampClosed);
  Elbow.write(70);
  Arm.write(ArmUp - 40);

  ArmLift.write(ArmLiftUp);
  Clamp.write(ClampMidOpen);
  Grabber.write(GrabberOpen);
  Spin.write(90);
  Spin2.write(90);

  Wash.write(0);
  delay(100);

  TrashCan.write(closeTrash);
  Arm.write(ArmUp);
  PlateBarrier.write(PlateBarrierClosed);

  ReverseArm();

  Serial.println("SETUP DONE");

}

void loop() {

  time = millis();

  if (time >= TimeB4Tests) {
    //Serial.println("blink");
    blinks = 5;
    lapse = 100;
    Blink();
    millisValueAtTest = millis();
    TimeB4Tests = (unsigned long int)millisValueAtTest + (unsigned long int)TimeLimitInSecondsMillVal;



  }

  EthernetClient client = server.available();

  if (client) {                             // if you get a client,
    // always power on when a request is sent
    // if (digitalRead(PWSW) == 1) {
    // PowerOn();
    // }
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

          client.print("HTTP/1.1 200 OKOK\r\nContent-Type: text/html\r\n\r\n");
          client.print(   "<HTML>");
          client.print(   "<BODY style='background-color:grey'>");
          client.print(    "<font style='color:black'>");
          client.print(    "<font style='font-family:electric toaster'>");
          client.print(     "<strong>");
          if (!IsPressed()) {
            client.println("<H3 style='font-size:100%;'>CAN OPENER IS LOCKED</H3>");
          }
          else {
            client.println("<H3 style='font-size:100%;'>CAN OPENER IS UNLOCKED</H3>");
          }
          client.print("Current Increment Value = ");
          client.print(increment);
          client.println(" Tower Position = ");
          client.print(Tower.read());
          client.println(" Arm Position = ");
          client.print(Arm.read());
          client.println(" Elbow Position = ");
          client.print(Elbow.read());
          client.println(" Wrist Position = ");
          client.print(Wrist.read());
          client.println(" Clamp Position = ");
          client.print(Clamp.read());
          client.println(" Lift position = ");
          client.print(Lift.read());
          client.println(" ArmLift Position = ");
          client.print(ArmLift.read());
          client.println(" PlateSensor Value = ");
          client.print(digitalRead(PlateSensor));

          client.print(F(

                         "<HEADER >"
                         "<H3 style='font-size:150%;'>CAN OPENER</H3>"
                         "<input type=submit value='TEST LOOP' style=width:120px;height:30px onClick=location.href='/runtestloop'>"
                         "<input type=submit value='TEST LOGS' style=width:120px;height:30px onClick=location.href='/runtestlogs'>"

                         "<center>"
                         "<hr>"
                         "<input type=submit value='SWITCH ON (unsafe)' style=width:160px;height:50px;color:red onClick=location.href='/switchon'>"

                         "<input type=submit value='POWER ON' style=width:160px;height:50px;color:red onClick=location.href='/poweron'>"
                         "<input type=submit value='POWER OFF' style=width:160px;height:50px;color:red onClick=location.href='/poweroff'>"
                         "</hr>"
                         "</center>"
                         "<input type=submit value='RUN ALL' style=width:120px;height:30px onClick=location.href='/DoAll'>"
                         "<input type=submit value='RUN ALL TEST' style=width:120px;height:30px;color:blue onClick=location.href='/AllTest'>"

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
                         "<input type=submit value='Pour The Food' style=width:160px;height:30px;color:blue onClick=location.href='/pourthefood'>"
                         "<input type=submit value='Bring To Trash' style=width:160px;height:30px;color:blue onClick=location.href='/bringtotrash'>"
                         "<br />"
                         "<hr>"
                         "<input type=submit value='Simple Catch' style=width:160px;height:30px;color:blue onClick=location.href='/justcatch'>"
                         "<input type=submit value='Simple Trash' style=width:160px;height:30px;color:blue onClick=location.href='/justbringtotrash'>"

                         "<hr>"
                         "<br />"
                         "<input type=submit value='Extractor FWD' style=width:160px;height:30px;color:blue onClick=location.href='/extractorFWD'>"
                         "<input type=submit value='Extractor RVS' style=width:160px;height:30px;color:blue onClick=location.href='/extractorRVS'>"
                         "<input type=submit value='Extractor STP' style=width:160px;height:30px;color:blue onClick=location.href='/extractorSTP'>"
                         "<br />"
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
                         "<input type=submit value='LIFT + ' style=width:120px;height:30px onClick=location.href='/liftplus'>"
                         "<input type=submit value='LIFT - ' style=width:120px;height:30px onClick=location.href='/liftminus'> "
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

                         "<br />"
                         "<hr >"
                         "<H3 style='font-size:150%;'>PLATE PUSH</H3>"

                         "</hr >"
                         "<input type=submit value='PUSH PLATE' style=width:120px;height:30px onClick=location.href='/pushplate'>"
                         "<br />"
                         "<input type=submit value='FORWARD' style=width:120px;height:30px onClick=location.href='/forward'>"
                         "<input type=submit value='REVERSE' style=width:120px;height:30px onClick=location.href='/reverse'>"
                         "<input type=submit value='STOP' style=width:120px;height:30px onClick=location.href='/stop'>"

                         "<br />"
                         "<input type=submit value='CLOSE PLATE BARRIER' style=width:250;height:30px onClick=location.href='/closeplatebarrier'>"
                         "<input type=submit value='OPEN PLATE BARRIER' style=width:250;height:30px onClick=location.href='/openplatebarrier'>"
                         "<br />"
                         "<input type=submit value='SPIN TEST' style=width:120px;height:30px onClick=location.href='/spintest'>"
                         "<br />"
                         "<input type=submit value='Plate Extractor REVERSE' style=width:180px;height:30px onClick=location.href='/Plateextractorreverse'>"
                         "<input type=submit value='Plate Extractor FWD' style=width:180px;height:30px onClick=location.href='/Plateextractorrun'>"
                         "<input type=submit value='Plate Extractor STOP' style=width:180px;height:30px onClick=location.href='/Plateextractorstop'>"

                         "<br />"
                         "<br />"
                         "<input type=submit value='ARM DOWN ' style=width:120px;height:30px onClick=location.href='/armLiftdown'>"
                         "<input type=submit value='ARM CENTER ' style=width:120px;height:30px onClick=location.href='/armLiftcenter'> "
                         "<input type=submit value='ARM UP ' style=width:120px;height:30px onClick=location.href='/armLiftup'> "

                         "<br />"
                         "<input type=submit value='ARM + ' style=width:120px;height:30px onClick=location.href='/armliftplus'>"
                         "<input type=submit value='ARM - ' style=width:120px;height:30px onClick=location.href='/armliftminus'> "
                         "<br />"
                         "<input type=submit value='OPEN TRASH' style=width:120px;height:30px onClick=location.href='/opentrash'>"
                         "<input type=submit value='CLOSE TRASH' style=width:120px;height:30px onClick=location.href='/closetrash'> "
                         "<input type=submit value='TRASH + ' style=width:120px;height:30px onClick=location.href='/trashplus'>"
                         "<input type=submit value='TRASH - ' style=width:120px;height:30px onClick=location.href='/trashminus'> "
                         "<br />"

                         "<hr />"
                         "<input id = 'servo' type = 'range' min = 0 max = 180 onchange = 'sendPosition(this.id, this.value);'>"


                         "</strong>"
                         "</BODY>"
                         "</HTML>"

                       )); // end of F macro


          delay(1);
          client.stop();
          Serial.println("client disonnected");

          // Check request:
          Serial.println("READING COMMAND: "); Serial.print(currentLine);

          if (currentLine.startsWith("GET /pushplate")) {
            LocatePlate();
          }
          else if (currentLine.startsWith("GET /poweron")) {
            PowerOn();
          }
          else if (currentLine.startsWith("GET /switchon")) {
            digitalWrite(PWSW, 0);
            powerIsOn = true;
          }
          else if (currentLine.startsWith("GET /extractorFWD")) {
            ForwardExtractor();
          }
          else if (currentLine.startsWith("GET /extractorRVS")) {
            ReverseExtractor();
          }
          else if (currentLine.startsWith("GET /extractorRVS")) {
            StopExtractor();
          }
          else if (currentLine.startsWith("GET /poweroff")) {
            PowerOff();
          }
          else if (currentLine.startsWith("GET /openplatebarrier")) {
            PlateBarrier.write(PlateBarrierOpen);
          }
          else if (currentLine.startsWith("GET /closeplatebarrier")) {
            PlateBarrier.write(PlateBarrierClosed);
          }
          else if (currentLine.startsWith("GET /forward")) {
            ForwardArm();
          }

          else if (currentLine.startsWith("GET /reverse")) {
            ReverseArm();
          }
          else if (currentLine.startsWith("GET /Plateextractorreverse")) {
            ReversePlateExtractor();

          }
          else if (currentLine.startsWith("GET /Plateextractorrun")) {
            ForwardPlateExtractor();
          }
          else if (currentLine.startsWith("GET /Plateextractorstop")) {
            StopPlateExtractor();
          }

          else if (currentLine.startsWith("GET /spintest")) {

            ForwardPlateExtractor();

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
            ReversePlateExtractor();
            delay(5000);
            StopPlateExtractor();
            Serial.println("SPIN TEST TERMINATED");
          }
          else if (currentLine.startsWith("GET /stop")) {
            digitalWrite(ArmMotor1, LOW);
            digitalWrite(ArmMotor2, LOW);
          }
          else if (currentLine.startsWith("GET /armLiftdown")) {

            ArmLift.write(ArmLiftDown);
          }
          else if (currentLine.startsWith("GET /armLiftcenter")) {

            ArmLift.write(ArmLiftCenter);
          }
          else if (currentLine.startsWith("GET /armLiftup")) {

            ArmLift.write(ArmLiftUp);
          }
          else if (currentLine.startsWith("GET /closetrash")) {
            TrashCan.write(closeTrash);
          }
          else if (currentLine.startsWith("GET /opentrash")) {
            TrashCan.write(openTrash);
          }
          else if (currentLine.startsWith("GET /armliftplus")) {
            int current = ArmLift.read();
            int newpos = current + increment;

            DesiredPos = newpos;
            ArmLift.write(DesiredPos);


          }
          else if (currentLine.startsWith("GET /armliftminus")) {
            int current = ArmLift.read();
            int newpos = current - increment;
            DesiredPos = newpos;
            ArmLift.write(DesiredPos);

          }
          else if (currentLine.startsWith("GET /on")) {
            On();
          }
          else if (currentLine.startsWith("GET /off")) {

            Off();
          }

          else if (currentLine.startsWith("GET /getready")) {
            ReadyToCatch();
          }
          else if (currentLine.startsWith("GET /catch")) {
            justcatchit = false;
            Catch();
          }
          else if (currentLine.startsWith("GET /justcatch")) {
            justCatch();
          }
          else if (currentLine.startsWith("GET /justbringtotrash")) {
            SimpleBringToTrash();
          }

          else if (currentLine.startsWith("GET /pourthefood")) {
            PourTheFood();
          }
          else if (currentLine.startsWith("GET /bringtotrash")) {
            BringToTrash();
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
            doitallTest = false;
            LoadAndLock();

          }
          else if (currentLine.startsWith("GET /AllTest")) {
            doitall = false;
            doitallTest = true;
            LoadAndLock();

          }
          else if (currentLine.startsWith("GET /runtestloop")) {
            TEST();

          }
          else if (currentLine.startsWith("GET /runtestlogs")) {
            logs();

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
          else if (currentLine.startsWith("GET /liftminus")) {
            int current = Lift.read();
            int newpos = current - increment;
            Lift.write(newpos);

          }
          else if (currentLine.startsWith("GET /liftplus")) {
            int current = Lift.read();
            int newpos = current + increment;
            Lift.write(newpos);

          }
          else if (currentLine.startsWith("GET /trashplus")) {
            int current = TrashCan.read();
            int newpos = current + increment;
            TrashCan.write(newpos);

          }
          else if (currentLine.startsWith("GET /trashminus")) {
            int current = TrashCan.read();
            int newpos = current - increment;
            TrashCan.write(newpos);

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
          Serial.println("");
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

void unload() {
  Gate.write(Channel); //  leave gate open
  Spin.write(1);
  Spin2.write(1);
  delay(300);
  Unlock();
  ForwardExtractor();
  ReverseExtractor();
  Lift.write(LiftUp); // lift can opener
  Gate.write(Channel); //  open
  count = 0;
  while ((CanPresence() || CanIsOnTrack()) && count < 10) {
    Push.write(retract);
    delay(500);
    ForwardExtractor();
    delay(400);
    ReverseExtractor();
    Push.write(pushit);
    delay(1500);
    Serial.println("");
    Serial.print("BREAK UNLOAD");
    count++;
  }

  ReadyToCatch();

  delay(1000);

  Spin.write(90);
  Spin2.write(90);



  ReverseExtractor();
  Push.write(retract);



}


void LoadAndLock() {

  countSecureCatch = 0; // catch error handling counter
  PowerOn();

  delay(10);
  //Barrier1.write(barrier1Closed); // close first barrier to hold the can
  //delay(1000);
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

  Push2.write(retract2);// keep the whole thing tight
  Gate.write(gateClosed); // keep the whole thing tight

  if (doitall) {
    On(); // holdown, lock and run the can opener
    delay(45000); // wait long enough so we're sure the can is open
    StopOpener();
    Gate.write(Channel); // open gate
    // set arm and tower in position
    ReadyToCatch();
    // offload and send the can into arm
    unload();
    delay(500);
    Catch();

  }
  else {
    Serial.println("DONE LOADING");
    if (doitallTest) {
      Serial.println("FULL TEST WITHOUT OPENING CAN");
      unload();
      delay(500);
      Catch();
    }
  }
}



void holdDown() {

  delay(10);
  Unlock();
  Push.write(pushit);
  Gate.write(gateClosed);
  Push2.write(pushit2);
  delay(500);
  Lift.write(LiftDown);
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
  while (!IsPressed() && count < 30) {
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
  Gate.write(gateClosed);
  Push2.write(pushit2);
  delay(100);
  Lift.write(LiftDown);
  delay(1000);
  Lift.write(LiftUp + 30);
  delay(1000);
  Lift.write(LiftDown);
  count = 0;
  while (count < 5) {
    Lift.write(LiftDown + 2);
    Lift.write(LiftDown);
    count++;
  }
  //Off(); // reset the ports status
  delay(100);
  analogWrite(pwmOpener, SpeedVal);
  count = 0;
  digitalWrite (MotorOpener1, HIGH);
  digitalWrite (MotorOpener2, LOW);
  while (IsPressed() && count < 10) {
    count++;
    delay(10);
  }
  delay(1500);
  digitalWrite (MotorOpener1, LOW);
  digitalWrite (MotorOpener2, LOW);
  lockdone = true;
}


void Blink() {
  blinksCount = 0;

  while (blinksCount < blinks) {
    digitalWrite(LED, 0); // led off
    delay(lapse);
    digitalWrite(LED, 1); // led on
    delay(lapse);
    blinksCount++;
  }
  digitalWrite(LED, 1); // led on


}

void ReadyToCatch() {
  Gate.write(Channel);
  Elbow.write(ElbowReadyToCatch);
  delay(200);
  Arm.write(ArmReadyToCatch);
  delay(200);
  Tower.write(TowerReadyToCatch);
  delay(200);
  Wrist.write(WristReadyToCatch);
  delay(200);
  Clamp.write(ClampOpen);

  Spin2.write(0);
  Spin.write(0);



  while (CanIsOnTrack()) {
    ;
  }
  Spin2.write(90);
  Spin.write(90);

}

void Catch() {

  Clamp.write(ClampOpen);
  delay(400);

  ReadyToCatch();

  Spin2.write(0);
  Spin.write(0);
  while (CanIsOnTrack() && count < 3) {
    ForwardExtractor();
    ReverseExtractor();
    count++;
  }
  Spin2.write(90);
  Spin.write(90);

  int ELBdesiredPos = ElbowCatch;
  int ArmdesiredPos = ArmCatch;
  int TowerdesiredPos = TowerCatch;
  int WristdesiredPos =  WristGrab;
  int ELBpos = Elbow.read();
  int Armpos = Arm.read();
  int Towerpos = Tower.read();
  int Wristpos = Wrist.read();

  while (1) {
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


    delay(delayservo);
    if (ELBpos == ELBdesiredPos && Armpos == ArmdesiredPos && Towerpos == TowerdesiredPos) {
      break;
    }

  }

  /// secure grab


  bool startover = false;
  totalfailure = false;


  while (1) {
    int pos;
    int desiredpos;
    Clamp.write(ClampClosed);
    delay(500);

    count = 0;

    countSecureCatch++;
    bool CWCLPhasRun = false;
    if (countSecureCatch > 4 && !CWCLPhasRun) {
      catchwithclamp();
      countSecureCatch = 0;
      CWCLPhasRun = true;
    }
    else if (CWCLPhasRun) {
      Serial.println("");
      Serial.print("TOTAL FAILURE");
      break;
    }

    count = 0;
    Arm.write(ArmdesiredPos);
    delay(400);

    if (candetected() || ignore) {
      Serial.println("");
      Serial.println("SUCCESS!!");

      while (count < 3) {
        Clamp.write(ClampMidOpen);
        delay(200);
        Clamp.write(ClampClosed);
        delay(200);
        count++;
      }
      Clamp.write(ClampClosed + 10);
      delay(800);

      if (!justcatchit && candetected()) {
        BringToPlate();
        break;
      }
      else if (justcatchit) {
        // test mode so lift it up to see if ok
        Serial.println("");
        Serial.print("TEST MODE");
        delay(400);
        Arm.write(ArmUp - 30);

        delay(800);
        Tower.write(TowerCenter);

        break;
      }
      else if (!candetected()) {
        Serial.println("");
        Serial.print("times failed: "); Serial.print(failed);
        failed++;
        if (failed < 4) {
          startOver();
        }
        else {
          Serial.println("TOTAL FAILURE");
          totalfailure = true;
          ignore = true;
          startOver();
          break;
        }
      }
      else {
        Serial.println("INCONSISTENT SENSOR FEEDBACK");
        startOver();
        break;
      }
    }
    else {
      Serial.println("NO CAN");
      failed++;
      if (failed < 4) {
        startOver();
        break;
      }
      else {
        Serial.println("TOTAL FAILURE, Proceeding without sensor");
        ignore = true;
        totalfailure = true;
        startOver();
        break;
      }
    }
  }
}

void catchwithclamp() {


  while (count < 3) {
    ReverseExtractor();

    Serial.println("");
    Serial.println("");
    Serial.print("TRYING TO BRING CAN DOWN WITH ARM");
    Clamp.write(ClampMidOpen);
    Wrist.write(WristGrab);
    delay(500);
    Arm.write(ArmCenter + 20);
    Clamp.write(ClampOpen);
    Elbow.write(ElbowUp);

    delay(500);
    Clamp.write(ClampClosed);
    delay(1000);
    Tower.write(180);

    delay(1000);

    Clamp.write(ClampOpen);
    while (Arm.read() > ArmCenter - 20) {
      Serial.println("pos");
      Arm.write(Arm.read() - 1);
      delay(100);

    }

    Spin.write(1);
    Spin2.write(1);
    ForwardExtractor();
    ReverseExtractor();
    delay(1500);

    while (Arm.read() > ArmCatch) {
      Serial.println("pos");
      Arm.write(Arm.read() - 1);
      delay(100);
    }


    /// arm is down probably touching can by now
    // pull can to the right
    // try 3 times



    while (Tower.read() > 170) {
      Tower.write(Tower.read() - 1);
      delay(100);

    }
    while (Tower.read() > 165 && !CanIsOnTrack()) {
      Tower.write(Tower.read() - 1);
      delay(100);
    }
    if (!CanIsOnTrack() || Tower.read() < 160) {
      //Tower.read < 160 means hasn't reached out due can seen on track
      // because canisontrack() could be no longer valid despite can
      // has been seen passing

      countSecureCatch = 0;

      Serial.println("");
      Serial.print("CAN RETRIEVED!!!!!");
      Spin.write(90);
      Spin2.write(90);
      break;
    }
    Spin.write(90);
    Spin2.write(90);
    count++;
  }


}


void justCatch() {
  /// FOR TESTS ONLY
  justcatchit = true;
  Catch();
  justcatchit = false;

}

void startOver() {
  Serial.println("");
  Serial.print("FAILURE, STARTING OVER");
  Catch();
}

void BringToPlate() {
  if (!candetected()) {
    Serial.print("CAN LOST");
    failed++;
    if (failed < 4) {
      startOver();
    }
    Serial.print("NO CAN, TOTAL FAILLURE");
  }
  else {
    while (1) {
      Clamp.write(ClampClosed + 10);

      int ELBpos = Elbow.read();
      int Armpos = Arm.read();
      int Towerpos = Tower.read();
      int Wristpos = Wrist.read();

      int ELBdesiredPos = 92;
      int ArmdesiredPos = 92;
      int TowerdesiredPos = 95;
      int WristdesiredPos = 14;
      count = 0;
      int pos = Armpos;
      Armpos = Armpos + 15;
      while (pos < Armpos) {
        pos++;
        Arm.write(pos);
        delay(100);
      }
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

        delay(delayservo);
        if (ELBpos == ELBdesiredPos && Wristpos == WristdesiredPos
            && Towerpos == TowerdesiredPos && Armpos == ArmdesiredPos) {
          break;
        }
      }
      delay(2000);
      if (!candetected()) {
        Serial.print("CAN LOST");
        failed++;
        if (failed < 4) {
          startOver();
        }
        Serial.print("NO CAN, TOTAL FAILLURE");
      }
      else {
        PourTheFood();
      }
      break;
    }
  }
}

void PourTheFood() {
  int desiredpos;
  int pos;

  Arm.write(80);
  Tower.write(TowerCenter + 15);
  Serial.println("POURING");

  Elbow.write(ElbowCenter + 35);

  delay(100);
  while (Wrist.read() < WristPour) {
    Wrist.write(Wrist.read() + 1);
    delay(5);
  }


  count = 0;

  delay(500);
  Wrist.write(WristPour);

  /// bring arm down
  desiredpos = 30;
  pos = Arm.read();

  Elbow.write(ElbowUp);
  delay(200);

  //Shake it out

  int up = 50;
  int down = 30;
  count = 0;
  while (count < 20) {

    // shake it out
    Arm.write(down);
    delay(100);
    Arm.write(up);
    delay(100);

    while (Wrist.read() < WristPour) {
      Wrist.write(Wrist.read() + 1);
      delay(5);
    }
    count++;
  }
  if (doitallTest) {
    Arm.write(50);
    delay(500);
    Clamp.write(ClampMidOpen); // leave can on plate
    delay(500);
    Serial.println("");
    Serial.print("LEAVING CAN ON PLATE");
  }
  desiredpos = 80;
  pos = Arm.read();
  while (pos < desiredpos) {
    pos++;
    Arm.write(pos);
    delay(50);
  }

  delay(2000);
  Wrist.write(WristGrab);

  BringToTrash();
}

void BringToTrash() {
  TrashCan.write(openTrash);
  /// first lift the arm and elbow up
  int Armdesiredpos = ArmUp - 10;  // up to avoid driping on food
  int pos = Arm.read();
  while (pos < Armdesiredpos) {
    pos++;
    Arm.write(pos);
    delay(delayservo);
  }
  Elbow.write(ElbowUp);

  delay(500);

  //bring tower to position
  int Towerdesiredpos = 50;  // tower to trash
  pos = Tower.read();
  while (pos > Towerdesiredpos) {
    pos--;
    Tower.write(pos);
    delay(delayservo);
  }

  Elbow.write(ElbowUp);
  Arm.write(50);
  delay(500);
  /// release the can
  int currArm = Arm.read();
  Clamp.write(ClampMidOpen - 20);
  delay(3000);

  // get out of trash's perimeter
  Clamp.write(ClampClosed);
  delay(500);
  Arm.write(ArmUp);
  delay(500);
  Elbow.write(ElbowUp);

  TrashCan.write(closeTrash);
  // wait for trash to slowly close
  delay(5000);

  // now go clean the clamp

  if (!simplebringtotrash) {
    CleanClamp();
  }
  else {
    simplebringtotrash = false;
  }


}

void SimpleBringToTrash() {
  simplebringtotrash = true;
  BringToTrash();

}

void CleanClamp() {
  TrashCan.write(closeTrash);
  Clamp.write(ClampFullClose);
  // wash

  int washcount = 0;
  int Armdesiredpos;
  int Towerdesiredpos;
  int pos;
  if (!doitallTest) {
    Towerdesiredpos = TowerLeft;  // tower to trash
    pos = Tower.read();
    while (pos > Towerdesiredpos) {
      pos--;
      Tower.write(pos);
      delay(delayservo);
    }

    while (washcount < 1) {
      // down to the bucket
      Elbow.write(ElbowCenter + 30);
      Armdesiredpos = 0;
      pos = Arm.read();
      while (pos > Armdesiredpos) {
        pos--;
        Arm.write(pos);
        delay(delayservo);
      }
      delay(100);
      count = 0;
      while (count < 30) {
        delay(300);
        Tower.write(TowerLeft + 15);
        Wrist.write(WristGrab);
        delay(400);
        Wrist.write(WristGrab + 20);
        Tower.write(TowerLeft);
        count++;
      }


      washcount++;
    }

    /// bring to dripping position
    Armdesiredpos = ArmCenter;
    pos = Arm.read();
    while (pos < Armdesiredpos) {
      pos++;
      Arm.write(pos);
      delay(delayservo);
    }

    Elbow.write(ElbowCenter);
    delay(300);
    // close clamp
    Clamp.write(ClampClosed);
    // shake it
    count = 0;
    while (count < 20) {
      Arm.write(80);
      delay(100);
      Arm.write(70);
      delay(100);
      count++;
    }
    // vibrate and drip
    pos = Wrist.read();
    while (count < 50) {
      Wrist.write(pos);
      delay(10);
      Wrist.write(pos - 5);
      delay(10);
      count++;
    }

  }
  else {
    Serial.println("");
    Serial.print("TEST MODE, NO WASH");
  }
  /// leave clamp dripping while pushing plate away
  LocatePlate();

  // get ready to go back
  Arm.write(ArmUp);
  Elbow.write(ElbowUp);

  /// now back to start position

  Towerdesiredpos = 150;  // tower back to right, but not to full extent to avoid jamming
  pos = Tower.read();
  while (pos < Towerdesiredpos) {
    pos++;
    Tower.write(pos);
    delay(delayservo);
  }
  Elbow.write(ElbowDown + 40);

  delay(500);
  Clamp.write(ClampClosed);
  Armdesiredpos = ArmCenter + 30;
  pos = Arm.read();
  while (pos > Armdesiredpos) {
    pos--;
    Arm.write(pos);
    delay(delayservo);
  }

  Wrist.write(WristGrab);
  TrashCan.write(closeTrash);

  delay(1000);
  locateTries = 0; // reset locateplate error handling value
  PowerOff();


}

//////////////PLATES ////////////////

void LocatePlate() {
  // PowerOn(); // for tests only

  Grabber.write(GrabberOpen);
  PlateBarrier.write(PlateBarrierOpen); // avoid barrier pushing plate to far in
  ReverseArm();
  int pos = ArmLift.read();
  int cal = 20;

  if (!IsLastPlate()) {
    while (1) {
      // time limit to prevent arm from destroying pretty much everything around
      long Limit = 8; // in seconds
      long TimeLimitMilliVal = Limit * seconds; // converted in millis
      unsigned long TimeLimit = (unsigned long int)TimeLimitMilliVal;

      // Calibration: get armlift down
      ArmLift.write(cal);
      delay(500);
      if (!plateVeryClose()) {
        // move arm forward until detection
        ForwardArm();
        Serial.println("");
        Serial.print("CALIBRATION FWD");

        Serial.println("TimeLimit = "); Serial.print(TimeLimit);
        // starting time
        time = millis();
        TimeLimit = (unsigned long int)time + (unsigned long int)TimeLimit;
        Serial.println(TimeLimit);

        while (!plateVeryClose() && time < TimeLimit) {
          time = millis();
        }
        if (time >= TimeLimit) {
          Serial.println("");
          if (cal < 3) {
            Serial.println("");
            Serial.print("FAILED TO CALIBRATE, trying again");
            LocatePlateAgain();
            TryAgain = false; // not that kind of failure
            cal++;
            break;
          }
          else {
            Serial.println("");
            Serial.print("FAILED TO CALIBRATE, STOPPING OPERATION");
            /// reversearm a little
            analogWrite(pwmArm, 255);
            digitalWrite(ArmMotor1, HIGH);
            digitalWrite(ArmMotor2, LOW);
            delay(2000);
            StopArm();
            platehasbeenpushed = true; // allows break for the !platehasbeenpushed argt below
          }
        }

        Serial.println("CALIBRATION DONE");
      }
      calTRY = 0;
      delay(1); // secure positive signal
      StopArm();
      Grabber.write(GrabberClosed); // grabber down to not get jammed into nearby structure (continuous servo)
      ArmLift.write(80);
      delay(500);
      pos = ArmLift.read();
      PlateBarrier.write(PlateBarrierClosed);
      delay(100);

      while (!platedetected()) {
        pos--;
        if (pos < 70) {
          Grabber.write(GrabberOpen); // back to suitable position
        }
        ArmLift.write(pos);
        Serial.println("");
        Serial.print("--"); Serial.print(pos);
        //Serial.println("");
        //Serial.print("detect value = "); Serial.print(analogRead(PlateSensorIR));
        //Serial.println("");
        //Serial.print("detect value = "); Serial.print(platedetected());

        // if pos == 0 means we're stuck onto plates
        if (pos <= 26) {
          Serial.println("");
          Serial.println("NO MORE PLATE");
          platehasbeenpushed = true; // needed to finish operations
          ArmLift.write(50);
          ReverseArm();
          // LocatePlateAgain();
          BREAK = true;
          break;
        }
        delay(100);
      }
      delay(200);

      while (!onePlateOnly() && pos < 70) {
        pos++;
        ArmLift.write(pos);
        Serial.println("");
        Serial.print("++"); Serial.print(pos);
        delay(100);
      }

      if (BREAK || pos <= 26 || pos >= 70) {
        Serial.println("");
        Serial.print("FAILED TO LOCATE -- BREAK");
        break;
      }

      platehasbeenpushed = false;
      //pos += 1;
      ArmLift.write(pos);
      int Max = 3;
      if (TryAgain) {
        Serial.println("");
        Serial.println("TRYING WITH NEW POS");
        Serial.println("");
        Serial.print("OLD POS = "); Serial.print(pos);
        Serial.println("");
        locateTries += 1;
        Serial.print("locateTries = "); Serial.print(locateTries);
        if (locateTries <= Max) {
          pos -= locateTries;
          Serial.println("");
          Serial.print("NEW POS = "); Serial.print(pos);
          ArmLift.write(pos);
          if (!IsLastPlate()) {
            if (locatetest) {
              Serial.println("");
              Serial.print("LOCATE TEST, NOT PUSHING");
              break;
            }
            else {
              PushPlateOut();
            }
          }
          else {
            Serial.println("LAST PLATE!!! NOT PUSHING");
            ReverseArm();
            break;
          }
        }
        else {
          Serial.println("FAILED TO LOCATE PLATE, STOPPING OPERATION");
          locateTries = 0;
          platehasbeenpushed = true; //needed to cancel all operations
        }

        ArmLift.write(pos);
      }
      else {
        if (locatetest) {
          Serial.println("");
          Serial.print("LOCATE TEST, NOT PUSHING");
        }
        else {
          PushPlateOut();
        }
      }

      break;
    }
  }
  else {
    Serial.println("LAST PLATE!!! NOT PUSHING");
    platehasbeenpushed = true;
  }

  Serial.println("WAITING FOR PLATE TO BE PUSHED AWAY");
  while (!platehasbeenpushed && !locatetest) {
    delay(1);
  }
  Serial.println("PLATE PUSHED AWAY. END OF PLATE PUSH");
  PlateBarrier.write(PlateBarrierClosed);

  // reset error handling values
  totalfailure = false;
  failed = 0;
  TryAgain = false;
  ignore = false;
  BREAK = false;
}

void LocatePlateAgain() {
  Serial.println("");
  Serial.print("Starting OVER");
  TryAgain = true;

  LocatePlate();
}

void PushPlateOut() {
  PlateBarrier.write(PlateBarrierClosed);
  Grabber.write(GrabberOpen);
  platehasbeenpushed = false;

  plateseenPassing = false;

  ForwardArm();

  count = 0;
  long Limit = 15; // in seconds
  long TimeLimitMilliVal = Limit * seconds; // converted in millis
  unsigned long TimeLimit = (unsigned long int)TimeLimitMilliVal;
  time = millis(); // starting time
  unsigned long start = time;
  TimeLimit = (unsigned long int)time + (unsigned long int)TimeLimit;

  // secure grab
  while (time < TimeLimit) {
    time = millis();
    unsigned long elapsed = (unsigned long int)time - (unsigned long int)start ;
    unsigned countdown = (((unsigned long int)TimeLimit - (unsigned long int)elapsed) - (unsigned long int)time) / seconds;
    Serial.println("");
    Serial.println("WAITING "); Serial.print(countdown); Serial.print(" SECONDS");
    delay(1000);
  }

  while (!PlateIsOnTheWay()) {

    if (!plategrab()) {
      Serial.println("");
      Serial.print("BARRIER PUSH BACK 2");
      PlateBarrier.write(PlateBarrierOpen);
      delay(500);
      PlateBarrier.write(PlateBarrierClosed); /// try to push back the plate
      delay(2000);

      if (!plategrab() && !PlateIsOnTheWay()) {
        // give it another chance for x seconds
        Limit = 3; // in seconds
        TimeLimitMilliVal = Limit * seconds; // converted in millis
        TimeLimit = (unsigned long int)TimeLimitMilliVal;
        time = millis(); // starting time
        TimeLimit = (unsigned long int)time + (unsigned long int)TimeLimit;
        while (!plategrab() && time < TimeLimit) {
          time = millis();
        }
        PlateBarrier.write(PlateBarrierOpen);
        if (!plategrab() && !PlateIsOnTheWay()) {
          delay(2000);
          Serial.println("FAILURE, STARTING OVER (1)");
          TryAgain = true;
          restartPlate();
          break;
        }
      }
    }

    if (!PlateIsOnTheWay()) {
      Serial.println("");
      Serial.print("GRAB OK");
      Grabber.write(GrabberClosed);
      PlateBarrier.write(PlateBarrierOpen);
      Serial.println("");
      Serial.println("NOW PUSHING");
    }
    else {
      Serial.println("");
      Serial.print("PLATE SEEN -----------------------------");
      break;
    }

    int pos = ArmLift.read();
    int desiredpos = pos + 3;
    count = 0;
    Limit = 45; // in seconds
    TimeLimitMilliVal = Limit * seconds; // converted in millis
    TimeLimit = (unsigned long int)TimeLimitMilliVal;
    time = millis(); // starting time
    TimeLimit = (unsigned long int)time + (unsigned long int)TimeLimit;

    Serial.println("");
    Serial.print("Waiting for "); Serial.print(Limit);
    Serial.print(" seconds (max) until plate passes over sensor");

    ForwardPlateExtractor();

    while (!PlateIsOnTheWay()  && time < TimeLimit) {
      // sensor condition must be met above all
      time = millis(); //wait and push for at least x seconds so plate can get to sensor
      // unless plate has been seen passing

      if (!plategrab() && !PlateIsOnTheWay()) {

        Serial.println("");
        Serial.print("PLATE LOST ?");
        delay(3000);
        // wait to double check (chaotic IR values)

        if (plategrab() && !PlateIsOnTheWay()) {
          Serial.println("");
          Serial.print("GRAB OK");

        }
        else {
          if (!plategrab() && !PlateIsOnTheWay()) {
            Serial.println("FAILURE, STARTING OVER (1)");
            TryAgain = true;
            restartPlate();
            break;
          }
        }
        delay(10);
      }

      if (IsLastPlate()) {
        /// push forward and see if it lasts
        count = 0;
        while (IsLastPlate() && count < 5 && !PlateIsOnTheWay()) {
          Serial.println("");
          Serial.print("LAST PLATE OR TOO MANY PLATES GRABBED?");
          delay(600);
          count++;
        }
        // double check and confirm
        if (IsLastPlate() && !PlateIsOnTheWay()) {
          Serial.println("");
          Serial.print("GRABBED TOO MANY PLATE OR LAST PLATE!!! STOPPING OPERATION");
          NeedGrabber = true;
          BREAK = true;
          break;
        }
      }

      if (pos < desiredpos) {
        pos++;
        ArmLift.write(pos);
      }
      if (time > TimeLimit + (TimeLimit / 2)) {
        Serial.println("");
        Serial.println("TOO LONG!");
        break;
      }

      delay(5);
    }
    if (BREAK) {
      Serial.println("");
      Serial.print("MAIN BREAK");
      break;
    }
  }

  //Serial.println("");
  Serial.println("PUSHING 25 MORE SECONDS TO SECURE EXTRACTION");
  delay(25000);
  /// needed time to push plate out onto roller

  ForwardPlateExtractor();

  platehasbeenpushed = true;

  TryAgain = false;


  ReverseArm();
  Grabber.write(GrabberOpen);
  StopPlateExtractor();

}


void TEST() {

  ///// DO NOT DELETE////
  if (digitalRead(PWSW) == 1) {
    PowerOn();
  }
  /////

  count = 0;



  locatetest = true;
  LocatePlate();

  /*
     while (count < 10) {
    ForwardExtractor();
    count++;
    }
    //retract
    analogWrite(pwmCanExtractor, 170);
    digitalWrite(CanExtractorMotor1, LOW);
    digitalWrite(CanExtractorMotor2, HIGH);
    delay(2000);
    //stop
    analogWrite(pwmCanExtractor, 0);
    digitalWrite(CanExtractorMotor1, LOW);
    digitalWrite(CanExtractorMotor2, LOW);


    count = 0;

    countSecureCatch = 5;
    catchwithclamp();
  */


  Serial.println("");
  Serial.print("Elapsed Time = "); Serial.print(time);
  Serial.println("");
  Serial.print("TEST LOOP DONE");
}

void restartPlate() {
  ReversePlateExtractor();
  ReverseArm();
  LocatePlate();
}

void ReverseArm() {

  if (!NeedGrabber) {
    Grabber.write(GrabberOpen); // proper position for retracted arm
  }
  if (powerIsOn) {
    count = 0;
    ArmLift.write(ArmLiftUp);
    analogWrite(pwmArm, 255);
    digitalWrite(ArmMotor1, HIGH);
    digitalWrite(ArmMotor2, LOW);
    Serial.println("RETRACTING LINEAR ARM");

    long Limit = 65; // in seconds
    long TimeLimitMilliVal = Limit * seconds; // converted in millis
    unsigned long TimeLimit = (unsigned long int)TimeLimitMilliVal;
    Serial.println("");
    Serial.print("TimeLimit = "); Serial.print(TimeLimit);

    time = millis(); // starting time
    TimeLimit = (unsigned long int)time + (unsigned long int)TimeLimit;
    Serial.println(TimeLimit);

    while (analogRead(IRArmSensor) >= 500 && time < TimeLimit) {
      //Serial.println(analogRead(IRArmSensor));
      time = millis();
      // if sensor is faulty and loop doesn't break,
      //loop will break as of max needed time to fully retract the arm is reached (50seconds)
    }
    delay(100); /// sensor's mess
    time = millis();
    if (time > TimeLimit) {
      Serial.println("PROBLEM WITH ARM LIFT SENSOR");

    }

    digitalWrite(ArmMotor1, LOW);
    digitalWrite(ArmMotor2, LOW);
    Serial.println("END OF REVERSE");

  }
  else {
    Serial.println("POWER IS OFF, breaking reverse loop, permanent reverse");
    analogWrite(pwmArm, 255);
    digitalWrite(ArmMotor1, HIGH);
    digitalWrite(ArmMotor2, LOW);
  }
  NeedGrabber = false;
  StopPlateExtractor();
}

void  restartReverseArm() {
  ReverseArm();
}

void ForwardExtractor() {
  String yesno = "NO";
  double Limit = 1.5; // in seconds
  long TimeLimitMilliVal = Limit * seconds; // converted in millis
  unsigned long TimeLimit = (unsigned long int)TimeLimitMilliVal;
  Serial.println("");
  Serial.print("TimeLimit = "); Serial.print(TimeLimit);

  analogWrite(pwmCanExtractor, 150);
  delay(1);
  if (ExtractorExtended()) {
    yesno = "yes";
  }
  Serial.println("");
  Serial.print("EXTENDED? : "); Serial.print(yesno);

  // starting time
  time = millis();
  TimeLimit = (unsigned long int)time + (unsigned long int)TimeLimit;

  // run
  digitalWrite(CanExtractorMotor1, HIGH);
  digitalWrite(CanExtractorMotor2, LOW);
  if (!extracted) {
    extracted = true;
    while (!ExtractorExtended() && time < TimeLimit) {
      time = millis();

    }
  }
  else {
    Serial.println("");
    Serial.print("ALREADY EXTRACTED");
    ReverseExtractor();
    delay(3000);
    if (ExtractorRetracted()) {
      ForwardExtractor();
    }
    else {
      Serial.println("");
      Serial.print("EXTRACTOR STUCK!!");
    }
  }

  StopExtractor();
  if (ExtractorExtended()) {
    yesno = "yes";
  }
  Serial.println("");
  Serial.print("EXTENDED? : "); Serial.print(yesno);

  extracted = false; // so manual test can be done at second hit
  ForwardHasRun = true;
  ReverseExtractor();

}

void ReverseExtractor() {
  String yesno = "NO";
  long Limit = 6; // in seconds // this sensor is in a more reliable setup
  long TimeLimitMilliVal = Limit * seconds; // converted in millis
  unsigned long TimeLimit = (unsigned long int)TimeLimitMilliVal;
  Serial.println("");
  Serial.print("TimeLimit = "); Serial.print(TimeLimit);

  time = millis(); // starting time
  TimeLimit = (unsigned long int)time + (unsigned long int)TimeLimit;
  Serial.println(TimeLimit);



  delay(1);
  Serial.println("");
  if (ExtractorRetracted()) {
    yesno = "yes";
  }


  Serial.print("RETRACTED? : "); Serial.print(yesno);
  // run
  digitalWrite(CanExtractorMotor1, LOW);
  digitalWrite(CanExtractorMotor2, HIGH);

  int SpeedValue = 255;
  if (!ForwardHasRun) {
    // startup speed must be slow... fast only when forward has already been run
    SpeedValue = 40;
  }
  while (!ExtractorRetracted() &&  time < TimeLimit) {
    analogWrite(pwmCanExtractor, SpeedValue);
    //Serial.println("");
    //Serial.print("RETRACT");
    time = millis();
    SpeedValue -= 10;
    if (SpeedValue <= 40) {
      SpeedValue = 40;
    }

    delay(5);
  }

  StopExtractor();
  if (ExtractorRetracted()) {
    yesno = "yes";
  }
  Serial.println("");
  Serial.print("RETRACTED? : "); Serial.print(yesno);
  extracted = false;
}

void StopExtractor() {

  digitalWrite(CanExtractorMotor1, LOW);
  digitalWrite(CanExtractorMotor2, LOW);
  analogWrite(pwmCanExtractor, 0);
}

void ForwardArm() {
  analogWrite(pwmArm, 255);
  digitalWrite(ArmMotor1, LOW);
  digitalWrite(ArmMotor2, HIGH);
  Serial.println("");
  Serial.print("ARM FORWARD");
}

void StopArm() {
  analogWrite(pwmArm, 0);
  digitalWrite(ArmMotor1, LOW);
  digitalWrite(ArmMotor2, LOW);
  Serial.println("ARM STOPPED");
}

void ForwardPlateExtractor() {
  analogWrite(pwmPEXT, PEXTSpeed);
  digitalWrite(PlateExtractorMotor1, LOW);
  digitalWrite(PlateExtractorMotor2, HIGH);
}

void ReversePlateExtractor() {
  analogWrite(pwmPEXT, PEXTSpeed);
  digitalWrite(PlateExtractorMotor1, HIGH);
  digitalWrite(PlateExtractorMotor2, LOW);
}

void StopPlateExtractor() {
  analogWrite(pwmPEXT, 0);
  digitalWrite(PlateExtractorMotor1, LOW);
  digitalWrite(PlateExtractorMotor2, LOW);
}

//////////////////////// BOOLEANS

boolean ExtractorExtended() {

  bool result = false;
  String yesno = "";
  Serial.println("");
  Serial.print("Extended Senor Val = "); Serial.print(analogRead(CanExtractorSensorOUT));
  if (analogRead(CanExtractorSensorOUT) > 400) {
    result = true;
  }
  else {
    result = false;
  }

  return result;
}
boolean ExtractorRetracted() {

  bool result = false;
  String yesno = "";
  Serial.println("");
  Serial.print("Extracted Senor Val = "); Serial.print(analogRead(CanExtractorSensorIN));
  if (analogRead(CanExtractorSensorIN) == 1023) {
    result = true;
  }
  else {
    result = false;
  }

  return result;
}

boolean platedetected() {

  bool result = false;
  String yesno = "";
  Serial.println("");
  int val = analogRead(PlateSensorIR);
  Serial.print("Plate Detect Val IR = "); Serial.print(val);
  if (val > 400) {
    result = true;
    yesno = "YES";
  }
  else {
    result = false;
    yesno = "NO";
  }


  //Serial.println("");
  //.print("Plate Sensor IR val: "); Serial.print(val);
  // Serial.println("");
  //Serial.print("Plate Detected? "); Serial.print(yesno);

  return result;
}

boolean plateVeryClose() {


  bool result = false;
  String yesno = "";
  Serial.println("");
  int val = analogRead(PlateGrabSensor);
  Serial.print("Plate Detect Val VERY CLOSE IR = "); Serial.print(val);
  if (val > 500 && val < 550) {
    result = true;
    yesno = "YES";
  }
  else {
    result = false;
    yesno = "NO";
  }


  //Serial.println("");
  //.print("Plate Sensor IR val: "); Serial.print(val);
  // Serial.println("");
  //Serial.print("Plate Detected? "); Serial.print(yesno);

  return result;
}

boolean onePlateOnly() {

  bool result = false;
  String yesno = "";
  Serial.println("");
  int val = analogRead(PlateGrabSensor);
  Serial.print("Plate Detect Val ONE PLATE ONLY IR = "); Serial.print(val);
  if (val < 350 ) {
    result = true;
    yesno = "YES";
  }
  else {
    result = false;
    yesno = "NO";
  }


  //Serial.println("");
  //.print("Plate Sensor IR val: "); Serial.print(val);
  // Serial.println("");
  //Serial.print("Plate Detected? "); Serial.print(yesno);

  return result;
}



boolean IsLastPlate() {

  int val = analogRead(LastPlate);

  bool result = false;
  String yesno = "";
  if (val < 400) {
    result = true;
    yesno = "YES";
  }
  else {
    result = false;
    yesno = "NO";
  }
  /*
    Serial.println("");
    Serial.print("LAST Plate Sensor IR val: "); Serial.print(val);
    Serial.println("");
    Serial.print("LAST PLATE? "); Serial.print(yesno);

  */
  return result;
}

boolean plategrab() {

  bool result = false;
  bool falseread = false;
  bool trueread = false;
  String yesno = "NO";
  int val1 = analogRead(PlateGrabSensor);
  //int val2 = digitalRead(PlateSensor) ;

  if (val1 > 300 && val1 < 700) {  // || val2 == LOW) {
    result = true;

    if (!trueread) {
      //Serial.println("");
      //Serial.println("GRAB OK "); Serial.print(val1);
      falseread = false;
      trueread = true;// so it prints out only when changed
    }
  }
  else {
    if (!falseread) {
      // Serial.println("");
      //Serial.println("GRAB LOST -- IR: "); Serial.print(val1);
      falseread = true;
      trueread = false;
    }
    //delay(400);
  }


  return result;
}

boolean PlateIsOnTheWay() {
  // plateseenPassing is a global boolean
  // it matters so this loop's bool value isn't reset at each request

  String yesno = "";
  int val0 = analogRead(PlateOnTheWay);
  delay(300);
  // check value's consistency
  int val1 = analogRead(PlateOnTheWay);
  delay(300);
  // check value's consistency
  int val2 = analogRead(PlateOnTheWay);
  delay(300);
  // check value's consistency
  int val3 = analogRead(PlateOnTheWay);
  float val = (val0 + val1 + val2 + val3) / 4;


  if (val > 700) {
    plateseenPassing = true;// will preserve this value as soon as event happened once
    yesno = "YES";

    Serial.println("");
    Serial.println("-------------- PLATE SEEN!! -VAL = "); Serial.print(val);

  }
  else if (!plateseenPassing) {
    plateseenPassing = false;
    yesno = "NO";
  }

  // Serial.println("");
  // Serial.print("IR VAL = "); Serial.print(val);

  return plateseenPassing;
}

boolean candetected() {

  bool result;

  if (analogRead(CanGrabbDetect2) < 500) {
    result = true;
    Serial.println("");
    Serial.println("CAN WITHIN GRASP!");

    // Clamp.write(ClampClosed);
  }
  else {
    result = false;
    Serial.println("");
    Serial.print("NO GRASP. : "); Serial.print(analogRead(CanGrabbDetect2));

    //Clamp.write(ClampOpen);
  }
  return result;
}


boolean CanIsOnTrack() {
  bool result;

  if (analogRead(CanOnTrackSensor) < 500) {
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



boolean CanPresence() {
  bool result;
  String yesno = "";
  Serial.println("");
  Serial.println("");
  Serial.print("CanPresenceSensor1 Val = "); Serial.print(analogRead(CanPresenceSensor1));
  Serial.println("");

  //analogRead(CanPresenceSensor1) < 700 &&

  if (analogRead(CanPresenceSensor1) > 500) {
    //|| analogRead(CanPresenceSensor2) < 500) {
    result = true;
    yesno = "YES";

  }
  else {
    result = false;
    yesno = "NO";

  }
  Serial.println("");
  Serial.print("CanPresence: "); Serial.print(yesno);
  delay(100);
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

void PowerOn() {

  blinks = 2;
  lapse = 200;
  Blink();
  digitalWrite(PWSW, 0);
  delay(500);
  ReverseExtractor();

  Arm.write(ArmUp);
  powerIsOn = true;
  delay(500);

  ReverseArm(); // arm needs to be reversed back or might be caught into clamp
  Clamp.write(ClampClosed);
  Elbow.write(ElbowUp);
  if (Tower.read() < TowerCenter) {
    while (Tower.read() < 0) {
      // if passed the trash can go to 0
      Tower.write(Tower.read() - 1);
      delay(5);
    }
  }
  else {
    //otherwise go to center
    while (Tower.read() > TowerCenter) {
      Tower.write(Tower.read() - 1);
      delay(5);
    }
  }
  while (Arm.read() > ArmDown) {
    Arm.write(Arm.read() - 1);
    delay(20);
  }

  Wrist.write(90);
  TrashCan.write(openTrash);
  delay(1000);
  Elbow.write(ElbowUp);


  while (Tower.read() > 0) {
    Tower.write(Tower.read() - 1);
    Clamp.write(ClampClosed);
    delay(10);
    Clamp.write(ClampClosed - 30);
    delay(10);
  }

  delay(200);
  int pos = Arm.read();
  while (pos < ArmUp) {
    pos++ ;
    Arm.write(pos);
    Clamp.write(ClampClosed);
    delay(10);
    Clamp.write(ClampClosed - 30);
    delay(10);
  }
  Clamp.write(ClampClosed);
  delay(500);
  Tower.write(TowerCenter);
  delay(1500);
  Wrist.write(WristGrab);
  Clamp.write(ClampClosed - 30);
  Elbow.write(ElbowCenter);
  Arm.write(ArmCenter);

  ReverseExtractor();
  delay(500);
  TrashCan.write(closeTrash);
}

void PowerOff() {

  digitalWrite(PWSW, 1);
  powerIsOn = false;
  blinks = 3;
  lapse = 200;
  Blink();
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


  //return 500 + len * 200 / 18;
  int microValue = map(len, 0, 180, 500, 2500);
  Serial.println("");
  Serial.print(DesiredPosDegrees); Serial.print("converted to"); Serial.print(microValue);
  Serial.println("");
  return microValue;

}

void RunServoMS() {

  ServoIsDone = false;
  delay(20);
  int  TrashCanRead = TrashCan.read();
  Serial.println("");
  Serial.print("TrashCanRead = "); Serial.print(TrashCanRead);
  int TrashCanPos = degree2ms(TrashCanRead);
  Serial.println("");
  Serial.print("TrashCanPos = "); Serial.print(TrashCanPos);
  Serial.println("");
  Serial.print("DesiredPosDegrees BEFORE CONVERSION = "); Serial.print(DesiredPosDegrees);
  DesiredPos = degree2ms(DesiredPosDegrees); // convert DesiredPosDegrees from degrees to microseconds (0-270 / 500-2500)
  Serial.println("");
  Serial.print("DesiredPos AFTER CONVERSION = "); Serial.print(DesiredPos);
  delay(10);

  while (TrashCanPos > DesiredPos) {
    Serial.println("");
    Serial.print("TrashCanRead= "); Serial.print(TrashCan.read());
    Serial.println("");
    Serial.print("TrashCanPos = "); Serial.print(TrashCanPos);
    TrashCan.writeMicroseconds(TrashCanPos);
    delayMicroseconds(2500);
    delay(10);
    TrashCanPos -= 100;
    if (TrashCanPos <= 500) {
      TrashCanPos = 500;
      TrashCan.writeMicroseconds(TrashCanPos);
      break;
    }
  }

  delay(1000);
  while (TrashCanPos < DesiredPos) {
    Serial.println("");
    Serial.print("TrashCanRead= "); Serial.print(TrashCan.read());
    Serial.println("");
    Serial.print("TrashCanPos = "); Serial.print(TrashCanPos);
    TrashCan.writeMicroseconds(TrashCanPos);
    delayMicroseconds(2500);
    delay(10);
    TrashCanPos += 100;
    if (TrashCanPos >= 2500) {
      TrashCanPos = 2500;
      TrashCan.writeMicroseconds(TrashCanPos);
      break;
    }
  }

  ServoIsDone = true;
}


/// LOGS

void logs() {
  count = 0;

  ///// DO NOT DELETE////
  if (digitalRead(PWSW) == 1) {
    PowerOn();
  }
  /////


  while (count < 200) {
    onePlateOnly();
    count++;
    delay(10);
  }



}
