int seconds = 1000;
long hour = (unsigned long int)3600 * (unsigned long int)seconds;
int desiredTime = 2;
long TimeLimit = (unsigned long int)desiredTime * (unsigned long int)hour;
int elapsed = 0 ;// variable redefined below with time()



unsigned long time;


void setup()
{

  Serial.begin(9600);
  pinMode(8, OUTPUT);// controls a relay connected to reset button of the other board
   
// wait for the other board to start
delay(4000); 
// reset the other board once 
digitalWrite(8, HIGH);
delay(1000);
digitalWrite(8, LOW);


}

void(* SoftReset) (void) = 0;


int Reset() {
  Serial.print("HARD RESET");
  digitalWrite(8, HIGH); // resets the board
  delay(500);
  digitalWrite(8, LOW); //

  // now wait a little and soft reset this board to restart clock
  delay(400);
  Serial.print("SOFT RESET");
  SoftReset();
}

void loop() {

  Serial.print("Current Elapsed Time: ");
  time = millis();
  //prints time since program started
  Serial.println(time);
  Serial.print("Time limit is 12 hours, that is: ");
  Serial.print(TimeLimit);
  Serial.print(" milliseconds ");
  // wait a second so as not to send massive amounts of data
  delay(1000);

  // automatic hard reset every 12 hours
  if (time >= TimeLimit) {
    Serial.print("Reseting the board");
    Reset();
  }
  else {

    long remain = (unsigned long int)TimeLimit - time;
    long remainingHours = (unsigned long int)remain / (unsigned long int)hour;

    // remaining minutes = nombre de minutes restantes moins celles déjà écoulées
    long remainingSeconds = (unsigned long int)remain / (unsigned long int)seconds;
    long Minutes = (unsigned long int)remainingSeconds / 60;

    Serial.print("Hard RESET in ");
    Serial.print(" ");
    Serial.print(Minutes);
    Serial.print(" ");
    Serial.print("minutes");
    Serial.print(" ");
    Serial.print("(");
    Serial.print(remainingSeconds);
    Serial.print(" ");
    Serial.print(" seconds) ");

    Serial.println(" ");


  }
}



