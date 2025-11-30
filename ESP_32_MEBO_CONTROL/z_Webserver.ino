


void webserver () {

  WiFiClient client = server.available();

  if (client) {                             // if you get a client,
    // Serial << "webserver running on Core " << (xPortGetCoreID()) << endl;
    // serverAtWork = true; // prevents overlap of webserver() loop
    //Blink(10, 50);
    //Serial.println("new client");           // print a message out the serial port

    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            // refresh the page automatically every 1 sec
            //            client.println("Refresh: 1");
            //            client.println();

            // the content of the HTTP response follows the header:
            client.println("<HTML>");
            client.println("<BODY style='background-color:black'>");
            client.println("<font style='color:yellow'>");
            client.println("<font style='font-family:electric toaster'>");
            client.println("<center>");
            client.println("<H1 style='font-size:400%;'>MEBO Control</H1>");

            client.println("<a href=\"/AllowOTA\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  Allow OTA </button></a>");


            if (!logs) {
              client.println("<a href=\"/logs\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  turn on logs </button></a>");
            }
            else {
              client.println("<a href=\"/logs\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  turn off logs </button></a>");
            }
            if (AllowOTA) {
              client.println("<H4 style='font-size:100%;'>OTA ready</H4>");
            }
            else
            {
              client.println("<H4 style='font-size:100%;'>Currently NOT READY for OTA</H4>");
            }
            

            client.println("<br />");
            //client.print("<H4 style='font-size:100%;'>N val = </H4>"); client.print(N);
            //            client.println("<br />");
            //            client.print("<H4 style='font-size:100%;'>FIR val = </H4>"); client.print(analogRead(FIR));
            //            client.println("<br />");
            //            client.print("<H4 style='font-size:100%;'>RRIR val = </H4>"); client.print(analogRead(RRIR));
            //            client.println("<br />");
            //            client.print("<H4 style='font-size:100%;'>PW val = </H4>"); client.print(analogRead(PW));
            //            client.println("<br />");
            //            client.print("<H4 style='font-size:100%;'>bumberPin val = </H4>"); client.print(digitalRead(bumberPin));
            //            client.println("<br />");
            //            client.print("<H4 style='font-size:100%;'>US distance val = </H4>"); client.print(digitalRead(bumberPin));

            client.println("<hr>");
            client.println("<a href=\"/ON\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  ON </button></a>");
            client.println("<br />");
            client.println("<a href=\"/OFF\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  OFF </button></a>");
            client.println("<br />");
            client.println("</hr>");
            client.println("<hr>");
            client.println("<strong>");
            client.println("<a href=\"/FWD\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:50px; position:center; top:530px; left:300px;'>^</button></a>");
            client.println("<br />");
            client.println("<a href=\"/LEFT\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> < </button></a>");
            client.println("<a href=\"/RIGHT\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  > </button></a>");
            client.println("<br />");
            client.println("<a href=\"/RVS\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   V   </button></a>");
            client.println("<br />");
            client.println("<a href=\"/STOP\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  STOP </button></a>");
            client.println("<br />");
            client.println("<hr>");
            client.println("<br />");
            client.println("<a href=\"/AUTO\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> Self Driving </button></a>");
            client.println("<a href=\"/ENDdrive\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  STOP DRIVE </button></a>");
            client.println("<a href=\"/SearchStation\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> Search Station </button></a>");
             client.println("<a href=\"/CaptureTheLineRVS\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  Capture A Line </button></a>");

            client.println("<a href=\"/FollowLineFWD\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  Follow The Line FWD </button></a>");
            client.println("<a href=\"/FollowLineRVS\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  Follow The Line RVS </button></a>");

            client.println("<br />");
            client.println("</hr>");
            client.println("<hr>");
            client.println("<a href=\"/pureFD\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  Simple FWD </button></a>");
            client.println("<br />");
            client.println("<a href=\"/simpleRV\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  Simple RVS </button></a>");
            client.println("<br />");
            client.println("<a href=\"/charge\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  charge battery </button></a>");
            client.println("<a href=\"/stopcharge\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  stop charge </button></a>");
            client.println("<a href=\"/dock\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> DOCK </button></a>");
            client.println("<br />");
            client.println("</hr>");
            client.println("<hr>");
            client.println("<a href=\"/POWERON\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> TURN ON </button></a>");
            client.println("<a href=\"http://192.168.10.105/L\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> -TURN OFF- </button></a>");
            client.println("<a href=\"/POWEROFF\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'> TURN OFF </button></a>");
            client.println("<br />");
            client.println("<a href=\"/init\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   reset controller  </button></a>");
            client.println("</hr>");
            client.println("<hr>");
            client.println("<a href=\"/RunATest\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  TEST </button></a>");

            client.println("</center>");
            client.println("</strong>");
            client.println("</BODY>");
            client.println("</HTML>");

            client.println("</hr>");
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;

          }
          else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check client request
        if (currentLine.equals("GET /FWD")) {

          fwd(SLOW, defaultDist);
        }
        if (currentLine.equals("GET /RVS")) {

          rvs(SLOW, defaultDist);

        }
        if (currentLine.equals("GET /STOP")) {
          STOP();

        }
        if (currentLine.equals("GET /ON")) {
          poweron();

        }
        if (currentLine.equals("GET /OFF")) {
          poweroff();

        }
        if (currentLine.equals("GET /RunATest")) {
          followRvs(25, 200);

        }
        if (currentLine.equals("GET /LEFT")) {
          left(FAST);

        }
        if (currentLine.equals("GET /RIGHT")) {
          right(FAST);
        }
        if (currentLine.equals("GET /AUTO")) {
          selfdrive();
        }
        if (currentLine.equals("GET /ENDdrive")) {
          STOPALL = true;
          STOP();
        }
        if (currentLine.equals("GET /pureFD")) {
          fwd(SLOW, 0);
        }
        if (currentLine.equals("GET /simpleRV")) {
          rvs(SLOW, 0);
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


        if (currentLine.equals("GET /SearchStation")) {
          SearchStation();
        }
        if (currentLine.equals("GET /FollowLineFWD")) {
          FollowLineFWD();
        }
        if (currentLine.equals("GET /FollowLineRVS")) {
          FollowLineRVS();
        }
        if (currentLine.equals("GET /CaptureTheLineRVS")) {
          CaptureTheLineRVS();
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

        if (currentLine.equals("GET /POWERON")) {

          String IP = WiFi.localIP().toString();// to avoid backward printed IP...

          String Message1 = "<a style='font-size:400%;'>This page will refresh in </a>";
          String refreshtime = "<a style='font-size:400%;'>5</a>";
          String Message2 = "<a style='font-size:400%;'> seconds</a>";
          String MessageFinal = Message1 + refreshtime + Message2;


          // the content of the HTTP response follows the header:
          client.println("<HTML>");
          client.println("<BODY style='background-color:black'>");
          client.println("<font style='color:green'>");
          client.println("<font style='font-family:electric toaster'>");
          client.println("<center>");
          client.println("<H1 style='font-size:400%;'>SHUTTING OFF MAIN POWER</H1>");
          client.println();
          client.print(MessageFinal);

          String url1 = "<meta http-equiv='refresh' content='0; url=http://";
          String url2 = "192.168.10.105/H";
          String url3 = "'/>";
          String UrlFinal = url1 + url2 + url3 ;
          Serial.println(UrlFinal);
          client.print(UrlFinal);


          client.println();

          client.println("</center>");
          client.println("</strong>");
          client.println("</BODY>");
          client.println("</HTML>");

          //client.stop();
        }
        if (currentLine.equals("GET /POWEROFF")) {

          String IP = WiFi.localIP().toString();// to avoid backward printed IP...

          String Message1 = "<a style='font-size:400%;'>This page will refresh in </a>";
          String refreshtime = "<a style='font-size:400%;'>5</a>";
          String Message2 = "<a style='font-size:400%;'> seconds</a>";
          String MessageFinal = Message1 + refreshtime + Message2;


          // the content of the HTTP response follows the header:
          client.println("<HTML>");
          client.println("<BODY style='background-color:black'>");
          client.println("<font style='color:green'>");
          client.println("<font style='font-family:electric toaster'>");
          client.println("<center>");
          client.println("<H1 style='font-size:400%;'>SHUTTING OFF MAIN POWER</H1>");
          client.println();
          client.print(MessageFinal);

          String url1 = "<meta http-equiv='refresh' content='0; url=http://";
          String url2 = "192.168.10.105/L";
          String url3 = "'/>";
          String UrlFinal = url1 + url2 + url3 ;
          Serial.println(UrlFinal);
          client.print(UrlFinal);


          client.println();

          client.println("</center>");
          client.println("</strong>");
          client.println("</BODY>");
          client.println("</HTML>");

          //client.stop();

        }
        if (currentLine.endsWith("GET /init")) {
          doReset = true;// do not run it from here otherwhise breaks page
        }
      }
      //Serial.println("");
      //Serial.print("webserver loop ");Serial.println(count);
      count++;
      if (count > 1000) {
        Serial.println("");
        Serial.println("BREAK");
        break;
      }
    }
    serverAtWork = false;
    count = 0;
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

