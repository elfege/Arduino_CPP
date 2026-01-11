
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

String currentLine = "";   // make a String to hold incoming data from the client

int const ArmMotor1 = 6 ;
int const ArmMotor2 = 7;
int const PlateSensor = A4;
int const PlateGrabbedSensor = A5;
int const USArmSenor = 14;
int const IRArmSensor = A7;

int device;
int DesiredPos;
int DesiredPosDegrees;
int deviceport;


int increment = 1;

int extractorStop = 90;
int extractorRun = 180;
int extractorReverse = 0;
int PlateBarrierClosed = 30;
int PlateBarrierOpen = 180;

int ArmLiftDown = 0;
int ArmLiftCenter = 30;
int ArmLiftUp = 60;


Servo PlateExtractor;
Servo PlateBarrier;

Servo ArmLift;

//int const ArmLift = 5;


void setup() {
  pinMode (ArmMotor1, OUTPUT);
  pinMode (ArmMotor2, OUTPUT);
  pinMode (PlateSensor, INPUT);
  pinMode (IRArmSensor, INPUT);

  delay(50);

  delay( 500 );   // allow some time (500 ms) after powerup and sketch start,
  Serial.begin(9600);
  delay(100);

  Serial.println("Initializing Ethernet Protocol");
  Ethernet.begin(mac, ip);
  Serial.println("Starting Ethernet Server");
  server.begin();
  Serial.println(Ethernet.localIP());

  PlateExtractor.attach(4);
  PlateBarrier.attach(3);
  ArmLift.attach(5);
  delay(50);


  ArmLift.write(ArmLiftUp);

  PlateBarrier.write(PlateBarrierOpen);

  PlateExtractor.write(extractorRun);
  delay(2000);
  PlateExtractor.write(extractorReverse);
  PlateBarrier.write(PlateBarrierClosed);
  delay(2000);
  PlateExtractor.write(90);


  ReverseArm();
  //LocatePlate();


}

void loop() {
  Serial.println("");
  Serial.print("IRAmSensor = "); Serial.print(analogRead(IRArmSensor));
  // Serial.print("PlateGrabbedSensor (LOOP) : "); Serial.print(analogRead(PlateGrabbedSensor));
  //Serial.println(analogRead(IRArmSensor));


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

          client.print("HTTP/1.1 200 OKOK\r\nContent-Type: text/html\r\n\r\n");
          client.print(   "<HTML>");
          client.print(   "<BODY style='background-color:grey'>");
          client.print(    "<font style='color:black'>");
          client.print(    "<font style='font-family:electric toaster'>");
          client.print(     "<strong>");

          client.print("Current Increment Value = ");
          client.print(increment);
          client.println(" ArmLift Position = ");
          client.print(ArmLift.read());
          client.println(" PlateSensor Value = ");
          client.print(analogRead(PlateSensor));


          client.print(F(

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
                         "<input type=submit value='OPN PLATE BARRIER' style=width:250;height:30px onClick=location.href='/openplatebarrier'>"
                         "<br />"
                         "<input type=submit value='SPIN TEST' style=width:120px;height:30px onClick=location.href='/spintest'>"

                         "<br />"
                         "<br />"
                         "<input type=submit value='ARM DOWN ' style=width:120px;height:30px onClick=location.href='/armLiftdown'>"
                         "<input type=submit value='ARM CENTER ' style=width:120px;height:30px onClick=location.href='/armLiftcenter'> "
                         "<input type=submit value='ARM UP ' style=width:120px;height:30px onClick=location.href='/armLiftup'> "

                         "<br />"
                         "<input type=submit value='ARM + ' style=width:120px;height:30px onClick=location.href='/armliftplus'>"
                         "<input type=submit value='ARM - ' style=width:120px;height:30px onClick=location.href='/armliftminus'> "
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


          delay(1);
          client.stop();
          Serial.println("client disonnected");

          // Check request:
          Serial.println("READING COMMAND: "); Serial.print(currentLine);
          if (currentLine.startsWith("GET /pushplate")) {
            LocatePlate();
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
          else if (currentLine.startsWith("GET /spintest")) {

            PlateExtractor.write(extractorRun);

            int count = 0;
            while (count < 10) {
              Serial.println("SPIN TEST");
              delay(1000);
              count++;
            }
            PlateExtractor.write(90);

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



          delay(1);
          //clearing string for next read
          Serial.println("String CLEARED");
          currentLine = "";

        }

      }
    }
  }

}

void LocatePlate() {
  PlateBarrier.write(PlateBarrierClosed);
  ReverseArm();
  delay(1000);
  StopArm();
  int start = 1;
  DesiredPos = start;

  ArmLift.write(start);
  int pos = 0;
  delay(500);


  while (analogRead(PlateSensor) < 200) {
    pos++;

    DesiredPos = pos;

    ArmLift.write(pos);
    Serial.println("");
    Serial.print("Sensor Value @++ : "); Serial.print(analogRead(PlateSensor));
    delay(100);

  }
  while (analogRead(PlateSensor) > 200) {
    pos--;

    DesiredPos = pos;

    ArmLift.write(pos);
    Serial.println("");
    Serial.print("Sensor Value @-- : "); Serial.print(analogRead(PlateSensor));
    delay(1000);

  }

  Serial.println("");
  Serial.print("Sensor Value @-- : "); Serial.print(analogRead(PlateSensor));

  delay(400);
  PushPlateOut();

}

