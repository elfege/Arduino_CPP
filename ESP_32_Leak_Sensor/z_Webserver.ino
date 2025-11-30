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
            // refresh the page automatically every 1 sec
            // client.println("Refresh: 1");
            // client.println();

            // the content of the HTTP response follows the header:
            client.println("<HTML>");client.println("<BODY style='background-color:navy'>");client.println("<font style='color:green'>");client.println("<font style='font-family:electric toaster'>");
            client.println("<center>");client.println("<H1 style='font-size:200%;'>Leak Sensor Bathroom</H1>");client.println("<H4 style='font-size:100%;'>OTA update capable</H4>");
            client.println("<br />");client.println("<br />");
            if (isWet()) {client.println("<H4 style='font-size:300%;'>WATER DETECTED !!!</H4>");}
            else {client.println("<H4 style='font-size:300%;'>SENSOR DRY</H4>");}
            client.println("<br />");
            client.println("<a href=\"/R\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   reset controller  </button></a>");
            client.println("<br />");client.println("</center>");client.println("</strong>");client.println("</BODY>");client.println("</HTML>");
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

