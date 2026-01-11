void webserver() {
  WiFiClient client = st_server.available();
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
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
            //client.println("Refresh: 5");
            //client.println();

            // the content of the HTTP response follows the header:
            client.println("<HTML>");
            client.println("<BODY style='background-color:teal'>");
            client.println("<font style='color:green'>");
            client.println("<font style='font-family:electric toaster'>");
            client.println("<center>");
            client.println("<H1 style='font-size:400%;color:yellow';>COMPUTER OFFICE</H1>");
            client.println("<H4 style='font-size:50%;color:yellow';>OTA CAPABLE</H4>");

            client.println("<br />");
            //client.println(WiFi.localIP());
            client.println("<br />");
            if (isOn()) {
              client.println("<H3 style='font-size:100%;color:yellow';>COMPUTER OFFICE IS RUNNING</H3>");
            }
            else {
              client.println("<H3 style='font-size:100%;color:white';>COMPUTER OFFICE IS OFF</H3>");
            }


            client.println("<strong>");
            client.println("<a href=\"/ON\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>   ON  </button></a>");
            client.println("<br />");
            client.println("<a href=\"/OFF\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>   OFF  </button></a>");
            client.println("<br />");
            client.println("<br />");
            client.println("<a href=\"/SLEEP\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>   SLEEP  </button></a>");
            client.println("<br />");
            client.println("<br />");
            client.println("<a href=\"/R\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   reset controller  </button></a>");
            client.println("<strong>");
            //            client.println("<a href=\"/TEST_1\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>  TEST ON  </button></a>");
            //            client.println("<br />");
            //            client.println("<a href=\"/TEST_0\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>  TEST OFF  </button></a>");
            //            client.println("<br />");

            client.println("</center>");
            client.println(F("<hr />"));
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

        // Check to see if the client request was "GET /H" or "GET /L":

        if (currentLine.endsWith("GET /ON")) {
          On();
        }
        if (currentLine.endsWith("GET /OFF")) {
          Off();
        }
        if (currentLine.endsWith("GET /SLEEP")) {
          Sleep();
        }
        if (currentLine.endsWith("GET /R")) {

          String Message1 = "<a style='font-size:400%;'>This page will refresh in </a>";
          String refreshtime = "<a style='font-size:400%;'>30</a>";
          String Message2 = "<a style='font-size:400%;'> seconds</a>";
          String MessageFinal = Message1 + refreshtime + Message2;


          // the content of the HTTP response follows the header:
          client.println("<HTML>");
          client.println("<BODY style='background-color:black'>");
          client.println("<font style='color:green'>");
          client.println("<font style='font-family:electric toaster'>");
          client.println("<center>");
          client.println("<H1 style='font-size:400%;'>RESETING CONTROLER</H1>");
          client.println();
          client.print(MessageFinal);


          //client.print("<meta http-equiv='refresh' content='10; url=http://philonyc.com/' />");


          String url1 = "<meta http-equiv='refresh' content='20; url=http://";
          String url2 = "'/>";
          String UrlFinal = url1 + WiFi.localIP() + url2 ;
          Serial.println(UrlFinal);
          client.print(UrlFinal);


          client.println();

          client.println("</center>");
          client.println("</strong>");
          client.println("</BODY>");
          client.println("</HTML>");

          client.stop();

          //ESP.restart();
          setup();
          break;
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
  delay(100);
}
