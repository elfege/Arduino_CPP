#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <BridgeServer.h>


BridgeServer server;
String startString;
long hits = 0;


void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(8, INPUT);
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  bridge.begin();
  server.listenOnLocalhost();
  server.begin();

}



int EverythingLOW()
{
  if (digitalRead(8) == LOW)
  {
    digitalWrite(2, LOW); //
    delay(500);
    digitalWrite(4, LOW); //
  }
}


int OpenDoor()
{
  if (digitalRead(8) == HIGH) // Somone rang the bell, open the door
  {
    digitalWrite(2, HIGH); // pressing the talk button
    delay(500);            // time for com to be established
    digitalWrite(4, HIGH); // Unlock the door
  }
}

void loop()
{

  YunClient client = server.accept();
  if (client) {
    process(client);

    client.stop();
  }
}


void process(YunClient client) {
  String command = client.readStringUntil('/');

  if (command == "digital") {
    digitalCommand(client);
  }
  if (command == "analog") {
    analogCommand(client);
  }
  if (command == "mode") {
    modeCommand(client);
  }
}

void digitalCommand(YunClient client) {
  int pin, value;
  pin = client.parseInt();

  if (client.read() == '/') {
    value = client.parseInt();
    digitalWrite(pin, value);
  }
  else {
    value = digitalRead(pin);
  }

  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" set to "));
  client.println(value);

  String key = "D";
  key += pin;
  Bridge.put(key, String(value));
}

void analogCommand(YunClient client) {
  int pin, value;

  pin = client.parseInt();

  if (client.read() == '/') {
    value = client.parseInt();
    analogWrite(pin, value);

    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" set to analog "));
    client.println(value);

    String key = "D";
    key += pin;
    Bridge.put(key, String(value));
  }
  else {
    value = analogRead(pin);

    client.print(F("Pin A"));
    client.print(pin);
    client.print(F(" reads analog "));
    client.println(value);

    String key = "A";
    key += pin;
    Bridge.put(key, String(value));
  }
}

void modeCommand(YunClient client) {
  int pin;
  pin = client.parseInt();
  if (client.read() != '/') {
    client.println(F("error"));
    return;
  }
  String mode = client.readStringUntil('\r');

  if (mode == "input") {
    pinMode(pin, INPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as INPUT!"));
    return;
  }

  if (mode == "output") {
    pinMode(pin, OUTPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as OUTPUT!"));
    return;
  }

  client.print(F("error: invalid mode "));
  client.print(mode);
}



