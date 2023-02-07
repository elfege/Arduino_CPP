//*******************************************************************************
//  SmartThings NodeMCU ESP8266 Wifi Library
//
//  License
//  (C) Copyright 2017 Dan Ogorchock
//
//  History
//  2017-02-10  Dan Ogorchock  Created
//  2017-12-29  Dan Ogorchock  Added WiFi.RSSI() data collection
//  2018-01-06  Dan Ogorchock  Simplified the MAC address printout to prevent confusion
//  2018-01-06  Dan Ogorchock  Added OTA update capability
//  2018-02-03  Dan Ogorchock  Support for Hubitat
//*******************************************************************************

#include "SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB.h"

namespace st
{


  //*******************************************************************************
  // SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB Constructor - Static IP
  //*******************************************************************************
    SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB::SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB(String ssid, String ssid2, String password, IPAddress localIP, IPAddress localGateway, IPAddress localSubnetMask, IPAddress localDNSServer, uint16_t serverPort, IPAddress hubIP, uint16_t hubPort, IPAddress hubIP2, uint16_t hubPortb, SmartThingsCallout_t *callout, String shieldType, bool enableDebug, int transmitInterval) :
    SmartThingsEthernet_DUAL_HUB(localIP, localGateway, localSubnetMask, localDNSServer, serverPort, hubIP, hubPort, hubIP2, hubPortb, callout, shieldType, enableDebug, transmitInterval, false),
    st_server(serverPort)
    {
      ssid.toCharArray(st_ssid, sizeof(st_ssid));
      ssid2.toCharArray(st_ssid2, sizeof(st_ssid2));
      password.toCharArray(st_password, sizeof(st_password));
    }

  //*******************************************************************************
  // SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB Constructor - DHCP
  //*******************************************************************************
    SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB::SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB(String ssid, String ssid2, String password, uint16_t serverPort, IPAddress hubIP, uint16_t hubPort, IPAddress hubIP2, uint16_t hubPortb, SmartThingsCallout_t *callout, String shieldType, bool enableDebug, int transmitInterval) :
    SmartThingsEthernet_DUAL_HUB(serverPort, hubIP, hubPort, hubIP2, hubPortb, callout, shieldType, enableDebug, transmitInterval, true),
    st_server(serverPort)
    {
      ssid.toCharArray(st_ssid, sizeof(st_ssid));
      ssid2.toCharArray(st_ssid2, sizeof(st_ssid2));
      password.toCharArray(st_password, sizeof(st_password));
    }

  //*******************************************************************************
  // SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB Constructor - DHCP
  //*******************************************************************************
    SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB::SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB(uint16_t serverPort, IPAddress hubIP, uint16_t hubPort, IPAddress hubIP2, uint16_t hubPortb, SmartThingsCallout_t *callout, String shieldType, bool enableDebug, int transmitInterval) :
    SmartThingsEthernet_DUAL_HUB(serverPort, hubIP, hubPort, hubIP2, hubPortb, callout, shieldType, enableDebug, transmitInterval, true),
    st_server(serverPort)
    {
      st_preExistingConnection = true;
    }



  //*****************************************************************************
  //SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB::~SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB()
  //*****************************************************************************
    SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB::~SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB()
    {

    }

  //*******************************************************************************
  /// Initialize SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB Library
  //*******************************************************************************
    void SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB::init(void)
    {

      if (!st_preExistingConnection) {
        Serial.println(F(""));
        Serial.println(F("Initializing ESP8266 WiFi network.  Please be patient..."));

        if (st_DHCP == false)
        {
          WiFi.config(st_localIP, st_localGateway, st_localSubnetMask, st_localDNSServer);
        }
      // attempt to connect to WiFi network
        WiFi.begin(st_ssid, st_password);
        Serial.print(F("Attempting to connect to WPA SSID: "));
        Serial.println(st_ssid);
        lastSSID = 1;
      }
      Start = millis();
      while (WiFi.status() != WL_CONNECTED && millis() - Start < 15000) {
        Serial.print(F("."));
      delay(500); // wait for connection:
    }
    if (WiFi.status() != WL_CONNECTED) {
      if (lastSSID != 2) {
        Serial.print("CONNECTING TO ");
        Serial.println(st_ssid2);
        WiFi.begin(st_ssid2, st_password);
        lastSSID = 2;
      }
      else {
        Serial.print("CONNECTING TO ");
        Serial.println(st_ssid);
        WiFi.begin(st_ssid, st_password);
        lastSSID = 1;
      }
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
        delay(4000);

        ESP.reset(); // esp.reset is hard reset
      }
      else {
        Serial.println("");
        Serial.println("WiFi connected");
      }
    }


    Serial.println();

    st_server.begin();

