#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEA
};
IPAddress ip(192, 168, 10, 192);
byte gateway[] = { 192, 168, 10, 1 };   //your router's IP address
byte subnet[] = { 255, 255, 255, 0 };    //subnet mask of the network
IPAddress myserver(192, 168, 10, 110);
EthernetServer server(80);
EthernetClient client();

int Value = HIGH;
int Val = 20;
int Multiplier = 48.0; // source's voltage and taking into account pull up resistor
int lastloop = 5; // we don't want last loop to run the first time this value is reset once powerOn was triggered
int runpower = 0;


void setup()
{

  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(8, INPUT);
  /*digitalWrite(8, LOW);  // turn on pulldown resistors*/

  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();

}

int PowerOn()
{
  lastloop = 0;
  runpower = 1;
  Serial.println("now pressing talk button");
  digitalWrite(2, LOW); // talk button to activate intercom
  delay(5000); // tenant has 5 secs to talk if needed
  Serial.println("now releasing talk button");
  digitalWrite(2, HIGH); // release talk button

  delay(200);
  Serial.println("now opening DOOR");
  digitalWrite(4, LOW); // press the door's button
  delay(2000);
  digitalWrite(4, HIGH); // release door's button

  Serial.println("Looking at videofeed");
  delay(10000); // leaves time to look at the videofeed

  Serial.println("now pressing door buttont to terminate com");
  digitalWrite(2, LOW); // press talk button to terminate com
  delay(200);
  digitalWrite(2, HIGH); // release talk button
}

int PowerOff()
{
  digitalWrite(2, HIGH); //    buzer is off on HIGH
  digitalWrite(4, HIGH); //

}


void loop() {

  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (Multiplier / 1023.0);
  
  Serial.println("Voltage is:");
  Serial.println(voltage);
  Serial.println("FYI this was a result for sensorValueval of:");
  Serial.println(sensorValue);

  delay(500);

  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    String buffer = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.print(c);
        buffer += c;
        if (c == '\n' && currentLineIsBlank) {

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();


          client.print("<form action=\"http://207.38.192.24:8002/\" >");
          if (sensorValue >= Val) {
            client.print("<p align=\"center\">Buzer is being<font color='green'>PRESSED</font></p>");
          }
          else {
            client.print("<p align=\"center\">Buzer is <font color='red'>idle</font></p>");
          }
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"11\">OFF</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"10\">ON</p>");
          client.print("<p align=\"center\"><input type=\"submit\" value=\"Submit\"> </p></form>");

          break;
        }
        if (c == '\n') {

          currentLineIsBlank = true;
          buffer = "";
        }
        else if (c == '\r') {
          if (buffer.indexOf("GET /?status=11") >= 0)
          {
            PowerOff();
          }
          if (buffer.indexOf("GET /?status=10") >= 0)
          {
            PowerOn();
          }

        }
        else {

          currentLineIsBlank = false;
        }
      }
    }

    {
      delay(500);
      client.stop();

    }
  }

  if (voltage >= Val && runpower == 0) // bell's ringing
  {
    Serial.println("sensorValueval is:");
    Serial.println(sensorValue);
    /*digitalWrite(8, Value); // reading the port floats it so reseting port to its default value*/
    /*Serial.println("if (sensorValue >= Val) // condition met, running powerOn");*/

    PowerOn();
  }

  else
  {
    Serial.println("if (sensorValue >= Val) // condition NOT met, doing NOTHING/ turning off switches");
    PowerOff();

  }

  Serial.println("END OF MAIN LOOP Making sure in the mean time that the com is terminated");

  while (voltage >= Val && lastloop <= 4 && runpower != 0)
  {


    digitalWrite(2, LOW); // press talk button to terminate com
    delay(200);
    digitalWrite(2, HIGH); // release talk button

    Serial.println("lastloop value is");
    Serial.println(lastloop);
    lastloop++;
    delay(200);

  }


  runpower = 0;

  Serial.println("END");

  delay(500);

}





