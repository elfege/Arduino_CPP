unsigned long ledMillis = millis();
#define LED                       2

/* TOUTCH BUTTONS */
#define modeSw                    18 
#define lowerTemp                 19
#define power                     13                     
#define raiseTemp                 26 
#define fanSpeed                  25

/* FEEDBACKS */
#define cooling                   34
#define dehumid                   35
#define fanonly                   36
#define fanlow                    39                 
#define fanmed                    32
#define fanhigh                   33 
#define heating                   21

void setup() {
  pinMode(LED,        OUTPUT); 
  pinMode(modeSw,     OUTPUT);
  pinMode(lowerTemp,  OUTPUT);
  pinMode(power,      OUTPUT);  
  pinMode(raiseTemp,  OUTPUT);
  pinMode(fanSpeed,      OUTPUT);

  digitalWrite(LED, 0);
  digitalWrite(modeSw, 1);
  digitalWrite(lowerTemp, 1);
  digitalWrite(power, 1);
  digitalWrite(raiseTemp, 1);
  digitalWrite(fanSpeed, 1);
  

  pinMode(cooling,    INPUT );
  pinMode(dehumid,    INPUT );
  pinMode(fanonly,    INPUT );
  pinMode(fanlow,     INPUT );
  pinMode(fanmed,     INPUT );
  pinMode(fanhigh,    INPUT );

  // setup debug serial port
  Serial.begin(115200);         // setup serial with a baud rate of 9600
  delay(10);
  Serial.println("setup..");  // print out 'setup..' on start

}

void loop() {
  if (millis() - ledMillis >= 1000)
    {
      digitalWrite(LED, !digitalRead(LED));
      ledMillis = millis();
      Serial.println("BLINK");
    }

}