    Serial.println(F(""));
    Serial.println(F("Enter the following three lines of data into ST App on your phone!"));
    Serial.print(F("localIP = "));
    Serial.println(WiFi.localIP());
    Serial.print(F("serverPort = "));
    Serial.println(st_serverPort);
    Serial.print(F("MAC Address = "));
    String strMAC(WiFi.macAddress());
    strMAC.replace(":", "");
    Serial.println(strMAC);
    Serial.println(F(""));
    Serial.print(F("SSID = "));
    Serial.println(st_ssid);
    Serial.print(F("PASSWORD = "));
    Serial.println(st_password);
    Serial.print(F("hubIP = "));
    Serial.println(st_hubIP);
    Serial.print(F("hubPort = "));
    Serial.println(st_hubPort);
    Serial.print(F("hubIP2 = "));
    Serial.println(st_hubIP2);
    Serial.print(F("hubPortb = "));
    Serial.println(st_hubPortb);
    Serial.print(F("RSSI = "));
    Serial.println(WiFi.RSSI());

    String("ESP8266_" + strMAC).toCharArray(st_devicename, sizeof(st_devicename));
    WiFi.hostname(st_devicename);

    Serial.println(F(""));
    Serial.println(F("SmartThingsESP8266WiFi: Intialized"));
    Serial.println(F(""));

    //Turn off Wirelss Access Point
    Serial.println(F("Disabling ESP8266 WiFi Access Point"));
    Serial.println(F(""));
    WiFi.mode(WIFI_STA);

    RSSIsendInterval = 5000;
    previousMillis = millis() - RSSIsendInterval;

    // Setup OTA Updates

    // Port defaults to 8266
    // ArduinoOTA.setPort(8266);

    // Hostname defaults to esp8266-[ChipID]
    ArduinoOTA.setHostname(st_devicename);

    // No authentication by default
    //ArduinoOTA.setPassword((const char*)"123");

    ArduinoOTA.onStart([]() {
      Serial.println("Start");
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();
    Serial.println("ArduinoOTA Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("ArduionOTA Host Name: ");
    Serial.println(ArduinoOTA.getHostname());
    Serial.println();
  }

  //*****************************************************************************
  // Run SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB Library
  //*****************************************************************************
  void SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB::run(void)
  {

    ArduinoOTA.handle();

    String readString;
    String tempString;
    String strRSSI;

    if (millis() - resendMillis > 15000 && resend != 0)
    {
      resend = 0;
      Serial.println("RESEND COUNTER RESET ----------");
    }

    if (WiFi.isConnected() == false)
    {
      Serial.println(F("**********************************************************"));
      Serial.println(F("**** WiFi Disconnected.  ESP8266 should auto-reconnect ***"));
      Serial.println(F("**********************************************************"));

      unsigned long w = millis();
      while (WiFi.status() != WL_CONNECTED && millis() - w < 5000) // give it 5 seconds
      {
        yield();
      }
      if (WiFi.status() != WL_CONNECTED)
      {
        Reconnect(); // try with different SSID
      }
    }
    else
    {
      if (millis() - previousMillis > RSSIsendInterval)
      {

        previousMillis = millis();

        if (RSSIsendInterval < RSSI_TX_INTERVAL)
        {
          RSSIsendInterval = RSSIsendInterval + 1000;
        }

        strRSSI = String("rssi ") + String(WiFi.RSSI());
        //send(strRSSI);

        if (_isDebugEnabled)
        {
          Serial.println(strRSSI);
        }
      }
    }

    WiFiClient client = st_server.available();
    if (client) {
      boolean currentLineIsBlank = true;
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          //read char by char HTTP request
          if (readString.length() < 200) {
            //store characters to string
            readString += c;
          }
          else
          {
            if (_isDebugEnabled)
            {
              Serial.println(F(""));
              Serial.println(F("SmartThings.run() - Exceeded 200 character limit"));
              Serial.println(F(""));
            }
          }
          // if you've gotten to the end of the line (received a newline
          // character) and the line is blank, the http request has ended,
          // so you can send a reply
          if (c == '\n' && currentLineIsBlank) {
            //now output HTML data header
            tempString = readString.substring(readString.indexOf('/') + 1, readString.indexOf('?'));

            if (tempString.length() > 0) {
              client.println(F("HTTP/1.1 200 OK")); //send new page
              client.println();
            }
            else {
              client.println(F("HTTP/1.1 204 No Content"));
              client.println();
              client.println();
              if (_isDebugEnabled)
              {
                Serial.println(F("No Valid Data Received"));
              }
            }
            break;
          }
          if (c == '\n') {
            // you're starting a new line
            currentLineIsBlank = true;
          }
          else if (c != '\r') {
            // you've gotten a character on the current line
            currentLineIsBlank = false;
          }
        }
      }

      delay(1);
      //stopping client
      client.stop();

      //Handle the received data after cleaning up the network connection
      if (tempString.length() > 0) {
        if (_isDebugEnabled)
        {
          Serial.print(F("Handling request from ST. tempString = "));
          Serial.println(tempString);
        }
        //Pass the message to user's SmartThings callout function
        tempString.replace("%20", " ");  //Clean up for Hubitat
        _calloutFunction(tempString);
      }

      readString = "";
      tempString = "";
    }
  }

