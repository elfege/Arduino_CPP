


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
            client.println("Refresh: 5");
            client.println();

            // the content of the HTTP response follows the header:
            client.println("<HTML>");
            client.println("<BODY style='background-color:blue'>");
            client.println("<font style='color:yellow'>");
            client.println("<font style='font-family:electric toaster'>");
            client.println("<center>");
            client.println("<H1 style='font-size:400%;'>HUMIDITY SENSOR BEDROOM</H1>");

            client.println("");
            client.print("<H4 style='font-size:100%;'>Humidity: </H4>"); client.println(humidityLevel());


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

      }
      if (currentLine.endsWith("GET /init")) {
        Reset();
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }

}


