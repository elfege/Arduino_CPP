void turn(float numberofturns, int vit1, int vit2, int precis, int nbme, int mode = 0);

//set the requency to 62500/625 = 50Hz
#include<Servo.h>
Servo myservo;
byte pwmPin = 3;
byte servoPin = 9;
float tours = 4;
int vitesse1 = 70;//vitesse au début du turn

int vitesse2 = 88;//vitesse à la fin du turn pour plus de précision et pendant que le servo ralentit

int nbmesures = 5;//nombre de mesures durant la fin du turn pour plus de précision

int precis = 350;//le servo va commencer ralentir 350/1024 tours avant la fin du turn pour plus de précision

void setup() {
  pinMode(pwmPin, INPUT);
  Serial.begin(115200);
  myservo.attach(servoPin);

}

void loop() {
  turn(tours, vitesse1, vitesse2, precis, nbmesures); //mettre turn(tours, vitesse1, vitesse2, precis, nbmesures); si on veut un code un peu plus rapide
  myservo.write(90);
  delay(2000);

  // vitesse1 += 1;
  Serial.println(vitesse1);



}

void turn(float numberofturns, int vit1, int vit2, int precis, int nbme, int mode = 0) {
  myservo.write(vit1);
  long int current = 0;
  if (mode == 0) { //mode rapide

    for (int i = 0; i < nbme; i++) {
      current += pulseIn(pwmPin, HIGH);
      //delay(1);
    }
    current = current / nbme;

  } else { // mode lent
    Serial.print("\n \n \n");

    for (int i = 0; i < 100; i++) {
      current += pulseIn(pwmPin, HIGH);
      //      Serial.println(current);
      //      Serial.print(" ");
      //delay(1);
    }
    current = current / 100;
    //    Serial.print("\n \n \n");
  }


  int desired = numberofturns * 1024 + current;
  int turns = 0;
  int add = 0; //add est le nombre de tours déja effectués

  //  Serial.print("\n Turns = ");
  //  Serial.print(numberofturns);
  //  Serial.print("current = ");
  //  Serial.print(current);
  //  Serial.print("  Desired = ");
  //  Serial.print(desired);

  //  Serial.print("\n\n\n\n\n");

  //commencer en tournant rapidement
  myservo.write(vit1);
  while (current + precis < desired) {
    //    Serial.println("rapide");
    current = pulseIn(pwmPin, HIGH) + add * 1024;

    //si un tour a ete effectué ajouter 1 à tours
    if (turns - 500 > current) {
      add += 1;
    }
    turns = current;
  }
  //finir en tournant ralentissant

  while (current < desired) {
    if (vit1 < vit2) {
      vit1++;
      myservo.write(vit1);
    }
    //    Serial.println("lent");
    //    Serial.print("current = ");
    //    Serial.print(current);
    //    Serial.print("   turns = ");
    //    Serial.println(pulseIn(pwmPin, HIGH));
    current = 0;
    //faire une moyenne des valeurs lues pour + de précision
    for (int i = 0; i < nbme; i++) {
      current += pulseIn(pwmPin, HIGH) + add * 1024;
    }
    current /= nbme;

    if (turns - 500 > current) {
      add += 1;
    }
    turns = current;
  }
  //stop
  myservo.write(90);

  //si le mode est précis, ajuster jusqu'a obtenir exactement la valeur désirée
  if (mode == 1) {
    desired = desired % 1024;
    do {
      current = 0;
      //faire une moyenne des valeurs lues pour + de précision
      Serial.println("\n \n \n");
      for (int i = 0; i < 100; i++) {
        current += pulseIn(pwmPin, HIGH);
        //        Serial.print(current);
        //        Serial.print(" ");
        //delay(1);
      }
      //      Serial.println("\n \n \n");
      current = current / 100;
      //      Serial.print(current);
      //      Serial.print("  des = ");
      //      Serial.println(desired);

      if (current > desired) {
        myservo.write(95);
        //delay((current - desired) * 10 + 10);
        myservo.write(90);
      }
      else if (current < desired) {
        myservo.write(89);
        //delay((desired - current) * 10 + 10);
        myservo.write(90);
      }
    } while (desired != current);
  }
}

