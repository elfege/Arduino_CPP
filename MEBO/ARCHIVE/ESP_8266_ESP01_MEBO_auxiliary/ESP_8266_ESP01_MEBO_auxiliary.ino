// mebo auxiliary power switch



#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
//#include <WiFiUdp.h>
//#include <ArduinoOTA.h>

#define PIN0 0
#define PIN2 2
#define PIN3 3

#define LED 1

const char* ssid     = WIFI_SSID2;
const char* password = WIFI_PASSWORD;

IPAddress ip(192, 168, 10, 105);
IPAddress gateway(192, 168, 10, 1);
IPAddress subnet(255, 255, 255, 0);

unsigned long previousMillisBlink = 0;
unsigned long Start = 0;
boolean AllowOTA = false;

int value = 0;

WiFiServer server(80);


void setup()
{
  Serial.begin(115000);
  delay(100);
  Serial.println("Booting");
  //WiFi.mode(WIFI_STA);

  pinMode(PIN0, OUTPUT);
  pinMode(PIN2, OUTPUT);
  pinMode(PIN3, OUTPUT);
  digitalWrite(PIN0, 0);
  digitalWrite(PIN2, 0);
  digitalWrite(PIN3, 0);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);



  WiFi.config(ip, gateway, subnet);

  WiFi.begin(ssid, password);

  Start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - Start < 5000) {
    delay(1000);
    Serial.print(".");
  }
  if (WiFi.status() != WL_CONNECTED) {
    Reset();
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();


  //
  //    // Port defaults to 3232
  //    ArduinoOTA.setPort(3232);
  //
  //    // Hostname defaults to esp3232-[MAC]
  //    // create OTA name
  //    const char *NameOTA = "AUXILIARY_POWER_MEBO";
  //    ArduinoOTA.setHostname(NameOTA);
  //
  //    // No authentication by default
  //    // ArduinoOTA.setPassword("admin");
  //
  //    // Password can be set with it's md5 value as well
  //    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  //    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  //
  //    ArduinoOTA.onStart([]() {
  //      Serial.println("Start");
  //    });
  //    ArduinoOTA.onEnd([]() {
  //      Serial.println("\nEnd");
  //    });
  //    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  //      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  //    });
  //    ArduinoOTA.onError([](ota_error_t error) {
  //      Serial.printf("Error[%u]: ", error);
  //      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
  //      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
  //      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
  //      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
  //      else if (error == OTA_END_ERROR) Serial.println("End Failed");
  //    });
  //    ArduinoOTA.begin();

  Serial.println("MEBO'S AUXILIARY POWER CONTROLLER READY.. ");

}



void loop() {

  // ArduinoOTA.handle();

  if (millis() - previousMillisBlink > 1000)
  {
    previousMillisBlink = millis();
    Blink(2, 200);

    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("WIFI DISCONNECTED. Waiting to see if library can fix it..");
      delay(5000); // wait for connection:
      if (WiFi.status() != WL_CONNECTED) {
        Serial.print("WIFI STILL DISCONNECTED. Hard Reseting board");
        delay(500);
        Reset(); // this is a hard reset on RST port.
      }
      else {
        Serial.print("WIFI connection restored..");
      }
    }
  }

  webserver();


}

void Reset() {
  Serial.println("RESTARTING ESP");
  Blink(10, 50);
  ESP.restart();
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

