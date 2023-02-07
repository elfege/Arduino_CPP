// Client variables
char linebuf[80];
int charcount = 0;


void webserver() {
  EthernetClient client = _server.available();

  if (client) {                             // if you get a client,
    String currentLine = "";
    //Serial.println("new client");
    memset(linebuf, 0, sizeof(linebuf));
    charcount = 0;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {

        char c = client.read();
        //read char by char HTTP request
        linebuf[charcount] = c;
        //Serial.write(c);
        if (charcount < sizeof(linebuf) - 1) charcount++;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          dashboardPage(client);
          break;
        }
        if (c == '\n') {

          // Check requests:
          //          Serial.println("READING COMMAND: "); Serial.print(currentLine);


          if (strstr(linebuf, "GET /powerOn") > 0) {
            On();
          }
          if (strstr(linebuf, "GET /off") > 0) {
            Off();
          }
          if (strstr(linebuf, "GET /deviceoff") > 0) {
            Serial.println("RECEIVED deviceoff string");
            pwstate = "off"; // received from ESP
            Refresh();// update hub
          }
          if (strstr(linebuf, "GET /deviceon") > 0) {
            Serial.println("RECEIVED deviceon string");
            pwstate = "on"; // received from ESP
            Refresh(); // update hub
          }
          if (strstr(linebuf, "GET /refresh") > 0) {
            //Serial.println("Sending HTTP GET");
            sendGET("refresh");
          }
          if (strstr(linebuf, "GET /noopen") > 0) {
            testRun = true;
          }
          if (strstr(linebuf, "GET /debug") > 0) {
            debug = !debug;
          }
          else if (strstr(linebuf, "GET /canceltest") > 0) {
            testRun = false;
            millisStop = millis();
          }
          else if (strstr(linebuf, "GET /holdpwithcompressors") > 0) {
            holdCanWithCompressors();
            millisStop = millis();
          }
          else if (strstr(linebuf, "GET /compresscan") > 0) {
            compressCan(true);
            millisStop = millis();
          }
          else if (strstr(linebuf, "GET /decompresscan") > 0) {
            decompressCan();
          }
          else if (strstr(linebuf, "GET /load") > 0) {
            loadAcan();
          }
          else if (strstr(linebuf, "GET /unload") > 0) {
            unload();
          }
          else if (strstr(linebuf, "GET /lock") > 0) {
            lock();
          }
          else if (strstr(linebuf, "GET /unlock") > 0) {
            unlock();
          }
          else if (strstr(linebuf, "GET /opencan") > 0) {
            openCan();
          }
          else if (strstr(linebuf, "GET /canopdown") > 0) {
            canOpDown();
          }
          else if (strstr(linebuf, "GET /armplus") > 0) {
            armPlus();
          }
          else if (strstr(linebuf, "GET /armminus") > 0) {
            armMinus();
          }
          else if (strstr(linebuf, "GET /canopup") > 0) {
            canOpUp();
          }
          else if (strstr(linebuf, "GET /retractlowerpusher") > 0) {
            Serial.println("linebuf, 'GET /retractlowerpusher'");
            retractlowerpusher();
          }
          else if (strstr(linebuf, "GET /pushlowerpusher") > 0) {
            Serial.println("linebuf, 'GET /pushlowerpusher'");
            pushlowerpusher();
          }
          else if (strstr(linebuf, "GET /fullpushlowerpusher") > 0) {
            Serial.println("linebuf, 'GET /fullpushlowerpusher'");
            fullPushLowerPusher();
          }
          else if (strstr(linebuf, "GET /fullyretractlowerpusher") > 0) {
            Serial.println("linebuf, 'GET /fullyretractlowerpusher'");
            fullyRetractLowerPusher();
          }
          else if (strstr(linebuf, "GET /lpplus") > 0) {
            Serial.println("linebuf, 'GET /lpplus'");
            lpplus();
          }
          else if (strstr(linebuf, "GET /uhplus") > 0) {
            Serial.println("linebuf, 'GET /uhplus'");
            uhplus();
          }

          else if (strstr(linebuf, "GET /fullretractupperholder") > 0) {
            Serial.println("linebuf, 'GET /fullretact'");
            fullRetractUpperHolder();
          }


          else if (strstr(linebuf, "GET /retractupperholder") > 0) {
            Serial.println("linebuf, 'GET /retractupperholder'");
            retractupperholder();
          }
          else if (strstr(linebuf, "GET /pushupperholder") > 0) {
            Serial.println("linebuf, 'GET /pushupperholder'");
            pushupperholder();
          }


          else if (strstr(linebuf, "GET /closegate") > 0) {
            closegate();
          }
          else if (strstr(linebuf, "GET /opengate") > 0) {
            opengate();
          }
          else if (strstr(linebuf, "GET /gatehold") > 0) {
            gateholdcan();
          }
          else if (strstr(linebuf, "GET /run") > 0) {
            RUN();
          }
          else if (strstr(linebuf, "GET /stop") > 0) {
            millisStop = millis();
            STOP = true;
          }
          else if (strstr(linebuf, "GET /C1push") > 0) {
            c1.write(180);
          }
          else if (strstr(linebuf, "GET /C1retract") > 0) {
            c1.write(0);
          }
          else if (strstr(linebuf, "GET /C2push") > 0) {
            c2.write(180);
          }
          else if (strstr(linebuf, "GET /C2retract") > 0) {
            c2.write(0);
          }
          else if (strstr(linebuf, "GET /testload") > 0) {
            testLoadAllCans();
          }
          else if (strstr(linebuf, "GET /simpleopencan") > 0) {
            simpleopencan();
          }
          else if (strstr(linebuf, "GET /canreceiverflat") > 0) {
            Serial.println("receiver flat cmd");
            canreceiverflat();
          }
          else if (strstr(linebuf, "GET /canreceiverup") > 0) {
            Serial.println("receiver up cmd");
            canreceiverup();
          }
          else if (strstr(linebuf, "GET /canreceivertrash") > 0) {
            Serial.println("trash cmd");
            canreceivertrash();
          }
          else if (strstr(linebuf, "GET /openlidremover") > 0) {
            Serial.println("openlidremover");
            openlidremover();
          }
          else if (strstr(linebuf, "GET /forceremovecan") > 0) {
            Serial.println("forceremovecan");
            forceRemoveCan();
          }
          else if (strstr(linebuf, "GET /removeLid") > 0) {
            Serial.println("removeLid");
            detachTheLid();
          }
          else if (strstr(linebuf, "GET /extractcan") > 0) {
            Serial.println("extractcan");
            extractcan();
          }
          else if (strstr(linebuf, "GET /logs") > 0) {
            logs = !logs;
          }


          else if (strstr(linebuf, "GET /reset") > 0) {
            Reset();
            return;
          }


          //you're starting a new line
          currentLineIsBlank = true;
          memset(linebuf, 0, sizeof(linebuf));
          charcount = 0;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
//    Serial.println("client disconnected");
  }
}

void dashboardPage(EthernetClient & client)
{

  //Serial.println("new client");           // print a message out the serial port
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.print("<HTML>");
  client.print("<center>");
  client.print("<H3 style='font-size:150%;'>CAN OPENER</H3>");
  client.print("<p>");
  client.print("<p>");
  if (testRun)
  {
    client.print("<H1 style='font-size:150%;'>***TEST MODE***</H1>"); client.print("<p>");
    client.print("<input type=submit value='RUN TEST' style=width:160px;background-color:blue;color:yellow;height:40px onClick=location.href='/run'>");
    client.print("<input type=submit value='DISABLE TEST' style=width:160px;background-color:blue;color:yellow;height:40px onClick=location.href='/canceltest'>");
  }
  else {
    client.print("<input type=submit value='RUN' style=width:160px;background-color:blue;color:yellow;height:40px onClick=location.href='/run'>");
    client.print("<input type=submit value='ENABLE TEST MODE (no opening)' style=width:300px;background-color:green;color:black;height:40px onClick=location.href='/noopen'>");
  }
  client.print("<input type=submit value='REFRESH' style=width:160px;background-color:grey;color:black;height:40px onClick=location.href='/refresh'>");
  if (debug)
  {
    client.print("<input type=submit value='DEBUG ENABLED' style=width:200px;background-color:grey;color:black;height:40px onClick=location.href='/debug'>");
  }
  else
  {
    client.print("<input type=submit value='DEBUG DISABLED' style=width:200px;background-color:grey;color:black;height:40px onClick=location.href='/debug'>");
  }
  client.print("<input type=submit value='TEST LOAD ALL CANS' style=width:160px;height:30px onClick=location.href='/testload'>");
  client.print(F("<hr>"));

  client.print(F("<input type=submit value='SWITCH ON ' style=width:160px;height:50px;color:red onClick=location.href='/powerOn'>"));
  client.print(F("<input type=submit value='SWITCH OFF' style=width:160px;height:50px;color:red onClick=location.href='/off'>"));
  client.print(F("<input type=submit value='STOP!' style=width:160px;height:50px;background-color:red;color:yellow onClick=location.href='/stop'>"));
  client.print(F("<input type=submit value='RESET' style=width:160px;height:50px;background-color:red;color:yellow onClick=location.href='/reset'>"));
  client.print("</p>");
  client.print("</center>");
  client.print("</head>");
  client.print("<BODY style='background-color:grey'>");
  client.print("<font style='color:black'>");
  client.print("<font style='font-family:electric toaster'>");
  client.print("<strong>");
  if (!isUnlocked()) {
    client.println("<H3 style='font-size:100%;'>CAN OPENER IS LOCKED </H3>");
  }
  else {
    client.println("<H3 style='font-size:100%;'>CAN OPENER IS UNLOCKED </H3>");
  }
  if (digitalRead(power) == 0) {
    client.println("<H3 style='font-size:100%;color:blue;'>CAT FEEDER IS ON</H3>");
  }
  else {
    client.println("<H3 style='font-size:100%;color:black;'>CAT FEEDER IS OFF</H3>");
  }


  client.println(" Gate Position = ");
  client.print(gate.read());
  client.println(" Arm Position = ");
  client.print(canopArm.read());
  client.println(" Upper Holder Position = ");
  client.print(uh.read());
  client.println(" Lower Pusher/Holder Position = ");
  client.print(lp.read());

  client.print(F("<p>")); client.print(F("</p>"));
  client.print(F("<hr>"));
  client.print(F("<input type=submit value='CLOSE GATE' style=width:160px;height:30px onClick=location.href='/closegate'>"));
  client.print(F("<input type=submit value='OPEN GATE' style=width:160px;height:30px onClick=location.href='/opengate'>"));
  client.print(F("<input type=submit value='GATE HOLD' style=width:160px;height:30px onClick=location.href='/gatehold'>"));
  client.print(F("</hr>"));
  client.print(F("<hr>"));
  client.print(F("<input type=submit value='LOAD' style=width:120px;height:30px onClick=location.href='/load'>"));

  client.print(F("<input type=submit value='LOCK' style=width:120px;height:30px onClick=location.href='/lock'>"));
  client.print(F("<input type=submit value='OPEN A CAN' style=width:120px;height:30px onClick=location.href='/opencan'>"));

  client.print(F("<input type=submit value='UNLOCK' style=width:120px;height:30px onClick=location.href='/unlock'>"));
  client.print(F("<input type=submit value='UNLOAD' style=width:120px;height:30px onClick=location.href='/unload'>"));
  client.print(F("<p>"));
  client.print(F("<input type=submit value='LIFT' style=width:120px;height:30px onClick=location.href='/canopup'>"));
  client.print(F("<input type=submit value='PUT DOWN' style=width:120px;height:30px onClick=location.href='/canopdown'>"));
  client.print(F("<input type=submit value='+' style=width:30px;height:30px onClick=location.href='/armplus'>"));
  client.print(F("<input type=submit value='-' style=width:30px;height:30px onClick=location.href='/armminus'>"));
  client.print(F("</p>"));
  client.print(F("<input type=submit value='REMOVE LID' style=width:120px;height:30px onClick=location.href='/removeLid'>"));
  client.print(F("<input type=submit value='SIMPLE OPEN CAN' style=width:120px;height:30px onClick=location.href='/simpleopencan'>"));

  /*client.print(F("<p>"));
    client.print(F("<input type=submit value='PUSH C1' style=width:160px;height:30px onClick=location.href='/C1push'>"));
    client.print(F("<input type=submit value='RETRACT C1' style=width:160px;height:30px onClick=location.href='/C1retract'>"));
    client.print(F("<input type=submit value='PUSH C2' style=width:160px;height:30px onClick=location.href='/C2push'>"));
    client.print(F("<input type=submit value='RETRACT C2' style=width:160px;height:30px onClick=location.href='/C2retract'>"));
    client.print(F("</p>"));
  */
  client.print(F("<p>"));
  client.print(F("<input type=submit value='RETRACT Upper Holder' style=width:160px;height:30px onClick=location.href='/retractupperholder'>"));
  client.print(F("<input type=submit value='PUSH Upper Holder' style=width:160px;height:30px onClick=location.href='/pushupperholder'>"));
  client.print(F("<input type=submit value='+' style=width:30px;height:30px onClick=location.href='/uhplus0'>"));
  client.print(F("<input type=submit value='FULL RETRACT (!)' style=width:160px;height:30px onClick=location.href='/fullretractupperholder'>"));
  client.print(F("</p>"));
  client.print(F("<p>"));
  client.print(F("<input type=submit value='RETRACT Lower Pusher' style=width:160px;height:30px onClick=location.href='/retractlowerpusher'>"));
  client.print(F("<input type=submit value='PUSH Lower Pusher' style=width:160px;height:30px onClick=location.href='/pushlowerpusher'>"));
  client.print(F("<input type=submit value='FULL PUSH' style=width:160px;height:30px onClick=location.href='/fullpushlowerpusher'>"));
  client.print(F("<input type=submit value='+' style=width:30px;height:30px onClick=location.href='/lpplus'>"));
  client.print(F("<input type=submit value='FULL RETRACT' style=width:160px;height:30px onClick=location.href='/fullyretractlowerpusher'>"));

  client.print(F("<p>"));
  client.print(F("<input type=submit value='EXTRACT CAN' style=width:160px;background-color:blue;color:yellow;height:30px onClick=location.href='/extractcan'>"));
  client.print(F("<input type=submit value='COMPRESS CAN (with lp)' style=width:relative;background-color:blue;color:yellow;height:30px onClick=location.href='/compresscan'>"));
  client.print(F("<input type=submit value='HOLD CAN WITH COMP' style=width:relative;background-color:blue;color:yellow;height:30px onClick=location.href='/holdpwithcompressors'>"));
  client.print(F("<input type=submit value='DECOMPRESS CAN' style=width:160px;background-color:blue;color:yellow;height:30px onClick=location.href='/decompresscan'>"));
  
  client.print(F("</p>"));



  client.print(F("<hr/>"));
  client.print(F("<input id = 'servo' type = 'range' min = 0 max = 180 onchange = 'sendPosition(this.id, this.value);'>"));


  client.print(F("</strong>"));
  client.print(F("</BODY>"));
  client.print(F("</HTML>"));

}


void sendGET(String val) //client function to send
{
  EthernetClient client;
  byte myserver[] = {192, 168, 10, 241};

  if (client.connect(myserver, 80)) {  //starts client connection, checks for connection

    Serial.println("Sending packet: 'Get/" + String(val) + "'");
    client.println("GET /" + String(val) + " HTTP/1.0"); //+ String(val)); //download text
    client.println(); //end of get request
  }
  else {
    Serial.println("connection failed"); //error message if no client connect
    Serial.println();
  }

  String str = "";
  unsigned long St = millis();
  while (!client.available() && millis() - St < 4000)
  {
    delay(1);
  }
  if (client.connected() && !client.available())
  {
    Serial.println("CLIENT DIDN'T RESPOND");
  }
  while (client.available()) { //connected or data available
    char c = client.read(); //gets byte from ethernet buffer
    Serial.print(c); //prints byte to serial monitor
    str += c;
  }
  Serial.print("RECEIVED STRING: "); Serial.println(str);

  client.stop(); //stop client
  Serial.println("sendGet client disconnected");
  Serial.println("==================");
}
