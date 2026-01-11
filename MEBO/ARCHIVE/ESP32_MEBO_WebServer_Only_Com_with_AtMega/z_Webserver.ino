

void webserver()
{
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          Serial.println("Sending response");

          // send a standard http response header
          // use \r\n instead of many println statements to speedup data send
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"  // the connection will be closed after completion of the response
          
            "\r\n");
             // "Refresh: 20\r\n"        // refresh the page automatically every 20 sec
          client.print("<!DOCTYPE HTML>\r\n");
          client.print("<html>\r\n");
          client.print("<BODY style='background-color:black'>");
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
                         "<a href=\"/ON\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  ON </button></a>"
                         "<br />"
                         "<a href=\"/OFF\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  OFF </button></a>"
                         "<br />"
                         "</hr>"
                         "<hr>"
                         "<strong>"
                         "<a href=\"/FWD\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:50px; position:center; top:530px; left:300px;'>^</button></a>"
                         "<br />"
                         "<a href=\"/LEFT\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> < </button></a>"
                         "<a href=\"/RIGHT\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  > </button></a>"
                         "<br />"
                         "<a href=\"/RVS\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   V   </button></a>"
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
                         "<a href=\"/POWERON\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> TURN ON </button></a>"
                         "<a href=\"http://192.168.10.105/L\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> -TURN OFF- </button></a>"
                         "<a href=\"/POWEROFF\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> TURN OFF </button></a>"
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

          client.print("<br>\r\n");
          client.print("</html>\r\n");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
          currentLine += c;
        }

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
          //Serial << "s =============================== " << s << endl;
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
          //Serial << "New to int =============================== " << n << endl;
          //Serial << "New httpdist =============================== " << httpdist << endl;


        }

        if (currentLine.equals("GET /disableCliff")) {
          disableCliff = true;
          cliffOFF = false;
        }
        if (currentLine.equals("GET /cliffOFF")) {
          cliffOFF = true;
        }
        if (currentLine.equals("GET /FWD")) {
          //fwd(MEDIUM, defaultDist);
        }
        if (currentLine.equals("GET /RVS")) {
          //rvs(MEDIUM, defaultDist);
        }
        if (currentLine.equals("GET /STOP")) {
          // STOP();
        }
        if (currentLine.equals("GET /ON")) {
          poweron();
        }
        if (currentLine.equals("GET /OFF")) {
          poweroff();
        }
        if (currentLine.equals("GET /RunATest")) {
          // runATest();
        }
        if (currentLine.equals("GET /LEFT")) {
          // left(MEDIUM);
        }
        if (currentLine.equals("GET /RIGHT")) {
          //right(MEDIUM);
        }
        if (currentLine.equals("GET /AUTO")) {
          //selfdrive();
        }
        if (currentLine.equals("GET /ENDdrive")) {
          STOPALL = true;
          STOP();
        }
        if (currentLine.equals("GET /pureFD")) {
          //fwd(MEDIUM, 0);
        }
        if (currentLine.equals("GET /simpleRV")) {
          //rvs(MEDIUM, 0);
        }
        if (currentLine.equals("GET /charge")) {
          //charge();
        }
        if (currentLine.equals("GET /stopcharge")) {
          // stopcharge();
        }
        if (currentLine.equals("GET /dock")) {
          //dock();
        }
        if (currentLine.equals("GET /cancelSprq")) {
          HttpReq = false;
        }
        if (currentLine.equals("GET /cancelSTOPALL")) {
          STOPALL = false;
        }
        if (currentLine.equals("GET /SearchStation")) {
          // SearchStation(15000, 1500, 25, 100);
        }
        if (currentLine.equals("GET /FollowLineFWD")) {
          //Serial << "Follow FWD command received " << endl;
          //FollowLineFWD();
        }
        if (currentLine.equals("GET /AllowOTA")) {
          //AllowOTA = true;
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
    }
    // give the web browser time to receive the data
    delay(10);

    // close the connection:
    client.stop();
    Serial.println("Client disconnected");
  }
}
