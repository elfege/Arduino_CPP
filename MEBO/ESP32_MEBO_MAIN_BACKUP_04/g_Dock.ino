void dock() {
  _server.send(200, "text/html", "Docking");
  docking = true;
  runIRreceiver = true;
  check = true;
  Signal = false;
  found = false;
  GotStuck = false;
  noblink = true;
  boolean wentleft = false;
  boolean wentright = false;
  boolean gotsignal = false;
  unsigned long Startrightscan;
  unsigned long StartleftScan;

  gotsignal = false;

  int sPeeD = 180;

  int del1 = 50;
  int del2 = 50;

  // move until you get an IR signal
  unsigned long StartSearch = millis();
  // move slowly backward to get the specific signal
  StartSearch = millis();
  int rdmdel;// random turn delay
  while (!found && !STOPALL && millis() - StartSearch < 120000)
  {
    gotsignal = false;
    int clrec = 0; // encoders temporary recorders
    int crrec = 0;
    int countStuck = 0;

    while (!gotsignal && !STOPALL && !GotStuck) {
      // scan until you got a signal

      STOP(); // resets cr and cl
      crrec = cr;
      sPeeD = 180;
      left(sPeeD);// go left up to X crrec value
      makeEncHappen = true; // allow encoders to count despite the fact that this is a turn command
      countStuck = 0;
      while (!gotsignal && !STOPALL && crrec < clrec + 60) {
        Serial.print("clrec + 60 = "); Serial.println(clrec + 60);
        crrec = cr;
        if (GotStuck) {
          GotStuck = false;
          sPeeD += 10;
          left(sPeeD);
          countStuck++;
          if (countStuck > 2) {
            break;
          }
        }
        Serial.print("crrec = "); Serial.println(crrec);
        loop();
        if (Signal) {
          STOP();
          Serial.println("SIGNAL!");
          gotsignal = true;
        }
      }
      STOP(); // resets cr and cl
      clrec = cl;
      // return to initial position + 60 clrecec
      sPeeD = 180;
      right(sPeeD); // go left up to X clrec value
      makeEncHappen = true;
      countStuck = 0;
      while (!gotsignal && !STOPALL && clrec < crrec + 60) {
        Serial.print("crrec + 60 = "); Serial.println(crrec + 60);
        clrec = cl;
        if (GotStuck) {
          GotStuck = false;
          sPeeD += 10;
          left(sPeeD);
          countStuck++;
          if (countStuck > 2) {
            break;
          }
        }
        Serial.print("clrec = "); Serial.println(clrec);
        loop();
        if (Signal) {
          STOP();
          Serial.println("SIGNAL!");
          gotsignal = true;
        }
      }
      STOP(); // resets cr and cl
      // continue right up to X cr value

      loop();
    }

    if (gotsignal) {
      loop();
      while (!found && !STOPALL && !GotStuck &&  RRIRVal() < 600 && RLIRVal() < 600)
      {
        loop();
        rvs(180, 0);
        encodersOk = false; // don't correct course
        ledcWrite(pwmChannelR, 150);
        unsigned long s = millis();
        while (!found && millis() - s < 500)
        {
          loop();
        }
        rvs(180, 0);
        encodersOk = false; // don't correct course
        ledcWrite(pwmChannelL, 150);
        s = millis();
        while (!found && millis() - s < 500)
        {
          loop();
        }
      }
      if (GotStuck || RRIRVal() > 600 || RLIRVal() > 600) {
        GotStuck = false;
        fwd(200, 10);
      }

      STOP();
    }
    loop();
  }
  STOP();


  if (found) {
    Serial.println("STATION ALIGNED!!!");
    FollowLineRVS();
  }
  else {
    Serial.println("FAILED...TO FIND THE STATION...");
    //FollowLineRVS();
  }

  if (!STOPALL) {
    // FollowLineRVS();
  }
  makeEncHappen = false;
}


void FollowLineRVS() {
  STOPALL = false;
  runIRreceiver = true; // IR station signal will be looked for
  armDown();
  stopcharge();
  delay(10);

  String state = "Docking with line";
  _server.send(200, "text/plain", state);

  check = true; // allow GotStuck to be checked
  GotStuck = false;
  flwln = true; // disable cliff sensing (deprecated)
  int last = 0;
  STOPALL = false;
  SPEED = 170;
  int del = 50;
  int test = 0;
  unsigned long GStart = millis();//  set a time limit
  bumped = false;
  noblink = true; // blink led takes millis...

  boolean dontrestartthisloop = false; // if this loop is called by dock, you don't want its restart feature to be triggered, dock already does it. 
  // in that case it'll be set to true at break point below. 

  while (!STOPALL && !haspower() && millis() - GStart < 120000) {
    loop();

    
      rvs(SPEED, 0);
      
 
    while (SeeLineLeftRear() && !STOPALL && !haspower() && !GotStuck) {
      loop();
      unsigned long thisS = millis();
      right(180);
      while (SeeLineRightRear() && !STOPALL && millis() - thisS < 50) {
        loop();
        if(GotStuck)
        {
          GotStuck = false;
          right(200);
        }
      }
      //Serial.print("left  "); Serial.println(SeeLineLeftFront());
      Serial.println("SL RVS");
      delay(del);
      loop();
    }
    while (SeeLineRightRear() && !STOPALL && !haspower() && !GotStuck) {
      loop();

      unsigned long thisS = millis();
      left(180);
      while (SeeLineRightRear() && !STOPALL && millis() - thisS < 50) {
        loop();
        if(GotStuck)
        {
          GotStuck = false;
          left(200);
        }
      }
      //Serial.print("right "); Serial.println(SeeLineRightFront());
      Serial.println("SR RVS");
      delay(del);
      loop();
    }
    if (GotStuck) {
      GotStuck = false;

      Serial.println("EXECUTING LINE FWD!!");
      unsigned long thisStart = millis();
      loop();
      while (!STOPALL && !BothFrontSeen() && millis() - thisStart < 2500 && !bumped && !frontIR()) {
        fwd(170, 0);
        execLineFwd();
        loop();
      }
      if(bumped){
        bumped = false;
      }
      if (!Signal) {
        fwd(170, 1000);
        dock();
        dontrestartthisloop = true;
        break;
      }
    }
  }



  STOP();

  noblink = false;

  STOPALL = false;
  flwln = false; // enable cliff sensing

  //rvs(200, 2);// little extra push toward contacts
  delay(500);
  if (haspower()) {
    STOP();
    charge();
    _server.send(200, "text/plain", "DOCKED");
  }
  else if (!STOPALL && millis() - GStart < 120000 && !dontrestartthisloop) {
    execLineFwd();
    restartFollowRvs();
  }

}

void restartFollowRvs() {
  unsigned long thisWait = millis();
  while (millis() - thisWait < 2000) {
    loop();
  }
}

void GetCloser()
{

  // get closer
  loop();
  if (Signal && !rearOBS()) {
    rvs(170, 3);
  }
  // avoid rear obstacle and getting stuck
  if (rearOBS() || GotStuck)
  {
    GotStuck = false;
    fwd(180, 10);
  }
  loop();
}
