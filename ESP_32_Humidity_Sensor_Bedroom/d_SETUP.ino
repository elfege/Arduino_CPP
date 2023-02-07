
void setup()
{


  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0);

  //******************************************************************************************
  // Setup the default values for the ADC.  Used for analog voltage reads.
  //  Notes:  analogReadResolution(12) sets the resolution for all pins. 12 = 0-4095, 11 = 0-2047, 10 = 0-1024, 9 = 0-512
  //          analogSetAttenuation(ADC_11db) sets the attenuation for all pins.  11db = 0-3.3v, 6dB range = 0-2.2v, 2.5db = 0-1.5v, 0db = 0-1v
  //          analogSetPinAttenuation(A7, ADC_11db) sets the attenuation for individual pins.
  //******************************************************************************************

  analogReadResolution(11);        // Default of 12 is not very linear.  Recommended to use 10 or 11 depending on needed resolution.
  analogSetAttenuation(ADC_6db);   // Default is 11db which is very noisy.  Recommended to use 2.5 or 6.

  //******************************************************************************************
  //Declare each Device that is attached to the Arduino
  //  Notes: - For each device, there is typically a corresponding "tile" defined in your
  //           SmartThings Device Hanlder Groovy code, except when using new COMPOSITE Device Handler
  //         - For details on each device's constructor arguments below, please refer to the
  //           corresponding header (.h) and program (.cpp) files.
  //         - The name assigned to each device (1st argument below) must match the Groovy
  //           Device Handler names.  (Note: "temphumid" below is the exception to this rule
  //           as the DHT sensors produce both "temperature" and "humidity".  Data from that
  //           particular sensor is sent to the ST Hub in two separate updates, one for
  //           "temperature" and one for "humidity")
  //         - The new Composite Device Handler is comprised of a Parent DH and various Child
  //           DH's.  The names used below MUST not be changed for the Automatic Creation of
  //           child devices to work properly.  Simply increment the number by +1 for each duplicate
  //           device (e.g. contact1, contact2, contact3, etc...)  You can rename the Child Devices
  //           to match your specific use case in the ST Phone Application.
  //******************************************************************************************
  //Polling Sensors
  //  static st::PS_Water               sensor1(F("water1"), 5, 10, PIN_WATER_1, WetVal);
  //  static st::PS_Water               sensor2(F("water2"), 60, 10, PIN_WATER_2, 500);
  //  static st::PS_Illuminance         sensor3(F("illuminance1"), 60, 20, PIN_ILLUMINANCE_1, 0, 4095, 0, 10000);
  //  static st::PS_Illuminance         sensor4(F("illuminance2"), 60, 30, PIN_ILLUMINANCE_2, 0, 4095, 0, 10000);
  //  static st::PS_Voltage             sensor5(F("voltage1"), 60, 40, PIN_VOLTAGE_1, 0, 4095, 0, 3300, 1, 100);
  //  //  static st::PS_Voltage             sensor5(F("voltage1"), 5, 1, PIN_VOLTAGE_1, 0, 4095, 0, 4095, 20, 75,  -0.000000025934, 0.0001049656215,  0.9032840665333,  204.642825355678);
  //  static st::PS_MQ2_Smoke           sensor6(F("smoke1"), 10, 3, PIN_SMOKE_1, 1000);
      static st::PS_TemperatureHumidity sensor7(F("temphumid1"), 15, 5, PIN_TEMPERATUREHUMIDITY_1, st::PS_TemperatureHumidity::DHT22, "temperature", "humidity");
  //  static st::PS_DS18B20_Temperature sensor8(F("temperature2"), 60, 55, PIN_TEMPERATURE_2, false, 10, 1);
  //
  //  //Interrupt Sensors
  //  static st::IS_Motion              sensor9(F("motion1"), PIN_MOTION_1, HIGH, false, 500);
  //  static st::IS_Motion              sensor10(F("motion2"), PIN_MOTION_2, HIGH, false, 500);
  //  static st::IS_Contact             sensor11(F("contact1"), PIN_CONTACT_1, LOW, true, 500);
  //  static st::IS_Contact             sensor12(F("contact2"), PIN_CONTACT_2, LOW, true, 500);
  //  static st::IS_Smoke               sensor13(F("smoke2"), PIN_SMOKE_2, HIGH, true, 500);
  //  static st::IS_Button              sensor14(F("button1"), PIN_BUTTON_1, 1000, LOW, true, 500);
  //  static st::IS_Button              sensor15(F("button2"), PIN_BUTTON_2, 1000, LOW, true, 500);
  //  static st::IS_CarbonMonoxide      sensor16(F("carbonMonoxide1"), PIN_CO_1, HIGH, true, 500);
  //
  //  //Special sensors/executors (uses portions of both polling and executor classes)
  //  //static st::IS_DoorControl         sensor17(F("doorControl1"), PIN_DOORCONTROL_CONTACT_1, LOW, true, PIN_DOORCONTROL_RELAY_1, LOW, true, 1000);
  //  static st::S_TimedRelay           sensor18(F("relaySwitch1"), PIN_TIMEDRELAY_1, LOW, false, 3000, 0, 1);

  //Executors
  //  static st::EX_Switch              executor1(F("switch1"), PIN_SWITCH_1, LOW, true);
  //  static st::EX_Alarm               executor2(F("alarm1"), PIN_ALARM_1, LOW, true);
  //  static st::EX_Alarm               executor3(F("alarm2"), PIN_ALARM_2, LOW, true, PIN_STROBE_2);

  //*****************************************************************************
  //  Configure debug print output from each main class
  //  -Note: Set these to "false" if using Hardware Serial on pins 0 & 1
  //         to prevent communication conflicts with the ST Shield communications
  //*****************************************************************************
  st::Everything::debug = true;
  st::Executor::debug = true;
  st::Device::debug = true;
  st::PollingSensor::debug = true;
  st::InterruptSensor::debug = true;

  //*****************************************************************************
  //Initialize the "Everything" Class
  //*****************************************************************************

  //Initialize the optional local callback routine (safe to comment out if not desired)
  //st::Everything::callOnMsgSend = callback;

  // call out function forward decalaration
  SmartThingsCallout_t messageCallout;

  //Create the SmartThings ESP32WiFi Communications Object
  //STATIC IP Assignment - Recommended
  bool enableDebug = true;
  st::Everything::SmartThing = new st::SmartThingsESP32WiFi(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);//st::receiveSmartString);

  //st::OTA_Name::CustDeviceName(NameOTA);

  //DHCP IP Assigment - Must set your router's DHCP server to provice a static IP address for this device's MAC address
  //st::Everything::SmartThing = new st::SmartThingsESP32WiFi(str_ssid, str_password, serverPort, hubIp, hubPort, st::receiveSmartString);

  //Run the Everything class' init() routine which establishes WiFi communications with SmartThings Hub
  st::Everything::init();

  //*****************************************************************************
  //Add each sensor to the "Everything" Class
  //*****************************************************************************
  // st::Everything::addSensor(&sensor1);
  //  st::Everything::addSensor(&sensor2);
  //  st::Everything::addSensor(&sensor3);
  //  st::Everything::addSensor(&sensor4);
  //  st::Everything::addSensor(&sensor5);
  //  st::Everything::addSensor(&sensor6);
  st::Everything::addSensor(&sensor7);
  //  st::Everything::addSensor(&sensor8);
  //  st::Everything::addSensor(&sensor9);
  //  st::Everything::addSensor(&sensor10);
  //  st::Everything::addSensor(&sensor11);
  //  st::Everything::addSensor(&sensor12);
  //  st::Everything::addSensor(&sensor13);
  //  st::Everything::addSensor(&sensor14);
  //  st::Everything::addSensor(&sensor15);
  //  st::Everything::addSensor(&sensor16);
  //  //st::Everything::addSensor(&sensor17);
  //  st::Everything::addSensor(&sensor18);

  //*****************************************************************************
  //Add each executor to the "Everything" Class
  //*****************************************************************************
  //  st::Everything::addExecutor(&executor1);
  //  st::Everything::addExecutor(&executor2);
  //  st::Everything::addExecutor(&executor3);

  //*****************************************************************************
  //Initialize each of the devices which were added to the Everything Class
  //*****************************************************************************
  //Serial.println("END OF SETUP");
  st::Everything::initDevices();

  server.begin();


  // Port defaults to 3232
  ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // create OTA name
  const char *NameOTA = "Humidity_Bedroom";
  ArduinoOTA.setHostname(NameOTA);


  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();

  Blink(10, 50);
}