void PushPlateOut() {
  while (1) {
    PlateExtractor.write(extractorReverse); // used as holder
    int pos = ArmLift.read();
    digitalWrite(ArmMotor1, LOW);
    digitalWrite(ArmMotor2, HIGH);

    PlateBarrier.write(PlateBarrierClosed);
    int grabcount = 0;

    while (analogRead(PlateGrabbedSensor) > 200 && grabcount < 16000) {
      grabcount++;
      delay(1);
    }
    Serial.println("");
    Serial.print("grabcount : "); Serial.print(grabcount);
    Serial.print("PlateGrabbedSensor Value @++ : "); Serial.print(analogRead(PlateGrabbedSensor));

    if (analogRead(PlateGrabbedSensor) < 200) {
      Serial.println("PLATE GRABBED!!");
    }
    else {
      Serial.println("FAILED!!!!! ");
      restartPlate();
      Serial.println("BREAK");
      break;
    }

    delay(3000);
    PlateBarrier.write(PlateBarrierOpen);
    PlateExtractor.write(extractorRun);
    delay(1000);
    pos = pos + 10;
    DesiredPos = pos;
    ArmLift.write(pos);
    delay(5000);
    pos = pos + 5;
    DesiredPos = pos;
    ArmLift.write(pos);
    while (analogRead(PlateGrabbedSensor) < 200 ||  analogRead(PlateSensor) < 200) {
      delay(1);
    }
    Serial.println("PLATE LOST?");
    PlateBarrier.write(PlateBarrierClosed); /// re-push back the plate to make sure
    delay(1000);
    PlateBarrier.write(PlateBarrierClosed); /// retract

    delay(5000);
    if (analogRead(PlateGrabbedSensor) < 200 ||  analogRead(PlateSensor) < 200) {
      Serial.println("PLATE STILL GRABBED!!!");
      while (analogRead(PlateGrabbedSensor) < 200 ||  analogRead(PlateSensor) < 200) {
        delay(1);
      }
    }
    setup();
    break;
  }
}

void restartPlate() {
  ReverseArm();
  delay(10000);
  LocatePlate();
}

void ReverseArm() {
  digitalWrite(ArmMotor1, HIGH);
  digitalWrite(ArmMotor2, LOW);
  while (analogRead(IRArmSensor) < 700) {
    Serial.println("");
    Serial.print("IRAmSensor = "); Serial.print(analogRead(IRArmSensor));
    Serial.println("Reversing");
    delay(100);
  }
  Serial.println("");
  Serial.print("IRAmSensor = "); Serial.print(analogRead(IRArmSensor));
  Serial.println("reverse DONE!");
  digitalWrite(ArmMotor1, LOW);
  digitalWrite(ArmMotor2, LOW);
}
void ForwardArm() {
  digitalWrite(ArmMotor1, LOW);
  digitalWrite(ArmMotor2, HIGH);
}

void StopArm() {
  digitalWrite(ArmMotor1, LOW);
  digitalWrite(ArmMotor2, LOW);
}


/////////////////////270 degrees servo mgnt///////////////////////////

////////// CONVERSION FUNCTION //////////////////
int degree2ms(unsigned int val)
{

  int microValue = map(val, 0, 180, 500, 2500);
  //return 500 + len * 200 / 18;
  return microValue;
}

void RunServoMS() {

  delay(20);
  int deviceRead = ArmLift.read();
  Serial.println("");
  Serial.print(" BEFORE deviceRead = "); Serial.print(deviceRead);
  int devicePos = degree2ms(deviceRead);
  Serial.println("");
  Serial.print(" AFTER devicePos = "); Serial.print(devicePos);
  Serial.println("");
  Serial.println("");
  delay(500);
  Serial.print(" BEFORE DesiredPosDegrees = "); Serial.print(DesiredPosDegrees);
  DesiredPos = degree2ms(DesiredPosDegrees);// convert DesiredPosDegrees from degrees to microseconds (0-270 / 500-2500)
  Serial.println("");
  Serial.print(" AFTER DesiredPos = "); Serial.print(DesiredPos);

  Serial.println("");
  Serial.print(" devicePos = "); Serial.print(devicePos);
  delay(10);

  if (devicePos > DesiredPos) {
    while (devicePos > DesiredPos) {


      //delayMicroseconds(2500);
      delay(50);
      ArmLift.writeMicroseconds(devicePos);
      devicePos -= 100;
      if (devicePos < 500) {
        devicePos = 500;
        Serial.println("BREAK");
        ArmLift.writeMicroseconds(devicePos);
        break;
      }
    }
    Serial.println("");
    Serial.print("Servo's read = "); Serial.print(ArmLift.read());
    Serial.println("");
    Serial.print("devicePos = "); Serial.print(devicePos);
  }
  else if (devicePos < DesiredPos) {

    while (devicePos < DesiredPos) {
      //delayMicroseconds(2500);
      delay(50);
      ArmLift.writeMicroseconds(devicePos);
      devicePos += 100;
      if (devicePos > 2500) {
        devicePos = 2500;
        Serial.println("BREAK");
        ArmLift.writeMicroseconds(devicePos);
        break;
      }
    }
    Serial.println("");
    Serial.print("Servo's read = "); Serial.print(ArmLift.read());
    Serial.println("");
    Serial.print("devicePos = "); Serial.print(devicePos);

  }

}
void PingUSArmSensor() {
  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode (USArmSenor, OUTPUT);
  digitalWrite(USArmSenor, LOW);
  delayMicroseconds(2);
  digitalWrite(USArmSenor, HIGH);
  delayMicroseconds(5);
  digitalWrite(USArmSenor, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(USArmSenor, INPUT);
  duration = pulseIn(USArmSenor, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  delay(100);

}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}







