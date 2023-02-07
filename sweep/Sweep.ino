#include <math.h>
#include <driver/adc.h>

// SERIAL ///
//#include "HardwareSerial.h"
//HardwareSerial Serial2(2); // as of ESP32 1.0.1 the object Serial2 seems to be already created in the HardwareSerial.cpp file... 
//#include "driver/adc.h"
//#include "esp_adc_cal.h"
//#include "SPIFFS.h"

///
#include <ESP32Servo.h>

#include "Arduino.h"
//#include <Streaming.h>      // Ref: http://arduiniana.org/libraries/streaming/ // not compatible under ESP32 hardware Serial
//#include <IRremote.h>


//******************************************************************************************
/// Wifi (for ajax webpage)
//******************************************************************************************
#include <ETH.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "index.h"  //Web page header file

//******************************************************************************************
/// Arduino OTA (redundent but necessary for custom name for now)
//******************************************************************************************
#include <ArduinoOTA.h>

#include <ESP32Servo.h>



//******************************************************************************************
//ESP32 WiFi Information
//******************************************************************************************
const char * str_ssid     = "WIFI_SSID";  //  <---You must edit this line!
const char * str_ssid2     = "WIFI_SSID2";
const char * str_password = "WIFI_PASSWORDGahsh4479#$";                   //  <---You must edit this line!
IPAddress ip(192, 168, 10, 223);       //Device IP Address       //  <---You must edit this line!
IPAddress gateway(192, 168, 10, 1);    //Router gateway          //  <---You must edit this line!
IPAddress subnet(255, 255, 255, 0);   //LAN subnet mask         //  <---You must edit this line!
IPAddress dnsserver(192, 168, 10, 1);  //DNS server              //  <---You must edit this line!


WebServer server(80);

//******************************************************************************************
//Define which Arduino Pins will be used for each device
//******************************************************************************************
//"RESERVED" pins for ESP32 - best to avoid
#define PIN_0_RESERVED             0  //reserved ESP32 boot/program upload
#define PIN_1_RESERVED             1  //reserved ESP32 for TX0
#define PIN_3_RESERVED             3  //reserved ESP32 for RX0
#define PIN_6_RESERVED             6  //reserved ESP32 for flash
#define PIN_7_RESERVED             7  //reserved ESP32 for flash
#define PIN_8_RESERVED             8  //reserved ESP32 for flash
#define PIN_9_RESERVED             9  //reserved ESP32 for flash
#define PIN_10_RESERVED           10  //reserved ESP32 for flash
#define PIN_11_RESERVED           11  //reserved ESP32 for flash

#define CHARGER             13 // charging power relay sends signal to ESP8266 // !!! boot fails if 12 is pulled high
#define POWERCamAndMotors   23 // power to motors and cameras

#define ENABLE_LEFT         19  //Left speed modulation 
#define MOTOR_LEFT1         18  //left motor
#define MOTOR_LEFT2         5

#define MOTOR_RIGHT1        17 // right motor 
#define MOTOR_RIGHT2        16
#define ENABLE_RIGHT        4 //Right speed modulation 

#define TRACK_MOTOR_A       3
#define TRACK_MOTOR_B       21


// analog pins
#define PW                  A0 // charging and battery current measurment GPIO36 

Servo arm;
#define armPin              12

//#define RST               13 // 

// in order to allow for USB upload without pressing the damn button, add a 47uf capacitor between EN and ground
// and a 1kohm resistor between 5V (not 3.3, doesn't work for me...) and EN to allow reboot after flash
/// https://randomnerdtutorials.com/solved-failed-to-connect-to-esp32-timed-out-waiting-for-packet-header/

#define LED                22 //



//GPIOs 34 to 39 are GPIOs – input only pins. These pins don’t have internal pull-ups or pull-down resistors.
//They can’t be used as outputs, so use these pins only as inputs:
//
//GPIO 34
//GPIO 35
//GPIO 36
//GPIO 37
//GPIO 38
//GPIO 39

Servo myservo;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32

int pos = 0;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
int servoPin = 12;

