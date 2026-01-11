void  webserver() {
  WiFiClient client = server.available();
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
            client.println("Refresh: 1");
            client.println();

            // the content of the HTTP response follows the header:
            client.println("<HTML>");
            client.println("<BODY style='background-color:black'>");
            client.println("<font style='color:green'>");
            client.println("<font style='font-family:electric toaster'>");
            client.println("<center>");
            client.println("<H1 style='font-size:400%;'>NEATO Vacuum Control</H1>");
            client.println("<H4 style='font-size:100%;'>OTA update capable</H4>");

            if (isAwake()) {

              if (isRunning()) {
                client.println("<br />");
                client.println("<H3 style='font-size:100%;'>NEATO Vacuum is CLEANING</H3>");
              }
              else {
                client.println("<br />");
                client.println("<H3 style='font-size:100%;'>NEATO Vacuum is ON but not cleaning</H3>");
              }
            }
            else {
              client.println("<H3 style='font-size:100%;'>NEATO Vacuum in SLEEP MODE</H3>");
            }
            if (wakingup && !isAwake()) {
              client.println("<br />");
              client.println("<H3 style='font-size:100%;'>Waking up NEATO Vacuum</H3>");
            }
            else if (isAwake() && !isRunning()) {
              client.println("<br />");
              client.println("<H3 style='font-size:100%;'>NEATO is AWAKE</H3>");
            }

            client.println("<br />");
            client.println("<br />");
            client.println("<br />");
            client.println("<strong>");
            client.println("<a href=\"/H\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:50px; position:center; top:530px; left:300px;'>   start  </button></a>");
            client.println("<br />");
            client.println("<a href=\"/L\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   stop   </button></a>");
            client.println("<br />");
            client.println("<a href=\"/W\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  wake up </button></a>");
            client.println("<br />");
            client.println("<a href=\"/boot\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  RESTART Neato </button></a>");

            client.println("<br />");
            client.println("<a href=\"/R\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   reset controller  </button></a>");
            client.println("<br />");
           
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

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          Alreadyrestarted = false;
          TurnOn();
        }
        if (currentLine.endsWith("GET /L")) {

          TurnOff();

        }
        if (currentLine.endsWith("GET /boot")) {
          restartRobot();

        }
        if (currentLine.endsWith("GET /W")) {
          wakingup = true;

          if (!isAwake()) {
            WakeUp();
          }

        }
        if (currentLine.endsWith("GET /R")) {

          client.println("<H1 style='font-size:100%;'>RESETING power controller</H1>");
          Reset();
          break;

        }

      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

