
//******************************************************************************************
//Arduino Setup() routine
//******************************************************************************************

void(* resetFunc) (void) = 0;//declare reset function at address 0

void setup()
{
  /*****************************************************************************
    configure reset pin
   *****************************************************************************/
  digitalWrite(rst, HIGH); // set it to high before setting its mode, otherwise it'll trigger reset
  pinMode(rst, OUTPUT);


  Serial.begin(115200);

  /*****************************************************************************
    Attach Servos
  *****************************************************************************/

  gate.attach(22);
  uh.attach(26);
  lp.attach(28);

  lp.write(pullIn);
  pushupperholder();
  
  
  c2.attach(30);
  c1.attach(32);
  lidRemover.attach(47);

  delay(1);
  lidRemover.write(180);
  c2.write(0);
  c1.write(1);


  canopArm.attach(24);
  delay(1);
  canOpUp();
  opengate();

  

  canReceiver.attach(49);
  delay(1);
  canReceiver.write(receiverflat);


  /*****************************************************************************
    Intialize Pins
  *****************************************************************************/

  pinMode (LED, OUTPUT);
  pinMode(power, OUTPUT);
  pinMode(pwCoordination, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(LA1, OUTPUT);
  pinMode(LA2, OUTPUT);
  decompressCan();
  pinMode(chamber1echo, INPUT);
  pinMode(chamber1trig, OUTPUT);
  


  delay(1);
  stopOpener();
  pinMode(pressed, INPUT_PULLUP);


  //*****************************************************************************
  //Initialize SmarThings
  //*****************************************************************************

  smartthing.init();


  Blink(3, 30);
  
  _server.begin();
  digitalWrite(power, 1);
  pwstate = "off";
  Refresh();
}
