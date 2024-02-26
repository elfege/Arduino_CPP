

bool canDown() {
  return !digitalRead(candown);
}

bool canInPosition() {
  return !digitalRead(canSensor);  // limit switch
}

bool eatingZonePushed()  // if old can pressing ignore sensor and use timer instead
{

  return !digitalRead(eatingZoneSensor);
}

bool pusherRetracted() {
  return !digitalRead(pusherRetract);
}

void wheelSpinning() {
  if (!canopening) {
    return;
  }

  // test for 1 change in sensor's state, and allow N millisecs for that to happen
  spin = wheelStateChange(1, 1500);  // global used only during opencan()
}

bool wheelStateChange(int occurrences, int test_duration) {
  /*
   * FUNCTION THAT TESTS FOR SPIN/NO SPIN of the can during opening operation
   *
   * PARAMETER 1: number of state change's occurences needed to declare that the can is duly spinning, or not. 
   * 
   * the higher the parameter 1 is, the less sensitive the rotary sensor will be
   * if paramater value is 1, then spin is detected if and only if there has been 1 and only 1 change of state within N millis() => more sensitive to spin, less sensitive to declaring a spin faillure
   * if paramter value is 3, state change must happen at least 4 times before the wheelStateChange() func returns true => less sensitive to spin, more sensitive to declaring there's a spin faillure
   *
   *
   * PARAMETER 2: test duration during which the handler can detect a change of state and thus declare spin, or not. 
   * 
   * The longer the test duration, the more sensitive to spin, and less sensitive to declaring a spin problem
   * The shorter the test duration, the less sensitive to spin, and more sensitive to declaring a spin problem
   * 
   * If you want LESS "no spin detection" decrease param 1 and increase param 2
   * 
   * 
   */

  // return true;  // always true if harware fails

  int R = digitalRead(wheel);
  unsigned long Start = millis();
  int c = 0;
  int detect_delay = 500;                                      // another way to modulate sensitivity: the longer, the more sensitive to spin, the less sensitive to spin faillures
  while (c < occurrences && millis() - Start < test_duration)  // detect at least N incrementation [occurences] within N milliseconds
  {
    while (R == digitalRead(wheel) && millis() - Start < detect_delay && !stopped)  // increment c based at the FIRST state change within N millis / or do nothing
    {
      httpHandler();
    }
    if (digitalRead(wheel) != R) {
      c++;
    }
  }
  //  term.println("WSC "+String(c));
  hasSpun = c >= occurrences;
  return c >= occurrences;  // return true if state has changed at least N [occurences] times during the 300ms
}

bool deformed() {
  // return false; // needs structural fixing - there's a current issue with multiple sensors


  int Read = analogRead(deformSensor);
  if (Read > 2000) {
    term.println("DEFORMATION => " + String(Read) + " RETURNING FALSE - NEEDS FIXING!!!!!!");
    // deformation = true;
    
    deformation = false;

    return deformation;
  } else {
    deformation = false;
    return false;
  }
}

bool canLocked() {
  return !digitalRead(lockSensor);
}

bool empty1() {
  return false;  // decomissioned analogRead(IR1_pile) < 1800;
}

bool empty2() {
  return analogRead(IR2_pile) < 1800;
}

bool empty() {
  return empty2();
}
bool oneCanLeft() {
  return !empty1() && empty2();
}
bool full() {
  return !empty1() && !empty2();
}
