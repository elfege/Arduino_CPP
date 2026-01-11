/*
void webserver () {

  WiFiClient client = server.available();

  if (client) {                             // if you get a client,
    count = 0;
    // Serial << "webserver running on Core " << (xPortGetCoreID()) << endl;
    //    Serial.println("new client");           // print a message out the serial port
    while (client.connected()) {            // loop while the client's connected // !mvt to allow ending the while loop when fwd(x, y) or rvs(x, y) are called
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor

        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:

          if (currentLine.length() == 0) {

            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
            client.print("<HTML>");
            client.print("<BODY style='background-color:blue'>");
            client.print("<font style='color:yellow'>");
            client.print("<font style='font-family:electric toaster'>");
            client.print("<center>");

            if (!logs) {
              client.print("<a href=\"/logs\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  turn on logs </button></a>");
            }
            else {
              client.print("<a href=\"/logs\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  turn off logs </button></a>");
            }
            if (AllowOTA) {
              client.print("<H4 style='font-size:100%;'>OTA ready</H4>");
            }
            else
            {
              client.print("<H4 style='font-size:100%;'>Currently NOT READY for OTA</H4>");
            }

            client.print(F(

                           "<H1 style='font-size:400%;'>MEBO Control</H1>"
                           "<a href=\"/AllowOTA\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  Allow OTA </button></a>"
                           "<br />"
                           "<hr>"
                           "<a href=\"http://192.168.10.105/H\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> -POWER ON- </button></a>"
                           "<a href=\"http://192.168.10.105/L\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> -POWER OFF- </button></a>"
                           "<br />"
                           "<a href=\"/ON\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> CAM ON </button></a>"
                           "<br />"
                           "<a href=\"/OFF\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> CAM OFF </button></a>"
                           "<br />"
                           "</hr>"

                           "<strong>"
                           "<hr>"
                           //                           "<OBJECT classid='clsid:9BE31822-FDAD-461B-AD51-BE1D1C159921' codebase='http://downloads.videolan.org/pub/videolan/vlc/latest/win32/axvlc.cab' width='640' height='480' id='vlc' events='True'>"
                           //                           "<param name='Src' value='rtsp://admin:1ROtrashrisk@192.168.10.181/h264Preview_01_main' />"
                           //                           "<param name='ShowDisplay' value='True' />"
                           //                           "<param name='AutoLoop' value='False' />"
                           //                           "<param name='AutoPlay' value='True' />"
                           //                           "<embed id='vlcEmb' type='application/x-google-vlc-plugin' version='VideoLAN.VLCPlugin.2' autoplay='yes' loop='no' width='640' height='480' target='rtsp://cameraipaddress' ></embed>"
                           //                           "</OBJECT>"
                           //                           "<OBJECT classid='clsid:02BF25D5-8C17-4B23-BC80-D3488ABDDC6B' width='640' height='480' codebase='http://www.apple.com/qtactivex/qtplugin.cab'>"
                           //                           "<param name='src' value='rtsp://admin:1ROtrashrisk@192.168.10.181/h264Preview_01_main'>"
                           //                           "<param name='autoplay' value='true'>"
                           //                           "<param name='controller' value='false'>"
                           //                           "<param name='loop' value='false'>"
                           //                           "<EMBED src='rtsp://admin:1ROtrashrisk@192.168.10.181/h264Preview_01_main' width='640' height='480' autoplay='true'controller='true' loop='false' bgcolor='#000000' pluginspage='http://www.apple.com/quicktime/download/'>"
                           //                           "</EMBED>"
                           //                           "</OBJECT>"

//                           "<object classid='clsid:D27CDB6E-AE6D-11cf-96B8-444553540000'codebase='http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=9,0,16,0'width='2560' height='1440' >"
//                           "<param name='src' value='rtsp://admin:1ROtrashrisk@192.168.10.181/h264Preview_01_main'>"
//                           "<param name='quality' value='high'>"
//                           "<param name='play' value='true'>"
//                           "<param name='LOOP' value='false'>"
//                           "<embed src='rtsp://admin:1ROtrashrisk@192.168.10.181/h264Preview_01_main' width='2560' height='1440' play='true' loop='false' quality='high' pluginspage='http://www.macromedia.com/go/getflashplayer' type='application/x-shockwave-flash'>"
//                           "</embed>"
//                           "</object>"
"<object>"
"<iframe src='http://g1.ipcamlive.com/player/player.php?alias=5c65d22832ef2' width='800px' height='450px'frameborder='0' allowfullscreen></iframe>"
"</object>"
                           "</hr>"
                           "<br />"
                           "<br />"
                           "<a href=\"/FWD\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:50px; position:left; top:530px; left:300px;'>^</button></a>"
                           "<a href=\"/ARMUP\"\"><button style='font-size:170%;margin-left: 100; background-color:darkgray; color:green; border-radius:50px; position:center; top:530px; left:300px;'>^</button></a>"

                           "<br />"
                           "<a href=\"/LEFT\"\"><button style='font-size:170%; margin-left: -50; background-color:darkgray; color:green;border-radius:50px; position:left; top:530px; left:300px;'> < </button></a>"
                           "<a href=\"/RIGHT\"\"><button style='font-size:170%;margin-left: 0; background-color:darkgray; color:green;border-radius:50px; position:left; top:530px; left:300px;'>  > </button></a>"
                           "<a href=\"/WRISTLEFT\"\"><button style='font-size:170%;margin-left: 0; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> < </button></a>"
                           "<a href=\"/WRISTRIGHT\"\"><button style='font-size:170%; margin-left: 50; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> > </button></a>"

                           "<br />"
                           "<a href=\"/RVS\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:left; top:530px; left:300px;'>   V   </button></a>"
                           "<a href=\"/ARMDOWN\"\"><button style='font-size:170%; margin-left: 100; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   V   </button></a>"
                           "<br />"
                           "<a href=\"/STOP\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  STOP </button></a>"
                           "<br />"

                           "<FORM ACTION='/' method=get >" //uses IP/port of web page
                           "SPEED: <INPUT TYPE=TEXT NAME='SPEED' VALUE='' SIZE='25' MAXLENGTH='50'><BR>"
                           "<INPUT TYPE=SUBMIT NAME='submit' VALUE='submit'>"
                           "</FORM>"

                           "<FORM ACTION='/' method=get >" //uses IP/port of web page
                           "DISTANCE: <INPUT TYPE=TEXT NAME='DISTANCE' VALUE='' SIZE='25' MAXLENGTH='50'><BR>"
                           "<INPUT TYPE=SUBMIT NAME='submit' VALUE='submit'>"
                           "</FORM>"

                           "<form method=get action=\"http://192.168.10.222:8888/\">"

                           "<input type='range' name=slider1 min='0' max='255' value='' onchange='this.form.submit()' step='1'>"
                           "<input type='range' name=slider2 min='0' max='1000' value='' onchange='this.form.submit()' step='1'>"
                           "<span id='range1'>0</span>"
                           "<script type='text/javascript'>\r\nfunction showValue1(newValue)\r\n{\r\ndocument.getElementById('range1').innerHTML=newValue;\r\n}\r\n</script>\r\n"
                           "<script type=!text/javascript!></script\r\n>"
                           "</form>"

                           "<br />"
                           "<a href=\"/cancelSprq\"\"><button style='font-size:170%; background-color:darkgray; color:blue;border-radius:50px; position:center; top:530px; left:300px;'>  Cancel Speed Request </button></a>"
                           "<br />"

                           "<hr>"
                           "<br />"
                           "<a href=\"/AUTO\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> Self Driving </button></a>"
                           "<a href=\"/ENDdrive\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  STOP ALL </button></a>"
                           "<a href=\"/SearchStation\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> Search Station </button></a>"

                           "<a href=\"/FollowLineFWD\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  Follow The Line FWD </button></a>"
                           "<a href=\"/cancelSTOPALL\"\"><button style='font-size:170%; background-color:darkgray; color:blue;border-radius:50px; position:center; top:530px; left:300px;'> Cancel STOPALL </button></a>"

                           //"<a href=\"/FollowLineRVS\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  Follow The Line RVS </button></a>"

                           "<br />"
                           "</hr>"
                           "<hr>"
                           "<a href=\"/pureFD\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  Simple FWD </button></a>"
                           "<br />"
                           "<a href=\"/simpleRV\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  Simple RVS </button></a>"
                           "<br />"
                           "<a href=\"/disableCliff\"\"><button style='font-size:170%; background-color:darkgray; color:blue;border-radius:50px; position:center; top:530px; left:300px;'>  Disable Cliff Sensors </button></a>"
                           "<br />"
                           "<a href=\"/charge\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  charge battery </button></a>"
                           "<a href=\"/stopcharge\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  stop charge </button></a>"
                           "<a href=\"/dock\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> DOCK </button></a>"
                           "<br />"
                           "</hr>"
                           "<hr>"
                           "<br />"
                           "<a href=\"/init\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   reset controller  </button></a>"
                           "</hr>"
                           "<hr>"
                           "<a href=\"/RunATest\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  TEST </button></a>"
                           "<br />"
                           "<a href=\"/cliffOFF\"\"><button style='font-size:170%; background-color:darkgray; color:blue;border-radius:50px; position:center; top:530px; left:300px;'>  CLIFF OFF </button></a>"
                           "<br />"

                           "</hr>"
                         )); // end of F macro

            client.println("</center>");
            client.println("</strong>");
            client.println("</BODY>");
            client.println("</HTML>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check client request
        //Serial << "READING COMMAND: =============================== " << currentLine << endl;

        if ((currentLine.startsWith("GET /?slider1=")) || (currentLine.startsWith("GET /?SPEED=") && currentLine.endsWith("submit"))) {

          // Length (with one extra character for the null terminator)
          int str_len = currentLine.length() + 1;

          // Prepare the character array (the buffer)
          char char_array[str_len];

          // Copy it over
          currentLine.toCharArray(char_array, str_len);
          //Serial << "currentLine char =============================== " << currentLine << endl;


          int i = 0;
          int s = str_len; // sizeof(currentLine);
          //Serial << "s =============================== " << s << endl;
          String New = "";
          /// 255 as string = 23 bytes
          for ( s > 0; i <= 23; i++) {
            if (isDigit(currentLine[i])) {
              New += currentLine[i];
              //Serial << "New =============================== " << New << endl;
            }
          }
          int n = New.toInt();  //convert String into a number
          if (n <= 200) {
            HttpReq = true; // tell future fwd and rvs cmds to keep this value as of now
            rqSp = n; // new speed value
            //            Serial << "New to int =============================== " << n << endl;
            //            Serial << "New rqSp =============================== " << rqSp << endl;
          }
          else {
            // FASTER THAN 200 might burn the motors!!!!
            HttpReq = true; // tell future fwd and rvs cmds to keep this value as of now
            rqSp = 200; // new speed value
          }
        }
        if ((currentLine.startsWith("GET /?slider2=")) || (currentLine.startsWith("GET /?DISTANCE=") && currentLine.endsWith("submit"))) {

          // Length (with one extra character for the null terminator)
          int str_len = currentLine.length() + 1;

          // Prepare the character array (the buffer)
          char char_array[str_len];

          // Copy it over
          currentLine.toCharArray(char_array, str_len);
          Serial << "currentLine char =============================== " << currentLine << endl;


          int i = 0;
          int s = str_len; // sizeof(currentLine);
          Serial << "s =============================== " << s << endl;
          String New = "";
          ///
          for ( s > 0; i <= 50; i++) {
            if (isDigit(currentLine[i])) {
              New += currentLine[i];
              //Serial << "New =============================== " << New << endl;
            }
          }
          int n = New.toInt();  //convert String into a number

          HttpDistReq = true;
          httpdist = n; // new speed value
          Serial << "New to int =============================== " << n << endl;
          Serial << "New httpdist =============================== " << httpdist << endl;


        }

        if (currentLine.equals("GET /disableCliff")) {
          disableCliff = true;
          cliffOFF = false;
        }
        if (currentLine.equals("GET /cliffOFF")) {
          cliffOFF = true;
        }

        if (currentLine.equals("GET /FWD")) {

          fwd(MEDIUM, defaultDist);
        }
        if (currentLine.equals("GET /RVS")) {

          rvs(MEDIUM, defaultDist);

        }
        if (currentLine.equals("GET /STOP")) {
          STOP();

        }
        if (currentLine.equals("GET /ON")) {
          meboOn();

        }
        if (currentLine.equals("GET /OFF")) {
          meboOff();

        }
        if (currentLine.equals("GET /RunATest")) {
          runATest();

        }
        if (currentLine.equals("GET /LEFT")) {
          left(MEDIUM);

        }
        if (currentLine.equals("GET /RIGHT")) {
          right(MEDIUM);
        }
        if (currentLine.equals("GET /AUTO")) {
          selfdrive();
        }
        if (currentLine.equals("GET /ENDdrive")) {
          STOPALL = true;
          STOP();
          STOPALL = true;
          STOP();
        }
        if (currentLine.equals("GET /pureFD")) {
          fwd(MEDIUM, 0);
        }
        if (currentLine.equals("GET /simpleRV")) {
          rvs(MEDIUM, 0);
        }
        if (currentLine.equals("GET /charge")) {
          charge();
        }
        if (currentLine.equals("GET /stopcharge")) {
          stopcharge();
        }
        if (currentLine.equals("GET /dock")) {
          dock();
        }
        if (currentLine.equals("GET /cancelSprq")) {
          HttpReq = false;
        }
        if (currentLine.equals("GET /cancelSTOPALL")) {
          STOPALL = false;
        }

        if (currentLine.equals("GET /SearchStation")) {
          SearchStation(15000, 1500, 25, 100);
        }
        if (currentLine.equals("GET /FollowLineFWD")) {
          Serial << "Follow FWD command received " << endl;
          FollowLineFWD();
        }

        if (currentLine.equals("GET /AllowOTA")) {
          AllowOTA = true;
        }
        if (currentLine.equals("GET /logs")) {
          if (logs) {
            logs = false;
          }
          else {
            logs = true;
          }
        }
        if (currentLine.equals("GET /init")) {
          doReset = true;// do not run it from here otherwhise breaks page
          RstCtDwnStart = millis();
        }
      }
      count++;
      if (count > 500) {
        //        Serial << "BREAK " << endl;
        break;
      }
    }
    //Serial << "rqSp =========NNN============ " << rqSp << endl;
    //stopping client
    client.stop();
    //Serial.println("client disonnected");
  }
}

*/