void setup() {


void setup()
{
  Serial.println("SERIAL BEGIN");
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);

  Serial.println("POWERCamAndMotors");
  pinMode(POWERCamAndMotors, OUTPUT);
  digitalWrite(POWERCamAndMotors, 0);// always off by default out of safety precaution

  Serial.println("TRACKS PINS");
  pinMode(TRACK_MOTOR_A, OUTPUT);
  pinMode(TRACK_MOTOR_B, OUTPUT);
  delay(1);
  trasckstop();

  Serial.println("MOTORS PINS");
  pinMode(ENABLE_LEFT, OUTPUT); ////
  pinMode(MOTOR_LEFT1, OUTPUT);
  pinMode(MOTOR_LEFT2, OUTPUT);
  pinMode(ENABLE_RIGHT, OUTPUT);
  pinMode(MOTOR_RIGHT1, OUTPUT);
  pinMode(MOTOR_RIGHT2, OUTPUT);


  Serial.println("ATTACHING SERVOS");
  arm.setPeriodHertz(50);    // standard 50 hz servo
  arm.attach(armPin, 500, 2500); // attaches the servo on pin 18 to the servo object
  // using default min/max of 1000us and 2000us
  // different servos may require different min/max settings
  // for an accurate 0 to 180 sweep

  Serial.println("MOTORS CONFIGURATION");
  // configure PWM functionalitites
  ledcSetup(pwmChannelL, freq, resolution);
  ledcSetup(pwmChannelR, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ENABLE_LEFT,  pwmChannelL);
  ledcAttachPin(ENABLE_RIGHT, pwmChannelR);
  delay(1);
  Serial.println("STOPPING MOTORS");
  MotorsStop();

  Serial.println("CHARGER");
  pinMode(CHARGER, OUTPUT);

  Serial.println("SETTING ADC");
  analogReadResolution(11);        // Default of 12 is not very linear.  Recommended to use 10 or 11 depending on needed resolution.
  analogSetAttenuation(ADC_6db);   // Default is 11db which is very noisy.  Recommended to use 2.5 (noted "ADC_2_5db") or 6.


  connection();
  Serial.println("STARTING SERVER AND XMLHttp");
  initXMLhttp();
  server.begin();

  Serial.println("STARTING OTA CONFIG");
  OTAconfig();

  Serial.println("SETUP SUCCESSFUL");
  Blink(5, 100);
}

void OTAconfig()
{

  // Port defaults to 3232
  ArduinoOTA.setPort(3232);
  // Hostname defaults to esp3232-[MAC]
  // create OTA name
  const char *NameOTA = "KOBRA";
  ArduinoOTA.setHostname(NameOTA);
  // No authentication by default
  // ArduinoOTA.setPassword("admin");
  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
  .onStart([]() {
    OTArequest = true;

    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("\nEnd");
    Blink(3, 50);
  })
  .onProgress([](unsigned int progress, unsigned int total) {

    if ((progress / (total / 100)) >= lastProgress + 10)
    {
      Serial.println();
      Serial.printf("OTA Upload: %u%%\r", (progress / (total / 100)));
      lastProgress = progress / (total / 100);
    }
    digitalWrite(LED, !digitalRead(LED));

  })
  .onError([](ota_error_t error) {
    OTArequest = false;
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    Reset();
  });
  ArduinoOTA.begin();
}


void connection()
{
  WiFi.disconnect(true);
  delay(1000);

  Serial.println(F("Disabling ESP32 WiFi Access Point"));
  Serial.println(F(""));
  WiFi.mode(WIFI_STA);

  WiFi.config(ip, gateway, subnet, dnsserver);

  Serial.println(F("Initializing ESP32 WiFi network.  Please be patient..."));
  delay(1000);

  // attempt to connect to WiFi network
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(str_ssid);
  lastSSID = 1;
  WiFi.begin(str_ssid, str_password);

  Start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - Start < 15000) {
    Serial.print(F("."));
    delay(500); // wait for connection:
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("FAILED!");
    Serial.print("TRYING TO CONNECT TO SECOND SSID: ");
    Serial.println(str_ssid);
    lastSSID = 2;
    WiFi.begin(str_ssid, str_password);

    Start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - Start < 15000) {
      Serial.print("- ");
      delay(500);
    }
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println(F(" ***********************************************"));
      Serial.println(F("           FAILED TO CONNECT TO ANY WIFI        "));
      Serial.println(F(" ***********************************************"));
      Serial.println(F("               EXECUTING HARD RESET             "));
      Serial.println(F(" ***********************************************"));

      esp_restart();
    }
    else {
      Serial.println("");
      Serial.println("WiFi connected!");


    }
  }


  Serial.println();
  Serial.println(F(""));
  Serial.print(F("localIP = "));
  Serial.println(WiFi.localIP());
  Serial.print(F("MAC address = "));
  Serial.println(WiFi.macAddress());

}
}

void loop() {

	for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
		// in steps of 1 degree
		myservo.write(pos);    // tell servo to go to position in variable 'pos'
		delay(15);             // waits 15ms for the servo to reach the position
	}
	for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
		myservo.write(pos);    // tell servo to go to position in variable 'pos'
		delay(15);             // waits 15ms for the servo to reach the position
	}
}
