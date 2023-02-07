void webserver() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
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
            client.println("<H1 style='font-size:400%;'>MEBO AUXILIARY POWER Control</H1>");


       
            client.println("<hr>");
            client.println("<a href=\"/H\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  ON </button></a>");
            client.println("<br />");
            client.println("<a href=\"/L\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  OFF </button></a>");
            

            client.println("</hr>");
            client.println("<hr>");
            client.println("<br />");
            client.println("<a href=\"/RESET\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>  RESET </button></a>");



            client.println("</center>");
            client.println("</strong>");
            client.println("</BODY>");
            client.println("</HTML>");

            client.println("</hr>");
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;

          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // check to see if a command was sent
        if (currentLine.equals("GET /H")) {
          digitalWrite(PIN0, HIGH);
          digitalWrite(PIN2, HIGH);
          digitalWrite(PIN3, HIGH);
        }
        if (currentLine.equals("GET /L")) {
          digitalWrite(PIN0, LOW);
          digitalWrite(PIN2, LOW);
          digitalWrite(PIN3, LOW);
        }
        if (currentLine.equals("GET /RESET")) {
          Reset();
        }
        if (currentLine.equals("GET /AllowOTA")) {
          AllowOTA = true;
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