  //*******************************************************************************
  /// Send Message out over Ethernet to the Hub
  //*******************************************************************************
  void SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB::send(String message)

  {
    messageToReSend = message; //back it up to resend it in case of failure




    //Make sure the client is stopped, to free up socket for new conenction
    st_client.stop();
    delay(1);

    if (st_client.connect(st_hubIP, st_hubPort))
    {
      st_client.println(F("POST / HTTP/1.1"));
      st_client.print(F("HOST: "));
      st_client.print(st_hubIP);
      st_client.print(F(":"));
      st_client.println(st_hubPort);
      st_client.println(F("CONTENT-TYPE: text"));
      st_client.print(F("CONTENT-LENGTH: "));
      st_client.println(message.length());
      st_client.println();
      st_client.println(message);

      waitForResponse();

    }
    else {

      Serial.println(F("***********************************************************"));
      Serial.print(st_hubIP); Serial.println(F("*****  CONNECTION FAILED *****"));
      Serial.println(F("***********************************************************"));

    }

    //Make sure the client is stopped, to free up socket for new conenction
    st_client.stop();
    delay(1);
    if (st_client.connect(st_hubIP2, st_hubPortb))
    {

      st_client.println(F("POST / HTTP/1.1"));
      st_client.print(F("HOST: "));
      st_client.print(st_hubIP2);
      st_client.print(F(":"));
      st_client.println(st_hubPortb);
      st_client.println(F("CONTENT-TYPE: text"));
      st_client.print(F("CONTENT-LENGTH: "));
      st_client.println(message.length());
      st_client.println();
      st_client.println(message);

      waitForResponse();

    }
    else {

      Serial.println(F("***********************************************************"));
      Serial.print(st_hubIP); Serial.println(F("*****  CONNECTION FAILED *****"));
      Serial.println(F("***********************************************************"));

    }



    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println(F("***********************************************************"));
      Serial.println(F("**** WiFi Disconnected.  ESP8266 should auto-reconnect ****"));
      Serial.println(F("***********************************************************"));
      ////Reconnect();

      unsigned long w = millis();
      while (WiFi.status() != WL_CONNECTED && millis() - w < 5000) // give it 5 seconds
      {
        yield();
      }
      if (WiFi.status() != WL_CONNECTED)
      {
        Reconnect(); // try with different SSID
      }
    }
  }

  void SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB::waitForResponse(void)
  {
    unsigned long timeout = millis();
    while (!st_client.available())
    {
      //Serial.print(".");
      if (millis() - timeout > 300)
      {
        Serial.println("***************Post request timed out ****************");
        st_client.stop();
        if (resend < 2)
        {
          resend++;
          Serial.print(F("***************RESEND #")); Serial.print(resend);
          send(messageToReSend);

          resendMillis = millis();
        }
        break;
      }
    }
    //if (_isDebugEnabled) { Serial.println(F("WiFi.send(): Reading for reply data "));}
    // read any data returned from the POST
    /*while (st_client.connected())
      {
      //while (st_client.available()) {
      char c = st_client.read(); //gets byte from ethernet buffer
                 //if (_isDebugEnabled) { Serial.print(c); } //prints byte to serial monitor
                 //}
      if((int) c == 255)
      {
      if(_isDebugEnabled)
      Serial.println(F("Breaking due to invalid value"));
      break;
      }
      else if((int) c > 2 && resend >= 2)
      {
      resend = 0;
      Serial.println("RESEND RESET --------------------------------------------------");
      }
      }
    */

    st_client.flush();
    st_client.stop();
    delay(1);
  }

  void SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB::Reconnect(void)
  {
    int count = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
      if (lastSSID == 1) {
        WiFi.begin(st_ssid2, st_password);
        Serial.print(F("USING ALTERNATE SSID: "));
        Serial.println(st_ssid2);
        count = 0;
        lastSSID = 2;
        while (WiFi.status() != WL_CONNECTED && count < 15)
        {
          delay(500); // wait for connection:
          Serial.print(F("."));
          count++;
        }
        if (WiFi.status() != WL_CONNECTED)
        {
          // failed to reconnect to 2nd SSID, restarting
          ESP.reset(); // esp.reset is hard reset
        }
      }
      else { // lost connection to 2nd SSID, connecting back to the 1st one
        WiFi.begin(st_ssid, st_password);
        Serial.print(F("Attempting to RE-connect to WPA SSID: "));
        Serial.println(st_ssid);
        lastSSID = 1;
        count = 0;
        while (WiFi.status() != WL_CONNECTED && count < 15) {
          delay(500); // wait for connection:
          Serial.print(F("."));
          count++;
        }
        if(count >= 15 && WiFi.status() != WL_CONNECTED)
        {
          ESP.reset();
        }
      }
    }
  }
}
