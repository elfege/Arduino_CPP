#define input 25 //  input read (GPIO25)(port 20 on breadboard 400);
#define LED 2



void setup() {
  pinMode(input, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  Blink(5, 100);


}

void loop() {

  countMicro();


}


void Blink(int times, int lapse) {
  int c = 0;
  while (c < times) {
    digitalWrite(LED, 1);
    delay(lapse);
    digitalWrite(LED, 0);
    delay(lapse);
    c++;
  }
}

int countMicro() {

  int c = 0; // counter
  int v; // read analog value

  Serial.println("WAITING.. for low val");
  Serial.print("c: "); Serial.println(c);
  v = digitalRead(input);
  while (v) { // as long input returns >n, wheel is not moving, hang in there and wait
    v = digitalRead(input);

    //delayMicroseconds(10);
  }
  Serial.print("LOOP 1 BROKE AT v = "); Serial.println(v);
  //  if (c >= 63) {
  //    c = 0;
  //  }
  c++;

  Serial.println("WAITING.. for high val");
  Serial.print("c: "); Serial.println(c);
  v = digitalRead(input);
  while (!v) { // as long input returns <n, wheel is not moving, hang in there and wait
    v = digitalRead(input);

    //delayMicroseconds(10);
  }
  Serial.print("LOOP 2 BROKE AT v = "); Serial.println(v);
  //  if (c >= 63) {
  //    c = 0;
  //  }
  c++;
  // value changed, there's motion

  return c;

}

